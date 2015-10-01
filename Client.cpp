#include "Client.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Client::Client(const char *host, int port, QObject *parent)
    : QObject(parent)
{
    nam = new QNetworkAccessManager(this);
    endpoint = "http://" + QString(host) + ":" + QString::number(port)
               + "/_matrix/client/api/v1/";
    //qDebug() << "Endpoint is" << endpoint;
}

void Client::onGetLogin()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    Q_ASSERT(m_replies.contains(reply));

    QByteArray resp = reply->readAll();
    //qDebug() << "Response has arrived" << resp;

    QJsonDocument doc(QJsonDocument::fromJson(resp));
    QJsonObject obj = doc.object();
    QJsonArray flows(obj["flows"].toArray());
    QJsonArray::const_iterator it;
    for (it = flows.begin(); it != flows.end(); it++) {
        QJsonObject flow = it->toObject();
        if (flow["type"].toString() == "m.login.password") {
            //qDebug() << "Got a supported flow";
            break;
        }
    }
    if (it == flows.end()) {
        qDebug() << "No supported login flow detected :(";
    }

    QJsonObject args {
        { "type", "m.login.password" },
        { "user", username },
        { "password", password },
    };
    QByteArray payload =
        QJsonDocument(args).toJson(QJsonDocument::Compact);
    QNetworkReply *rep = 0;
    rep = nam->post(request_login(), payload);

    connect(rep, SIGNAL(finished()), this, SLOT(onPostLogin()));

    reply->deleteLater();
}

void Client::onPostLogin()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    Q_ASSERT(m_replies.contains(reply));

    QByteArray resp = reply->readAll();
    //qDebug() << "Response has arrived" << resp;

    QJsonDocument doc(QJsonDocument::fromJson(resp));
    QJsonObject obj = doc.object();
    access_token = obj["access_token"].toString();

    QNetworkReply *rep = 0;
    rep = nam->get(request_initialSync());
    connect(rep, SIGNAL(finished()), this, SLOT(onInitialSync()));

    reply->deleteLater();
}

void Client::onInitialSync()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    Q_ASSERT(m_replies.contains(reply));

    QByteArray resp = reply->readAll();
    //qDebug() << "Response has arrived" << resp;

    QJsonDocument doc(QJsonDocument::fromJson(resp));
    QJsonObject obj = doc.object();
    QJsonArray presence = obj["presence"].toArray();
    QJsonArray::const_iterator it;
    for (it = presence.begin(); it != presence.end(); it++) {
        QJsonObject buddy = it->toObject();
        qDebug() << buddy;
    }
}

void Client::login(const char *user, const char *pass)
{
    username = user;
    password = pass;

    QNetworkReply *rep = 0;
    rep = nam->get(request_login());

    connect(rep, SIGNAL(finished()), this, SLOT(onGetLogin()));
}

QNetworkRequest Client::request_login()
{
    QNetworkRequest req;
    QString url = endpoint + "login";
    req.setUrl(QUrl(url));
    return req;
}

QNetworkRequest Client::request_initialSync()
{
    QNetworkRequest req;
    QUrl url(endpoint + "initialSync?limit=1&access_token="
             + access_token);
    req.setUrl(url);
    return req;
}
