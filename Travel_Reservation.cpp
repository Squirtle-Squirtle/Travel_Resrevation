#include <iostream>
#include <pqxx/pqxx> 
#include <cstring>
#include <string>

using namespace std;
using namespace pqxx;

struct tour {
    char ad_place[20], ad_code[20], ad_country[20], ad_days[20], ad_prices[20];
    char us_phone[20], us_name[20], us_people[20], us_date[20], us_status[20], us_dcode[20];
};

class Tourist {
public:
    void ad_pack(tour r);
    void ad_writedata();
    void ad_display();
    void ad_modify();
    void us_pack(tour r);
    void us_writedata();
    void us_display();
    void us_search();
    void us_modify();
};

// ADMIN PANEL
void Tourist::ad_pack(tour r) {
    try {
        connection C("dbname = tourist_reservation user = postgres password = 12345678 \
                      hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else {
            cout << "Can't open database" << endl;
            return;
        }

        string sql = "INSERT INTO TRIP (AD_CODE, AD_PLACE, AD_COUNTRY, AD_DAYS, AD_PRICES) " \
            "VALUES ('" + string(r.ad_code) + "', '" + string(r.ad_place) + "', '" + string(r.ad_country) + "', '" + string(r.ad_days) + "', '" + string(r.ad_prices) + "');";

        work W(C);
        W.exec(sql);
        W.commit();
        cout << "Records created successfully" << endl;
        // C.disconnect();

    }
    catch (const std::exception& e) {
        cerr << e.what() << std::endl;
    }
}

void Tourist::ad_writedata() {
    tour s;
    cout << "Enter the Destination Code\nD-";
    cin >> s.ad_code;
    cout << "Enter the Destination Place\n";
    cin >> s.ad_place;
    cout << "Enter the Country of the Destination\n";
    cin >> s.ad_country;
    cout << "Enter the Number of days of the Trip\n";
    cin >> s.ad_days;
    cout << "Enter the Cost of the trip\n";
    cin >> s.ad_prices;
    ad_pack(s);
}

void Tourist::ad_display() {
    try {
        connection C("dbname = tourist_reservation user = postgres password = 12345678 \
                      hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else {
            cout << "Can't open database" << endl;
            return;
        }

        string sql = "SELECT * from TRIP";
        nontransaction N(C);
        result R(N.exec(sql));

        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "Sr No.\t\tDEST. CODE\tPLACE\t\tCOUNTRY\t\tDAYS\t\tPRICES\n";
        cout << "-------------------------------------------------------------------------------------------------\n";
        int c = 1;
        int cnt = 0;
        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            cout << cnt++ << "                " << c[0].as<string>() << "\t\t" << c[1].as<string>() << "\t\t" << c[2].as<string>() << "\t\t" << c[3].as<string>() << "\t\t" << c[4].as<string>() << endl;
        }
        cout << "Operation done successfully" << endl;
        //C.disconnect();

    }
    catch (const std::exception& e) {
        cerr << e.what() << std::endl;
    }
}

void Tourist::ad_modify() {
    try {
        connection C("dbname = tourist_reservation user = postgres password = 12345678 hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else {
            cout << "Can't open database" << endl;
            return;
        }

        char ad_code[15];
        int ch;
        cout << "\nENTER THE DESTINATION CODE TO BE MODIFIED: ";
        cin >> ad_code;

        string sql = "SELECT * from TRIP WHERE AD_CODE = '" + string(ad_code) + "';";

        // Use a new transaction for the SELECT operation
        {
            nontransaction N(C);
            result R(N.exec(sql));

            if (R.size() == 0) {
                cout << "TRIP NOT FOUND" << endl;
                return;
            }

            for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
                cout << "VALUES OF THE TRIP\n";
                cout << "\nDestination Code: " << c[0].as<string>();
                cout << "\nDestination Place: " << c[1].as<string>();
                cout << "\nDestination Country: " << c[2].as<string>();
                cout << "\nNo of days of Trip: " << c[3].as<string>();
                cout << "\nTotal Cost of the Trip: " << c[4].as<string>();
                cout << "\nWhat you want to Update: ";
                cout << "\n\n\t\tEnter 1 for changing DESTINATION PLACE\n";
                cout << "\t\tEnter 2 for changing DAYS OF THE TRIP\n";
                cout << "\t\tEnter 3 for changing COST OF THE TRIP\n";
                cout << "\t\t\t";
                cin >> ch;

                switch (ch) {
                case 1: {
                    string new_place, new_country;
                    cout << "Destination Place: ";
                    cin >> new_place;
                    cout << "Destination Country: ";
                    cin >> new_country;
                    sql = "UPDATE TRIP set AD_PLACE = '" + new_place + "', AD_COUNTRY = '" + new_country + "' where AD_CODE = '" + string(ad_code) + "';";
                    break;
                }
                case 2: {
                    string new_days;
                    cout << "No of days of Trip: ";
                    cin >> new_days;
                    sql = "UPDATE TRIP set AD_DAYS = '" + new_days + "' where AD_CODE = '" + string(ad_code) + "';";
                    break;
                }
                case 3: {
                    string new_prices;
                    cout << "Total Cost of the Trip: ";
                    cin >> new_prices;
                    sql = "UPDATE TRIP set AD_PRICES = '" + new_prices + "' where AD_CODE = '" + string(ad_code) + "';";
                    break;
                }
                default:
                    cout << "Invalid choice";
                    return;
                }
            }
        }

        // Use a new transaction for the UPDATE operation
        {
            work W(C);
            W.exec(sql);
            W.commit();
            cout << "Trip updated successfully" << endl;
        }

    }
    catch (const std::exception& e) {
        cerr << e.what() << std::endl;
    }
}


// USER PANEL
void Tourist::us_pack(tour r) {
    try {
        connection C("dbname = tourist_reservation user = postgres password = 12345678 \
                      hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else {
            cout << "Can't open database" << endl;
            return;
        }

        string sql = "INSERT INTO RESERVATION (US_NAME, US_PHONE, US_PEOPLE, US_DCODE, US_DATE, US_STATUS) " \
            "VALUES ('" + string(r.us_name) + "', '" + string(r.us_phone) + "', '" + string(r.us_people) + "', '" + string(r.us_dcode) + "', '" + string(r.us_date) + "', '" + string(r.us_status) + "');";

        work W(C);
        W.exec(sql);
        W.commit();
        cout << "Records created successfully" << endl;
        // C.disconnect();

    }
    catch (const std::exception& e) {
        cerr << e.what() << std::endl;
    }
}

void Tourist::us_writedata() {
    tour r;
    cout << "ENTER YOUR NAME: ";
    cin >> r.us_name;
    cout << "ENTER YOUR CONTACT NO.: ";
    cin >> r.us_phone;
    cout << "ENTER NO. OF TOURIST: ";
    cin >> r.us_people;
    cout << "ENTER THE DESTINATION CODE: D-";
    cin >> r.us_dcode;
    cout << "ENTER DATE OF JOURNEY (DD/MM/YY): ";
    cin >> r.us_date;
    cout << "\nENTER 'Confirm' TO CONFIRM YOUR RESERVATION: ";
    cin >> r.us_status;
    us_pack(r);
}

void Tourist::us_display() {
    try {
        connection C("dbname = tourist_reservation user = postgres password = 12345678 \
                      hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else {
            cout << "Can't open database" << endl;
            return;
        }

        string sql = "SELECT * from RESERVATION";
        nontransaction N(C);
        result R(N.exec(sql));

        cout << "------------------------------------------------------------------------------------------------------------------------\n";
        cout << "Sr No.\tNAME\t\tCONTACT NO\t\tTOTAL PEOPLE\t\tDEST. CODE\t\tDATE\t\tSTATUS\n";
        cout << "------------------------------------------------------------------------------------------------------------------------\n";
        int c = 1;
        int cnt = 0;
        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            cout << cnt++ << "\t" << c[0].as<string>() << "\t\t" << c[1].as<string>() << "\t\t" << c[2].as<string>() << "\t\t" << c[3].as<string>() << "\t\t\t" << c[4].as<string>() << "\t" << c[5].as<string>() << "\t\t" << endl;
        }
        cout << "Operation done successfully" << endl;
        // C.disconnect();

    }
    catch (const std::exception& e) {
        cerr << e.what() << std::endl;
    }
}

void Tourist::us_modify() {
    try {
        connection C("dbname = tourist_reservation user = postgres password = 12345678 hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else {
            cout << "Can't open database" << endl;
            return;
        }

        char us_name[15];
        cout << "\nEnter Your name to cancel your reservation: ";
        cin >> us_name;

        string sql = "SELECT * from RESERVATION WHERE US_NAME = '" + string(us_name) + "';";

        // Use a new transaction for the SELECT operation
        tour s;
        {
            nontransaction N(C);
            result R(N.exec(sql));

            if (R.size() == 0) {
                cout << "RECORD NOT FOUND" << endl;
                return;
            }


            for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
                cout << "\nYour Reservation Details are:\n";
                cout << "\nName: " << c[0].as<string>();
                cout << "\nContact No.: " << c[1].as<string>();
                cout << "\nNo.of People: " << c[2].as<string>();
                cout << "\nDestination: " << c[3].as<string>();
                cout << "\nDate of Journey: " << c[4].as<string>();
                cout << "\nStatus: " << c[5].as<string>();
                cout << "\n\nENTER 'Cancel' TO CANCEL YOUR TICKET: \n";
                cin >> s.us_status;
                cout << "\nYour Reservation has been Cancelled\n";
                cout << "\n\nName: " << c[0].as<string>();
                cout << "\nContact No.: " << c[1].as<string>();
                cout << "\nNo.of People: " << c[2].as<string>();
                cout << "\nDestination: " << c[3].as<string>();
                cout << "\nDate of Journey: " << c[4].as<string>();
                cout << "\nStatus: " << s.us_status;
            }
        }

        sql = "UPDATE RESERVATION set US_STATUS = '" + string(s.us_status) + "' where US_NAME = '" + string(us_name) + "';";

        // Use a new transaction for the UPDATE operation
        {
            work W(C);
            W.exec(sql);
            W.commit();
            cout << "\nReservation updated successfully" << endl;
        }

    }
    catch (const std::exception& e) {
        cerr << e.what() << std::endl;
    }
}


void Tourist::us_search() {
    try {
        connection C("dbname = tourist_reservation user = postgres password = 12345678 \
                      hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        else {
            cout << "Can't open database" << endl;
            return;
        }

        char us_name[15];
        cout << "\nENTER THE NAME TO BE SEARCHED: ";
        cin >> us_name;

        string sql = "SELECT * from RESERVATION WHERE US_NAME = '" + string(us_name) + "';";
        nontransaction N(C);
        result R(N.exec(sql));

        if (R.size() == 0) {
            cout << "RESERVATION NOT FOUND" << endl;
            return;
        }

        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
            cout << "\n RESERVATION FOUND\n";
            cout << "\nNAME: " << c[0].as<string>();
            cout << "\nCONTACT NO.: " << c[1].as<string>();
            cout << "\nNO. OF PEOPLE: " << c[2].as<string>();
            cout << "\nDESTINATION CODE: " << c[3].as<string>();
            cout << "\nDATE OF JOURNEY: " << c[4].as<string>();
            cout << "\nSTATUS: " << c[5].as<string>();
        }
        // C.disconnect();

    }
    catch (const std::exception& e) {
        cerr << e.what() << std::endl;
    }
}

int main() {
    cout << "                              \t\t WELCOME TO TOURIST RESERVATION SYSTEM\n\n\n";
    cout << "                                   \t    TOURIST RESERVATION SOFTWARE\n\n\n";
    cout << "                                     \t      PRESS ENTER TO CONTINUE....\n\n";
    if (cin.get() == '\n') {
        system("cls");
        int choice, ch1, ch2;
        Tourist obj;
        cout << "\n Who are You?\n";
        cout << "1. ADMIN\n";
        cout << "2. USER\n\n";
        cout << "Enter any other key for exit\n\n\n";
        cout << "Enter Your Option\n";
        cin >> choice;
        switch (choice) {
        case 1:
            system("cls");
            cout << "WELCOME ADMIN\n";
            cout << "Enter your option\n";
            cout << "1. New trips\n";
            cout << "2. Display a Trip\n";
            cout << "3. Display a Reservation\n";
            cout << "4. Update Trip\n";
            cout << "Enter any other key for exit\n\n";
            while (1) {
                cout << "\nENTER UR CHOICE: ";
                cin >> ch1;
                switch (ch1) {
                case 1:
                    obj.ad_writedata();
                    break;
                case 2:
                    obj.ad_display();
                    break;
                case 3:
                    obj.us_display();
                    break;
                case 4:
                    obj.ad_modify();
                    break;
                default:
                    exit(0);
                }
            }
        case 2:
            system("cls");
            cout << "WELCOME USER\n";
            cout << "Enter your option\n";
            cout << "1. New Reservation\n";
            cout << "2. Search Reservation\n";
            cout << "3. Cancel Reservation\n";
            cout << "Enter any other key for exit\n\n\n";
            while (1) {
                cout << "\nENTER UR CHOICE: ";
                cin >> ch2;
                switch (ch2) {
                case 1:
                    cout << "-----------------------------------";
                    cout << "\n Choose Your destination\n";
                    cout << "-----------------------------------\n";
                    obj.ad_display();
                    cout << "\n";
                    obj.us_writedata();
                    break;
                case 2:
                    obj.us_search();
                    break;
                case 3:
                    obj.us_modify();
                    break;
                default:
                    exit(0);
                }
            }
        default:
            exit(0);
        }
    }
}
