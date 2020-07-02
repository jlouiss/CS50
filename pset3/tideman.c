#include <cs50.h>
#include <stdio.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    bool candidate_exists = false;

    for (int i = 0; i < candidate_count; i++)
    {
        if (name == candidates[i])
        {
            candidate_exists = true;
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i >= j)
            {
                continue;
            }

            // tricky
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j)
            {
                continue;
            }

            if (preferences[i][j] > preferences[j][i])
            {
                pair pair;
                pair.winner = i;
                pair.loser = j;
                pairs[pair_count] = pair;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count - i - 1; j++)
        {
            int strength_1 = preferences[pairs[j].winner][pairs[j].loser];
            int strength_2 = preferences[pairs[j + 1].winner][pairs[j + 1].loser];

            if (strength_1 < strength_2)
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
        }
    }

    return;
}

// loop recursively to detect any cycles in the locked array
bool is_cyclic(int start, int winner, int loser)
{
    if (locked[loser][start])
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (is_cyclic(start, loser, i))
            {
                return true;
            }
        }
    }

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        pair pair = pairs[i];

        if (is_cyclic(pair.winner, pair.winner, pair.loser))
        {
            continue;
        }
        else
        {
            locked[pair.winner][pair.loser] = true;
        }
    }

    return;
}

/**
 * The winner is the candidate that has no arrows pointing towards them
 * Meaning their column in locked[][] has false values only
 * e.g.:
 * locked = [
 *     [0, 1, 1],
 *     [0, 0, 0],
 *     [0, 1, 0]
 * ];
 *
 * winner is 0
 */
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_winner = true;

        for (int j = 0; j < candidate_count; j++)
        {
            // [j][i] -> loop through columns
            if (locked[j][i])
            {
                is_winner = false;
                break;
            }
        }

        if (is_winner)
        {
            printf("%s\n", candidates[i]);
        }
    }

    return;
}
