#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define MOVIE_NUM 3
#define ROW_NUM 6
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
    string name;
    string location;
    int timeSlot[6] = {9, 12, 15, 18, 21, 0};
    Movie movies[MOVIE_NUM];

public:
    Theater();
    int DisplayMovies();
    void DisplayMovieDetails(int id);
    int DisplayTimeSlots();
    Movie GetMovie(int id);
    int GetTimeSlot(int time) { return timeSlot[time + 1]; }
};

Theater::Theater()
{
    ifstream movieFile("Movies.txt", ios::in);

    int i = 0;
    if (movieFile.is_open())
    {
        while (movieFile.good())
        {
            movies[i].id = i + 1;
            movieFile >> movies[i];
            i++;
        }
    }
    movieFile.close();
}
//
// void Theater::AddMovies()
// {
//     ofstream movieFile("Movies.txt", ios::app);
//     Movie m;
//     m.SetDetails();
//     movieFile << m;
// }
//
// void Theater::RemoveMovieFromFile()
// {
//     ifstream movieFile("Movies.txt", ios::in);
//     ofstream temp("temp.txt", ios::out);
//
//     Movie m;
//     m.SetDetails();
//
//     Movie curr;
//
//     if (movieFile.is_open())
//     {
//         while (movieFile >> curr)
//         {
//             if (!(curr.name == m.name))
//             {
//                 temp << curr;
//             }
//         }
//     }
//     movieFile.close();
//     temp.close();
//
//     remove("Movies.txt");
//     rename("temp.txt", "Movies.txt");
// }

int Theater::DisplayMovies()
{
    int choice;

    cout << endl;
    cout << "Select a Movie" << endl << endl;
    for (int i = 0; i < MOVIE_NUM; i++)
    {
        cout << "\t[" << i + 1 << "] " << movies[i].name << endl;
    }

    cout << endl;
    cout << "Movie number: ";
    cin >> choice;
    try
    {
        if (choice > 0 && choice <= MOVIE_NUM)
        {
            return choice;
        }
        throw choice;
    }
    catch (int choice)
    {
        cout << "INVALID CHOICE. ENTER CORRECT MOVIE NUMBER." << endl;
        system("pause");
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
        cout << "INVALID CHOICE. ENTER CORRECT SLOT NUMBER" << endl;
        system("pause");
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
    friend istream& operator>>(istream& file, Ticket& t);
    friend ostream& operator<<(ostream& file, Ticket& t);
    friend bool operator==(const Ticket& lhs, const Ticket& rhs);
    friend class Booking;
    friend class Billing;

public:
    void SetDetails(int slot, int mno);
};

bool operator==(const Ticket& lhs, const Ticket& rhs)
{
    if (lhs.timeSlot == rhs.timeSlot && lhs.movieID == rhs.movieID &&
        lhs.rowNo == rhs.rowNo && lhs.seatNo == rhs.seatNo &&
        lhs.type == rhs.type)
    {
        return true;
    }
    return false;
}

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
    if (rowNo <= 2)
    {
        type = "Platinum";
    }
    else if (rowNo > 2 && rowNo <= 4)
    {
        type = "Gold";
    }
    else if (rowNo > 4 && rowNo <= 6)
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
    }
    catch (exception e)
    {
        // cout << "Exception(in Ticket)" << endl;
    }

    getline(file, t.type);
    getline(file, temp);

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
    cout << "Movie Name: " << m.name << "\t\tTime: " << time << ":00("
         << t.timeSlot << ")" << endl;
    cout << "Seat Type: " << t.type << endl;
    cout << "Seat: " << row << t.seatNo << endl;
    cout << "Price: Rs. " << t.price << endl;
    cout << "------------------------------------------------------" << endl;
}

class Booking
{
private:
    int bookedSeats[5 + 1][MOVIE_NUM + 1][ROW_NUM + 1][SEAT_NUM + 1] = {0};
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

Ticket Booking::GetTicket(int tNum)
{
    for (int i = 0; i < tickets.size(); i++)
    {
        if (tickets[i].ticketNum == tNum)
        {
            return tickets[i];
        }
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
    Ticket t = GetTicket(tNum);

    if (CheckSeat(t.timeSlot, t.movieID, t.rowNo, t.seatNo) == 1)
    {
        bookedSeats[t.timeSlot][t.movieID][t.rowNo][t.seatNo] = 0;
        RemoveTicketFromFile(tNum);
    }
    else if (CheckSeat(t.timeSlot, t.movieID, t.rowNo, t.seatNo) == 0)
    {
        cout << "TICKET DOESN'T EXIST IN OUR DATABASE. DID YOU ENTER THE "
                "CORRECT DETAILS?"
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
        if (i <= 2 && skip % 2 == 0)
        {
            cout << "[Platinum]" << endl;
            skip++;
        }
        else if (i > 2 && i <= 4 && skip % 2 == 1)
        {
            cout << "[Gold]" << endl;
            skip++;
        }
        else if (i > 4 && i <= 6 && skip % 2 == 0)
        {
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
        cout << endl << endl;
    }
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

int loggedIn = 0;

class MemberDatabase
{
private:
    vector<Member> members;

public:
    MemberDatabase();
    void Display(Member m);
    void DisplayAllRecords();
    void SaveRecord(Member m);
    bool Login(string name);
    bool CheckPassword(Member m, string password);
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

void MemberDatabase::DisplayAllRecords()
{
    cout << "======================================================\n"
         << "                   MEMBER INFORMATION                 \n"
         << "======================================================\n";

    for (int i = 0; i < members.size(); i++)
    {
        cout << "Account Number : " << members[i].accountNumber << endl;
        cout << "Name           : " << members[i].name << endl;
        cout << "Phone no.      : " << members[i].phone << endl;
        cout << "E-mail         : " << members[i].email << endl;
        cout << "------------------------------------------------------"
             << endl;
    }
}

void MemberDatabase::Display(Member m)
{
    cout << "======================================================\n"
         << "                   MEMBER INFORMATION                 \n"
         << "======================================================\n";

    cout << "Account Number : " << m.accountNumber << endl;
    cout << "Name           : " << m.name << endl;
    cout << "Phone no.      : " << m.phone << endl;
    cout << "E-mail         : " << m.email << endl;
    cout << "------------------------------------------------------" << endl;
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
                    system("pause");
                }
            }
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

int MemberMenu()
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
        cout << "[4] Exit" << endl << endl;
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

            cout << "Logged: " << logged << endl;
            system("pause");

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
            cout << "Exiting" << endl;
            break;
        }

        default:
            cout << "INVALID CHOICE" << endl << endl;
            system("pause");
            break;
        }
    }

    return choice;
}

int main()
{
    srand(time(0));  // generates random values

    Title();
    MemberMenu();

    return 0;
}
