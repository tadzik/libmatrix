#include "Client.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

void Client::onGetLogin()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    //Q_ASSERT(m_replies.contains(reply));

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

    ownUserid = "@" + username + ":" + synapseHostname;

    reply->deleteLater();
}

void Client::onPostLogin()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    //Q_ASSERT(m_replies.contains(reply));

    QByteArray resp = reply->readAll();
    //qDebug() << "Response has arrived" << resp;

    QJsonDocument doc(QJsonDocument::fromJson(resp));
    QJsonObject obj = doc.object();
    access_token = obj["access_token"].toString();

    qDebug() << access_token;

    doPresenceList();
    //qDebug() << "Subscribing to shit";
    //subscribe("@krzysiek:matrix.org");
    //subscribe("@sergot:matrix.org");
    //doInitialSync();
    waitForEvents();

    reply->deleteLater();
}

void Client::onInitialSync()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    //Q_ASSERT(m_replies.contains(reply));

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

    reply->deleteLater();
}

void Client::onSubscribe()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QByteArray resp = reply->readAll();
    qDebug() << resp;
    reply->deleteLater();
}

void Client::onEvents()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QByteArray resp = reply->readAll();
    QJsonDocument doc(QJsonDocument::fromJson(resp));
    QJsonObject obj = doc.object();
    QJsonArray chunk = obj["chunk"].toArray();

    QJsonArray::const_iterator it;
    for (it = chunk.begin(); it != chunk.end(); it++) {
        QJsonObject ev = it->toObject();
        if (ev["type"].toString() == "m.presence") {
            presenceTracker->onPresenceEvent(ev["content"].toObject());
        } else if (ev["type"].toString() == "m.room.message") {
            qDebug() << "Message in room" << ev["room_id"].toString()
                     << "sent by" << ev["user_id"].toString()
                     << ": "
                     << ev["content"].toObject()["body"].toString();
        }
    }
    presenceTracker->display();

    QString end = obj["end"].toString();
    waitForEvents(&end);
    reply->deleteLater();
}
