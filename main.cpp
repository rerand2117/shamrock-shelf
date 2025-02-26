#include <iostream>
#include <string>
#include "mysql.h"
#include <sstream>
#include <iomanip>

using namespace std;

MYSQL *db_conn;

// Connect to the database
void connectToDatabase(const string &host, const string &user, const string &password, const string &db) {
    db_conn = mysql_init(NULL);
    if (!db_conn) {
        cerr << "MySQL initialization failed!" << endl;
        exit(EXIT_FAILURE);
    }

    if (!mysql_real_connect(db_conn, host.c_str(), user.c_str(), password.c_str(), db.c_str(), 0, NULL, 0)) {
        cerr << "Connection failed: " << mysql_error(db_conn) << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Connected to the database successfully!" << endl;
}

// Close the database connection
void closeDatabase() {
    mysql_close(db_conn);
    cout << "Database connection closed." << endl;
}

// Login function
bool login(bool &isMainUser) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    stringstream query;
    query << "SELECT * FROM Auth_User WHERE username = '" << username
          << "' AND password = '" << password << "'";

    if (mysql_query(db_conn, query.str().c_str()) == 0) {
        MYSQL_RES *res = mysql_store_result(db_conn);
        if (res && mysql_num_rows(res) > 0) {
            cout << "Login successful!" << endl;
            mysql_free_result(res);
            isMainUser = false;
            return true;
        }
        mysql_free_result(res);
    }

    query.str("");
    query.clear();
    query << "SELECT * FROM User WHERE username = '" << username
          << "' AND password = '" << password << "'";

    if (mysql_query(db_conn, query.str().c_str()) == 0) {
        MYSQL_RES *res = mysql_store_result(db_conn);
        if (res && mysql_num_rows(res) > 0) {
            cout << "Login successful (Main User)!" << endl;
            mysql_free_result(res);
            isMainUser = true;
            return true;
        }
        mysql_free_result(res);
    }

    cout << "Invalid username or password." << endl;
    return false;
}

// Add ingredient
void addIngredient() {
    string name, category, measure_unit, expiration_date;
    cout << "Enter ingredient name: ";
    cin >> name;
    cout << "Enter category: ";
    cin >> category;
    cout << "Enter measurement unit: ";
    cin >> measure_unit;
    cout << "Enter expiration date (YYYY-MM-DD): ";
    cin >> expiration_date;

    stringstream query;
    query << "INSERT INTO Ingredient (name, category, measure_unit, expiration) VALUES ('"
          << name << "', '" << category << "', '" << measure_unit << "', '" << expiration_date << "')";

    if (mysql_query(db_conn, query.str().c_str()) == 0) {
        cout << "Ingredient added successfully!" << endl;
    } else {
        cerr << "Error adding ingredient: " << mysql_error(db_conn) << endl;
    }
}

// Edit ingredient
void editIngredient() {
    string name, attribute, newValue;
    cout << "Enter the ingredient name to edit: ";
    cin >> name;
    cout << "Enter the attribute to modify (category, measure_unit, expiration): ";
    cin >> attribute;
    cout << "Enter the new value: ";
    cin >> newValue;

    stringstream query;
    query << "UPDATE Ingredient SET " << attribute << " = '" << newValue
          << "' WHERE name = '" << name << "'";

    if (mysql_query(db_conn, query.str().c_str()) == 0) {
        cout << "Ingredient updated successfully!" << endl;
    } else {
        cerr << "Error updating ingredient: " << mysql_error(db_conn) << endl;
    }
}

// Delete ingredient
void deleteIngredient() {
    string name;
    cout << "Enter the ingredient name to delete: ";
    cin >> name;

    stringstream query;
    query << "DELETE FROM Ingredient WHERE name = '" << name << "'";

    if (mysql_query(db_conn, query.str().c_str()) == 0) {
        cout << "Ingredient deleted successfully!" << endl;
    } else {
        cerr << "Error deleting ingredient: " << mysql_error(db_conn) << endl;
    }
}

// Check expirations
void checkExpirations() {
    stringstream query;
    query << "SELECT name, expiration FROM Ingredient WHERE expiration <= CURDATE() + INTERVAL 7 DAY";

    if (mysql_query(db_conn, query.str().c_str()) == 0) {
        MYSQL_RES *res = mysql_store_result(db_conn);
        MYSQL_ROW row;
        cout << "Ingredients nearing expiration:" << endl;
        while ((row = mysql_fetch_row(res)) != NULL) {
            cout << "Name: " << row[0] << ", Expiration Date: " << row[1] << endl;
        }
        mysql_free_result(res);
    } else {
        cerr << "Error checking expirations: " << mysql_error(db_conn) << endl;
    }
}

// Add user
void addUser() {
    string username, password, email, first_name, last_name;
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter first name: ";
    cin >> first_name;
    cout << "Enter last name: ";
    cin >> last_name;

    stringstream query;
    query << "INSERT INTO User (username, password, email, first_name, last_name) VALUES ('"
          << username << "', '" << password << "', '" << email << "', '" << first_name << "', '" << last_name << "')";

    if (mysql_query(db_conn, query.str().c_str()) == 0) {
        cout << "User added successfully!" << endl;
    } else {
        cerr << "Error adding user: " << mysql_error(db_conn) << endl;
    }
}

// Main menu
void mainMenu(bool isMainUser) {
    int choice;
    do {
        cout << "\nMenu:\n"
             << "1. Add Ingredient\n"
             << "2. Edit Ingredient\n"
             << "3. Delete Ingredient\n"
             << "4. Check Expirations\n"
             << "5. Add User\n"
             << "6. Exit\n"
             << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addIngredient();
                break;
            case 2:
                editIngredient();
                break;
            case 3:
                deleteIngredient();
                break;
            case 4:
                checkExpirations();
                break;
            case 5:
                if (isMainUser) {
                    addUser();
                } else {
                    cout << "Only main users can add new users!" << endl;
                }
                break;
            case 6:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice, try again!" << endl;
        }
    } while (choice != 6);
}

// Main function
int main() {
    connectToDatabase("localhost", "root", "your_password", "grocery_db");

    bool isMainUser = false;
    if (login(isMainUser)) {
        mainMenu(isMainUser);
    }

    closeDatabase();
    return 0;
}

