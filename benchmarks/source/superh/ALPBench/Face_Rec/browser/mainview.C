#include "mainview.h"

#include "filelistview.h"
#include "imageview.h"
#include "formview.h"
#include <qpushbutton.h>
#include "facedata.h"

#include <iostream.h>


inline int max(int x, int y){ return x > y ? x : y;} 

MainView::MainView(QWidget* parent, const char* name)
  :QWidget(parent, name){
  sourceview = new ImageView(this,"Source Image");
  //sourceview->LoadImage("00749fa010_941201.pgm");
  normview = new ImageView(this,"Nomalized Image");
  //normview->LoadFR("00749fa010_941201.nrm",130,150);
  filelistview = new FileListView(this,sourceview,normview);
  formview = new FormView(this);
  filelistview->form = formview;
  loadbutton = new QPushButton("Load Data",this);
  //  QObject::connect(loadbutton,SIGNAL(clicked(void)), &facedata, SLOT(LoadData(void)));
  QObject::connect(loadbutton,SIGNAL(clicked(void)), this, SLOT(freeForLoad(void)));
  QObject::connect(this,SIGNAL(loadData(void)), &facedata, SLOT(LoadData(void)));
  QObject::connect(&facedata,SIGNAL(loadForm(void)), filelistview, SLOT(loadForm(void)));
  savebutton = new QPushButton("Save Data",this);
  QObject::connect(savebutton,SIGNAL(clicked(void)), this, SLOT(freeForSave(void)));
  QObject::connect(this,SIGNAL(saveData(void)), &facedata, SLOT(SaveData(void)));
  QObject::connect(&facedata,SIGNAL(saveForm(void)), filelistview, SLOT(saveForm(void)));
  clearbutton = new QPushButton("Clear Data", this);
  QObject::connect(clearbutton,SIGNAL(clicked(void)), &facedata, SLOT(ClearData(void)));

  QObject::connect(this,SIGNAL(changeData(QString, QString)), filelistview, SLOT(changeData(QString, QString)));
  QObject::connect(this,SIGNAL(nextFace()), filelistview, SLOT(nextItem()));
  QObject::connect(this,SIGNAL(prevFace()), filelistview, SLOT(prevItem()));
  
  grabKeyboard();
  resize(width(),height());
}

void MainView::resizeEvent(QResizeEvent*){
  if(sourceview&&normview&&filelistview&&formview){
  if(sourceview) sourceview->move(0,0);
  if(normview) normview->move(sourceview->width(),0);
  if(filelistview) filelistview->move(0,sourceview->height());
  if(filelistview) filelistview->resize(sourceview->width() + normview->width(),height()-sourceview->height());
  if(formview) formview->move(sourceview->width()+normview->width(),0);
  if(formview) formview->resize(width()-(sourceview->width()+normview->width()),height() - 30);
  if(loadbutton) loadbutton->move(sourceview->width()+normview->width(),height()-30);
  if(loadbutton) loadbutton->resize((width() - (sourceview->width()+normview->width()))/3,30);
  if(savebutton) savebutton->move(loadbutton->width()+sourceview->width()+normview->width(),height()-30);
  if(savebutton) savebutton->resize((width() - (sourceview->width()+normview->width()))/3,30);
  if(clearbutton) clearbutton->move(2*loadbutton->width()+sourceview->width()+normview->width(),height()-30);
  if(clearbutton) clearbutton->resize((width() - (sourceview->width()+normview->width()))/3,30);
  }
}
  
void MainView::keyPressEvent ( QKeyEvent * event){
  //  cout << event->key() << endl;
  int k = event->key();
  cout << "key: " << k << endl;
  switch(k){
  case 4117:
  case 4100:
    emit nextFace();
    break;
  case 4115:
  case 4099:
    emit prevFace();
    break;

  case 49: 
    //    cout << "Q" << endl;
    emit changeData("GENDER","MALE");
    break;
  case 50: 
    emit changeData("GENDER","FEMALE");
    break;
  case 81: 
    emit changeData("RACE","WHITE");
    break;
  case 87: 
    emit changeData("RACE","BLACK");
    break;
  case 69: 
    emit changeData("RACE","ASIAN");
    break;
  case 82: 
    emit changeData("RACE","OTHER");
    break;
  case 65: 
    emit changeData("AGE","TEEN");
    break;
  case 83: 
    emit changeData("AGE","20");
    break;
  case 68: 
    emit changeData("AGE","30");
    break;
  case 70: 
    emit changeData("AGE","40");
    break;
  case 71: 
    emit changeData("AGE","50");
    break;
  case 72: 
    emit changeData("AGE","60+");
    break;
  case 90: 
    emit changeData("SKIN","CLEAR");
    break;
  case 88: 
    emit changeData("SKIN","FRECKELED");
    break;
  case 67: 
    emit changeData("SKIN","WRINKLED");
    break;
  case 86: 
    emit changeData("SKIN","BOTH");
    break;
  case 66: 
    emit changeData("SKIN","OTHER");
    break;
  case 54: 
    emit changeData("GLASSES","YES");
    break;
  case 55: 
    emit changeData("GLASSES","NO");
    break;
  case 56: 
    emit changeData("FACIAL_HAIR","YES");
    break;
  case 57: 
    emit changeData("FACIAL_HAIR","NO");
    break;
  case 85: 
    emit changeData("EXPRESSION","NEUTRAL");
    break;
  case 73: 
    emit changeData("EXPRESSION","OTHER");
    break;
  case 75: 
    emit changeData("MOUTH","CLOSED");
    break;
  case 76: 
    emit changeData("MOUTH","OPEN");
    break;
  case 59: 
    emit changeData("MOUTH","TEETH");
    break;
  case 39: 
    emit changeData("MOUTH","OTHER");
    break;
  case 44: 
    emit changeData("EYES","CLOSED");
    break;
  case 46: 
    emit changeData("EYES","OPEN");
    break;
  case 47: 
    emit changeData("EYES","OTHER");
    break;
  case 79: 
    emit changeData("BANGS","YES");
    break;
  case 80: 
    emit changeData("BANGS","NO");
    break;
  case 48: 
    emit changeData("MAKEUP","YES");
    break;
  case 45: 
    emit changeData("MAKEUP","NO");
    break;
  };
}

void MainView::freeForSave(){
  releaseKeyboard();
  emit saveData();
  grabKeyboard();

}

void MainView::freeForLoad(){
  releaseKeyboard();
  emit loadData();
  grabKeyboard();

}
