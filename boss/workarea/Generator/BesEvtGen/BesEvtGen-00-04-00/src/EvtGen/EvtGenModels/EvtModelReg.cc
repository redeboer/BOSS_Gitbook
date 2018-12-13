//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/COPYRIGHT
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtModel.cc
//
// Description:
//
// Modification history:
//
//    RYD     September 25, 1996         Module created
//
//------------------------------------------------------------------------
// 
#include "EvtGenBase/EvtPatches.hh"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "EvtGenBase/EvtModel.hh"

#include "EvtGenModels/EvtTauVectornu.hh"
#include "EvtGenModels/EvtVVP.hh"
#include "EvtGenModels/EvtSLN.hh"
#include "EvtGenModels/EvtISGW2.hh"
#include "EvtGenModels/EvtMelikhov.hh"
#include "EvtGenModels/EvtSLPole.hh"
#include "EvtGenModels/EvtISGW.hh"
#include "EvtGenModels/EvtBHadronic.hh"
#include "EvtGenModels/EvtVSS.hh"
#include "EvtGenModels/EvtVSSMix.hh"
#include "EvtGenModels/EvtVSSBMixCPT.hh"
#include "EvtGenModels/EvtVSPPwave.hh"
#include "EvtGenModels/EvtGoityRoberts.hh"
#include "EvtGenModels/EvtSVS.hh"
#include "EvtGenModels/EvtTSS.hh"
#include "EvtGenModels/EvtTVSPwave.hh"
#include "EvtGenModels/EvtSVVHelAmp.hh"
#include "EvtGenModels/EvtSVPHelAmp.hh"
#include "EvtGenModels/EvtSVPCP.hh"
#include "EvtGenModels/EvtVVSPwave.hh"
#include "EvtGenModels/EvtDDalitz.hh"
#include "EvtGenModels/EvtOmegaDalitz.hh"
#include "EvtGenModels/EvtPi0Dalitz.hh"
#include "EvtGenModels/EvtEtaDalitz.hh"
#include "EvtGenModels/EvtPhsp.hh"
#include "EvtGenModels/EvtCalHelAmp.hh"
#include "EvtGenModels/EvtRexc.hh"
#include "EvtGenModels/EvtConExc.hh" 
#include "EvtGenModels/EvtBtoXsgamma.hh"
#include "EvtGenModels/EvtBtoXsll.hh"
#include "EvtGenModels/EvtBtoXsEtap.hh"
#include "EvtGenModels/EvtSSSCP.hh"
#include "EvtGenModels/EvtSSSCPpng.hh"
#include "EvtGenModels/EvtSTSCP.hh"
#include "EvtGenModels/EvtSTS.hh"
#include "EvtGenModels/EvtSSSCPT.hh"
#include "EvtGenModels/EvtSVSCP.hh"
#include "EvtGenModels/EvtSSDCP.hh"
#include "EvtGenModels/EvtSVSNONCPEIGEN.hh"
#include "EvtGenModels/EvtSVVNONCPEIGEN.hh"
#include "EvtGenModels/EvtSVVCP.hh"
#include "EvtGenModels/EvtSVVCPLH.hh"
#include "EvtGenModels/EvtSVSCPLH.hh"
#include "EvtGenModels/EvtSll.hh"
#include "EvtGenModels/EvtVll.hh"
#include "EvtGenModels/EvtEDM.hh"
#include "EvtGenModels/EvtTaulnunu.hh"
#include "EvtGenModels/EvtTauHadnu.hh"
#include "EvtGenModels/EvtTauScalarnu.hh"
#include "EvtGenModels/EvtKstarnunu.hh"
#include "EvtGenModels/EvtbTosllBall.hh"
#include "EvtGenModels/EvtSingleParticle.hh"
#include "EvtGenModels/EvtSingleParticle2.hh"
#include "EvtGenModels/EvtSinglePoint.hh"
#include "EvtGenModels/EvtVectorIsr.hh"
#include "EvtGenModels/EvtVPHOtoVISRHi.hh"
#include "EvtGenModels/EvtVPHOtoVISR.hh"
#include "EvtGenModels/EvtSLBKPole.hh"

#include "EvtGenModels/EvtHQET.hh"
#include "EvtGenModels/EvtHQET2.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenModels/EvtHelAmp.hh"
#include "EvtGenModels/EvtPartWave.hh"

#include "EvtGenModels/EvtBto2piCPiso.hh"
#include "EvtGenModels/EvtBtoKpiCPiso.hh"
#include "EvtGenModels/EvtSVSCPiso.hh"
#include "EvtGenModels/EvtVVpipi.hh"
#include "EvtGenModels/EvtVVPIPI_WEIGHTED.hh"

#include "EvtGenModels/EvtBTo4piCP.hh"
#include "EvtGenModels/EvtBTo3piCP.hh"
#include "EvtGenModels/EvtCBTo3piP00.hh"
#include "EvtGenModels/EvtCBTo3piMPP.hh"
#include "EvtGenModels/EvtBToKpipiCP.hh"
#include "EvtGenModels/EvtBsquark.hh"
#include "EvtGenModels/EvtPhiDalitz.hh"

#include "EvtGenModels/EvtLNuGamma.hh"

#include "EvtGenModels/EvtVub.hh"
#include "EvtGenModels/EvtVubHybrid.hh"
#include "EvtGenModels/EvtVubNLO.hh"
#include "EvtGenModels/EvtPto3P.hh"
#include "EvtGenModels/EvtBtoKD3P.hh"
#include "EvtGenModels/EvtKstarstargamma.hh"
#include "EvtGenModels/EvtFlatQ2.hh"
#include "EvtGenModels/EvtLambdaP_BarGamma.hh"
#include "EvtGenModels/EvtKKLambdaC.hh"
#include "EvtGenModels/EvtMultibody.hh"
#include "EvtGenModels/EvtJpipi.hh"
#include "EvtGenModels/EvtAngSam.hh"
#include "EvtGenModels/EvtAngSam3.hh"
#include "EvtGenModels/EvtAngSamX.hh"
#include "EvtGenModels/EvtAngSamLab.hh"
#include "EvtGenModels/EvtS2GV.hh"
#include "EvtGenModels/EvtAV2GV.hh" 
#include "EvtGenModels/EvtJ2BB1.hh" 
#include "EvtGenModels/EvtJ2BB2.hh"
#include "EvtGenModels/EvtJ2BB3.hh"
#include "EvtGenModels/EvtChi0BB1.hh"
#include "EvtGenModels/EvtChi1BB1.hh"
#include "EvtGenModels/EvtChi2BB1.hh"
#include "EvtGenModels/EvtChi0BB2.hh"
#include "EvtGenModels/EvtChi1BB2.hh"
#include "EvtGenModels/EvtChi2BB2.hh"

#include "EvtGenModels/EvtHypWK.hh"         //BES , 2007
#include "EvtGenModels/EvtTauGamMu.hh"    
#include "EvtGenModels/EvtHypNonLepton.hh"  //from CLEO 2009-04
#include "EvtGenModels/EvtDeBD.hh"
#include "EvtGenModels/EvtT2GV.hh"
#include "EvtGenModels/EvtP2GC0.hh"
#include "EvtGenModels/EvtP2GC1.hh"    
#include "EvtGenModels/EvtP2GC2.hh"
#include "EvtGenModels/EvtDIY.hh"   
#include "EvtGenModels/EvtRhoPi.hh"
#include "EvtGenModels/EvtHAngSam3.hh"
#include "EvtGenModels/EvtAngH2.hh"
#include "EvtGenModels/EvtMassH1.hh"
#include "EvtGenModels/EvtMassH2.hh"
#include "EvtGenModels/EvtmH2.hh"
#include "EvtGenModels/EvtBody3.hh"
#include "EvtGenModels/EvtMBody3.hh"
#include "EvtGenModels/EvtJPE.hh"
#include "EvtGenModels/EvtPBB1.hh"
#include "EvtGenModels/EvtPBB2.hh" 
#include "EvtGenModels/EvtSPL.hh" 
#include "EvtGenModels/EvtHelPPJ.hh"
#include "EvtGenModels/EvtLundCharm.hh"
#include "EvtGenModels/EvtLunda.hh"
#include "EvtGenModels/EvtPhokhara_pipi.hh"
#include "EvtGenModels/EvtPhokhara_pi0pi0pipi.hh"
#include "EvtGenModels/EvtPhokhara_4pi.hh"
#include "EvtGenModels/EvtPhokhara_ppbar.hh"
#include "EvtGenModels/EvtPhokhara_nnbar.hh"
#include "EvtGenModels/EvtPhokhara_KK.hh"
#include "EvtGenModels/EvtPhokhara_K0K0.hh"
#include "EvtGenModels/EvtPhokhara_pipipi0.hh"
#include "EvtGenModels/EvtPhokhara_LLB.hh"
#include "EvtGenModels/EvtPhokhara_pipieta.hh"
#include "EvtGenModels/EvtTauola.hh" 
#include "EvtGenModels/EvtOpenCharm.hh"
#include "EvtGenModels/EvtmPhsp.hh" 
#include "EvtGenModels/EvtLambdac2pKpi.hh"
#include "EvtGenModels/EvtTrackGen.hh" 
#include "EvtGenModels/EvtJTO3P.hh" 
#include "EvtGenModels/EvtDMix.hh" 
#include "EvtGenModels/EvtD0mixDalitz.hh"

#ifndef EVTSTANDALONE
#include "EvtGenModels/EvtUclaII.hh"
#include "EvtGenModels/EvtJetSet.hh"
#include "EvtGenModels/EvtJscont.hh"
#else
#include "EvtGenModels/EvtPythia.hh"
#include "EvtGenModels/EvtPyGaGa.hh"
#include "EvtGenModels/EvtPycont.hh"
#endif

#include "EvtGenModels/EvtModelReg.hh"
using std::fstream;
using std::cout;
using std::endl;

EvtModelReg::EvtModelReg() 
{
    EvtModel &modelist=EvtModel::instance();

    modelist.Register(new EvtLambdaP_BarGamma);
    modelist.Register(new EvtFlatQ2);
    modelist.Register(new EvtTauHadnu);
    modelist.Register(new EvtTauVectornu);
    modelist.Register(new EvtVVP);
    modelist.Register(new EvtSLN);
    modelist.Register(new EvtISGW2);
    modelist.Register(new EvtMelikhov);
    modelist.Register(new EvtSLPole);
    modelist.Register(new EvtHQET);
    modelist.Register(new EvtHQET2);
    modelist.Register(new EvtISGW);
    modelist.Register(new EvtBHadronic);
    modelist.Register(new EvtVSS);
    modelist.Register(new EvtVSSMix);
    modelist.Register(new EvtVSSBMixCPT);
    modelist.Register(new EvtVSPPwave);
    modelist.Register(new EvtGoityRoberts);
    modelist.Register(new EvtSVS);
    modelist.Register(new EvtTSS);
    modelist.Register(new EvtTVSPwave);
    modelist.Register(new EvtSVVHelAmp);
    modelist.Register(new EvtSVPHelAmp);
    modelist.Register(new EvtSVPCP);
    modelist.Register(new EvtVVSPwave);
    modelist.Register(new EvtDDalitz);
    modelist.Register(new EvtOmegaDalitz);
    modelist.Register(new EvtEtaDalitz);
    modelist.Register(new EvtPhsp);
    modelist.Register(new EvtCalHelAmp);
    modelist.Register(new EvtRexc);
    modelist.Register(new EvtLambdac2pKpi);
    modelist.Register(new EvtTrackGen);
    modelist.Register(new EvtConExc);
    modelist.Register(new EvtBtoXsgamma);
    modelist.Register(new EvtBtoXsll);
    modelist.Register(new EvtBtoXsEtap);
    modelist.Register(new EvtSSSCP);
    modelist.Register(new EvtSSSCPpng);
    modelist.Register(new EvtSTSCP);
    modelist.Register(new EvtSTS);
    modelist.Register(new EvtSSSCPT);
    modelist.Register(new EvtSVSCP);
    modelist.Register(new EvtSSDCP);
    modelist.Register(new EvtSVSNONCPEIGEN);
    modelist.Register(new EvtSVVNONCPEIGEN);
    modelist.Register(new EvtSVVCP);
    modelist.Register(new EvtSVVCPLH);
    modelist.Register(new EvtSVSCPLH);
    modelist.Register(new EvtSll);
    modelist.Register(new EvtVll);
    modelist.Register(new EvtEDM);
    modelist.Register(new EvtTaulnunu);
    modelist.Register(new EvtTauScalarnu);
    modelist.Register(new EvtKstarnunu);
    modelist.Register(new EvtbTosllBall);
    modelist.Register(new EvtBto2piCPiso);
    modelist.Register(new EvtBtoKpiCPiso);
    modelist.Register(new EvtSVSCPiso);
    modelist.Register(new EvtSingleParticle);
    modelist.Register(new EvtSingleParticle2);
    modelist.Register(new EvtSinglePoint);
    modelist.Register(new EvtVectorIsr);
    modelist.Register(new EvtVPHOtoVISRHi);
    modelist.Register(new EvtVPHOtoVISR);
    modelist.Register(new EvtSLBKPole);
    modelist.Register(new EvtPi0Dalitz);
    modelist.Register(new EvtHelAmp);
    modelist.Register(new EvtPartWave);
    modelist.Register(new EvtVVpipi);
    modelist.Register(new EvtVVPIPI_WEIGHTED);
    modelist.Register(new EvtBsquark);
    modelist.Register(new EvtPhiDalitz); 

    modelist.Register(new EvtBTo4piCP);
    modelist.Register(new EvtBTo3piCP);
    modelist.Register(new EvtCBTo3piP00);
    modelist.Register(new EvtCBTo3piMPP);
    modelist.Register(new EvtBToKpipiCP);

    modelist.Register(new EvtLNuGamma);
    modelist.Register(new EvtKstarstargamma);

    modelist.Register(new EvtVub);
    modelist.Register(new EvtVubHybrid);
    modelist.Register(new EvtVubNLO);
    modelist.Register(new EvtPto3P);
    modelist.Register(new EvtBtoKD3P);
    modelist.Register(new EvtKKLambdaC);
    modelist.Register(new EvtMultibody);
    modelist.Register(new EvtJpipi);
    modelist.Register(new EvtAngSam);
    modelist.Register(new EvtAngSamLab);
    modelist.Register(new EvtAngSam3);
    modelist.Register(new EvtAngSamX);
    modelist.Register(new EvtP2GC0);
    modelist.Register(new EvtP2GC1);
    modelist.Register(new EvtP2GC2);
    modelist.Register(new EvtDIY);  
    modelist.Register(new EvtRhoPi);
    modelist.Register(new EvtHAngSam3);
    modelist.Register(new EvtMassH1);
    modelist.Register(new EvtMassH2);
    modelist.Register(new EvtmH2);
    modelist.Register(new EvtAngH2);
    modelist.Register(new EvtBody3);
    modelist.Register(new EvtMBody3);
    modelist.Register(new EvtJPE);
    modelist.Register(new EvtPBB1);
    modelist.Register(new EvtPBB2);
    modelist.Register(new EvtSPL);
    modelist.Register(new EvtHelPPJ);
    modelist.Register(new EvtS2GV);
    modelist.Register(new EvtAV2GV);  
    modelist.Register(new EvtT2GV);
    modelist.Register(new EvtJ2BB1);
    modelist.Register(new EvtJ2BB2);
    modelist.Register(new EvtJ2BB3);
    modelist.Register(new EvtChi0BB1);
    modelist.Register(new EvtChi1BB1);
    modelist.Register(new EvtChi2BB1);
    modelist.Register(new EvtChi0BB2);
    modelist.Register(new EvtChi1BB2);
    modelist.Register(new EvtChi2BB2);
    modelist.Register(new EvtHypWK);
    modelist.Register(new EvtTauGamMu);
    modelist.Register(new EvtHypNonLepton);
    modelist.Register(new EvtDeBD);
    modelist.Register(new EvtLundCharm);
    modelist.Register(new EvtLunda);
    modelist.Register(new EvtPhokhara_pipi);
    modelist.Register(new EvtPhokhara_pi0pi0pipi);
    modelist.Register(new EvtPhokhara_4pi);
    modelist.Register(new EvtPhokhara_ppbar);
    modelist.Register(new EvtPhokhara_nnbar);
    modelist.Register(new EvtPhokhara_KK);
    modelist.Register(new EvtPhokhara_K0K0);
    modelist.Register(new EvtPhokhara_pipipi0);
    modelist.Register(new EvtPhokhara_LLB);
    modelist.Register(new EvtPhokhara_pipieta);
    modelist.Register(new EvtTauola);
    modelist.Register(new EvtOpenCharm);
    modelist.Register(new EvtmPhsp);
    modelist.Register(new EvtJTO3P);
    modelist.Register(new EvtDMix);
    modelist.Register(new EvtD0mixDalitz);

#ifndef EVTSTANDALONE
    modelist.Register(new EvtUclaII);
    cout << "Will register EvtJetSet"<<endl;
    modelist.Register(new EvtJetSet);
    modelist.Register(new EvtJscont);
#else
    modelist.Register(new EvtPycont);
    cout << "Will register EvtPythia"<<endl;
    modelist.Register(new EvtPythia);
    modelist.Register(new EvtPyGaGa);
#endif

}








