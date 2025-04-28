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

using namespace std;

class User 
{
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
    vector<string> movies =
    {
        "Chhava", "Bahubali 2", "Pushpa 2", 
        "KGF 2", "Avengers Endgame", "Captain America: Brave New World"
    };

    struct Booking {
        string movie;
        vector<string> seats;
        string bookingTime;
        bool paid;
    };
    vector<Booking> userBookings;

    bool userExists(const string& user) 
    {
        ifstream file("users.txt");
        if (!file) return false;

        string storedUser, storedPass, storedCity;
        while (file >> storedUser >> storedPass) 
        {
            file.ignore();
            getline(file, storedCity);
            if (storedUser == user) 
            {
                return true;
            }
        }
        return false;
    }

    string getPassword() 
    {
        string pass;
        char ch;
        cout << "Enter password: ";
        while (true) {
            ch = _getch();
            if (ch == '\r') break;
            if (ch == '\b' && !pass.empty()) 
            {
                pass.pop_back();
                cout << "\b \b";
            } else if (ch >= 32 && ch <= 126) 
            {
                pass += ch;
                cout << '*';
            }
        }
        cout << endl;
        return pass;
    }

    string getSeatFileName(const string& movieName) 
    {
        return "seats_" + movieName + ".dat";
    }

    void loadSeatsForMovie(const string& movieName) 
    {
        bookedSeats.assign(totalSeats, 0);
        string filename = getSeatFileName(movieName);
        ifstream file(filename, ios::binary);
        if (file) 
        {
            file.read(reinterpret_cast<char*>(bookedSeats.data()), totalSeats * sizeof(char));
            file.close();
        }
    }

    void saveSeatsForMovie(const string& movieName) 
    {
        string filename = getSeatFileName(movieName);
        ofstream file(filename, ios::binary);
        if (file) 
        {
            file.write(reinterpret_cast<const char*>(bookedSeats.data()), totalSeats * sizeof(char));
            file.close();
        }
    }

    void saveAllBookings() 
    {
        ofstream bookingFile("bookings.txt");
        if (bookingFile.is_open()) {
            for (const auto& booking : userBookings) {
                if (booking.seats.empty()) continue; // Skip empty bookings
                
                bookingFile << username << " " << booking.movie << " " << booking.bookingTime << " " 
                           << (booking.paid ? "1" : "0") << " ";
                for (const auto& seat : booking.seats) {
                    bookingFile << seat << " ";
                }
                bookingFile << endl;
            }
            bookingFile.close();
        }
    }

    bool isValidSeat(const string& seatLabel) 
    {
        if (seatLabel.length() < 2) return false;
        
        char rowChar = toupper(seatLabel[0]);
        if (rowChar < 'A' || rowChar >= 'A' + rows) return false;
        
        try 
        {
            int seatNum = stoi(seatLabel.substr(1));
            if (seatNum < 1 || seatNum > seatsPerRow) return false;
        } catch (...) 
        {
            return false;
        }
        
        return true;
    }

    string getCurrentDateTime() 
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
        return string(buf);
    }

public:
    User() : bookedSeats(totalSeats, 0) {}

    void registerUser() 
    {
        cout << "Enter username: ";
        cin >> username;
        if (userExists(username)) 
        {
            cout << "Username already exists. Please choose a different one." << endl;
            return;
        }
        password = getPassword();
        cout << "Select your city (0: Ahmedabad, 1: Surat, 2: Nadiad, 3: Vadodara, 4: Rajkot): ";
        int cityIndex;
        cin >> cityIndex;
        if (cityIndex < 0 || cityIndex >= cities.size()) 
        {
            cout << "Invalid city selection." << endl;
            return;
        }
        city = cities[cityIndex];

        ofstream file("users.txt", ios::app);
        file << username << " " << password << endl << city << endl;
        cout << "Registration successful!" << endl;
    }

    void loginUser() 
    {
        cout << "Enter username: ";
        cin >> username;
        if (!userExists(username)) 
        {
            cout << "User does not exist." << endl;
            return;
        }
        password = getPassword();
        ifstream file("users.txt");
        string storedUser, storedPass, storedCity;
        while (file >> storedUser >> storedPass) 
        {
            file.ignore();
            getline(file, storedCity);
            if (storedUser == username && storedPass == password) 
            {
                loggedin = true;
                city = storedCity;
                cout << "Login successful!" << endl;
                loadUserBookings();
                return;
            }
        }
        cout << "Invalid credentials." << endl;
    }

    void showSeatingLayout() 
    {
        cout << "Seating arrangement for " << currentMovie << ":\n";
        cout << "-----------------------------------\n";
        cout << "   ";
        for (int i = 1; i <= seatsPerRow; ++i) 
        {
            cout << " " << setw(2) << i << " ";
        }
        cout << endl;

        for (int row = 0; row < rows; ++row) {
            cout << static_cast<char>('A' + row) << " | ";
            for (int seat = 0; seat < seatsPerRow; ++seat) 
            {
                int seatIndex = row * seatsPerRow + seat;
                cout << (bookedSeats[seatIndex] ? "[X]" : "[ ]") << " ";
            }
            cout << endl;
        }
        cout << "-----------------------------------\n";
    }

    bool bookTicket(const string& seatLabel) 
    {
        if (!isValidSeat(seatLabel)) 
        {
            cout << "Invalid seat number format. Please use format like A1, B2, etc." << endl;
            return false;
        }
    
        int row = toupper(seatLabel[0]) - 'A';
        int seat = stoi(seatLabel.substr(1)) - 1;
        int seatIndex = row * seatsPerRow + seat;
    
        if (bookedSeats[seatIndex]) 
        {
            cout << "Seat " << seatLabel << " is already booked. Please choose another seat." << endl;
            return false;
        }
    
        bookedSeats[seatIndex] = 1;
        saveSeatsForMovie(currentMovie);
        return true;
    }

    bool removeTicket(const string& seatLabel) 
    {
        if (!isValidSeat(seatLabel)) 
        {
            cout << "Invalid seat number format." << endl;
            return false;
        }
    
        int row = toupper(seatLabel[0]) - 'A';
        int seat = stoi(seatLabel.substr(1)) - 1;
        int seatIndex = row * seatsPerRow + seat;
    
        if (!bookedSeats[seatIndex]) 
        {
            cout << "No booking found for seat " << seatLabel << "." << endl;
            return false;
        }
    
        bookedSeats[seatIndex] = 0;
        saveSeatsForMovie(currentMovie);
        
        // Remove the seat from userBookings
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

    bool isLoggedIn() const 
    {
        return loggedin;
    }

    void homepage() 
    {
        if (!currentMovie.empty()) 
        {
            loadSeatsForMovie(currentMovie);
        }
    
        while (true) 
        {
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
    
            switch (choice) 
            {
                case 1: 
                    bookTicketMenu(); 
                    break;
                case 2: 
                    removeTicketMenu(); 
                    break;
                case 3: 
                    displayBill(); 
                    break;
                case 4: 
                    return;
                default: 
                    cout << "Invalid choice! Try again.\n";
            }
        }
    }

    void loadUserBookings() 
    {
        userBookings.clear();
        ifstream bookingFile("bookings.txt");
        if (!bookingFile) return;
    
        string line;
        while (getline(bookingFile, line)) 
        {
            istringstream iss(line);
            string user, movie, bookingTime, paidStr;
            iss >> user >> movie >> bookingTime >> paidStr;
            
            if (user == username) 
            {
                Booking booking;
                booking.movie = movie;
                booking.bookingTime = bookingTime;
                booking.paid = (paidStr == "1");
                
                string seat;
                while (iss >> seat) 
                {
                    booking.seats.push_back(seat);
                }
                
                // Only add if there are actual seats booked
                if (!booking.seats.empty()) {
                    userBookings.push_back(booking);
                }
            }
        }
        bookingFile.close();
    }

    void bookTicketMenu() 
    {
        system("CLS");
        cout << "Available Movies:\n";
        for (size_t i = 0; i < movies.size(); i++) 
        {
            cout << i + 1 << ". " << movies[i] << "\n";
        }
        cout << movies.size() + 1 << ". Back to Main Menu\n";

        int choice;
        cout << "Select a movie (1-" << movies.size() + 1 << "): ";
        cin >> choice;

        if (choice > 0 && choice <= movies.size()) 
        {
            currentMovie = movies[choice - 1];
            loadSeatsForMovie(currentMovie);
            showMovieDetails(choice - 1);
        }
    }

    void showMovieDetails(int movieIndex) 
    {
        system("CLS");
        vector<string> details = 
        {
            "The film explores the period after the death of Chhatrapati Shivaji Maharaj...",
            "The Baahubali movie series follows the story of Shivudu...",
            "Pushpa: The Rise follows Pushpa Raj...",
            "The K.G.F movie series follows Rocky...",
            "In 2018, 23 days after Thanos erased half of all life...",
            "In Captain America: Brave New World, Sam Wilson..."
        };
        vector<string> casts = 
        {
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

        if (choice == 1) 
        {
            showSeating();
        }
    }

    void showSeating() 
    {
        while (true) {
            system("CLS");
            showSeatingLayout();
            
            int ticketCount;
            cout << "How many tickets do you want to book? (0 to cancel): ";
            cin >> ticketCount;
    
            if (ticketCount == 0) return;
            if (ticketCount < 0) 
            {
                cout << "Invalid number of tickets. Please try again.\n";
                continue;
            }
    
            // Create a new booking for this session
            Booking newBooking;
            newBooking.movie = currentMovie;
            newBooking.bookingTime = getCurrentDateTime();
            newBooking.paid = false;
            
            int ticketsBooked = 0;
            while (ticketsBooked < ticketCount) 
            {
                system("CLS");
                showSeatingLayout();
                
                cout << "Booking ticket " << ticketsBooked + 1 << " of " << ticketCount << endl;
                string seatLabel;
                cout << "Enter seat number (e.g., A1, B2): ";
                cin >> seatLabel;
    
                if (bookTicket(seatLabel)) 
                {
                    newBooking.seats.push_back(seatLabel); // This should correctly add the booked seat
                    ticketsBooked++;
                    system("CLS");
                    showSeatingLayout();
                    cout << "Successfully booked seat " << seatLabel << "!\n";
                    
                    if (ticketsBooked < ticketCount) 
                    {
                        cout << "\nPlease book the next ticket...\n";
                        cout << "Press any key to continue...";
                        _getch();
                    }
                }
                else 
                {
                    cout << "Failed to book seat " << seatLabel << ". Please try again.\n";
                    cout << "Press any key to continue...";
                    _getch();
                }
            }
            
            // Add the new booking to user's bookings
            userBookings.push_back(newBooking);
            saveAllBookings();
            
            if(ticketCount == 1) 
            {
                cout << "\nSuccessfully booked " << ticketsBooked << " ticket!\n";
            } 
            else
            {
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

    void removeTicketMenu() 
    {
        loadUserBookings();
        if (userBookings.empty()) 
        {
            cout << "You don't have any bookings to cancel.\n";
            cout << "Press any key to continue...";
            _getch();
            return;
        }
        
        // Let user select which booking to modify
        system("CLS");
        cout << "Select booking to modify:\n";
        for (size_t i = 0; i < userBookings.size(); i++) {
            cout << i+1 << ". " << userBookings[i].movie << " (" 
                 << userBookings[i].seats.size() << " seats) - "
                 << (userBookings[i].paid ? "Paid" : "Unpaid") << endl;
        }
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        if (choice == 0) return;
        if (choice < 1 || choice > userBookings.size()) {
            cout << "Invalid choice!\n";
            cout << "Press any key to continue...";
            _getch();
            return;
        }
        
        Booking& selectedBooking = userBookings[choice-1];
        currentMovie = selectedBooking.movie;
        loadSeatsForMovie(currentMovie);
        
        while (true) 
        {
            system("CLS");
            showSeatingLayout();
            
            cout << "Current booking for " << selectedBooking.movie << ":\n";
            for (const auto& seat : selectedBooking.seats) {
                cout << seat << " ";
            }
            cout << endl;
            
            string seatLabel;
            cout << "Enter seat number to cancel (e.g., A1, B2) or 0 to go back: ";
            cin >> seatLabel;
    
            if (seatLabel == "0") {
                saveAllBookings();
                return;
            }
            
            if (removeTicket(seatLabel)) 
            {
                // Update the selected booking's seats
                auto& seats = selectedBooking.seats;
                seats.erase(remove(seats.begin(), seats.end(), seatLabel), seats.end());
                
                system("CLS");
                showSeatingLayout();
                cout << "Seat " << seatLabel << " has been successfully canceled.\n";
                
                if (seats.empty()) {
                    userBookings.erase(userBookings.begin() + (choice-1));
                    cout << "All seats canceled for this booking.\n";
                    saveAllBookings();
                    cout << "Press any key to continue...";
                    _getch();
                    return;
                }
            }
    
            cout << "\n1. Cancel another ticket\n";
            cout << "2. Back to Main Menu\n";
            cout << "Enter your choice (1-2): ";
            
            int option;
            cin >> option;
            if (option == 2) {
                saveAllBookings();
                return;
            }
        }
    }

    void displayBill() 
    {
        loadUserBookings();
        
        // Remove any empty bookings that might have slipped through
        userBookings.erase(
            remove_if(userBookings.begin(), userBookings.end(), 
                     [](const Booking& b) { return b.seats.empty(); }),
            userBookings.end());
        
        if (userBookings.empty()) 
        {
            cout << "No bookings found!\n";
            cout << "Press any key to continue...";
            _getch();
            return;
        }
        
        // Group bookings by movie and payment status
        map<string, vector<Booking>> movieBookings;
        for (const auto& booking : userBookings) {
            movieBookings[booking.movie].push_back(booking);
        }
        
        // Check if all bookings are paid
        bool allPaid = true;
        for (const auto& booking : userBookings) {
            if (!booking.paid) {
                allPaid = false;
                break;
            }
        }
        
        if (allPaid) {
            showFinalBill(movieBookings);
            return;
        }
        
        // Display unpaid bookings grouped by movie
        system("CLS");
        const double TICKET_PRICE = 110.0;
        double totalAmount = 0;
        
        cout << "============================================\n";
        cout << "          PENDING BOOKINGS (UNPAID)         \n";
        cout << "============================================\n";
        
        for (const auto& movieGroup : movieBookings) {
            const string& movie = movieGroup.first;
            const vector<Booking>& bookings = movieGroup.second;
            
            // Skip paid bookings
            bool allPaidForMovie = true;
            for (const auto& booking : bookings) {
                if (!booking.paid) {
                    allPaidForMovie = false;
                    break;
                }
            }
            if (allPaidForMovie) continue;
            
            cout << "  Movie: " << movie << "\n";
            
            // Collect all unpaid seats for this movie
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
            
            // Show payment options for unpaid bookings
            processPayment();
        } else {
            cout << "  No unpaid bookings found!\n";
            cout << "============================================\n";
            cout << "\nPress any key to continue...";
            _getch();
        }
        
        // Show paid bookings if any exist
        bool hasPaidBookings = false;
        for (const auto& booking : userBookings) {
            if (booking.paid) {
                hasPaidBookings = true;
                break;
            }
        }
        
        if (hasPaidBookings) {
            showFinalBill(movieBookings);
        }
    }
    
    void processPayment() 
    {
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
        
        int paymentChoice;
        cin >> paymentChoice;
        
        if (paymentChoice == 4) {
            return;
        }
        
        string paymentMethod;
        switch (paymentChoice) {
            case 1: paymentMethod = "Cash"; break;
            case 2: paymentMethod = "Debit/Credit Card"; break;
            case 3: paymentMethod = "Online Payment"; break;
            default: 
                cout << "Invalid option selected.\n";
                cout << "Press any key to continue...";
                _getch();
                return;
        }
        
        // Mark all unpaid bookings as paid
        for (auto& booking : userBookings) {
            if (!booking.paid) {
                booking.paid = true;
            }
        }
        saveAllBookings();
        
        // Show payment confirmation
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
        
        cout << "\nPress any key to continue...";
        _getch();
    }
    
    void showFinalBill(const map<string, vector<Booking>>& movieBookings) 
{
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
    
    // Iterate through the user's bookings only
    for (const auto& booking : userBookings) {
        if (!booking.paid) continue; // Skip unpaid bookings
        
        const string& movie = booking.movie;
        cout << "  Movie: " << movie << "\n";
        
        // Collect all seats and booking times for this movie
        vector<string> allSeats = booking.seats; // Directly use the seats from the booking
        string bookingTimes = booking.bookingTime; // Use the booking time from the booking
        int totalTickets = allSeats.size(); // Count tickets directly from the booking
        
        double movieAmount = totalTickets * TICKET_PRICE;
        totalAmount += movieAmount;
        
        cout << "  Booking Times: " << bookingTimes << "\n";
        cout << "  Seats: ";
        for (const auto& seat : allSeats) {
            cout << seat << " "; // Display the booked seats
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

int main() 
{
    User user;
    while (true) 
    {
        system("CLS");
        cout << "***** Welcome to Movie Ticket Booking *****\n";
        cout << "*******************************************\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "*******************************************\n";

        int choice;
        cout << "Enter your choice (1-3): ";
        cin >> choice;

        switch (choice) 
        {
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