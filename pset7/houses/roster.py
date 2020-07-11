from sys import argv, exit
import cs50


def main():
    if (len(argv) != 2):
        exit(1)

    db = cs50.SQL("sqlite:///students.db")
    result = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

    for student in result:
        first = student["first"]
        middle = student["middle"] if student["middle"] != "NULL" else ""
        last = student["last"]
        birth = student["birth"]
        print(first, end="")
        if middle:
            print(f" {middle}", end="")
        print(f" {last}, born {birth}")

    exit(0)


main()