#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <QDebug>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Client : public QObject {
Q_OBJECT;
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

    void doInitialSync();
    void doPresenceList();

private slots:
    void onGetLogin();
    void onPostLogin();
    void onInitialSync();
    void onPresenceList();
    void onSubscribe();

public:
    Client(const char *, int, QObject *parent = 0);

    void login(const char *, const char *);
    void subscribe(const char *);
};

#endif
