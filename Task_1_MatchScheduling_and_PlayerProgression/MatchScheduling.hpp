#ifndef MATCH_SCHEDULING_HPP
#define MATCH_SCHEDULING_HPP

#include <iostream>
#include <string>
#include "../Task_2_TourRegis_and_PlayerQueue/RegistrationAndQueue_Manager.hpp" 

using namespace std;

// Match struct holding two players and result flag
struct Match {
    playerNode* player1;
    playerNode* player2;
    playerNode* winner;

    Match(playerNode* p1 = nullptr, playerNode* p2 = nullptr)
        : player1(p1), player2(p2), winner(nullptr) {
    }

    void showMatch() {
        cout << "Match: " << (player1 ? player1->name : "Bye");
        cout << " VS " << (player2 ? player2->name : "Bye") << endl;
    }
};

// Simple queue for matches (linked-list)
struct MatchNode {
    Match data;
    MatchNode* next;
    MatchNode(const Match& m) : data(m), next(nullptr) {}
};

class MatchQueue {
    MatchNode* front;
    MatchNode* rear;
    int size;

public:
    MatchQueue() : front(nullptr), rear(nullptr), size(0) {}

    ~MatchQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    bool isEmpty() { return front == nullptr; }

    void enqueue(const Match& m) {
        MatchNode* newNode = new MatchNode(m);
        if (rear) rear->next = newNode;
        rear = newNode;
        if (!front) front = rear;
        size++;
    }

    Match dequeue() {
        if (isEmpty()) {
            cout << "MatchQueue is empty\n";
            return Match();
        }
        MatchNode* temp = front;
        Match ret = temp->data;
        front = front->next;
        if (!front) rear = nullptr;
        delete temp;
        size--;
        return ret;
    }

    int getSize() { return size; }

    void displayQueue() {
        cout << "Matches in queue: " << size << endl;
        MatchNode* curr = front;
        int count = 1;
        while (curr) {
            cout << count << ". ";
            curr->data.showMatch();
            curr = curr->next;
            count++;
        }
    }
};

// Simple stack for matches (linked-list) for knockout
struct MatchStackNode {
    Match data;
    MatchStackNode* next;
    MatchStackNode(const Match& m) : data(m), next(nullptr) {}
};

class MatchStack {
    MatchStackNode* topNode;
    int size;

public:
    MatchStack() : topNode(nullptr), size(0) {}

    ~MatchStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool isEmpty() { return topNode == nullptr; }

    void push(const Match& m) {
        MatchStackNode* newNode = new MatchStackNode(m);
        newNode->next = topNode;
        topNode = newNode;
        size++;
    }

    Match pop() {
        if (isEmpty()) {
            cout << "MatchStack is empty\n";
            return Match();
        }
        MatchStackNode* temp = topNode;
        Match ret = temp->data;
        topNode = topNode->next;
        delete temp;
        size--;
        return ret;
    }

    Match peek() {
        if (isEmpty()) {
            return Match();
        }
        return topNode->data;
    }

    int getSize() { return size; }
};

// MatchScheduler class to manage progression through qualifier and knockout
class MatchScheduler {
    RegistrationManager* regManager;  // link to registration manager
    MatchQueue qualifierMatches;
    MatchStack knockoutMatches;
    playerNode** checkedInPlayers;    // array of pointers to checked-in players
    int checkedInCount;

public:
    MatchScheduler(RegistrationManager* regMgr)
        : regManager(regMgr), checkedInPlayers(nullptr), checkedInCount(0) {
    }

    ~MatchScheduler() {
        if (checkedInPlayers) {
            delete[] checkedInPlayers;
        }
    }

    // Fetch players checked in from RegistrationManager's CheckInQueue
    void fetchCheckedInPlayers() {
        int count = regManager->getCheckedInCount();
        checkedInCount = count;
        if (checkedInPlayers) delete[] checkedInPlayers;
        checkedInPlayers = new playerNode * [checkedInCount];
        for (int i = 0; i < checkedInCount; ++i) {
            checkedInPlayers[i] = regManager->getCheckedInPlayerAt(i);
        }
    }

    // Schedule qualifier matches: pair players by order
    void scheduleQualifier() {
        cout << "Scheduling Qualifier matches...\n";
        fetchCheckedInPlayers();
        qualifierMatches = MatchQueue(); // reset queue

        // Pair players in order, if odd # players last gets a bye (nullptr)
        for (int i = 0; i < checkedInCount; i += 2) {
            playerNode* p1 = checkedInPlayers[i];
            playerNode* p2 = (i + 1 < checkedInCount) ? checkedInPlayers[i + 1] : nullptr;
            Match m(p1, p2);
            qualifierMatches.enqueue(m);
        }
        qualifierMatches.displayQueue();
    }

    // Simulate qualifier matches and record winners (for demo: pick player1 if both exist)
    void playQualifierMatches() {
        cout << "Playing Qualifier matches...\n";
        knockoutMatches = MatchStack();  // reset knockout matches stack
        while (qualifierMatches.getSize() > 0) {
            Match m = qualifierMatches.dequeue();
            m.showMatch();

            // simple logic: if p2 is nullptr, p1 winner else p1 wins arbitrarily
            if (m.player2 == nullptr) {
                m.winner = m.player1;
            }
            else {
                // for demo, choose the player with lex smaller name as winner
                if (m.player1->name < m.player2->name)
                    m.winner = m.player1;
                else
                    m.winner = m.player2;
            }
            cout << "Winner: " << m.winner->name << endl << endl;
            // push winners to knockout stack for next stage
            knockoutMatches.push(m);
        }
    }

    // Play the knockout rounds until one winner remain
    void playKnockoutRounds() {
        cout << "Playing Knockout rounds...\n";
        // To play knockout rounds, pop two matches at a time,
        // create a new match with winners, and push back until 1 left.

        MatchStack roundStack;
        while (knockoutMatches.getSize() > 1) {
            Match m1 = knockoutMatches.pop();
            Match m2 = knockoutMatches.pop();

            // If only one match left in stack (odd number), pass it forward
            if (m2.player1 == nullptr && m2.player2 == nullptr) {
                roundStack.push(m1);
                break;
            }

            playerNode* w1 = m1.winner;
            playerNode* w2 = m2.winner;

            cout << "Next Round Match: " << w1->name << " VS " << w2->name << endl;

            // Decide winner, here lex smallest name for demo
            Match nextMatch(w1, w2);
            if (w1->name < w2->name)
                nextMatch.winner = w1;
            else
                nextMatch.winner = w2;

            cout << "Winner: " << nextMatch.winner->name << endl << endl;

            roundStack.push(nextMatch);
        }

        // If odd number of matches, push last one forward
        if (knockoutMatches.getSize() == 1)
            roundStack.push(knockoutMatches.pop());

        knockoutMatches = roundStack;  // Assign new round to knockoutMatches

        // Repeat until 1 winner remains
        if (knockoutMatches.getSize() > 1)
            playKnockoutRounds();
        else if (knockoutMatches.getSize() == 1) {
            Match finalMatch = knockoutMatches.peek();
            cout << "=== Tournament Winner: " << finalMatch.winner->name << " ===" << endl;
        }
    }
};

#endif