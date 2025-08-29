#include "login.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

Login::Login(Database *db, QWidget *parent) : QDialog{parent}, db(db) {

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
    if (db->checkUserCredentials(username(), password())) {
        accept();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}