/****************************************************************************
** Form interface generated from reading ui file 'MainWindow.ui'
**
** Created: Sun Dec 8 14:54:28 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class GraphView;
class QListView;
class QListViewItem;
class QPushButton;

class MainWindow : public QDialog
{ 
    Q_OBJECT

public:
    MainWindow( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~MainWindow();

    GraphView* graphview;
    QListView* pointList;
    QPushButton* loadButton;
    QPushButton* saveButton;
    QPushButton* resetButton;
    QListView* imageList;


public slots:
    virtual void loadImage( QListViewItem * );
    virtual void updatePoint( int, double, double );

};

#endif // MAINWINDOW_H
