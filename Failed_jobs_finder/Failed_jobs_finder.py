import os


def find_unmatched_scripts(output_txt):
    PrinOut = False

    dir = os.getenv("PyDir")
    list_file = os.getenv("List_File")
    # os.system("ls " + dir + "master-routine_validation_01-eScattering >" + dir + list_file)
    # os.system("ls -1 . >" + list_file)

    # Obtain prefix
    with open(dir + "/grid_submission.xml", 'r') as xml_file:
        for xml_line in xml_file:
            if "<parallel>" not in xml_line:
                prefix_index = xml_line.rfind('/')
                prefix = xml_line[:prefix_index] + "/"
                break

    xml_file.close()

    # Initialize the lists
    gst_root_files = []
    sh_files = []

    # Read the input .txt file
    with open(dir + "/" + list_file, 'r') as file:
        # with open(input_txt, 'r') as file:
        for line in file:
            line = line.strip()
            if line.endswith('.gst.root'):
                gst_root_files.append(line)
            elif line.endswith('.sh'):
                sh_files.append(line)

    print("Number of '.gst.root' files: " + str(len(gst_root_files)))
    print("Number of '.sh' files: " + str(len(sh_files)))

    print("\n")

    # List to store unmatched .sh files
    unmatched_sh_files = []

    # Check for unmatched .sh files
    for sh_file in sh_files:
        matched = False
        sh_file_base_name = os.path.splitext(sh_file)[0]

        if PrinOut:
            print(sh_file_base_name)

        for gst_file in gst_root_files:
            gst_file_base_name0 = os.path.splitext(gst_file)[0]
            gst_file_base_name = os.path.splitext(gst_file_base_name0)[0]

            if PrinOut:
                print(gst_file_base_name)

            if sh_file_base_name == gst_file_base_name:
                matched = True
                break
        if not matched:
            unmatched_sh_files.append(sh_file)

    # Write unmatched .sh files to the output .txt file
    with open(output_txt, 'w') as file:
        file.write('<parallel>\n')
        for sh_file in unmatched_sh_files:
            file.write(prefix + sh_file + '\n')
        file.write('</parallel>\n')

    os.system("cp " + output_txt + " " + dir + "/" + output_txt)

    print("Expected Number of '.gst.root' and '.sh' files: " + str(len(sh_files) * 2))
    print("Number of unmatched '.sh' files: " + str(len(unmatched_sh_files)))


if __name__ == "__main__":
    input_txt = "input.txt"  # List of files in master-routine_validation_01-eScattering
    output_txt = "grid_submission.xml"  # output .xml file
    find_unmatched_scripts(output_txt)
