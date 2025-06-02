#ifndef MATCH_SCHEDULING_HPP
#define MATCH_SCHEDULING_HPP

#include <iostream>
#include <string>
#include "../Task_2_TourRegis_and_PlayerQueue/RegistrationAndQueue_Manager.hpp"
using namespace std;

struct Match {
    playerNode* player1;
    playerNode* player2;
    int winnerId;

	Match() : player1(nullptr), player2(nullptr), winnerId(-1) {} //-1 indicates no winner yet
};

class MatchSchedulingManager {
    playerNode* qualifiers;
    int qualifierCount;

    playerNode* groupStage;
    int groupCount;

    Match* knockoutMatches;
    int knockoutCount;

public:
    MatchSchedulingManager() : qualifiers(nullptr), qualifierCount(0),
        groupStage(nullptr), groupCount(0),
        knockoutMatches(nullptr), knockoutCount(0) {
    }

    ~MatchSchedulingManager() {
        if (qualifiers) delete[] qualifiers;
        if (groupStage) delete[] groupStage;
        if (knockoutMatches) delete[] knockoutMatches;
    }

    // Initialize qualifiers using finalist array from RegistrationManager
    void setQualifiers(playerNode finalists[], int count) {
        if (qualifiers) delete[] qualifiers;
        qualifierCount = count;
        qualifiers = new playerNode[qualifierCount];
        for (int i = 0; i < qualifierCount; ++i) {
            qualifiers[i] = finalists[i]; // copy player info
        }
        cout << "Qualifiers set with " << qualifierCount << " players." << endl;
    }

    // Helper to print player info
    void printPlayer(playerNode* p) {
        if (p != nullptr)
            cout << "[" << p->id << ": " << p->name << "]";
        else
            cout << "[---]";
    }

    // Schedule matches for the qualifier stage - pair by adjacent players
    void scheduleQualifierMatches() {
        if (qualifierCount < 2) {
            cout << "Not enough players for qualifier matches." << endl;
            return;
        }
        int pairCount = qualifierCount / 2;
        if (knockoutMatches) delete[] knockoutMatches;
        knockoutCount = pairCount;

        knockoutMatches = new Match[knockoutCount];
        for (int i = 0; i < knockoutCount; ++i) {
            knockoutMatches[i].player1 = &qualifiers[2 * i];
            knockoutMatches[i].player2 = &qualifiers[2 * i + 1];
            knockoutMatches[i].winnerId = -1; // undecided
        }
        cout << "Qualifier matches scheduled: " << knockoutCount << " matches." << endl;
    }

    void displayMatches() {
        if (knockoutCount == 0) {
            cout << "No matches scheduled currently." << endl;
            return;
        }
        cout << "Scheduled Matches:" << endl;
        for (int i = 0; i < knockoutCount; ++i) {
            cout << "Match " << i + 1 << ": ";
            printPlayer(knockoutMatches[i].player1);
            cout << " vs ";
            printPlayer(knockoutMatches[i].player2);
            if (knockoutMatches[i].winnerId == -1)
                cout << " - Winner: Not decided yet." << endl;
            else
                cout << " - Winner: PlayerID " << knockoutMatches[i].winnerId << endl;
        }
        cout << endl;
    }

    // Advance winner for a match, pushes winner to groupStage array for demonstration
    void advanceWinner(int matchIndex, int winnerId) {
        if (matchIndex < 0 || matchIndex >= knockoutCount) {
            cout << "Invalid match index." << endl;
            return;
        }
        Match& m = knockoutMatches[matchIndex];
        if (m.winnerId != -1) {
            cout << "Winner already decided for this match." << endl;
            return;
        }
        if ((m.player1 && m.player1->id == winnerId) || (m.player2 && m.player2->id == winnerId)) {
            m.winnerId = winnerId;
            cout << "Winner for Match " << matchIndex + 1 << " is PlayerID " << winnerId << "." << endl;
            // Add winner to group stage array (simple implementation)
            addGroupStagePlayer(winnerId);
        }
        else {
            cout << "Winner ID " << winnerId << " does not match players in this match: ";
            printPlayer(m.player1);
            cout << " vs ";
            printPlayer(m.player2);
            cout << endl;
        }
    }

    // Add winner player node to group stage list (dynamically resized array)
    void addGroupStagePlayer(int winnerId) {
        // Find playerNode in qualifiers with winnerId
        playerNode* winnerPlayer = nullptr;
        for (int i = 0; i < qualifierCount; ++i) {
            if (qualifiers[i].id == winnerId) {
                winnerPlayer = &qualifiers[i];
                break;
            }
        }
        if (winnerPlayer == nullptr) {
            cout << "Winner player not found in qualifiers." << endl;
            return;
        }
        // Expand groupStage array by 1
        playerNode* newGroupStage = new playerNode[groupCount + 1];
        for (int i = 0; i < groupCount; ++i) {
            newGroupStage[i] = groupStage[i];
        }
        newGroupStage[groupCount] = *winnerPlayer;
        groupCount++;
        if (groupStage) delete[] groupStage;
        groupStage = newGroupStage;
        cout << "Player " << winnerPlayer->name << " advanced to group stage." << endl;
    }

    // Display players in group stage
    void displayGroupStage() {
        if (groupCount == 0) {
            cout << "No players in group stage yet." << endl;
            return;
        }
        cout << "Group Stage Players (" << groupCount << "):" << endl;
        for (int i = 0; i < groupCount; ++i) {
            cout << i + 1 << ". ID: " << groupStage[i].id << " Name: " << groupStage[i].name << endl;
        }
        cout << endl;
    }

    // Further methods to schedule group stage matches and knockout rounds can be added similarly
};

#endif