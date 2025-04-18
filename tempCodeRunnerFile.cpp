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