#include "GraphView.h"

#include <qpainter.h>
#include <qimage.h>
#include <iostream.h>
#include <csuCommonUtil.h>


GraphView::GraphView(QWidget* parent, char * name):
QWidget(parent,name),image(1,1,32){
    image.fill((uint)qRgb(0,0,0));
    im = NULL;
    setCursor(crossCursor);
    setBackgroundMode(NoBackground);
    disc = NULL;
    mouseTrack = 0;
}
void GraphView::setDiscription(GraphDiscription d){
    disc = d;
    repaint(true);
}


/* GraphView::LoadImage(Image img)
 * This fuction copies data from an Image into a QImage that is displayed in the
 * widget.
 */
void GraphView::loadImage(Image img){
    im = img;

    image = QImage(width(),height(),32);
    if(im){

        float min = IE(im,0,0,0);
        float max = IE(im,0,0,0);
        for(int x = 0; x < im->width; x++){
            for(int y = 0; y < im->height; y++){
                for(int c = 0; c < im->channels; c++){
                    if(min > IE(im,x,y,c)) min = IE(im,x,y,c);
                    if(max < IE(im,x,y,c)) max = IE(im,x,y,c);
                }
            }
        }

        double iScale = im->width/(double)image.width();
        double jScale = im->height/(double)image.height();

        for(int i = 0; i < image.width(); i++){
            for(int j = 0; j < image.height(); j++){
                int level = ROUND(255.0 * (interpLinear(im,i*iScale,j*jScale,0) - min )/(max-min));

                image.setPixel(i,j,(uint)qRgb(level,level,level));
            }
        }
    }

    repaint(TRUE);
}

void GraphView::loadGraph(GraphDiscription){

}

void GraphView::clearImage(){
    image.fill((uint)qRgb(0,0,0));
    repaint(true);
}

void drawCrossHair(QPainter& paint, int x, int y){
    paint.setPen(QColor(255,0,0));
    paint.moveTo(x,y-4);
    paint.lineTo(x,y+4);
    paint.moveTo(x-4,y);
    paint.lineTo(x+4,y);
}

void drawEdge(QPainter& paint, int x1, int y1, int x2, int y2){
    paint.setPen(QColor(0,0,255));
    paint.moveTo(x1,y1);
    paint.lineTo(x2,y2);
}

void GraphView::paintEvent(QPaintEvent*){
    QPixmap backbuffer;
    backbuffer.convertFromImage (image);
    QPainter paint(&backbuffer);
    if(disc){
        for(int i = 0; i < disc->numEdge; i++){
            int x1,y1,x2,y2;
            getPoint(x1, y1, disc->verts[disc->edges[i].vert1]);
            getPoint(x2, y2, disc->verts[disc->edges[i].vert2]);
            drawEdge(paint,x1,y1,x2,y2);
        }
        for(int i = 0; i < disc->numVert; i++){
            int x,y;
            getPoint(x, y, disc->verts[i]);
            drawCrossHair(paint,x,y);
        }
    }
    paint.end();
    bitBlt(this,0,0,&backbuffer,0,0,width(),height(),CopyROP,FALSE);
}


void GraphView::mousePressEvent ( QMouseEvent * me){
    /* Select the closest point */
    Vert p;
    setPoint(me->x(), me->y(), p);

    if(disc){
        double bestdist = DIST_2D(p.x,p.y,disc->verts[0].x,disc->verts[0].y);
        sPoint = 0;
        for(int i = 0; i < disc->numVert; i++){
            cout << DIST_2D(p.x,p.y,disc->verts[i].x,disc->verts[i].y)  << " ";
            if(bestdist > DIST_2D(p.x,p.y,disc->verts[i].x,disc->verts[i].y)){
                bestdist = DIST_2D(p.x,p.y,disc->verts[i].x,disc->verts[i].y);
                sPoint = i;
            }
        }
    }

    cout << disc->vertLabels[sPoint] << " (" << sPoint << ")"<< endl;

    setPoint(me->x(), me->y(), disc->verts[sPoint]);

    emit pointChanged(sPoint,disc->verts[sPoint].x,disc->verts[sPoint].y);
    mouseTrack = 1;
    repaint(true);
}

void GraphView::mouseMoveEvent ( QMouseEvent * me){
    if(mouseTrack){
        setPoint(me->x(), me->y(), disc->verts[sPoint]);

        emit pointChanged(sPoint,disc->verts[sPoint].x,disc->verts[sPoint].y);
        repaint(true);
    }
}

void GraphView::mouseReleaseEvent ( QMouseEvent * me){
    setPoint(me->x(), me->y(), disc->verts[sPoint]);

    emit pointChanged(sPoint,disc->verts[sPoint].x,disc->verts[sPoint].y);
    mouseTrack = 0;
    repaint(true);
}

void GraphView::resizeEvent(QResizeEvent* e){
    QWidget::resizeEvent(e);
    loadImage(im);
}

void GraphView::getPoint(int& x, int &y, const Vert& vert) const{
    if(!im) return;
    x = ROUND(vert.x*image.width()/(double)im->width);
    y = ROUND(vert.y*image.height()/(double)im->height);
}

void GraphView::setPoint(int x, int y, Vert& vert) const{
    if(!im) return;
    vert.x = x*im->width/(double)image.width();
    vert.y = y*im->height/(double)image.height();

}


