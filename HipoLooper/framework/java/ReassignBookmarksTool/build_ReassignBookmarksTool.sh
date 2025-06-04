#!/bin/bash

COLOR_START='\033[35m'
COLOR_END='\033[0m'

echo ""
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo "${COLOR_START}= Bookmarks reassign tool builder                                     =${COLOR_END}"
echo "${COLOR_START}=======================================================================${COLOR_END}"
echo ""

echo "${COLOR_START}Cleaning previous build artifacts...${COLOR_END}"
rm -f ReassignBookmarksTool.class ReassignBookmarksTool.jar
rm -rf lib
echo ""

echo "${COLOR_START}Downloading required libraries...${COLOR_END}"
echo ""

# PDFBox + dependencies
curl -O https://repo1.maven.org/maven2/org/apache/pdfbox/pdfbox/2.0.27/pdfbox-2.0.27.jar
curl -O https://repo1.maven.org/maven2/org/apache/pdfbox/fontbox/2.0.27/fontbox-2.0.27.jar

# Jackson JSON dependencies
curl -O https://repo1.maven.org/maven2/com/fasterxml/jackson/core/jackson-core/2.15.2/jackson-core-2.15.2.jar
curl -O https://repo1.maven.org/maven2/com/fasterxml/jackson/core/jackson-annotations/2.15.2/jackson-annotations-2.15.2.jar
curl -O https://repo1.maven.org/maven2/com/fasterxml/jackson/core/jackson-databind/2.15.2/jackson-databind-2.15.2.jar
curl -O https://repo1.maven.org/maven2/commons-logging/commons-logging/1.2/commons-logging-1.2.jar

echo ""
echo "${COLOR_START}Libraries downloaded.${COLOR_END}"


# Copy your Java source code (make sure the path is correct)
cp /Users/alon/Projects/utils/HipoLooper/framework/java/ReassignBookmarksTool/ReassignBookmarksTool.java .

mkdir -p lib
mv *.jar lib/

echo ""
echo "${COLOR_START}Compiling ReassignBookmarksTool.java...${COLOR_END}"
javac --release 11 -cp ".:lib/*" ReassignBookmarksTool.java
echo ""

if [ $? -ne 0 ]; then
    echo "${COLOR_START}Compilation failed.${COLOR_END}"
    exit 1
fi

echo "${COLOR_START}Building .jar file...${COLOR_END}"
jar --create --file ReassignBookmarksTool.jar --main-class ReassignBookmarksTool -C . ReassignBookmarksTool.class
echo ""

echo "${COLOR_START}Done. You can now use:${COLOR_END}"
echo "  java -cp .:lib/* -jar ReassignBookmarksTool.jar input.pdf bookmarks.json output.pdf"
echo ""
