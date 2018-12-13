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
// Module: EvtPsi3Sdecay.hh
//
// Description: Routine to re-select the psi(4040) decay according the .
//              measured xsection at different energy point, see CLEOc measurement:
//              PRD 80, 072001   
// Modification history:
//
//    Ping R.-G.    December, 2010   Module created
//
//------------------------------------------------------------------------
#ifndef EVTPsi3Sdecay_HH
#define EVTPsi3Sdecay_HH

#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtPolInt.hh"
#include <stdlib.h>
#include <string>
#include <vector>
using std::endl;

class EvtPsi3Sdecay{
public:
  // constructor
  EvtPsi3Sdecay(double ecms, EvtParticle *parent) {  //for 2-body decays
  //initializer
  Ecms = ecms;
  theParent = parent;
  Ndaugs=parent->getNDaug();
  nsize = 32;

  _excflag=0;  
  x.clear();
  // open charm cross section, see PRD 80, 072001, xs in unit pb.
  double xx[32]={3.72968, 3.73922, 3.87180, 3.87987, 3.93660, 3.97,  3.99, 4.01, 4.01392, 4.015, 4.02052, 4.03, 4.06, 4.08040, 4.12, 4.14, 4.16, 4.17, 4.18, 4.2,  4.22420,  4.26, 4.30, 4.34, 4.38, 4.42, 4.46, 4.50, 4.54, 4.58, 4.62, 4.66 }; // 32 energy points
  double y0[32]={0.,      3  ,     51,      54,      74,      86,    133,  76,   23,      10,    139,     334,  410,  374,     303,  177,  167,  177,  179,  180,  142,      86,  31,    49,   65,   196,  52,   87,   166,  14,   33,   49};  // 0) D0D0bar cross section
  double y1[32]={0.,      0.,      81,      86,      118,     137,   90,   135,  57,      38,    101,     196,  480,  423,     310,  200,  200,  182,  197,  181,  146,      94,   108,  96,   154,  165,  171,  106,  27,   144,  36,   22};  // 1) D+D-
  double y2[32]={0.,      0.,      0.,      713,     983,     1140,  1370, 1660, 1868,    1920,  1792,    1600, 1115, 976,     700,  675,  626,  636,  605,  515,  525,      540,  748,  880,  556,  657,  477,  494,  320,  616,  575,  373}; // 2)D0D*0bar
  double y3[32]={0.,      0.,      0.,      713,     983,     1140,  1370, 1660, 1868,    1920,  1792,    1600, 1115, 976,     700,  675,  626,  636,  605,  515,  525,      540,  748,  880,  556,  657,  477,  494,  320,  616,  575,  373}; // 3)D0bar D*0
  double y4[32]={0.,      0.,      0.,      0,       0,       0,     0,    0,    0,       213,   928,     2000, 2290, 2376,    2550, 2443, 2566, 2363, 2173, 1830, 1205,     269,  822,  1045, 1020, 820,  398,  588,  690,  459,  360,  398}; // 4)D*0 D*0bar
  double y5[32]={0.,      0.,      0.,      0,       962,     1115,  1375, 1650, 1810,    1851,  1770,    1650, 1085, 983,     780,  688,  688,  642,  648,  535,  525,      511,  748,  880,  556,  657,  477,  494,  320,  616,  575,  373}; // 5)D*+D-
  double y6[32]={0.,      0.,      0.,      0,       962,     1115,  1375, 1650, 1810,    1851,  1770,    1650, 1085, 983,     780,  688,  688,  642,  648,  535,  525,      511,  748,  880,  556,  657,  477,  494,  320,  616,  575,  373}; // 6)D*-D+
  double y7[32]={0.,      0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       1400, 2390, 2353,    2280, 2556, 2479, 2357, 2145, 1564, 1033,     237,  822,  1045, 1020, 820,  398,  588,  690,  459,  360,  398}; // 7)D*+D*-
  double y8[32]={0.,      0.,      0.,      0,       0,       102,   133,  269,  254,     250,   219,     174,  51,   42,      26,   25,   15,   34,   7,    15,   28,       47,   106,  70,   36,   10,   2,    28,   60,   60,   48,   36};  // 8)Ds+ Ds-
  double y9[32]={0.,      0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       239,  342,  452.5,458,  444.5,406,  250,      17,   157,  184,  159,  178.5,146,  85.5, 33,   51.5, 95,   136}; // 9)Ds*+ Ds-
  double y10[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       239,  342,  452.5,458,  444.5,406,  250,      17,   157,  184,  159,  178.5,146,  85.5, 33,   51.5,  95,  136}; //10)Ds*- Ds+
  double y11[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        440,  398,  428,  310,  131,  0,    45,   126,  98,   39,   0};   //11)Ds*+ Ds*-
  double y12[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    12,   9,       3.8,  32.4, 32.4, 37,   48,   61.2, 58,       53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //12)D*+ D- pi0 //------ DD* pi----
  double y13[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    12,   9,       3.8,  32.4, 32.4, 37,   48,   61.2, 58,       53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //13)D*- D+ pi0
  double y14[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    24,   18,      7.5,  68.6, 64.8, 73.3, 95.8, 122.5,116,      106.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110}; //14)D*+ anti-D0 pi-
  double y15[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    24,   18,      7.5,  68.6, 64.8, 73.3, 95.8, 122.5,116,      106.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110}; //15)D*- D0 pi+
  double y16[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    24,   18,      7.5,  68.6, 64.8, 73.3, 95.8, 122.5,116,      106.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110}; //16)D+ anti-D*0 pi-
  double y17[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    24,   18,      7.5,  68.6, 64.8, 73.3, 95.8, 122.5,116,      106.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110}; //17)D- D*0 pi+
  double y18[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //18) D*+ D*- pi0 //------D*D*pi, above 4.26Gev, assumed xs as D*D pi
  double y19[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        107.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110 };//19) anti-D*0 D*+ pi-
  double y20[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        107.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110 };//20) D*0 D*- pi+
  double y21[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //21) D*0 D*0bar pi0
  double y22[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    12,   9,       3.8,  32.4, 32.4, 37,   48,   61.2, 58,       53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //22)D*0 D0bar pi0 //------ DD* pi----
  double y23[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    12,   9,       3.8,  32.4, 32.4, 37,   48,   61.2, 58,       53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //23)D*bar D0  pi0
  d0d0bar.clear();
  dpdm.clear();
  d0dst0bar.clear();
  dst0dst0bar.clear();
  d0bardst0.clear();
  dstpdm.clear();
  dstmdp.clear();
  dstpdstm.clear();
  dspdsm.clear();
  dsspdsm.clear();
  dssmdsp.clear();
  dsspdssm.clear();
  xs12.clear();
  xs13.clear();
  xs14.clear();
  xs15.clear();
  xs16.clear();
  xs17.clear();
  xs18.clear();
  xs19.clear();
  xs20.clear();
  xs21.clear();
  xs22.clear();
  xs23.clear();

  for(int i=0;i<32;i++){
    x.push_back(xx[i]);
    d0d0bar.push_back(y0[i]);
    dpdm.push_back(y1[i]);
    d0dst0bar.push_back(y2[i]);
    d0bardst0.push_back(y3[i]);
    dst0dst0bar.push_back(y4[i]);
    dstpdm.push_back(  y5[i]);
    dstmdp.push_back(  y6[i]);
    dstpdstm.push_back(y7[i]);
    dspdsm.push_back(  y8[i]);
    dsspdsm.push_back( y9[i]);
    dssmdsp.push_back( y10[i]);
    dsspdssm.push_back( y11[i]);
    xs12.push_back( y12[i] );   
    xs13.push_back( y13[i] );   
    xs14.push_back( y14[i] );   
    xs15.push_back( y15[i] );   
    xs16.push_back( y16[i] );   
    xs17.push_back( y17[i] );   
    xs18.push_back( y18[i] );   
    xs19.push_back( y19[i] );   
    xs20.push_back( y20[i] ); 
    xs21.push_back( y21[i] );   
    xs22.push_back( y22[i] );   
    xs23.push_back( y23[i] ); 
  }
 }

//---
  EvtPsi3Sdecay() {//for 2-body and 3-body decays
  //initializer
  //  Ecms = ecms;
  nsize = 32;
 
  x.clear();
  // open charm cross section, see PRD 80, 072001, xross section in pb
  double xx[32]={3.72968, 3.73922, 3.87180, 3.87987, 3.93660, 3.97,  3.99, 4.01, 4.01392, 4.015, 4.02052, 4.03, 4.06, 4.08040, 4.12, 4.14, 4.16, 4.17, 4.18, 4.2,  4.22420,  4.26, 4.30, 4.34, 4.38, 4.42, 4.46, 4.50, 4.54, 4.58, 4.62, 4.66 }; // 32 energy points
  double y0[32]={0.,      3  ,     51,      54,      74,      86,    133,  76,   23,      10,    139,     334,  410,  374,     303,  177,  167,  177,  179,  180,  142,      86,  31,    49,   65,   196,  52,   87,   166,  14,   33,   49};  // 0) D0D0bar cross section
  double y1[32]={0.,      0.,      81,      86,      118,     137,   90,   135,  57,      38,    101,     196,  480,  423,     310,  200,  200,  182,  197,  181,  146,      94,   108,  96,   154,  165,  171,  106,  27,   144,  36,   22};  // 1) D+D-
  double y2[32]={0.,      0.,      0.,      713,     983,     1140,  1370, 1660, 1868,    1920,  1792,    1600, 1115, 976,     700,  675,  626,  636,  605,  515,  525,      540,  748,  880,  556,  657,  477,  494,  320,  616,  575,  373}; // 2)D0D*0bar
  double y3[32]={0.,      0.,      0.,      713,     983,     1140,  1370, 1660, 1868,    1920,  1792,    1600, 1115, 976,     700,  675,  626,  636,  605,  515,  525,      540,  748,  880,  556,  657,  477,  494,  320,  616,  575,  373}; // 3)D0bar D*0
  double y4[32]={0.,      0.,      0.,      0,       0,       0,     0,    0,    0,       213,   928,     2000, 2290, 2376,    2550, 2443, 2566, 2363, 2173, 1830, 1205,     269,  822,  1045, 1020, 820,  398,  588,  690,  459,  360,  398}; // 4)D*0 D*0bar
  double y5[32]={0.,      0.,      0.,      0,       962,     1115,  1375, 1650, 1810,    1851,  1770,    1650, 1085, 983,     780,  688,  688,  642,  648,  535,  525,      511,  748,  880,  556,  657,  477,  494,  320,  616,  575,  373}; // 5)D*+D-
  double y6[32]={0.,      0.,      0.,      0,       962,     1115,  1375, 1650, 1810,    1851,  1770,    1650, 1085, 983,     780,  688,  688,  642,  648,  535,  525,      511,  748,  880,  556,  657,  477,  494,  320,  616,  575,  373}; // 6)D*-D+
  double y7[32]={0.,      0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       1400, 2390, 2353,    2280, 2556, 2479, 2357, 2145, 1564, 1033,     237,  822,  1045, 1020, 820,  398,  588,  690,  459,  360,  398}; // 7)D*+D*-
  double y8[32]={0.,      0.,      0.,      0,       0,       102,   133,  269,  254,     250,   219,     174,  51,   42,      26,   25,   15,   34,   7,    15,   28,       47,   106,  70,   36,   10,   2,    28,   60,   60,   48,   36};  // 8)Ds+ Ds-
  double y9[32]={0.,      0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       239,  342,  452.5,458,  444.5,406,  250,      17,   157,  184,  159,  178.5,146,  85.5, 33,   51.5, 95,   136}; // 9)Ds*+ Ds-
  double y10[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       239,  342,  452.5,458,  444.5,406,  250,      17,   157,  184,  159,  178.5,146,  85.5, 33,   51.5,  95,  136}; //10)Ds*- Ds+
  double y11[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        440,  398,  428,  310,  131,  0,    45,   126,  98,   39,   0};   //11)Ds*+ Ds*-
  double y12[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    12,   9,       3.8,  32.4, 32.4, 37,   48,   61.2, 58,       53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //12)D*+ D- pi0 //------ DD* pi----
  double y13[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    12,   9,       3.8,  32.4, 32.4, 37,   48,   61.2, 58,       53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //13)D*- D+ pi0
  double y14[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    24,   18,      7.5,  68.6, 64.8, 73.3, 95.8, 122.5,116,      106.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110}; //14)D*+ anti-D0 pi-
  double y15[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    24,   18,      7.5,  68.6, 64.8, 73.3, 95.8, 122.5,116,      106.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110}; //15)D*- D0 pi+
  double y16[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    24,   18,      7.5,  68.6, 64.8, 73.3, 95.8, 122.5,116,      106.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110}; //16)D+ anti-D*0 pi-
  double y17[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    24,   18,      7.5,  68.6, 64.8, 73.3, 95.8, 122.5,116,      106.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110}; //17)D- D*0 pi+
  double y18[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //18) D*+ D*- pi0 //------D*D*pi, above 4.26Gev, assumed xs as D*D pi
  double y19[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        107.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110 };//19) anti-D*0 D*+ pi-
  double y20[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        107.3,98,   378,  650,  564,  167,  304,  359,  381,  497,  110 };//20) D*0 D*- pi+
  double y21[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    0,    0,       0,    0,    0,    0,    0,    0,    0,        53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //21) D*0 D*0bar pi0
  double y22[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    12,   9,       3.8,  32.4, 32.4, 37,   48,   61.2, 58,       53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //22)D*0 D0bar pi0 //------ DD* pi----
  double y23[32]={0.,     0.,      0.,      0,       0,       0,     0,    0,    0,       0,     0,       0,    12,   9,       3.8,  32.4, 32.4, 37,   48,   61.2, 58,       53.6, 49,   169,  325,  282,  83.5, 152,  179,  190,  248,  55 }; //23)D*bar D0  pi0
  d0d0bar.clear();
  dpdm.clear();
  d0dst0bar.clear();
  dst0dst0bar.clear();
  d0bardst0.clear();
  dstpdm.clear();
  dstmdp.clear();
  dstpdstm.clear();
  dspdsm.clear();
  dsspdsm.clear();
  dssmdsp.clear();
  dsspdssm.clear();
  xs12.clear();
  xs13.clear();
  xs14.clear();
  xs15.clear();
  xs16.clear();
  xs17.clear();
  xs18.clear();
  xs19.clear();
  xs20.clear();
  xs21.clear();
  xs22.clear();
  xs23.clear();
  for(int i=0;i<32;i++){
    x.push_back(xx[i]);
    d0d0bar.push_back(y0[i]);
    dpdm.push_back(y1[i]);
    d0dst0bar.push_back(y2[i]);
    d0bardst0.push_back(y3[i]);
    dst0dst0bar.push_back(y4[i]);
    dstpdm.push_back(  y5[i]);
    dstmdp.push_back(  y6[i]);
    dstpdstm.push_back(y7[i]);
    dspdsm.push_back(  y8[i]);
    dsspdsm.push_back( y9[i]);
    dssmdsp.push_back( y10[i]);
    dsspdssm.push_back( y11[i]);
    xs12.push_back( y12[i] );   
    xs13.push_back( y13[i] );   
    xs14.push_back( y14[i] );   
    xs15.push_back( y15[i] );   
    xs16.push_back( y16[i] );   
    xs17.push_back( y17[i] );   
    xs18.push_back( y18[i] );   
    xs19.push_back( y19[i] );   
    xs20.push_back( y20[i] );  
    xs21.push_back( y21[i] );   
    xs22.push_back( y22[i] );   
    xs23.push_back( y23[i] );    
  }

//---- initilize Vmode
 
  VmodeSon.clear();
  //0: D0 anti-D0
  Vson.clear();
  Vson.push_back("D0"); Vson.push_back("anti-D0"); 
  VmodeSon.push_back(Vson);

  //1: D+ D-
  Vson.clear();
  Vson.push_back("D+"); Vson.push_back("D-"); 
  VmodeSon.push_back(Vson);

  //2: D0 anti-D*0
  Vson.clear();
  Vson.push_back("D0"); Vson.push_back("anti-D*0"); 
  VmodeSon.push_back(Vson);

  //3: anti-D0 D*0
  Vson.clear();
  Vson.push_back("anti-D0"); Vson.push_back("D*0"); 
  VmodeSon.push_back(Vson);

  //4: D*0 anti-D*0
  Vson.clear();
  Vson.push_back("D*0"); Vson.push_back("anti-D*0"); 
  VmodeSon.push_back(Vson);

  //5: D*+ D-
  Vson.clear();
  Vson.push_back("D*+"); Vson.push_back("D-"); 
  VmodeSon.push_back(Vson);

  //6: D*- D+
  Vson.clear();
  Vson.push_back("D*-"); Vson.push_back("D+"); 
  VmodeSon.push_back(Vson);

  //7: D*+ D*-
  Vson.clear();
  Vson.push_back("D*+"); Vson.push_back("D*-"); 
  VmodeSon.push_back(Vson);

  //8: D_s+ D_s-
  Vson.clear();
  Vson.push_back("D_s+"); Vson.push_back("D_s-"); 
  VmodeSon.push_back(Vson);

  //9: D_s*+ D_s-
  Vson.clear();
  Vson.push_back("D_s*+"); Vson.push_back("D_s-"); 
  VmodeSon.push_back(Vson);

  //10: D_s*- D_s+
  Vson.clear();
  Vson.push_back("D_s*-"); Vson.push_back("D_s+"); 
  VmodeSon.push_back(Vson);

  //11: D_s*+ D_s*-
  Vson.clear();
  Vson.push_back("D_s*+"); Vson.push_back("D_s*-"); 
  VmodeSon.push_back(Vson);

  //12: D*+ D- pi0
  Vson.clear();
  Vson.push_back("D*+"); Vson.push_back("D-");Vson.push_back("pi0"); 
  VmodeSon.push_back(Vson);

  //13: D*- D+ pi0
  Vson.clear();
  Vson.push_back("D*-"); Vson.push_back("D+");Vson.push_back("pi0"); 
  VmodeSon.push_back(Vson);

  //14: D*+ anti-D0 pi-
  Vson.clear();
  Vson.push_back("D*+"); Vson.push_back("anti-D0");Vson.push_back("pi-"); 
  VmodeSon.push_back(Vson);

  //15: D*- D0 pi+
  Vson.clear();
  Vson.push_back("D*-"); Vson.push_back("D0");Vson.push_back("pi+"); 
  VmodeSon.push_back(Vson);

  //16: D+ anti-D*0 pi-
  Vson.clear();
  Vson.push_back("D+"); Vson.push_back("anti-D*0");Vson.push_back("pi-"); 
  VmodeSon.push_back(Vson);

  //17: D- D*0 pi+
  Vson.clear();
  Vson.push_back("D-"); Vson.push_back("D*0");Vson.push_back("pi+"); 
  VmodeSon.push_back(Vson);

  //18: D*+ D*- pi0
  Vson.clear();
  Vson.push_back("D*+"); Vson.push_back("D*-");Vson.push_back("pi0"); 
  VmodeSon.push_back(Vson);

  //19: anti-D*0 D*+ pi-
  Vson.clear();
  Vson.push_back("anti-D*0"); Vson.push_back("D*+");Vson.push_back("pi-"); 
  VmodeSon.push_back(Vson);

  //20: D*0 D*- pi+
  Vson.clear();
  Vson.push_back("D*0"); Vson.push_back("D*-");Vson.push_back("pi+"); 
  VmodeSon.push_back(Vson);

  //21: D*0 D*0bar pi0
  Vson.clear();
  Vson.push_back("D*0"); Vson.push_back("anti-D*0");Vson.push_back("pi0"); 
  VmodeSon.push_back(Vson);

  //22: D0bar D*0 pi0
  Vson.clear();
  Vson.push_back("anti-D0"); Vson.push_back("D*0");Vson.push_back("pi0"); 
  VmodeSon.push_back(Vson);

  //23: D*0bar D0 pi0
  Vson.clear();
  Vson.push_back("anti-D*0"); Vson.push_back("D0");Vson.push_back("pi0"); 
  VmodeSon.push_back(Vson);

 }

  // destructor
  virtual ~EvtPsi3Sdecay(){}

  int findPoints(); //return the lower size at the end of the segment containing Ecms points
  double polint( std::vector <double> points);
  bool choseDecay(); //determing accept or reject a generated decay
  EvtParticle * choseDecay(EvtParticle* par);
  int getDecay(double ecms); //determing accept or reject a generated decay
  double theProb(std::vector<double> myxs,int ich);
  int findMode( );
  int getMode(){return _themode;};
  std::vector<EvtId> getVId(int mode);


  void PHSPDecay(EvtParticle * par);
  std::vector<EvtId> getDaugId(){return Vid;}
  std::vector<EvtVector4R> getDaugP4(){return v_p4;}
  bool AngSam(EvtVector4R parent_p4cm,EvtVector4R son_p4cm,double alpha);
  void setMode(int m){
    if(m<0 || m>32) {std::cout<<"EvtPsi3Decay::setMode: bad mode"<<std::endl;abort();}
    _themode = m;_excflag=1;}
private:
  double Ecms;
  EvtParticle *theParent;
  EvtId *Daughters;
  int Ndaugs;
  std::vector <double> x;
  std::vector <double> d0d0bar;     // 0)
  std::vector <double> dpdm;        // 1)
  std::vector <double> d0dst0bar;   // 2)
  std::vector <double> d0bardst0;   // 3)
  std::vector <double> dst0dst0bar; // 4)
  std::vector <double> dstpdm;      // 5)
  std::vector <double> dstmdp;      // 6)
  std::vector <double> dstpdstm;    // 7)
  std::vector <double> dspdsm;      // 8)
  std::vector <double> dsspdsm;     // 9)
  std::vector <double> dssmdsp;     //10)
  std::vector <double> dsspdssm;    //11)
  std::vector <double> xs12;        //12)
  std::vector <double> xs13;        //13)
  std::vector <double> xs14;        //14)
  std::vector <double> xs15;        //15)
  std::vector <double> xs16;        //16)
  std::vector <double> xs17;        //17)
  std::vector <double> xs18;        //18)
  std::vector <double> xs19;        //19)
  std::vector <double> xs20;        //20)
  std::vector <double> xs21;        //21)
  std::vector <double> xs22;        //22)
  std::vector <double> xs23;        //23)

  int nsize;  //the simze of the energy points
  double theXsection[50];
  int theLocation;
  std::vector<std::string> Vson;
  std::vector<EvtId> Vid;

  std::vector< std::vector<std::string> > VmodeSon;
  std::vector< std::vector<EvtId>       > VmodeId;
  int _themode,_excflag;
  //
  std::vector<EvtVector4R> v_p4;
  static int psi3Scount;
};


#endif
