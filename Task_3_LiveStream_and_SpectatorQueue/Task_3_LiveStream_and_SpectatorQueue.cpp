#include "Audience_Manager.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Helper function for input
string promptStr(const string& msg) {
    cout << msg;
    string s;
    getline(cin, s);
    return s;
}

int main() {
    cout << "=== Audience Seating System ===\n";
    int vipCap, infCap, genCap, liveCap;
    cout << "Enter VIP seat capacity: ";   cin >> vipCap;
    cout << "Enter Influencer seat capacity: "; cin >> infCap;
    cout << "Enter General seat capacity: ";    cin >> genCap;
    cout << "Enter Live Stream slot capacity: "; cin >> liveCap;
    cin.ignore();

    AudienceManager audienceMgr(vipCap, infCap, genCap, liveCap);
    int nextAudienceID = 1; // ID auto-generator

    while (true) {
        cout << "\nOptions:\n";
        cout << "1. Add an audience member\n";
        cout << "2. Show seating status\n";
        cout << "3. Audience member leaves a specific area\n";
        cout << "4. Exit\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            int id = nextAudienceID++; // auto-generated ID

            string name = promptStr("Enter name: ");

            cout << "Select audience tier:\n";
            cout << "  1. VIP\n";
            cout << "  2. Influencer\n";
            cout << "  3. Live Stream\n";
            cout << "  4. General\n";
            cout << "Enter choice [1-4]: ";

            string tierInput;
            getline(cin, tierInput);

            int tierChoice = 4;  // default to General
            if (!tierInput.empty()) {
                std::istringstream ss(tierInput);
                if (!(ss >> tierChoice) || tierChoice < 1 || tierChoice > 4) {
                    cout << "Invalid tier. Defaulting to General.\n";
                    tierChoice = 4;
                }
            }
            else {
                cout << "No input detected. Defaulting to General.\n";
                tierChoice = 4;
            }

            bool isVIP = false, isInf = false, isLive = false;

            switch (tierChoice) {
            case 1: isVIP = true; break;
            case 2: isInf = true; break;
            case 3: isLive = true; break;
            case 4: /* general: all false */ break;
            }

            audienceMgr.seatAudienceMember(id, name, isVIP, isInf, isLive);
            cout << "Assigned Audience ID: " << id << endl;
        }
        else if (choice == 2) {
            audienceMgr.displayStatus();
        }
        else if (choice == 5) {
            cout << "Goodbye!\n";
            break;
        }
        else if (choice == 3) {
            cout << "Select area to remove from:\n";
            cout << " 1. VIP\n 2. Influencer\n 3. Live Stream\n 4. General\n 5. Overflow\n";
            cout << "Enter choice [1-5]: ";
            int areaChoice;
            cin >> areaChoice;
            cin.ignore();

            string area;
            switch (areaChoice) {
            case 1: area = "VIP"; break;
            case 2: area = "Influencer"; break;
            case 3: area = "Live Stream"; break;
            case 4: area = "General"; break;
            case 5: area = "Overflow"; break;
            default:
                cout << "Invalid area choice.\n";
                return 0;
            }

            cout << "Enter Audience ID to remove: ";
            int id;
            cin >> id;
            cin.ignore();

            audienceMgr.removeFromArea(area, id);
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}