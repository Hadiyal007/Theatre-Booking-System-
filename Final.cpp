#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> 
#include <stdlib.h>
#include <ctime>

using namespace std;
static int x=1;
static int b=1;
class User {
private:
    string username;
    string password;
    string city;
    int loggedin=0;
    
    string cities[5] = {"Ahmedabad", "Surat", "Nadiad", "Vadodara", "Rajkot"};

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
        

        while(true)
        {
            if(x)
            {
             system("CLS");
            }
        cout << "Enter a new username: ";
        cin >> username;

        if (userExists(username)) 
        {
            x=0;
            cout << "Username already exists! Try another one.\n";
        }
        else
        break;
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
        x=0;
        system("CLS");
        cout << "Sign-up successful! You can now log in.\n";
        login();

    }

    void login() {
        if(x)
        {
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
                break;
            }
        }
        file.close();
        
        if (loginSuccess) {
            system("CLS");
            cout << "Login successful! Welcome, " << username << ".\n";
            cout << "Your registered city is: " <<city  << ".\n";
            loggedin = 1;
        }
         else
         {
            cout << "Incorrect username or password. Try again.\n";
        }
    }

    int Loggedin()
    {
        return loggedin;
    }

    time_t getCurrentTime() {
        return time(0);
    }

    void homepage()
    {
        system("CLS");
        int choice;
        cout << "\n********************\n";
        cout << "1.book ticket\n";
        cout << "2.remove ticket\n";
        cout << "3.bill\n";
        cout << "4.exit\n";
        cout << "********************\n";
        while(1){ 
        cout << "Enter your choice (1-4) : " ;                
        cin >> choice;     
        switch (choice) {
            case 1:bookticket(); break;
            case 2:;break;
            case 3:; break;
            case 4:exit(0);
            default: cout << "Invalid choice! Try again.\n";continue;
        }
       }
    }

    void bookticket()
    {
        system("CLS");
        int choice;        
        cout << "which movie ticket you want to book \n";
        cout << "\n********************\n";
        cout << "1.Chhava\n";
        cout << "2.Bahubali 2\n";
        cout << "3.Pushpa 2\n";
        cout << "4.KGF 2\n";
        cout << "5.Avengers Endgame\n";
        cout << "6.Captain America : Brave New World\n";
        cout << "********************\n";
        cout << "If you want go previous page enter 0.\n";
        while(1){
            cout << "Enter your choice (0-6) : " ;
            cin >> choice;
            switch (choice) {
                case 0:homepage(); break;
                case 1:movie1(); break;
                case 2:movie2();break;
                case 3:movie3(); break;
                case 4:movie4(); break;
                case 5:movie5(); break;
                case 6:movie6(); break;
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }

    void movie1()
    {
        system("CLS");
        cout << "\n********************\n\n";
        cout << "Movie Name : Chhava \n\n";
        cout << "Movie Details : The film explores the period after the death of Chhatrapati Shivaji Maharaj,\nwhen his son, Sambhaji, takes up the mantle of leadership and continues the Maratha resistance \nagainst the expanding Mughal empire led by Aurangzeb. \n\n";
        cout << "Movie Cast : Vicky Kaushal, Rashmika Mandanna, Akshaye Khanna,Ashutosh Rana, Divya Dutta. \n\n";
        cout << "********************\n\n";
        int choice;
        cout << "Enter 1 for book ticket.\n";
        cout << "If you want to choose another movie enter 0.\n";
        cout << "Enter your choice (0/1) : ";
        while(1)
        {
            cin >> choice;
            if(choice==0)
            {
                bookticket();
            }
            else if(choice==1)
            {
                show1();
                break;
            }
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 1 for book ticket : ";

                continue;
            }
        } 
    }

    void movie2()
    {
        system("CLS");
        cout << "\n********************\n\n";
        cout << "Movie Name : Bahubali 2 \n\n";
        cout << "Movie Details : The Baahubali movie series, set in the fictional kingdom of Mahishmati, \nfollows the story of Shivudu,who discovers his true identity as the son of the slain \nprince Amarendra Baahubali, and seeks revenge against his uncle Bhallala Deva,\nwho usurped the throne. \n\n";
        cout << "Movie Cast : Prabhas, Rana Daggubatti, Ramya Krishnan, Satyraj, Nasir, Subbaraju\n\n";
        cout << "********************\n\n";
        int choice;
        cout << "Enter 1 for book ticket.\n";
        cout << "If you want to choose another movie enter 0.\n";
        cout << "Enter your choice (0/1) : ";
        while(1)
        {
            cin >> choice;
            if(choice==0)
            {
                bookticket();
            }
            else if(choice==1)
            {
                show2();
                break;
            }
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 1 for book ticket : ";

                continue;
            }
        } 
    }

    void movie3()
    {
        system("CLS");
        cout << "\n********************\n\n";
        cout << "Movie Name : Pushpa 2 \n\n";
        cout << "Movie Details : Pushpa: The Rise follows Pushpa Raj, a laborer who rises through the ranks of a red sandalwood \nsmuggling syndicate, facing powerful enemies and ultimately becoming a respected smuggler, while Pushpa: \nThe Rule sees him navigating a world of politics and rivalries as he faces a vengeful rival and makes big deals.\n\n";
        cout << "Movie Cast : Allu Arjun, Rashmika Mandana, Farhad Fasil, Sunil, Anusuya Baradwaj, Jagdeesh, Jagpati Babu\n\n";
        cout << "********************\n\n";
        int choice;
        cout << "Enter 1 for book ticket.\n";
        cout << "If you want to choose another movie enter 0.\n";
        cout << "Enter your choice (0/1) : ";
        while(1)
        {
            cin >> choice;
            if(choice==0)
            {
                bookticket();
            }
            else if(choice==1)
            {
                show3();
                break;
            }
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 1 for book ticket : ";

                continue;
            }
        } 
    }

    void movie4()
    {
        system("CLS");
        cout << "\n********************\n\n";
        cout << "Movie Name : KGF 2 \n\n";
        cout << "Movie Details : The K.G.F movie series follows Rocky, a mercenary, who rises to power in \nthe Kolar Gold Fields (KGF) after fulfilling his mother's promise to become rich and powerful, \nultimately becoming a feared overlord and battling enemies to maintain his control. \n\n";
        cout << "Movie Cast : Yash, Sreenidhi Sheti, Sanjay Dutt, Ravina Tondon, Prakashraj\n\n";
        cout << "********************\n\n";
        int choice;
        cout << "Enter 1 for book ticket.\n";
        cout << "If you want to choose another movie enter 0.\n";
        cout << "Enter your choice (0/1) : ";
        while(1)
        {
            cin >> choice;
            if(choice==0)
            {
                bookticket();
            }
            else if(choice==1)
            {
                show4();
                break;
            }
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 1 for book ticket : ";

                continue;
            }
        } 
    }

    void movie5()
    {
        system("CLS");
        cout << "\n********************\n\n";
        cout << "Movie Name : Avengers Endgame \n\n";
        cout << "Movie Details : Plot. In 2018, 23 days after Thanos erased half of all life in the universe, Carol Danvers \nrescues Tony Stark and Nebula from deep space. They reunite with the remaining Avengers-Bruce Banner, \nSteve Rogers, Thor, Natasha Romanoff, and James Rhodes-and Rocket on Earth.\n\n";
        cout << "Movie Cast : Robert Dawney Jr., Chris Evans, Chris Hamsworth, Tom Holand, Mark Ruffelo, Scarlet Johnson\n\n";
        cout << "********************\n\n";
        int choice;
        cout << "Enter 1 for book ticket.\n";
        cout << "If you want to choose another movie enter 0.\n";
        cout << "Enter your choice (0/1) : ";
        while(1)
        {
            cin >> choice;
            if(choice==0)
            {
                bookticket();
            }
            else if(choice==1)
            {
                show5();
                break;
            }
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 1 for book ticket : ";

                continue;
            }
        } 
    }

    void movie6()
    {
        system("CLS");
        cout << "\n********************\n\n";
        cout << "Movie Name : Captain America : Brave New World \n\n";
        cout << "Movie Details : In Captain America: Brave New World, Sam Wilson, the new Captain America, grapples with \nthe weight of the mantle while facing an international crisis and a potential global conflict, all \nwhile navigating a complex political landscape and the reappearance of old foes. \n\n";
        cout << "Movie Cast : Anthony Mackie, Harrison Ford, Danny Ramirez and Tim Blake Nelson. \n\n";
        cout << "********************\n\n";
        int choice;
        cout << "Enter 1 for book ticket.\n";
        cout << "If you want to choose another movie enter 0.\n";
        cout << "Enter your choice (0/1) : ";
        while(1)
        {
            cin >> choice;
            if(choice==0)
            {
                bookticket();
            }
            else if(choice==1)
            {
                show6();
                break;
            }
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 1 for book ticket : ";

                continue;
            }
        } 
    }

    void show1()
    {
        system("CLS");
        cout << "\n************************\n";
        cout << "1.Morning Show(10.00 AM)\n";
        cout << "2.Evening Show(04.00 PM)\n";
        cout << "3.Morning Show(09.00 PM)\n";
        cout << "************************\n";
        int choice;
        cout << "If you want to jump previus page enter 0.\n";
       
        while(1)
        {
            cout << "Enter your choice (0-3) : ";
            cin >> choice;
            switch (choice) {
                case 0:movie1(); break;
                case 1:seating1(); break;
                case 2:seating2();break;
                case 3:seating3(); break;
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }
    
    void show2()
    {
        system("CLS");
        cout << "\n************************\n";
        cout << "1.Morning Show(10.00 AM)\n";
        cout << "2.Evening Show(04.00 PM)\n";
        cout << "3.Morning Show(09.00 PM)\n";
        cout << "************************\n";
        int choice;
        cout << "If you want to jump previus page enter 0.\n";
       
        while(1)
        {
            cout << "Enter your choice (0-3) : ";
            cin >> choice;
            switch (choice) {
                case 0:movie2(); break;
                case 1:seating4(); break;
                case 2:seating5();break;
                case 3:seating6(); break;
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }

    void show3()
    {
        system("CLS");
        cout << "\n************************\n";
        cout << "1.Morning Show(10.00 AM)\n";
        cout << "2.Evening Show(04.00 PM)\n";
        cout << "3.Morning Show(09.00 PM)\n";
        cout << "************************\n";
        int choice;
        cout << "If you want to jump previus page enter 0.\n";
       
        while(1)
        {
            cout << "Enter your choice (0-3) : ";
            cin >> choice;
            switch (choice) {
                case 0:movie3(); break;
                case 1:seating7(); break;
                case 2:seating8();break;
                case 3:seating9(); break;
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }

    void show4()
    {
        system("CLS");
        cout << "\n************************\n";
        cout << "1.Morning Show(10.00 AM)\n";
        cout << "2.Evening Show(04.00 PM)\n";
        cout << "3.Morning Show(09.00 PM)\n";
        cout << "************************\n";
        int choice;
        cout << "If you want to jump previus page enter 0.\n";
       
        while(1)
        {
            cout << "Enter your choice (0-3) : ";
            cin >> choice;
            switch (choice) {
                case 0:movie4(); break;
                case 1:seating10(); break;
                case 2:seating11();break;
                case 3:seating12(); break;
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }

    void show5()
    {
        system("CLS");
        cout << "\n************************\n";
        cout << "1.Morning Show(10.00 AM)\n";
        cout << "2.Evening Show(04.00 PM)\n";
        cout << "3.Morning Show(09.00 PM)\n";
        cout << "************************\n";
        int choice;
        cout << "If you want to jump previus page enter 0.\n";
       
        while(1)
        {
            cout << "Enter your choice (0-3) : ";
            cin >> choice;
            switch (choice) {
                case 0:movie5(); break;
                case 1:seating13(); break;
                case 2:seating14();break;
                case 3:seating15(); break;
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }

    void show6()
    {
        system("CLS");
        cout << "\n************************\n";
        cout << "1.Morning Show(10.00 AM)\n";
        cout << "2.Evening Show(04.00 PM)\n";
        cout << "3.Morning Show(09.00 PM)\n";
        cout << "************************\n";
        int choice;
        cout << "If you want to jump previus page enter 0.\n";
       
        while(1)
        {
            cout << "Enter your choice (0-3) : ";
            cin >> choice;
            switch (choice) {
                case 0:movie6(); break;
                case 1:seating16(); break;
                case 2:seating17();break;
                case 3:seating18(); break;
                default: cout << "Invalid choice! Try again.\n"; continue;
            }
        }
    }

    void seating1() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats1.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats1.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating2() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats2.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats2.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating3() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats3.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats3.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating4() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats4.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats4.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating5() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats5.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats5.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating6() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats6.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats6.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating7() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats7.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats7.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating8() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats8.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats8.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating9() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats9.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats9.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating10() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats10.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats10.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating11() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats11.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats11.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating12() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats12.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats12.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating13() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats13.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats13.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating14() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats14.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats14.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating15() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats15.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats15.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating16() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats16.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats16.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating17() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats17.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats17.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }

    void seating18() {
        system("CLS");
        int numSeats, selseat;
        bool seatarr[106] = {false};
        time_t storedTime, currentTime = getCurrentTime();
    
        ifstream infile("seats18.dat", ios::binary);
        if (infile) {
            infile.read(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
            infile.read(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
            infile.close();
            
            if (difftime(currentTime, storedTime) > 86400) { 
                cout << "Seats have been reset after 24 hours.\n";
                fill(begin(seatarr), end(seatarr), false); 
                storedTime = currentTime;
            }
        } else {
            storedTime = currentTime; 
        }    
        char rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        int seatno = 1;
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";  
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";    
        cout << "How many seats do you want to book? ";
        cin >> numSeats;   
        if (numSeats < 1 || numSeats > 105) {
            cout << "Invalid number of seats. Please try again.\n";
            return;
        }
        for (int i = 0; i < numSeats; i++) {
            cout << "Enter seat number to book (1-105): ";
            cin >> selseat;
    
            if (selseat < 1 || selseat > 105) {
                cout << "Invalid seat number. Please try again.\n";
                i--;
                continue;
            }
            if (seatarr[selseat]) {
                cout << "Seat already booked! Choose another seat.\n";
                i--;
                continue;
            }
            seatarr[selseat] = true;
        }
    
        ofstream outfile("seats18.dat", ios::binary);
        outfile.write(reinterpret_cast<char*>(&storedTime), sizeof(time_t)); 
        outfile.write(reinterpret_cast<char*>(seatarr), 106 * sizeof(bool)); 
        outfile.close();
    
        system("CLS");
        seatno = 1;
        rowLabel = 'A';
        cout << "\n------------------------ SEAT LAYOUT -------------------------\n";
        for (int i = 0; i < 7; i++) {
            cout << rowLabel << "  ";
            for (int j = 0; j < 15 && seatno <= 105; j++) {
                if (seatarr[seatno])
                    cout << " X  ";
                else if (seatno < 10)
                    cout << "00" << seatno << " ";
                else if (seatno < 100)
                    cout << "0" << seatno << " ";
                else
                    cout << seatno << " ";
                seatno++;
            }
            rowLabel++;
            cout << endl;
        }
        cout << "--------------------------------------------------------------\n";
        int choice ;
        cout << "Enter 0 For go to Previus Page : ";
        while(1){
            cin >> choice;
            if(choice==0)
                show1();
            else{
                cout << "Invalid input! Please enter a number.\n";
                cout << "Please,Enter 0 for go to previus page : ";
                continue;
            }
        } 
    }
    
};

int main() {
    User user;
    int choice;
    //system("CLS");
    while (b) {
        if(user.Loggedin()) break;
        cout << "\n\n|| If you are a new user, please sign up first! ||"; 
        cout << "\n********************\n";
        cout << "1. Sign Up\n2. Login\n3. Exit\n";
        cout << "********************\n";
        cout << "Enter your choice: ";
        

        if (!(cin >> choice)) {

            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: user.signUp(); break;
            case 2: user.login();break;
            case 3: cout << "Exiting program...\n"; return 0;
            default: cout << "Invalid choice! Try again.\n";
        }
        
    }
    user.homepage();
    return 0;
}