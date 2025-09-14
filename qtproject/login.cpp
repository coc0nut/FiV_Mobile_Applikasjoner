#include "login.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

Login::Login(Database *db, User *user, QWidget *parent)
    : QDialog{parent}, db(db), user(user) {

    setStyleSheet(QString("background: %1; color: %2;").arg(bgColorDark, textColorDark));

    setWindowTitle("Note.io");
    resize(640, 320);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget *formContainer = new QWidget(this);
    QVBoxLayout *formLayout = new QVBoxLayout(formContainer);
    formLayout->setContentsMargins(32, 32, 32, 32);

    // Username layout
    QHBoxLayout *userLayout = new QHBoxLayout();
    
    userLayout->addWidget(new QLabel("username:"));
    userNameEdit = new QLineEdit();
    userLayout->addWidget(userNameEdit);
    
    // Pass layout
    QHBoxLayout *passLayout = new QHBoxLayout();
    
    passLayout->addWidget(new QLabel("password:"));
    passWordEdit = new QLineEdit();
    passWordEdit->setEchoMode(QLineEdit::Password);
    passLayout->addWidget(passWordEdit);

    // Login button layout
    loginButton = new QPushButton("Login");
    connect(loginButton, &QPushButton::clicked, this, &Login::onLoginClicked);

    newAccountButton = new QPushButton("New Account");
    connect(newAccountButton, &QPushButton::clicked, this, &Login::onNewAccountClicked);

    formLayout->addLayout(userLayout);
    formLayout->addLayout(passLayout);
    formLayout->addWidget(loginButton);
    formLayout->addWidget(newAccountButton);

    // formContainer->setStyleSheet(
    //     "background: #fff;"
    //     "border-radius: 16px;"
    //     "border: 1px solid #ccc;"
    // );

    mainLayout->addStretch();
    mainLayout->addWidget(formContainer, 0, Qt::AlignCenter);
    mainLayout->addStretch();
}

QString const Login::username() {
    return userNameEdit->text();
}

QString const Login::password() {
    return passWordEdit->text();
}



void Login::onLoginClicked() {
    QString hashed = db->hashPassword(password());
    if (db->checkUserCredentials(username(), hashed)) {
        accept();

    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

void Login::onNewAccountClicked() {
    QString hashed = db->hashPassword(password());
    if (db->checkUserExists(username())) {
        QMessageBox::warning(this, "Username exists", "Choose another username");
    } else if (!db->checkUserCredentials(username(), hashed)) {
        if (db->addUser(username(), hashed)) {
            QMessageBox::information(this, "Account created", "Your account was successfully created. Please login.");
        } else {
            QMessageBox::warning(this, "Could not add user", "Maybe user already exists.");
        }
    } else {
        QMessageBox::warning(this, "Failed to create account", "User already exists.");
    }
}
