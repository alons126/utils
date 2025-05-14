# reattach_screens.py

def extract_screen_names(filename):
    screen_names = []
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            # Typical screen -ls output line: "12345.ttys001.hostname"
            if '.' in line and ('(Detached)' in line or '(Attached)' in line):
                screen_id = line.split()[0]  # take the first field
                screen_names.append(screen_id)
    return screen_names

def generate_reattach_commands(screen_names):
    commands = [f"screen -r {screen}" for screen in screen_names]
    return commands

def main():
    input_file = 'screens.rtf'  # <-- change if needed
    # input_file = 'screens.txt'  # <-- change if needed

    screen_names = extract_screen_names(input_file)

    if not screen_names:
        print("No valid screens found.")
        return

    commands = generate_reattach_commands(screen_names)

    print("Generated reattach commands:")
    for cmd in commands:
        print(cmd)

if __name__ == "__main__":
    main()
