#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include <iostream>
#include <string>
using namespace std;

// Player struct
struct TennisPlayer {
    string playerID;
    string playerName;
    int playerRanking;
    string playerStatus;
    int wins;  // Track win match count in round-robin stage
};

// Match struct to store match details
struct Match {
    string matchID;
    string player1ID;
    string player2ID;
    string stage;
    string winnerID;
    string status;
    string score;
};

// Array-Based Queue implementation for tournament management
class TournamentQueue {
private:
    TennisPlayer* players;
    int maxSize;
    int queueFront;
    int queueRear;
    int currentSize;
    string stageName;  // Refer to queue name
    int matchCounter;  // To generate unique match IDs

public:
    // Constructor and destructor
    TournamentQueue();
    TournamentQueue(string stageName, int maxSize);
    ~TournamentQueue();

    // Queue methods
    void enqueue(TennisPlayer player);
    TennisPlayer dequeue();
    bool isEmpty();
    bool isFull();
    int getSize();

    // File handlers
    void readPlayer(const string& filename);
    void writeMatch(const Match& match);
    void checkMatchFileExisting();

    // Player and tournament related
    void displayPlayers();
    string generateScore();
    void scheduleMatch();  // Updated to display scheduled matches
    void runQualifiers();  // 12 -> 6 players
    void runRoundRobin(); // 6 -> 4 players
    void runKnockout();   // 4 -> 2 players
    void runFinals();     // Final and 3rd place
    void displayFinalRankings();
};

#endif