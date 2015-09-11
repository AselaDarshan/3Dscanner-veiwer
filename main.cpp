#include "viewer.h"
#include "connector.h"
#include <QApplication>
#include <QMainWindow>

int main (int argc, char *argv[])
{

  QApplication a (argc, argv);
  Viewer w;
  w.show ();
  //a.exec ();



 // connector=Connector();


  return a.exec ();
}
