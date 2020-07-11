from sys import exit


"""
Coleman-Liau index
The formula for this is 0.0588 * L - 0.296 * S - 15.8
where
L is the average number of letters per 100 words in the text, and
S is the average number of sentences per 100 words in the text
"""


def coleman_liau_index(l, s):
    return 0.0588 * l - 0.296 * s - 15.8


def main():
    string = input("Text: ")
    letters_count = 0
    word_count = 1
    sentence_count = 0

    for char in string:
        if (char >= "a" and char <= "z") or (char >= "A" and char <= "Z"):
            letters_count += 1
        elif char == " ":
            word_count += 1
        elif char == "." or char == "!" or char == "?":
            sentence_count += 1

    average_letters_per_hundred_words = letters_count / word_count * 100.0
    average_sentences_per_hundred_words = sentence_count / word_count * 100.0
    grade = coleman_liau_index(
        average_letters_per_hundred_words, average_sentences_per_hundred_words)

    if grade < 16 and grade >= 0:
        print(f"Grade {round(grade)}")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print("Before Grade 1")

    exit(0)


main()