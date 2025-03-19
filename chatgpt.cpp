#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <conio.h> // For getch() on Windows
#include <stdlib.h>
using namespace std;

static int x = 1;
static int b = 1;

class MovieShow {
    string showName;
    int hour, minute;

public:
    MovieShow() {}

    void setShow(string name, int h, int m) {
        showName = name;
        hour = h;
        minute = m;
    }

    void displayShow(bool isTomorrow) {
        cout << showName << " - ";
        if (isTomorrow) {
            cout << "Tomorrow at ";
        } else {
            cout << "Today at ";
        }
        cout << hour << ":" << (minute < 10 ? "0" : "") << minute << endl;
    }

    bool isTomorrow(int currentHour, int currentMinute) {
        return (currentHour > hour || (currentHour == hour && currentMinute > minute));
    }

    int getTotalMinutes() const {
        return hour * 60 + minute;
    }
};

class Theatre {
    MovieShow morningShow, eveningShow, nightShow;
    int currentHour, currentMinute;

public:
    void getCurrentTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        currentHour = ltm->tm_hour;
        currentMinute = ltm->tm_min;

       
    }

    void initializeShows() {
        morningShow.setShow("Morning Show", 10, 0);
        eveningShow.setShow("Evening Show", 18, 0);
        nightShow.setShow("Night Show", 21, 0);
    }

    void displayAvailableShows() {
        cout << "\nAvailable Shows:\n";
        
        MovieShow shows[3] = {morningShow, eveningShow, nightShow};

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2 - i; ++j) {
                if (shows[j].getTotalMinutes() < shows[j + 1].getTotalMinutes()) {
                    swap(shows[j], shows[j + 1]);
                }
            }
        }

        for (int i = 0; i < 3; ++i) {
            shows[i].displayShow(shows[i].isTomorrow(currentHour, currentMinute));
        }
    }

    void showBookingProcess() {
        getCurrentTime();
        initializeShows();
        displayAvailableShows();
    }
};

class User {
private:
    string username;
    string password;
    string city;
    int loggedin = 0;

    string cities[5] = {"Ahmedabad", "Surat", "Nadiad", "Vadodara", "Rajkot"};
    Theatre theatre;  // Added Theatre object inside User class

    bool userExists(const string &user) {
        ifstream file("users.txt");
        if (!file) return false;

        string storedUser, storedPass, storedCity;
        while (file >> storedUser >> storedPass) {
            file.ignore();
            getline(file, storedCity);
            if (storedUser == user) {
                return true;
            }
        }
        return false;
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

public:
    void displayCities() {
        cout << "Select your city:\n";
        for (int i = 0; i < 5; i++) {
            cout << i + 1 << ". " << cities[i] << endl;
        }
    }

    void signUp() {
        while (true) {
            if (x) {
                system("CLS");
            }
            cout << "Enter a new username: ";
            cin >> username;

            if (userExists(username)) {
                x = 0;
                cout << "Username already exists! Try another one.\n";
            } else break;
        }

        password = getPassword();

        displayCities();
        int choice;
        cout << "Enter your choice (1-5): ";

        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cout << "Invalid input! Please enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        city = cities[choice - 1];

        ofstream file("users.txt", ios::app);
        if (!file) {
            cout << "Error opening file!\n";
            return;
        }

        file << username << " " << password << " " << city << endl;
        file.close();
        x = 0;
        system("CLS");
        cout << "Sign-up successful! You can now log in.\n";
        login();
    }

    void login() {
        if (x) {
            system("CLS");
        }
        cout << "Enter your username: ";
        cin >> username;

        ifstream file("users.txt");
        if (!file) {
            cout << "No users found! Please sign up first.\n";
            return;
        }
        password = getPassword();

        string storedUser, storedPass, storedCity;
        bool loginSuccess = false;

        while (file >> storedUser >> storedPass) {
            file.ignore();
            getline(file, storedCity);
            city = storedCity;
            if (storedUser == username && storedPass == password) {
                loginSuccess = true;
            }
        }
        file.close();

        if (loginSuccess) {
            system("CLS");
            cout << "Login successful! Welcome, " << username << ".\n";
            cout << "Your registered city is: " << city << ".\n";
            loggedin = 1;
        } else {
            cout << "Incorrect username or password. Try again.\n";
        }
    }

    int Loggedin() {
        return loggedin;
    }

    void homepage() {
        system("CLS");
        int choice;
        cout << "\n********************\n";
        cout << "1. Book Ticket\n";
        cout << "2. Remove Ticket\n";
        cout << "3. Bill\n";
        cout << "4. Exit\n";
        cout << "********************\n";
        while (true) {
            cout << "Enter your choice (1-4) : ";
            cin >> choice;
            switch (choice) {
                case 1: bookticket(); break;
                case 2: cout << "Remove ticket feature coming soon...\n"; break;
                case 3: cout << "Bill feature coming soon...\n"; break;
                case 4: exit(0);
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }

    void bookticket() {
        system("CLS");
        int choice;
        cout << "Which movie ticket do you want to book?\n";
        cout << "\n********************\n";
        cout << "1. Chhava\n";
        cout << "2. Bahubali 2\n";
        cout << "3. Pushpa 2\n";
        cout << "4. KGF 2\n";
        cout << "5. Avengers Endgame\n";
        cout << "6. Captain America: Brave New World\n";
        cout << "********************\n";
        cout << "If you want to go to the previous page, enter 0.\n";

        while (true) {
            cout << "Enter your choice (0-6): ";
            cin >> choice;
            if (choice == 0) homepage();
            else if (choice >= 1 && choice <= 6) {
                cout << "Timings for the movie:\n";
                theatre.showBookingProcess();
                break;
            } else {
                cout << "Invalid choice! Try again.\n";
            }
        }
    }
};

// Main Menu
int main() {
    User user;
    int choice;

    while (b) {
        if (user.Loggedin()) break;

        cout << "\n\n|| New user? Sign up first! ||";
        cout << "\n1. Sign Up\n2. Login\n3. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;
        switch (choice) {
            case 1: user.signUp(); break;
            case 2: user.login(); break;
            case 3: return 0;
            default: cout << "Invalid choice! Try again.\n";
        }
    }
    user.homepage();
    return 0;
}
