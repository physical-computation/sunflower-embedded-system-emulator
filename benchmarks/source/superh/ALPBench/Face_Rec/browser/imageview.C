#include "imageview.h"

#include <qpainter.h>
#include <qimage.h>
#include <iostream.h>

//DSB
#include <fstream.h>
extern ofstream eyecoords;

extern "C" float* readferetraster(const char* filename, int numpix);

ImageView::ImageView(QWidget* parent, char * name):
    QWidget(parent,name),
    image(130,150),
    leyex(-1.0), leyey(-1.0), reyex(-1.0), reyey(-1.0){
  resize(130,150);
  image.fill(QColor(255,255,255));

}


void ImageView::LoadFR(const char* filename, int w, int h){
    fname = filename;
  image.resize(w,h);
  image.fill(QColor(255,255,255));
  QPainter paint(&image);
  
  float* im = readferetraster(filename,w*h);
  float min = im[0];
  float max = im[0];
  for(int c = 0; c < w*h; c++){
    if(min > im[c]) min = im[c];
    if(max < im[c]) max = im[c];
  }
 for(int i = 0; i < image.width(); i++)
  for(int j = 0; j < image.height(); j++){
    int level = 255 * (im[i+j*image.width()]-min)/(max-min);
    paint.setPen(QColor(level,level,level));
    paint.drawPoint(i,j);
  }
  
  paint.end();
  delete[] im;
  
  resize(image.width(), image.height());
  repaint(TRUE);
}

void ImageView::LoadImage(const char* filename){
  fname = filename;
  if(image.load(filename)){
    //DSB
    image = image.xForm(QWMatrix().scale(scaleFactor,scaleFactor));
    //cout << "image <" << filename << "> loaded." << endl;;
    resize(image.width(), image.height());
    repaint(TRUE);
  }
  else{
    cerr << "Could not load image: <" << filename << ">" << endl;
  }
  repaint();
}

void ImageView::clearImage(){
  image.fill(QColor(255,255,255));
  repaint();
}

//DSB
#include <qpainter.h>
void ImageView::paintEvent(QPaintEvent*){
  bitBlt(this,0,0,&image,0,0,image.width(),image.height(),CopyROP,FALSE);
  QPainter paint(this);
    paint.setPen(QColor(255,0,0));
    paint.drawLine(leyex*scaleFactor-3,leyey*scaleFactor,leyex*scaleFactor+3,leyey*scaleFactor);
    paint.drawLine(leyex*scaleFactor,leyey*scaleFactor-3,leyex*scaleFactor,leyey*scaleFactor+3);
    paint.drawLine(reyex*scaleFactor-3,reyey*scaleFactor,reyex*scaleFactor+3,reyey*scaleFactor);
    paint.drawLine(reyex*scaleFactor,reyey*scaleFactor-3,reyex*scaleFactor,reyey*scaleFactor+3);

}


//DSB
void ImageView::mousePressEvent ( QMouseEvent * me){
    if(me->button() == Qt::LeftButton){
        cout << "left eye: " << me->x()/scaleFactor << "X" << me->y()/scaleFactor << endl;
        leyex = me->x()/scaleFactor;
        leyey = me->y()/scaleFactor;
    }
    else{
        cout << "right eye: " << me->x()/scaleFactor << "X" << me->y()/scaleFactor << endl;
        reyex = me->x()/scaleFactor;
        reyey = me->y()/scaleFactor;
        eyecoords << fname << "\t" << leyex <<  "\t" << leyey << "\t" << reyex << "\t" << reyey << endl;
    }
    repaint(true);

}

float ImageView::scaleFactor = 2.0;
