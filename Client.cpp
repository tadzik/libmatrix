#include "Client.hpp"

Client::Client(const char *host, int port, QObject *parent)
    : QObject(parent)
{
    nam = new QNetworkAccessManager(this);
    synapseHostname = host;
    endpoint = "http://" + synapseHostname + ":" + QString::number(port)
               + "/_matrix/client/api/v1/";
    //qDebug() << "Endpoint is" << endpoint;
}

void Client::login(const char *user, const char *pass)
{
    username = user;
    password = pass;

    QNetworkReply *rep = 0;
    rep = nam->get(request_login());

    connect(rep, SIGNAL(finished()), this, SLOT(onGetLogin()));
}

void Client::subscribe(const char *userid)
{
    QNetworkReply *rep = 0;
    QString payload("{\"invite\":[\"");
    payload += userid;
    payload += +"\"]}";
    rep = nam->post(request_subscribe(), payload.toUtf8());
    connect(rep, SIGNAL(finished()), this, SLOT(onSubscribe()));
}

void Client::doInitialSync()
{
    QNetworkReply *rep = 0;
    rep = nam->get(request_initialSync());
    connect(rep, SIGNAL(finished()), this, SLOT(onInitialSync()));
}

void Client::doPresenceList()
{
    QNetworkReply *rep = 0;
    rep = nam->get(request_presenceList());
    connect(rep, SIGNAL(finished()), this, SLOT(onPresenceList()));
}

