Ranked Voting System: Tideman Algorithm
This C++ program implements the Tideman voting algorithm, a ranked-choice voting system that ensures the election of a Condorcet winner (a candidate preferred over every other candidate in head-to-head comparisons) if one exists.
Features
Ranked Voting: Allows voters to rank candidates in order of preference.
Pairwise Comparisons: Records pairwise preferences between candidates based on voter rankings.
Sorting by Strength of Victory: Sorts candidate pairs in decreasing order of victory strength.
Cycle Detection: Locks pairs into a directed graph while avoiding cycles to preserve a valid ranking.
Winner Declaration: Identifies the source of the graph as the election winner.
