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
                return true;
            }
            cout << "VIP Seating Full for " << name << endl;
        }

        if (isInfluencer) {
            if (influencerSeats.addMember(member)) {
                cout << "Seated Influencer " << name << " (ID: " << id << ") in Influencer area." << endl;
                return true;
            }
            cout << "Influencer Seating Full for " << name << endl;
        }

        if (hasLiveStream) {
            if (liveStreamSlots.addMember(member)) {
                cout << "Allocated Live Stream slot to " << name << " (ID: " << id << ")." << endl;
                return true;
            }
            cout << "Live Stream Slots Full for " << name << endl;
        }

        if (generalSeats.addMember(member)) {
            cout << "Seated General spectator " << name << " (ID: " << id << ") in General seating." << endl;
            return true;
        }

        // All seats full, add to overflow
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
        if (overflowQueue.isEmpty())
            return;
        // Attempt to move one member from overflow to an appropriate seating
        const AudienceMember* memberPtr = overflowQueue.peekFront();
        if (memberPtr == nullptr)
            return; // Overflow queue is empty

        AudienceMember member = *memberPtr;

        // Remove from overflow only if seating is available
        bool seated = seatAudienceMember(member.id, member.name, member.isVIP, member.isInfluencer, member.hasLiveStreamSetup);
        if (seated) {
            overflowQueue.dequeue();
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