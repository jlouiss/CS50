from sys import argv, exit
import cs50
import csv


def main():
    if len(argv) != 2:
        exit(1)

    db = cs50.SQL("sqlite:///students.db")
    with open(argv[1]) as csv_file:
        reader = csv.DictReader(csv_file)

        for student in reader:
            name = student["name"].split(" ")
            first_name = name[0]
            middle_name = name[1] if len(name) == 3 else None
            last_name = name[-1]

            print(student["name"], student["house"])
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       first_name, middle_name, last_name, student["house"], student["birth"])

    exit(0)


main()