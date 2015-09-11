#ifndef PCLVIEWER_H
#define PCLVIEWER_H
//#include <vtkAutoInit.h>
//#include <vtkRenderingOpenGL.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL);
#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#include <iostream>

// Qt
#include <QMainWindow>

// Point Cloud Library
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

// Visualization Toolkit (VTK)
#include <vtkRenderWindow.h>

#include "connector.h"

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

#define CLOUD_SIZE 100000
namespace Ui
{
  class PCLViewer;
}

class Viewer : public QMainWindow
{
  Q_OBJECT
//private:Connector connector;

public:
  explicit Viewer (QWidget *parent = 0);
  ~Viewer ();

public slots:
  void
  randomButtonPressed ();

  void
  RGBsliderReleased ();

  void
  pSliderValueChanged (int value);

  void dataReceiver(QString data);


protected:
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
  PointCloudT::Ptr cloud;

  unsigned int red;
  unsigned int green;
  unsigned int blue;

private slots:
  void on_connect_clicked();

  void on_send_clicked();

  void on_random_clicked();

  void on_reset_clicked();

  void on_clear_clicked();

private:
  int i;
  Ui::PCLViewer *ui;
  Connector *connector;
  QThread *thread;
  bool randPoints;
  bool paused;
  int numberOfPoints;

};

#endif // PCLVIEWER_H
