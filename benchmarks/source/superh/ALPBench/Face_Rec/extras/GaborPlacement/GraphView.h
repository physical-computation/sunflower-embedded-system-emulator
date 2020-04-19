#ifndef IMAGEVIEW_INCLUDED
#define IMAGEVIEW_INCLUDED

#include <qwidget.h>
#include <qpixmap.h>
#include <qimage.h>
#include <csuCommonImage.h>
#include <csuGaborUtil.h>

class GraphView:public QWidget{
    Q_OBJECT
public:
    GraphView(QWidget* parent, char * name);
public slots:
    void loadImage(Image im);
    void clearImage();

    void loadGraph(GraphDiscription);

    void resizeEvent(QResizeEvent* e);
    void paintEvent(QPaintEvent*);

    void getPoint(int& x, int &y, const Vert& vert) const;
    void setPoint(int x, int y, Vert& vert) const ;
    void setDiscription(GraphDiscription d);

    virtual void mousePressEvent ( QMouseEvent * me);
    virtual void mouseMoveEvent ( QMouseEvent * me);
    virtual void mouseReleaseEvent ( QMouseEvent * me);

signals:
    void pointChanged(int, double, double);

private:
    QImage image;
    QString fname;

    Image im;
    GraphDiscription disc;
    int sPoint;
    int mouseTrack;


};



#endif
