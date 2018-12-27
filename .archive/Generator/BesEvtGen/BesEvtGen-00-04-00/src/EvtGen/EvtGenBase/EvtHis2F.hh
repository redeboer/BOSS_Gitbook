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

#ifndef EvtHis2F_HH
#define EvtHis2F_HH

#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TApplication.h"
#include "TROOT.h"
#include "TChain.h"
#include "TDirectory.h"
#include "EvtGenBase/EvtRandom.hh"

class EvtHis2F {

public:

  EvtHis2F(){}
  virtual ~EvtHis2F();
  void init();
  void setFile(const char* dtfile);
  void setHTitle(const char* htitle);
  const char* getFile();
  const char* getHTitle();
  void HFill(double xmass2, double ymass2);
  double getZvalue(double m12_square, double m13_square);
  void HReweight();
  bool AR(double xmass2,double ymass2); // major function: judge to accept or reject an event
  bool AR(double xmass2,double ymass2, double zmax, TH2F *h2);
  void setZmax( );
  void setZmax(TH2F *H2);
  double getZmax();
  void setHmc(TH2F *H2);
  void setHdt(TH2F *H2);
  void setHwt(TH2F *H2){ HWT=(TH2F*) H2->Clone("HWT");}

  TH2F*  getHmc( );
  TH2F*  getHdt( );
  TH2F*  getHwt( );
  
  void setBINSx(int bx){BINSx=bx;}
  void setBINSy(int by){BINSy=by;}
  void setXlow (double xl){xlow=xl;}
  void setXup(double xu){xup=xu;}
  void setYlow (double yl){ylow=yl;}
  void setYup(double yu){yup=yu;}

  void setBins(TH2F *h2);
  int getBINSx(){return BINSx;}
  int getBINSy(){return BINSy;}
  double getXlow(){return xlow;}
  double getYlow(){return ylow;}
  double getXup(){return xup;}
  double getYup(){return yup;}

  void show(TH2F *h2);
  void showFrame(TH2F *h2);

  void init(TH2F *hmc, TH2F *hdt, TH2F *hwt);

private:
  TH2F *HDATA, *HMC, *HWT; //input DATA, MC AND RE-WEIGHTED DALITIZ PLOT
  TFile *dataf;
  double zmax;  //maxium value at the reweight diagram
  int BINSx, BINSy;
  double xlow,xup,ylow,yup;
  const char *datafile, *datatitle;
  TDirectory dir;
  int icount;
  double max1,max2,max3;
  TH2F *WT1,*WT2,*WT3;
};


#endif

