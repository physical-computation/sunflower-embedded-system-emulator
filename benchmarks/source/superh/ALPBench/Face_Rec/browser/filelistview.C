#include "filelistview.h"

#include "imageview.h"


#include "qfiledialog.h"

#include "formview.h"

#include "facedata.h"

#include <qdir.h>

#include <iostream.h>

#define IMAGE_COL 0
#define PERSON_COL 1
#define SOURCE_COL 2
#define NORM_COL 3
#define DATA_COL 8


int min(int x, int y){ x < y ? x : y;}

QString getBaseName(QString filename){
  QString basename(filename);
  for(int i = 0; i < basename.length(); i++){
    if(basename[i] == '.'){
     
      basename.truncate(i);
      //      cout << basename << endl;
      return basename;
    }
  }
  return basename;
}
    

FileListView::FileListView(QWidget* parent,ImageView* sview,ImageView* nview):
  QListView(parent,"Files"),
  sourceview(sview),
  normview(nview),
  currentimage(0)
{  
  addColumn("Base Name");
  addColumn("Person");
  addColumn("Source");
  addColumn("Norm");
  addColumn("#");
  addColumn("Type");
  addColumn("Camera");
  addColumn("Date");
  addColumn("Data");
  

  QDir sourceDir(QFileDialog::getExistingDirectory());
  sourcepath = sourceDir.path();
  QDir normDir(QFileDialog::getExistingDirectory());
  normpath = normDir.path();
  int count = min(sourceDir.count(),normDir.count());
  for(int i = 0; i < sourceDir.count(); i++){
    if(sourceDir[i].length() && sourceDir[i][0] != '.'){ 
      QString base = getBaseName(sourceDir[i]);
      files[base].sourceName = sourceDir[i];
      files[base].person = base;
      files[base].person.truncate(5);
      files[base].basename = base;
      files[base].type = base;
      files[base].type.remove(0,5).remove(2,100);
      files[base].type = base;
      files[base].type.remove(0,5).remove(2,100);
      files[base].camera = base;
      files[base].camera.remove(0,7).remove(3,100);
      int datestart = 10;
      for(; datestart < base.length(); datestart++){
	if(base[datestart] == '_') break;
      }
      datestart++;
      files[base].date = base;
      files[base].date.remove(0,datestart).remove(6,100);
    }
  }

  for(int i = 0; i < normDir.count(); i++){
    if(normDir[i].length() && normDir[i][0] != '.'){
      QString base(getBaseName(normDir[i]));
      files[base].normName = normDir[i];
      files[base].person = base;
      files[base].person.truncate(5);
      files[base].basename = base;
      files[base].type = base;
      files[base].type.remove(0,5).remove(2,100);
      files[base].type = base;
      files[base].type.remove(0,5).remove(2,100);
      files[base].camera = base;
      files[base].camera.remove(0,7).remove(3,100);
      int datestart = 10;
      for(; datestart < base.length(); datestart++){
	if(base[datestart] == '_') break;
      }
      datestart++;
      files[base].date = base;
      files[base].date.remove(0,datestart).remove(6,100);
    }
  }
  
  int personinstance = 0;
  QString lastperson = "";
  std::map<QString,FileInfo>::iterator each = files.begin();
  for( ; each != files.end() ; each++){
    QString name = getBaseName(each->second.sourceName);
    if(name == "-"){
      name = getBaseName(each->second.normName);
    }
    QString person = name;
    person.truncate(5);
    if(person == lastperson) personinstance++;
    else personinstance = 0;
    lastperson = person;
    QListViewItem* tempitem = new QListViewItem(
		      this,
		      each->second.basename,
		      each->second.person,
		      each->second.sourceName,
		      each->second.normName,
		      QString(personinstance < 10 ? "0" : "") + QString().setNum(personinstance),
		      each->second.type,
		      each->second.camera,
		      each->second.date);
    tempitem->setText(8,"NO");
  }

  QObject::connect(this,SIGNAL(selectionChanged ( QListViewItem * )),this,SLOT(switchImages( QListViewItem *)));

}
 
void FileListView::switchImages( QListViewItem* selection){

  if(currentimage){
    form->storedata(&facedata.persondata[currentimage->text(PERSON_COL)],&facedata.imagedata[currentimage->text(IMAGE_COL)]);
    facedata.imagedata[currentimage->text(IMAGE_COL)].person = currentimage->text(PERSON_COL); // save the person field
    currentimage->setText(DATA_COL,"YES");
  }
  currentimage = selection;
  form->loaddata(
		 facedata.persondata.count(currentimage->text(PERSON_COL)) ? &facedata.persondata[currentimage->text(PERSON_COL)] : NULL ,
		 facedata.imagedata.count( currentimage->text(IMAGE_COL)) ? &facedata.imagedata[currentimage->text(IMAGE_COL)]  : NULL );


  QString sourcefile = sourcepath + "/" + selection->text(SOURCE_COL);
  if(selection->text(SOURCE_COL) != "-")
    sourceview->LoadImage(sourcefile);
  else
    sourceview->clearImage();
  QString normfile = normpath + "/" + selection->text(NORM_COL);
  if(selection->text(NORM_COL) != "-")
    normview->LoadFR(normfile,130,150);
  else
    normview->clearImage();

}
void FileListView::saveForm(){

  if(currentimage){
    form->storedata(&facedata.persondata[currentimage->text(PERSON_COL)],&facedata.	imagedata[currentimage->text(IMAGE_COL)]);
    facedata.imagedata[currentimage->text(IMAGE_COL)].person = currentimage->text(PERSON_COL); 
        // save the person field
  }

}

void FileListView::loadForm(){
    QListViewItem* temp = firstChild();
    int i = 0;
    while(temp){
        i++;
        if( facedata.imagedata.count( temp->text(IMAGE_COL)) ){
            temp->setText(DATA_COL,"YES");
        } else {
            temp->setText(DATA_COL,"NO");
        }
            
        temp = temp->nextSibling();	
    }
    cout << "Updated " << i << endl;

  if(currentimage){
  form->loaddata(
		 facedata.persondata.count(currentimage->text(NORM_COL)) ? &facedata.persondata[currentimage->text(NORM_COL)] : NULL ,
		 facedata.imagedata.count( currentimage->text(SOURCE_COL)) ? &facedata.imagedata[currentimage->text(SOURCE_COL)]  : NULL );
  }

}

void FileListView::changeData(QString field, QString value){
  saveForm();
  if(currentimage){
    if(field == "GENDER" || field == "RACE" || field == "AGE" || field == "SKIN" ){
      facedata.persondata[currentimage->text(PERSON_COL)].covariates[field] = value;
    } else {
      facedata.imagedata[currentimage->text(IMAGE_COL)].covariates[field] = value;
    }
   
    form->loaddata(
		   facedata.persondata.count(currentimage->text(PERSON_COL)) ? &facedata.persondata[currentimage->text(PERSON_COL)] : NULL ,
		   facedata.imagedata.count( currentimage->text(IMAGE_COL)) ? &facedata.imagedata[currentimage->text(IMAGE_COL)]  : NULL );
  }
}

void FileListView::nextItem(){
  setCurrentItem(currentItem()->itemBelow());
}

void FileListView::prevItem(){
  setCurrentItem(currentItem()->itemAbove());

}
