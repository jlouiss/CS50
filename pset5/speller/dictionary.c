// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 500;

// Hash table
node *table[N];

// Keep track of generated hashes to free up the memory afterwards
int hashes[N];
int total_hash_count = 0;
int total_word_count = 0;

// Returns true if word is in dictionary
bool check(const char *word)
{
    int word_hash = hash(word);
    node *cursor = table[word_hash];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
// source: https://www.reddit.com/r/cs50/comments/eo4zro/good_hash_function_for_speller/
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    int c = *word;
    c = tolower(c);

    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c;
        c = *word++;
        c = tolower(c);
    }

    return hash % N;
}

bool add_word_to_table(char *word)
{
    node *word_node = malloc(sizeof(node));
    if (!word_node)
    {
        return false;
    }

    int node_hash = hash(word);
    strcpy(word_node->word, word);

    // push node at the beginning of the chain
    word_node->next = table[node_hash];
    table[node_hash] = word_node;

    return true;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        total_word_count++;

        if (!add_word_to_table(word))
        {
            return false;
        }
    }

    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return total_word_count;
}

bool free_node(node *word_node)
{
    node *cursor = word_node;
    node *tmp = NULL;

    while (!cursor)
    {
        tmp = cursor;
        cursor = cursor->next;
        free(tmp);
    }

    return !cursor->next && !tmp->next;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < total_hash_count; i++)
    {
        if (!free_node(table[hashes[i]]))
        {
            return false;
        }
    }

    return true;
}
