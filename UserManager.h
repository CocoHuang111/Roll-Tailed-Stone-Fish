#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <unordered_map>
#include <QString>
#include <QJsonObject>

using namespace std;

class User;

class UserManager {
public:
    unordered_map<QString, User*> users;
    QString dataFilePath = "users.json";

    ~UserManager();
    bool createUser(const QString& username, const QString& password, const QString& contact);
    User* getUser(const QString& username);
    bool saveUsersToFile();
    bool loadUsersFromFile();
};

#endif // USERMANAGER_H
