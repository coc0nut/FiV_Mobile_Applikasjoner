#include "profilepage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

ProfilePage::ProfilePage(Database *db, User *user, QWidget *parent)
    : QWidget{parent}, db(db), user(user)
{
    // Properties

    setStyleSheet(QString(
            "background: %1; color: %2; border-radius: 8px;"
        ).arg(bgColorDark, textColorDark, bgColor, textColor)
    );

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // profileLayout
    QVBoxLayout *profileLayout = new QVBoxLayout(this);

    // header

    QLabel *header = new QLabel("Profile");

    header->setStyleSheet(QString(
        "font-size: 46px; font-weight: bold; background: %1; color: %2;"
    ).arg(bgColor, textColor));

    // Set name and email

    QVBoxLayout *userDetailsLayout = new QVBoxLayout();
    userDetailsLayout->setSpacing(2);

    QLabel *userDetailsTitle = new QLabel("Userdetails", this);
    userDetailsTitle->setStyleSheet(QString("background: %1; color: %2; font-size: 23px; font-weight: bold;").arg(bgColorDark, textColorDark));
    userDetailsLayout->addWidget(userDetailsTitle);

    username = new QLabel("User: " + user->username());

    userDetailsLayout->addWidget(username);

    QHBoxLayout *nameLayout = new QHBoxLayout();

    QLabel *name = new QLabel("Name: ", this);
    nameLayout->addWidget(name);

    nameEdit = new QLineEdit(user->name());
    nameEdit->setStyleSheet(QString(
        "background: %1; color: %2;"
        "border-radius: 8px;"
        "font-size: 18px;"
        "padding: 10px;"
    ).arg(bgColorDark, textColorDark));
    nameEdit->setPlaceholderText("Enter your name");

    nameLayout->addWidget(nameEdit);
    userDetailsLayout->addLayout(nameLayout);

    QHBoxLayout *emailLayout = new QHBoxLayout();

    QLabel *email = new QLabel("Email: ", this);
    emailLayout->addWidget(email);

    emailEdit = new QLineEdit(user->email());
    emailEdit->setStyleSheet(QString(
        "background: %1; color: %2;"
        "border-radius: 8px;"
        "font-size: 18px;"
        "padding: 10px;"
        ).arg(bgColorDark, textColorDark));
    emailEdit->setPlaceholderText("Enter your email");

    emailLayout->addWidget(emailEdit);
    userDetailsLayout->addLayout(emailLayout);

    QWidget *userDetailsWidget = new QWidget(this);
    userDetailsWidget->setLayout(userDetailsLayout);

    QPushButton *changeUserDetailsButton = new QPushButton("Set details", this);
    changeUserDetailsButton->setStyleSheet(QString("background: %1; color: %2;").arg(bgColor, textColor));
    changeUserDetailsButton->setFixedHeight(40);

    userDetailsLayout->addWidget(changeUserDetailsButton);
    connect(changeUserDetailsButton, &QPushButton::clicked, this, [this]() {
        this->user->setName(nameEdit->text());
        this->user->setEmail(emailEdit->text());
        if (this->db->updateUser(this->user)) {
            emit userDetailsChanged();
            QMessageBox::information(this, "Success", "Profile updated successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to update profile.");
        }

    });

    // changePassowrdLayout

    QVBoxLayout *changePasswordLayout = new QVBoxLayout();
    changePasswordLayout->setSpacing(2);
    changePasswordLayout->setAlignment(Qt::AlignTop);

        chpasswd = new QLabel("Change Password", this);
        chpasswd->setStyleSheet(QString("background: %1; color: %2; font-size: 23px; font-weight: bold;").arg(bgColorDark, textColorDark));

        changePasswordLayout->addWidget(chpasswd);


        // old password edit
        oldPasswordEdit = new QLineEdit("", this);
        oldPasswordEdit->setStyleSheet(QString(
            "background: %1; color: %2;"
            "border-radius: 8px;"
            "font-size: 18px;"
            "padding: 10px;"
        ).arg(bgColorDark, textColorDark));
        oldPasswordEdit->setPlaceholderText("Enter old password...");
        oldPasswordEdit->setEchoMode(QLineEdit::Password);

    changePasswordLayout->addWidget(oldPasswordEdit);

        // new password edit
        newPasswordEdit = new QLineEdit("", this);
        newPasswordEdit->setStyleSheet(QString(
           "background: %1; color: %2;"
           "border-radius: 8px;"
           "font-size: 18px;"
           "padding: 10px;"
           ).arg(bgColorDark, textColorDark));
        newPasswordEdit->setPlaceholderText("Enter new password...");
        newPasswordEdit->setEchoMode(QLineEdit::Password);

    changePasswordLayout->addWidget(newPasswordEdit);

        // confirm new password
        confirmPasswordEdit = new QLineEdit("", this);
        confirmPasswordEdit->setStyleSheet(QString(
            "background: %1; color: %2;"
            "border-radius: 8px;"
            "font-size: 18px;"
            "padding: 10px;"
        ).arg(bgColorDark, textColorDark));
        confirmPasswordEdit->setPlaceholderText("Confirm new password...");
        confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    changePasswordLayout->addWidget(confirmPasswordEdit);

        // Changed Password button

    QPushButton *changePasswordButton = new QPushButton("Set password", this);
    changePasswordButton->setStyleSheet(QString("background: %1; color: %2;").arg(bgColor, textColor));
    changePasswordButton->setFixedHeight(40);

    changePasswordLayout->addWidget(changePasswordButton);
    connect(changePasswordButton, &QPushButton::clicked, this, &ProfilePage::onChangePasswordClicked);

    QWidget *changePasswordWidget = new QWidget(this);
    // changePasswordWidget->setFixedWidth(350);
    changePasswordWidget->setLayout(changePasswordLayout);


    // Profile page layout

    profileLayout->setAlignment(Qt::AlignTop);
    profileLayout->setSpacing(2);
    profileLayout->addWidget(header);
    profileLayout->addWidget(userDetailsWidget);
    profileLayout->addWidget(changePasswordWidget);

}

void ProfilePage::onChangePasswordClicked()
{
    QString oldPassword = oldPasswordEdit->text();
    QString newPassword = newPasswordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();
    // qDebug() << oldPassword;
    QString oldPasswordHashed = db->hashPassword(oldPassword);

    // qDebug() << "old password: " << oldPassword << " | " << oldPasswordHashed;
    // qDebug() << "Raw password (as hex):" << oldPassword.toUtf8().toHex();

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "Error", "Password confirmation failed.");
        return;
    }

    QString newPasswordHashed = db->hashPassword(newPassword);
    // qDebug() << "new password: " << newPassword << " | " << newPasswordHashed;

    if (oldPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields must be populated.");
        return;
    }

   
    if (!db->checkUserCredentials(user->username(), oldPasswordHashed)) {
        QMessageBox::warning(this, "Error", "Wrong current password");
        return;
    }
    user->setPassword(newPasswordHashed);
    db->updateUser(user);
    // Optionally, show success message
}
