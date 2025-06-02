#ifndef AUDIENCE_QUEUE_MANAGER_HPP
#define AUDIENCE_QUEUE_MANAGER_HPP

#include <iostream>
#include <string>
#include <limits>
using namespace std;

// Structure for Audience Member
struct AudienceMember {
    int id;
    string name;
    bool isVIP;
    bool isInfluencer;
    bool hasLiveStreamSetup;
};

class FixedArrayQueue {
    AudienceMember* seats;
    int capacity;
    int count;

public:
    FixedArrayQueue(int cap) : capacity(cap), count(0) {
        seats = new AudienceMember[capacity];
    }

    ~FixedArrayQueue() {
        delete[] seats;
    }

    bool isFull() const {
        return count == capacity;
    }

    bool isEmpty() const {
        return count == 0;
    }

    // Adds a member to the seats if space available
    bool addMember(const AudienceMember& member) {
        if (isFull())
            return false;
        seats[count++] = member;
        return true;
    }

    // Remove member by ID (linear search)
    bool removeMemberById(int id) {
        for (int i = 0; i < count; ++i) {
            if (seats[i].id == id) {
                // Shift left the rest
                for (int j = i; j < count - 1; ++j) {
                    seats[j] = seats[j + 1];
                }
                --count;
                return true;
            }
        }
        return false;
    }

    void displaySeats(const string& seatName) const {
        cout << seatName << " [" << count << "/" << capacity << "]:" << endl;
        if (count == 0) {
            cout << "  (None)" << endl;
            return;
        }
        for (int i = 0; i < count; ++i) {
            cout << "  ID(" << seats[i].id << ") " << seats[i].name << endl;
        }
    }

    int getCount() const {
        return count;
    }
};

struct OverflowNode {
    AudienceMember member;
    OverflowNode* next;
    OverflowNode(const AudienceMember& m) : member(m), next(nullptr) {}
};

// Linked list queue for overflow spectators
class OverflowQueue {
    OverflowNode* front;
    OverflowNode* rear;
    int size;

public:
    OverflowQueue() : front(nullptr), rear(nullptr), size(0) {}

    ~OverflowQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    const AudienceMember* peekFront() const {
        if (front == nullptr)
            return nullptr;
        return &(front->member);
    }

    void enqueue(const AudienceMember& member) {
        OverflowNode* node = new OverflowNode(member);
        if (rear == nullptr) {
            front = rear = node;
        }
        else {
            rear->next = node;
            rear = node;
        }
        ++size;
        cout << "Added " << member.name << " (ID: " << member.id << ") to overflow queue." << endl;
    }

    // Dequeue from overflow
    bool dequeue() {
        if (front == nullptr)
            return false;
        OverflowNode* temp = front;
        front = front->next;
        if (front == nullptr)
            rear = nullptr;
        cout << "Removed " << temp->member.name << " (ID: " << temp->member.id << ") from overflow queue." << endl;
        delete temp;
        --size;
        return true;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    int getSize() const {
        return size;
    }

    void displayQueue() const {
        cout << "Overflow Queue [" << size << "]:" << endl;
        if (isEmpty()) {
            cout << "  (Empty)" << endl;
            return;
        }
        OverflowNode* current = front;
        int pos = 1;
        while (current != nullptr) {
            cout << "  " << pos++ << ". ID(" << current->member.id << ") " << current->member.name << endl;
            current = current->next;
        }
    }
};

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

#endif