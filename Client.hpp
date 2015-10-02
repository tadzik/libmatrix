#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <QDebug>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>

#include "PresenceTracker.hpp"

class Client : public QObject {
Q_OBJECT;
    PresenceTracker *presenceTracker;

    QNetworkAccessManager *nam;
    QString endpoint;
    QString username;
    QString password;
    QString access_token;
    QString synapseHostname;
    QString ownUserid;

    QNetworkRequest request_login();
    QNetworkRequest request_subscribe();
    QNetworkRequest request_initialSync();
    QNetworkRequest request_presenceList();
    QNetworkRequest request_events(QString *since = nullptr);

    void doInitialSync();
    void doPresenceList();

    void waitForEvents(QString *since = nullptr);

private slots:
    void onGetLogin();
    void onPostLogin();
    void onInitialSync();
    void onSubscribe();
    void onEvents();

public:
    Client(const char *, int, QObject *parent = 0);

    void login(const char *, const char *);
    void subscribe(const char *);
};

#endif
