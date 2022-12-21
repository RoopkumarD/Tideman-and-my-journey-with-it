// Code doesn't work

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int switch1;
int winner[MAX];

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
int recursion(int array[], int loser, int len);
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
        winner[i] = 0;
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
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    int temp[candidate_count];

    for (int k = 0; k < candidate_count; k++)
    {
        temp[k] = k;
    }

    for (int m = 0; m < candidate_count; m++)
    {
        for (int x = 0; x < candidate_count; x++)
        {
            if (ranks[m] == temp[x])
            {
                temp[x] = -1;
            }
        }

        for (int y = 0; y < candidate_count; y++)
        {
            if (temp[y] != -1)
            {
                preferences[ranks[m]][y]++;
            }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    int index = 0;

    for (int u = 0; u < candidate_count - 1; u++)
    {
        for (int q = u + 1; q < candidate_count; q++)
        {
            if (preferences [u][q] < preferences [q][u])
            {
                pairs[index].winner = q;
                pairs[index].loser = u;
                index++;
                pair_count++;
            }
            else if (preferences [u][q] > preferences [q][u])
            {
                pairs[index].winner = u;
                pairs[index].loser = q;
                index++;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for (int a = 0; a < pair_count - 1; a++)
    {
        for (int d = 0; d < pair_count - 1; d++)
        {
            if (preferences[pairs[d].winner][pairs[d].loser] < preferences[pairs[d + 1].winner][pairs[d + 1].loser])
            {
                pair t = pairs[d];
                pairs[d] = pairs[d + 1];
                pairs[d + 1] = t;
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    int can[candidate_count];

    for (int k = 0; k < pair_count; k++)
    {
        for (int u = 0; u < candidate_count; u++)
        {
            can[u] = u;
        }

        locked[pairs[k].winner][pairs[k].loser] = true;

        for (int m = 0; m < candidate_count; m++)
        {
            if (m == pairs[k].loser || m == pairs[k].winner)
            {
                can[m] = -1;
            }
        }

        int array[candidate_count - 1];

        array[0] = pairs[k].winner;

        for (int x = 1; x < candidate_count - 1; x++)
        {
            for (int z = 0; z < candidate_count; z++)
            {
                if (can[z] != -1)
                {
                    array[x] = can[z];
                    can[z] = -1;
                    break;
                }
            }
        }

        int len = sizeof(array) / sizeof(int);

        int r = recursion(array, pairs[k].loser, len); // input is correct

        if (r == 1)
        {
            switch1 = 0;
            locked[pairs[k].winner][pairs[k].loser] = false;
            pairs[k].winner = pairs[k].loser = -1;
        }

        if (locked[pairs[k].winner][pairs[k].loser] == true)
        {
            winner[pairs[k].winner]++;
            winner[pairs[k].loser]--;
        }
    }

    return;
}

int recursion(int array[], int loser, int len)
{
    int temp[len];

    for (int i = 0; i < len; i++)
    {
        for (int h = 0; h < len; h++)
        {
            temp[h] = array[h];
        }

        if (locked[loser][temp[i]] == 1)
        {
            if (i == 0)
            {
                switch1 = 1;
                return 1;
            }

            int new_array[len - 1];
            new_array[0] = temp[0];
            temp[0] = -1;

            for (int r = 0; r < len; r++)
            {
                if (temp[r] == temp[i])
                {
                    temp[r] = -1;
                }
            }

            for (int a = 1; a < len; a++)
            {
                for (int b = 0; b < len; b++)
                {
                    if (temp[b] != -1)
                    {
                        new_array[a] = temp[b];
                        temp[b] = -1;
                        break;
                    }
                }
            }

            int length = sizeof(new_array) / sizeof(int);

            recursion(new_array, array[i], length);

            if (switch1 == 1)
            {
                return 1;
            }
        }
    }

    return 0;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int winner_index = 0;

    for (int a = 1; a < candidate_count; a++)
    {
        if (winner[winner_index] < winner[a])
        {
            winner_index = a;
        }
    }

    printf("%s\n", candidates[winner_index]);

    return;
}

// Print the winner of the election
// void print_winner(void)
// {
//     // TODO
//     int* win = malloc(1 * sizeof(int));

//     if (win == NULL)
//     {
//         return;
//     }

//     int winner_points = 0;

//     for (int w = 0; w < candidate_count; w++)
//     {
//         if (winner_points < winner[w])
//         {
//             winner_points = winner[w];
//         }
//     }

//     int index = -1;

//     for (int s = 0; s < candidate_count; s++)
//     {
//         if (winner_points == winner[s])
//         {
//             index++;

//             win = realloc(win, (index + 1) * sizeof(int));

//             if (win == NULL)
//             {
//                 free(win);
//                 return;
//             }

//             win[index] = s;
//         }
//     }

//     int lengt = index + 1;

//     while (lengt != 1)
//     {
//         win = choose_winner(win, &lengt);

//         if (win == NULL)
//         {
//             return;
//         }

//     }

//     printf("%s\n", candidates[win[0]]);

//     free(win);

//     return;
// }

// int* choose_winner(int* war, int *x)
// {
//     int len = *x;
//     *x = -1;

//     int* temp = malloc(1 * sizeof(int));

//     if (temp == NULL)
//     {
//         free(war);
//         return temp;
//     }

//     for (int a = 0; a < len - 1; a++)
//     {
//         for (int b = a + 1; b < len; b++)
//         {
//             for (int c = 0; c < pair_count; c++)
//             {
//                 if (pairs[c].winner == war[a] && pairs[c].loser == war[b])
//                 {
//                     *x = *x + 1;

//                     temp = realloc(temp, (*x + 1) * sizeof(int));

//                     if (temp == NULL)
//                     {
//                         free(war);
//                         free(temp);
//                         return NULL;
//                     }

//                     temp[*x] = war[a];
//                 }
//                 else if (pairs[c].winner == war[b] && pairs[c].loser == war[a])
//                 {
//                     *x = *x + 1;

//                     temp = realloc(temp, (*x + 1) * sizeof(int));

//                     if (temp == NULL)
//                     {
//                         free(war);
//                         free(temp);
//                         return NULL;
//                     }

//                     temp[*x] = war[b];
//                 }
//             }
//         }
//     }

//     *x = *x + 1;

//     free(war);
//     return temp;
// }
