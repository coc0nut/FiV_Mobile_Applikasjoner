#include "networkmanager.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>
#include <QJsonArray>

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

bool NetworkManager::fetchCurrentUser() {
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

bool NetworkManager::changePassword(QString const &old_password, QString const &new_password, QString const &confirm_password) {
    if (!m_manager) return false;
    if (accessToken.isEmpty()) {
        qDebug() << "changePassword: no access token available";
        return false;
    }

    QJsonObject payload;
    payload.insert("old_password", old_password);
    payload.insert("new_password", new_password);
    payload.insert("confirm_password", confirm_password);

    QJsonDocument doc(payload);
    QByteArray body = doc.toJson();

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("me/change-password/")));
    QNetworkRequest req(apiUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    if (accessToken.count('.') == 2) {
        req.setRawHeader("Authorization", QByteArray("Bearer " + accessToken.toUtf8()));
    } else {
        req.setRawHeader("Authorization", QByteArray("Token ") + accessToken.toUtf8());
    }

    QNetworkReply *r = m_manager->post(req, body);
    if(!r) return false;
    r->setProperty("requestType", "changePassword");
    reply = r;
    return true;
}

bool NetworkManager::fetchTodos() {
    if (!m_manager) return false;
    if (accessToken.isEmpty()) {
        qDebug() << "fetchTodos: no access token available";
        return false;
    }

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("todos/")));
    QNetworkRequest req(apiUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    if (accessToken.count('.') == 2) {
        req.setRawHeader("Authorization", QByteArray("Bearer ") + accessToken.toUtf8());
    } else {
        req.setRawHeader("Authorization", QByteArray("Token ") + accessToken.toUtf8());
    }

    QNetworkReply *r = m_manager->get(req);
    if (!r) return false;
    r->setProperty("requestType", "fetchTodos");
    reply = r;
    return true;
}

bool NetworkManager::createTodo(const QString &title, const QString &text, bool completed, const QString &due) {
    if (!m_manager) return false;
    if (accessToken.isEmpty()) {
        qDebug() << "createTodo: no access token available";
        return false;
    }

    QJsonObject payload;
    payload.insert("title", title);
    payload.insert("text", text);
    payload.insert("completed", completed);
    if (!due.isEmpty()) {
        payload.insert("due", due);
    }

    QJsonDocument doc(payload);
    QByteArray body = doc.toJson();

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("todos/")));
    QNetworkRequest req(apiUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    if (accessToken.count('.') == 2) {
        req.setRawHeader("Authorization", QByteArray("Bearer ") + accessToken.toUtf8());
    } else {
        req.setRawHeader("Authorization", QByteArray("Token ") + accessToken.toUtf8());
    }

    QNetworkReply *r = m_manager->post(req, body);
    if (!r) return false;
    r->setProperty("requestType", "createTodo");
    reply = r;
    return true;
}

bool NetworkManager::updateTodo(int todoId, const QString &title, const QString &text, bool completed, const QString &due) {
    if (!m_manager) return false;
    if (accessToken.isEmpty()) {
        qDebug() << "updateTodo: no access token available";
        return false;
    }

    QJsonObject payload;
    payload.insert("title", title);
    payload.insert("text", text);
    payload.insert("completed", completed);
    if (!due.isEmpty()) {
        payload.insert("due", due);
    }

    QJsonDocument doc(payload);
    QByteArray body = doc.toJson();

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("todos/%1/").arg(todoId)));
    QNetworkRequest req(apiUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));

    // add Authorization header
    if (accessToken.count('.') == 2) {
        req.setRawHeader("Authorization", QByteArray("Bearer ") + accessToken.toUtf8());
    } else {
        req.setRawHeader("Authorization", QByteArray("Token ") + accessToken.toUtf8());
    }

    QNetworkReply *r = m_manager->put(req, body);
    if (!r) return false;
    r->setProperty("requestType", "updateTodo");
    r->setProperty("todoId", todoId);
    reply = r;
    return true;
}

bool NetworkManager::deleteTodo(int todoId) {
    if (!m_manager) return false;
    if (accessToken.isEmpty()) {
        qDebug() << "deleteTodo: no access token available";
        return false;
    }

    QUrl apiUrl = m_url.resolved(QUrl(QStringLiteral("todos/%1/").arg(todoId)));
    QNetworkRequest req(apiUrl);

    // add Authorization header
    if (accessToken.count('.') == 2) {
        req.setRawHeader("Authorization", QByteArray("Bearer ") + accessToken.toUtf8());
    } else {
        req.setRawHeader("Authorization", QByteArray("Token ") + accessToken.toUtf8());
    }

    QNetworkReply *r = m_manager->deleteResource(req);
    if (!r) return false;
    r->setProperty("requestType", "deleteTodo");
    r->setProperty("todoId", todoId); // store the ID to remove from vector later
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

void NetworkManager::onRefreshTimeout() {
    if (!refreshToken.isEmpty()) {
        qDebug() << "Refresh timer fired, requesting new access token";
        setRefreshToken();
    } else {
        qDebug() << "No refresh token available; cannot refresh";
        emit refreshFailed(QStringLiteral("No refresh token available"));
    }
}

QString NetworkManager::getAccessToken() const {
    return accessToken;
}

void NetworkManager::setAccessToken(const QString &newAccessToken) {
    accessToken = newAccessToken;
}


void NetworkManager::onReplyFinished(QNetworkReply *reply) {
    if (!reply) return;

    QByteArray const response = reply->readAll();
    QString const requestType = reply->property("requestType").toString();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << "HTTP response for" << requestType << "- status:" << statusCode.toInt();
    qDebug() << "Body:" << response;
    
    if (requestType == QLatin1String("deleteTodo")) {
        int todoId = reply->property("todoId").toInt();
        
        // Check for successful delete (204 No Content)
        if (reply->error() == QNetworkReply::NoError) {
            // Remove the todo from the vector
            auto it = std::find_if(Todo::todos.begin(), Todo::todos.end(), 
                [todoId](const Todo* todo) { return todo->id() == todoId; });
            
            if (it != Todo::todos.end()) {
                delete *it; // delete the Todo object
                Todo::todos.erase(it); // remove from vector
                Todo::todo_count = Todo::todos.size();
                
                qDebug() << "=== NetworkManager: Deleted todo ===";
                qDebug() << "Deleted todo ID:" << todoId;
                qDebug() << "Total todos now:" << Todo::todo_count;
            }
            
            qDebug() << "NetworkManager: About to emit todoDeleted signal for ID:" << todoId;
            emit todoDeleted(todoId);
        } else {
            qDebug() << "NetworkManager: Delete failed with error:" << reply->errorString();
            emit todoDeleteFailed(reply->errorString());
        }
        reply->deleteLater();
        return;
    }
    
    if (reply->error() != QNetworkReply::NoError) {

        if (requestType == QLatin1String("refresh")) {
            emit refreshFailed(reply->errorString());
        } else if (requestType == QLatin1String("me")) {
            emit currentUserFetchedFailed(reply->errorString());
        } else if (requestType == QLatin1String("updateMe")) {
            emit currentUserUpdateFailed(reply->errorString());
        } else if (requestType == QLatin1String("login")) {
            emit loginFailed(reply->errorString());
        } else if (requestType == QLatin1String("fetchTodos")) {
            emit todosFetchFailed(reply->errorString());
        } else if (requestType == QLatin1String("createTodo")) {
            emit todoCreateFailed(reply->errorString());
        } else if (requestType == QLatin1String("updateTodo")) {
            emit todoUpdateFailed(reply->errorString());
        } else if (requestType == QLatin1String("deleteTodo")) {
            emit todoDeleteFailed(reply->errorString());
        } else if (requestType == QLatin1String("changePassword")) {
            emit passwordChangeFailed(reply->errorString());
        }
        reply->deleteLater();
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        if (requestType == QLatin1String("refresh")) {
            emit refreshFailed(QStringLiteral("Invalid JSON Response"));
        } else if (requestType == QLatin1String("fetchTodos")) {
            emit todosFetchFailed(QStringLiteral("Invalid JSON response"));
        } else {
            emit loginFailed(QStringLiteral("Invalid JSON response"));
        }
        reply->deleteLater();
        return;
    }
    
    if (requestType == QLatin1String("fetchTodos")) {
        if (doc.isArray()) {
            QJsonArray todosArray = doc.array();
            
            qDeleteAll(Todo::todos);
            Todo::todos.clear();
            
            for (const QJsonValue &value : todosArray) {
                if (value.isObject()) {
                    QJsonObject todoObj = value.toObject();
                    
                    Todo *newTodo = new Todo(nullptr);
                    if (todoObj.contains("id")) newTodo->setId(todoObj.value("id").toInt());
                    if (todoObj.contains("user_id")) newTodo->setUser_id(todoObj.value("user_id").toInt());
                    if (todoObj.contains("title")) newTodo->setTitle(todoObj.value("title").toString());
                    if (todoObj.contains("text")) newTodo->setText(todoObj.value("text").toString());
                    if (todoObj.contains("completed")) newTodo->setCompleted(todoObj.value("completed").toBool() ? 1 : 0);
                    if (todoObj.contains("created_on")) newTodo->setCreated_on(todoObj.value("created_on").toString());
                    if (todoObj.contains("updated_on")) newTodo->setUpdated_on(todoObj.value("updated_on").toString());
                    if (todoObj.contains("due") && !todoObj.value("due").isNull()) {
                        newTodo->setDue(todoObj.value("due").toString());
                    }
                    
                    Todo::todos.push_back(newTodo);
                }
            }
            
            Todo::todo_count = Todo::todos.size();
            qDebug() << "Fetched" << Todo::todo_count << "todos from server";
            for (const auto &todo : Todo::todos) {
                qDebug() << "Server todo: " << todo->id() << todo->title() << todo->text();
            }
            emit todosFetched(todosArray);
        } else {
            emit todosFetchFailed(QStringLiteral("Expected JSON array but got object"));
        }
        reply->deleteLater();
        return;
    }

    if (requestType == QLatin1String("createTodo")) {
        if (doc.isObject()) {
            QJsonObject todoObj = doc.object();
            
            // Create new Todo and add to vector
            Todo *newTodo = new Todo(nullptr);
            if (todoObj.contains("id")) newTodo->setId(todoObj.value("id").toInt());
            if (todoObj.contains("user_id")) newTodo->setUser_id(todoObj.value("user_id").toInt());
            if (todoObj.contains("title")) newTodo->setTitle(todoObj.value("title").toString());
            if (todoObj.contains("text")) newTodo->setText(todoObj.value("text").toString());
            if (todoObj.contains("completed")) newTodo->setCompleted(todoObj.value("completed").toBool() ? 1 : 0);
            if (todoObj.contains("created_on")) newTodo->setCreated_on(todoObj.value("created_on").toString());
            if (todoObj.contains("updated_on")) newTodo->setUpdated_on(todoObj.value("updated_on").toString());
            if (todoObj.contains("due") && !todoObj.value("due").isNull()) {
                newTodo->setDue(todoObj.value("due").toString());
            }
            
            Todo::todos.push_back(newTodo);
            Todo::todo_count = Todo::todos.size();
            
            qDebug() << "=== NetworkManager: Created todo ===";
            qDebug() << "New todo:" << newTodo->id() << newTodo->title() << newTodo->text();
            qDebug() << "Total todos now:" << Todo::todo_count;
            
            emit todoCreated(todoObj);
        } else {
            emit todoCreateFailed(QStringLiteral("Expected JSON object but got array"));
        }
        reply->deleteLater();
        return;
    }

    if (requestType == QLatin1String("updateTodo")) {
        if (doc.isObject()) {
            QJsonObject todoObj = doc.object();
            int todoId = reply->property("todoId").toInt();
            
            for (auto &existingTodo : Todo::todos) {
                if (existingTodo->id() == todoId) {
                    if (todoObj.contains("title")) existingTodo->setTitle(todoObj.value("title").toString());
                    if (todoObj.contains("text")) existingTodo->setText(todoObj.value("text").toString());
                    if (todoObj.contains("completed")) existingTodo->setCompleted(todoObj.value("completed").toBool() ? 1 : 0);
                    if (todoObj.contains("updated_on")) existingTodo->setUpdated_on(todoObj.value("updated_on").toString());
                    if (todoObj.contains("due") && !todoObj.value("due").isNull()) {
                        existingTodo->setDue(todoObj.value("due").toString());
                    }
                    
                    qDebug() << "=== NetworkManager: Updated todo ===";
                    qDebug() << "Updated todo:" << existingTodo->id() << existingTodo->title() << existingTodo->text();
                    break;
                }
            }
            
            emit todoUpdated(todoObj);
        } else {
            emit todoUpdateFailed(QStringLiteral("Expected JSON object but got array"));
        }
        reply->deleteLater();
        return;
    }

    if (requestType == QLatin1String("changePassword")) {
        if (doc.isObject()) {
            QJsonObject responseObj = doc.object();

            if (responseObj.contains("message")) {
                QString message = responseObj.value("message").toString();
                qDebug() << "=== NetworkManager: Password changed successfully ===";
                qDebug() << "Server message:" << message;
                emit passwordChanged(message);
            } else {
                emit passwordChangeFailed(QStringLiteral("Unexpected success response format"));
            }
        } else {
            emit passwordChangeFailed(QStringLiteral("Expected JSON object but got array"));
        }
        reply->deleteLater();
        return;
    }

    
    if (!doc.isObject()) {
        if (requestType == QLatin1String("refresh")) {
            emit refreshFailed(QStringLiteral("Expected JSON object"));
        } else {
            emit loginFailed(QStringLiteral("Expected JSON object"));
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

            if (m_refreshTimer) {
                const int testIntervalMs = (60 * 5) * 1000;
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
            accessToken = token;
            refreshToken.clear();
            emit loginSucceeded(accessToken, refreshToken, userId, username);
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
            if (m_refreshTimer) {
                const int testIntervalMs = (60 * 5) * 1000;
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

