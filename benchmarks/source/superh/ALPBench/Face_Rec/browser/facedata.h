#ifndef FACEDATA_INCLUDED
#define FACEDATA_INCLUDED

#include <map>
#include <qstring.h>

#include <qwidget.h>


class PersonData{
public:
  //QString person;
  std::map< QString, QString > covariates; // map of covariate to value

};

class ImageData{
public:
  //  QString image;
  QString person;
  std::map< QString, QString > covariates; // map of covariate to value

};


class FaceData:public QObject{
Q_OBJECT


public:
std::map< QString , PersonData > persondata; //map from person label to person data
std::map< QString , ImageData > imagedata; //map form image label to image data

  void AddOtherFields();
public slots:

  void LoadData();
  void SaveData();
  void ClearData();

  signals:
  void saveForm(void);
  void loadForm(void);
};

extern FaceData facedata; 

//extern std::map< QString , PersonData > persondata; //map from person label to person data
//extern std::map< QString , ImageData > imagedata; //map form image label to image data


#endif
