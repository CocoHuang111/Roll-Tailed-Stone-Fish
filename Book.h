#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QJsonObject>

class Book {
public:
    QString title;
    QString author;
    QString publisher;
    int publishYear;
    QString isbn;
    QStringList tags;
    double price;
    QString description;
    QString coverImage;
    QDateTime listedTime;
    QString sellerId;
    bool isSold = false;
    QString buyerId;    // 购买者ID

    // 辅助函数
    bool operator==(const Book &other) const;
    QJsonObject toJson() const;
    static Book fromJson(const QJsonObject &obj);
    void update();
};

#endif // BOOK_H
