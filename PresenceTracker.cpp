#include "PresenceTracker.hpp"
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

PresenceTracker::PresenceTracker(QObject *parent) : QObject(parent)
{
}

void PresenceTracker::display()
{
    qDebug() << "Presence list:";
    QMap<QString,PresenceDetails*>::const_iterator it;
    for (it = storage.constBegin(); it != storage.constEnd(); it++) {
        if (it.value()->freund) {
            QString line;
            line += it.key();
            line += " is ";
            line += it.value()->type;
            qDebug() << line;
        }
    }
    qDebug() << "==============";
}

void PresenceTracker::onPresenceList()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QByteArray resp = reply->readAll();
    QJsonDocument doc(QJsonDocument::fromJson(resp));
    QJsonArray arr = doc.array();
    QJsonArray::const_iterator it;
    for (it = arr.begin(); it != arr.end(); it++) {
        QJsonObject buddy = it->toObject();
        onPresenceEvent(buddy, true);
    }
    display();
}

void PresenceTracker::onPresenceEvent(const QJsonObject& p, bool freund)
{
    QString uid = p["user_id"].toString();
    if (!storage.contains(uid)) {
        storage[uid] = new PresenceDetails();
        storage[uid]->freund = freund;
    }
    storage[uid]->type = p["presence"].toString();
}
