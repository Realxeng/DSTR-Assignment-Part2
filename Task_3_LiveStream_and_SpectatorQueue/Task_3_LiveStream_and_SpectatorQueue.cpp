#include "Audience_Manager.hpp"

int main() {
    // Initialize AudienceManager with limited seats for testing overflow
    AudienceManager audienceMgr(5, 3, 5, 2); // Reduced general seats to 5 for quick overflow

    // Add VIPs (5 seats available)
    audienceMgr.seatAudienceMember(101, "Victor", true, false, false);
    audienceMgr.seatAudienceMember(102, "Vera", true, false, false);
    audienceMgr.seatAudienceMember(103, "Vincent", true, false, false);
    audienceMgr.seatAudienceMember(104, "Violet", true, false, false);
    audienceMgr.seatAudienceMember(105, "Vince", true, false, false);
    audienceMgr.seatAudienceMember(106, "Vicky", true, false, false); // Should overflow

    // Add Influencers (3 seats available)
    audienceMgr.seatAudienceMember(201, "Nina", false, true, false);
    audienceMgr.seatAudienceMember(202, "Nolan", false, true, false);
    audienceMgr.seatAudienceMember(203, "Natalie", false, true, false);
    audienceMgr.seatAudienceMember(204, "Neil", false, true, false); // Should overflow

    // Add Live Stream slots (2 slots available)
    audienceMgr.seatAudienceMember(301, "Olive", false, false, true);
    audienceMgr.seatAudienceMember(302, "Oscar", false, false, true);
    audienceMgr.seatAudienceMember(303, "Opal", false, false, true); // Should overflow

    // Add General Spectators (5 seats available)
    audienceMgr.seatAudienceMember(401, "Pat", false, false, false);
    audienceMgr.seatAudienceMember(402, "Paul", false, false, false);
    audienceMgr.seatAudienceMember(403, "Pam", false, false, false);
    audienceMgr.seatAudienceMember(404, "Pete", false, false, false);
    audienceMgr.seatAudienceMember(405, "Pia", false, false, false);
    audienceMgr.seatAudienceMember(406, "Phil", false, false, false); // Should overflow
    audienceMgr.seatAudienceMember(407, "Polly", false, false, false); // Should overflow

    // Display current seating and overflow status
    audienceMgr.displayStatus();

    // Attempt to fill seats from overflow (if any seats have freed up, not in this example)
    audienceMgr.tryFillOverflow();

    return 0;
}