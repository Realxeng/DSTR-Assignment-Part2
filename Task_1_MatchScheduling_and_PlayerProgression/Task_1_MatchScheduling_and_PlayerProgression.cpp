
#include "../Task_1_MatchScheduling_and_PlayerProgression/MatchScheduling.hpp"
#include "../Task_2_TourRegis_and_PlayerQueue/RegistrationAndQueue_Manager.hpp"

#include <iostream>

using namespace std;

int main() {
    const int CAPACITY = 6; // Number of finalists
    RegistrationManager regManager(CAPACITY);

    // Register players (same as your friend's example)
    regManager.registerPlayer(1, "Alice", true, false);
    regManager.registerPlayer(2, "Bob", false, false);
    regManager.registerPlayer(3, "Charlie", false, true);
    regManager.registerPlayer(4, "David", false, false);
    regManager.registerPlayer(5, "Eve", true, false);
    regManager.registerPlayer(6, "Frank", false, false);

    regManager.showAll();

    regManager.finaliseList();
    regManager.showAll();

    regManager.checkIn(1);
    regManager.checkIn(2);
    regManager.checkIn(3);
    regManager.checkIn(4);
    regManager.checkIn(5);
    regManager.checkIn(6);
    regManager.showAll();

    // Now start match scheduling
    MatchSchedulingManager scheduler;

    // Feed finalist players to scheduler
    scheduler.setQualifiers(regManager.getFinalists(), regManager.getFinalistCount());

    // Schedule qualifier matches
    scheduler.scheduleQualifierMatches();
    scheduler.displayMatches();

    // Simulate some results - advance winners of the matches by player ID
    scheduler.advanceWinner(0, 1); // Alice wins match 1 (valid)
    scheduler.advanceWinner(1, 5); // Eve wins match 2 (valid)
    scheduler.advanceWinner(2, 6); // Frank wins match 3 (valid)

    scheduler.displayMatches();
    scheduler.displayGroupStage();

    return 0;
}