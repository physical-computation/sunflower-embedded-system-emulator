/*===========================================================================
  Short demonstration program to show how to begin structuring the first
  programming lab assignment for CS410, Spring 2001.   

 Last update,  January 30, 2001   Ross Beveridge
 ===========================================================================*/

/* ------------------------ INCLUDE FILES --------------------------------- */

#include "mainview.h"
#include <qapplication.h>
#include <iostream.h>
#include <fstream.h>

/* ---------------------------- MAIN -------------------------------------- */

// DSB
ofstream eyecoords("EyeCoords.txt");

int main( int argc, char **argv )
{
  // Create the application and the interface object. The interface
  // object handles all of the work and is a subclass of QWidget. 
  QApplication app(argc, argv);
  //QMainWindow* mainwin = new QMainWindow(0,"Face Browser");
  MainView* mainview= new MainView(0,"FaceBrowser");
  //mainwin->setCentralWidget(mainwindow);
  app.setMainWidget( mainview );
  mainview->resize(1000,700);
  mainview->show();

  return app.exec();
}





























































