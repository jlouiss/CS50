"""
Usage: python dna.py <dna_database>.csv <dna_sequence>.txt

Given a database and a sequence, match the DNA sequence against the database.
"""
from sys import argv, exit
import csv
import re


dna_sequence = ""
dna_database = []
str_list = []
dna_sequence_str_count = {} # a dictionary of {STR: number_of_repetitions}

"""
Load the csv file in memory and extract the STRs from the csv header
"""
def parse_database():
    global str_list

    with open(argv[1]) as csv_file:
        reader = csv.reader(csv_file)
        # extract STRs from the header (e.g.: name, STR1, STR2, STR3, ...)
        str_list = next(reader)[1:]
        for line in reader:
            dna_database.append(line)


def parse_dna_sequence():
    global dna_sequence

    with open(argv[2]) as dna_sequence_file:
        dna_sequence = dna_sequence_file.readline()

    for str in str_list:
        matches = re.findall(r'(?:' + str + ')+', dna_sequence)
        if not matches:
            dna_sequence_str_count[str] = 0
            continue

        largest_match = max(matches, key = len)
        dna_sequence_str_count[str] = largest_match.count(str)


def main():
    if len(argv) != 3:
        print("Usage: python dna.py <dna_db_file> <dna_sequence_file>")
        exit(1)

    match = None
    parse_database()
    parse_dna_sequence()

    for row in dna_database:
        row_match = True

        for index, str_count in enumerate(row[1:]):
            if (dna_sequence_str_count[str_list[index]] != int(str_count)):
                row_match = False
                break

        if row_match:
            match = row[0]
            break

    if not match:
        print("No match")
        exit(1)

    print(match)
    exit(0)

main()