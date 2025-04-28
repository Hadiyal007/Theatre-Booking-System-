#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <map>
#include <limits>
#include <chrono>

using namespace std;
using namespace std::chrono;

class User {
private:
    string username;
    string password;
    string city;
    bool loggedin = false;
    vector<string> cities = {"Ahmedabad", "Surat", "Nadiad", "Vadodara", "Rajkot"};
    vector<bool> bookedSeats;
    const int rows = 10;
    const int seatsPerRow = 10;
    const int totalSeats = rows * seatsPerRow;
    string currentMovie;
    vector<string> movies = {
        "Chhava", "Bahubali 2", "Pushpa 2", 
        "KGF 2", "Avengers Endgame", "Captain America: Brave New World"
    };
    const string ADMIN_PASSWORD = "admin@1";

    struct Booking {
        string movie;
        vector<string> seats;
        string bookingTime;
        bool paid;
        time_t bookingTimestamp;
    };
    vector<Booking> userBookings;

    bool userExists(const string& user) {
        ifstream file("users.txt");
        if (!file.is_open()) return false;

        string storedUser, storedPass, storedCity;
        while (file >> storedUser >> storedPass) {
            file.ignore();
            getline(file, storedCity);
            if (storedUser == user) {
                file.close();
                return true;
            }
        }
        file.close();
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
        string sanitized;
        for (char c : movieName) {
            if (isalnum(c)) {
                sanitized += toupper(c);
            } else if (c == ' ') {
                sanitized += '_';
            }
        }
        return "seats_" + sanitized + ".dat";
    }

    void loadSeatsForMovie(const string& movieName) {
        bookedSeats.assign(totalSeats, false);
        string filename = getSeatFileName(movieName);
        ifstream file(filename, ios::binary);
        if (file.is_open()) {
            for (int i = 0; i < totalSeats; i++) {
                bool status;
                file.read(reinterpret_cast<char*>(&status), sizeof(bool));
                bookedSeats[i] = status;
            }
            file.close();
        }
    }

    void saveSeatsForMovie(const string& movieName) {
        string filename = getSeatFileName(movieName);
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            for (bool status : bookedSeats) {
                file.write(reinterpret_cast<const char*>(&status), sizeof(bool));
            }
            file.close();
        }
    }

    void saveAllBookings() {
        ofstream bookingFile("bookings.txt");
        if (bookingFile.is_open()) {
            for (const auto& booking : userBookings) {
                if (booking.seats.empty()) continue;
                
                bookingFile << username << "|" << booking.movie << "|" 
                          << booking.bookingTime << "|" 
                          << (booking.paid ? "1" : "0") << "|"
                          << booking.bookingTimestamp << "|";
                for (const auto& seat : booking.seats) {
                    bookingFile << seat << ",";
                }
                bookingFile << endl;
            }
            bookingFile.close();
        }
    }

    bool isValidSeat(const string& seatLabel) {
        if (seatLabel.length() < 2 || seatLabel.length() > 3) return false;
        
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

    string getCurrentDateTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
        return string(buf);
    }

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int getValidatedInput(int min, int max) {
        int input;
        while (true) {
            cin >> input;
            if (cin.fail() || input < min || input > max) {
                cout << "Invalid input. Please enter a number between " 
                     << min << " and " << max << ": ";
                clearInputBuffer();
            } else {
                clearInputBuffer();
                return input;
            }
        }
    }

    string getValidatedString(const string& prompt, bool isSeat = false) {
        string input;
        while (true) {
            cout << prompt;
            getline(cin, input);
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please try again.\n";
                continue;
            }
            
            if (input.empty()) {
                cout << "Input cannot be empty. Please try again.\n";
                continue;
            }
            
            if (isSeat && !isValidSeat(input)) {
                cout << "Invalid seat format. Please use format like A1, B2, etc.\n";
                continue;
            }
            
            return input;
        }
    }

    int countAvailableSeats() {
        return count(bookedSeats.begin(), bookedSeats.end(), false);
    }

    void checkAndResetExpiredBookings() {
        time_t now = time(0);
        vector<Booking> updatedBookings;
        
        for (const auto& booking : userBookings) {
            double hoursSinceBooking = difftime(now, booking.bookingTimestamp) / 3600.0;
            if (hoursSinceBooking < 4 || booking.paid) {
                updatedBookings.push_back(booking);
            } else {
                // Release the seats
                for (const auto& seat : booking.seats) {
                    int row = toupper(seat[0]) - 'A';
                    int seatNum = stoi(seat.substr(1)) - 1;
                    int seatIndex = row * seatsPerRow + seatNum;
                    bookedSeats[seatIndex] = false;
                }
                saveSeatsForMovie(booking.movie);
            }
        }
        
        userBookings = updatedBookings;
        saveAllBookings();
    }

    bool isAdmin() {
        cout << "Enter admin password: ";
        string enteredPassword = getPassword();
        return enteredPassword == ADMIN_PASSWORD;
    }

    void groupBookingsByMovie() {
        map<string, vector<Booking*>> movieGroups;
        for (auto& booking : userBookings) {
            movieGroups[booking.movie].push_back(&booking);
        }
    }

public:
    User() : bookedSeats(totalSeats, false) {}

    void registerUser() {
        username = getValidatedString("Enter username: ");
        
        if (username.find(' ') != string::npos) {
            cout << "Username cannot contain spaces.\n";
            return;
        }

        if (userExists(username)) {
            cout << "Username already exists. Please choose a different one.\n";
            return;
        }

        password = getPassword();
        
        cout << "Select your city:\n";
        for (size_t i = 0; i < cities.size(); i++) {
            cout << i + 1 << ". " << cities[i] << "\n";
        }
        cout << "Enter choice (1-" << cities.size() << "): ";
        
        int cityIndex = getValidatedInput(1, cities.size()) - 1;
        city = cities[cityIndex];

        ofstream file("users.txt", ios::app);
        if (file.is_open()) {
            file << username << " " << password << endl << city << endl;
            file.close();
            cout << "Registration successful!\n";
        } else {
            cout << "Error saving user data.\n";
        }
    }

    void resetAllBookings() {
        if (!isAdmin()) {
            cout << "Invalid admin password!\n";
            return;
        }

        // Clear all seat files
        for (const auto& movie : movies) {
            string filename = getSeatFileName(movie);
            ofstream file(filename, ios::binary | ios::trunc);
            if (file.is_open()) {
                vector<bool> emptySeats(totalSeats, false);
                for (bool status : emptySeats) {
                    file.write(reinterpret_cast<const char*>(&status), sizeof(bool));
                }
                file.close();
            }
        }

        // Clear bookings file
        ofstream bookingFile("bookings.txt", ios::trunc);
        bookingFile.close();

        userBookings.clear();
        cout << "All bookings have been reset!\n";
    }

    void loginUser() {
        username = getValidatedString("Enter username: ");
        
        if (!userExists(username)) {
            cout << "User does not exist.\n";
            return;
        }

        password = getPassword();
        ifstream file("users.txt");
        if (!file.is_open()) {
            cout << "Error accessing user database.\n";
            return;
        }

        string storedUser, storedPass, storedCity;
        bool found = false;
        while (file >> storedUser >> storedPass) {
            file.ignore();
            getline(file, storedCity);
            if (storedUser == username && storedPass == password) {
                loggedin = true;
                city = storedCity;
                cout << "Login successful!\n";
                found = true;
                break;
            }
        }
        file.close();

        if (!found) {
            cout << "Invalid credentials.\n";
        } else {
            loadUserBookings();
        }
    }

    void showSeatingLayout() {
        cout << "\nSeating arrangement for " << currentMovie << ":\n";
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
        cout << "Available seats: " << countAvailableSeats() << "/" << totalSeats << endl;
    }

    bool bookTicket(const string& seatLabel) {
        if (!isValidSeat(seatLabel)) {
            cout << "Invalid seat number format. Please use format like A1, B2, etc.\n";
            return false;
        }
    
        int row = toupper(seatLabel[0]) - 'A';
        int seat = stoi(seatLabel.substr(1)) - 1;
        int seatIndex = row * seatsPerRow + seat;
    
        if (bookedSeats[seatIndex]) {
            cout << "Seat " << seatLabel << " is already booked. Please choose another seat.\n";
            return false;
        }
    
        bookedSeats[seatIndex] = true;
        saveSeatsForMovie(currentMovie);
        return true;
    }

    bool removeTicket(const string& seatLabel) {
        if (!isValidSeat(seatLabel)) {
            cout << "Invalid seat number format.\n";
            return false;
        }
    
        int row = toupper(seatLabel[0]) - 'A';
        int seat = stoi(seatLabel.substr(1)) - 1;
        int seatIndex = row * seatsPerRow + seat;
    
        if (!bookedSeats[seatIndex]) {
            cout << "No booking found for seat " << seatLabel << ".\n";
            return false;
        }
    
        // Check if this seat belongs to the current user
        bool userOwnsSeat = false;
        for (const auto& booking : userBookings) {
            if (find(booking.seats.begin(), booking.seats.end(), seatLabel) != booking.seats.end()) {
                userOwnsSeat = true;
                break;
            }
        }
        
        if (!userOwnsSeat) {
            cout << "You can only cancel seats you've booked.\n";
            return false;
        }
    
        bookedSeats[seatIndex] = false;
        saveSeatsForMovie(currentMovie);
        
        for (auto it = userBookings.begin(); it != userBookings.end(); ) {
            auto& seats = it->seats;
            auto seatIt = find(seats.begin(), seats.end(), seatLabel);
            if (seatIt != seats.end()) {
                seats.erase(seatIt);
                if (seats.empty()) {
                    it = userBookings.erase(it);
                    continue;
                }
            }
            ++it;
        }
        
        saveAllBookings();
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
            cout << "\n***** Movie Ticket Booking System *****\n";
            cout << "Welcome, " << username << " (" << city << ")\n";
            cout << "1. Book Tickets\n";
            cout << "2. Cancel Tickets\n";
            cout << "3. View Booking Summary\n";
            cout << "4. Logout\n";
            cout << "*\n";
    
            cout << "Enter your choice (1-4): ";
            int choice = getValidatedInput(1, 4);
    
            switch (choice) {
                case 1: bookTicketMenu(); break;
                case 2: removeTicketMenu(); break;
                case 3: displayBill(); break;
                case 4: return;
            }
        }
    }

    void loadUserBookings() {
        userBookings.clear();
        ifstream bookingFile("bookings.txt");
        if (!bookingFile.is_open()) return;
    
        string line;
        while (getline(bookingFile, line)) {
            size_t pos = 0;
            string token;
            vector<string> tokens;
            
            while ((pos = line.find('|')) != string::npos) {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);
            
            if (tokens.size() < 6) continue;
            
            string user = tokens[0];
            if (user != username) continue;
            
            Booking booking;
            booking.movie = tokens[1];
            booking.bookingTime = tokens[2];
            booking.paid = (tokens[3] == "1");
            booking.bookingTimestamp = stol(tokens[4]);
            
            string seatsStr = tokens[5];
            size_t seatPos = 0;
            while ((seatPos = seatsStr.find(',')) != string::npos) {
                token = seatsStr.substr(0, seatPos);
                if (!token.empty()) {
                    booking.seats.push_back(token);
                }
                seatsStr.erase(0, seatPos + 1);
            }
            
            if (!booking.seats.empty()) {
                userBookings.push_back(booking);
            }
        }
        bookingFile.close();
        
        // Check for expired bookings
        checkAndResetExpiredBookings();
    }

    void bookTicketMenu() {
        while (true) {
            system("CLS");
            cout << "Available Movies:\n";
            for (size_t i = 0; i < movies.size(); i++) {
                cout << i + 1 << ". " << movies[i] << "\n";
            }
            cout << movies.size() + 1 << ". Back to Main Menu\n";

            cout << "Select a movie (1-" << movies.size() + 1 << "): ";
            int choice = getValidatedInput(1, movies.size() + 1);

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

        cout << "\n***** Movie Details *****\n";
        cout << "Title: " << movies[movieIndex] << "\n";
        cout << "Synopsis: " << details[movieIndex] << "\n";
        cout << "Cast: " << casts[movieIndex] << "\n";
        cout << "\n";

        cout << "1. Book Tickets\n";
        cout << "2. Back to Movies\n";
        cout << "Enter your choice (1-2): ";
        int choice = getValidatedInput(1, 2);

        if (choice == 1) {
            showSeating();
        }
    }

    void showSeating() {
        while (true) {
            system("CLS");
            showSeatingLayout();
            
            int available = countAvailableSeats();
            cout << "How many tickets do you want to book? (0 to cancel, " << available << " available): ";
            int ticketCount;
            while (!(cin >> ticketCount) || ticketCount < 0 || (ticketCount > available && available > 0)) {
                cout << "Invalid input. Please enter a number between 0 and " << available << ": ";
                clearInputBuffer();
            }
            clearInputBuffer();
    
            if (ticketCount == 0) return;
            if (available == 0) {
                cout << "No seats available for this movie.\n";
                cout << "Press any key to continue...";
                _getch();
                return;
            }
    
            Booking newBooking;
            newBooking.movie = currentMovie;
            newBooking.bookingTime = getCurrentDateTime();
            newBooking.paid = false;
            newBooking.bookingTimestamp = time(0);
            
            int ticketsBooked = 0;
            while (ticketsBooked < ticketCount) {
                system("CLS");
                showSeatingLayout();
                
                cout << "Booking ticket " << ticketsBooked + 1 << " of " << ticketCount << endl;
                string seatLabel = getValidatedString("Enter seat number (e.g., A1, B2): ", true);

                if (bookTicket(seatLabel)) {
                    newBooking.seats.push_back(seatLabel);
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
            
            userBookings.push_back(newBooking);
            saveAllBookings();
            
            cout << "\nSuccessfully booked " << ticketsBooked << " ticket" 
                 << (ticketsBooked > 1 ? "s" : "") << "!\n";
            
            cout << "1. Book more tickets\n";
            cout << "2. Back to Main Menu\n";
            cout << "Enter your choice (1-2): ";
            
            int choice = getValidatedInput(1, 2);
            if (choice == 2) return;
        }
    }

    void removeTicketMenu() {
        loadUserBookings();
        
        // Group bookings by movie
        map<string, vector<Booking*>> movieGroups;
        for (auto& booking : userBookings) {
            movieGroups[booking.movie].push_back(&booking);
        }

        if (movieGroups.empty()) {
            cout << "You don't have any bookings to cancel.\n";
            cout << "Press any key to continue...";
            _getch();
            return;
        }

        while (true) {
            system("CLS");
            cout << "Your Bookings by Movie:\n";
            cout << "-----------------------\n";
            
            // Display movie options
            int index = 1;
            vector<string> movieNames;
            for (const auto& pair : movieGroups) {
                cout << index++ << ". " << pair.first 
                     << " (" << pair.second.size() << " booking" 
                     << (pair.second.size() > 1 ? "s" : "") << ")\n";
                movieNames.push_back(pair.first);
            }
            cout << "0. Back to Main Menu\n";
            cout << "-----------------------\n";
            cout << "Select a movie to cancel tickets (1-" << movieGroups.size() << "): ";
            
            int movieChoice;
            while (!(cin >> movieChoice) || movieChoice < 0 || movieChoice > movieGroups.size()) {
                cout << "Invalid input. Please enter a number between 0 and " << movieGroups.size() << ": ";
                clearInputBuffer();
            }
            clearInputBuffer();
            
            if (movieChoice == 0) {
                return;
            }
            
            string selectedMovie = movieNames[movieChoice - 1];
            currentMovie = selectedMovie;
            loadSeatsForMovie(currentMovie);
            
            // Display all bookings for this movie
            vector<Booking*>& bookingsForMovie = movieGroups[selectedMovie];
            
            while (true) {
                system("CLS");
                showSeatingLayout();  // Show seat layout immediately after selecting movie
                
                cout << "Bookings for " << selectedMovie << ":\n";
                cout << "-----------------------\n";
                
                for (size_t i = 0; i < bookingsForMovie.size(); i++) {
                    Booking* booking = bookingsForMovie[i];
                    cout << i + 1 << ". Booking Time: " << booking->bookingTime << "\n";
                    cout << "   Seats: ";
                    for (const auto& seat : booking->seats) {
                        cout << seat << " ";
                    }
                    cout << "\n   Status: " << (booking->paid ? "Paid" : "Unpaid") << "\n";
                    cout << "-----------------------\n";
                }
                cout << "0. Back to Movie Selection\n";
                cout << "-----------------------\n";
                cout << "Select a booking to modify (1-" << bookingsForMovie.size() << "): ";
                
                int bookingChoice;
                while (!(cin >> bookingChoice) || bookingChoice < 0 || bookingChoice > bookingsForMovie.size()) {
                    cout << "Invalid input. Please enter a number between 0 and " << bookingsForMovie.size() << ": ";
                    clearInputBuffer();
                }
                clearInputBuffer();
                
                if (bookingChoice == 0) {
                    break;
                }
                
                Booking* selectedBooking = bookingsForMovie[bookingChoice - 1];
                
                while (true) {
                    system("CLS");
                    showSeatingLayout();  // Show seat layout before canceling
                    
                    cout << "Selected Booking:\n";
                    cout << "Movie: " << selectedBooking->movie << "\n";
                    cout << "Time: " << selectedBooking->bookingTime << "\n";
                    cout << "Seats: ";
                    for (const auto& seat : selectedBooking->seats) {
                        cout << seat << " ";
                    }
                    cout << "\nStatus: " << (selectedBooking->paid ? "Paid" : "Unpaid") << "\n";
                    cout << "-----------------------\n";
                    
                    string seatLabel = getValidatedString("Enter seat number to cancel (e.g., A1, B2) or 0 to go back: ", true);
            
                    if (seatLabel == "0") {
                        break;
                    }
                    
                    if (removeTicket(seatLabel)) {
                        // Remove the seat from the booking
                        auto& seats = selectedBooking->seats;
                        seats.erase(remove(seats.begin(), seats.end(), seatLabel), seats.end());
                        
                        system("CLS");
                        showSeatingLayout();
                        cout << "Seat " << seatLabel << " has been successfully canceled.\n";
                        
                        if (seats.empty()) {
                            // Remove the empty booking from all containers
                            for (auto it = userBookings.begin(); it != userBookings.end(); ++it) {
                                if (&(*it) == selectedBooking) {
                                    userBookings.erase(it);
                                    break;
                                }
                            }
                            
                            // Remove from movieGroups
                            bookingsForMovie.erase(bookingsForMovie.begin() + (bookingChoice - 1));
                            
                            cout << "All seats canceled for this booking.\n";
                            cout << "Press any key to continue...";
                            _getch();
                            break;
                        }
                    } else {
                        cout << "Failed to cancel seat " << seatLabel << ".\n";
                    }
            
                    cout << "\n1. Cancel another ticket from this booking\n";
                    cout << "2. Back to Booking Selection\n";
                    cout << "Enter your choice (1-2): ";
                    
                    int option = getValidatedInput(1, 2);
                    if (option == 2) {
                        break;
                    }
                }
                
                // If we've removed all bookings for this movie, break out
                if (bookingsForMovie.empty()) {
                    movieGroups.erase(selectedMovie);
                    break;
                }
            }
            
            // If no more bookings left at all, return to main menu
            if (movieGroups.empty()) {
                cout << "\nAll your bookings have been canceled.\n";
                cout << "Press any key to return to main menu...";
                _getch();
                return;
            }
        }
    }

    void displayBill() {
        loadUserBookings();
        
        userBookings.erase(
            remove_if(userBookings.begin(), userBookings.end(), 
                     [](const Booking& b) { return b.seats.empty(); }),
            userBookings.end());
        
        if (userBookings.empty()) {
            cout << "No bookings found!\n";
            cout << "Press any key to continue...";
            _getch();
            return;
        }
        
        map<string, vector<Booking>> movieBookings;
        for (const auto& booking : userBookings) {
            movieBookings[booking.movie].push_back(booking);
        }
        
        bool allPaid = all_of(userBookings.begin(), userBookings.end(), 
                            [](const Booking& b) { return b.paid; });
        
        if (allPaid) {
            showFinalBill(movieBookings);
            return;
        }
        
        system("CLS");
        const double TICKET_PRICE = 110.0;
        double totalAmount = 0;
        
        cout << "============================================\n";
        cout << "          PENDING BOOKINGS (UNPAID)         \n";
        cout << "============================================\n";
        
        for (const auto& movieGroup : movieBookings) {
            const string& movie = movieGroup.first;
            const vector<Booking>& bookings = movieGroup.second;
            
            bool allPaidForMovie = all_of(bookings.begin(), bookings.end(), 
                                         [](const Booking& b) { return b.paid; });
            if (allPaidForMovie) continue;
            
            cout << "  Movie: " << movie << "\n";
            
            vector<string> allSeats;
            string bookingTimes;
            int totalTickets = 0;
            
            for (const auto& booking : bookings) {
                if (!booking.paid) {
                    allSeats.insert(allSeats.end(), booking.seats.begin(), booking.seats.end());
                    if (!bookingTimes.empty()) bookingTimes += ", ";
                    bookingTimes += booking.bookingTime;
                    totalTickets += booking.seats.size();
                }
            }
            
            if (totalTickets == 0) continue;
            
            double movieAmount = totalTickets * TICKET_PRICE;
            totalAmount += movieAmount;
            
            cout << "  Booking Times: " << bookingTimes << "\n";
            cout << "  Seats: ";
            for (const auto& seat : allSeats) {
                cout << seat << " ";
            }
            cout << "\n";
            cout << "  Tickets: " << totalTickets << "\n";
            cout << "  Amount: Rs." << fixed << setprecision(2) << movieAmount << "\n";
            cout << "--------------------------------------------\n";
        }
        
        if (totalAmount > 0) {
            cout << "  TOTAL PENDING: Rs." << fixed << setprecision(2) << totalAmount << "\n";
            cout << "============================================\n";
            
            cout << "\nPress any key to view payment options...";
            _getch();
            processPayment();
        } else {
            cout << "  No unpaid bookings found!\n";
            cout << "============================================\n";
            cout << "\nPress any key to continue...";
            _getch();
        }
        
        bool hasPaidBookings = any_of(userBookings.begin(), userBookings.end(), 
                                     [](const Booking& b) { return b.paid; });
        
        if (hasPaidBookings) {
            showFinalBill(movieBookings);
        }
    }
    
    void processPayment() {
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
            
            int paymentChoice = getValidatedInput(1, 4);
            
            if (paymentChoice == 4) {
                return;
            }
            
            string paymentMethod;
            switch (paymentChoice) {
                case 1: paymentMethod = "Cash"; break;
                case 2: paymentMethod = "Debit/Credit Card"; break;
                case 3: paymentMethod = "Online Payment"; break;
            }
            
            for (auto& booking : userBookings) {
                if (!booking.paid) {
                    booking.paid = true;
                }
            }
            saveAllBookings();
            
            system("CLS");
            string paymentTime = getCurrentDateTime();
            cout << "============================================\n";
            cout << "            PAYMENT CONFIRMATION           \n";
            cout << "============================================\n";
            cout << "  User: " << username << "\n";
            cout << "  Payment Method: " << paymentMethod << "\n";
            cout << "  Status: CONFIRMED\n";
            cout << "  Payment Time: " << paymentTime << "\n";
            cout << "============================================\n";
            cout << "       Thank you for your payment!         \n";
            cout << "============================================\n";
            
            cout << "\n1. View Booking Summary\n";
            cout << "2. Return to Main Menu\n";
            cout << "Enter choice: ";
            
            int choice = getValidatedInput(1, 2);
            if (choice == 1) {
                map<string, vector<Booking>> movieBookings;
                for (const auto& booking : userBookings) {
                    movieBookings[booking.movie].push_back(booking);
                }
                showFinalBill(movieBookings);
            }
            return;
        }
    }
    
    void showFinalBill(const map<string, vector<Booking>>& movieBookings) {
        system("CLS");
        const double TICKET_PRICE = 110.0;
        
        cout << "============================================\n";
        cout << "            FINAL BOOKING SUMMARY          \n";
        cout << "============================================\n";
        cout << "  User: " << username << "\n";
        cout << "  City: " << city << "\n";
        cout << "  Current Date: " << getCurrentDateTime() << "\n";
        cout << "============================================\n";
        
        double totalAmount = 0;
        
        for (const auto& booking : userBookings) {
            if (!booking.paid) continue;
            
            const string& movie = booking.movie;
            cout << "  Movie: " << movie << "\n";
            
            vector<string> allSeats = booking.seats;
            string bookingTimes = booking.bookingTime;
            int totalTickets = allSeats.size();
            
            double movieAmount = totalTickets * TICKET_PRICE;
            totalAmount += movieAmount;
            
            cout << "  Booking Time: " << bookingTimes << "\n";
            cout << "  Seats: ";
            for (const auto& seat : allSeats) {
                cout << seat << " ";
            }
            cout << "\n";
            cout << "  Tickets: " << totalTickets << "\n";
            cout << "  Amount: Rs." << fixed << setprecision(2) << movieAmount << " (Paid)\n";
            cout << "--------------------------------------------\n";
        }
        
        cout << "  TOTAL AMOUNT: Rs." << fixed << setprecision(2) << totalAmount << "\n";
        cout << "============================================\n";
        cout << "       All payments completed!             \n";
        cout << "============================================\n";
        
        cout << "\nPress any key to continue...";
        _getch();
    }
};

int main() {
    User user;
    while (true) {
        system("CLS");
        cout << "***** Welcome to Movie Ticket Booking *****\n";
        cout << "*\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Admin: Reset All Bookings\n";
        cout << "4. Exit\n";
        cout << "*\n";

        cout << "Enter your choice (1-4): ";
        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cout << "Invalid input. Please enter a number between 1 and 4: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
                user.resetAllBookings();
                break;
            case 4:
                return 0;
        }
        
        cout << "\nPress any key to continue...";
        _getch();
    }

    return 0;
}
