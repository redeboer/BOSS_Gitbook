//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006      Ping Rong-Gang, Pang Cai-Ying@IHEP
//
// Module: EvtPsi3Sdecay.cc
//
// Description: Routine to re-select the psi(4040) decay according the .
//              measured xsection at different energy point, see CLEOc measurement:
//              PRD 80, 072001   
// Modification history:
//
//    Ping R.-G.    December, 2010   Module created
//
//------------------------------------------------------------------------

#include "EvtGenModels/EvtPsi3Sdecay.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include <string>
#include <iostream>
#include <cmath>
int EvtPsi3Sdecay::psi3Scount=0;

  int  EvtPsi3Sdecay::findPoints(){
    if(Ecms < x[0] ){theLocation =0;} else
      if(Ecms >=x[nsize-1]) {theLocation = nsize-1;} else{
	for (int i=0;i<nsize-1;i++){
	  if( x[i] <= Ecms  && x[i+1] > Ecms) {theLocation=i;} 
	}
      }
    return theLocation;
}

double EvtPsi3Sdecay::polint(std::vector <double> vy ){

  theLocation = findPoints();
  double xs;
  if(theLocation==nsize-1){xs = vy[nsize-1];} else{
   xs = (vy[theLocation +1 ]- vy[theLocation]) / (x[theLocation+1]-x[theLocation])*(Ecms - x[theLocation])+vy[theLocation];
  }
  if(xs <0 ) xs = 0;
  //  for(int i=0;i<vy.size();i++){ std::cout<<"channel "<<i<<" xsection:  "<<vy[i]<<std::endl;}
  // std::cout<<"Ecms, theLocation= "<<Ecms<<" "<<theLocation<<" xs= "<<xs<<std::endl; 
  return xs;
 }

double EvtPsi3Sdecay::theProb(std::vector<double> myxs,int ich){
  int Nchannels=myxs.size();
  //---debuging
  // std::cout<<"Nchannel= "<<Nchannels<<endl;
  
  std::vector <double> thexs;
  thexs.clear();
  double sumxs=0;
  for(int j=0;j<Nchannels;j++){
    sumxs += myxs[j];  
    thexs.push_back(sumxs);
    //----debugging
    // std::cout<<"thexs["<<j<<"]= "<<myxs[j]<<std::endl;
  }
  if(sumxs == 0) {std::cout<<"EvtPsi3Sdecay::theProb, denominator is 0"<<std::endl;::abort();}
  return thexs[ich] / sumxs ;  
}

int EvtPsi3Sdecay::findMode(){
  // mode index: 0) D0D0bar, 1)D+D-; 2)D0D*0bar , 3)D0bar D*0, 4)D*0 D*0 bar, 5)D*+D-  6)D*-D+  7)D*+ D*- 8) Ds+ Ds-
  // more modes: 9) D_s^*+  D_s^-  ;10) D_s^*-  D_s^+;        11) D_s^*+  D_s^*- 

    std::string son0,son1,son2;
    Vson.clear();
    Vid.clear();
    for(int i=0;i<Ndaugs;i++){
      std::string nson=EvtPDL::name(theParent->getDaug(i)->getId());
      if(nson!="gammaFSR" && nson!="gamma"){ Vson.push_back(nson);Vid.push_back(theParent->getDaug(i)->getId());}
    }
    int nh=Vson.size();
    //debugging
    //std::cout<<"nh= "<<nh<<" "<<Vson[0]<<" "<<Vson[1]<<" "<<Vson[2]<<std::endl;
    //theParent->printTree();

  if(nh == 2){
    //std::cout<<"2 final states: "<<Vson[0]<<" "<<Vson[1]<<std::endl;
    son0 = Vson[0];son1 = Vson[1];
    if(son0 == "D0"      && son1 == "anti-D0"  || son1 == "D0"      && son0 == "anti-D0")  {return 0;} else
    if(son0 == "D+"      && son1 == "D-"       || son1 == "D+"      && son0 == "D-"     )  {return 1;} else
    if(son0 == "D0"      && son1 == "anti-D*0" || son1 == "D0"      && son0 == "anti-D*0") {return 2;} else
    if(son0 == "anti-D0" && son1 == "D*0"      || son1 == "anti-D0" && son0 == "D*0")      {return 3;} else
    if(son0 == "D*0"     && son1 == "anti-D*0" || son1 == "D*0"     && son0 == "anti-D*0") {return 4;} else
    if(son0 == "D*+"     && son1 == "D-"       || son1 == "D*+"     && son0 == "D-")       {return 5;} else
    if(son0 == "D*-"     && son1 == "D+"       || son1 == "D*-"     && son0 == "D+")       {return 6;} else
    if(son0 == "D*+"     && son1 == "D*-"      || son1 == "D*+"     && son0 == "D*-")      {return 7;} else
    if(son0 == "D_s+"    && son1 == "D_s-"     || son1 == "D_s+"    && son0 == "D_s-")     {return 8;} else
    if(son0 == "D_s*+"   && son1 == "D_s-"     || son1 == "D_s*+"   && son0 == "D_s-")     {return 9;} else
    if(son0 == "D_s*-"   && son1 == "D_s+"     || son1 == "D_s*-"   && son0 == "D_s+")     {return 10;}else
    if(son0 == "D_s*+"   && son1 == "D_s*-"    || son1 == "D_s*+"   && son0 == "D_s*-")    {return 11;}else {goto ErrInfo;}
  } else if(nh == 3){
    //std::cout<<"3 final states: "<<Vson[0]<<" "<<Vson[1]<<" "<<Vson[2]<<std::endl;
    son0 = Vson[0];son1 = Vson[1];son2 = Vson[2];
   if(son0 == "D*+"     && son1 == "D-"       && son2 == "pi0"  )  {return 12;} else
   if(son0 == "D*-"     && son1 == "D+"       && son2 == "pi0"  )  {return 13;} else
   if(son0 == "D*+"     && son1 == "anti-D0"  && son2 == "pi-"  )  {return 14;} else
   if(son0 == "D*-"     && son1 == "D0"       && son2 == "pi+"  )  {return 15;} else
   if(son0 == "D+"      && son1 == "anti-D*0" && son2 == "pi-"  )  {return 16;} else
   if(son0 == "D-"      && son1 == "D*0"      && son2 == "pi+"  )  {return 17;} else
   if(son0 == "D*+"     && son1 == "D*-"      && son2 == "pi0"  )  {return 18;} else
   if(son0 == "anti-D*0" &&son1 == "D*+"      && son2 == "pi-"  )  {return 19;} else
   if(son0 == "D*0"     && son1 == "D*-"      && son2 == "pi+"  )  {return 20;} else
   if(son0 == "D*0"     && son1 == "anti-D*0" && son2 == "pi0"  )  {return 21;} else
   if(son0 == "D0"      && son1 == "anti-D*0" && son2 == "pi0"  )  {return 22;} else
   if(son0 == "anti-D0" && son1 == "D*0"      && son2 == "pi0"  )  {return 23;} else {goto ErrInfo;}
  }
  ErrInfo:
   std::cout<<"Not open charm decay"<<std::endl;
   std::cout<<"final states \"";
   for(int j=0;j<nh;j++){
     std::cout<<Vson[j]<<" ";
   }
   std::cout<<" \" is not in the Psi3Decay list, see  $BESEVTGENROOT/src/EvtGen/EvtGenModels/EvtPsi3Sdecay.hh"<<std::endl;
   ::abort();
   
   
}
 

 bool EvtPsi3Sdecay::choseDecay(){ //determing accept or reject a generated decay

   // findPoints();
 double d0d0bar_xs=polint(d0d0bar);
 double dpdm_xs   = polint(dpdm);
 double d0dst0bar_xs = polint(d0dst0bar);
 double d0bardst0_xs = polint(d0bardst0);

 double dst0dst0bar_xs = polint(dst0dst0bar);
 double dstpdm_xs = polint(dstpdm);

 double dstmdp_xs = polint(dstmdp);
 double dstpdstm_xs = polint(dstpdstm);

 double dspdsm_xs = polint(dspdsm);

 double dsspdsm_xs = polint(dsspdsm);
 double dssmdsp_xs = polint(dssmdsp);

 double dsspdssm_xs = polint(dsspdssm);
 //--- DDpi modes
 double _xs12    = polint(xs12);
 double _xs13    = polint(xs13);
 double _xs14    = polint(xs14);
 double _xs15    = polint(xs15);
 double _xs16    = polint(xs16);
 double _xs17    = polint(xs17);
 double _xs18    = polint(xs18);
 double _xs19    = polint(xs19);
 double _xs20    = polint(xs20);
 double _xs21    = polint(xs21);
 double _xs22    = polint(xs22);
 double _xs23    = polint(xs23);
 
 int ich = findMode();
 // std::cout<<"calculated XS "<< d0d0bar_xs<<" "<<dpdm_xs<<" "<<d0dst0bar_xs<<" "<<d0bardst0_xs<< " "<<dst0dst0bar_xs<<" "<<dstpdm_xs<< " "<<dstmdp_xs<<" "<<dstpdstm_xs<<" "<<dspdsm_xs<<std::endl;
 

 double xmtotal=0;
 for(int i=0;i<Vid.size();i++){
   xmtotal += EvtPDL::getMinMass(Vid[i]);
 } 
 double mparent= theParent->getP4().mass();
 // std::cout<<"mparent= "<<mparent<<", xmtotal= "<<xmtotal<<endl;
 if (mparent<xmtotal){return false;}


 std::vector<double> myxs; myxs.clear();
 myxs.push_back(d0d0bar_xs);  //0
 myxs.push_back(dpdm_xs);
 myxs.push_back(d0dst0bar_xs); //2
 myxs.push_back(d0bardst0_xs);
 myxs.push_back(dst0dst0bar_xs); //4
 myxs.push_back(dstpdm_xs);
 myxs.push_back(dstmdp_xs);     //6
 myxs.push_back(dstpdstm_xs);
 myxs.push_back(dspdsm_xs);   //8
 myxs.push_back(dsspdsm_xs);
 myxs.push_back(dssmdsp_xs);   //10
 myxs.push_back(dsspdssm_xs);  //11

 myxs.push_back(_xs12);         //12        
 myxs.push_back(_xs13);         //13        
 myxs.push_back(_xs14);         //14        
 myxs.push_back(_xs15);         //15        
 myxs.push_back(_xs16);         //16        
 myxs.push_back(_xs17);         //17        
 myxs.push_back(_xs18);         //18        
 myxs.push_back(_xs19);         //19        
 myxs.push_back(_xs20);         //20
 myxs.push_back(_xs21);         //21        
 myxs.push_back(_xs22);         //22        
 myxs.push_back(_xs23);         //23

 double Prop0,Prop1;
 if(ich==0){ Prop0=0;} else
   {
     Prop0 = theProb(myxs,ich-1);
   }
 Prop1 = theProb(myxs,ich);

 double pm= EvtRandom::Flat(0.,1);
 bool flag = false;
 if( Prop0 < pm && pm<= Prop1 ) flag = true;

 //--- debuging

 if(flag) {
   //std::cout<<"findMode= "<<ich<<std::endl;
   //for(int i=0;i<myxs.size();i++){ std::cout<<"channel "<<i<<" myxs:  "<<myxs[i]<<std::endl;}
   //std::cout<<"prop0,prop1= "<<Prop0<<" "<<Prop1<<std::endl;
 }

 //-------------

 return flag;
}
//---

EvtParticle* EvtPsi3Sdecay::choseDecay(EvtParticle * par){ //decay par
  double  xm = par->mass();
  int themode = getDecay(xm);
  std::vector< EvtId > theid = getVId(themode);
  int ndaugjs = theid.size();
  EvtId  myId[3];
  for(int i=0;i<ndaugjs;i++){myId[i]=theid[i];}
  par->makeDaughters(ndaugjs,myId);

  for(int i=0;i<par->getNDaug();i++){
    EvtParticle* di=par->getDaug(i);
    double xmi=EvtPDL::getMinMass(di->getId());
    di->setMass(xmi);
   }
  par->initializePhaseSpace(ndaugjs,myId);
  _themode = themode;
  return par;
}


//--

void EvtPsi3Sdecay::PHSPDecay(EvtParticle * par){//decay par
  v_p4.clear();Vid.clear();
  double  xm = par->mass();
  EvtVector4R p_ini(xm,0,0,0);
  EvtParticle* mypar= EvtParticleFactory::particleFactory(par->getId(),p_ini);

  int themode = getDecay(xm);
  std::vector< EvtId > theid = getVId(themode);
  int ndaugjs = theid.size();
  EvtId  myId[3];
  for(int i=0;i<ndaugjs;i++){myId[i]=theid[i];}
  mypar->makeDaughters(ndaugjs,myId);

  for(int i=0;i<mypar->getNDaug();i++){
    EvtParticle* di=mypar->getDaug(i);
    double xmi=EvtPDL::getMinMass(di->getId());
    di->setMass(xmi);
   }
 loop:
   mypar->initializePhaseSpace(ndaugjs,myId);
  
  //-- here to do amgular distribution sampling
   bool pp = (themode == 0||themode == 1 ||themode ==6);  //V->PP mode, alpha = -1
   bool vp = (themode >=2 && themode <=5 || themode >=7 && themode <=9 ); // V->VP mode, alpha = 1
   bool flag1 = false;
   double alpha;
   if(pp){alpha=-1;}else if(vp){alpha=1;} else {alpha=0;}
   EvtVector4R pp4=par->getP4();
   EvtVector4R sp4=mypar->getDaug(1)->getP4();
   flag1=AngSam(pp4,sp4,alpha);
   if(alpha != 0 && !flag1 ) goto loop;
  //--
   _themode = themode;
  for(int i=0;i<mypar->getNDaug();i++){
    EvtParticle* di=mypar->getDaug(i);
    v_p4.push_back( di->getP4() );
    Vid.push_back(myId[i]);
    // std::cout<<"Vid "<<EvtPDL::name(Vid[i])<<" p4: "<<v_p4[i]<<std::endl;
   }
  

  /*********** same function can be realized 
  double mass[3];
  EvtVector4R p4[30];
  for(int i=0;i<ndaugjs;i++){mass[i]=mypar->getDaug(i)->mass();}
  EvtGenKine::PhaseSpace(ndaugjs,mass,p4,xm);
  _themode = themode;
  for(int i=0;i<mypar->getNDaug();i++){
    v_p4.push_back( p4[i] );
    Vid.push_back(myId[i]);
    // std::cout<<"Vid "<<EvtPDL::name(Vid[i])<<" p4: "<<v_p4[i]<<std::endl;
   }
  *************/
  mypar->deleteTree();
  return;
}


//--

 int EvtPsi3Sdecay::getDecay(double ecms){ //pick up a decay by the accept-reject sampling method
   if(ecms<3.97 || ecms >4.66){std::cout<<"EvtPsi3Sdecay::getDecay: You need to set the CMS energy between 3.97~4.66 GeV, but you have ecms= "<<ecms<< " GeV.The lower end can be set at KKMC"<<std::endl; }
   if(_excflag ==1) return _themode;
   Ecms = ecms;
   // findPoints();
 double d0d0bar_xs=polint(d0d0bar);
 double dpdm_xs   = polint(dpdm);
 double d0dst0bar_xs = polint(d0dst0bar);
 double d0bardst0_xs = polint(d0bardst0);

 double dst0dst0bar_xs = polint(dst0dst0bar);
 double dstpdm_xs = polint(dstpdm);

 double dstmdp_xs = polint(dstmdp);
 double dstpdstm_xs = polint(dstpdstm);

 double dspdsm_xs = polint(dspdsm);

 double dsspdsm_xs = polint(dsspdsm);
 double dssmdsp_xs = polint(dssmdsp);

 double dsspdssm_xs = polint(dsspdssm);

 double _xs12    = polint(xs12);
 double _xs13    = polint(xs13);
 double _xs14    = polint(xs14);
 double _xs15    = polint(xs15);
 double _xs16    = polint(xs16);
 double _xs17    = polint(xs17);
 double _xs18    = polint(xs18);
 double _xs19    = polint(xs19);
 double _xs20    = polint(xs20);
 double _xs21    = polint(xs21);
 double _xs22    = polint(xs22);
 double _xs23    = polint(xs23);
 

 std::vector<double> myxs; myxs.clear();
 myxs.push_back(d0d0bar_xs);    //0 
 myxs.push_back(dpdm_xs);       //1
 myxs.push_back(d0dst0bar_xs);  //2
 myxs.push_back(d0bardst0_xs);  //3
 myxs.push_back(dst0dst0bar_xs);//4
 myxs.push_back(dstpdm_xs);     //5
 myxs.push_back(dstmdp_xs);     //6
 myxs.push_back(dstpdstm_xs);   //7
 myxs.push_back(dspdsm_xs);     //8
 myxs.push_back(dsspdsm_xs);    //9
 myxs.push_back(dssmdsp_xs);    //10
 myxs.push_back(dsspdssm_xs);   //11
 myxs.push_back(_xs12);         //12        
 myxs.push_back(_xs13);         //13        
 myxs.push_back(_xs14);         //14        
 myxs.push_back(_xs15);         //15        
 myxs.push_back(_xs16);         //16        
 myxs.push_back(_xs17);         //17        
 myxs.push_back(_xs18);         //18        
 myxs.push_back(_xs19);         //19        
 myxs.push_back(_xs20);         //20   
 myxs.push_back(_xs21);         //21        
 myxs.push_back(_xs22);         //22        
 myxs.push_back(_xs23);         //23       

 double mytotxs=0;
 for(int i=0;i<myxs.size();i++){mytotxs += myxs[i];}
 if(psi3Scount==0) {std::cout<<"The total xs at "<<ecms<<" is: "<<mytotxs<<" pb"<<std::endl;psi3Scount++;} 
 int niter = 0;
 loop:
 int ich = (int)24*EvtRandom::Flat(0.,1);//sampling modes over 24 channel

 niter++;
 if(niter>1000) {std::cout<<"EvtPsi3Sdecay:getDecay() do loops over 1000 times at Ecm= "<<ecms<<std::endl;abort();}

 double xmtotal=0;
 std::vector<EvtId> theVid;
 theVid.clear();
 theVid = getVId(ich);
 for(int i=0;i<theVid.size();i++){
   xmtotal += EvtPDL::getMinMass(theVid[i]);
 } 

 if(Ecms < xmtotal ) {goto loop;}

 double Prop0,Prop1;
 if(ich==0){ Prop0=0;} else
   {
     Prop0 = theProb(myxs,ich-1);
   }
 Prop1 = theProb(myxs,ich);

 double pm= EvtRandom::Flat(0.,1);

 if( Prop0 < pm && pm<= Prop1 ) {return ich;}
 else {goto loop;}

}

//----
std::vector<EvtId> EvtPsi3Sdecay::getVId(int mode){
  std::vector<EvtId> theVid;
  theVid.clear();
  for(int i=0;i<VmodeSon[mode].size();i++){
    EvtId theId = EvtPDL::getId(VmodeSon[mode][i]);
    theVid.push_back(theId);
  }
  return theVid;
}


bool EvtPsi3Sdecay::AngSam(EvtVector4R parent_p4cm,EvtVector4R son_p4cm,double alpha){
  EvtHelSys angles(parent_p4cm,son_p4cm);  
  double theta=angles.getHelAng(1);
  //double phi  =angles.getHelAng(2);
  //double gamma=0;
  double costheta=cos(theta);  //using helicity angles in parent system
  double max_alpha;
  if(alpha>=0) {max_alpha = 1+alpha;}else
    {max_alpha=1;}
  double ags = (1+alpha*costheta*costheta)/max_alpha;
  double rand=EvtRandom::Flat(0.0, 1.0);
  if(rand <=ags) {return true;}
  else {return false;}
}
