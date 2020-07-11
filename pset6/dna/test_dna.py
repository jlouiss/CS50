from sys import argv, exit
from os import path
import csv
import subprocess

"""
Usage: python test_dna.py <path_to_csv>.csv
"""


def main():
    if len(argv) != 2:
        print("Usage: python test_dna.py <path_to_csv>.csv")
        exit(1)

    if not path.isfile(argv[1]):
        print("Invalid file")
        exit(1)

    with open(argv[1]) as csv_file:
        csv_reader = csv.reader(csv_file)
        for (database, sequence, expected_result) in csv_reader:
            # print(f"running: python dna.py {database} {sequence}")

            # read output from another python script
            # https://stackoverflow.com/questions/6086047/get-output-of-python-script-from-within-python-script
            process = subprocess.Popen(
                ['python3', 'dna.py', database, sequence],
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT)
            result = process.communicate()[0]

            # print(f"result: {result.decode().rstrip()}")
            # print(result == "No match")
            # print(result.decode().rstrip() == "No match")

            print("PASS" if result.decode().rstrip() == expected_result else "FAIL", end="\n\n")

    exit(0)


main()