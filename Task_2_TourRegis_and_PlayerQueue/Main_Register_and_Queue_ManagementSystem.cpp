#include "RegistrationAndQueue_Manager.hpp"

int main() {
    const int CAPACITY = 5; // Maximum number of finalists
    RegistrationManager regManager(CAPACITY);

    // Register players
    regManager.registerPlayer(1, "Alice", true, false);
    regManager.registerPlayer(2, "Bob", false, false);
    regManager.registerPlayer(3, "Charlie", false, true);
    regManager.registerPlayer(4, "David", false, false);
    regManager.registerPlayer(5, "Eve", true, false);

    regManager.showAll();

    
    // Finalize the list of finalists
    regManager.finaliseList();
    regManager.showAll();

    // Check in some players
    regManager.checkIn(3);
    regManager.checkIn(1);
    regManager.checkIn(2);
    regManager.showAll();

    // Withdraw a player and check replacement
    regManager.withdraw(4);
    regManager.showAll();

    return 0;
}