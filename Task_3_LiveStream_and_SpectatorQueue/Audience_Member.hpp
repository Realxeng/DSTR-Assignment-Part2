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

#endif