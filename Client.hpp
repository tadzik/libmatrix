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

    QNetworkRequest request_login();
    QNetworkRequest request_initialSync();

private slots:
    void onGetLogin();
    void onPostLogin();
    void onInitialSync();

public:
    Client(const char *, int, QObject *parent = 0);

    void login(const char *, const char *);
};

#endif
