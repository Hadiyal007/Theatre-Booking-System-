#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> // For getch() on Windows
#include<stdlib.h>

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

    // Function to check if username already exists
    bool userExists(const string& user) {
        ifstream file("users.txt");
        if (!file) return false;  // File does not exist yet

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

    // Function to take hidden password input
    string getPassword() {
        string pass;
        char ch;
        cout << "Enter password: ";
        while (true) {
            ch = _getch();
            if (ch == '\r') break;  // Enter key pressed
            if (ch == '\b' && !pass.empty()) {  // Handle backspace
                pass.pop_back();
                cout << "\b \b";  // Move cursor back, erase *, move back again
            } else if (ch != '\b') {
                pass.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        return pass;
    }

public:
    // Function to display city options
    void displayCities() {
        cout << "Select your city:\n";
        for (int i = 0; i < 5; i++) {
            cout << i + 1 << ". " << cities[i] << endl;
        }
    }
     
    // Function to sign up a new user
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
        
        while (!(cin >> choice) || choice < 1 || choice > 5) {  // Input validation
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

    // Function to log in a user
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
        cout << "Enter your choice (1-4) : " ;
        cin >> choice;
        switch (choice) {
            case 1:bookticket(); break;
            case 2:;break;
            case 3:; break;
            case 4:; break;
            default: cout << "Invalid choice! Try again.\n";
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
            default: cout << "Invalid choice! Try again.\n";
        }
    }



    void movie1()
    {
        system("CLS");
        cout << "\n********************\n";
        cout << "Movie Name : Chhava \n";
        cout << "Movie Details : \n";
        cout << "Movie Cast : \n";
        cout << "********************\n";
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
        
    }

    void movie3()
    {
        
    }

    void movie4()
    {
        
    }

    void movie5()
    {
        
    }

    void movie6()
    {
        
    }

    void show1()
    {
        
    }
};

// Main menu
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