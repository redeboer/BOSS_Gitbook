//--------------------------------------------------------------------------
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006  Ping Rong-Gang @IHEP, Wang Weiping @USTC  
//
// Module: EvtXsection.hh
//
// Description: To define cross section for the continuum exclusive process
// Experimental cross section taken from PRD73,012005, PRD76,092006, also 
// see a review: Rev. Mod. Phys. 83,1545
// Modification history:
//
//    Ping R.-G.       Nov., 2012       Module created
//
  /*******************--- mode definition: also see EvtXsection.cc
 mode==0: ppbar
 mode==1: nnbar
 mode==2: Lambda0 anti-Lambda0
 mode==3: Sigma0 anti-Sigma0
 mode==4: Lambda0 anti-Sigma0
 mode==5: Sigma0 anti-Lambda0
 mode==6: pi+ pi-
 mode==7: pi+ pi- pi0
 mode==8: K+K- pi0
 mode==9: KsK+pi- 
 mode==10: KsK-pi+
 mode==11: K+K-eta
 mode==12: 2(pi+pi-)
 mode==13: pi+pi-2pi0
 mode==14: K+K-pi+pi-
 mode==15: K+K-2pi0
 mode==16: 2(K+K-)
 mode==17: 2(pi+pi-)pi0
 mode==18: 2(pi+pi-)eta
 mode==19: K+K-pi+pi-pi0  		
 mode==20: K+K-pi+pi-eta
 mode==21: 3(pi+pi-)
 mode==22: 2(pi+pi-pi0)
 mode==23: phi eta
 mode==24: phi pi0
 mode==25: K+K*-
 mode==26: K-K*+
 mode==27: K_SK*0-bar
 mode==28: K*0(892)K+pi-
 mode==29: K*0(892)K-pi+
 mode==30: K*+K-pi0
 mode==31: K*-K+pi0
 mode==32: K_2*(1430)0 K+pi-
 mode==33: K_2*(1430)0 K-pi+
 mode==34: K+K-rho
 mode==35: phi pi+pi-
 mode==36: phi f0(980)
 mode==37: eta pi+pi-
 mode==38: omega pi+ pi-
 mode==39: omega f0(980)
 mode==40: eta' pi+ pi-
 mode==41: f_1(1285)pi+pi-
 mode==42: omega K+K-
 mode==43: omega pi+pi-pi0
 mode==44: Sigma+ Sigma- (Sigma0 Sigma0-bar SU(3) extention )
 mode==45: K+K-
 mode==46: K_S K_L
 mode==47: omega eta
 mode==48: phi eta'
 mode==49: phi K+K-
 mode==50: ppbar pi0
 mode==51: ppbar eta
 mode==52: omega pi0

 mode==70: D0 anti-D0
 mode==71: D+D-
 mode==72: D+D*-
 mode==73: D-D*+
 mode==74: D*+D*-
 mode==68: D0 anti-D*0
 mode==69: anti-D0 D*0
 mode==67: D*0 anti-D*0

 mode==59: D0 anti-D0 pi0
 mode==63: D+D-pi0
 mode==75: D0D-pi+
 mode==76: anti-D0 D+pi-

 mode==77: D0D*-pi+
 mode==78: anti-D0 D*+pi-
 mode==65: D-D*0pi+
 mode==66: D+ anti-D*0pi-
 mode==60: anti-D0 D*0pi0
 mode==61: D0 anti-D*0pi0
 mode==62: D+D*-pi0
 mode==64: D-D*+pi0

 mode==90: pi+pi- J/psi
 mode==92: pi0pi0 J/psi
 mode==91: pi+pi- psi(2S)
 mode==79: pi0pi0 psi(2S)
 mode==80: eta J/psi
 mode==81: pi+pi- h_c
 mode==82: pi0pi0 h_c
 mode==83: K+K- J/psi
 mode==84: KsKs J/psi

 mode==93: D_s+ D_s-
 mode==94: D_s^{*+}D_s^-
 mode==95: D_s^{*-}D_s^+
 mode==85: D_s^{*+}D_s^{*-}

 mode==96: Lambda_c+ anti-Lamba_c-
*/

//------------------------------------------------------------------------
//
#include "EvtGenModels/EvtXsection.hh"
#include "EvtGenBase/EvtPDL.hh"
struct myclass{
bool operator() (int i,int j){return (i<j);}
}mysort;

#include "EvtGenModels/EvtFitXsection.dat"
EvtXsection::~EvtXsection(){}

void EvtXsection::ini_data0(int mode){

  xx.clear();yy.clear();er.clear();
  nbins=yy.size();

} 

void EvtXsection::ini_data_diy(){//user provide xs list

  xx.clear();yy.clear();er.clear();

  ifstream file("xs_user.txt");

  if (!file){
    cout << "EvtXsection.cc: The input file not found. The default file name should be xs_user.txt!"<<std::endl;
    exit(0);
  }

  double xm,xs,xs_er;

  while(!file.eof()){
    file>>xm>>xs>>xs_er;
    //std::cout<<"read XS: "<<xm<<" "<<xs<<" "<<xs_er<<std::endl;
    xx.push_back(xm);
    yy.push_back(xs);
    er.push_back(xs_er);
  }

  xx.pop_back();
  yy.pop_back();
  er.pop_back();
  nbins=yy.size();
  file.close();
  _unit="";
  msg="";

}

void EvtXsection::ini_data_multimode(){//multi-exclusive modes
  xx.clear();yy.clear();er.clear();

  if(_vmd.size()==0){std::cout<<"EvtXsection::ini_data_multimode: No mode indexes are available"<<std::endl; abort();}

  double xL=10.0;
  double xU=-1.0;

  for(int i=0;i<_vmd.size();i++){
    ini_data(_vmd[i]);
    double x0=getXlw();
    double x1=getXup();
    if(x0<xL) xL=x0; 
    if(x1>xU) xU=x1;
  }

  std::cout<<"The low and up end of multimodes: "<<xL<<" ~ "<<xU<<std::endl; 
  double stp=0.0005; //5 MeV for bin width
  double xm;
  double xs=0; //xsection in nb;
  double xs_er=0;
  double mypb=1;
  std::vector<double>uxx,uyy,uer;
  uxx.clear();uyy.clear();uer.clear();
  for(double xxm=xL;xxm<xU;xxm+=stp){
    xm=xxm;
    xs   =0;
    xs_er=0;
    for(int  i=0;i<_vmd.size();i++){
      ini_data(_vmd[i]);
      std::string myunit=getUnit();
      if(myunit=="pb"){ mypb=0.001;}else{mypb=1;}      
      xs   += mypb*getXsection(xxm);
      xs_er+= mypb*getErr(xxm);
      //std::cout<<_vmd[i]<< ": "<<xm<<" "<<xs<<" "<<xs_er<<std::endl;
    }//loop over mode
    uxx.push_back(xm);
    uyy.push_back(xs);
    uer.push_back(xs_er);
  } //loop over mass

  xx.clear();yy.clear();er.clear();
  for(int i=0;i<uxx.size();i++){
    xx.push_back(uxx[i]);yy.push_back(uyy[i]);er.push_back(uer[i]);
  }
  //debugging
  //for(int i=0;i<yy.size();i++){
  //  std::cout<<xx[i]<<" "<<yy[i]<<std::endl;
  //}
  _unit="nb";
  nbins=yy.size();
  msg="multi-exclusive mode";
}

int EvtXsection::getMode(std::vector<EvtId> evtdaugs){
  /*******************--- mode definition:
  0: ppbar
  1: nnbar
  2: Lambda0 anti-Lambda0
  3: Sigma0 anti-Sigma0
  4: Lambda0 anti-Sigma0
  5: Sigma0 anti-Lambda0
  6: pi+ pi-
  7: pi+ pi- pi0
  8: K+K- pi0
  9: KsK+pi- 
  10: KsK-pi+
  11: K+K-eta
  *************************************/
  int pp[2]={-2212,2212};
  int nn[2]={-2112,2112};
  int pipi[2]={-211,211};
  int pi3[3]={-211,111,211};
  int kkpi0[3]={-321,111,321}; //K+K-pi0
  int kskpi[3]={-211,310,321};//KsK+pi-

  std::vector<int> vson;vson.clear();
  for(int i=0;i<evtdaugs.size();i++){vson.push_back( EvtPDL::getStdHep(evtdaugs[i]) );}
  sort(vson.begin(),vson.end(),mysort);

  if(vson.size() ==2 ){
    if(vson[0]==pp[0] && vson[1]==pp[1] ) {return 0;}
    if(vson[0]==nn[0] && vson[1]==nn[1] ) {return 1;}
    if(vson[0]==pipi[0] && vson[1]==pipi[1] ) {return 2;}
  }else if(vson.size()==3){

  }

}
//----
double EvtXsection::getXsection(double mx){
  if(_mode == -1){return Xsection_c(mx);} 
  //else if(_mode == 0 || _mode == 2 || _mode == 3 || _mode == 4 ||_mode == 5){ return Xsection_a(mx);}
  else {return  Xsection_b(mx);}
}

double EvtXsection::getErr(double mx){
  //if(_mode <= 5 && _mode !=1 ){ return Err_a(mx);}
  //else {return  Err_b(mx);}
  return  Err_b(mx);
}
//----

double EvtXsection::Xsection_a(double mx){
  if (mx <= xx[0]) return 0.;
  if (mx > xx[nbins]) return 0.;
  int thebin = getXBin_a(mx);
  //-- debug
  //std::cout<<"Mode= "<<_mode<<", thebin"<<thebin<<std::endl;
  return yy[thebin]; 
}

double EvtXsection::Xsection_b(double mx){// interpolation of the cross section between two bins
  if (mx <= xx[0]) return 0.;
  if (mx > xx[nbins-1]) return 0.;
  int thebin = getXBin_b(mx);
  double ylow = yy[thebin];
  double yup  = yy[thebin+1];

  double xlow = xx[thebin];
  double xup  = xx[thebin+1]; 

  double yi=ylow + (yup-ylow)/(xup-xlow)*(mx-xlow);
  return yi;
}

//----
double EvtXsection::Err_a(double mx){
  if(_mode == -1) return 0.;
  if (mx <= xx[0]) return er[0];
  if (mx > xx[nbins]) return 0.;
  int thebin = getXBin_a(mx);
  //-- debug
  //std::cout<<"Mode= "<<_mode<<", thebin"<<thebin<<std::endl;
  return er[thebin]; 
}

double EvtXsection::Err_b(double mx){// interpolation of the cross section error between two bins
  if (mx <= xx[0]) return er[0];
  if (mx > xx[nbins-1]) return 0.;
  int thebin = getXBin_b(mx);
  double ylow = er[thebin];
  double yup  = er[thebin+1];

  double xlow = xx[thebin];
  double xup  = xx[thebin+1]; 

  double yi=ylow + (yup-ylow)/(xup-xlow)*(mx-xlow);
  return yi;
}

int EvtXsection::getXBin_a(double mx){
  if(mx <= xx[0]) return 0;
  if(mx > xx[nbins]) return nbins;
  for(int i=0;i<nbins;i++){
    if(mx <= xx[i+1] && mx > xx[i]) return i; //mx at i-th bin
  }
}

int EvtXsection::getXBin_b(double mx){
  if(mx <= xx[0]) return 0;
  if(mx > xx[nbins-1]) return nbins;
  for(int i=0;i<nbins-1;i++){
    if(mx <= xx[i+1] && mx > xx[i]) return i; //mx at i to i+1 bin
  }
}

int EvtXsection::getXBin(double mx,std::vector<double> vy){
  double nbins = vy.size();
  if(vy[0]<mx || mx<vy[nbins-1]) {std::cout<<"Outside vacuum pol. value"<<std::endl;abort();}
  for(int i=0;i<nbins-1;i++){
    if(mx <= vy[i+1] && mx > vy[i]) return i; //mx at i to i+1 bin
  }
}

double EvtXsection::Xsection_c(double mx){// in unit nb
   double pi=3.1415926;
   double s= mx*mx;
   EvtId pid = EvtPDL::evtIdFromStdHep(pdgcode );
   double mass = EvtPDL::getMeanMass(pid);
   double width = EvtPDL::getWidth(pid);
   double mass2 = mass*mass;
   double width2 = width*width;
   double br = (s - mass2)*(s - mass2) + mass2 * width2;
   bree = 1; //this value is canceled when calculation the correction factor;
   double sigma = 12*pi*bree*width2/br;
   double nbar = 4E05;  // ! GeV-2 = 4*10^5 nbar
   double thexs = sigma*nbar;
   // std::cout<<"EvtXsection::Xsection_c: "<<mass<<" "<<width<<" "<<thexs<<std::endl;
   // msg = "Calculate the correction factor for " + EvtPDL::name(pid);
   
   return thexs;
 }

void EvtXsection::setBW(int pdg){
  pdgcode = pdg;
  EvtId pid = EvtPDL::evtIdFromStdHep(pdgcode );
  double maxM= EvtPDL::getMaxMass(pid);
  double minM= EvtPDL::getMinMass(pid);
  double meanM = EvtPDL::getMeanMass(pid);
  double width = EvtPDL::getWidth(pid);
  // std::cout<<minM<<" <=M<= "<<maxM<<std::endl;
  double xstp=(maxM-minM)/100.;
  xx.clear();yy.clear();er.clear();
  for(int i=0;i<100;i++){
    double m=i*xstp;
    EvtComplex im(0.,1.);
    EvtComplex bw=1./(m*m-meanM*meanM+im*m*width);
    double amps = abs2(bw);
    xx.push_back(m); yy.push_back(amps); er.push_back(0.);
  }
  nbins=yy.size();
}

void EvtXsection::setModes(std::vector<int> vmd){
  _vmd.clear();
  for(int i=0;i<vmd.size();i++){
    _vmd.push_back(vmd[i]);
  }
}
