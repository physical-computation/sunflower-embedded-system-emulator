#ifndef FILELISTVIEW_INCLUDED
#define FILELISTVIEW_INCLUDED

#include <qwidget.h>
#include <qlistview.h>

#include <map>

struct FileInfo{
  FileInfo(){
    sourceName = "-";
    normName = "-";
    basename = "-";
    person = "-";
    type = "-";
    camera = "-";
    date = "-";
    
  }
    
  QString sourceName;
  QString normName;
  QString basename;
  QString person;
  QString type;
  QString camera;
  QString date;
  
  
};

class ImageView;
class FormView;

class FileListView: public QListView{
Q_OBJECT
public:
FileListView(QWidget*,ImageView*, ImageView*);

 public slots:
 void switchImages(QListViewItem*);
  void saveForm();
  void loadForm();
  void changeData(QString field, QString value);

  void nextItem();
  void prevItem();

  FormView* form;

private:
  std::map<QString,FileInfo> files;
  QString sourcepath;
  QString normpath;

  ImageView* sourceview;
  ImageView* normview;
  QListViewItem* currentimage;
  


};



#endif
