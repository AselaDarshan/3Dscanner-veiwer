#ifndef CONNECTOR_H
#define CONNECTOR_H
#include <iostream>

#include <QThread>
#include <QObject>
#include"connectionhelper.h"



class Connector:public QThread

{
      Q_OBJECT
private:
    ConnectionHelper* connectionHelper;
    void run();
public:
    Connector();
    int connectToScanner();
    bool sendData(std::string message);

    void disconnectScanner();
signals:
    void dataReceived(QString data);
public slots:
    void startReceiving();


};

#endif // CONNECTOR_H
