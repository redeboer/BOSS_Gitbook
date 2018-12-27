//--------------------------------------------------------------------------
//
// Environment:
//
// Module: EvtGen/EvtHis2F.hh
//
// Description: Class to handle H2F histogram
//
// Modification history:
//
//    PING RG     September 8, 2010         Module created
//
//------------------------------------------------------------------------

#include "EvtGenBase/EvtHis2F.hh"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>


 using namespace std;
 using std::endl;

EvtHis2F::~EvtHis2F(){}

const char* EvtHis2F::getFile(){
  return datafile;
}

const char* EvtHis2F::getHTitle(){
  return datatitle;
}

void EvtHis2F::setFile(const char* dtfile){
  datafile=dtfile;
  return;
}

void EvtHis2F::setHTitle(const char* htitle){
  datatitle=htitle;
  return;
}

void   EvtHis2F::setHmc(TH2F *H2){
  HMC=(TH2F*) H2->Clone("HMC");
  return;
}

void   EvtHis2F::setHdt(TH2F *H2){
  HDATA=(TH2F*) H2->Clone("HDATA");
  return;
}

TH2F*  EvtHis2F::getHmc(){
  return HMC;
}

TH2F*  EvtHis2F::getHdt(){
  return HDATA;
}

TH2F*  EvtHis2F::getHwt(){
  return HWT;
}

void  EvtHis2F::show(TH2F *h2){
  TAxis* Xaxis=h2->GetXaxis();
  TAxis* Yaxis=h2->GetYaxis();
 
  int bx  =Xaxis->GetLast();  
  int by  =Yaxis->GetLast();

  for(int i=1;i<bx+1;i++){
    for(int j=1;j<by+1;j++){
      int ij=h2->GetBin(i,j);
      double xij=h2->GetBinContent(ij);
      std::cout<<"i,j,cell,value "<<i<<" "<<j<<" "<<ij<<" "<<xij<<std::endl;
    }
  }
}

void  EvtHis2F::showFrame(TH2F *h2){
  TAxis* Xaxis=h2->GetXaxis();
  TAxis* Yaxis=h2->GetYaxis();
 
  int bx  =Xaxis->GetLast();  
  int by  =Yaxis->GetLast();
  
  double x1 =Xaxis->GetBinLowEdge(1);
  double y1 =Yaxis->GetBinLowEdge(1);
  double x2 =Xaxis->GetBinUpEdge(bx);
  double y2 =Yaxis->GetBinUpEdge(by);
  std::cout<<"N_x, x_low, x_up; N_y, y_low, y_up "
           <<bx<<" "<<x1<<" "<<x2<<" "<<by<<" "<<y1<<" "<<y2<<std::endl;
}

void  EvtHis2F::init(){
  const char* dtfile =getFile();  // get data root file as input
  const char* htitle =getHTitle();  // get Dalitz plot title

  TFile dataf(dtfile);
  HDATA = (TH2F*)dataf.Get(htitle);

  double ndata=HDATA->Integral();
  std::cout<<"Number events in HDATA= "<<ndata<<std::endl;

  TAxis* Xaxis=HDATA->GetXaxis();
  TAxis* Yaxis=HDATA->GetYaxis();
 
  BINSx  =Xaxis->GetLast();  
  BINSy  =Yaxis->GetLast();
  
  xlow=Xaxis->GetBinLowEdge(1);
  ylow=Yaxis->GetBinLowEdge(1);
  xup =Xaxis->GetBinUpEdge(BINSx);
  yup =Yaxis->GetBinUpEdge(BINSy);

  std::cout<<"BINSx,xlow,xup,BINSy,ylow,yup: "<<BINSx<<" ,"<<xlow<<", "<<xup<<", "<<BINSy<<", "<<ylow<<", "<<yup<<std::endl;
  HMC = new TH2F("myHMC","",BINSx,xlow,xup,BINSy,ylow,yup);
  HWT = new TH2F("myHWT","",BINSx,xlow,xup,BINSy,ylow,yup);
  HMC ->SetDirectory(0);
  HWT ->SetDirectory(0);
  HDATA->SetDirectory(0);
  icount=0;
  return;
}

void  EvtHis2F::HFill(double m12s,double m13s){
  HMC->Fill(m12s,m13s,1.0);
  icount++;
  //  if(icount==100000)show(HMC);
  if(icount==100000) showFrame(HMC);
  return;
}

void  EvtHis2F::HReweight(){  //reweight Dalitz Plot after get HDATA and HMC
  std::cout<<"Now I reweight the MC to the data Dalitz Plot"<<std::endl;
  double ndata=HDATA->Integral();
  std::cout<<"Number events in Dalitz plot = "<<ndata<<std::endl;
  double nmc=HMC->Integral();
  std::cout<<"Number of events in HMC reweight= "<<nmc<<std::endl;
  HWT->Divide(HDATA,HMC);
  ndata=HWT->Integral();
  std::cout<<"Number of events after reweight= "<<ndata<<std::endl;
  double norm=nmc/ndata;
  HWT->Scale(norm);
  nmc=HMC->Integral();
  std::cout<<"Number of events in HMC after scale= "<<nmc<<std::endl;
}

void  EvtHis2F::setBins(TH2F *h2){
  TAxis* Xaxis=h2->GetXaxis();
  TAxis* Yaxis=h2->GetYaxis();
  BINSx  =Xaxis->GetLast();  
  BINSy  =Yaxis->GetLast();
  xlow=Xaxis->GetBinLowEdge(1);
  ylow=Yaxis->GetBinLowEdge(1);
  xup =Xaxis->GetBinUpEdge(BINSx);
  yup =Yaxis->GetBinUpEdge(BINSy);
}
void  EvtHis2F::setZmax(TH2F *hwt){
  double ymax=0;
  setBins(hwt);

   for(int dx=1;dx <= BINSx;dx++){
     for(int dy=1;dy <= BINSy;dy++){
       int nxy=hwt->GetBin(dx,dy);
       double ncell=hwt->GetBinContent(nxy);
       if(ncell<=0) continue;
       if(ncell>ymax){ymax=ncell;}
     }
   }
   zmax=ymax;
}

void  EvtHis2F::setZmax(){
  double ymax=0;
  setBins(HWT);

   for(int dx=1;dx <= BINSx;dx++){
     for(int dy=1;dy <= BINSy;dy++){
       int nxy=HWT->GetBin(dx,dy);
       double ncell=HWT->GetBinContent(nxy);
       if(ncell<=0) continue;
       if(ncell>ymax){ymax=ncell;}
     }
   }
   zmax=ymax;
}

double  EvtHis2F::getZmax(){
  return zmax;
}

double  EvtHis2F::getZvalue(double xmass2,double ymass2){
  int xbin = HWT->GetXaxis()->FindBin(xmass2);
  int ybin = HWT->GetYaxis()->FindBin(ymass2);
  int xybin= HWT->GetBin(xbin,ybin);
  double zvalue=HWT->GetBinContent(xybin);
//  std::cout<<"xmass, ymass, zvalue "<<xmass2<<" "<<ymass2<<" "<<zvalue<<std::endl;
  return zvalue;
}

bool  EvtHis2F::AR(double xmass2,double ymass2){//accept: true. reject: false
  //call this function after initiation and filling MC histogram HMC
  bool accept;
  accept=false;
  if(xmass2<xlow || xmass2>xup || ymass2<ylow || ymass2>yup) {return accept;}
  double zvalue=getZvalue(xmass2,ymass2);
  double ratio=zvalue/zmax;
  double rndm=EvtRandom::Flat(0.0, 1.0);  
  //  std::cout<<"zvalue, zmax , rndm "<<zvalue<<" "<<zmax<<" "<<rndm<<std::endl;
  if(ratio > rndm){accept=true;} else {accept=false;}
  return accept;
}

bool  EvtHis2F::AR(double xmass2,double ymass2, double zmax, TH2F *h2){//accept: true. reject: false
  //call this function after initiation and filling MC histogram HMC
  bool accept;
  accept=false;
  setBins(h2);
  if(xmass2<xlow || xmass2>xup || ymass2<ylow || ymass2>yup) {return accept;}
  int xbin = h2->GetXaxis()->FindBin(xmass2);
  int ybin = h2->GetYaxis()->FindBin(ymass2);
  int xybin= h2->GetBin(xbin,ybin);
  double zvalue=h2->GetBinContent(xybin);

  double ratio=zvalue/zmax;
  double rndm=EvtRandom::Flat(0.0, 1.0);  
  //  std::cout<<"zvalue, zmax , rndm "<<zvalue<<" "<<zmax<<" "<<rndm<<std::endl;
  if(ratio > rndm){accept=true;} else {accept=false;}
  return accept;
}

void EvtHis2F::init(TH2F *hmc, TH2F *hdt, TH2F *hwt){
  setHmc(hmc);
  setHdt(hdt);
  setHwt(hwt);
  HReweight();
  setZmax();
}

