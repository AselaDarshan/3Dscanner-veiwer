#include "connector.h"
#include <QFuture>
#include <QThread>
#include <qtconcurrentrun.h>
using namespace QtConcurrent;
using namespace std;

Connector::Connector()
{
    cout<<"\nConnector created"<<endl;
    connectionHelper = new ConnectionHelper();

}

int Connector::connectToScanner(){

    // QFuture<void> f = run(initialize);
    // f.waitForFinished();
    // this->start();

    //CameraManagement * camMgr = new CameraManagment;
    //  QThread * thread = new QThread;
    //    this->moveToThread( thread );
    //    thread->start();
    return connectionHelper->initialize();

    // return true;
}
 void Connector::disconnectScanner(){
    connectionHelper->disconnect();
 }

bool Connector::sendData(string message){
    connectionHelper->sendData(message);
}


void Connector ::run(){
//    cout<<"recv thread started"<<endl;
//    QString data;

//    data=QString::fromStdString(connectionHelper->recieve());

//   while(1)
//   {
//        if(!data.isNull())
//            emit dataReceived(data);
//        data=QString::fromStdString(connectionHelper->recieve());
//        cout<<"get data"<<endl;
//    }
}

void Connector::startReceiving(){

    cout<<"recv thread started"<<endl;
    QString data;
    string message;

   // data=QString::fromStdString();

   while(1)
   {
       message= connectionHelper->recieve();
        if(message.compare("faild")){
            data=QString::fromStdString(message);
            emit dataReceived(data);

        }
        QThread::msleep(1);
        //cout<<"get data"<<endl;
    }
}
