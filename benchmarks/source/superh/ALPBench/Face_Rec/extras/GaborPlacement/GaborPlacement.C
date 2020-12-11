#include <iostream.h>
#include <qapplication.h>
#include "GaborWindow.h"
#include "GaborPlacement.h"
#include <stdlib.h>

void usage(const char* progname){
    cout << "usage: " << progname << " image_list sample_graph image_directory graph_directory\n\n" << endl;
    cout << "    The current version of this program does not use any buttons.\n"
            "    To use the program you pass in an image list, a sample graph\n"
            "    that will be placed over all the faces, an image directory that\n"
            "    containes sfi images of the faces, and a graph directory where\n"
            "    graph discriptions will be saved.\n"
            "\n"
            "    In the program you click each image, place the graph nodes in\n"
            "    the correct location and repeat.  As soon as you leave an image\n"
            "    the current graph is saved in the graph directory under the \n"
            "    image file name.\n";
    exit(1);
}

void process_command(){}

int main( int argc, char* argv[] ){

    QApplication app(argc, argv);
    GaborWindow mainWindow;
    if(argc == 5){
        mainWindow.init(argv[1],argv[2],argv[3], argv[4]);
    }
    else{
        usage(argv[0]);
    }
    app.setMainWidget(&mainWindow);
    mainWindow.show();

    int ret = app.exec();
    
    return ret; 
}
