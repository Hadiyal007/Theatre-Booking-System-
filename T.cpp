#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <ctime>

using namespace std;

class User {
private:
    string username;
    string password;
    string city;
    bool loggedin = false;
    vector<string> cities = {"Ahmedabad", "Surat", "Nadiad", "Vadodara", "Rajkot", "Anand", "Sabarkantha", "Surendranagar"};
    vector<char> bookedSeats;
    const int rows = 10;
    const int seatsPerRow = 10;
    const int totalSeats = rows * seatsPerRow;
    string currentMovie;
    vector<string> movies = {
        "Chhava", "Bahubali 2", "Pushpa 2", 
        "KGF 2", "Avengers Endgame", "Captain America: Brave New World"
    };
    time_t lastBookingTime;
    time_t paymentTime;

    int getValidInput(int min, int max) {
        int choice;
        while (true) {
            cin >> choice;
            if (cin.fail() || choice < min || choice > max) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number between " << min << " and " << max << ": ";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }

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
        
        time_t currentTime = time(nullptr);
        if (difftime(currentTime, lastBookingTime) >= 3 * 3600) {
            resetSeats();
        }
    }

    void resetSeats() {
        bookedSeats.assign(totalSeats, 0);
        cout << "All seats have been reset after 3 hours.\n";
        saveSeatsForMovie(currentMovie);
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
            time_t now = time(nullptr);
            bookingFile << username << " " << currentMovie << " " << now << " ";
            for (int i = 0; i < totalSeats; ++i) {
                if (bookedSeats[i]) {
                    bookingFile << static_cast<char>('A' + (i / seatsPerRow)) << (i % seatsPerRow + 1) << " ";
                }
            }
            bookingFile << endl;
            bookingFile.close();
            lastBookingTime = now;
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

    void deleteOldBookings() {
        time_t currentTime = time(nullptr);
        ifstream bookingFile("bookings.txt");
        if (!bookingFile) return;

        vector<string> linesToKeep;
        string line;
        
        while (getline(bookingFile, line)) {
            istringstream iss(line);
            string user, movie;
            iss >> user >> movie;
            
            // Only keep bookings that aren't for this user or are recent
            if (user != username || difftime(currentTime, paymentTime) < 3 * 3600) {
                linesToKeep.push_back(line);
            }
        }
        bookingFile.close();

        // Rewrite the file with only the lines we want to keep
        ofstream outFile("bookings.txt");
        for (const auto& l : linesToKeep) {
            outFile << l << endl;
        }
    }

public:
    User() : bookedSeats(totalSeats, 0), lastBookingTime(0), paymentTime(0) {}

    void registerUser() {
        cout << "Enter username: ";
        cin >> username;
        if (userExists(username)) {
            cout << "Username already exists. Please choose a different one." << endl;
            return;
        }
        password = getPassword();
        
        cout << "Select your city:\n";
        for (size_t i = 0; i < cities.size(); i++) {
            cout << i << ": " << cities[i] << "\n";
        }
        cout << "Enter city choice (0-" << cities.size()-1 << "): ";
        int cityIndex = getValidInput(0, cities.size()-1);
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

    bool isLoggedIn() const {
        return loggedin;
    }

    void homepage() {
        if (!currentMovie.empty()) {
            loadSeatsForMovie(currentMovie);
        }
    
        while (true) {
            system("CLS");
            cout << "\n----- Movie Ticket Booking System ----- \n";
            cout << "***************************************\n";
            cout << "1. Book Tickets\n";
            cout << "2. Cancel Tickets\n";
            cout << "3. View Booking Summary\n";
            cout << "4. Logout\n";
            cout << "***************************************\n";
    
            cout << "Enter your choice (1-4): ";
            int choice = getValidInput(1, 4);
    
            switch (choice) {
                case 1: 
                    bookTicketMenu(); 
                    break;
                case 2: 
                    loadUserBookings();
                    removeTicketMenu(); 
                    break;
                case 3: 
                    displayBill(); 
                    break;
                case 4: 
                    return;
            }
        }
    }

    void loadUserBookings() {
        ifstream bookingFile("bookings.txt");
        if (!bookingFile) return;
    
        string line;
        while (getline(bookingFile, line)) {
            istringstream iss(line);
            string user, movie;
            iss >> user >> movie;
            
            if (user == username) {
                currentMovie = movie;
                loadSeatsForMovie(currentMovie);
                break;
            }
        }
        bookingFile.close();
    
        if (currentMovie.empty()) {
            cout << "No bookings found for this user.\n";
            cout << "Press any key to continue...";
            _getch();
        }
    }

    void bookTicketMenu() {
        while (true) {
            system("CLS");
            cout << "Available Movies:\n";
            cout << "************************************\n";
            for (size_t i = 0; i < movies.size(); i++) {
                cout << i + 1 << ". " << movies[i] << "\n";
            }
            cout << movies.size() + 1 << ". Back to Main Menu\n";
            cout << "************************************\n";
            
            cout << "Select a movie (1-" << movies.size() + 1 << "): ";
            int choice = getValidInput(1, movies.size() + 1);

            if (choice == movies.size() + 1) return;
            
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

        cout << "\n----- Movie Details -----\n";
        cout << "****************************\n";
        cout << "Title: " << movies[movieIndex] << "\n";
        cout << "Synopsis: " << details[movieIndex] << "\n";
        cout << "Cast: " << casts[movieIndex] << "\n";
        cout << "****************************\n";

        cout << "1. Book Tickets\n";
        cout << "2. Back to Movies\n";
        cout << "Enter your choice (1-2): ";
        int choice = getValidInput(1, 2);

        if (choice == 1) {
            showSeating();
        }
    }

    void showSeating() {
        while (true) {
            system("CLS");
            showSeatingLayout();
            
            cout << "How many tickets do you want to book? (0 to cancel): ";
            int ticketCount = getValidInput(0, totalSeats);
    
            if (ticketCount == 0) return;
    
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
            
            int choice = getValidInput(1, 2);
            if (choice == 2) return;
        }
    }

    void removeTicketMenu() {
        if (currentMovie.empty()) {
            cout << "You don't have any bookings to cancel.\n";
            cout << "Press any key to continue...";
            _getch();
            return;
        }
    
        while (true) {
            system("CLS");
            showSeatingLayout();
            
            string seatLabel;
            cout << "Enter seat number to cancel (e.g., A1, B2) or 0 to go back: ";
            cin >> seatLabel;

            if (seatLabel == "0") return;
            
            if (removeTicket(seatLabel)) {
                system("CLS");
                showSeatingLayout();
                cout << "Seat " << seatLabel << " has been successfully canceled.\n";
            }
    
            cout << "\n1. Cancel another ticket\n";
            cout << "2. Back to Main Menu\n";
            cout << "Enter your choice (1-2): ";
            
            int choice = getValidInput(1, 2);
            if (choice == 2) return;
        }
    }

    void displayBill() {
        system("CLS");
        
        ifstream bookingFile("bookings.txt");
        if (!bookingFile) {
            cout << "No bookings found!\n";
            cout << "Press any key to continue...";
            _getch();
            return;
        }    
    
        // Get current date and time
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char dateStr[11];
        char timeStr[9];
        strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", ltm);
        strftime(timeStr, sizeof(timeStr), "%H:%M:%S", ltm);
    
        // Structure to hold booking information
        struct BookingRecord {
            string movie;
            vector<string> seats;
            string bookingTime;
        };
    
        vector<BookingRecord> allBookings;
        string line;
        
        // Read all bookings for this user
        while (getline(bookingFile, line)) {
            istringstream iss(line);
            string user, movie, timestamp;
            
            // Read user, movie and timestamp
            if (!(iss >> user >> movie >> timestamp)) {
                continue;  // Skip malformed lines
            }
            
            if (user != username) {
                continue;  // Skip other users' bookings
            }
    
            // Read all seats
            vector<string> seats;
            string seat;
            while (iss >> seat) {
                seats.push_back(seat);
            }
    
            // Convert timestamp to readable time
            try {
                time_t bookingTime = stol(timestamp);
                tm *btm = localtime(&bookingTime);
                char bookingTimeStr[9];
                strftime(bookingTimeStr, sizeof(bookingTimeStr), "%H:%M:%S", btm);
                
                allBookings.push_back({movie, seats, bookingTimeStr});
            } catch (...) {
                continue;  // Skip entries with invalid timestamps
            }
        }
        bookingFile.close();
    
        if (allBookings.empty()) {
            cout << "No bookings found for this user.\n";
            cout << "Press any key to continue...";
            _getch();
            return;
        }
    
        // Sort bookings by movie name
        sort(allBookings.begin(), allBookings.end(), 
            [](const BookingRecord& a, const BookingRecord& b) {
                return a.movie < b.movie;
            });
    
        const double TICKET_PRICE = 110.0;
        double totalAmount = 0;
    
        // Display header
        system("CLS");
        cout << "============================================\n";
        cout << "              BOOKING SUMMARY               \n";
        cout << "============================================\n";
        cout << "  User: " << username << "\n";
        cout << "  City: " << city << "\n";
        cout << "  Date: " << dateStr << "\n";
        cout << "  Current Time: " << timeStr << "\n";
        cout << "--------------------------------------------\n";
        
        // Display bookings grouped by movie
        string currentMovie;
        int movieTickets = 0;
        double movieAmount = 0;
        bool firstMovie = true;
    
        for (const auto& booking : allBookings) {
            if (booking.movie != currentMovie) {
                // Print summary for previous movie
                if (!firstMovie) {
                    cout << "  TOTAL for " << currentMovie << ": " << movieTickets 
                         << " tickets, Rs." << fixed << setprecision(2) << movieAmount << "\n";
                    cout << "--------------------------------------------\n";
                }
                
                // Start new movie section
                currentMovie = booking.movie;
                movieTickets = 0;
                movieAmount = 0;
                firstMovie = false;
                
                cout << "  Movie: " << currentMovie << "\n";
                cout << "  ------------------------------------------\n";
            }
            
            // Display booking details
            cout << "  Booking Time: " << booking.bookingTime << "\n";
            cout << "  Seats: ";
            for (const auto& seat : booking.seats) {
                cout << seat << " ";
            }
            cout << "\n";
            cout << "  Tickets: " << booking.seats.size() << "\n";
            cout << "  Amount: Rs." << fixed << setprecision(2) 
                 << (booking.seats.size() * TICKET_PRICE) << "\n";
            cout << "  ------------------------------------------\n";
            
            // Update totals
            movieTickets += booking.seats.size();
            movieAmount += booking.seats.size() * TICKET_PRICE;
            totalAmount += booking.seats.size() * TICKET_PRICE;
        }
        
        // Print summary for the last movie
        if (!firstMovie) {
            cout << "  TOTAL for " << currentMovie << ": " << movieTickets 
                 << " tickets, Rs." << fixed << setprecision(2) << movieAmount << "\n";
            cout << "--------------------------------------------\n";
        }
        
        // Display grand total
        cout << "  GRAND TOTAL: Rs." << fixed << setprecision(2) << totalAmount << "\n";
        cout << "============================================\n";
        
        // Proceed to payment options
        cout << "\nPress any key to view payment options...";
        _getch();
    
        // Payment options menu
        while (true) {
            system("CLS");
            cout << "============================================\n";
            cout << "           SELECT PAYMENT METHOD           \n";
            cout << "============================================\n";
            cout << "1. Cash\n";
            cout << "2. Debit/Credit Card\n";
            cout << "3. Online Payment\n";
            cout << "4. Cancel Payment\n";
            cout << "============================================\n";
            cout << "Enter your choice (1-4): ";
            
            int paymentChoice = getValidInput(1, 4);
            
            string paymentMethod;
            switch (paymentChoice) {
                case 1: 
                    paymentMethod = "Cash";
                    cout << "\nPlease pay ₹" << totalAmount << " at the theater counter.\n";
                    break;
                case 2: 
                    paymentMethod = "Debit/Credit Card";
                    cout << "\nCard payment selected for Rupees" << totalAmount << "\n";
                    break;
                case 3: 
                    paymentMethod = "Online Payment";
                    cout << "\nOnline payment selected for Rupees" << totalAmount << "\n";
                    break;
                case 4: 
                    cout << "\nPayment cancelled.\n";
                    cout << "Press any key to continue...";
                    _getch();
                    return;
            }
        
            // Record payment time
            paymentTime = time(nullptr);
            
            // Display payment confirmation
            system("CLS");
            cout << "============================================\n";
            cout << "            PAYMENT CONFIRMATION           \n";
            cout << "============================================\n";
            cout << "  User: " << username << "\n";
            cout << "  Total Amount: Rs." << fixed << setprecision(2) << totalAmount << "\n";
            cout << "  Payment Method: " << paymentMethod << "\n";
            cout << "  Status: CONFIRMED\n";
            cout << "  Date: " << dateStr << "\n";
            cout << "  Time: " << timeStr << "\n";
            cout << "============================================\n";
            cout << "       Thank you for your payment!         \n";
            cout << "============================================\n";
            
            // Delete old bookings
            deleteOldBookings();
            
            cout << "\nPress any key to continue...";
            _getch();
            return;
        }
    }
};

int main() {
    User user;
    while (true) {
        system("CLS");
        cout << "----- Welcome to Movie Ticket Booking -----\n";
        cout << "*******************************************\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "*******************************************\n";

        cout << "Enter your choice (1-3): ";
        int choice;
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice! Please enter a number between 1-3.\n";
            cout << "Press any key to try again...";
            _getch();
            continue;
        }

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
        }
        
        cout << "\nPress any key to continue...";
        _getch();
    }

    return 0;
}