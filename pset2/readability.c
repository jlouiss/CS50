#include <stdio.h>
#include <cs50.h>

int main(int argc, string argv[])
{
    string text = get_string("Text: ");
    int letters_count = 0;
    int word_count = 1;
    int sentence_count = 0;

    // count words
    for (int i = 0; i < strlen(text); i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letters_count++;
        }
        else if (text[i] == ' ')
        {
            word_count++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count++;
        }
    }

    // calculate grade
    float grade = 0.0588 * (100 * letters_count / (float) word_count) - 0.296 * (100 * sentence_count / (float) word_count) - 15.8;
    if (grade < 16 && grade >= 0)
    {
        printf("Grade %i\n", (int) round(grade));
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Before Grade 1\n");
    }
}
