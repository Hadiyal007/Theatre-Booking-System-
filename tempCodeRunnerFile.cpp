#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <sstream>
#include <iomanip>

using namespace std;

class User {
private:
    string username;
    string password;
    string city;
    bool loggedin = false;
    vector<string> cities = {"Ahmedabad", "Surat", "Nadiad", "Vadodara", "Rajkot"};
    vector<bool> bookedSeats; // To track booked seats
    const int rows = 10;
    const int seatsPerRow = 10;
    const int totalSeats = rows * seatsPerRow;
    string currentMovie;
    vector<string> movies = {
        "Chhava", "Bahubali 2", "Pushpa 2", 
        "KGF 2", "Avengers Endgame", "Captain America: Brave New World"
    };

    bool userExists(const string& user) {
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
            ch = getch();
            if (ch == '\r') break;
            if (ch == '\b' && !pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            } else if (ch >= 32 && ch <= 126) {
                pass += ch;
                cout << '*';
            }
        }
        cout << endl;
        return pass;
    }

    string getSeatFileName(const string& movieName) {
        return "seats_" + movieName + ".dat";
    }

    void loadSeatsForMovie(const string& movieName) {
        bookedSeats.assign(totalSeats, false);
        string filename = getSeatFileName(movieName);
        ifstream file(filename, ios::binary);
        if (file) {
            file.read(reinterpret_cast<char*>(bookedSeats.data()), totalSeats * sizeof(bool));
            file.close();
        }
    }

    void saveSeatsForMovie(const string& movieName) {
        string filename = getSeatFileName(movieName);
        ofstream file(filename, ios::binary);
        if (file) {
            file.write(reinterpret_cast<const char*>(bookedSeats.data()), totalSeats * sizeof(bool));
            file.close();
        }
    }

    void saveBooking() {
        ofstream bookingFile("bookings.txt", ios::app);
        if (bookingFile.is_open()) {
            bookingFile << username << " " << currentMovie << " ";
            for (int i = 0; i < totalSeats; ++i) {
                if (bookedSeats[i]) {
                    bookingFile << static_cast<char>('A' + (i / seatsPerRow)) << (i % seatsPerRow + 1) << " ";
                }
            }
            bookingFile << endl;
            bookingFile.close();
        }
    }

public:
    User() : bookedSeats(totalSeats, false) {}

    void registerUser() {
        cout << "Enter username: ";
        cin >> username;
        if (userExists(username)) {
            cout << "Username already exists. Please choose a different one." << endl;
            return;
        }
        password = getPassword();
        cout << "Select your city (0: Ahmedabad, 1: Surat, 2: Nadiad, 3: Vadodara, 4: Rajkot): ";
        int cityIndex;
        cin >> cityIndex;
        if (cityIndex < 0 || cityIndex >= cities.size()) {
            cout << "Invalid city selection." << endl;
            return;
        }
        city = cities[cityIndex];

        ofstream file("users.txt", ios::app);
        file << username << " " << password << endl << city << endl;
        cout << "Registration successful!" << endl;
    }

    void loginUser() {
        cout << "Enter username: ";
        cin >> username;
        if (!userExists(username)) {
            cout << "User does not exist." << endl;
            return;
        }
        password = getPassword();
        ifstream file("users.txt");
        string storedUser, storedPass, storedCity;
        while (file >> storedUser >> storedPass) {
            file.ignore();
            getline(file, storedCity);
            if (storedUser == username && storedPass == password) {
                loggedin = true;
                city = storedCity;
                cout << "Login successful!" << endl;
                return;
            }
        }
        cout << "Invalid credentials." << endl;
    }

    void showSeatingLayout() {
        cout << "Seating arrangement for " << currentMovie << ":\n";
        cout << "-----------------------------------\n";
        cout << "   ";
        for (int i = 1; i <= seatsPerRow; ++i) {
            cout << " " << setw(2) << i << " ";
        }
        cout << endl;

        for (int row = 0; row < rows; ++row) {
            cout << static_cast<char>('A' + row) << " | ";
            for (int seat = 0; seat < seatsPerRow; ++seat) {
                int seatIndex = row * seatsPerRow + seat;
                cout << (bookedSeats[seatIndex] ? "[X]" : "[ ]") << " ";
            }
            cout << endl;
        }
        cout << "-----------------------------------\n";
    }

    void bookTicket(const string& seatLabel) {
        if (seatLabel.length() < 2) {
            cout << "Invalid seat number format." << endl;
            return;
        }

        int row = toupper(seatLabel[0]) - 'A';
        int seat;
        try {
            seat = stoi(seatLabel.substr(1)) - 1;
        } catch (...) {
            cout << "Invalid seat number." << endl;
            return;
        }

        if (row < 0 || row >= rows || seat < 0 || seat >= seatsPerRow) {
            cout << "Invalid seat number." << endl;
            return;
        }

        int seatIndex = row * seatsPerRow + seat;
        if (bookedSeats[seatIndex]) {
            cout << "Seat already booked." << endl;
            return;
        }
        bookedSeats[seatIndex] = true;
        cout << "Ticket booked for seat " << seatLabel << "." << endl;
        saveSeatsForMovie(currentMovie);
        saveBooking();
    }

    void removeTicket(const string& seatLabel) {
        if (seatLabel.length() < 2) {
            cout << "Invalid seat number format." << endl;
            return;
        }

        int row = toupper(seatLabel[0]) - 'A';
        int seat;
        try {
            seat = stoi(seatLabel.substr(1)) - 1;
        } catch (...) {
            cout << "Invalid seat number." << endl;
            return;
        }

        if (row < 0 || row >= rows || seat < 0 || seat >= seatsPerRow) {
            cout << "Invalid seat number." << endl;
            return;
        }

        int seatIndex = row * seatsPerRow + seat;
        if (!bookedSeats[seatIndex]) {
            cout << "No booking found for seat " << seatLabel << "." << endl;
            return;
        }
        bookedSeats[seatIndex] = false;
        cout << "Booking removed for seat " << seatLabel << "." << endl;
        saveSeatsForMovie(currentMovie);
        saveBooking();
    }

    void displayBill() {
        int totalBooked = count(bookedSeats.begin(), bookedSeats.end(), true);
        double totalAmount = totalBooked * 10.0;
        cout << "Total tickets booked for " << currentMovie << ": " << totalBooked << endl;
        cout << "Total amount: $" << totalAmount << endl;
    }

    bool isLoggedIn() const {
        return loggedin;
    }

    void homepage() {
        system("CLS");
        int choice;
        cout << "\n********************\n";
        cout << "1. Book Ticket\n";
        cout << "2. Remove Ticket\n";
        cout << "3. View Bill\n";
        cout << "4. Exit\n";
        cout << "********************\n";
        while (true) {
            cout << "Enter your choice (1-4): ";
            cin >> choice;
            switch (choice) {
                case 1: bookTicketMenu(); break;
                case 2: removeTicketMenu(); break;
                case 3: displayBill(); break;
                case 4: exit(0);
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }

    void bookTicketMenu() {
        system("CLS");
        cout << "Select a movie to book tickets:\n";
        for (size_t i = 0; i < movies.size(); i++) {
            cout << i + 1 << ". " << movies[i] << "\n";
        }
        cout << movies.size() + 1 << ". Go Back\n";

        int choice;
        cout << "Enter your choice (1-" << movies.size() + 1 << "): ";
        cin >> choice;

        if (choice > 0 && choice <= movies.size()) {
            currentMovie = movies[choice - 1];
            loadSeatsForMovie(currentMovie);
            showMovieDetails(choice - 1);
        }
    }

    void showMovieDetails(int movieIndex) {
        system("CLS");
        vector<string> details = {
            "The film explores the period after the death of Chhatrapati Shivaji Maharaj...",
            "The Baahubali movie series follows the story of Shivudu...",
            "Pushpa: The Rise follows Pushpa Raj...",
            "The K.G.F movie series follows Rocky...",
            "In 2018, 23 days after Thanos erased half of all life...",
            "In Captain America: Brave New World, Sam Wilson..."
        };
        vector<string> casts = {
            "Vicky Kaushal, Rashmika Mandanna...",
            "Prabhas, Rana Daggubatti...",
            "Allu Arjun, Rashmika Mandana...",
            "Yash, Sreenidhi Sheti...",
            "Robert Downey Jr., Chris Evans...",
            "Anthony Mackie, Harrison Ford..."
        };

        cout << "\n********************\n";
        cout << "Movie Name: " << movies[movieIndex] << "\n";
        cout << "Movie Details: " << details[movieIndex] << "\n";
        cout << "Movie Cast: " << casts[movieIndex] << "\n";
        cout << "********************\n";

        int choice;
        cout << "1. Book tickets\n";
        cout << "2. Go back\n";
        cout << "Enter your choice (1-2): ";
        cin >> choice;

        if (choice == 1) {
            showSeating();
        }
    }

    void showSeating() {
        while (true) {
            system("CLS");
            showSeatingLayout();
            int ticketCount;
            cout << "How many tickets do you want to book? ";
            cin >> ticketCount;

            if (ticketCount <= 0) {
                cout << "Invalid number of tickets. Please try again.\n";
                continue;
            }

            for (int i = 0; i < ticketCount; ++i) {
                string seatLabel;
                cout << "Enter seat number to book (e.g., A1, B2): ";
                cin >> seatLabel;
                bookTicket(seatLabel);
                system("CLS");
                showSeatingLayout();
            }

            int choice;
            cout << "1. Book more tickets\n";
            cout << "2. Go to homepage\n";
            cout << "Enter your choice (1-2): ";
            cin >> choice;

            if (choice == 2) {
                system("CLS");
                homepage();
                return;
            }
        }
    }

    void removeTicketMenu() {
        while (true) {
            system("CLS");
            showSeatingLayout();
            string seatLabel;
            cout << "Enter seat number to remove booking (e.g., A1, B2) or 0 to go back: ";
            cin >> seatLabel;

            if (seatLabel == "0") {
                return;
            }

            removeTicket(seatLabel);
        }
    }
};

int main() {
    User user;
    int option;

    while (true) {
        system("CLS");
        cout << "Welcome to the Movie Ticket Booking System\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice (1-3): ";
        cin >> option;

        switch (option) {
            case 1:
                user.registerUser();
                break;
            case 2:
                user.loginUser();
                if (user.isLoggedIn()) {
                    user.homepage();
                }
                break;
            case 3:
                exit(0);
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}