// AMA SVISTOYN TA SXOLEIA MAS H KATI POU NA EXEI SXESH ME TO TEMPLEOS H TERRY THA GINEI KATI POLU KAKO


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iomanip>
#include <cstring>
#include <limits>
#include <chrono>
#include <thread>

using namespace std;

// Structure to store password information
struct Password {
    char passwordType[50];  // Type/name (e.g., "Gmail", "Facebook")
    char password[100];     // The encrypted password in hex
};

vector<Password> passwords;
const string FILENAME = "passwords.bin";

// XOR key
// MHN TO SVISI KANIS THA TO GAMISO :) -MARIOS
// Apo edw perasan kila iq
const string XOR_KEY = "Terry THE GOAT Davis";

/**
 * Read an integer choice between min and max (inclusive), reprompting on invalid input.
 */
int getChoice(int min, int max) {
    int c;
    while (true) {
        if (cin >> c && c >= min && c <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return c;
        }
        cout << "Invalid choice. Please enter a number between " << min << " and " << max << ": ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

/**
 * XOR‐encrypts (and decrypts) a string, returning a hex-encoded result.
 */
string encryptPassword(const string& plain) {
    stringstream ss;
    ss << hex << setfill('0');
    for (size_t i = 0; i < plain.size() && i < 49; ++i) {
        unsigned char ec = plain[i] ^ XOR_KEY[i % XOR_KEY.size()];
        ss << setw(2) << int(ec);
    }
    return ss.str();
}

/**
 * Decodes a hex string and XOR‐decrypts it.
 */
string decryptPassword(const string& hexed) {
    string result;
    for (size_t i = 0; i + 1 < hexed.size(); i += 2) {
        string byteStr = hexed.substr(i, 2);
        unsigned char ec = static_cast<unsigned char>(stoi(byteStr, nullptr, 16));
        char dc = ec ^ XOR_KEY[(i/2) % XOR_KEY.size()];
        result.push_back(dc);
    }
    return result;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int loadPasswords() {
    ifstream file(FILENAME, ios::binary);
    passwords.clear();
    if (!file.is_open()) return 0;
    Password temp;
    while (file.read(reinterpret_cast<char*>(&temp), sizeof(temp))) {
        passwords.push_back(temp);
    }
    file.close();
    return passwords.size();
}

void savePasswords() {
    ofstream file(FILENAME, ios::binary);
    if (!file.is_open()) {
        cout << "Unable to open file for saving.\n";
        return;
    }
    for (auto& p : passwords) {
        file.write(reinterpret_cast<const char*>(&p), sizeof(p));
    }
    file.close();
}

void viewPasswords() {
    loadPasswords();
    clearScreen();
    cout << passwords.size() << " Total passwords:\n";
    cout << "------------------------------------\n";
    for (size_t i = 0; i < passwords.size(); ++i) {         //karipidi view
        string encryptedHex(passwords[i].password);
        string plain = decryptPassword(encryptedHex);
        cout << (i + 1) << ". " 
             << setw(15) << left << passwords[i].passwordType
             << ": " << plain << "\n";
    }
    cout << "------------------------------------\n";
}

void addPassword() {
    loadPasswords();
    clearScreen();

    cout << "Add New Password\n";
    cout << "1. Custom input\n";
    cout << "2. Generate random\n";
    cout << "3. Back\n";
    cout << "Choice: ";
    int choice = getChoice(1, 3);
    if (choice == 3) return;

    Password newPass;
    cout << "Password Type: ";
    cin.getline(newPass.passwordType, sizeof(newPass.passwordType));

    string plain;
    if (choice == 1) {
        cout << "Password (max 49 chars): ";
        getline(cin, plain);
        if (plain.size() > 49) {
            cout << "Too long—truncating to 49 characters.\n";
            plain.resize(49);
        }
    } else {
        int length;
        cout << "Desired length (max 49): ";
        length = getChoice(1, 49);
        const string charset =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789"
            "!@#$%^&*()_-+=<>?";
        srand(static_cast<unsigned>(time(nullptr)));
        for (int i = 0; i < length; ++i) {
            plain += charset[rand() % charset.size()];
        }
        cout << "Generated password: " << plain << "\n";
    }

    string hexed = encryptPassword(plain);
    memset(newPass.password, 0, sizeof(newPass.password));
    strncpy(newPass.password, hexed.c_str(), sizeof(newPass.password) - 1);

    passwords.push_back(newPass);
    savePasswords();
    cout << "Password stored encrypted.\n";
}

int searchPassword(const string& type) {
    for (size_t i = 0; i < passwords.size(); ++i) {
        if (type == passwords[i].passwordType) return i;
    }
    return -1;
}

void deletePassword() {
    loadPasswords();
    if (passwords.empty()) {
        clearScreen();
        cout << "No passwords.\n";
        return;
    }
    viewPasswords();
    cout << "Type to delete: ";
    string type;
    getline(cin, type);

    int idx = searchPassword(type);
    if (idx >= 0) {
        passwords.erase(passwords.begin() + idx);
        savePasswords();
        clearScreen();
        cout << "Deleted.\n";
    } else {
        cout << "Not found.\n";
    }
}

void editPassword() {
    loadPasswords();
    if (passwords.empty()) {
        clearScreen();
        cout << "No passwords.\n";
        return;
    }
    viewPasswords();
    cout << "Type to edit: ";
    string type;
    getline(cin, type);

    int idx = searchPassword(type);
    if (idx < 0) {
        cout << "Not found.\n";
        return;
    }

    clearScreen();
    cout << "1. Custom input\n";
    cout << "2. Generate random\n";
    cout << "3. Back\n";
    cout << "Choice: ";
    int choice = getChoice(1, 3);
    if (choice == 3) return;

    string plain;
    if (choice == 1) {
        cout << "New password (max 49): ";
        getline(cin, plain);
        if (plain.size() > 49) plain.resize(49);
    } else {
        int length;
        cout << "Desired length (max 49): ";
        length = getChoice(1, 49);
        const string charset =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789"
            "!@#$%^&*()_-+=<>?";
        srand(static_cast<unsigned>(time(nullptr)));
        for (int i = 0; i < length; ++i) {
            plain += charset[rand() % charset.size()];
        }
        cout << "Generated: " << plain << "\n";
    }

    string hexed = encryptPassword(plain);
    memset(passwords[idx].password, 0, sizeof(passwords[idx].password));
    strncpy(passwords[idx].password, hexed.c_str(), sizeof(passwords[idx].password) - 1);
    savePasswords();

    cout << "Password updated.\n";
}

void showMenu() {
    clearScreen();
    cout << "PASSWORD MANAGER\n"
            "----------------\n"
            "1. View Passwords\n"
            "2. Add New Password\n"
            "3. Generate Password\n"
            "4. Edit Password\n"
            "5. Delete Password\n"
            "6. Exit\n"
            "----------------\n"
            "Enter your choice: ";
}

// MHN TO SVISI KANIS THA TO GAMISO :) -MARIOS
// kanei optimize ton kodika kai trexei kalutera 

void showTempleOSBanner() {
    cout << "\033[1;36m"; // Light blue for "Temple"
    cout << R"(
   __                                        __           ______    ______  
  /  |                                      /  |          /      \  /      \ 
 _$$ |_     ______   _____  ____    ______  $$ |  ______ /$$$$$$  |/$$$$$$  |
/ $$   |   /      \ /     \/    \  /      \ $$ | /      \ $$ |  $$ |$$ \__$$/ 
$$$$$$/   /$$$$$$  |$$$$$$ $$$$  |/$$$$$$  |$$ |/$$$$$$  |$$ |  $$ |$$      \ 
  $$ | __ $$    $$ |$$ | $$ | $$ |$$ |  $$ |$$ |$$    $$ |$$ |  $$ | $$$$$$  |
  $$ |/  |$$$$$$$$/ $$ | $$ | $$ |$$ |__$$ |$$ |$$$$$$$$/ $$ \__$$ |/  \__$$ |
  $$  $$/ $$       |$$ | $$ | $$ |$$    $$/ $$ |$$       |$$    $$/ $$    $$/ 
   $$$$/   $$$$$$$/ $$/  $$/  $$/ $$$$$$$/  $$/  $$$$$$$/  $$$$$$/   $$$$$$/  
                                  $$ |                                      
                                  $$ |                                      
                                  $$/       
)";

    cout << "\033[0m"; // Reset color

    // Wait 8 seconds
    this_thread::sleep_for(chrono::seconds(8));
}
int main() {
	showTempleOSBanner();
    srand(static_cast<unsigned>(time(nullptr)));
    int choice;
    do {
        showMenu();
        choice = getChoice(1, 6);
        switch (choice) {
            case 1: viewPasswords();    break;
            case 2: addPassword();     break;
            case 3: addPassword();     break;  // Generate is inside addPassword
            case 4: editPassword();    break;
            case 5: deletePassword();  break;
            case 6: clearScreen(); cout << "Goodbye!\n"; break;
        }
        if (choice != 6) {
            cout << "Press Enter to continue...";
            cin.get();
        }
    } while (choice != 6);
    return 0;
}
//Pomakos was here (MegasPaisios)
//BigKaris was here also