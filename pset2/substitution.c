#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";
const int KEY_LENGTH = 26;

int key_is_invalid(string key);
int key_has_duplicates(string key);
void translate_message(string message, int cipher_key[]);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    string key = argv[1];

    // validate
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.");
        return 1;
    }
    else if (key_is_invalid(key) || key_has_duplicates(key))
    {
        return 1;
    }

    // calculate an array of deltas for each letter in the key
    int cipher_key[KEY_LENGTH];
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        cipher_key[i] = tolower(argv[1][i]) - ALPHABET[i];
    }

    string message = get_string("plaintext:  ");
    translate_message(message, cipher_key);

    return 0;
}

int key_is_invalid(string key)
{
    int is_invalid = 0;

    for (int i = 0, length = strlen(key); i < length; i++)
    {
        if (tolower(key[i]) < 'a' || tolower(key[i]) > 'z')
        {
            is_invalid = 1;
            break;
        }
        printf("\n");
    }

    return is_invalid;
}

int key_has_duplicates(string key)
{
    int has_duplicates = 0;

    for (int i = 0, length = strlen(key); i < length; i++)
    {
        for (int j = i + 1; j < length; j++)
        {
            if (key[i] == key[j])
            {
                has_duplicates = 1;
                break;
            }
        }
    }

    return has_duplicates;
}

void translate_message(string message, int cipher_key[])
{
    printf("ciphertext: ");

    for (int i = 0, length = strlen(message); i < length; i++)
    {
        char character = message[i];
        char result;

        // only translate letters
        if (tolower(character) >= 'a' && tolower(character <= 'z'))
        {
            int index = (int) tolower(character) - 'a';
            result = character + cipher_key[index];
        }
        else
        {
            result = character;
        }

        printf("%c", result);
    }

    printf("\n");
}
