#include "facedata.h"
#include <iostream.h>
#include <fstream.h>
#include <qfiledialog.h>

//std::map< QString , PersonData > persondata; //map from person label to person data

//std::map< QString , ImageData > imagedata; //map form image label to image data

FaceData facedata;
void FaceData::AddOtherFields(){
   for(std::map<QString,ImageData>::iterator eachimage = imagedata.begin();
      eachimage != imagedata.end();
      eachimage++){
     (*eachimage).second.covariates["MOUTH2"] = (*eachimage).second.covariates["MOUTH"] == "CLOSED" ? "CLOSED" : "OTHER";
   }
   for(std::map<QString,PersonData>::iterator eachperson = persondata.begin();
      eachperson != persondata.end();
      eachperson++){
     if( 
	(*eachperson).second.covariates["AGE"] == "TEEN" ||
	(*eachperson).second.covariates["AGE"] == "20" ||
	(*eachperson).second.covariates["AGE"] == "30"
       ){
       (*eachperson).second.covariates["AGE2"] = "YOUNG";
     }
     else if( 
	(*eachperson).second.covariates["AGE"] == "40" ||
	(*eachperson).second.covariates["AGE"] == "50" ||
	(*eachperson).second.covariates["AGE"] == "60+"
       ){
       (*eachperson).second.covariates["AGE2"] = "OLD";
     }
     else{
       (*eachperson).second.covariates["AGE2"] = "ERROR";
     }
     if( 
	(*eachperson).second.covariates["SKIN"] == "CLEAR"
	){
       (*eachperson).second.covariates["SKIN2"] = "CLEAR";
     }
     else if( 
	(*eachperson).second.covariates["SKIN"] == "FRECKELED" ||
	(*eachperson).second.covariates["SKIN"] == "WRINKLED" ||
	(*eachperson).second.covariates["SKIN"] == "BOTH" ||
	(*eachperson).second.covariates["SKIN"] == "OTHER"
	){
       (*eachperson).second.covariates["SKIN2"] = "OTHER";
     }
     else{
       (*eachperson).second.covariates["SKIN"] = "NO_DATA";
       (*eachperson).second.covariates["SKIN2"] = "NO_DATA";
     }
   }
} 

void FaceData::LoadData(){
  cout << "Loading Data..." << endl;
  ifstream file(QFileDialog::getOpenFileName());
  int num_images = 0;
  file >> num_images;
  for(int i = 0; i < num_images; i ++){
    char person[256], image[256], attribute[256], value[256];
    int personattributes, imageattributes;
    file >> image >> person >> personattributes >> imageattributes;
    imagedata[image].person = person;
    for(int j = 0; j < personattributes; j++){
      file >> attribute >> value;
      persondata[person].covariates[attribute] = value;
    }
    for(int j = 0; j < imageattributes; j++){
      file >> attribute >> value;
      imagedata[image].covariates[attribute] = value;
    }
    
  }
  AddOtherFields();
  emit loadForm();
	
}


void FaceData::SaveData(){
 emit saveForm();
 AddOtherFields();
  ofstream file(QFileDialog::getSaveFileName());
  cout << "Saving Data..." << endl;
  file << imagedata.size() << endl;
  for(std::map<QString,ImageData>::iterator eachimage = imagedata.begin();
      eachimage != imagedata.end();
      eachimage++){
    file << eachimage->first 
	 << "\t" << eachimage->second.person 
	 << "\t" << persondata[eachimage->second.person].covariates.size()
	 << "\t" << eachimage->second.covariates.size() << "\t";
    for(std::map<QString,QString>::iterator eachcovariate = persondata[eachimage->second.person].covariates.begin();
	eachcovariate != persondata[eachimage->second.person].covariates.end();
	eachcovariate++){
      file << eachcovariate->first << "\t" << eachcovariate->second << "\t";
    }
    for(std::map<QString,QString>::iterator eachcovariate = eachimage->second.covariates.begin();
	eachcovariate != eachimage->second.covariates.end();
	eachcovariate++){
      file << eachcovariate->first << "\t" << eachcovariate->second << "\t";
    }

    file << endl;
  }

}

void FaceData::ClearData(){ 
  persondata.clear(); 
  imagedata.clear(); 
  cout<< "Data cleared." << endl;
  emit loadForm();
}
//FaceData::~FaceData(){}
 
