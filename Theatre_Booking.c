/*
  This project is about a THEATRE TICKET BOOKING SYSTEM.
  We have developed this project in C++ and after convert this in C.
  The system allows users to book movie tickets efficiently and manage seat availability.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_USERS 100
#define MAX_SEATS 100
#define MAX_MOVIES 6
#define MAX_CITIES 5
#define MAX_BOOKINGS 100
#define MAX_SEATS_PER_BOOKING 10
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_CITY_LEN 50
#define MAX_MOVIE_NAME_LEN 50
#define MAX_SEAT_LABEL_LEN 5
#define ROWS 10
#define SEATS_PER_ROW 10
#define TOTAL_SEATS (ROWS * SEATS_PER_ROW)

typedef struct {
    char movie[MAX_MOVIE_NAME_LEN];
    char seats[MAX_SEATS_PER_BOOKING][MAX_SEAT_LABEL_LEN];
    int seatCount;
    char bookingTime[20];
    bool paid;
    time_t bookingTimestamp;
} Booking;

typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char city[MAX_CITY_LEN];
    bool loggedIn;
    Booking bookings[MAX_BOOKINGS];
    int bookingCount;
    bool bookedSeats[TOTAL_SEATS];
    char currentMovie[MAX_MOVIE_NAME_LEN];
} User;

const char* cities[MAX_CITIES] = {"Ahmedabad", "Surat", "Nadiad", "Vadodara", "Rajkot"};
const char* movies[MAX_MOVIES] = {
    "Chhava", "Bahubali 2", "Pushpa 2", 
    "KGF 2", "Avengers Endgame", "Captain America: Brave New World"
};
const char* movieDetails[MAX_MOVIES] = {
    "The film explores the period after the death of Chhatrapati Shivaji Maharaj...",
    "The Baahubali movie series follows the story of Shivudu...",
    "Pushpa: The Rise follows Pushpa Raj...",
    "The K.G.F movie series follows Rocky...",
    "In 2018, 23 days after Thanos erased half of all life...",
    "In Captain America: Brave New World, Sam Wilson..."
};
const char* movieCasts[MAX_MOVIES] = {
    "Vicky Kaushal, Rashmika Mandanna...",
    "Prabhas, Rana Daggubati...",
    "Allu Arjun, Rashmika Mandana...",
    "Yash, Sreenidhi Sheti...",
    "Robert Downey Jr., Chris Evans...",
    "Anthony Mackie, Harrison Ford..."
};
const char* ADMIN_PASSWORD = "admin@1";

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void clearInputBuffer() {
    while (getchar() != '\n');
}

void getPassword(char* password) {
    printf("Enter password: ");
    int i = 0;
    char ch;
    while (1) {
        ch = _getch();
        if (ch == '\r') break;
        if (ch == '\b' && i > 0) {
            password[--i] = '\0';
            printf("\b \b");
        } else if (ch >= 32 && ch <= 126 && i < MAX_PASSWORD_LEN - 1) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

void getStringInput(const char* prompt, char* input, int maxLength, bool isSeat) {
    while (1) {
        printf("%s", prompt);
        fgets(input, maxLength, stdin);
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) {
            setColor(12);
            printf("Input cannot be empty. Please try again.\n");
            setColor(15);
            continue;
        }
        
        if (isSeat) {
            if (strlen(input) < 2 || strlen(input) > 3) {
                setColor(12);
                printf("Invalid seat format. Please use format like A1, B2, etc.\n");
                setColor(15);
                continue;
            }
            
            char rowChar = toupper(input[0]);
            if (rowChar < 'A' || rowChar >= 'A' + ROWS) {
                setColor(12);
                printf("Invalid row. Please use letters A-J.\n");
                setColor(15);
                continue;
            }
            
            char* endptr;
            long seatNum = strtol(input + 1, &endptr, 10);
            if (*endptr != '\0' || seatNum < 1 || seatNum > SEATS_PER_ROW) {
                setColor(12);
                printf("Invalid seat number. Please use numbers 1-10.\n");
                setColor(15);
                continue;
            }
        }
        
        break;
    }
}

int getValidatedInput(int min, int max) {
    int input;
    while (1) {
        if (scanf("%d", &input) != 1 || input < min || input > max) {
            printf("Invalid input. Please enter a number between %d and %d: ", min, max);
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return input;
        }
    }
}

void getCurrentDateTime(char* buffer) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

bool userExists(const char* username) {
    FILE* file = fopen("users.txt", "r");
    if (!file) return false;

    char storedUser[MAX_USERNAME_LEN];
    char storedPass[MAX_PASSWORD_LEN];
    char storedCity[MAX_CITY_LEN];
    bool found = false;

    while (fscanf(file, "%s %s\n", storedUser, storedPass) == 2) {
        fgets(storedCity, MAX_CITY_LEN, file);
        storedCity[strcspn(storedCity, "\n")] = '\0';
        
        if (strcmp(storedUser, username) == 0) {
            found = true;
            break;
        }
    }

    fclose(file);
    return found;
}

void getSeatFileName(const char* movieName, char* filename) {
    strcpy(filename, "seats_");
    int pos = strlen(filename);
    
    for (int i = 0; movieName[i] && pos < 100; i++) {
        if (isalnum(movieName[i])) {
            filename[pos++] = toupper(movieName[i]);
        } else if (movieName[i] == ' ') {
            filename[pos++] = '_';
        }
    }
    strcpy(filename + pos, ".dat");
}

void loadSeatsForMovie(User* user, const char* movieName) {
    char filename[100];
    getSeatFileName(movieName, filename);
    
    FILE* file = fopen(filename, "rb");
    if (file) {
        for (int i = 0; i < TOTAL_SEATS; i++) {
            bool status;
            fread(&status, sizeof(bool), 1, file);
            user->bookedSeats[i] = status;
        }
        fclose(file);
    } else {
        for (int i = 0; i < TOTAL_SEATS; i++) {
            user->bookedSeats[i] = false;
        }
    }
}

void saveSeatsForMovie(const char* movieName, bool* bookedSeats) {
    char filename[100];
    getSeatFileName(movieName, filename);
    
    FILE* file = fopen(filename, "wb");
    if (file) {
        for (int i = 0; i < TOTAL_SEATS; i++) {
            fwrite(&bookedSeats[i], sizeof(bool), 1, file);
        }
        fclose(file);
    }
}

int countAvailableSeats(const bool* bookedSeats) {
    int count = 0;
    for (int i = 0; i < TOTAL_SEATS; i++) {
        if (!bookedSeats[i]) count++;
    }
    return count;
}

bool isValidSeat(const char* seatLabel) {
    if (strlen(seatLabel) < 2 || strlen(seatLabel) > 3) return false;
    
    char rowChar = toupper(seatLabel[0]);
    if (rowChar < 'A' || rowChar >= 'A' + ROWS) return false;
    
    char* endptr;
    long seatNum = strtol(seatLabel + 1, &endptr, 10);
    if (*endptr != '\0' || seatNum < 1 || seatNum > SEATS_PER_ROW) return false;
    
    return true;
}

int seatLabelToIndex(const char* seatLabel) {
    int row = toupper(seatLabel[0]) - 'A';
    int seat = atoi(seatLabel + 1) - 1;
    return row * SEATS_PER_ROW + seat;
}

void registerUser(User* user) {
    setColor(15);
    getStringInput("Enter username: ", user->username, MAX_USERNAME_LEN, false);
    
    if (strchr(user->username, ' ') != NULL) {
        setColor(12);
        printf("Username cannot contain spaces.\n");
        setColor(15);
        return;
    }

    if (userExists(user->username)) {
        setColor(12);
        printf("Username already exists. Please choose a different one.\n");
        setColor(15);
        return;
    }

    getPassword(user->password);
    
    printf("Select your city:\n");
    for (int i = 0; i < MAX_CITIES; i++) {
        printf("%d. %s\n", i + 1, cities[i]);
    }
    printf("Enter choice (1-%d): ", MAX_CITIES);
    
    int cityIndex = getValidatedInput(1, MAX_CITIES) - 1;
    strcpy(user->city, cities[cityIndex]);

    FILE* file = fopen("users.txt", "a");
    if (file) {
        fprintf(file, "%s %s\n%s\n", user->username, user->password, user->city);
        fclose(file);
        setColor(10);
        printf("Registration successful!\n");
        setColor(15);
    } else {
        setColor(12);
        printf("Error saving user data.\n");
        setColor(15);
    }
}

void resetAllBookings() {
    char password[MAX_PASSWORD_LEN];
    getPassword(password);
    
    if (strcmp(password, ADMIN_PASSWORD) != 0) {
        setColor(12);
        printf("Invalid admin password!\n");
        setColor(15);
        return;
    }

    for (int i = 0; i < MAX_MOVIES; i++) {
        char filename[100];
        getSeatFileName(movies[i], filename);
        FILE* file = fopen(filename, "wb");
        if (file) {
            bool emptySeats[TOTAL_SEATS] = {false};
            fwrite(emptySeats, sizeof(bool), TOTAL_SEATS, file);
            fclose(file);
        }
    }

    FILE* bookingFile = fopen("bookings.txt", "w");
    if (bookingFile) fclose(bookingFile);

    setColor(10);
    printf("All bookings have been reset!\n");
    setColor(15);
}

void loginUser(User* user) {
    getStringInput("Enter username: ", user->username, MAX_USERNAME_LEN, false);
    
    if (!userExists(user->username)) {
        setColor(12);
        printf("User does not exist.\n");
        setColor(15);
        return;
    }

    getPassword(user->password);
    FILE* file = fopen("users.txt", "r");
    if (!file) {
        setColor(12);
        printf("Error accessing user database.\n");
        setColor(15);
        return;
    }

    char storedUser[MAX_USERNAME_LEN];
    char storedPass[MAX_PASSWORD_LEN];
    char storedCity[MAX_CITY_LEN];
    bool found = false;

    while (fscanf(file, "%s %s\n", storedUser, storedPass) == 2) {
        fgets(storedCity, MAX_CITY_LEN, file);
        storedCity[strcspn(storedCity, "\n")] = '\0';
        
        if (strcmp(storedUser, user->username) == 0 && strcmp(storedPass, user->password) == 0) {
            user->loggedIn = true;
            strcpy(user->city, storedCity);
            setColor(10);
            printf("Login successful!\n");
            setColor(15);
            found = true;
            break;
        }
    }
    fclose(file);

    if (!found) {
        setColor(12);
        printf("Invalid credentials.\n");
        setColor(15);
    }
}

void showSeatingLayout(const User* user) {
    const int TOTAL_WIDTH = 70; 
    char title[100];
    
    setColor(14);
    printf("======================================================================\n"); 
    snprintf(title, sizeof(title), "Seating arrangement for %s", user->currentMovie);
    int padding = (TOTAL_WIDTH - strlen(title)) / 2;
    if (padding < 0) padding = 0;
    printf("%*s%s\n", padding, "", title);
    
    printf("======================================================================\n");
    setColor(15);
    printf("\n--------------------------------------------\n");
    printf("   ");
    for (int i = 1; i <= SEATS_PER_ROW; ++i) {
        printf(" %2d ", i);
    }
    printf("\n");

    for (int row = 0; row < ROWS; ++row) {
        printf("%c | ", 'A' + row);
        for (int seat = 0; seat < SEATS_PER_ROW; ++seat) {
            int seatIndex = row * SEATS_PER_ROW + seat;
            printf("%s ", user->bookedSeats[seatIndex] ? "[X]" : "[ ]");
        }
        printf("\n");
    }
    printf("\n--------------------------------------------\n");
    setColor(11);
    printf("Available seats: %d/%d\n", countAvailableSeats(user->bookedSeats), TOTAL_SEATS);
    setColor(15);
}

bool bookTicket(User* user, const char* seatLabel) {
    if (!isValidSeat(seatLabel)) {
        setColor(12);
        printf("Invalid seat number format. Please use format like A1, B2, etc.\n");
        setColor(15);
        return false;
    }

    int seatIndex = seatLabelToIndex(seatLabel);
    
    if (user->bookedSeats[seatIndex]) {
        setColor(12);
        printf("Seat %s is already booked. Please choose another seat.\n", seatLabel);
        setColor(15);
        return false;
    }

    user->bookedSeats[seatIndex] = true;
    saveSeatsForMovie(user->currentMovie, user->bookedSeats);
    return true;
}

bool removeTicket(User* user, const char* seatLabel) {
    if (!isValidSeat(seatLabel)) {
        setColor(12);
        printf("Invalid seat number format.\n");
        setColor(15);
        return false;
    }

    int seatIndex = seatLabelToIndex(seatLabel);
    
    if (!user->bookedSeats[seatIndex]) {
        setColor(12);
        printf("No booking found for seat %s.\n", seatLabel);
        setColor(15);
        return false;
    }

    for (int i = 0; i < user->bookingCount; i++) {
        for (int j = 0; j < user->bookings[i].seatCount; j++) {
            if (strcmp(user->bookings[i].seats[j], seatLabel) == 0) {
                for (int k = j; k < user->bookings[i].seatCount - 1; k++) {
                    strcpy(user->bookings[i].seats[k], user->bookings[i].seats[k+1]);
                }
                user->bookings[i].seatCount--;
                if (user->bookings[i].seatCount == 0) {
                    for (int k = i; k < user->bookingCount - 1; k++) {
                        user->bookings[k] = user->bookings[k+1];
                    }
                    user->bookingCount--;
                }
                
                user->bookedSeats[seatIndex] = false;
                saveSeatsForMovie(user->currentMovie, user->bookedSeats);
                return true;
            }
        }
    }
    
    setColor(12);
    printf("You can only cancel seats you've booked.\n");
    setColor(15);
    return false;
}

void loadUserBookings(User* user) {
    user->bookingCount = 0;
    FILE* bookingFile = fopen("bookings.txt", "r");
    if (!bookingFile) return;

    char line[500];
    while (fgets(line, sizeof(line), bookingFile)) {
        char* token = strtok(line, "|");
        if (!token) continue;
        
        if (strcmp(token, user->username) != 0) continue;
        
        Booking booking;
        booking.seatCount = 0;
        
        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(booking.movie, token, MAX_MOVIE_NAME_LEN);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(booking.bookingTime, token, 20);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        booking.paid = (strcmp(token, "1") == 0);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        booking.bookingTimestamp = atol(token);
        
        token = strtok(NULL, "|");
        if (!token) continue;
        
        char* seatToken = strtok(token, ",");
        while (seatToken && booking.seatCount < MAX_SEATS_PER_BOOKING) {
            strncpy(booking.seats[booking.seatCount++], seatToken, MAX_SEAT_LABEL_LEN);
            seatToken = strtok(NULL, ",");
        }
        
        if (booking.seatCount > 0 && user->bookingCount < MAX_BOOKINGS) {
            user->bookings[user->bookingCount++] = booking;
        }
    }
    fclose(bookingFile);
}

void showMovieDetails(User* user, int movieIndex) {
    system("cls");
    setColor(14);
    printf("======================================================================\n");
    printf("                             Movie Details                        \n");
    printf("======================================================================\n");
    setColor(15);
    printf("\nTitle: %s\n", movies[movieIndex]);
    printf("Synopsis: %s\n", movieDetails[movieIndex]);
    printf("Cast: %s\n", movieCasts[movieIndex]);
    printf("\n----------------------------------------------------------------------\n");

    printf("1. Book Tickets\n");
    printf("2. Back to Movies\n");
    printf("Enter your choice (1-2): ");
    int choice = getValidatedInput(1, 2);

    if (choice == 1) {
        strcpy(user->currentMovie, movies[movieIndex]);
        loadSeatsForMovie(user, user->currentMovie);
        
        while (1) {
            system("cls");
            showSeatingLayout(user);
            
            int available = countAvailableSeats(user->bookedSeats);
            printf("How many tickets do you want to book? (0 to cancel, %d available): ", available);
            int ticketCount = getValidatedInput(0, available);
            
            if (ticketCount == 0) return;
            if (available == 0) {
                setColor(12);
                printf("No seats available for this movie.\n");
                setColor(15);
                printf("Press any key to continue...");
                _getch();
                return;
            }
            
            Booking newBooking;
            strcpy(newBooking.movie, user->currentMovie);
            getCurrentDateTime(newBooking.bookingTime);
            newBooking.paid = false;
            newBooking.bookingTimestamp = time(NULL);
            newBooking.seatCount = 0;
            
            int ticketsBooked = 0;
            while (ticketsBooked < ticketCount) {
                system("cls");
                showSeatingLayout(user);
                
                printf("Booking ticket %d of %d\n", ticketsBooked + 1, ticketCount);
                char seatLabel[MAX_SEAT_LABEL_LEN];
                getStringInput("Enter seat number (e.g., A1, B2): ", seatLabel, MAX_SEAT_LABEL_LEN, true);

                if (bookTicket(user, seatLabel)) {
                    strcpy(newBooking.seats[newBooking.seatCount++], seatLabel);
                    ticketsBooked++;
                    system("cls");
                    showSeatingLayout(user);
                    printf("Successfully booked seat %s!\n", seatLabel);
                    
                    if (ticketsBooked < ticketCount) {
                        printf("\nPlease book the next ticket...\n");
                        printf("Press any key to continue...");
                        _getch();
                    }
                } else {
                    printf("Failed to book seat %s. Please try again.\n", seatLabel);
                    printf("Press any key to continue...");
                    _getch();
                }
            }
            
            if (user->bookingCount < MAX_BOOKINGS) {
                user->bookings[user->bookingCount++] = newBooking;
            }
            
            FILE* bookingFile = fopen("bookings.txt", "a");
            if (bookingFile) {
                fprintf(bookingFile, "%s|%s|%s|%d|%ld|", 
                       user->username, 
                       newBooking.movie, 
                       newBooking.bookingTime, 
                       newBooking.paid ? 1 : 0,
                       newBooking.bookingTimestamp);
                
                for (int i = 0; i < newBooking.seatCount; i++) {
                    fprintf(bookingFile, "%s%s", newBooking.seats[i], (i == newBooking.seatCount - 1) ? "" : ",");
                }
                fprintf(bookingFile, "\n");
                fclose(bookingFile);
            }
            
            setColor(10);
            printf("\nSuccessfully booked %d ticket%s!\n", ticketsBooked, ticketsBooked > 1 ? "s" : "");
            setColor(15);
            printf("1. Book more tickets\n");
            printf("2. Back to Main Menu\n");
            printf("Enter your choice (1-2): ");
            
            int choice = getValidatedInput(1, 2);
            if (choice == 2) return;
        }
    }
}

void bookTicketMenu(User* user) {
    while (1) {
        system("cls");
        setColor(14);
        printf("======================================================================\n");
        printf("                           Available Movies                         \n");
        printf("======================================================================\n\n");
        setColor(15);
        for (int i = 0; i < MAX_MOVIES; i++) {
            printf("%d. %s\n", i + 1, movies[i]);
        }
        printf("%d. Back to Main Menu\n", MAX_MOVIES + 1);
        printf("----------------------------------------------------------------------\n");

        printf("Select a movie (1-%d): ", MAX_MOVIES + 1);
        int choice = getValidatedInput(1, MAX_MOVIES + 1);

        if (choice == MAX_MOVIES + 1) return;
        showMovieDetails(user, choice - 1);
    }
}

void cancelTicketMenu(User* user) {
    loadUserBookings(user);
    
    if (user->bookingCount == 0) {
        setColor(12);
        printf("You don't have any bookings to cancel.\n");
        setColor(15);
        printf("Press any key to continue...");
        _getch();
        return;
    }

    while (1) {
        system("cls");
        printf("Your Bookings:\n");
        printf("-----------------------\n");
        for (int i = 0; i < user->bookingCount; i++) {
            const Booking* booking = &user->bookings[i];
            printf("%d. Movie: %s\n", i + 1, booking->movie);
            printf("   Time: %s\n", booking->bookingTime);
            printf("   Seats: ");
            for (int j = 0; j < booking->seatCount; j++) {
                printf("%s ", booking->seats[j]);
            }
            printf("\n   Status: %s\n", booking->paid ? "Paid" : "Unpaid");
            printf("-----------------------\n");
        }
        printf("0. Back to Main Menu\n");
        printf("-----------------------\n");
        printf("Select a booking to cancel (1-%d or 0 to exit): ", user->bookingCount);
        
        int bookingChoice = getValidatedInput(0, user->bookingCount);
        
        if (bookingChoice == 0) {
            return;
        }
        
        Booking* selectedBooking = &user->bookings[bookingChoice - 1];
        strcpy(user->currentMovie, selectedBooking->movie);
        loadSeatsForMovie(user, user->currentMovie);
        
        while (1) {
            system("cls");
            printf("Selected Booking:\n");
            printf("Movie: %s\n", selectedBooking->movie);
            printf("Time: %s\n", selectedBooking->bookingTime);
            printf("Seats: ");
            for (int j = 0; j < selectedBooking->seatCount; j++) {
                printf("%s ", selectedBooking->seats[j]);
            }
            printf("\nStatus: %s\n", selectedBooking->paid ? "Paid" : "Unpaid");
            printf("-----------------------\n");
            
            printf("1. Cancel entire booking\n");
            printf("2. Cancel specific seats\n");
            printf("3. Back to booking list\n");
            printf("Enter your choice (1-3): ");
            
            int option = getValidatedInput(1, 3);
            
            if (option == 1) {
                for (int j = 0; j < selectedBooking->seatCount; j++) {
                    int seatIndex = seatLabelToIndex(selectedBooking->seats[j]);
                    user->bookedSeats[seatIndex] = false;
                }
        
                for (int j = bookingChoice - 1; j < user->bookingCount - 1; j++) {
                    user->bookings[j] = user->bookings[j+1];
                }
                user->bookingCount--;
                
                saveSeatsForMovie(user->currentMovie, user->bookedSeats);
                
                FILE* bookingFile = fopen("bookings.txt", "w");
                if (bookingFile) {
                    for (int j = 0; j < user->bookingCount; j++) {
                        const Booking* b = &user->bookings[j];
                        fprintf(bookingFile, "%s|%s|%s|%d|%ld|", 
                               user->username, b->movie, b->bookingTime, 
                               b->paid ? 1 : 0, b->bookingTimestamp);
                        
                        for (int k = 0; k < b->seatCount; k++) {
                            fprintf(bookingFile, "%s%s", b->seats[k], (k == b->seatCount - 1) ? "" : ",");
                        }
                        fprintf(bookingFile, "\n");
                    }
                    fclose(bookingFile);
                }
                
                printf("Booking successfully canceled!\n");
                printf("Press any key to continue...");
                _getch();
                
                if (user->bookingCount == 0) {
                    return;
                }
                
                break;
            }
            else if (option == 2) {
                printf("Enter seat numbers to cancel (comma separated, e.g., A1,B2): ");
                char seatsInput[100];
                fgets(seatsInput, sizeof(seatsInput), stdin);
                seatsInput[strcspn(seatsInput, "\n")] = '\0';
                
                char* seatToken = strtok(seatsInput, ",");
                bool allCancelled = true;
                
                while (seatToken) {
                    char seatLabel[MAX_SEAT_LABEL_LEN];
                    strncpy(seatLabel, seatToken, MAX_SEAT_LABEL_LEN);
                    
                    bool found = false;
                    for (int j = 0; j < selectedBooking->seatCount; j++) {
                        if (strcmp(selectedBooking->seats[j], seatLabel) == 0) {
                            int seatIndex = seatLabelToIndex(seatLabel);
                            user->bookedSeats[seatIndex] = false;
                            for (int k = j; k < selectedBooking->seatCount - 1; k++) {
                                strcpy(selectedBooking->seats[k], selectedBooking->seats[k+1]);
                            }
                            selectedBooking->seatCount--;
                            found = true;
                            break;
                        }
                    }
                    
                    if (!found) {
                        allCancelled = false;
                    }
                    
                    seatToken = strtok(NULL, ",");
                }
                
                if (allCancelled) {
                    printf("Selected seats successfully canceled!\n");
                } else {
                    printf("Some seats couldn't be canceled (may not belong to you).\n");
                }
                
                if (selectedBooking->seatCount == 0) {
                    for (int j = bookingChoice - 1; j < user->bookingCount - 1; j++) {
                        user->bookings[j] = user->bookings[j+1];
                    }
                    user->bookingCount--;
                }
                
                saveSeatsForMovie(user->currentMovie, user->bookedSeats);
                
                FILE* bookingFile = fopen("bookings.txt", "w");
                if (bookingFile) {
                    for (int j = 0; j < user->bookingCount; j++) {
                        const Booking* b = &user->bookings[j];
                        fprintf(bookingFile, "%s|%s|%s|%d|%ld|", 
                               user->username, b->movie, b->bookingTime, 
                               b->paid ? 1 : 0, b->bookingTimestamp);
                        
                        for (int k = 0; k < b->seatCount; k++) {
                            fprintf(bookingFile, "%s%s", b->seats[k], (k == b->seatCount - 1) ? "" : ",");
                        }
                        fprintf(bookingFile, "\n");
                    }
                    fclose(bookingFile);
                }
                
                printf("Press any key to continue...");
                _getch();
                if (user->bookingCount == 0) {
                    return;
                }
                break;
            }
            else if (option == 3) {
                break;
            }
        }
    }
}

void processPayment(User* user) {
    while (1) {
        system("cls");
        setColor(14);
        printf("============================================\n");
        printf("           SELECT PAYMENT METHOD           \n");
        printf("============================================\n");
        setColor(15);
        printf("1. Cash\n");
        printf("2. Debit/Credit Card\n");
        printf("3. Online Payment\n");
        printf("4. Cancel Payment\n");
        printf("============================================\n");
        printf("Enter your choice (1-4): ");
        
        int paymentChoice = getValidatedInput(1, 4);
        
        if (paymentChoice == 4) {
            return;
        }
        
        const char* paymentMethod;
        switch (paymentChoice) {
            case 1: paymentMethod = "Cash"; break;
            case 2: paymentMethod = "Debit/Credit Card"; break;
            case 3: paymentMethod = "Online Payment"; break;
        }
        for (int i = 0; i < user->bookingCount; i++) {
            if (!user->bookings[i].paid) {
                user->bookings[i].paid = true;
            }
        }

        FILE* bookingFile = fopen("bookings.txt", "w");
        if (bookingFile) {
            for (int i = 0; i < user->bookingCount; i++) {
                const Booking* b = &user->bookings[i];
                fprintf(bookingFile, "%s|%s|%s|%d|%ld|", 
                       user->username, b->movie, b->bookingTime, 
                       b->paid ? 1 : 0, b->bookingTimestamp);
                
                for (int j = 0; j < b->seatCount; j++) {
                    fprintf(bookingFile, "%s%s", b->seats[j], (j == b->seatCount - 1) ? "" : ",");
                }
                fprintf(bookingFile, "\n");
            }
            fclose(bookingFile);
        }
        
        system("cls");
        char paymentTime[20];
        getCurrentDateTime(paymentTime);
        setColor(14);
        printf("============================================\n");
        printf("            PAYMENT CONFIRMATION           \n");
        printf("============================================\n");
        setColor(15);
        printf("  User: %s\n", user->username);
        printf("  Payment Method: %s\n", paymentMethod);
        printf("  Status: CONFIRMED\n");
        printf("  Payment Time: %s\n", paymentTime);
        printf("============================================\n");
        printf("       Thank you for your payment!         \n");
        printf("============================================\n");
        
        printf("\n1. View Booking Summary\n");
        printf("2. Return to Main Menu\n");
        printf("Enter choice: ");
        
        int choice = getValidatedInput(1, 2);
        if (choice == 1) {
            system("cls");
            const double TICKET_PRICE = 110.0;
            setColor(14);
            printf("============================================\n");
            printf("            FINAL BOOKING SUMMARY          \n");
            printf("============================================\n");
            setColor(15);
            printf("  User: %s\n", user->username);
            printf("  City: %s\n", user->city);
            char currentTime[20];
            getCurrentDateTime(currentTime);
            printf("  Current Date: %s\n", currentTime);
            printf("============================================\n");
            
            double totalAmount = 0;
            
            for (int i = 0; i < user->bookingCount; i++) {
                const Booking* booking = &user->bookings[i];
                if (!booking->paid) continue;
                
                printf("  Movie: %s\n", booking->movie);
                printf("  Booking Time: %s\n", booking->bookingTime);
                printf("  Seats: ");
                for (int j = 0; j < booking->seatCount; j++) {
                    printf("%s ", booking->seats[j]);
                }
                printf("\n");
                printf("  Tickets: %d\n", booking->seatCount);
                double movieAmount = booking->seatCount * TICKET_PRICE;
                totalAmount += movieAmount;
                printf("  Amount: Rs.%.2f (Paid)\n", movieAmount);
                printf("--------------------------------------------\n");
            }
            
            printf("  TOTAL AMOUNT: Rs.%.2f\n", totalAmount);
            setColor(10);
            printf("============================================\n");
            printf("       All payments completed!             \n");
            printf("============================================\n");
            setColor(15);
            printf("\nPress any key to continue...");
            _getch();
        }
        return;
    }
}

void displayBill(User* user) {
    loadUserBookings(user);
    
    int validBookings = 0;
    for (int i = 0; i < user->bookingCount; i++) {
        if (user->bookings[i].seatCount > 0) {
            user->bookings[validBookings++] = user->bookings[i];
        }
    }
    user->bookingCount = validBookings;
    
    if (user->bookingCount == 0) {
        setColor(12);
        printf("No bookings found!\n");
        setColor(15);
        printf("Press any key to continue...");
        _getch();
        return;
    }
   
    bool allPaid = true;
    for (int i = 0; i < user->bookingCount; i++) {
        if (!user->bookings[i].paid) {
            allPaid = false;
            break;
        }
    }
    
    if (allPaid) {
        system("cls");
        const double TICKET_PRICE = 110.0;
        setColor(14);
        printf("============================================\n");
        printf("            FINAL BOOKING SUMMARY          \n");
        printf("============================================\n");
        setColor(15);
        printf("  User: %s\n", user->username);
        printf("  City: %s\n", user->city);
        char currentTime[20];
        getCurrentDateTime(currentTime);
        printf("  Current Date: %s\n", currentTime);
        printf("============================================\n");
        
        double totalAmount = 0;
        
        for (int i = 0; i < user->bookingCount; i++) {
            const Booking* booking = &user->bookings[i];
            printf("  Movie: %s\n", booking->movie);
            printf("  Booking Time: %s\n", booking->bookingTime);
            printf("  Seats: ");
            for (int j = 0; j < booking->seatCount; j++) {
                printf("%s ", booking->seats[j]);
            }
            printf("\n");
            printf("  Tickets: %d\n", booking->seatCount);
            double movieAmount = booking->seatCount * TICKET_PRICE;
            totalAmount += movieAmount;
            printf("  Amount: Rs.%.2f (Paid)\n", movieAmount);
            printf("--------------------------------------------\n");
        }
        
        printf("  TOTAL AMOUNT: Rs.%.2f\n", totalAmount);
        setColor(10);
        printf("============================================\n");
        printf("       All payments completed!             \n");
        printf("============================================\n");
        setColor(15);
        printf("\nPress any key to continue...");
        _getch();
        return;
    }
    
    system("cls");
    const double TICKET_PRICE = 110.0;
    double totalAmount = 0;
    setColor(14);
    printf("============================================\n");
    printf("          PENDING BOOKINGS (UNPAID)         \n");
    printf("============================================\n");
    setColor(15);
    
    for (int i = 0; i < user->bookingCount; i++) {
        const Booking* booking = &user->bookings[i];
        if (booking->paid) continue;
        
        printf("  Movie: %s\n", booking->movie);
        printf("  Booking Time: %s\n", booking->bookingTime);
        printf("  Seats: ");
        for (int j = 0; j < booking->seatCount; j++) {
            printf("%s ", booking->seats[j]);
        }
        printf("\n");
        printf("  Tickets: %d\n", booking->seatCount);
        double movieAmount = booking->seatCount * TICKET_PRICE;
        totalAmount += movieAmount;
        printf("  Amount: Rs.%.2f\n", movieAmount);
        printf("--------------------------------------------\n");
    }
    
    if (totalAmount > 0) {
        printf("  TOTAL PENDING: Rs.%.2f\n", totalAmount);
        printf("============================================\n");
        
        printf("\nPress any key to view payment options...");
        _getch();
        processPayment(user);
    } else {
        printf("  No unpaid bookings found!\n");
        printf("============================================\n");
        printf("\nPress any key to continue...");
        _getch();
    }
    
    bool hasPaidBookings = false;
    for (int i = 0; i < user->bookingCount; i++) {
        if (user->bookings[i].paid) {
            hasPaidBookings = true;
            break;
        }
    }
    
    if (hasPaidBookings) {
        system("cls");
        const double TICKET_PRICE = 110.0;
        setColor(14);
        printf("============================================\n");
        printf("            FINAL BOOKING SUMMARY          \n");
        printf("============================================\n");
        setColor(15);
        printf("  User: %s\n", user->username);
        printf("  City: %s\n", user->city);
        char currentTime[20];
        getCurrentDateTime(currentTime);
        printf("  Current Date: %s\n", currentTime);
        printf("============================================\n");
        
        double totalAmount = 0;
        
        for (int i = 0; i < user->bookingCount; i++) {
            const Booking* booking = &user->bookings[i];
            if (!booking->paid) continue;
            
            printf("  Movie: %s\n", booking->movie);
            printf("  Booking Time: %s\n", booking->bookingTime);
            printf("  Seats: ");
            for (int j = 0; j < booking->seatCount; j++) {
                printf("%s ", booking->seats[j]);
            }
            printf("\n");
            printf("  Tickets: %d\n", booking->seatCount);
            double movieAmount = booking->seatCount * TICKET_PRICE;
            totalAmount += movieAmount;
            printf("  Amount: Rs.%.2f (Paid)\n", movieAmount);
            printf("--------------------------------------------\n");
        }
        
        printf("  TOTAL AMOUNT: Rs.%.2f\n", totalAmount);
        setColor(10);
        printf("============================================\n");
        printf("       All payments completed!             \n");
        printf("============================================\n");
        setColor(15);
        printf("\nPress any key to continue...");
        _getch();
    }
}

void homepage(User* user) {
    if (strlen(user->currentMovie) > 0) {
        loadSeatsForMovie(user, user->currentMovie);
    }

    while (1) {
        system("cls");
        setColor(14);
        printf("======================================================================\n");
        printf("                  Movie Ticket Booking System                         \n");
        printf("======================================================================\n");
        setColor(11);
        printf("\nWelcome, %s (%s)\n", user->username, user->city);
        setColor(15);
        printf("1. Book Tickets\n");
        printf("2. Cancel Tickets\n");
        printf("3. View Booking Summary\n");
        printf("4. Logout\n");
        printf("\n----------------------------------------------------------------------\n");

        printf("Enter your choice (1-4): ");
        int choice = getValidatedInput(1, 4);

        switch (choice) {
            case 1: bookTicketMenu(user); break;
            case 2: cancelTicketMenu(user); break;
            case 3: displayBill(user); break;
            case 4: return;
        }
    }
}

int main() {
    User user = {0};
    user.loggedIn = false;
    user.bookingCount = 0;
    
    while (1) {
        system("cls");
        setColor(14);
        printf("======================================================================\n");
        printf("                  Welcome to Movie Ticket Booking                     \n");
        printf("======================================================================\n");
        setColor(15);
        printf("\n1. Register\n");
        printf("2. Login\n");
        printf("3. Admin: Reset All Bookings\n");
        printf("4. Exit\n");
        printf("\n----------------------------------------------------------------------\n");

        printf("Enter your choice (1-4): ");
        int choice = getValidatedInput(1, 4);

        switch (choice) {
            case 1:
                registerUser(&user);
                break;
            case 2:
                loginUser(&user);
                if (user.loggedIn) {
                    homepage(&user);
                }
                break;
            case 3:
                resetAllBookings();
                break;
            case 4:
                return 0;
        }
        
        printf("\nPress any key to continue...");
        _getch();
    }

    return 0;
}