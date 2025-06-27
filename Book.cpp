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

QList<Book> Book::loadFromFile(const QString& filePath) {
    QList<Book> books;

    // 1. 打开文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << file.errorString();
        return books; // 返回空列表
    }

    // 2. 读取并解析JSON
    QByteArray jsonData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull() || !doc.isArray()) {
        qWarning() << "Invalid JSON format or not an array";
        return books;
    }

    // 3. 转换为Book对象
    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& value : jsonArray) {
        QJsonObject obj = value.toObject();
        books.append(Book::fromJson(obj)); // 复用现有的fromJson方法
    }

    return books;
}
/*
使用示例：
// 保存示例
QList<Book> allBooks;
allBooks.append(book1);
allBooks.append(book2);

if (Book::saveToFile(allBooks)) {
    qDebug() << "保存成功";
}

// 加载示例
QList<Book> loadedBooks = Book::loadFromFile();
if (!loadedBooks.isEmpty()) {
    qDebug() << "共加载" << loadedBooks.size() << "本书";
}

// 加载书籍数据
QList<Book> allBooks = Book::loadFromFile();
if (allBooks.isEmpty()) {
    qDebug() << "没有加载到书籍数据，可能是文件不存在或为空";
} else {
    qDebug() << "成功加载" << allBooks.size() << "本书";
    for (const Book& book : allBooks) {
        // 处理每本书...
    }
}*/
