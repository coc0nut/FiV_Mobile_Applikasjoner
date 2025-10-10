#include "networkmanager.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

NetworkManager::NetworkManager(User *user, Todo *todo, QObject *parent)
    : QObject{parent}, user(user), todo(todo)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &NetworkManager::onReplyFinished);

    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setSingleShot(true);
    connect(m_refreshTimer, &QTimer::timeout, this, &NetworkManager::onRefreshTimeout);
}

bool NetworkManager::login(QString const &username, QString const &password) {
    if (!m_manager)
        return false;
    if (username.isEmpty() || password.isEmpty())
        return false;

    QJsonObject payload;
    payload.insert("username", username);
    payload.insert("password", password);
    QJsonDocument doc(payload);
    QByteArray body = doc.toJson();

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("api/token/")));
    QNetworkRequest req(apiUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QNetworkReply *r = m_manager->post(req, body);
    if (!r)
        return false;

    r->setProperty("requestType", "login");
    reply = r;
    return true;
}

bool NetworkManager::fetchCurrentUser()
{
    if (!m_manager) return false;
    if (accessToken.isEmpty()) {
        qDebug() << "fetchCurrentUser: no access token available";
        return false;
    }

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("me/")));
    QNetworkRequest req(apiUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    if (accessToken.count('.') == 2) {
        req.setRawHeader("Authorization", QByteArray("Bearer ") + accessToken.toUtf8());
    } else {
        req.setRawHeader("Authorization", QByteArray("Token ") + accessToken.toUtf8());
    }

    QNetworkReply *r = m_manager->get(req);
    if (!r) return false;
    r->setProperty("requestType", "me");
    reply = r;
    return true;
}

bool NetworkManager::updateCurrentUser(QString const &firstName, QString const &lastName, QString const &email) {
    if (!m_manager) return false;
    if (accessToken.isEmpty()) {
        qDebug() << "updateCurrentUser: no access token available";
        return false;
    }

    QJsonObject payload;
    if (!firstName.isEmpty()) payload.insert("first_name", firstName);
    if (!lastName.isEmpty())  payload.insert("last_name", lastName);
    if (!email.isEmpty())  payload.insert("email", email);

    QJsonDocument doc(payload);
    QByteArray body = doc.toJson();

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("me/")));
    QNetworkRequest req(apiUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    if (accessToken.count('.') == 2) {
        req.setRawHeader("Authorization", QByteArray("Bearer " + accessToken.toUtf8()));
    } else {
        req.setRawHeader("Authorization", QByteArray("Token ") + accessToken.toUtf8());
    }

    QNetworkReply *r = m_manager->put(req, body);
    if (!r) return false;
    r->setProperty("requestType", "updateMe");
    reply = r;
    return true;
}

bool NetworkManager::setRefreshToken(QString const &refreshTokenIn) {

    if (!m_manager) { return false; }

    QString token = refreshTokenIn;
    
    if (token.isEmpty()) {
        token = refreshToken;
    }
    
    if (token.isEmpty()) { return false; }

    QJsonObject payload;
    payload.insert("refresh", token);
    QJsonDocument doc(payload);
    QByteArray body = doc.toJson();

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("api/token/refresh/")));
    QNetworkRequest req(apiUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    QNetworkReply *r = m_manager->post(req, body);
    if (!r) { return false; }
    r->setProperty("requestType", "refresh");
    reply = r;

    return true;
}

void NetworkManager::onReplyFinished(QNetworkReply *reply) {
    if (!reply) return;

    QByteArray const response = reply->readAll();
    QString const requestType = reply->property("requestType").toString();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << "HTTP response for" << requestType << "- status:" << statusCode.toInt();
    qDebug() << "Body:" << response;

    if (reply->error() != QNetworkReply::NoError) {

        if (requestType == QLatin1String("refresh")) {
            emit refreshFailed(reply->errorString());
        } else if (requestType == QLatin1String("me")) {
            emit currentUserFetchedFailed(reply->errorString());
        } else if (requestType == QLatin1String("updateMe")) {
            emit currentUserUpdateFailed(reply->errorString());
        }else if (requestType == QLatin1String("login")) {
            emit loginFailed(reply->errorString());
        }
        reply->deleteLater();
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        if (requestType == QLatin1String("refresh")) {
            emit refreshFailed(QStringLiteral("Invalid JSON Response"));
        } else {
            emit loginFailed(QStringLiteral("Invalid JSON response"));
        }
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();

    if (requestType == QLatin1String("updateMe")) {
        if (user) {
            if (obj.contains("first_name")) user->setFirst_name(obj.value("first_name").toString());
            if (obj.contains("last_name")) user->setLast_name(obj.value("last_name").toString());
            if (obj.contains("email")) user->setEmail(obj.value("email").toString());

            qDebug() << "User updated:" << user->username() << "ID:" << user->id();
        }
        emit currentUserUpdated(obj);
        reply->deleteLater();
        return;
    }

    if (requestType == QLatin1String("me")) {
        if (user) {
            if (obj.contains("id")) user->setId(obj.value("id").toInt());
            if (obj.contains("username")) user->setUsername(obj.value("username").toString());
            if (obj.contains("email")) user->setEmail(obj.value("email").toString());
            if (obj.contains("first_name")) user->setFirst_name(obj.value("first_name").toString());
            if (obj.contains("last_name")) user->setLast_name(obj.value("last_name").toString());
            if (obj.contains("last_login")) user->setLast_login(obj.value("last_login").toString());
            if (obj.contains("is_superuser")) user->setIs_superuser(obj.value("is_superuser").toBool());
            if (obj.contains("is_staff")) user->setIs_staff(obj.value("is_staff").toBool());
            if (obj.contains("is_active")) user->setIs_active(obj.value("is_active").toBool());
            if (obj.contains("date_joined")) user->setDate_joined(obj.value("date_joined").toString());
            
            qDebug() << "User populated:" << user->username() << "ID:" << user->id();
        }
        emit currentUserFetched(obj);
        reply->deleteLater();
        return;
    }

    if (requestType == QLatin1String("login")) {
        if (obj.contains("access")) {
            accessToken = obj.value("access").toString();
            refreshToken = obj.value("refresh").toString();
            emit loginSucceeded(accessToken, refreshToken, 0, QString());

            // start test refresh timer (60 seconds) for testing purposes
            if (m_refreshTimer) {
                const int testIntervalMs = (60 * 5) * 1000; // 60 seconds (testing)
                m_refreshTimer->start(testIntervalMs);
                qDebug() << "Refresh timer started for testing (seconds):" << testIntervalMs / 1000;
            }

            reply->deleteLater();
            return;
        }

        if (obj.contains("token")) {
            QString token = obj.value("token").toString();
            int userId = obj.value("user_id").toInt(0);
            QString username = obj.value("username").toString();
            // store token in access for TokenAuth
            accessToken = token;
            refreshToken.clear();
            emit loginSucceeded(accessToken, refreshToken, userId, username);
            // no refresh scheduling for TokenAuth
            reply->deleteLater();
            return;
        }

        emit loginFailed(QStringLiteral("Unexpected login response"));
        reply->deleteLater();
        return;
    }

    if (requestType == QLatin1String("refresh")) {

        if (obj.contains("access")) {
            accessToken = obj.value("access").toString();
            if (obj.contains("refresh")) {
                refreshToken = obj.value("refresh").toString();
            }

            emit refreshSucceeded(accessToken);
            // restart refresh timer (300 seconds)
            if (m_refreshTimer) {
                const int testIntervalMs = (60 * 5) * 1000; // 60 seconds (testing)
                m_refreshTimer->start(testIntervalMs);
                qDebug() << "Refresh timer restarted for testing (seconds):" << testIntervalMs / 1000;
            }

            reply->deleteLater();
            return;
        }

        emit refreshFailed(QStringLiteral("Unexpected refresh response"));
        reply->deleteLater();
        return;
    }
    reply->deleteLater();
}

void NetworkManager::onRefreshTimeout()
{
    // attempt refresh using stored refreshToken
    if (!refreshToken.isEmpty()) {
        qDebug() << "Refresh timer fired, requesting new access token";
        setRefreshToken();
    } else {
        qDebug() << "No refresh token available; cannot refresh";
        emit refreshFailed(QStringLiteral("No refresh token available"));
    }
}

QString NetworkManager::getAccessToken() const
{
    return accessToken;
}

void NetworkManager::setAccessToken(const QString &newAccessToken)
{
    accessToken = newAccessToken;
}
