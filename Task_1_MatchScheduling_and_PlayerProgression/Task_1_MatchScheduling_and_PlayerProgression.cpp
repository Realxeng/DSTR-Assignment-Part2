
#include "../Task_1_MatchScheduling_and_PlayerProgression/MatchScheduling.hpp"
#include "../Task_2_TourRegis_and_PlayerQueue/RegistrationAndQueue_Manager.hpp"
#include <iostream>

using namespace std;

int main() {
    const int CAPACITY = 6; // Number of finalists
    RegistrationManager regManager(CAPACITY);

    cout << "----- REGISTERING PLAYERS -----" << endl;
    regManager.registerPlayer(1, "Alice", true, false);
    regManager.registerPlayer(2, "Bob", false, false);
    regManager.registerPlayer(3, "Charlie", false, true);
    regManager.registerPlayer(4, "David", false, false);
    regManager.registerPlayer(5, "Eve", true, false);
    regManager.registerPlayer(6, "Frank", false, false);

    cout << "\nDisplaying all registered players:" << endl;
    regManager.showAll();

    cout << "\n----- FINALISING LIST OF FINALISTS -----" << endl;
    regManager.finaliseList();

    cout << "\nDisplaying all players after finalising finalists:" << endl;
    regManager.showAll();

    cout << "\n----- CHECK-IN PLAYERS -----" << endl;
    regManager.checkIn(1);
    regManager.checkIn(2);
    regManager.checkIn(3);
    regManager.checkIn(4);
    regManager.checkIn(5);
    regManager.checkIn(6);

    cout << "\nDisplaying status after check-in:" << endl;
    regManager.showAll();

    cout << "\n----- STARTING MATCH SCHEDULING -----" << endl;
    MatchSchedulingManager scheduler;

    // Pass finalists to scheduler
    cout << "Setting qualifiers using finalists from registration manager." << endl;
    scheduler.setQualifiers(regManager.getFinalists(), regManager.getFinalistCount());

    cout << "Scheduling qualifier matches based on qualifiers." << endl;
    scheduler.scheduleQualifierMatches();

    cout << "\nDisplaying scheduled qualifier matches:" << endl;
    scheduler.displayMatches();

    cout << "Simulating match results and advancing winners..." << endl;
    scheduler.advanceWinner(0, 1); // Alice wins match 1
    scheduler.advanceWinner(1, 5); // Eve wins match 2
    scheduler.advanceWinner(2, 6); // Frank wins match 3

    cout << "\nDisplaying updated matches with winners:" << endl;
    scheduler.displayMatches();

    cout << "Displaying players who advanced to group stage:" << endl;
    scheduler.displayGroupStage();

    return 0;
}