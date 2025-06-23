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

void Book::update() {
    // 更新逻辑
}
