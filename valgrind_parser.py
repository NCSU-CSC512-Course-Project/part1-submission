# Python script to parse the output of a valgrind log file,
# to collect the total number of instructions that were exectued for the given program.
import sys

if __name__ == "__main__":

    # Get file name 
    args = sys.argv

    if len(args) < 2:
        raise ValueError("No file name to parse!")

    filename = args[1]

    print(filename)

    
