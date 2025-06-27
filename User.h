#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <QString>
#include <QJsonObject>
#include <QCryptographicHash>


using namespace std;

class Book; // 前向声明

class User {
public:

    //交易记录（选择性添加）
    struct TransactionRecord {
        QDateTime time;
        QString type;       // "purchase" 或 "recharge"
        double amount;
        QString detail;     // 如购买的书籍ISBN
    };

    string username;
    string password;
    unordered_set<string> preferences;
    vector<Book*> booksOnSale;
    string contact;
    double tokens;
    vector<string> achievements;

    User(const string& name, const string& pwd,
         const string& contactInfo, double initialTokens = 0.0);
    vector<TransactionRecord> transactionHistory; // 交易记录容器


    // 各种成员函数声明...
    QJsonObject toJson() const;
    static User* fromJson(const QJsonObject &obj);
    bool verifyPassword(const std::string& inputPassword) const;
    bool removeBookFromSale(Book* book);
    void addBookForSale(Book* book);
    const vector<Book*>& getBooksOnSale() const;
    void rechargeTokens(double amount);
    double getTokens() const;
    bool purchaseBook(Book* book);
    void addTransactionRecord(const TransactionRecord& record);
    const vector<TransactionRecord>& getTransactionHistory() const;
};

#endif // USER_H
