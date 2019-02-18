#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"
#include "GaudiKernel/SmartDataPtr.h"

#include "EventModel/EventModel.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "DTagAlg/LocalPhotonSelector.h"

LocalPhotonSelector::LocalPhotonSelector()
{
   IJobOptionsSvc* jobSvc;
   Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

   PropertyMgr m_propMgr;

   //Declare the properties
   m_propMgr.declareProperty("MinEnergy", m_minEnergy = 0.025);

   m_propMgr.declareProperty("InBarrelOrEndcap",  m_useBarrelEndcap   = true);
   m_propMgr.declareProperty("MaxCosThetaBarrel", m_maxCosThetaBarrel = 0.8);
   m_propMgr.declareProperty("MinCosThetaEndcap", m_minCosThetaEndcap = 0.84);
   m_propMgr.declareProperty("MaxCosThetaEndcap", m_maxCosThetaEndcap = 0.92);
   m_propMgr.declareProperty("MinEndcapEnergy",   m_minEndcapEnergy   = 0.050);
 
   m_propMgr.declareProperty("ApplyTimeCut",    m_applyTimeCut = true);
   m_propMgr.declareProperty("MinTime",         m_minTime      = 0.);
   m_propMgr.declareProperty("MaxTime",         m_maxTime      = 14.);
   m_propMgr.declareProperty("PhotonDeltaTime", m_deltaTime    = 10.);
 
   m_propMgr.declareProperty("ApplyDangCut", m_applyDangCut = true);
   m_propMgr.declareProperty("MinDang",      m_minDang      = 20.0);

   jobSvc->setMyProperties("LocalPhotonSelector", &m_propMgr);
}

bool LocalPhotonSelector::operator() (CDPhoton& aPhoton) {

  aPhoton.setUserTag(1);
   SmartDataPtr<EvtRecEvent> recEvt(eventSvc(), EventModel::EvtRec::EvtRecEvent);
   SmartDataPtr<EvtRecTrackCol> recTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol);

   EvtRecTrack* recTrk = const_cast<EvtRecTrack*>( aPhoton.photon() );

   if ( !recTrk->isEmcShowerValid() ) return false;
 
   RecEmcShower *emcTrk = recTrk->emcShower();
   double eraw = emcTrk->energy();
   double phi =  emcTrk->phi();
   double the =  emcTrk->theta();
   HepLorentzVector shP4( eraw * sin(the) * cos(phi),
			  eraw * sin(the) * sin(phi),
			  eraw * cos(the),
			  eraw );

   double cosThetaSh = shP4.vect().cosTheta();
 
 
   /// Minimum energy
   if (shP4.e() <= m_minEnergy) return false;


   /// Barrel/Endcap
   if ( m_useBarrelEndcap ) {
      bool inBarrelEndcap = false;

      if(fabs(cosThetaSh) < m_maxCosThetaBarrel) inBarrelEndcap = true;

      if((fabs(cosThetaSh) > m_minCosThetaEndcap)
	    &&(fabs(cosThetaSh) < m_maxCosThetaEndcap)
	    &&(shP4.e() > m_minEndcapEnergy)) inBarrelEndcap = true;

      if ( !inBarrelEndcap ) return false;
   }
 
   /// Time, only apply timing cuts if "recEvt->totalCharged() > 0"
   if ( m_applyTimeCut ) {
      double time = emcTrk->time();
      if ( recEvt->totalCharged() > 0 ) {
	 if ( time < m_minTime || time > m_maxTime ) return false;
      }
      else {
	 RecEmcShower* firstG = (*(recTrkCol->begin()))->emcShower();
	 double deltaTime = fabs(time - firstG->time());
	 if ( deltaTime > 10 ) return false;
      }
   }

   /// Dang
   if (m_applyDangCut && recEvt->totalCharged() > 0) {
      Hep3Vector emcpos(emcTrk->x(), emcTrk->y(), emcTrk->z());
      double dang = 200.;

      for (int j = 0; j < recEvt->totalCharged(); j++) {
	 EvtRecTrackIterator jtTrk = recTrkCol->begin() + j;
	 if ( !(*jtTrk)->isExtTrackValid() ) continue;
	 RecExtTrack* extTrk = (*jtTrk)->extTrack();
	 if ( extTrk->emcVolumeNumber() == -1 ) continue;
	 Hep3Vector extpos = extTrk->emcPosition();
	 double  angd1 = extpos.angle(emcpos);
	 if ( angd1 < dang ) dang = angd1;
      }

      if ( dang < 200 ) {
	 dang = dang * 180 / (CLHEP::pi);
	 if (dang <= m_minDang) return( false );
      }
   }  // End of "recEvt->totalCharged() > 0" IF
 
   return true;
}

LocalPhotonSelector photonSelector;
