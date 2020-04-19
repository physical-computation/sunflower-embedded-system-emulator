/*
 *  GaborWindow.h
 *  QTGaborGui 
 *
 *  Created by David  Bolme on Mon Jul 29 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GABOR_WINDOW_INCLUDED
#define GABOR_WINDOW_INCLUDED

#include "MainWindow.h"
#include <csuCommonImage.h>
#include <csuGaborUtil.h>

class GaborWindow:public MainWindow{
    Q_OBJECT
public:
    GaborWindow();
    void init(char* imagelist, char* samplegraph, char* imagedir, char* graphdir);
    virtual void loadData();
    virtual void loadImage(QListViewItem*);
    virtual void resetGraph();
    virtual void resetPoint();
    virtual void saveData();
    virtual void updatePoint(int,double,double);

    char* imageDir;
    char* graphDir;
    Image currentImage;
    QString currentImageName;
    GraphDiscription discription;
    double pointx;
    double pointy;
};

#endif



