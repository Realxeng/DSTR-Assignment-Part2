
#include "../Task_1_MatchScheduling_and_PlayerProgression/MatchScheduling.hpp"
#include "../Task_2_TourRegis_and_PlayerQueue/RegistrationAndQueue_Manager.hpp"

int main() {
    RegistrationManager regManager(8);

    // Register some players — reuse your friend's method (id, name, earlybird, wildcard)
    regManager.registerPlayer(1, "Alice", true, false);
    regManager.registerPlayer(2, "Bob", false, false);
    regManager.registerPlayer(3, "Charlie", false, true);
    regManager.registerPlayer(4, "David", false, false);
    regManager.registerPlayer(5, "Eve", true, false);
    regManager.registerPlayer(6, "Frank", false, false);
    regManager.registerPlayer(7, "Grace", false, false);
    regManager.registerPlayer(8, "Heidi", false, false);

    // Finalize list and check in all players
    regManager.finaliseList();

    regManager.checkIn(1);
    regManager.checkIn(2);
    regManager.checkIn(3);
    regManager.checkIn(4);
    regManager.checkIn(5);
    regManager.checkIn(6);
    regManager.checkIn(7);
    regManager.checkIn(8);

    regManager.showAll();

    // Create match scheduler and run qualifier + knockout
    MatchScheduler scheduler(&regManager);
    scheduler.scheduleQualifier();
    scheduler.playQualifierMatches();
    scheduler.playKnockoutRounds();

    return 0;
}