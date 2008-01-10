#ifndef IMAGEVIEW_INCLUDED
#define IMAGEVIEW_INCLUDED

#include <qwidget.h>
#include <qpixmap.h>


class ImageView:public QWidget{
Q_OBJECT
public:
ImageView(QWidget* parent, char * name);
  void LoadFR(const char* filename, int w, int h);
  void LoadImage(const char* filename);
  void clearImage();

  void paintEvent(QPaintEvent*);
  

  virtual void mousePressEvent ( QMouseEvent * me);


private:
  QPixmap image;
  QString fname;
  float leyex, leyey, reyex, reyey;
  static float scaleFactor;


};



#endif
