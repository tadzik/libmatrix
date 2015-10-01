#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QApplication>
#include "Client.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Client cl("matrix.org", 80);
    cl.login("tadzik", "jajajajaaj");

    app.exec();
    return 0;
}
