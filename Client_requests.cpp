#include "Client.hpp"

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

QNetworkRequest Client::request_presenceList()
{
    QNetworkRequest req;
    QUrl url(endpoint + "presence/list/" + ownUserid + "?access_token="
             + access_token);
    req.setUrl(url);
    return req;
}

QNetworkRequest Client::request_subscribe()
{
    QNetworkRequest req;
    QUrl url(endpoint + "presence/list/" + ownUserid + "?access_token="
             + access_token);
    req.setUrl(url);
    return req;
}

QNetworkRequest Client::request_events(QString *since)
{
    QNetworkRequest req;
    QString path = endpoint + "events?access_token=" + access_token;
    if (since != nullptr) {
        path += "&from=" + *since;
    }
    QUrl url(path);
    req.setUrl(url);
    return req;
}
