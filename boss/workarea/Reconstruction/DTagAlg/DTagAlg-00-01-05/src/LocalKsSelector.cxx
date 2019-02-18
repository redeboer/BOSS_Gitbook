#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EvtRecEvent/EvtRecVeeVertex.h"
#include "DTagAlg/LocalKsSelector.h"
#include "VertexFit/VertexFit.h"
#include "VertexFit/SecondVertexFit.h"
#include "VertexFit/IVertexDbSvc.h"

LocalKsSelector::LocalKsSelector()
{
   IJobOptionsSvc* jobSvc;
   Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

   PropertyMgr m_propMgr;

   m_propMgr.declareProperty("KsMinMassCut",  m_minMass = 0.470 );
   m_propMgr.declareProperty("KsMaxMassCut",  m_maxMass = 0.528 );
   m_propMgr.declareProperty("KsMaxChisq",    m_maxChisq = 100 );

   m_propMgr.declareProperty("DoSecondaryVFit",   m_doSecondaryVFit = false );
   m_propMgr.declareProperty("KsMaxVFitChisq",    m_maxVFitChisq = 100 );
   m_propMgr.declareProperty("KsMinFlightSig",    m_minFlightSig = 2.0 );

   jobSvc->setMyProperties("LocalKsSelector", &m_propMgr);
}

bool LocalKsSelector::operator() (CDKs& aKs) {

  aKs.setUserTag(1);
  EvtRecVeeVertex* ks = const_cast<EvtRecVeeVertex*>( aKs.navKshort() );

   if ( ks->vertexId() != 310 ) return false;

   double mass = ks->mass();
   if ((mass <= m_minMass)||(mass >= m_maxMass)) return false;
   if ( ks->chi2() >= m_maxChisq ) return false;

   if(mass < 0.487 || mass > 0.511)
     aKs.setUserTag(2);

   if( !m_doSecondaryVFit ) return true;

   // --------------------------------------------------
   // Do a secondary vertex fit and check the flight significance
   // --------------------------------------------------

   EvtRecTrack* veeTrack1 = ks->pairDaughters().first;
   RecMdcKalTrack* veeKalTrack1 = veeTrack1->mdcKalTrack();
   WTrackParameter veeInitialWTrack1 = WTrackParameter(0.13957018, veeKalTrack1->getZHelix(), veeKalTrack1->getZError());

   EvtRecTrack* veeTrack2 = ks->pairDaughters().second;
   RecMdcKalTrack* veeKalTrack2 = veeTrack2->mdcKalTrack();
   WTrackParameter veeInitialWTrack2 = WTrackParameter(0.13957018, veeKalTrack2->getZHelix(), veeKalTrack2->getZError());

   VertexParameter wideVertex;
   HepPoint3D vWideVertex(0., 0., 0.);
   HepSymMatrix evWideVertex(3, 0);

   evWideVertex[0][0] = 1.0e12;
   evWideVertex[1][1] = 1.0e12;
   evWideVertex[2][2] = 1.0e12;

   wideVertex.setVx(vWideVertex);
   wideVertex.setEvx(evWideVertex);

   // First, perform a vertex fit
   VertexFit* vtxfit = VertexFit::instance();
   vtxfit->init();

   // add the daughters
   vtxfit->AddTrack(0,veeInitialWTrack1);
   vtxfit->AddTrack(1,veeInitialWTrack2);
   vtxfit->AddVertex(0,wideVertex,0,1);

   // do the fit
   vtxfit->Fit(0);
   vtxfit->Swim(0);
   vtxfit->BuildVirtualParticle(0);

   // Now perform the secondary vertex fit
   SecondVertexFit* svtxfit = SecondVertexFit::instance();
   svtxfit->init();

   // add the primary vertex
   VertexParameter beamSpot;
   HepPoint3D vBeamSpot(0., 0., 0.);
   HepSymMatrix evBeamSpot(3, 0);

   IVertexDbSvc*  vtxsvc;
   Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
   if(vtxsvc->isVertexValid()){
     double* dbv = vtxsvc->PrimaryVertex();
     double*  vv = vtxsvc->SigmaPrimaryVertex();
     for (unsigned int ivx = 0; ivx < 3; ivx++){
       vBeamSpot[ivx] = dbv[ivx];
       evBeamSpot[ivx][ivx] = vv[ivx] * vv[ivx];
     }
   }
   else{
     cout << "KSSELECTOR ERROR:  Could not find a vertex from VertexDbSvc" << endl;
     return false;
   }

   beamSpot.setVx(vBeamSpot);
   beamSpot.setEvx(evBeamSpot);

   VertexParameter primaryVertex(beamSpot);
   svtxfit->setPrimaryVertex(primaryVertex);

   // add the secondary vertex
   svtxfit->setVpar(vtxfit->vpar(0));

   // add the Ks or lambda
   svtxfit->AddTrack(0,vtxfit->wVirtualTrack(0));

   // do the second vertex fit
   // if the fit fails, the default values will not be changed
   if( !svtxfit->Fit() ) return false;

   // save the new ks parameters
   double vfitlength = svtxfit->decayLength();
   double vfiterror = svtxfit->decayLengthError();
   double vfitchi2 = svtxfit->chisq();
   double flightsig = 0;
   if( vfiterror != 0 )
     flightsig = vfitlength/vfiterror;

   // if the ks does not meet the criteria, the information from
   // the secondary vertex fit will not be filled (default = -999)
   if( vfitchi2 > m_maxVFitChisq ) return false;
   if( flightsig < m_minFlightSig ) return false;
    
   return true;
}

LocalKsSelector ksSelector;
