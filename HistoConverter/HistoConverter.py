import re

def find_str(s, char):
    index = 0

    if char in s:
        c = char[0]
        for ch in s:
            if ch == c:
                if s[index:index+len(char)] == char:
                    return index

            index += 1

    return -1

def HistoConverter(input_file, output_decl_file, output_def_file):
    with open(input_file, 'r') as infile, \
         open(output_decl_file, 'w') as decl_file, \
         open(output_def_file, 'w') as def_file:

        line_number = 0

        for line in infile:
            line_number = line_number + 1
            declaration0 = ""
            declaration = ""
            HistName = ""
            definition = ""

            # if (line.find('// ') != -1 or line.find('#pragma') != -1 or line.find('/*') != -1):
            #     decl_file.write(line)
            #     def_file.write(line)
            # elif (line == "\n" or line == "\n\n"):
            #     decl_file.write(line)
            #     def_file.write(line)
            # elif line.find("HistoList.push_back") != 1:
            if "HistoList.push_back" not in line:
                if (("TH1D *" in line) or ("TH2D *" in line)):

                    if ("[7]" not in line):
                    # if (line.find("[") == -1):
                        if "TH2D *" in line:
                            print(line)

                        if (line.find(" = ") != -1):
                            # print(line[:find_str(line, " = ")])
                            declaration0 = line[:find_str(line, " = ")]
                            declaration = declaration0+";\n"
                            decl_file.write(declaration)

                            # print(declaration)

                            if (line.find("TH1D *") != -1):
                                HistName = declaration0[find_str(line, "*")+1:]
                            elif (line.find("TH2D *") != -1):
                                HistName = declaration0[find_str(line, "*")+1:]

                            definition = line[find_str(line, " = "):]
                            definition = definition.replace("  ", "")
                            def_file.write(HistName  + definition)
                            def_file.write("HistoList.push_back(" + HistName + ");\n")
                    else:
                        decl_file.write(line)
                        def_file.write(line)
                else:
                    decl_file.write(line)
                    def_file.write(line)

            # print(line)
                            # print(definition)
                    # elif (line.find("[") != -1 and line.find("]") != -1):
                    #     declaration = line[:find_str(line, "[")]+"[7];\n"
                    #     decl_file.write(declaration)
                    # else:
                    #     decl_file.write("\n")
                    #     def_file.write("\n")

            if line_number == 59:
                print("line 59:")
                print(line)

        # for line in infile:
        #     # Match the histogram declaration and definition pattern
        #     match = re.match(
        #         r'\s*(TH1[DF]|TH2[DF])\s*\*\s*(\w+)\s*=\s*new\s*(TH1[DF]|TH2[DF])\(([^;]+)\);',
        #         line
        #     )
        #     if match:
        #         # Extract parts: type, name, and constructor arguments
        #         hist_type = match.group(1)  # e.g., TH1D or TH2D
        #         hist_name = match.group(2)  # e.g., h_n_multiplicity_badN_epFDn_Step3
        #         constructor = match.group(3) + "(" + match.group(4) + ")"  # Full constructor
        #
        #         # Prepare declaration and definition
        #         declaration = f"{hist_type} *{hist_name};\n"
        #         definition = f"{hist_name} = new {constructor};\n"
        #         push_back = f"    HistoList.push_back({hist_name});\n"
        #
        #         # Write to files
        #         decl_file.write(declaration)
        #         def_file.write(definition)
        #         def_file.write(push_back)
        #     else:
        #         # Write non-matching lines as-is to the definition file
        #         def_file.write(line)

if __name__ == "__main__":
    input_header_file = "/Users/alon/Projects/Neutron_reconstruction_improvements/src/Histograms/ManualVetoHistograms.h"  # Replace with the path to your input .h file
    # input_header_file = "input.h"  # Replace with the path to your input .h file
    output_declaration_file = "declarations.h"
    output_definition_file = "definitions.h"

    HistoConverter(input_header_file, output_declaration_file, output_definition_file)
    print("Processing complete. Check 'declarations.h' and 'definitions.h' for results.")

