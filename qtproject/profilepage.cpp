#include "profilepage.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

ProfilePage::ProfilePage(NetworkManager *net, Database *db, User *user, QWidget *parent)
    : QWidget{parent}, db(db), user(user), net(net)
{
    // Properties

    setStyleSheet(QString(
            "background: %1; color: %2; border-radius: 8px;"
        ).arg(bgColorDark, textColorDark)
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
    firstName = new QLabel("First name: " + user->first_name());
    lastName = new QLabel("Last name: " + user->last_name());

    userDetailsLayout->addWidget(username);


    QHBoxLayout *first_nameLayout = new QHBoxLayout();
    QLabel *first_name = new QLabel("First name: ", this);
    first_nameLayout->addWidget(first_name);

    first_nameEdit = new QLineEdit(user->first_name());
    first_nameEdit->setStyleSheet(QString(
        "background: %1; color: %2;"
        "border-radius: 8px;"
        "font-size: 18px;"
        "padding: 10px;"
    ).arg(bgColorDark, textColorDark));
    first_nameEdit->setPlaceholderText("Enter your first name");

    first_nameLayout->addWidget(first_nameEdit);
    userDetailsLayout->addLayout(first_nameLayout);

    QHBoxLayout *last_nameLayout = new QHBoxLayout();
    QLabel *last_name = new QLabel("Last name: ", this);
    last_nameLayout->addWidget(last_name);

    last_nameEdit = new QLineEdit(user->last_name());
    last_nameEdit->setStyleSheet(QString(
        "background: %1; color: %2;"
        "border-radius: 8px;"
        "font-size: 18px;"
        "padding: 10px;"
    ).arg(bgColorDark, textColorDark));
    last_nameEdit->setPlaceholderText("Enter your last name");

    last_nameLayout->addWidget(last_nameEdit);
    userDetailsLayout->addLayout(last_nameLayout);

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

    changeUserDetailsButton = new QPushButton("Set details", this);
    changeUserDetailsButton->setStyleSheet(QString("background: %1; color: %2;").arg(bgColor, textColor));
    changeUserDetailsButton->setFixedHeight(40);

    userDetailsLayout->addWidget(changeUserDetailsButton);
    connect(changeUserDetailsButton, &QPushButton::clicked, this, [this]() {
        QString firstName = first_nameEdit->text();
        QString lastName = last_nameEdit->text();
        QString email = emailEdit->text();



        if (this->net->updateCurrentUser(firstName, lastName, email)) {
            this->user->setFirst_name(firstName);
            this->user->setLast_name(lastName);
            this->user->setEmail(email);
        } else {
            QMessageBox::warning(this, "Error", "Failed to start update request.");
        }

    });

    connect(net, &NetworkManager::currentUserUpdated, this, [this](const QJsonObject &obj) {
        changeUserDetailsButton->setEnabled(true);
        emit userDetailsChanged();  // Emit signal to notify other components
        QMessageBox::information(this, "Success", "Profile updated successfully.");
    });

    connect(net, &NetworkManager::currentUserUpdateFailed, this, [this](const QString &error) {
        changeUserDetailsButton->setEnabled(true);
        QMessageBox::warning(this, "Error", QString("Failed to update profile: %1").arg(error));
    });

    connect(net, &NetworkManager::passwordChanged, this, [this](const QString &message) {
        changePasswordButton->setEnabled(true);
        changePasswordButton->setText("Set password");

        oldPasswordEdit->clear();
        newPasswordEdit->clear();
        confirmPasswordEdit->clear();

        QMessageBox::information(this, "Success", "Password changed successfully!");
        qDebug() << "ProfilePage: Password changed successfully:" << message;
    });

    connect(net, &NetworkManager::passwordChangeFailed, this, [this](const QString &error) {
        changePasswordButton->setEnabled(true);
        changePasswordButton->setText("Set password");

        QMessageBox::warning(this, "Error", QString("Failed to change password: %1").arg(error));
        qDebug() << "ProfilePage: Password change failed:" << error;
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

    changePasswordButton = new QPushButton("Set password", this);
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

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "Error", "Password confirmation failed.");
        return;
    }

    if (oldPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "All password fields must be populated.");
        return;
    }

    changePasswordButton->setEnabled(false);
    changePasswordButton->setText("Changing password");
   
    if (!net->changePassword(oldPassword, newPassword, confirmPassword)) {
        QMessageBox::warning(this, "Error", "Wrong current password");
        return;
    }

}
