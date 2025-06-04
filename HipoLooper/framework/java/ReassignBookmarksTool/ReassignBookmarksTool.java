// package HipoLooper.framework.java.ReassignBookmarksTool;
import com.fasterxml.jackson.databind.*;
import java.io.*;
import java.util.*;
import org.apache.pdfbox.pdmodel.PDDocument;
import org.apache.pdfbox.pdmodel.common.PDPageLabels;
import org.apache.pdfbox.pdmodel.interactive.documentnavigation.outline.*;

public class ReassignBookmarksTool {
    // ANSI color codes for output
    public static final String RESET = "\u001B[0m";
    public static final String RED = "\u001B[31m"; // Errors
    public static final String GREEN = "\u001B[32m"; // Success/info
    public static final String CYAN = "\u001B[36m"; // Warnings (changed to cyan)

    static class BookmarkEntry {
        public String title;
        public int page;
        public String parent; // optional
    }

    public static void main(String[] args) throws Exception {
        System.out.println(GREEN + "\nArguments received:" + RESET);

        for (int i = 0; i < args.length; i++) { System.out.println(GREEN + "  arg[" + i + "]: " + RESET + args[i]); }

        if (args.length < 2) {
            System.out.println(CYAN + "Usage:" + RESET);
            System.out.println(GREEN + "  java -jar ReassignBookmarksTool.jar input.pdf bookmarks.json output.pdf" + RESET);
            System.out.println(GREEN + "  java -jar ReassignBookmarksTool.jar extract input.pdf output.json" + RESET);
            System.out.println(GREEN + "  java -jar ReassignBookmarksTool.jar reassign input.pdf bookmarks.json output.pdf [hierarchical]" + RESET);
            return;
        }

        if (args[0].equals("extract")) {
            String inputPDF = args[1];
            File inputFile = new File(inputPDF);
            String outputJSON = new File(inputFile.getParent(), "bookmarks.json").getAbsolutePath();
            extractBookmarks(inputPDF, outputJSON);

            System.out.println(GREEN + "\nBookmark extraction completed!" + RESET);
            System.out.println(GREEN + "Extracted bookmarks saved to: " + RESET + outputJSON + "\n");
        } else if (args[0].equals("reassign")) {
            boolean hierarchical = args.length > 4 && args[4].equalsIgnoreCase("hierarchical");
            String inputPDF = args[1];
            String bookmarkJSON = args[2];
            String outputPDF = args[3];
            reassignBookmarks(inputPDF, bookmarkJSON, outputPDF, hierarchical);

            // Delete bookmark JSON after reassignment if it was created in input directory
            File tempJson = new File(new File(inputPDF).getParent(), "bookmarks.json");
            if (tempJson.exists() && tempJson.getAbsolutePath().equals(new File(bookmarkJSON).getAbsolutePath())) {
                if (tempJson.delete()) { System.out.println(GREEN + "Temporary bookmark file deleted: " + RESET + tempJson.getAbsolutePath()); }
            }

            System.out.println(GREEN + "\nBookmark reassignment completed!" + RESET);
            System.out.println(GREEN + "Reassigned PDF file saved to: " + RESET + outputPDF + "\n");
        }
    }

    public static void extractBookmarks(String inputPDF, String outputJSON) throws IOException {
        try (PDDocument document = PDDocument.load(new File(inputPDF))) {
            PDDocumentOutline outline = document.getDocumentCatalog().getDocumentOutline();
            if (outline == null) {
                System.out.println(RED + "No bookmarks found." + RESET);
                return;
            }

            List<BookmarkEntry> bookmarks = new ArrayList<>();
            extractOutlineItems(outline, null, bookmarks, document);

            ObjectMapper mapper = new ObjectMapper();
            mapper.writerWithDefaultPrettyPrinter().writeValue(new File(outputJSON), bookmarks);
        }
    }

    private static void extractOutlineItems(PDOutlineNode node, String parentTitle, List<BookmarkEntry> list, PDDocument doc) throws IOException {
        PDOutlineItem current = node.getFirstChild();
        while (current != null) {
            String title = current.getTitle();
            if (!title.startsWith("Page ")) {
                BookmarkEntry entry = new BookmarkEntry();
                entry.title = title;
                entry.page = doc.getPages().indexOf(current.findDestinationPage(doc)) + 1;
                if (parentTitle != null) entry.parent = parentTitle;
                list.add(entry);

                if (current.hasChildren()) { extractOutlineItems(current, title, list, doc); }
            }
            current = current.getNextSibling();
        }
    }

    public static void reassignBookmarks(String inputPDF, String bookmarkJSON, String outputPDF, boolean hierarchical) throws IOException {
        try (PDDocument document = PDDocument.load(new File(inputPDF))) {
            // Remove old bookmarks
            document.getDocumentCatalog().setDocumentOutline(null);

            // Read new bookmarks
            ObjectMapper mapper = new ObjectMapper();
            BookmarkEntry[] entries = mapper.readValue(new File(bookmarkJSON), BookmarkEntry[].class);

            Map<String, PDOutlineItem> titleMap = new HashMap<>();
            PDDocumentOutline outline = new PDDocumentOutline();
            document.getDocumentCatalog().setDocumentOutline(outline);

            for (BookmarkEntry entry : entries) {
                PDOutlineItem item = new PDOutlineItem();
                item.setTitle(entry.title);
                item.setDestination(document.getPage(entry.page - 1));

                if (hierarchical && entry.parent != null && titleMap.containsKey(entry.parent)) {
                    titleMap.get(entry.parent).addLast(item);
                } else {
                    outline.addLast(item);
                }

                titleMap.put(entry.title, item);
            }

            outline.openNode();
            document.save(outputPDF);
        }
    }
}