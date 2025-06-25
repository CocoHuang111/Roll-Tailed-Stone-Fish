#include "Book.h"
#include <QJsonArray>

bool Book::operator==(const Book &other) const {
    return isbn == other.isbn;
}

QJsonObject Book::toJson() const {
    QJsonObject obj;
    obj["title"] = title;
    obj["author"] = author;
    obj["publisher"] = publisher;
    obj["publishYear"] = publishYear;
    obj["isbn"] = isbn;

    QJsonArray tagsArray;
    for (const auto& tag : tags) {
        tagsArray.append(tag);
    }
    obj["tags"] = tagsArray;

    obj["price"] = price;
    obj["description"] = description;
    obj["coverImage"] = coverImage;
    obj["listedTime"] = listedTime.toString(Qt::ISODate);
    obj["sellerId"] = sellerId;

    return obj;
}

Book Book::fromJson(const QJsonObject &obj) {
    Book book;
    book.title = obj["title"].toString();
    book.author = obj["author"].toString();
    // 其他字段...
    return book;
}

bool Book::saveToFile(const QList<Book>& books, const QString& filePath) {
    // 1. 准备JSON数组
    QJsonArray jsonArray;
    for (const Book& book : books) {
        jsonArray.append(book.toJson());
    }

    // 2. 创建JSON文档
    QJsonDocument doc(jsonArray);

    // 3. 写入文件
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "无法打开文件:" << file.errorString();
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented)); // Indented格式更易读
    file.close();
    return true;
}
