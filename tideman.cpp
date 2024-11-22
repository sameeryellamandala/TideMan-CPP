#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
struct Pair {
    int winner;
    int loser;
};

// Array of candidates
string candidates[MAX];
Pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main()
{
    // Get the number of candidates
    cout << "Enter the number of candidates: ";
    cin >> candidate_count;

    // Validate candidate count
    if (candidate_count > MAX) {
        cout << "Maximum number of candidates is " << MAX << endl;
        return 1;
    }

    // Get the candidates' names
    cout << "Enter the names of the candidates: \n";
    for (int i = 0; i < candidate_count; i++) {
        cout << "Candidate " << i + 1 << ": ";
        cin >> candidates[i];
    }

    // Clear graph of locked-in pairs
    for (int i = 0; i < candidate_count; i++) {
        for (int j = 0; j < candidate_count; j++) {
            locked[i][j] = false;
        }
    }

    pair_count = 0;

    // Get the number of voters
    int voter_count;
    cout << "Enter the number of voters: ";
    cin >> voter_count;

    // Query for votes
    for (int i = 0; i < voter_count; i++) {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        cout << "Voter " << i + 1 << ":\n";
        for (int j = 0; j < candidate_count; j++) {
            string name;
            cout << "Rank " << j + 1 << ": ";
            cin >> name;

            if (!vote(j, name, ranks)) {
                cout << "Invalid vote.\n";
                return 3;
            }
        }

        record_preferences(ranks);
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();

    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[]) {
    // Check if the candidate exists
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i] == name) {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) {
    // Update preferences for each pair of candidates
    for (int i = 0; i < candidate_count; i++) {
        for (int j = i + 1; j < candidate_count; j++) {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void) {
    for (int i = 0; i < candidate_count; i++) {
        for (int j = 0; j < candidate_count; j++) {
            if (i != j) {
                if (preferences[i][j] > preferences[j][i]) {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }
                else if (preferences[j][i] > preferences[i][j]) {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                    pair_count++;
                }
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void) {
    // Sort pairs based on the difference of preferences
    sort(pairs, pairs + pair_count, [](Pair a, Pair b) {
        int strength_a = preferences[a.winner][a.loser] - preferences[a.loser][a.winner];
        int strength_b = preferences[b.winner][b.loser] - preferences[b.loser][b.winner];
        return strength_a > strength_b;
    });
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void) {
    for (int i = 0; i < pair_count; i++) {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // Lock the pair
        locked[winner][loser] = true;

        // Check for cycles
        bool cycle = false;
        for (int j = 0; j < candidate_count; j++) {
            if (locked[loser][j]) // if there's a locked cycle involving loser
            {
                cycle = true;
                break;
            }
        }

        // If there's a cycle, undo the lock
        if (cycle) {
            locked[winner][loser] = false;
        }
    }
}

// Print the winner of the election
void print_winner(void) {
    for (int i = 0; i < candidate_count; i++) {
        bool is_winner = true;
        for (int j = 0; j < candidate_count; j++) {
            if (locked[j][i]) // if any candidate is locked over this candidate
            {
                is_winner = false;
                break;
            }
        }

        if (is_winner) {
            cout << "The winner is: " << candidates[i] << endl;
            return;
        }
    }
}
