#include "Tournament.hpp"
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>  // for random
#include <ctime>    // random can depend on time

TournamentQueue::TournamentQueue()
{
    players = nullptr;
    maxSize = 0;
    queueFront = 0;
    queueRear = -1;
    currentSize = 0;
    stageName = "";
    matchCounter = 0;
}

TournamentQueue::TournamentQueue(string stageName, int maxSize)
{
    players = new TennisPlayer[maxSize];
    this->maxSize = maxSize;
    queueFront = 0;
    queueRear = -1;
    currentSize = 0;
    this->stageName = stageName;
    matchCounter = 0;
}

TournamentQueue::~TournamentQueue()
{
    if (players) {
        delete[] players;
        players = nullptr;
    }
    cout << endl << "--Tournament Queue " << stageName << " has been deleted!--" << endl;
}

// Queue methods
void TournamentQueue::enqueue(TennisPlayer player) {
    if (!isFull()) {
        queueRear = (queueRear + 1) % maxSize;
        players[queueRear] = player;
        currentSize++;
    }
    else {
        cout << "Queue is full" << endl;
    }
}

TennisPlayer TournamentQueue::dequeue()
{
    if (isEmpty()) {
        cout << "Queue is empty" << endl;
        return TennisPlayer{ "", "", 0, "active", 0 };
    }
    TennisPlayer player = players[queueFront];
    queueFront = (queueFront + 1) % maxSize;
    currentSize--;
    return player;
}

bool TournamentQueue::isEmpty()
{
    return currentSize == 0;
}

bool TournamentQueue::isFull()
{
    return currentSize == maxSize;
}

int TournamentQueue::getSize()
{
    return currentSize;
}

// File handlers
void TournamentQueue::readPlayer(const string& filename)
{
    ifstream file(filename);
    string line;
    while (getline(file, line) && !isFull()) {
        TennisPlayer player;
        size_t pos1 = line.find(" ");
        size_t pos2 = line.find(" ", pos1 + 1);
        size_t pos3 = line.find(" ", pos2 + 1);

        player.playerID = line.substr(0, pos1);
        player.playerName = line.substr(pos1 + 1, pos2 - pos1 - 1);
        player.playerStatus = line.substr(pos2 + 1, pos3 - pos2 - 1);
        player.playerRanking = stoi(line.substr(pos3 + 1));
        player.wins = 0;

        // Only enqueue players with "active" status
        if (player.playerStatus == "active") {
            enqueue(player);
        }
    }
    file.close();
}

void TournamentQueue::writeMatch(const Match& match)
{
    // For the first match, use ios::out to overwrite or create a new file
    // For subsequent matches, use ios::app to append
    ios_base::openmode mode = (matchCounter == 1) ? ios::out : ios::app;
    
    ofstream file("match.txt", mode);
    if (file.is_open()) {
        // Get player names using match player IDs
        string player1Name = "", player2Name = "", winnerName = "";
        string stage = (match.stage == "Round Robin") ? "Round-Robin" : match.stage;
        
        // Find player names from their IDs by searching through all possible players
        // This requires checking the player.txt file
        ifstream playerFile("data/player.txt");
        if (playerFile.is_open()) {
            string line;
            while (getline(playerFile, line)) {
                size_t pos1 = line.find(" ");
                string playerID = line.substr(0, pos1);
                size_t pos2 = line.find(" ", pos1 + 1);
                string playerName = line.substr(pos1 + 1, pos2 - pos1 - 1);
                
                if (playerID == match.player1ID) {
                    player1Name = playerName;
                }
                if (playerID == match.player2ID) {
                    player2Name = playerName;
                }
                if (playerID == match.winnerID) {
                    winnerName = playerName;
                }
                
                // If we've found all players, we can stop reading
                if (!player1Name.empty() && !player2Name.empty() && !winnerName.empty()) {
                    break;
                }
            }
            playerFile.close();
        }
        
        // Write match info including score
        file << match.matchID << " " << player1Name << " " << player2Name << " "
            << stage << " " << winnerName << " " << match.status << " "
            << match.score << "\n";
        file.close();
    }
    else {
        cout << "Error opening match.txt" << endl;
    }
}

void TournamentQueue::checkMatchFileExisting()
{
    // Check if file exists using ifstream
    std::ifstream fileCheck("data/match.txt");

    if (fileCheck.good()) {
        // File exists, close the ifstream before deleting
        fileCheck.close();

        // Delete the existing file
        if (std::remove("data/match.txt") != 0) {
            // Handle error if deletion fails
            std::cerr << "Error deleting existing match.txt file" << std::endl;
            return;
        }
    }

    // Create a new empty file
    std::ofstream newFile("data/match.txt");
    if (!newFile.is_open()) {
        // Handle error if file creation fails
        std::cerr << "Error creating new match.txt file" << std::endl;
        return;
    }

    // Close the new file
    newFile.close();
}

// Player and tournament related
void TournamentQueue::displayPlayers()
{
    cout << "\n" << stageName << " Registered Players:\n";
    for (int i = 0; i < currentSize; i++) {
        int index = (queueFront + i) % maxSize;
        cout << "Player ID: " << players[index].playerID << ", Name: " << players[index].playerName<< endl;
    }
}

string TournamentQueue::generateScore()
{
    string scoreSampleSet[10] = { "15 14","15 13","15 12","15 11","15 10","15 9","15 8","15 7","15 6", "15 5"};

    // Seed the random number generator with current time
    srand(static_cast<unsigned int>(time(0)));

    //first random index
    int scoreSetIndex = rand() % 10;

    // Output  scores
    return scoreSampleSet[scoreSetIndex];
}

void TournamentQueue::scheduleMatch()
{
    cout << "\nScheduling " << stageName << " Matches:\n";
    string prefix;
    matchCounter = 0; // Reset match counter at the beginning of each stage

    if (stageName == "Qualifiers") prefix = "Q";
    else if (stageName == "Round Robin") prefix = "RR";
    else if (stageName == "Knockout") prefix = "K";
    else if (stageName == "Finals") prefix = "F";
    else prefix = "M";  // Fallback

    if (stageName == "Round Robin") {
        // Create a temporary array to hold all players
        TennisPlayer tempPlayers[6];
        int tempCount = 0;

        // Copy all players from queue to temporary array
        for (int i = 0; i < currentSize; i++) {
            int index = (queueFront + i) % maxSize;
            tempPlayers[tempCount++] = players[index];
        }

        // Schedule round robin matches in proper rounds
        // For 6 players, we need 5 rounds, with 3 matches per round
        cout << "Round Robin Schedule (5 rounds):\n";

        // Using the "circle method" for round robin scheduling
        // Player at index 0 stays fixed, others rotate
        for (int round = 0; round < tempCount - 1; round++) {
            cout << "\nRound " << (round + 1) << ":\n";

            // First match of the round always involves player 0
            string matchID = prefix + to_string(++matchCounter);
            cout << "Match ID: " << matchID << " " << tempPlayers[0].playerName
                << " vs " << tempPlayers[round + 1].playerName << endl;

            // Other matches in this round
            for (int match = 1; match < tempCount / 2; match++) {
                int player1 = (round + match) % (tempCount - 1) + 1;
                int player2 = (round + tempCount - match) % (tempCount - 1) + 1;

                matchID = prefix + to_string(++matchCounter);
                cout << "Match ID: " << matchID << ", Players: " << tempPlayers[player1].playerName
                    << " vs " << tempPlayers[player2].playerName << endl;
            }
        }
    }
    else {
        // For other stages, just pair adjacent players
        for (int i = 0; i < currentSize - 1; i += 2) {
            int index1 = (queueFront + i) % maxSize;
            int index2 = (queueFront + i + 1) % maxSize;
            string matchID = prefix + to_string(++matchCounter);
            cout << "Match ID: " << matchID << ", Players: " << players[index1].playerName
                << " vs " << players[index2].playerName << endl;
        }
    }
}

void TournamentQueue::runQualifiers() {
    cout << "\nRunning Qualifiers (12 -> 6 players):\n";
    TournamentQueue nextStage("Round Robin", 6);

    // Store all players in a temporary array
    TennisPlayer allPlayers[12];
    int playerCount = 0;

    while (!isEmpty()) {
        allPlayers[playerCount++] = dequeue();
    }

    // Process qualifier matches - 6 matches total with 12 players
    for (int i = 0; i < playerCount; i += 2) {
        TennisPlayer p1 = allPlayers[i];
        TennisPlayer p2 = allPlayers[i + 1];

        // Determine winner based on player ranking (lower is better)
        TennisPlayer winner = (p1.playerRanking < p2.playerRanking) ? p1 : p2;

        // Create and record match
        Match match;
        match.matchID = "Q" + to_string((i / 2) + 1);
        match.player1ID = p1.playerID;
        match.player2ID = p2.playerID;
        match.stage = "Qualifiers";
        match.winnerID = winner.playerID;
        match.status = "Completed";
        match.score = generateScore();

        writeMatch(match);

        cout << "Match ID: " << match.matchID << ", Winner: " << winner.playerName << endl;

        // Add winner to next stage
        nextStage.enqueue(winner);
    }

    // Update this tournament object for the Round Robin stage
    delete[] players;
    maxSize = 6;
    players = new TennisPlayer[maxSize];
    queueFront = 0;
    queueRear = -1;
    currentSize = 0;
    stageName = "Round Robin";

    // Transfer players and display advancing players
    TennisPlayer advancing[6];
    int advCount = 0;
    while (!nextStage.isEmpty()) {
        advancing[advCount++] = nextStage.dequeue();
        enqueue(advancing[advCount - 1]);
    }

    cout << "\nPlayers Advancing to Round Robin Stage: ";
    for (int i = 0; i < advCount; i++) {
        cout << advancing[i].playerName;
        if (i < advCount - 1) cout << ", ";
        else cout << endl;
    }
}

void TournamentQueue::runRoundRobin() {
    cout << "\nRunning Round Robin (6 players):\n";

    // Create array to store players and track wins
    TennisPlayer tournamentPlayers[6];
    int playerCount = 0;

    // Transfer players from queue to array
    while (!isEmpty()) {
        tournamentPlayers[playerCount++] = dequeue();
    }

    // Reset all wins to 0
    for (int i = 0; i < playerCount; i++) {
        tournamentPlayers[i].wins = 0;
    }

    // Run all matches in round robin format using the "circle method"
    cout << "\nPlaying Round Robin Matches:\n";

    for (int round = 0; round < playerCount - 1; round++) {
        cout << "\nRound " << (round + 1) << " Results:\n";

        // First match of the round always involves player 0
        Match match;
        match.matchID = "RR" + to_string(round * (playerCount / 2) + 1);
        match.player1ID = tournamentPlayers[0].playerID;
        match.player2ID = tournamentPlayers[round + 1].playerID;
        match.stage = "Round Robin";
        match.status = "Completed";
        match.score = generateScore();

        // Determine winner
        if (tournamentPlayers[0].playerRanking < tournamentPlayers[round + 1].playerRanking) {
            match.winnerID = tournamentPlayers[0].playerID;
            tournamentPlayers[0].wins++;
            cout << "Match ID: " << match.matchID << ", Winner: "
                << tournamentPlayers[0].playerName << endl;
        }
        else {
            match.winnerID = tournamentPlayers[round + 1].playerID;
            tournamentPlayers[round + 1].wins++;
            cout << "Match ID: " << match.matchID << ", Winner: "
                << tournamentPlayers[round + 1].playerName << endl;
        }
        writeMatch(match);

        // Other matches in this round
        for (int matchIdx = 1; matchIdx < playerCount / 2; matchIdx++) {
            int player1 = (round + matchIdx) % (playerCount - 1) + 1;
            int player2 = (round + playerCount - matchIdx) % (playerCount - 1) + 1;

            Match match;
            match.matchID = "RR" + to_string(round * (playerCount / 2) + matchIdx + 1);
            match.player1ID = tournamentPlayers[player1].playerID;
            match.player2ID = tournamentPlayers[player2].playerID;
            match.stage = "Round Robin";
            match.status = "Completed";
            match.score = generateScore();

            // Determine winner
            if (tournamentPlayers[player1].playerRanking < tournamentPlayers[player2].playerRanking) {
                match.winnerID = tournamentPlayers[player1].playerID;
                tournamentPlayers[player1].wins++;
                cout << "Match ID: " << match.matchID << ", Winner: "
                    << tournamentPlayers[player1].playerName << endl;
            }
            else {
                match.winnerID = tournamentPlayers[player2].playerID;
                tournamentPlayers[player2].wins++;
                cout << "Match ID: " << match.matchID << ", Winner: "
                    << tournamentPlayers[player2].playerName << endl;
            }
            writeMatch(match);
        }
    }

    // Sort players by number of wins (descending)
    for (int i = 0; i < playerCount - 1; i++) {
        for (int j = 0; j < playerCount - i - 1; j++) {
            if (tournamentPlayers[j].wins < tournamentPlayers[j + 1].wins) {
                TennisPlayer temp = tournamentPlayers[j];
                tournamentPlayers[j] = tournamentPlayers[j + 1];
                tournamentPlayers[j + 1] = temp;
            }
        }
    }

    // Display round robin results
    cout << "\nRound Robin Final Standings:\n";
    for (int i = 0; i < playerCount; i++) {
        cout << (i + 1) << ". " << tournamentPlayers[i].playerName
            << ": " << tournamentPlayers[i].wins << " wins" << endl;
    }

    // Update tournament for Knockout stage
    delete[] players;
    maxSize = 4;
    players = new TennisPlayer[maxSize];
    queueFront = 0;
    queueRear = -1;
    currentSize = 0;
    stageName = "Knockout";

    // Take top 4 players by wins
    cout << "\nPlayers Advancing to Knockout Stage: ";
    for (int i = 0; i < 4; i++) {
        enqueue(tournamentPlayers[i]);
        cout << tournamentPlayers[i].playerName;
        if (i < 3) cout << ", ";
        else cout << endl;
    }
}

void TournamentQueue::runKnockout() {
    cout << "\nRunning Knockout (4 -> 2 players):\n";
    TournamentQueue nextStage("Finals", 4);

    // Get all 4 players
    TennisPlayer knockoutPlayers[4];
    int playerCount = 0;

    while (!isEmpty()) {
        knockoutPlayers[playerCount++] = dequeue();
    }

    // Match 1: Player 1 vs Player 4
    Match match1;
    match1.matchID = "K1";
    match1.player1ID = knockoutPlayers[0].playerID;
    match1.player2ID = knockoutPlayers[3].playerID;
    match1.stage = "Knockout";
    match1.status = "Completed";
    match1.score = generateScore();

    TennisPlayer winner1 = (knockoutPlayers[0].playerRanking < knockoutPlayers[3].playerRanking) ?
        knockoutPlayers[0] : knockoutPlayers[3];
    TennisPlayer loser1 = (knockoutPlayers[0].playerRanking < knockoutPlayers[3].playerRanking) ?
        knockoutPlayers[3] : knockoutPlayers[0];

    match1.winnerID = winner1.playerID;
    writeMatch(match1);
    cout << "Match ID: " << match1.matchID << ", Winner: " << winner1.playerName << endl;

    // Match 2: Player 2 vs Player 3
    Match match2;
    match2.matchID = "K2";
    match2.player1ID = knockoutPlayers[1].playerID;
    match2.player2ID = knockoutPlayers[2].playerID;
    match2.stage = "Knockout";
    match2.status = "Completed";
    match2.score = generateScore();

    TennisPlayer winner2 = (knockoutPlayers[1].playerRanking < knockoutPlayers[2].playerRanking) ?
        knockoutPlayers[1] : knockoutPlayers[2];
    TennisPlayer loser2 = (knockoutPlayers[1].playerRanking < knockoutPlayers[2].playerRanking) ?
        knockoutPlayers[2] : knockoutPlayers[1];

    match2.winnerID = winner2.playerID;
    writeMatch(match2);
    cout << "Match ID: " << match2.matchID << " - Winner: " << winner2.playerName << endl;

    // Enqueue for finals in order:
    // 1. Winners (for championship)
    // 2. Losers (for 3rd place match)
    nextStage.enqueue(winner1);
    nextStage.enqueue(winner2);
    nextStage.enqueue(loser1);
    nextStage.enqueue(loser2);

    // Update this tournament object for Finals
    delete[] players;
    maxSize = 4;
    players = new TennisPlayer[maxSize];
    queueFront = 0;
    queueRear = -1;
    currentSize = 0;
    stageName = "Finals";

    // Transfer players and display advancing players
    TennisPlayer advancing[4];
    int advCount = 0;
    while (!nextStage.isEmpty()) {
        advancing[advCount++] = nextStage.dequeue();
        enqueue(advancing[advCount - 1]);
    }

    cout << "\nPlayers Advancing to Finals Stage: ";
    for (int i = 0; i < advCount; i++) {
        cout << advancing[i].playerName;
        if (i < advCount - 1) cout << ", ";
        else cout << endl;
    }
}

void TournamentQueue::runFinals() {
    cout << "\nRunning Finals:\n";

    // Get all players from queue
    TennisPlayer finalPlayers[4];
    int playerCount = 0;

    while (!isEmpty()) {
        finalPlayers[playerCount++] = dequeue();
    }

    // First two players are in championship, last two are in 3rd place match

    // Championship match
    Match championship;
    championship.matchID = "F1";
    championship.player1ID = finalPlayers[0].playerID;
    championship.player2ID = finalPlayers[1].playerID;
    championship.stage = "Finals";
    championship.status = "Completed";
    championship.score = generateScore();

    TennisPlayer champion = (finalPlayers[0].playerRanking < finalPlayers[1].playerRanking) ?
        finalPlayers[0] : finalPlayers[1];
    TennisPlayer runnerUp = (finalPlayers[0].playerRanking < finalPlayers[1].playerRanking) ?
        finalPlayers[1] : finalPlayers[0];

    championship.winnerID = champion.playerID;
    writeMatch(championship);
    cout << "Match ID: " << championship.matchID << ", Champion: " << champion.playerName << endl;

    // 3rd place match
    Match thirdPlace;
    thirdPlace.matchID = "F2";
    thirdPlace.player1ID = finalPlayers[2].playerID;
    thirdPlace.player2ID = finalPlayers[3].playerID;
    thirdPlace.stage = "Finals";
    thirdPlace.status = "Completed";
    thirdPlace.score = generateScore();

    TennisPlayer third = (finalPlayers[2].playerRanking < finalPlayers[3].playerRanking) ?
        finalPlayers[2] : finalPlayers[3];
    TennisPlayer fourth = (finalPlayers[2].playerRanking < finalPlayers[3].playerRanking) ?
        finalPlayers[3] : finalPlayers[2];

    thirdPlace.winnerID = third.playerID;
    writeMatch(thirdPlace);
    cout << "Match ID: " << thirdPlace.matchID << ", Third Place: " << third.playerName << endl;

    // Update tournament for final rankings
    delete[] players;
    maxSize = 4;
    players = new TennisPlayer[maxSize];
    queueFront = 0;
    queueRear = -1;
    currentSize = 0;
    stageName = "Final Rankings";

    // Enqueue players in order of final ranking
    enqueue(champion);
    enqueue(runnerUp);
    enqueue(third);
    enqueue(fourth);

    displayFinalRankings();
}

void TournamentQueue::displayFinalRankings() {
    cout << "\nFinal Tournament Rankings:\n";
    for (int i = 0; i < currentSize; i++) {
        int index = (queueFront + i) % maxSize;
        cout << "Rank " << (i + 1) << ": " << players[index].playerName
            << " (Original World Rank: " << players[index].playerRanking << ")\n";
    }
}
