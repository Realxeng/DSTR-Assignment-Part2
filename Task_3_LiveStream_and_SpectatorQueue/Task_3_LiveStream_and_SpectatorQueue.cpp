#include "Audience_Manager.hpp"

int main() {
    // Declare and initialize AudienceManager properly
    AudienceManager audienceMgr(5, 3, 20, 2);

    // Proper calls with no extra characters and semicolons
    audienceMgr.seatAudienceMember(101, "Victor", true, false, false);
    audienceMgr.seatAudienceMember(102, "Nina", false, true, false);
    audienceMgr.seatAudienceMember(103, "Olive", false, false, true);
    audienceMgr.seatAudienceMember(104, "Pat", false, false, false);

    // Display current seating status
    audienceMgr.displayStatus();

    // Handle overflow if needed
    audienceMgr.tryFillOverflow();

    return 0;
}