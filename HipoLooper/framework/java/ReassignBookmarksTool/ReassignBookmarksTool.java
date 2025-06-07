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

    /**
     * Entry point for the ReassignBookmarksTool utility.
     *
     * This method parses command-line arguments to determine the operation mode.
     * Supported operations:
     * - "extract": Extracts bookmarks from a PDF and writes them to a JSON file.
     * - "reassign": Reassigns bookmarks from a JSON file to a PDF, optionally in hierarchical format.
     *
     * Arguments:
     *   extract input.pdf output.json
     *   reassign input.pdf bookmarks.json output.pdf [hierarchical]
     *
     * @param args Command-line arguments specifying the operation and its parameters.
     * @throws Exception If an error occurs during file I/O or processing.
     */
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

            // Delete bookmark JSON after reassignment, with canonical path check
            try {
                File tempJson = new File(new File(inputPDF).getParent(), "bookmarks.json");
                File actualJson = new File(bookmarkJSON);

                if (tempJson.getCanonicalPath().equals(actualJson.getCanonicalPath()) && actualJson.exists()) {
                    if (actualJson.delete()) {
                        System.out.println(GREEN + "Temporary bookmark file deleted: " + RESET + actualJson.getAbsolutePath());
                    } else {
                        System.out.println(RED + "Failed to delete temporary bookmark file: " + RESET + actualJson.getAbsolutePath());
                    }
                }
            } catch (IOException e) {
                System.out.println(RED + "Error verifying or deleting bookmark JSON: " + RESET + e.getMessage());
            }

            System.out.println(GREEN + "\nBookmark reassignment completed!" + RESET);
            System.out.println(GREEN + "Reassigned PDF file saved to: " + RESET + outputPDF + "\n");
        }
    }

    /**
     * Extracts bookmarks from a PDF file and writes them to a JSON file.
     *
     * This method loads the input PDF file, retrieves its outline/bookmarks (if any),
     * and stores a structured representation of those bookmarks in a JSON file.
     * Bookmarks with titles starting with "Page " are ignored.
     *
     * @param inputPDF     Path to the input PDF file from which bookmarks will be extracted.
     * @param outputJSON   Path to the output JSON file where the extracted bookmarks will be saved.
     * @throws IOException If reading the PDF or writing the JSON file fails.
     */
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

    /**
     * Recursively extracts bookmark entries from a PDF outline node.
     *
     * This helper method walks through the outline tree structure of a PDF,
     * filters out any bookmarks whose titles start with "Page ",
     * and populates a list with bookmark data including hierarchy if applicable.
     *
     * @param node        The current PDF outline node.
     * @param parentTitle The title of the parent bookmark, or null if at root level.
     * @param list        The list to populate with extracted BookmarkEntry instances.
     * @param doc         The PDF document being processed.
     * @throws IOException If accessing the PDF structure fails.
     */
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

    /**
     * Reassigns bookmarks to a PDF file based on a JSON definition.
     *
     * This method removes any existing bookmarks from the input PDF, then
     * adds new bookmarks as defined in a JSON file. Bookmarks can be added
     * hierarchically if specified.
     *
     * @param inputPDF     Path to the PDF file to modify.
     * @param bookmarkJSON Path to the JSON file containing bookmark definitions.
     * @param outputPDF    Path where the modified PDF will be saved.
     * @param hierarchical If true, organizes bookmarks hierarchically based on parent relationships.
     * @throws IOException If reading the PDF or JSON, or writing the new PDF fails.
     */
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
        // Delete the bookmark JSON file if it exists
        File jsonFile = new File(bookmarkJSON);
        if (jsonFile.exists()) {
            if (jsonFile.delete()) {
                System.out.println(GREEN + "Temporary bookmark file deleted: " + RESET + jsonFile.getAbsolutePath());
            } else {
                System.out.println(RED + "Failed to delete temporary bookmark file: " + RESET + jsonFile.getAbsolutePath());
            }
        } else {
            System.out.println(CYAN + "Bookmark JSON not deleted: file not found." + RESET);
        }
    }
}