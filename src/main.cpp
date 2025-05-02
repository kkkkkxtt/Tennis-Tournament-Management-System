#include "Tournament.hpp"

//read player.txt
//loop iterate player.txt to create each player object 
//display all player
//schedule match
//iterated player to the match(qualifiers), each match 2 player, left 6 player advanced
//play match
//schedule match
//iterated player to the round robin match, win match count for first 4 advanced
//play match
//schedule match
//left 4 player play knockout,
// play match
// schedule match
//left 2 winner play final, 2 loser play knockout for 3rd runner
//play match
//after all match, display match advanced result (Ranking )

int main() 
{
    // Initialize tournament with 12 players
    TournamentQueue tournament("Qualifiers", 12);
    // Load players from file
    tournament.readPlayer("player.txt");
    tournament.checkMatchFileExisting();

    cout << "--Tennis Championship Management System--" << endl;

    // Run tournament stages
    //Qualifier 12 players -> 6 players
    tournament.displayPlayers();
    tournament.scheduleMatch();
    tournament.runQualifiers();

    //Round Robin 6 players -> 4 players
    tournament.displayPlayers();
    tournament.scheduleMatch();
    tournament.runRoundRobin();

    //Knockout 4 players -> each 2 players
    tournament.displayPlayers();
    tournament.scheduleMatch();
    tournament.runKnockout();

    //Final championship, 2nd place and 3rd place
    tournament.displayPlayers();
    tournament.scheduleMatch();
    tournament.runFinals();

    tournament.displayFinalRankings();

    return 0;
}