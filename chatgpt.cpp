#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> // For getch() on Windows
#include <unordered_map>

using namespace std;

class User {
private:
    string username, password, city;
    string cities[5] = {"Ahmedabad", "Surat", "Nadiad", "Vadodara", "Rajkot"};
    unordered_map<string, string> users; // Store usernames and hashed passwords

    void loadUsers() {
        ifstream file("users.txt");
        string storedUser, storedPass, storedCity;
        while (file >> storedUser >> storedPass) {
            file.ignore();
            getline(file, storedCity);
            users[storedUser] = storedPass;
        }
        file.close();
    }

    bool userExists(const string& user) {
        return users.find(user) != users.end();
    }

    string getPassword() {
        string pass;
        char ch;
        cout << "Enter password: ";
        while (true) {
            ch = _getch();
            if (ch == '\r') break;
            if (ch == '\b' && !pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            } else if (ch != '\b') {
                pass.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        return pass;
    }

    string hashPassword(const string& pass) {
        hash<string> hasher;
        return to_string(hasher(pass)); // Convert hashed value to string
    }

public:
    User() { loadUsers(); }

    void displayCities() {
        cout << "Select your city:\n";
        for (int i = 0; i < 5; i++) {
            cout << i + 1 << ". " << cities[i] << endl;
        }
    }

    void signUp() {
        system("CLS");
        while (true) {
            cout << "Enter a new username: ";
            cin >> username;
            if (userExists(username)) {
                cout << "Username already exists! Try another one.\n";
            } else break;
        }

        password = hashPassword(getPassword());

        displayCities();
        int choice;
        cout << "Enter your choice (1-5): ";
        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << "Invalid input! Enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        city = cities[choice - 1];
        ofstream file("users.txt", ios::app);
        file << username << " " << password << " " << city << endl;
        file.close();
        users[username] = password;

        cout << "Sign-up successful! You can now log in.\n";
        login();
    }

    void login() {
        system("CLS");
        int attempts = 3;
        while (attempts--) {
            cout << "Enter your username: ";
            cin >> username;
            password = hashPassword(getPassword());

            ifstream file("users.txt");
            string storedUser, storedPass, storedCity;
            bool loginSuccess = false;
            while (file >> storedUser >> storedPass) {
                file.ignore();
                getline(file, storedCity);
                if (storedUser == username && storedPass == password) {
                    city = storedCity;
                    loginSuccess = true;
                    break;
                }
            }
            file.close();

            if (loginSuccess) {
                system("CLS");
                cout << "Login successful! Welcome, " << username << ".\n";
                cout << "Your registered city is: " << city << ".\n";
                return;
            } else {
                cout << "Incorrect username or password. Attempts left: " << attempts << "\n";
            }
        }
        cout << "Too many failed attempts! Exiting...\n";
    }
};

int main() {
    User user;
    int choice;
    while (true) {
        cout << "\n\n|| If you are a new user, please sign up first! ||";
        cout << "\n1. Sign Up\n2. Login\n3. Exit\nEnter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: user.signUp(); break;
            case 2: user.login(); break;
            case 3: cout << "Exiting program...\n"; return 0;
            default: cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
