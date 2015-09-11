#include "viewer.h"
#include "../build/ui_viewer.h"
#include "string.h"

Viewer::Viewer (QWidget *parent) :
    QMainWindow (parent),
    ui (new Ui::PCLViewer)
{
    paused = true;
    thread = new QThread();

    ui->setupUi (this);
    ui->send->setEnabled(false);
    ui->reset->setEnabled(false);
    ui->random->setVisible(false);
    this->setWindowTitle ("Visualizer");

    // Setup the cloud pointer
    cloud.reset (new PointCloudT);
    // The number of points in the cloud
    cloud->points.resize (CLOUD_SIZE);

    // The default color
    red   = 128;
    green = 128;
    blue  = 128;

    // Fill the cloud with some points
    //    for (size_t i = 0; i < cloud->points.size (); ++i)
    //    {
    //        cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
    //        cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
    //        cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);

    //        cloud->points[i].r = red;
    //        cloud->points[i].g = green;
    //        cloud->points[i].b = blue;
    //    }

    // Set up the QVTK window
    viewer.reset (new pcl::visualization::PCLVisualizer ("viewer", false));
    ui->qvtkWidget->SetRenderWindow (viewer->getRenderWindow ());
    viewer->setupInteractor (ui->qvtkWidget->GetInteractor (), ui->qvtkWidget->GetRenderWindow ());
    ui->qvtkWidget->update ();


    // Connect point size slider
    connect (ui->horizontalSlider_p, SIGNAL (valueChanged (int)), this, SLOT (pSliderValueChanged (int)));

    viewer->addPointCloud (cloud, "cloud");
    pSliderValueChanged (3);
    viewer->resetCamera ();
    ui->qvtkWidget->update ();


    randPoints=true;
    numberOfPoints = 0;
    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, QColor(0, 0, 0));
    ui->textEdit->setPalette(p);
    ui->textEdit->setTextColor(QColor(0, 150,200));

}



void
Viewer::RGBsliderReleased ()
{
    // Set the new color
    for (size_t i = 0; i < cloud->size (); i++)
    {
        cloud->points[i].r = red;
        cloud->points[i].g = green;
        cloud->points[i].b = blue;
    }
    viewer->updatePointCloud (cloud, "cloud");
    ui->qvtkWidget->update ();
}

void
Viewer::pSliderValueChanged (int value)
{
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, value, "cloud");
    ui->qvtkWidget->update ();
}


Viewer::~Viewer ()
{
    //delete ui;
}
bool connected=false;
void Viewer::on_connect_clicked()
{
    ui->send->setText("Start");
    if(!connected){
        connector = new Connector();

        //connect receiver slot
        connect (connector, SIGNAL (dataReceived(QString )), this, SLOT (dataReceiver(QString )));
        QString text;
        i=connector->connectToScanner();
        if(i==-1){
            text = "Cloud not created socket!";
        }
        else if(i==-2){
            text = "Unable to connect";
        }
        else{
            text ="Scanner connected. Socket: ";
            QString s = QString::number(i);
            text+=s;
            connector->moveToThread( thread );

            connect(thread, SIGNAL(started()), connector, SLOT(startReceiving()));

            cout<<"strating recv thread"<<endl;
            thread->start();
            ui->connect->setText("Disconnect");
            connected=true;

            ui->send->setEnabled(true);
            ui->reset->setEnabled(true);
        }
        ui->textEdit->append(text.toStdString().c_str());

    }
    else{
        connector->disconnectScanner();
        ui->textEdit->append("Scanner disconnected");
        ui->connect->setText("Connect");
        thread->terminate();
        connected=false;

        ui->send->setEnabled(false);
        ui->reset->setEnabled(false);
    }
}
bool started=false;
void Viewer::on_send_clicked()
{
    if(!started){
        started = true;
        connector->sendData("start");
    }
    if(paused){
        connector->sendData("next");//ui->textEdit->toPlainText().toStdString());
        paused=false;
        ui->send->setText("Pause");
    }
    else{
        paused=true;
        ui->send->setText("Resume");
    }
}

void Viewer::dataReceiver(QString data){
    ui->textEdit->append("From scanner: "+data);
    QString x=data.split(",").at(0);
    QString y=data.split(",").at(1);
    QString z=data.split(",").at(2);
    QString r=data.split(",").at(3);
    QString g=data.split(",").at(4);
    QString b=data.split(",").at(5);

    cloud->points[numberOfPoints].x=x.toInt()/1000.0;
    cloud->points[numberOfPoints].y=y.toInt()/1000.0;
    cloud->points[numberOfPoints].z=z.toInt()/1000.0;
    cloud->points[numberOfPoints].r = r.toInt();
    cloud->points[numberOfPoints].g = g.toInt();
    cloud->points[numberOfPoints++].b = b.toInt();
    if(numberOfPoints%400==0){
        viewer->updatePointCloud (cloud, "cloud");
        ui->qvtkWidget->update ();
    }
    if(!paused){
        connector->sendData("next");
    }
    ui->pointCount->display(numberOfPoints);
}

void Viewer::on_random_clicked()
{
    if(randPoints){
        // Setup the cloud pointer
        cloud.reset (new PointCloudT);
        // The number of points in the cloud
        cloud->points.resize (CLOUD_SIZE);
        for (size_t i = 0; i < cloud->points.size (); ++i)
        {
            cloud->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
            cloud->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
            cloud->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
            //cout<<cloud->points[i].x<<endl;
            cloud->points[i].r = red;
            cloud->points[i].g = green;
            cloud->points[i].b = blue;
        }
        for (size_t i = 0; i < cloud->size(); i++)
        {
            cloud->points[i].r = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
            cloud->points[i].g = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
            cloud->points[i].b = 255 *(1024 * rand () / (RAND_MAX + 1.0f));
        }

        viewer->updatePointCloud (cloud, "cloud");
        ui->qvtkWidget->update ();
        randPoints=false;
        ui->random->setText("Points from Scanner");
    }
    else{
        // Setup the cloud pointer
        cloud.reset (new PointCloudT);
        // The number of points in the cloud
        cloud->points.resize (CLOUD_SIZE);
        viewer->updatePointCloud (cloud, "cloud");
        ui->qvtkWidget->update ();
        randPoints=true;
        ui->random->setText("Random Points");
        numberOfPoints=0;
    }
}

void Viewer::on_reset_clicked()
{
    connector->sendData("rset");
}

void
Viewer::randomButtonPressed ()
{
    printf ("Random button was pressed\n");


}

void Viewer::on_clear_clicked()
{
    // Setup the cloud pointer
    cloud.reset (new PointCloudT);
    // The number of points in the cloud
    cloud->points.resize (CLOUD_SIZE);
    viewer->updatePointCloud (cloud, "cloud");
    ui->qvtkWidget->update ();
    numberOfPoints=0;
}
