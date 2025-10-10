#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QUrl>
#include <QTimer>
#include "todo.h"
#include "user.h"

class QNetworkAccessManager;
class QNetworkReply;

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(User *user, Todo *todo, QObject *parent = nullptr);

    bool login(QString const &username, QString const &password);
    
    bool fetchCurrentUser();
    bool updateCurrentUser(QString const &firstName, QString const &lastName, QString const &email);
    
    void setUrl(QUrl const &url) { m_url = url; }
    QUrl getUrl() { return m_url; }
    
    QString getAccessToken() const;
    void setAccessToken(const QString &newAccessToken);
    
    QString getRefreshToken() const;
    bool setRefreshToken(const QString &refreshTokenIn = QString());

signals:
    void loginSucceeded(QString const &accessToken, QString const &refreshToken, int userId, QString const &username);
    void loginFailed(QString const &error);

    void refreshSucceeded(QString const &newAccessToken);
    void refreshFailed(QString const &error);

    void currentUserFetched(QJsonObject const &userObj);
    void currentUserFetchedFailed(QString const &error);

    void currentUserUpdated(const QJsonObject &userObj);
    void currentUserUpdateFailed(const QString &error);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onRefreshTimeout();


private:
    QUrl m_url = QUrl(QStringLiteral("http://127.0.0.1:8000"));
    QNetworkAccessManager *m_manager = nullptr;
    QNetworkReply *reply = nullptr;
    QString accessToken, refreshToken;

    QTimer *m_refreshTimer = nullptr;
    int m_refreshMarginSeconds = 60;

    User *user;
    Todo *todo;
};

#endif // NETWORKMANAGER_H
