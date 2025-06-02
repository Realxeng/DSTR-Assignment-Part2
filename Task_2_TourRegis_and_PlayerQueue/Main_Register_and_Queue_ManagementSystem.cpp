/**
 * TASK 2: TOURNAMENT REGISTRATION & PLAYER QUEUEING
 * Handling player registrations, last-minute check-ins, and ensuring an orderly tournament start.
 * --------------------------------------------------------------------
 * 
 * Register players and add them to finalists or waiting list based on their registration status.
 * Implement check-ins for players. (PDF)
 * Prioritize early-bird signups and wildcard entries.  (PDF)
 * Handle withdrawals and replacements efficiently. (PDF)
 * Display all queues.
 */

#include <iostream>
#include <string>

#include "RegistrationAndQueue_Manager.hpp"

using namespace std;

// Clear the input buffer until a newline character is found
void clearStdin() {
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    cout << "===== Tournament Registration & Queue Management Demo =====" << endl;

    // Ask the user for tournament capacity
    int capacity = 0;
    cout << "Enter tournament capacity (max participant): ";
    cin >> capacity;

    // Construct the manager with that capacity
    RegistrationManager regManager(capacity);
    cout << "\\n>>> Capacity set to " << capacity << " players." << endl << endl;


    // Preload some players for demonstration
    regManager.registerPlayer(1, "Alice", false, false);
    regManager.registerPlayer(2, "Bob", true, false);
    regManager.registerPlayer(3, "Charlie", false, true);
    regManager.registerPlayer(4, "Diana", false, false);
    regManager.registerPlayer(5, "Eve", true, false);
    regManager.registerPlayer(6, "Frank", false, true);
    regManager.registerPlayer(7, "Grace", false, false);
    regManager.registerPlayer(8, "Heidi", true, false);
    regManager.registerPlayer(9, "Ivan", false, true);
    regManager.registerPlayer(10, "Judy", false, false);
    cout << ">>> Preloaded 10 players for demonstration." << endl << endl;
    

    // Menu loop
    while (true) {
        cout << "==============================================" << endl;
        cout << " 1) Register a new player" << endl;
        cout << " 2) Finalise list of finalists" << endl;
        cout << " 3) Check in a finalist" << endl;
        cout << " 4) Withdraw a player" << endl;
        cout << " 5) Show all queues" << endl;
        cout << " 6) Exit" << endl;
        cout << "==============================================" << endl;
        cout << "Choose an option [1-6]: ";

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 6) {
            cout << "  [!] Please enter a valid option (1–6): ";
            cin.clear();
        }
        clearStdin();

        if (choice == 1) {
            // -------------------------------
            // Register a new player
            // -------------------------------
            int id;
            string name;
            char ebChar, wcChar;
            bool isEarlyBird = false, isWildcard = false;

            cout << endl << "[Registration] Enter player ID (integer): ";
            cin >> id;
            clearStdin();

            cout << "Enter player name: ";
            getline(cin, name);

            cout << "Is this player an Early‐Bird? (y/n): ";
            cin >> ebChar; clearStdin();
            isEarlyBird = (ebChar == 'y' || ebChar == 'Y');

            cout << "Is this player a Wildcard?   (y/n): ";
            cin >> wcChar; clearStdin();
            isWildcard  = (wcChar == 'y' || wcChar == 'Y');

            regManager.registerPlayer(id, name, isEarlyBird, isWildcard);

        } else if (choice == 2) {
            // -------------------------------
            // Finalise the top 'n' finalists
            // -------------------------------
            regManager.finaliseList();

        } else if (choice == 3) {
            // -------------------------------
            // Check in a finalist
            // -------------------------------
            int pid;
            cout << endl << "[Check-In] Enter finalist ID to check in: ";
            cin >> pid; clearStdin();

            regManager.checkIn(pid);

        } else if (choice == 4) {
            // -------------------------------
            // Withdraw a player (and auto-replace)
            // -------------------------------
            int pid;
            cout << endl << "[Withdrawal] Enter player ID to withdraw: ";
            cin >> pid;
            clearStdin();

            regManager.withdraw(pid);

        } else if (choice == 5) {
            // -------------------------------
            // Show the contents of all queues
            // -------------------------------
            regManager.showAll();

        } else if (choice == 6) {
            // -------------------------------
            // Exit
            // -------------------------------
            break;

        } else {
            cout << "  [!] Please choose a valid option (1–6)." << endl << endl;
        }
    }

    return 0;
}
