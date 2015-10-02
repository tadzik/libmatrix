#ifndef _PRESENCE_TRACKER_H_
#define _PRESENCE_TRACKER_H_

#include <QMap>
#include <QJsonObject>
#include <QString>
#include <QDateTime>
#include <QObject>

struct PresenceDetails {
    QString type; // should really be some enum
    QString status;
    QDateTime lastActivity;
    bool freund;
};

class PresenceTracker : public QObject {

Q_OBJECT;
    QMap<QString,PresenceDetails*> storage;

public:
    PresenceTracker(QObject *parent = 0);
    void onPresenceEvent(const QJsonObject &o, bool freund = false);
    void display();

public slots:
    void onPresenceList();
};

#endif
