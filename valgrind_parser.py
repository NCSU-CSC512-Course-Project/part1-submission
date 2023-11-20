# Python script to parse the output of a valgrind log file,
# to collect the total number of instructions that were exectued for the given program.
import sys

if __name__ == "__main__":
    # Get args
    args = sys.argv

    if len(args) < 2:
        raise ValueError("No file name to parse!")

    # Get filename
    filename = args[1]

    # Open file for parsing
    with open(filename, "r") as valgrind_file:
        if valgrind_file.closed:
            raise FileNotFoundError("Error opening Valgrind file for parsing!")
        lines = valgrind_file.readlines()
        for line in lines:
            words = line.split()
            if "Collected" in words:
                instruction_count = words[-1]
                print(
                    f"The total number of executed instructions for the program was: {instruction_count}"
                )
        valgrind_file.close()
