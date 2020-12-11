/*
 *  GaborWindow.cpp
 *  QTGaborGui
 *
 *  Created by David  Bolme on Mon Jul 29 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "GaborWindow.h"
#include <stdio.h>
#include <qlistview.h>
#include "GaborPlacement.h"
#include "csuCommonFile.h"
#include "GraphView.h"
#include <iostream.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define READ_ONLY 1

GaborWindow::GaborWindow(){
    currentImage = NULL;
    imageDir = NULL;
    pointList->setSelectionMode(QListView::Single);
}

void GaborWindow::init(char* imagelist, char* samplegraph, char* imagedir, char* graphdir){
    char name[1024];

    imageDir = imagedir;
    graphDir = graphdir;

    FILE* file = fopen(imagelist,"r");
    while(file && fscanf(file, "%s", name) == 1){
        imageList->insertItem(new QListViewItem(imageList,name));
    }
    fclose(file);

    discription = readGraphDiscription(samplegraph);
    graphview->setDiscription(discription);

    for(int i = 0; i < discription->numVert; i++){
        pointList->insertItem(new QListViewItem(pointList,discription->vertLabels[i], QString::number(discription->verts[i].x), QString::number(discription->verts[i].y)));
    }
    
    //connect(imageList,SIGNAL(selectionChanged(QListViewItem*)), this,SLOT(loadImage(QListViewItem*)));
}

void GaborWindow::loadData(){
    qWarning( "Not implemented yet!" );
}


void GaborWindow::loadImage(QListViewItem* item){
    cout << "loading image" << endl;
    if(item){
        if(currentImage){
            freeImage(currentImage);

            if(!READ_ONLY){
                saveGraphDiscription(makePath(graphDir,currentImageName),discription);
            }
            else{
                printf("READ ONLY MODE\n");
            }
        }
        printf("Loading image: %s\n",item->text(0).latin1());
        currentImageName = item->text(0);
        if(currentImageName.right(4) ==  ".sfi"){
            currentImage = readRawImage(makePath(imageDir,item->text(0).latin1()));
        }
        else if(currentImageName.right(4) == ".pgm"){
            currentImage = readRawImage(makePath(imageDir,item->text(0).latin1()));
        }
        else{
            cout << "Cannot read image format for image: " << currentImageName << endl;
        }

        graphview->loadImage(currentImage);
        int ok;
        {
            struct stat buf;
            int err = stat (makePath(graphDir,item->text(0).latin1()), &buf);
            ok = (err == 0) && S_ISREG (buf.st_mode) && (access (makePath(graphDir,item->text(0).latin1()), R_OK) == 0);
        }
        if(ok){
            freeGraphDiscription(discription);
            discription = readGraphDiscription(makePath(graphDir,currentImageName));
            graphview->setDiscription(discription);
        }


    }
    else{
        printf("No Image Clicked\n");
    }
    repaint();
}

void GaborWindow::resetGraph(){
}


void GaborWindow::resetPoint(){
    qWarning( "Not implemented yet!" );
}


void GaborWindow::saveData(){
    qWarning( "Not implemented yet!" );
}


void GaborWindow::updatePoint(int point,double x,double y){
    QListViewItem* tmp = pointList->firstChild();
    while(tmp != NULL){
        if(tmp->text(0) == QString(discription->vertLabels[point])){
            pointList->setSelected(tmp,true);
            tmp->setText(1,QString::number(x));
            tmp->setText(2,QString::number(y));
            pointList->ensureItemVisible(tmp);
            tmp->repaint();
            tmp = NULL;
        }
        else{
            tmp = tmp->nextSibling();
        }
    }
    pointx = x;
    pointy = y;
}



