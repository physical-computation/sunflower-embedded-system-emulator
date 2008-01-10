#include "formview.h"

#include "radiowidget.h"

#include "facedata.h"

#include <qlabel.h>

#include <iostream.h>

FormView::FormView(QWidget* parent):QWidget(parent){
  RadioWidget* rw;

  rw = new RadioWidget("GENDER",this);
  rw->addValue("MALE");
  rw->addValue("FEMALE");
  rw->setDefault(0);
  rw->move(0,0);
  personform.push_back(rw);
  rw = new RadioWidget("RACE",this);
  rw->addValue("WHITE");
  rw->addValue("BLACK");
  rw->addValue("ASIAN");
  rw->addValue("OTHER");
  rw->setDefault(0);
  rw->move(0,0);
  personform.push_back(rw);
  rw = new RadioWidget("AGE",this);
  rw->addValue("TEEN");
  rw->addValue("20");
  rw->addValue("30");
  rw->addValue("40");
  rw->addValue("50");
  rw->addValue("60+");
  rw->setDefault(2);
  rw->move(0,0);
  personform.push_back(rw);
  rw = new RadioWidget("SKIN",this);
  rw->addValue("CLEAR");
  rw->addValue("FRECKELED");
  rw->addValue("WRINKLED");
  rw->addValue("BOTH");
  rw->addValue("OTHER");
  rw->setDefault(0);
  rw->move(0,0);
  personform.push_back(rw);
  rw = new RadioWidget("GLASSES",this);
  rw->addValue("YES");
  rw->addValue("NO");
  rw->setDefault(1);
  rw->move(0,0);
  imageform.push_back(rw);
  rw = new RadioWidget("FACIAL_HAIR",this);
  rw->addValue("YES");
  rw->addValue("NO");
  rw->setDefault(1);
  rw->move(0,0);
  imageform.push_back(rw);
  rw = new RadioWidget("EXPRESSION",this);
  rw->addValue("NEUTRAL");
  rw->addValue("OTHER");
  rw->setDefault(0);
  rw->move(0,0);
  imageform.push_back(rw);
  rw = new RadioWidget("MOUTH",this);
  rw->addValue("CLOSED");
  rw->addValue("OPEN");
  rw->addValue("TEETH");
  rw->addValue("OTHER");
  rw->setDefault(0);
  rw->move(0,0);
  imageform.push_back(rw);
  rw = new RadioWidget("EYES",this);
  rw->addValue("CLOSED");
  rw->addValue("OPEN");
  rw->addValue("OTHER");
  rw->setDefault(1);
  rw->move(0,0);
  imageform.push_back(rw);
  rw = new RadioWidget("BANGS",this);
  rw->addValue("YES");
  rw->addValue("NO");
  rw->setDefault(1);
  rw->move(0,0);
  imageform.push_back(rw);
  rw = new RadioWidget("MAKEUP",this);
  rw->addValue("YES");
  rw->addValue("NO");
  rw->setDefault(1);
  rw->move(0,0);
  imageform.push_back(rw);
}

inline int max(int x, int y){ return x > y ? x : y ; }

void FormView::resizeEvent(QResizeEvent*){
  int currenty = 5;
  int labelx = 0;
  for(int i = 0; i < personform.size(); i++){
    personform[i]->move(5,currenty);
    currenty += personform[i]->height();
  }
  for(int i = 0; i < imageform.size(); i++){
    imageform[i]->move(5,currenty);
    currenty += imageform[i]->height();
  }


}
  
void FormView::storedata(PersonData* pdata, ImageData* idata){
  for(int i = 0; i < personform.size(); i++){
    if(pdata){
      pdata->covariates[personform[i]->currentLabel()] = personform[i]->currentValue();
    } 
  }
  for(int i = 0; i < imageform.size(); i++){
    if(idata){
      idata->covariates[imageform[i]->currentLabel()] = imageform[i]->currentValue();
    } 
  }
}
  
void FormView::loaddata(PersonData* pdata, ImageData* idata){
  for(int i = 0; i < personform.size(); i++){
    if(pdata && pdata->covariates.count(personform[i]->currentLabel())){
      //cout << personform[i]->currentLabel() << endl;
      personform[i]->setValue(pdata->covariates[personform[i]->currentLabel()]);
    }
    else{
      //cout << "NOT" << personform[i]->currentLabel() << endl;
      if(personform[i])personform[i]->reset();
    }
  }
  for(int i = 0; i < imageform.size(); i++){
    if(idata && idata->covariates.count(imageform[i]->currentLabel())){
      imageform[i]->setValue(idata->covariates[imageform[i]->currentLabel()]);
    }
    else{
      if(imageform[i])imageform[i]->reset();
    }
  }

}
