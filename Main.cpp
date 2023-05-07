#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define ROW_NUM 10
#define SEAT_NUM 10

using namespace std;

class Movie
{
private:
    int id;
    string name;
    int duration;
    float rating;

private:
    void SetDetails();

    friend istream& operator>>(istream& file, Movie& m);
    friend ostream& operator<<(ostream& file, Movie& m);
    friend class Theater;
    friend class Billing;
};

void Movie::SetDetails()
{
    cout << "Movie Name: ";
    cin >> name;
    cout << "Duration: ";
    cin >> duration;
    cout << "Rating: ";
    cin >> rating;
}

istream& operator>>(istream& file, Movie& m)
{
    string temp;

    try
    {
        getline(file, m.name);

        getline(file, temp);
        m.duration = stoi(temp);

        getline(file, temp);
        m.rating = stof(temp);

        getline(file, temp);
    }
    catch (...)
    {
        // cout << "Exception(in Movie)" << endl;
    }

    return file;
}

ostream& operator<<(ostream& file, Movie& m)
{
    file << m.name << endl;
    file << m.duration << endl;
    file << m.rating << endl;
    file << "--------" << endl;

    return file;
}

class Theater
{
private:
    int movieNum;
    string name;
    string location;
    int timeSlot[6] = {9, 12, 15, 18, 21, 0};
    vector<Movie> movies;

public:
    Theater();
    int DisplayMovies();
    void DisplayMovieDetails(int id);
    int DisplayTimeSlots();
    Movie GetMovie(int id);
    int GetTimeSlot(int time) { return timeSlot[time + 1]; }
    void AddMovie();
    void DeleteMovie(string name);

    friend class Manager;
};

Theater::Theater()
{
    ifstream movieFile("Movies.txt", ios::in);

    int i = 0;
    Movie m;
    if (movieFile.is_open())
    {
        while (movieFile.good())
        {
            movieFile >> m;
            m.id = i + 1;
            movies.push_back(m);
            i++;
        }
    }
    movieNum = i - 1;
    movieFile.close();
}

void Theater::AddMovie()
{
    Movie m;
    m.SetDetails();

    ofstream movieFile("Movies.txt", ios::app);
    movieFile << m;
    movieNum++;
    m.id = movieNum;
    movies.pop_back();  // some error causes empty record to be pushed
    movies.push_back(m);

    for (int i = 0; i < movieNum; i++)
    {
        cout << "\t[" << i + 1 << "] " << movies[i].name << endl;
    }

    cout << endl;
    cout << "NEW MOVIE ADDED SUCCESSFULLY" << endl;
}

void Theater::DeleteMovie(string name)
{
    ifstream movieFile("Movies.txt", ios::in);
    ofstream temp("temp.txt", ios::out);

    Movie curr;
    if (movieFile.is_open())
    {
        while (movieFile >> curr)
        {
            if (!(curr.name == name))
            {
                temp << curr;
            }
        }
    }
    movieFile.close();
    temp.close();

    remove("Movies.txt");
    rename("temp.txt", "Movies.txt");

    ifstream mFile("Movies.txt", ios::in);
    movies.clear();
    int i = 0;
    Movie m;
    if (mFile.is_open())
    {
        while (mFile.good())
        {
            mFile >> m;
            m.id = i + 1;
            movies.push_back(m);
            i++;
        }
    }
    movieNum = i - 1;
    mFile.close();

    cout << endl;
    cout << "MOVIE REMOVED SUCCESSFULLY" << endl << endl;
}

int Theater::DisplayMovies()
{
    int choice;

    cout << endl;
    cout << "Select a Movie" << endl << endl;

    for (int i = 0; i < movieNum; i++)
    {
        cout << "\t[" << i + 1 << "] " << movies[i].name << endl;
    }

    cout << endl;
    cout << "Movie number: ";
    cin >> choice;

    try
    {
        if (choice > 0 && choice <= movieNum)
        {
            return choice;
        }
        throw choice;
    }
    catch (int choice)
    {
        cout << endl;
        cout << "INVALID CHOICE. ENTER CORRECT MOVIE NUMBER." << endl << endl;
        return -1;
    }
}

void Theater::DisplayMovieDetails(int id)
{
    int index = id - 1;
    if (index >= 0)
    {
        cout << endl;
        cout << "--------Movie Details--------" << endl;
        cout << "Movie Name:\t" << movies[index].name << endl;
        cout << "Duration:\t" << movies[index].duration << " minutes" << endl;
        cout << "Ratings:\t" << movies[index].rating << " / 10" << endl;
    }
}

int Theater::DisplayTimeSlots()
{
    int choice;

    cout << endl;
    cout << "Select a time slot" << endl << endl;

    for (int i = 0; i < 5; i++)
    {
        cout << "\t[" << i + 1 << "] " << timeSlot[i] << ":00 to "
             << timeSlot[i + 1] << ":00" << endl;
    }

    cout << endl;
    cout << "Slot number: ";
    cin >> choice;
    cout << endl;

    try
    {
        if (choice > 0 && choice <= 5)
        {
            return choice;
        }
        throw choice;
    }
    catch (int choice)
    {
        cout << endl;
        cout << "INVALID CHOICE. ENTER CORRECT SLOT NUMBER" << endl << endl;
        return -1;
    }
}

Movie Theater::GetMovie(int id) { return movies[id - 1]; }

class Ticket
{
private:
    int ticketNum;
    int timeSlot;
    int movieID;
    int seatNo, rowNo;
    int price;
    string type;

private:
    int SetRow();
    void SetSeat();
    void SetType();
    void SetPrice();
    void SetTicketNum();

public:
    void SetDetails(int slot, int mno);

    friend istream& operator>>(istream& file, Ticket& t);
    friend ostream& operator<<(ostream& file, Ticket& t);
    friend class Booking;
    friend class Billing;
};

void Ticket::SetDetails(int slot, int mno)
{
    SetTicketNum();
    timeSlot = slot;
    movieID = mno;
    SetRow();
    SetSeat();
    SetType();
    SetPrice();
}

int Ticket::SetRow()
{
    char c;
    cout << "Row: ";
    cin >> c;
    int i = c;

    while (i < 'A' || i > (ROW_NUM + 'A' - 1))  // i > (6 + 65 - 1)
    {
        cout << "Enter the correct row (capital): ";
        cin >> c;
        i = c;
    }

    rowNo = i - 'A' + 1;  // 65 - 65 + 1, 66 - 65 + 1
    return rowNo;
}

void Ticket::SetSeat()
{
    cout << "Seat No: ";
    cin >> seatNo;
    while (seatNo <= 0 || seatNo > SEAT_NUM)
    {
        cout << "Enter the correct seat no: ";
        cin >> seatNo;
    }
}

void Ticket::SetType()
{
    if (rowNo <= 3)
    {
        type = "Platinum";
    }
    else if (rowNo > 3 && rowNo <= 6)
    {
        type = "Gold";
    }
    else if (rowNo > 6 && rowNo <= 10)
    {
        type = "Silver";
    }
    cout << "Type: " << type << endl << endl;
}

void Ticket::SetPrice()
{
    if (type == "Platinum")
        price = 750;
    else if (type == "Gold")
        price = 500;
    else if (type == "Silver")
        price = 250;
}

void Ticket::SetTicketNum() { ticketNum = rand() % 99 + 100; }

istream& operator>>(istream& file, Ticket& t)
{
    string temp;
    try
    {
        getline(file, temp);
        t.ticketNum = stoi(temp);

        getline(file, temp);
        t.timeSlot = stoi(temp);

        getline(file, temp);
        t.movieID = stoi(temp);

        getline(file, temp);
        t.rowNo = stoi(temp);

        getline(file, temp);
        t.seatNo = stoi(temp);

        getline(file, temp);
        t.price = stoi(temp);

        getline(file, t.type);
        getline(file, temp);
    }
    catch (exception e)
    {
        // cout << "Exception(in Ticket)" << endl;
    }

    return file;
}

ostream& operator<<(ostream& file, Ticket& t)
{
    file << t.ticketNum << endl;
    file << t.timeSlot << endl;
    file << t.movieID << endl;
    file << t.rowNo << endl;
    file << t.seatNo << endl;
    file << t.price << endl;
    file << t.type << endl;
    file << "--------" << endl;

    return file;
}

class Billing
{
public:
    static void PrintReceipt(Ticket t);
};

void Billing::PrintReceipt(Ticket t)
{
    Theater theater;
    Movie m = theater.GetMovie(t.movieID);
    int time = theater.GetTimeSlot(t.timeSlot);
    char row = t.rowNo + 64;

    system("cls");
    cout << "======================================================\n"
         << "                       RECEIPT                        \n"
         << "======================================================\n";

    cout << "Ticket Number: " << t.ticketNum << endl;
    cout << "Movie Name: " << m.name << "\t\tTime: " << time << ":00" << endl;
    cout << "Duration: " << m.duration << " minutes" << endl;
    cout << "Seat Type: " << t.type << endl;
    cout << "Seat: " << row << t.seatNo << endl;
    cout << "Price: Rs. " << t.price << endl;
    cout << "------------------------------------------------------" << endl
         << endl;
}

class Booking
{
private:
    int bookedSeats[5 + 1][20 + 1][ROW_NUM + 1][SEAT_NUM + 1] = {0};
    vector<Ticket> tickets;

private:
    int CheckSeat(int timeSlot, int mId, int row, int seat);
    void ReserveSeat(Ticket t);
    void RemoveTicketFromFile(int tNum);
    Ticket GetTicket(int tNum);

public:
    Booking();
    void BookTicket(int slot, int mno);
    void CancelTicket(int tNum);
    void DisplayAvailableSeats(int timeSlot, int mId);
};

Booking::Booking()
{
    ifstream booking("Tickets.txt", ios::in);

    Ticket t;

    if (booking.is_open())
    {
        while (booking >> t)
        {
            bookedSeats[t.timeSlot][t.movieID][t.rowNo][t.seatNo] = 1;
            tickets.push_back(t);
        }
        booking.close();
    }
}

void Booking::BookTicket(int slot, int mno)
{
    Ticket t;
    t.SetDetails(slot, mno);
    ReserveSeat(t);
}

int Booking::CheckSeat(int timeSlot, int mId, int row, int seat)
{
    return bookedSeats[timeSlot][mId][row][seat];
}

void Booking::ReserveSeat(Ticket t)
{
    ofstream booking("Tickets.txt", ios::app);

    if (CheckSeat(t.timeSlot, t.movieID, t.rowNo, t.seatNo) == 0)
    {
        booking << t;
        tickets.push_back(t);
        bookedSeats[t.timeSlot][t.movieID][t.rowNo][t.seatNo] = 1;
        Billing::PrintReceipt(t);
    }
    else
    {
        cout << "SEAT IS RESERVED. SELECT ANOTHER SEAT" << endl;
    }
}

void Booking::RemoveTicketFromFile(int tNum)
{
    ifstream booking("Tickets.txt", ios::in);
    ofstream temp("temp.txt", ios::out);

    Ticket curr;
    if (booking.is_open())
    {
        while (booking >> curr)
        {
            if (!(curr.ticketNum == tNum))
            {
                temp << curr;
            }
        }
    }
    booking.close();
    temp.close();

    remove("Tickets.txt");
    rename("temp.txt", "Tickets.txt");

    cout << endl;
    cout << "TICKET CANCELLED SUCCESSFULLY" << endl << endl;
}

void Booking::CancelTicket(int tNum)
{
    Ticket t;
    bool flag = false;

    for (int i = 0; i < tickets.size(); i++)
    {
        if (tickets[i].ticketNum == tNum)
        {
            t = tickets[i];
            flag = true;
        }
    }

    if (flag)
    {
        if (CheckSeat(t.timeSlot, t.movieID, t.rowNo, t.seatNo) == 1)
        {
            bookedSeats[t.timeSlot][t.movieID][t.rowNo][t.seatNo] = 0;
            RemoveTicketFromFile(tNum);
        }
    }
    else
    {
        cout << endl;
        cout << "TICKET DOESN'T EXIST IN OUR DATABASE. DID YOU ENTER THE "
                "CORRECT TICKET NUMBER?"
             << endl
             << endl;
    }
}

void Booking::DisplayAvailableSeats(int timeSlot, int mId)
{
    int skip = 0;

    cout << endl << endl;
    cout << "--------Displaying Seats--------" << endl;
    for (int i = 1; i <= ROW_NUM; i++)
    {
        if (i <= 3 && skip % 2 == 0)
        {
            cout << "[Platinum]" << endl;
            skip++;
        }
        else if (i > 3 && i <= 6 && skip % 2 == 1)
        {
            cout << endl;
            cout << "[Gold]" << endl;
            skip++;
        }
        else if (i > 6 && i <= 10 && skip % 2 == 0)
        {
            cout << endl;
            cout << "[Silver]" << endl;
            skip++;
        }
        char c = i + 64;
        cout << c << ": ";

        for (int j = 1; j <= SEAT_NUM; j++)
        {
            if (CheckSeat(timeSlot, mId, i, j))
            {
                cout << "\t[X]";
            }
            else
            {
                cout << "\t[" << j << "]";
            }
        }
        cout << endl;
    }

    cout << endl;
    cout << "\t\t\t-----------------------------------" << endl;
    cout << "\t\t\t     All Eyes This Way Please!" << endl;
    cout << endl << endl;
    cout << "X: Seat Taken" << endl << endl << endl;

    cout << "--------Price Details--------" << endl;
    cout << "Platinum: \tRs. 750" << endl;
    cout << "Gold: \t\tRs. 500" << endl;
    cout << "Silver: \tRs. 250" << endl << endl;
}

class Customer
{
protected:
    string name, phone, email;
    int numTicket;

private:
    void SetName();
    void SetPhone();
    void SetEmail();

public:
    void SetDetails();
};

void Customer::SetDetails()
{
    SetName();
    SetPhone();
    SetEmail();
}

void Customer::SetName()
{
    cout << "Enter your name: ";
    cin >> name;
}

void Customer::SetPhone()
{
    cout << "Enter your phone number: ";
    cin >> phone;

    while (phone.length() != 10)
    {
        cout << endl;
        cout << "Phone number should have 10 digits" << endl;
        cout << "Enter a valid phone number: ";
        cin >> phone;
    }
}

void Customer::SetEmail()
{
    int validEmail = 0;

    cout << "Enter your email address: ";
    cin >> email;

    while (!validEmail)
    {
        for (int i = 0; i < email.length(); ++i)
        {
            if (email.at(i) == '@')
            {
                validEmail = 1;
                return;
            }
        }

        cout << "INVALID EMAIL ADDRESS" << endl;
        cout << "Enter a valid email address: ";
        cin >> email;
    }
}

class Member : Customer
{
private:
    string accountNumber, password;

private:
    void SetPassword();

public:
    void Register();

    friend istream& operator>>(istream& file, Member& m);
    friend ostream& operator<<(ostream& file, Member& m);
    friend class MemberDatabase;
};

void Member::Register()
{
    cout << endl;
    cout << "Registering for Membership:" << endl;

    Customer::SetDetails();
    accountNumber = to_string((rand() % 9999) + 10000);
    SetPassword();
}

void Member::SetPassword()
{
    string temp;
    int flag = 1;

    while (flag)
    {
        cout << "Enter your password: ";
        cin >> password;
        cout << "Reenter your password: ";
        cin >> temp;
        if (temp != password)
        {
            cout << "Password does not match" << endl;
            flag = 1;
        }
        else
        {
            flag = 0;
        }
    }
}

istream& operator>>(istream& file, Member& m)
{
    string temp;
    getline(file, m.accountNumber);
    getline(file, m.name);
    getline(file, m.email);
    getline(file, m.phone);
    getline(file, m.password);
    getline(file, temp);

    return file;
}

ostream& operator<<(ostream& file, Member& m)
{
    file << m.accountNumber << endl;
    file << m.name << endl;
    file << m.email << endl;
    file << m.phone << endl;
    file << m.password << endl;
    file << "----------" << endl;

    return file;
}

class MemberDatabase
{
private:
    vector<Member> members;

public:
    MemberDatabase();
    void SaveRecord(Member m);
    bool Login(string name);
    bool CheckPassword(Member m, string password);
    void DisplayMemberInformation();
};

MemberDatabase::MemberDatabase()
{
    Member member;
    ifstream memberFile("Members.txt", ios::in);

    if (memberFile.is_open())
    {
        while (memberFile >> member)
        {
            members.push_back(member);
        }
    }

    memberFile.close();
}

void MemberDatabase::SaveRecord(Member m)
{
    ofstream memberFile("Members.txt", ios::app);

    if (memberFile.is_open())
    {
        memberFile << m;
        members.push_back(m);
        cout << "Record saved successfully" << endl;
    }

    memberFile.close();
}

bool MemberDatabase::CheckPassword(Member m, string password)
{
    if (m.password == password)
    {
        return true;
    }
    return false;
}

bool MemberDatabase::Login(string name)
{
    string password;
    bool logged = false;
    for (int i = 0; i < members.size(); i++)
    {
        if (name == members[i].name)
        {
            Member m = members[i];
            cout << "Password: ";
            cin >> password;
            if (CheckPassword(m, password))
            {
                logged = true;
                cout << "Logged In successfully" << endl << endl;
            }
            else
            {
                cout << "WRONG PASSWORD" << endl << endl;
            }
            break;
        }
    }
    if (!logged)
    {
        cout << "MEMBER DOESN'T EXIST. SIGNUP FIRST." << endl << endl;
    }
    return logged;
}

void MemberDatabase::DisplayMemberInformation()
{
    cout << "=========================================================" << endl;
    cout << "                   MEMBER INFORMATION                    " << endl;
    cout << "=========================================================" << endl;

    for (int i = 0; i < members.size(); i++)
    {
        cout << "Account Number : " << members[i].accountNumber << endl;
        cout << "Name           : " << members[i].name << endl;
        cout << "Phone no.      : " << members[i].phone << endl;
        cout << "E-mail         : " << members[i].email << endl;
        cout << "---------------------------------------------------------"
             << endl;
    }
}

class Manager
{
private:
    string name, password;

public:
    bool Login();
};

bool Manager::Login()
{
    int chance = 3;
    bool flag = false;

    while (chance > 0)
    {
        cout << "Username: ";
        cin >> name;
        cout << "Password: ";
        cin >> password;

        if (name == "admin" && password == "admin")
        {
            cout << "Welcome to Manager Menu" << endl;
            flag = true;
            break;
        }
        else
        {
            cout << "Invalid username or password" << endl;
        }
        chance--;
    }

    if (!flag)
    {
        cout << "Exceeded maximum attempts. Terminating." << endl;
        exit(0);
    }

    return flag;
}

// helper functions

void Title()
{
    system("cls");
    const int WIDTH1 = 90;
    const int WIDTH2 = 20;
    const string heading = "MOVIE TICKET RESERVATION SYSTEM";

    cout << endl;

    for (int i = 0; i < WIDTH2; i++)
    {
        cout << ' ';
    }
    for (int i = 0; i < WIDTH1; i++)
    {
        cout << '*';
    }

    cout << endl << endl;
    for (int i = 0; i < 6; i++)
    {
        cout << "\t";
    }
    cout << heading;
    cout << endl << endl;

    for (int i = 0; i < WIDTH2; i++)
    {
        cout << ' ';
    }
    for (int i = 0; i < WIDTH1; i++)
    {
        cout << '*';
    }
    cout << endl << endl;
}

// global variables

Theater theater;
Booking booking;
MemberDatabase db;
Billing billing;
Manager manager;

void TicketMenu()
{
    int slot, mno, ch = 0;

    while (ch != 3)
    {
        Title();
        cout << endl;
        cout << "[1] Book a Ticket" << endl;
        cout << "[2] Cancel Ticket" << endl;
        cout << "[3] Previous Menu" << endl << endl;
        cout << "Choice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
        {
            system("cls");

            mno = theater.DisplayMovies();
            if (mno != -1)
            {
                system("cls");
                slot = theater.DisplayTimeSlots();
                if (slot != -1)
                {
                    system("cls");
                    theater.DisplayMovieDetails(mno);
                    booking.DisplayAvailableSeats(slot, mno);
                    booking.BookTicket(slot, mno);
                }
            }
            system("pause");
            break;
        }

        case 2:
        {
            system("cls");

            int tNum;
            cout << "Enter Ticket Number: ";
            cin >> tNum;

            booking.CancelTicket(tNum);

            system("pause");
            break;
        }

        case 3:
        {
            break;
        }

        default:
            cout << "INVALID CHOICE" << endl << endl;
            system("pause");
            break;
        }
    }
}

void CustomerMenu()
{
    int choice = 0;
    string name;

    while (choice != 4)
    {
        Title();

        cout << endl;
        cout << "[1] Login (for Members)" << endl;
        cout << "[2] Signup for Membership" << endl;
        cout << "[3] Continue as guest" << endl;
        cout << "[4] Previous Menu" << endl << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << endl;
            cout << "Enter the details of the Member:" << endl;
            cout << "Name: ";
            cin >> name;
            bool logged = db.Login(name);

            if (logged)
            {
                TicketMenu();
            }

            system("pause");
            break;
        }

        case 2:
        {
            Member m;
            m.Register();
            db.SaveRecord(m);

            TicketMenu();
            break;
        }

        case 3:
        {
            Customer c;
            c.SetDetails();

            TicketMenu();
            break;
        }

        case 4:
        {
            break;
        }

        default:
            cout << "INVALID CHOICE" << endl << endl;
            system("pause");
            break;
        }
    }
}

void ManagerMenu()
{
    int choice = 0;
    string name;
    Manager m;

    while (choice != 4)
    {
        Title();

        cout << endl;
        cout << "[1] Add Movie" << endl;
        cout << "[2] Delete Movie" << endl;
        cout << "[3] Display Member Information" << endl;
        cout << "[4] Previous Menu" << endl << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            theater.AddMovie();
            system("pause");
            break;
        }

        case 2:
        {
            string name;
            cout << "Movie Name: ";
            cin >> name;
            theater.DeleteMovie(name);
            system("pause");
            break;
        }

        case 3:
        {
            db.DisplayMemberInformation();
            system("pause");
            break;
        }

        case 4:
        {
            break;
        }

        default:
            cout << "INVALID CHOICE" << endl << endl;
            system("pause");
            break;
        }
    }
}

int main()
{
    srand(time(0));  // generates random values

    int choice = 0;

    while (choice != 3)
    {
        Title();

        cout << endl;
        cout << "[1] Customer Menu" << endl;
        cout << "[2] Manager Menu" << endl;
        cout << "[3] Exit" << endl << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            CustomerMenu();
            break;
        }

        case 2:
        {
            ManagerMenu();
            break;
        }

        case 3:
        {
            cout << "Exiting" << endl;
            break;
        }

        default:
            cout << "INVALID CHOICE" << endl;
            system("pause");
            break;
        }
    }
    return 0;
}
