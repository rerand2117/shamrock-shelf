#ifndef MYSQL_H
#define MYSQL_H

#include <mysql.h>

void connectToDatabase(const std::string &host, const std::string &user, const std::string &password, const std::string &db);
void closeDatabase();
bool login(bool &isMainUser);
void addIngredient();
void editIngredient();
void deleteIngredient();
void checkExpirations();
void addUser();
void mainMenu(bool isMainUser);

#endif /* MYSQL_H */
