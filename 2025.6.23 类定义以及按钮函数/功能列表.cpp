// MainWindow.cpp
#include "MainWindow.h"
#include <QMessageBox>


// MainWindow.h
#include "Book.h"
#include "User.h"
#include "ui_mainwindow.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

//这里的MAINWindow类就是mainwindow.h的补充，类后面的函数补充定义是对mainwindow.cpp的补充
//补充的主要功能都是按按钮的事件，功能列在下面了，kfls检查一下按钮对不对得上
//可以检查看一下函数内命名正不正确，复制粘贴到mainwindow.h就可以了
//类下面的函数完整定义复制粘贴到mainwindow.cpp里面
//这份文件里可能会有一些报错，好像是因为重定义（因为已经有了一份mainwindow.h的头文件）
/*代码中出现了一些Widget内容，ds解释如下：
在Qt框架中，transactionListWidget 是一个 QListWidget 控件，用于在图形界面中显示用户的交易历史记录列表。以下是详细说明：
*QListWidget 的基本概念
    作用：显示可滚动的条目列表（每个条目可以是文字、图标等）
    典型用途：消息记录、历史记录、文件列表等需要逐条展示的场景
    对应头文件：#include <QListWidget>

*在项目中的具体应用
    在Qt Designer中的添加方式
    打开 .ui 文件
    从控件栏拖拽 List Widget 到窗口
    右键重命名为 transactionListWidget
*/

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
        // ... 其他代码 ...

private slots:
    void on_registerButton_clicked()        // 注册按钮点击事件
    void on_loginButton_clicked();          // 登录按钮点击槽函数
    void on_logoutButton_clicked()          // 退出登陆点击槽函数
    void logout();                          // 显式声明退出函数（也可直接使用槽函数）
    void on_uploadBookButton_clicked();     // 上传书籍按钮点击事件
    void on_removeBookButton_clicked();     // 下架书籍按钮点击事件
    void on_rechargeButton_clicked();       // 充值按钮点击事件
    void updateBalanceDisplay();            // 更新余额显示
    void on_purchaseButton_clicked();       // 购买书籍按钮
    void on_searchButton_clicked();         // 搜索按钮点击槽
    void on_editProfileButton_clicked()     // 用户编辑资料功能按钮
    void on_changePasswordButton_clicked()  // 用户修改密码按钮


private:
    Ui::MainWindow *ui;
    User* currentUser;                   // 当前登录的用户对象
    UserManager userManager;             // 用户管理对象
    void refreshMyBooksList();           // 刷新书籍列表显示
    void refreshBookListDisplay();       // 更新书架
    Book* getSelectedBook() const;
    void MainWindow::displayTransactionHistory() //显示交易记录（选择性添加）
    void handleSuccessfulLogin(User* user);  // 登录成功后的处理
    void cleanupAfterLogout(); //退出后的处理
    bool validateAndSave(const Ui::ProfileDialog& ui); //用户修改资料是否合法
    void showChangePasswordDialog(); //用户修改密码
    bool isValidContact(const QString& contact)  //用户修改的联系方式是否合法
    bool validatePasswordChange(const QString &oldPass, const QString &newPass) // 用户修改的新密码是否合法
    QList<Book*> searchBooks(const SearchParams& params); //搜索算法
    //下面是ds给的一个搜索参数例子
    struct SearchParams {
        QString keyword;
        QStringList tags;
        double minPrice = 0;
        double maxPrice = 10000;
        bool onlyAvailable = true;
    };
};


//注册按钮（实现功能：新建用户） 函数声明在private slots里面
void MainWindow::on_registerButton_clicked() {
    // 1. 获取用户输入
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    QString confirmPassword = ui->confirmPasswordEdit->text();
    QString contact = ui->contactEdit->text().trimmed();

    // 2. 验证输入
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名和密码不能为空！");
        return;
    }
    if (password != confirmPassword) {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致！");
        return;
    }
    if (password.length() < 6) {
        QMessageBox::warning(this, "错误", "密码长度至少为6位！");
        return;
    }

    // 3. 调用用户管理逻辑
    if (userManager.createUser(username, password, contact)) {
        QMessageBox::information(this, "成功", "注册成功！");
        ui->stackedWidget->setCurrentIndex(1);  // 跳转到登录页面
    } else {
        QMessageBox::critical(this, "错误", "用户名已存在！");
    }
}

//登陆功能
void MainWindow::on_loginButton_clicked() {
    // 1. 获取用户输入
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    // 2. 输入验证
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户名和密码不能为空");
        return;
    }

    // 3. 调用UserManager验证登录
    User* user = userManager.getUser(username);

    // 4. 验证用户是否存在及密码是否正确
    if (!user || !user->verifyPassword(password.toStdString())) {
        QMessageBox::critical(this, "登录失败", "用户名或密码错误");

        // 安全建议：清空密码框
        ui->passwordEdit->clear();

        return;
    }

    // 5. 登录成功处理
    handleSuccessfulLogin(user);
}

//退出登陆
void MainWindow::on_logoutButton_clicked() {
    // 确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认退出",
                                  "确定要退出登录吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    logout();
}


void MainWindow::logout() {
    if (!currentUser) return;

    // 1. 保存临时数据
    userManager.saveUsersToFile();  // 持久化用户数据
    saveCartToLocal();             // 示例：保存购物车到本地

    // 2. 清理用户状态
    cleanupAfterLogout();

    // 3. 切换界面
    ui->stackedWidget->setCurrentIndex(0);  // 返回登录页
    ui->statusBar->showMessage("已安全退出", 3000);

    // 4. 日志记录
    qInfo() << "用户退出:" << QString::fromStdString(currentUser->username);
}

void MainWindow::cleanupAfterLogout() {
    // 清理敏感信息
    ui->passwordEdit->clear();

    // 重置界面状态
    ui->welcomeLabel->setText("请登录");
    ui->balanceLabel->setText("0.00");

    // 禁用需登录的功能
    ui->uploadButton->setEnabled(false);
    ui->purchaseButton->setEnabled(false);

    // 释放内存
    currentUser = nullptr;
}

void MainWindow::handleSuccessfulLogin(User* user) {
    // 1. 保存当前用户指针
    currentUser = user;

    // 2. 更新界面状态
    ui->stackedWidget->setCurrentIndex(1);  // 跳转到主页面
    ui->welcomeLabel->setText(tr("欢迎，%1！").arg(QString::fromStdString(user->username)));
    updateBalanceDisplay();  // 更新余额显示
    refreshBookListDisplay(); // 刷新书籍列表

    // 3. 记录登录状态（可选）
    qInfo() << "用户登录:" << QString::fromStdString(user->username);

    // 4. 启用需要登录的功能
    ui->uploadBookButton->setEnabled(true);
    ui->purchaseButton->setEnabled(true);
}

//上传书功能 函数声明在private slots里面
void MainWindow::on_uploadBookButton_clicked() {
    // 1. 检查用户是否已登录
    if (!currentUser) {
        QMessageBox::warning(this, "错误", "请先登录！");
        return;
    }

    // 2. 获取用户输入的书籍信息
    QString title = ui->bookTitleEdit->text().trimmed();
    double price = ui->priceSpinBox->value();
    QString tagsText = ui->tagsEdit->text().trimmed();

    // 3. 验证输入合法性
    if (title.isEmpty()) {
        QMessageBox::warning(this, "错误", "书籍名称不能为空！");
        return;
    }
    if (price <= 0) {
        QMessageBox::warning(this, "错误", "价格必须大于0！");
        return;
    }

    // 4. 处理标签（将逗号分隔的字符串转为 QStringList）
    QStringList tags = tagsText.split(',', Qt::SkipEmptyParts);
    for (QString& tag : tags) {
        tag = tag.trimmed();
    }

    // 5. 创建 Book 对象并关联当前用户
    Book* newBook = new Book();
    newBook->title = title;
    newBook->price = price;
    newBook->tags = tags;
    newBook->sellerId = QString::fromStdString(currentUser->username); // 自动填入卖家信息
    newBook->listedTime = QDateTime::currentDateTime();               // 记录上架时间

    // 6. 添加到用户的在售书籍列表
    currentUser->addBookForSale(newBook);

    // 7. 保存数据到文件
    userManager.saveUsersToFile();

    // 8. 提示用户并刷新界面
    QMessageBox::information(this, "成功", "书籍上传成功！");
    ui->bookTitleEdit->clear();
    ui->priceSpinBox->setValue(0);
    ui->tagsEdit->clear();

    // 可选：刷新书籍列表显示
    refreshMyBooksList();
}


//下架书籍功能 函数声明在private slots里面
void MainWindow::on_removeBookButton_clicked() {
    // 1. 检查用户是否已登录
    if (!currentUser) {
        QMessageBox::warning(this, "错误", "请先登录！");
        return;
    }

    // 2. 获取当前选中的书籍（假设使用QListWidget显示书籍列表）
    QListWidgetItem* selectedItem = ui->myBooksList->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "错误", "请选择要下架的书籍！");
        return;
    }

    // 3. 从item中获取Book对象（假设设置了UserData）
    Book* bookToRemove = selectedItem->data(Qt::UserRole).value<Book*>();
    if (!bookToRemove) {
        QMessageBox::critical(this, "错误", "书籍数据无效！");
        return;
    }

    // 4. 确认操作
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认",
                                  QString("确定要下架《%1》吗？").arg(bookToRemove->title),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    // 5. 从用户书籍列表中移除
    currentUser->removeBookFromSale(bookToRemove);

    // 6. 更新数据文件
    userManager.saveUsersToFile();

    // 7. 刷新界面
    refreshMyBooksList();
    QMessageBox::information(this, "成功", "书籍已下架！");

    // 8. 释放内存（如果不再需要该Book对象）
    delete bookToRemove;
}


//下架书籍后更新个人书籍功能 函数声明在private里面
void MainWindow::refreshMyBooksList() {
    ui->myBooksList->clear();
    if (!currentUser) return;

    for (Book* book : currentUser->getBooksOnSale()) {
        QListWidgetItem* item = new QListWidgetItem(
            QString("%1 - ¥%2 - 标签: %3")
                .arg(book->title)
                .arg(book->price)
                .arg(book->tags.join(", "))
            );
        item->setData(Qt::UserRole, QVariant::fromValue(book));
        ui->myBooksList->addItem(item);
    }
}


//充值功能按钮
void MainWindow::on_rechargeButton_clicked() {
    // 1. 检查用户是否登录
    if (!currentUser) {
        QMessageBox::warning(this, "错误", "请先登录！");
        return;
    }

    // 2. 获取充值金额
    double amount = ui->amountSpinBox->value();

    // 3. 验证金额有效性
    if (amount <= 0) {
        QMessageBox::warning(this, "错误", "充值金额必须大于0！");
        return;
    }

    // 4. 调用用户充值方法
    currentUser->rechargeTokens(amount);

    // 5. 更新界面显示
    updateBalanceDisplay();

    // 6. 保存数据
    userManager.saveUsersToFile();

    // 7. 提示用户
    QMessageBox::information(this, "成功",
                             QString("成功充值 %1 代币！当前余额: %2").arg(amount).arg(currentUser->getTokens()));
}


//显示充值后的余额功能
void MainWindow::updateBalanceDisplay() {
    if (currentUser) {
        ui->balanceLabel->setText(
            QString("当前余额: %1").arg(currentUser->getTokens()));
    } else {
        ui->balanceLabel->setText("当前余额: --");
    }
}


//购买书籍
void MainWindow::on_purchaseButton_clicked() {
    // 1. 检查用户登录
    if (!currentUser) {
        QMessageBox::warning(this, "错误", "请先登录！");
        return;
    }

    // 2. 获取选中的书籍
    Book* selectedBook = getSelectedBook();
    if (!selectedBook || selectedBook->isSold) {
        QMessageBox::warning(this, "错误", "请选择有效书籍！");
        return;
    }

    // 3. 检查是否是自己的书
    if (selectedBook->sellerId == QString::fromStdString(currentUser->username)) {
        QMessageBox::warning(this, "错误", "不能购买自己的书籍！");
        return;
    }

    // 4. 确认对话框
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "确认购买",
                                    QString("确定要购买《%1》吗？价格：%2 代币").arg(selectedBook->title).arg(selectedBook->price),
                                    QMessageBox::Yes | QMessageBox::No);

    if (confirm != QMessageBox::Yes) return;

    // 5. 执行交易
    if (currentUser->purchaseBook(selectedBook)) {
        // 6. 更新数据
        userManager.saveUsersToFile();

        // 7. 刷新界面
        refreshBookListDisplay();
        updateBalanceDisplay();

        QMessageBox::information(this, "成功", "购买成功！");
    } else {
        QMessageBox::warning(this, "失败", "余额不足或交易失败！");
    }
}

Book* MainWindow::getSelectedBook() const {
    QModelIndex index = ui->booksTableView->currentIndex();
    if (index.isValid()) {
        return index.data(Qt::UserRole).value<Book*>();
    }
    return nullptr;
}

void MainWindow::refreshBookListDisplay() {
    model->clear(); // QStandardItemModel

    for (Book* book : allBooks) {
        QStandardItem* item = new QStandardItem(
            QString("%1\n作者: %2\n价格: %3").arg(book->title).arg(book->author).arg(book->price));

        item->setData(QVariant::fromValue(book), Qt::UserRole);
        item->setEnabled(!book->isSold);  // 已售书籍禁用选择
        model->appendRow(item);
    }
    ui->booksTableView->setModel(model);
}

// 显示交易记录（选择性添加）
void MainWindow::displayTransactionHistory() {
    ui->transactionListWidget->clear();

    if (!currentUser) return;

    for (const auto& record : currentUser->getTransactionHistory()) {
        QString entry = QString("[%1] %2: %3 (%4)")
        .arg(record.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(record.type)
            .arg(record.amount)
            .arg(record.detail);

        ui->transactionListWidget->addItem(entry);
    }
}

// 搜索部分
void MainWindow::on_searchButton_clicked() {
    // 1. 收集搜索参数
    SearchParams params;
    params.keyword = ui->searchLineEdit->text().trimmed();
    params.tags = ui->tagsEdit->text().split(',', Qt::SkipEmptyParts);
    params.minPrice = ui->minPriceSpinBox->value();
    params.maxPrice = ui->maxPriceSpinBox->value();
    params.onlyAvailable = ui->availableCheckBox->isChecked();

    // 2. 调用搜索接口（交给zjls了）
    QList<Book*> results = searchBooks(params);

    // 3. 显示结果
    displaySearchResults(results);
}

void MainWindow::displaySearchResults(const QList<Book*>& books) {
    ui->resultsTable->clearContents();
    ui->resultsTable->setRowCount(books.size());

    for (int i = 0; i < books.size(); ++i) {
        Book* book = books.at(i);
        ui->resultsTable->setItem(i, 0, new QTableWidgetItem(book->title));
        // ...填充其他列...
    }
}

// 搜索部分（ds建议）
QList<Book*> MainWindow::searchBooks(const SearchParams& params) {
    
    QUrl url("http://localhost:8080/api/books/search");
    QVariantMap fieldQueries;

    if (!params.keyword.isEmpty()) {
        fieldQueries["author"] = params.keyword;
        fieldQueries["title"] = params.keyword;
        fieldQueries["description"] = params.keyword;
    }

    if (!params.tags.isEmpty()) {
        fieldQueries["tags"] = params.tags.join(" ");
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject requestBody = QJsonObject::fromVariantMap({
        {"fieldQueries", QJsonObject::fromVariantMap(fieldQueries)},
        {"topN", ui->resultCountSpinBox->value()},
        {"minPrice", params.minPrice},
        {"maxPrice", params.maxPrice}
    });

    
    QNetworkReply *reply = networkManager->post(
        request,
        QJsonDocument(requestBody).toJson()
    );

    // TODO: 处理reply



    
    // 示例伪代码
    QList<Book*> matchedBooks;

    for (Book* book : allBooks) {
        // 条件1：关键词匹配（书名/作者/描述）
        bool keywordMatch = params.keyword.isEmpty() ||
                            book->title.contains(params.keyword, Qt::CaseInsensitive) ||
                            book->author.contains(params.keyword, Qt::CaseInsensitive);

        // 条件2：标签匹配
        bool tagsMatch = params.tags.isEmpty() ||
                         std::any_of(params.tags.begin(), params.tags.end(),
                                     [&](const QString& tag){ return book->tags.contains(tag); });

        // 条件3：价格区间
        bool priceMatch = book->price >= params.minPrice &&
                          book->price <= params.maxPrice;

        // 条件4：仅显示可购
        bool availableMatch = !params.onlyAvailable || !book->isSold;

        if (keywordMatch && tagsMatch && priceMatch && availableMatch) {
            matchedBooks.append(book);
        }
    }

    return matchedBooks;
}

//用户编辑资料功能
void MainWindow::on_editProfileButton_clicked() {
    if (!currentUser) return;

    // 创建编辑对话框
    QDialog dialog(this);
    Ui::ProfileDialog ui;
    ui.setupUi(&dialog);

    // 加载当前数据
    ui.usernameEdit->setText(QString::fromStdString(currentUser->username));
    ui.contactEdit->setText(QString::fromStdString(currentUser->contact));
    loadAvatar(ui.avatarLabel, currentUser->avatarPath);

    // 连接保存按钮
    connect(ui.saveButton, &QPushButton::clicked, [&]() {
        if (validateAndSave(ui)) {
            dialog.accept();
        }
    });

    dialog.exec();
}

bool MainWindow::validateAndSave(const Ui::ProfileDialog& ui) {
    // 1. 验证用户名
    QString newUsername = ui.usernameEdit->text().trimmed();
    if (newUsername.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名不能为空");
        return false;
    }

    // 2. 验证联系方式
    QString newContact = ui.contactEdit->text().trimmed();
    if (!isValidContact(newContact)) {  // 需实现验证函数
        QMessageBox::warning(this, "错误", "联系方式格式不正确");
        return false;
    }

    // 3. 更新用户数据
    currentUser->username = newUsername.toStdString();
    currentUser->contact = newContact.toStdString();

    // 4. 触发保存
    userManager.saveUsersToFile();

    // 5. 更新界面
    updateProfileDisplay();
    return true;
}

bool MainWindow::isValidContact(const QString& contact) {
    // 手机号验证
    QRegularExpression phoneRegex(R"(^1[3-9]\d{9}$)");
    // 邮箱验证
    QRegularExpression emailRegex(R"(^\w+@[a-zA-Z_]+?\.[a-zA-Z]{2,3}$)");

    return phoneRegex.match(contact).hasMatch() ||
           emailRegex.match(contact).hasMatch();
}


//修改密码
void MainWindow::on_changePasswordButton_clicked() {
    if (!currentUser) return;

    // 1. 专用密码修改对话框
    QDialog dialog(this);
    Ui::PasswordDialog ui;
    ui.setupUi(&dialog);

    // 2. 连接验证逻辑
    connect(ui.confirmButton, &QPushButton::clicked, [&]() {
        QString oldPass = ui.oldPassEdit->text();
        QString newPass = ui.newPassEdit->text();
        QString confirmPass = ui.confirmPassEdit->text();

        if (validatePasswordChange(oldPass, newPass, confirmPass)) {
            currentUser->setPassword(newPass.toStdString()); // 内部会加密
            userManager.saveUsersToFile();
            QMessageBox::information(this, "成功", "密码已更新");
            dialog.accept();
        }
    });

    dialog.exec();
}

bool MainWindow::validatePasswordChange(
    const QString& oldPass,
    const QString& newPass,
    const QString& confirmPass
    ) {
    // 独立密码验证逻辑
    if (!currentUser->verifyPassword(oldPass.toStdString())) {
        showError("原密码不正确");
        return false;
    }
    if (newPass != confirmPass) {
        showError("两次输入的新密码不一致");
        return false;
    }
    if (newPass.length() < 8) {
        showError("密码长度至少8位");
        return false;
    }
    return true;
}

