#include "Audience_Member.hpp";

class AudienceManager {
    FixedArrayQueue vipSeats;
    FixedArrayQueue influencerSeats;
    FixedArrayQueue liveStreamSlots;
    FixedArrayQueue generalSeats;
    OverflowQueue overflowQueue;

public:
    AudienceManager(int maxVIP, int maxInfluencer, int maxGeneral, int maxLiveStream)
        : vipSeats(maxVIP),
        influencerSeats(maxInfluencer),
        liveStreamSlots(maxLiveStream),
        generalSeats(maxGeneral)
    {
    }

    bool seatAudienceMember(int id, const string& name, bool isVIP, bool isInfluencer, bool hasLiveStream) {
        AudienceMember member = { id, name, isVIP, isInfluencer, hasLiveStream };

        if (isVIP) {
            if (vipSeats.addMember(member)) {
                cout << "Seated VIP " << name << " (ID: " << id << ") in VIP area." << endl;
                return true;  // Important: return after success
            }
            cout << "VIP Seating Full for " << name << " (ID: " << id << ")." << endl;
        }

        if (isInfluencer) {
            if (influencerSeats.addMember(member)) {
                cout << "Seated Influencer " << name << " (ID: " << id << ") in Influencer area." << endl;
                return true;  // Important: return after success
            }
            cout << "Influencer Seating Full for " << name << " (ID: " << id << ")." << endl;
        }

        if (hasLiveStream) {
            if (liveStreamSlots.addMember(member)) {
                cout << "Allocated Live Stream slot to " << name << " (ID: " << id << ")." << endl;
                return true;  // Important: return after success
            }
            cout << "Live Stream Slots Full for " << name << " (ID: " << id << ")." << endl;
        }

        // Try general seating last
        if (generalSeats.addMember(member)) {
            cout << "Seated General spectator " << name << " (ID: " << id << ") in General seating." << endl;
            return true;  // Important: return after success
        }

        // All seats full; add to overflow ONLY once
        cout << "All seating areas full. Adding " << name << " (ID: " << id << ") to overflow queue.\n";
        overflowQueue.enqueue(member);
        return false;
    }

    void removeAudienceMember(int id) {
        if (vipSeats.removeMemberById(id)) {
            cout << "Removed audience member with ID " << id << " from VIP seats." << endl;
            tryFillOverflow();
            return;
        }
        if (influencerSeats.removeMemberById(id)) {
            cout << "Removed audience member with ID " << id << " from Influencer seats." << endl;
            tryFillOverflow();
            return;
        }
        if (liveStreamSlots.removeMemberById(id)) {
            cout << "Removed audience member with ID " << id << " from Live Stream slots." << endl;
            tryFillOverflow();
            return;
        }
        if (generalSeats.removeMemberById(id)) {
            cout << "Removed audience member with ID " << id << " from General seats." << endl;
            tryFillOverflow();
            return;
        }
        cout << "Audience member with ID " << id << " not found in any seating." << endl;
    }

    void tryFillOverflow() {
    // Only try to seat the member at the front of the overflow queue
    while(!overflowQueue.isEmpty()) {
        const AudienceMember* memberPtr = overflowQueue.peekFront();

        if (memberPtr == nullptr) return;

        AudienceMember member = *memberPtr;

        // Try to manually assign to categories *without* using seatAudienceMember() directly:
        bool seated = false;
        if (member.isVIP && vipSeats.addMember(member)) {
            cout << "Moved " << member.name << " (ID: " << member.id << ") from overflow to VIP seat.\n";
            seated = true;
        } else if (member.isInfluencer && influencerSeats.addMember(member)) {
            cout << "Moved " << member.name << " (ID: " << member.id << ") from overflow to Influencer seat.\n";
            seated = true;
        } else if (member.hasLiveStreamSetup && liveStreamSlots.addMember(member)) {
            cout << "Moved " << member.name << " (ID: " << member.id << ") from overflow to Live Stream slot.\n";
            seated = true;
        } else if (generalSeats.addMember(member)) {
            cout << "Moved " << member.name << " (ID: " << member.id << ") from overflow to General seat.\n";
            seated = true;
        }

        if (seated) {
            overflowQueue.dequeue(); // Only dequeue if successfully seated
        } else {
            break; // If current member still can't be seated, stop trying (as queue is in order)
        }
    }
}

    void displayStatus() const {
        cout << "\n=== Audience Seating Status ===" << endl;
        vipSeats.displaySeats("VIP Seats");
        influencerSeats.displaySeats("Influencer Seats");
        liveStreamSlots.displaySeats("Live Stream Slots");
        generalSeats.displaySeats("General Seats");
        overflowQueue.displayQueue();
        cout << "================================\n" << endl;
    }
};