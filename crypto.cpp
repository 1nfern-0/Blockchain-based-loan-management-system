//Blockchain based Credit Scoring

/*
	       Name                   ID
	Atishya Gupta         - 2017A7PS1482H
	R Hariharan           - 2017A7PS0065H
	V Rachana Srikruthi   - 2017A7PS0086H
	V Chandrahaas         - 2015A7PS0010H
*/

#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iomanip>
#include "stdafx.h"

#include <mysql.h>
#include "mysql_connection.h"


#define SERVER "localhost"
#define USER "user1"
#define PASSWORD "Ati1283"
#define DATABASE "crypto"
#define PORT 3306

using namespace std;
int qstate;

template <typename T>

/*
    Database tables used:
    1. Lender table named as lender
    2. Borrower table named as borrower
    3. Transactions table named as block
    4. Borrower-requests table named as requests
*/

std::string to_string(T value)
{
    //create an output string stream
    std::ostringstream os;

    //throw the value into the string stream
    os << value;

    //convert the string stream into a string and return
    return os.str();
}

//Structure to store borrower-request
struct Borrower
{
    std::string username;
    long int amount;
};

bool verifyTransaction(std::string username)
{
    std::cout << "\n\nVerifying Transaction using Zero Knowledge Proof";
    int g=0, p=0, y=0, r, h, b, rnd, s, i;
    bool val = true;
    /*
        Get g,p,y from borrower table of corresponding username
    */

    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn = mysql_init(0);

    conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

    if (conn) {
        string query11 = "Select g,p,y from borrower where username='" + username + "'";
        const char* q11 = query11.c_str();
        qstate = mysql_query(conn, q11);


        if (!qstate) {
            res = mysql_store_result(conn);
            while (row = mysql_fetch_row(res)) {

               g = stoi(row[0]);
               p = stoi(row[1]);
               y = stoi(row[2]);
            }

        }

    }
    else {
        puts("Connection to database has failed");
    }


    srand(time(0));
    //Choosing a random no. of rounds from 6 to 10
    rnd = rand() % 5 + 6;
    std::cout << "\n\ng value: " << g << "\np value: " << p << "\ny value: " << y << "\nNumber of rounds: " << rnd;
    //Zero Knowledge Proof Algorithm
    for (i = 0; i < rnd; i++)
    {
        std::cout << "\n\nRound " << i + 1 << " of " << rnd << ":";
        //Step 1
        std::cout << "\nChoose a random number r where 0 <= r < " << p - 1 << ".";
        std::cout << "\nValue of h ( h = (" << g << "^r) mod " << p << " ): ";
        std::cin >> h;
        //Step 2
        //Choosing random bit 0 or 1
        b = rand() % 2;
        std::cout << "\nYour random bit b value: " << b;
        //Step 3
        std::cout << "\nValue of s ( s = (r + " << b << "*credit score) mod " << p - 1 << " ): ";
        std::cin >> s;
        //Step 4
        int lhs = 1, rhs = 1, j;
        for (j = 0; j < s; j++)
        {
            lhs *= g;
            if (lhs >= g)
            {
                lhs %= p;
            }
        }
        if (b == 0)
        {
            rhs = h % p;
        }
        else
        {
            rhs = (h * y) % p;
        }
        //ZKP failing condition
        if (lhs != rhs)
        {
            val = false;
        }
    }
    return val;
}

bool mineBlock(std::string username)
{
    std::cout << "\n\nProof of Work";
    bool val;
    std::string input, hash_input, user_input;
    int i;
    srand(time(0));
    //Generating a 7-digit random input
    for (i = 0; i < 7; i++)
    {
        char r = rand() % 10 + 48;
        input.push_back(r);
    }
    //Generating hash value of the input
    std::hash<std::string> hash_string;
    hash_input = to_string(hash_string(input));
    //Proof of Work
    //Asking the user to decrypt the hash value and obtain the input
    std::cout << "\nHash value: " << hash_input;
    std::cout << "\nDecrypt the hash value and obtain the input.\nInput: ";
    std::cin >> user_input;
    return true;
    if (input == user_input)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Creating and adding block to the blockchain
void createBlock(std::string lender, std::string borrower, long int amount)
{
    /*
        remove the entry in borrower-request table with corresponding borrower and amount
    */
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;
    conn = mysql_init(0);

    conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

    if (conn) {
        string query4 = "Delete from requests where (borrower_uid='" + borrower + "' and amount=" + to_string(amount) + ")";
        const char* q4 = query4.c_str();
        qstate = mysql_query(conn, q4);
    }
    else {
        puts("Connection to database has failed");
    }
    //Deleted the selected entry from requests table

    //getting number of rows already in blockchain
    int n;
        string query14 = "Select count(*) from block";
        const char* q14 = query14.c_str();
        qstate = mysql_query(conn, q14);

        if (!qstate) {
            res = mysql_store_result(conn);
            while (row = mysql_fetch_row(res)) {
                n = stoi(row[0]);
            }

        }

    if (n==0)
    {

        std::string hash_new;
        std::string prev_hash = "0000000";
        std::hash<std::string> hash_string;
        hash_new = to_string(hash_string(prev_hash));

        /* Adding first block in database*/
        MYSQL* conn;
        MYSQL_ROW row;
        MYSQL_RES* res;
        conn = mysql_init(0);

        conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

        if (conn) {
            string query5 = "INSERT into block values('" + lender + "','" + borrower + "'," + to_string(amount) + ",'" + hash_new + "','" + prev_hash + "')";
            const char* q5 = query5.c_str();
            qstate = mysql_query(conn, q5);


            //another query for storing hash_new into prev_hash of next tuple
            string query12 = "INSERT into block(prev_hash) values('" + hash_new + "')";
            const char* q12 = query12.c_str();
            qstate = mysql_query(conn, q12);


        }
        else {
            puts("Connection to database has failed");
        }

        //Added first block in database

    }
    else
    {
        std::string prev_hash;
        MYSQL* conn;
        MYSQL_ROW row;
        MYSQL_RES* res;
        conn = mysql_init(0);

        conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

        if (conn) {
            string query13 = "Select prev_hash from block where lender_uid='undefined'";
            const char* q13 = query13.c_str();
            qstate = mysql_query(conn, q13);

            if (!qstate) {
                res = mysql_store_result(conn);
                while (row = mysql_fetch_row(res)) {
                    prev_hash = row[0];
                }

            }


            /*
       add lender, borrower, amount in transactions table
   */
   //Adding the block in database

            std::string hash_new;
            std::hash<std::string> hash_string;
            hash_new = to_string(hash_string(prev_hash));


            string query6 = "UPDATE block SET lender_uid='" + lender + "',borrower_uid='" + borrower + "',amount=" + to_string(amount) + ",hash='" + hash_new + "' where prev_hash='" + prev_hash + "'";
            const char* q6 = query6.c_str();
            qstate = mysql_query(conn, q6);



            //another query for storing hash_new into prev_hash of next tuple
            string query17 = "INSERT into block(prev_hash) values('" + hash_new + "')";
            const char* q17 = query17.c_str();
            qstate = mysql_query(conn, q17);

        }
        else {
            puts("Connection to database has failed");
        }





        //Added block in database
    }
}
void viewUser(std::string username, int opt)
{
    if (opt == 0)
    {
        int n;
        string lender_transactions[100];
        int amount[100];

        //fetch lender previous transactions
        MYSQL* conn;
        MYSQL_ROW row;
        MYSQL_RES* res;
        conn = mysql_init(0);

        conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

        if (conn) {
            string query15 = "select borrower_uid,amount from block where lender_uid='" + username + "'";
            const char* q15 = query15.c_str();
            qstate = mysql_query(conn, q15);

            if (!qstate) {
                res = mysql_store_result(conn);
                int i = 0;
                while (row = mysql_fetch_row(res)) {
                    lender_transactions[i]=row[0];
                    amount[i]=stoi(row[1]);
                    i++;
                }
                n = i;

            }

        }
        else {
            puts("Connection to database has failed");
        }

        //fetched lender previous transactions

        //Display all transactions
        cout << "   Borrower_username       " << "Amount\n";
        for (int i = 0; i < n; i++) {
           // printf("%d. %s               %d\n", i + 1, lender_transactions[i], amount[i]);
            cout << i + 1 << ". " << setfill(' ') << setw(4 - (to_string(i + 2)).size()) << lender_transactions[i] << setfill(' ') << setw(29 - lender_transactions[i].size()) << amount[i] <<'\n';
        }
    }
    else
    {
        int n;
        string borrower_transactions[100];
        int amount[100];

        //fetch borrower previous transactions
        MYSQL* conn;
        MYSQL_ROW row;
        MYSQL_RES* res;
        conn = mysql_init(0);

        conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

        if (conn) {
            string query15 = "select lender_uid,amount from block where borrower_uid='" + username + "'";
            const char* q15 = query15.c_str();
            qstate = mysql_query(conn, q15);

            if (!qstate) {
                res = mysql_store_result(conn);
                int i = 0;
                while (row = mysql_fetch_row(res)) {
                    borrower_transactions[i]=row[0];
                    amount[i]=stoi(row[1]);
                    i++;
                }
                n = i;

            }

        }
        else {
            puts("Connection to database has failed");
        }

        //fetched borrower previous transactions

        //Display all transactions
        cout << "   Lender_username       " << "Amount\n";
        for (int i = 0; i < n; i++) {
            cout << i + 1 << ". " << setfill(' ') << setw(6 - (to_string(i + 2)).size()) << borrower_transactions[i] << setfill(' ') << setw(27 - borrower_transactions[i].size()) << amount[i] << '\n';
        }
    }
}

int main(int argc, char** argv)
{
    int code;
    std::string username, password;
    std::cout << "Blockchain Loan Interface\n\n";
    std::cout << "1. Lender\n2. Borrower\nChoose code: ";
    std::cin >> code;
    if (code == 1)
    {
        int code1;
        std::cout << "\n1. Login\n2. Register\nChoose code: ";
        std::cin >> code1;
        if (code1 == 1)
        {
            int code2;
            std::cout << "\nLogin";
            std::cout << "Username:\n";
            std::cin.ignore();
            getline(std::cin, username);
            std::cout << "Password:\n";
            getline(std::cin, password);
            std::string db_pass;


            /*
                search through lender table and find table.user==username.
                store the corresponding password in db_pass.
            */


                MYSQL* conn;
                MYSQL_ROW row;
                MYSQL_RES* res;
                conn = mysql_init(0);

                conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

                if (conn) {

                    string query1 = "Select password from lender where username ='" + username + "'";
                    const char* q1 = query1.c_str();
                    qstate = mysql_query(conn, q1);

                    if (!qstate) {
                        res = mysql_store_result(conn);
                        while (row = mysql_fetch_row(res)) {
                            db_pass = row[0];
                        }

                    }

                }
                else {
                    puts("Connection to database has failed");
                }




            if (password == db_pass)
            {
                std::cout << "\n    ***Login Successful***\n";
                int code3;
                std::cout << "\n1. Check requests\n2. View previous transactions\nChoose code: ";
                std::cin >> code3;
                if (code3 == 1)
                {
                    int n=0, i;
                    Borrower b[100];
                    /*
                        store no. of entries in borrower-request table in n
                        get all the entries from borrower-request table and store in b array
                    */
                    MYSQL* conn;
                    MYSQL_ROW row;
                    MYSQL_RES* res;
                    conn = mysql_init(0);

                    conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

                    if (conn) {
                        string query2 = "Select count(*) from requests";
                        const char* q2 = query2.c_str();
                        qstate = mysql_query(conn, q2);

                        if (!qstate) {
                            res = mysql_store_result(conn);
                            while (row = mysql_fetch_row(res)) {
                                n = stoi(row[0]);
                            }

                        }

                        string query3 = "Select * from requests";
                         const char *q3 = query3.c_str();
                        qstate = mysql_query(conn, q3);

                        if (!qstate) {
                            res = mysql_store_result(conn);
                            int i = 0;
                            while (row = mysql_fetch_row(res)) {
                                b[i].username = row[0];
                                b[i].amount = stoi(row[1]);
                                i++;
                            }
                        }


                    }
                    else {
                        puts("Connection to database has failed");
                    }

                    //gathered information from requests table;

                    for (i = 0; i < n; i++)
                    {
                        std::cout << "\n" << i + 1 << ". Borrower: " << b[i].username << " Request: " << b[i].amount;
                    }
                    std::cout << "\n";
                    std::cout << "\nChoose code: ";
                    int code4;
                    std::cin >> code4;
                    //A new block is created for the transaction between the lender and the chosen borrower
                    createBlock(username, b[code4 - 1].username, b[code4 - 1].amount);
                    std::cout << "\nLoan Sanctioned!\nBlock Created!";
                }
                else if (code3 == 2)
                {
                    //Second argument 0 denotes lender's previous transactions
                    viewUser(username, 0);
                }
                else
                {
                    std::cout << "\nError";
                }
            }
            else
            {
                std::cout << "\nLogin failed";
            }
        }
        else if (code1 == 2)
        {
            int g, p, f;
            std::cout << "\nRegister";
            std::cout << "\n\nUsername:\n";
            std::cin.ignore();
            getline(std::cin, username);
            std::cout << "Password:\n";
            getline(std::cin, password);
            /*
                add username, password in lender table
            */
            MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);

            conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

            if (conn) {
                //puts("Successful connection to database:");
                string query = "Insert into lender values('" + username + "','" + password + "')";
                const char* q = query.c_str();
                qstate = mysql_query(conn, q);

            }
            else {
                puts("Connection to database has failed");
            }

            std::cout << "\nSuccessfully Registered!";
        }
        else
        {
            std::cout << "\nError";
        }
    }
    else if (code == 2)
    {
        int code1;
        std::cout << "\n1. Login\n2. Register\nChoose code: ";
        std::cin >> code1;
        if (code1 == 1)
        {
            int code2;
            std::cout << "\nLogin";
            std::cout << "Username:\n";
            std::cin.ignore();
            getline(std::cin, username);
            std::cout << "Password:\n";
            getline(std::cin, password);
            std::string db_pass;
            /*
                search through borrower table and find table.user==username.
                store the corresponding password in db_pass.
            */
            MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);

            conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

            if (conn) {
                //puts("Successful connection to database:");
                string query = "Select password from borrower where username ='" + username + "'";
                const char* q = query.c_str();
                qstate = mysql_query(conn, q);

                if (!qstate) {
                    res = mysql_store_result(conn);
                    while (row = mysql_fetch_row(res)) {
                        db_pass = row[0];
                        break;
                    }

                }

            }
            else {
                puts("Connection to database has failed");
            }


            if (password == db_pass)
            {
                int code3;
                std::cout << "\n    ***LOGIN SUCCESSFUL***\n";
                std::cout << "\n1. Ask new loan\n2. View previous transactions\nChoose code: ";
                std::cin >> code3;
                if (code3 == 1)
                {
                    //verifyTransaction returns false if ZKP fails, true if otherwise
                    bool val = verifyTransaction(username);
                    if (!val)
                    {
                        std::cout << "\nSecurity Breach. Request not created.";
                    }
                    else
                    {
                        std::cout << "\nCredit score verified successfully through Zero Knowledge Proof!";
                        //mineBlock returns false if POW is not done successfully, true if otherwise
                        bool val1 = mineBlock(username);
                        if (!val1)
                        {
                            std::cout << "\nProof of Work not done successfully. Request not created.";
                        }
                        else
                        {
                            std::cout << "\nProof of Work done successfully!";
                            int amount;
                            std::cout << "\n\nLoan request amount: ";
                            std::cin >> amount;
                            /*
                                add username and amount in borrower-request table
                            */
                            MYSQL* conn;
                            MYSQL_ROW row;
                            MYSQL_RES* res;
                            conn = mysql_init(0);

                            conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

                            if (conn) {
                                //puts("Successful connection to database:");
                                string query = "Insert into requests values('" + username + "'," + to_string(amount) + ")";
                                const char* q = query.c_str();
                                qstate = mysql_query(conn, q);

                            }
                            else {
                                puts("Connection to database has failed");
                            }
                            std::cout << "\nRequest created successfully!";
                            std::cout << "\nYou have done all the work to successfully mine a block! Your block will be added if a lender approves your request and transacts with you!";
                        }
                    }
                }
                else if (code3 == 2)
                {
                    //Second argument 1 denotes borrower's previous transactions
                    viewUser(username, 1);
                }
                else
                {
                    std::cout << "\nError";
                }
            }
            else
            {
                std::cout << "\nLogin failed";
            }
        }
        else if (code1 == 2)
        {
            int g, p, y;
            std::cout << "\nRegister";
            std::cout << "\n\nUsername:\n";
            std::cin.ignore();
            getline(std::cin, username);
            std::cout << "Password:\n";
            getline(std::cin, password);
            std::cout << "g of y function of credit score: ";
            std::cin >> g;
            std::cout << "p of y function of credit score: ";
            std::cin >> p;
            std::cout << "y function value of credit score ( y = (" << g << "^credit score) mod " << p << " ): ";
            std::cin >> y;
            /*
                add username, password, g, p, f in borrower database
            */
            MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);

            conn = mysql_real_connect(conn, "localhost", "user1", "Ati1283", "crypto", 3306, NULL, 0);

            if (conn) {
                string query = "Insert into borrower values('" + username + "','" + password + "'," + to_string(g) + "," + to_string(p) + "," + to_string(y) + ")";
                const char* q = query.c_str();
                qstate = mysql_query(conn, q);

            }
            else {
                puts("Connection to database has failed");
            }
            std::cout << "\nSuccessfully Registered!";
        }
        else
        {
            std::cout << "\nError";
        }
    }
    else
    {
        std::cout << "\nError";
    }
    return 0;
}
