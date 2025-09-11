#include "profilepage.h"

#include <QVBoxLayout>
#include <QPushButton>


ProfilePage::ProfilePage(Database *db, User *user, QWidget *parent)
    : QWidget{parent}, db(db), user(user)
{
    // Properties

    setStyleSheet(QString(
            "background: %1; color: %2; border-radius: 8px;"
    ).arg(bgColorDark, textColorDark));

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // profileLayout
    QVBoxLayout *profileLayout = new QVBoxLayout(this);

    // header

    QLabel *header = new QLabel(user->username());

    header->setStyleSheet(QString(
        "font-size: 46px; font-weight: bold; background: %1; color: %2;"
    ).arg(bgColor, textColor));


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

    QPushButton *changePasswordButton = new QPushButton("Change Password", this);

    changePasswordLayout->addWidget(changePasswordButton);
    connect(changePasswordButton, &QPushButton::clicked, this, &ProfilePage::onChangePasswordClicked);

    QWidget *changePasswordWidget = new QWidget(this);
    changePasswordWidget->setFixedWidth(350);
    changePasswordWidget->setLayout(changePasswordLayout);


    profileLayout->setAlignment(Qt::AlignTop);
    profileLayout->setSpacing(2);
    profileLayout->addWidget(header);
    profileLayout->addWidget(changePasswordWidget);





}

void ProfilePage::onChangePasswordClicked()
{
    QString oldPassword = oldPasswordEdit->text();
    QString newPassword = newPasswordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();
    qDebug() << oldPassword;
    QString oldPasswordHashed = db->hashPassword(oldPassword);

    qDebug() << "old password: " << oldPassword << " | " << oldPasswordHashed;
    qDebug() << "Raw password (as hex):" << oldPassword.toUtf8().toHex();

    if (newPassword != confirmPassword) {
        return;
    }

    QString newPasswordHashed = db->hashPassword(newPassword);
    qDebug() << "new password: " << newPassword << " | " << newPasswordHashed;

    if (oldPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        // Show error message
        return;
    }

   
    if (!db->checkUserCredentials(user->username(), oldPasswordHashed)) {
        // Show error message
        return;
    }
    user->setPassword(newPasswordHashed);
    db->updateUser(user);
    // Optionally, show success message
}
