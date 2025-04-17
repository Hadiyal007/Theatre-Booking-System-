#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <sstream>
#include <iomanip>
#include <cctype>

using namespace std;

class User {
private:
    string username;
    string password;
    string city;
    bool loggedin = false;
    vector<string> cities = {"Ahmedabad", "Surat", "Nadiad", "Vadodara", "Rajkot"};
    vector<char> bookedSeats;
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
            ch = _getch();
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
        bookedSeats.assign(totalSeats, 0);
        string filename = getSeatFileName(movieName);
        ifstream file(filename, ios::binary);
        if (file) {
            file.read(reinterpret_cast<char*>(bookedSeats.data()), totalSeats * sizeof(char));
            file.close();
        }
    }

    void saveSeatsForMovie(const string& movieName) {
        string filename = getSeatFileName(movieName);
        ofstream file(filename, ios::binary);
        if (file) {
            file.write(reinterpret_cast<const char*>(bookedSeats.data()), totalSeats * sizeof(char));
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

    bool isValidSeat(const string& seatLabel) {
        if (seatLabel.length() < 2) return false;
        
        char rowChar = toupper(seatLabel[0]);
        if (rowChar < 'A' || rowChar >= 'A' + rows) return false;
        
        try {
            int seatNum = stoi(seatLabel.substr(1));
            if (seatNum < 1 || seatNum > seatsPerRow) return false;
        } catch (...) {
            return false;
        }
        
        return true;
    }

public:
    User() : bookedSeats(totalSeats, 0) {}

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

    bool bookTicket(const string& seatLabel) {
        if (!isValidSeat(seatLabel)) {
            cout << "Invalid seat number format. Please use format like A1, B2, etc." << endl;
            return false;
        }
    
        int row = toupper(seatLabel[0]) - 'A';
        int seat = stoi(seatLabel.substr(1)) - 1;
        int seatIndex = row * seatsPerRow + seat;
    
        if (bookedSeats[seatIndex]) {
            cout << "Seat " << seatLabel << " is already booked. Please choose another seat." << endl;
            return false;
        }
    
        bookedSeats[seatIndex] = 1;
        saveSeatsForMovie(currentMovie);
        saveBooking();
        return true;
    }

    bool removeTicket(const string& seatLabel) {
        if (!isValidSeat(seatLabel)) {
            cout << "Invalid seat number format." << endl;
            return false;
        }
    
        int row = toupper(seatLabel[0]) - 'A';
        int seat = stoi(seatLabel.substr(1)) - 1;
        int seatIndex = row * seatsPerRow + seat;
    
        if (!bookedSeats[seatIndex]) {
            cout << "No booking found for seat " << seatLabel << "." << endl;
            return false;
        }
    
        bookedSeats[seatIndex] = 0;
        saveSeatsForMovie(currentMovie);
        saveBooking();
        return true;
    }

    void displayBill() {
        int totalBooked = count(bookedSeats.begin(), bookedSeats.end(), 1);
        double totalAmount = totalBooked * 10.0;
        cout << "\n----- Booking Summary -----\n";
        cout << "Movie: " << currentMovie << endl;
        cout << "Tickets booked: " << totalBooked << endl;
        cout << "Total amount: $" << fixed << setprecision(2) << totalAmount << endl;
        cout << "--------------------------\n";
    }

    bool isLoggedIn() const {
        return loggedin;
    }

    void homepage() {
        while (true) {
            system("CLS");
            cout << "\n***** Movie Ticket Booking System *****\n";
            cout << "1. Book Tickets\n";
            cout << "2. Cancel Tickets\n";
            cout << "3. View Booking Summary\n";
            cout << "4. Logout\n";
            cout << "***************************************\n";

            int choice;
            cout << "Enter your choice (1-4): ";
            cin >> choice;

            switch (choice) {
                case 1: bookTicketMenu(); break;
                case 2: removeTicketMenu(); break;
                case 3: displayBill(); break;
                case 4: return;
                default: cout << "Invalid choice! Try again.\n";
            }
        }
    }

    void bookTicketMenu() {
        system("CLS");
        cout << "Available Movies:\n";
        for (size_t i = 0; i < movies.size(); i++) {
            cout << i + 1 << ". " << movies[i] << "\n";
        }
        cout << movies.size() + 1 << ". Back to Main Menu\n";

        int choice;
        cout << "Select a movie (1-" << movies.size() + 1 << "): ";
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
            "Prabhas, Rana Daggubati...",
            "Allu Arjun, Rashmika Mandana...",
            "Yash, Sreenidhi Sheti...",
            "Robert Downey Jr., Chris Evans...",
            "Anthony Mackie, Harrison Ford..."
        };

        cout << "\n***** Movie Details *****\n";
        cout << "Title: " << movies[movieIndex] << "\n";
        cout << "Synopsis: " << details[movieIndex] << "\n";
        cout << "Cast: " << casts[movieIndex] << "\n";
        cout << "************************\n";

        int choice;
        cout << "1. Book Tickets\n";
        cout << "2. Back to Movies\n";
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
            cout << "How many tickets do you want to book? (0 to cancel): ";
            cin >> ticketCount;
    
            if (ticketCount == 0) return;
            if (ticketCount < 0) {
                cout << "Invalid number of tickets. Please try again.\n";
                continue;
            }
    
            int ticketsBooked = 0;
            while (ticketsBooked < ticketCount) {
                system("CLS");
                showSeatingLayout();
                
                cout << "Booking ticket " << ticketsBooked + 1 << " of " << ticketCount << endl;
                string seatLabel;
                cout << "Enter seat number (e.g., A1, B2): ";
                cin >> seatLabel;
    
                if (bookTicket(seatLabel)) {
                    ticketsBooked++;
                    system("CLS");
                    showSeatingLayout();
                    cout << "Successfully booked seat " << seatLabel << "!\n";
                    
                    // If more tickets to book, show intermediate message
                    if (ticketsBooked < ticketCount) {
                        cout << "\nPlease book the next ticket...\n";
                        cout << "Press any key to continue...";
                        _getch();
                    }
                } else {
                    cout << "Failed to book seat " << seatLabel << ". Please try again.\n";
                    cout << "Press any key to continue...";
                    _getch();
                }
            }
            
            if(ticketCount == 1) {
                cout << "\nSuccessfully booked " << ticketsBooked << " ticket!\n";
            } else {
                cout << "\nSuccessfully booked " << ticketsBooked << " tickets!\n";
            }
            
            cout << "1. Book more tickets\n";
            cout << "2. Back to Main Menu\n";
            cout << "Enter your choice (1-2): ";
            
            int choice;
            cin >> choice;
            if (choice == 2) return;
        }
    }
    void removeTicketMenu() {
        while (true) {
            system("CLS");
            showSeatingLayout();
            
            string seatLabel;
            cout << "Enter seat number to cancel (e.g., A1, B2) or 0 to go back: ";
            cin >> seatLabel;
    
            if (seatLabel == "0") return;
            
            // Store the current state to check if the seat was actually booked
            int row = toupper(seatLabel[0]) - 'A';
            int seat = stoi(seatLabel.substr(1)) - 1;
            int seatIndex = row * seatsPerRow + seat;
            bool wasBooked = bookedSeats[seatIndex];
            
            removeTicket(seatLabel);
            
            // Only refresh if the seat was actually booked and now removed
            if (wasBooked && !bookedSeats[seatIndex]) {
                system("CLS");
                showSeatingLayout();
                cout << "Seat " << seatLabel << " has been successfully canceled.\n";
            }
    
            cout << "\n1. Cancel another ticket\n";
            cout << "2. Back to Main Menu\n";
            cout << "Enter your choice (1-2): ";
            
            int choice;
            cin >> choice;
            if (choice == 2) return;
        }
    }
};

int main() {
    User user;
    while (true) {
        system("CLS");
        cout << "***** Welcome to Movie Ticket Booking *****\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "******************************************\n";

        int choice;
        cout << "Enter your choice (1-3): ";
        cin >> choice;

        switch (choice) {
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
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
        
        cout << "\nPress any key to continue...";
        _getch();
    }

    return 0;
}