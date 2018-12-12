#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/PropertyMgr.h"
#include "VertexFit/IVertexDbSvc.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/ISvcLocator.h"

#include "EventModel/EventModel.h"
#include "EventModel/Event.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "DstEvent/TofHitStatus.h"
#include "EventModel/EventHeader.h"



#include "TMath.h"
#include "GaudiKernel/INTupleSvc.h"
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IHistogramSvc.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/TwoVector.h"
using CLHEP::Hep3Vector;
using CLHEP::Hep2Vector;
using CLHEP::HepLorentzVector;
#include "CLHEP/Geometry/Point3D.h"
#ifndef ENABLE_BACKWARDS_COMPATIBILITY
   typedef HepGeom::Point3D<double> HepPoint3D;
#endif
#include "RhopiAlg/Rhopi.h"

//#include "VertexFit/KinematicFit.h"
#include "VertexFit/KalmanKinematicFit.h"
#include "VertexFit/VertexFit.h"
#include "VertexFit/Helix.h"
#include "ParticleID/ParticleID.h"

#include <vector>
//const double twopi = 6.2831853;
//const double pi = 3.1415927;
const double mpi = 0.13957;
const double xmass[5] = {0.000511, 0.105658, 0.139570,0.493677, 0.938272};
//const double velc = 29.9792458;  tof_path unit in cm.
const double velc = 299.792458;   // tof path unit in mm
typedef std::vector<int> Vint;
typedef std::vector<HepLorentzVector> Vp4;

int Ncut0,Ncut1,Ncut2,Ncut3,Ncut4,Ncut5,Ncut6;

/////////////////////////////////////////////////////////////////////////////

Rhopi::Rhopi(const std::string& name, ISvcLocator* pSvcLocator) :
  Algorithm(name, pSvcLocator) {
  
  //Declare the properties  
  declareProperty("Vr0cut", m_vr0cut=1.0);
  declareProperty("Vz0cut", m_vz0cut=5.0);
  declareProperty("EnergyThreshold", m_energyThreshold=0.04);
  declareProperty("GammaPhiCut", m_gammaPhiCut=20.0);
  declareProperty("GammaThetaCut", m_gammaThetaCut=20.0);
  declareProperty("GammaAngleCut", m_gammaAngleCut=20.0);
  declareProperty("Test4C", m_test4C = 1);
  declareProperty("Test5C", m_test5C = 1);
  declareProperty("CheckDedx", m_checkDedx = 1);
  declareProperty("CheckTof",  m_checkTof = 1);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode Rhopi::initialize(){
  MsgStream log(msgSvc(), name());

  log << MSG::INFO << "in initialize()" << endmsg;
  
  StatusCode status;
  NTuplePtr nt1(ntupleSvc(), "FILE1/vxyz");
  if ( nt1 ) m_tuple1 = nt1;
  else {
    m_tuple1 = ntupleSvc()->book ("FILE1/vxyz", CLID_ColumnWiseTuple, "ks N-Tuple example");
    if ( m_tuple1 )    {
      status = m_tuple1->addItem ("vx0",   m_vx0);
      status = m_tuple1->addItem ("vy0",   m_vy0);
      status = m_tuple1->addItem ("vz0",   m_vz0);
      status = m_tuple1->addItem ("vr0",   m_vr0);
      status = m_tuple1->addItem ("rvxy0",  m_rvxy0);
      status = m_tuple1->addItem ("rvz0",   m_rvz0);
      status = m_tuple1->addItem ("rvphi0", m_rvphi0);
    }
    else    { 
      log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple1) << endmsg;
      return StatusCode::FAILURE;
    }
  }

  NTuplePtr nt2(ntupleSvc(), "FILE1/photon");
  if ( nt2 ) m_tuple2 = nt2;
  else {
    m_tuple2 = ntupleSvc()->book ("FILE1/photon", CLID_ColumnWiseTuple, "ks N-Tuple example");
    if ( m_tuple2 )    {
      status = m_tuple2->addItem ("dthe",   m_dthe);
      status = m_tuple2->addItem ("dphi",   m_dphi);
      status = m_tuple2->addItem ("dang",   m_dang);
      status = m_tuple2->addItem ("eraw",   m_eraw);
    }
    else    { 
      log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple2) << endmsg;
      return StatusCode::FAILURE;
    }
  }


  NTuplePtr nt3(ntupleSvc(), "FILE1/etot");
  if ( nt3 ) m_tuple3 = nt3;
  else {
    m_tuple3 = ntupleSvc()->book ("FILE1/etot", CLID_ColumnWiseTuple, "ks N-Tuple example");
    if ( m_tuple3 )    {
      status = m_tuple3->addItem ("m2gg",   m_m2gg);
      status = m_tuple3->addItem ("etot",   m_etot);
    }
    else    { 
      log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple3) << endmsg;
      return StatusCode::FAILURE;
    }
  }
  if(m_test4C==1) {
    NTuplePtr nt4(ntupleSvc(), "FILE1/fit4c");
    if ( nt4 ) m_tuple4 = nt4;
    else {
      m_tuple4 = ntupleSvc()->book ("FILE1/fit4c", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple4 )    {
	status = m_tuple4->addItem ("chi2",   m_chi1);
	status = m_tuple4->addItem ("mpi0",   m_mpi0);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple4) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // test 4C


  if(m_test5C==1) {
    NTuplePtr nt5(ntupleSvc(), "FILE1/fit5c");
    if ( nt5 ) m_tuple5 = nt5;
    else {
      m_tuple5 = ntupleSvc()->book ("FILE1/fit5c", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple5 )    {
	status = m_tuple5->addItem ("chi2",   m_chi2);
	status = m_tuple5->addItem ("mrh0",   m_mrh0);
	status = m_tuple5->addItem ("mrhp",   m_mrhp);
	status = m_tuple5->addItem ("mrhm",   m_mrhm);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple5) << endmsg;
	return StatusCode::FAILURE;
      }
    }
 
    NTuplePtr nt6(ntupleSvc(), "FILE1/geff");
    if ( nt6 ) m_tuple6 = nt6;
    else {
      m_tuple6 = ntupleSvc()->book ("FILE1/geff", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple6 )    {
	status = m_tuple6->addItem ("fcos",   m_fcos);
	status = m_tuple6->addItem ("elow",   m_elow);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple6) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // test 5c

  if(m_checkDedx == 1) {
    NTuplePtr nt7(ntupleSvc(), "FILE1/dedx");
    if ( nt7 ) m_tuple7 = nt7;
    else {
      m_tuple7 = ntupleSvc()->book ("FILE1/dedx", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple7 )    {
	status = m_tuple7->addItem ("ptrk",   m_ptrk);
	status = m_tuple7->addItem ("chie",   m_chie);
	status = m_tuple7->addItem ("chimu",   m_chimu);
	status = m_tuple7->addItem ("chipi",   m_chipi);
	status = m_tuple7->addItem ("chik",   m_chik);
	status = m_tuple7->addItem ("chip",   m_chip);
	status = m_tuple7->addItem ("probPH",   m_probPH);
	status = m_tuple7->addItem ("normPH",   m_normPH);
	status = m_tuple7->addItem ("ghit",   m_ghit);
	status = m_tuple7->addItem ("thit",   m_thit);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple7) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // check dE/dx

  if(m_checkTof == 1) {
    NTuplePtr nt8(ntupleSvc(), "FILE1/tofe");
    if ( nt8 ) m_tuple8 = nt8;
    else {
      m_tuple8 = ntupleSvc()->book ("FILE1/tofe",CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple8 )    {
	status = m_tuple8->addItem ("ptrk",   m_ptot_etof);
	status = m_tuple8->addItem ("cntr",   m_cntr_etof);
	status = m_tuple8->addItem ("ph",  m_ph_etof);
	status = m_tuple8->addItem ("rhit", m_rhit_etof);
	status = m_tuple8->addItem ("qual", m_qual_etof);
	status = m_tuple8->addItem ("te",   m_te_etof);
	status = m_tuple8->addItem ("tmu",   m_tmu_etof);
	status = m_tuple8->addItem ("tpi",   m_tpi_etof);
	status = m_tuple8->addItem ("tk",   m_tk_etof);
	status = m_tuple8->addItem ("tp",   m_tp_etof);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple8) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // check Tof:endcap



  if(m_checkTof == 1) {
    NTuplePtr nt9(ntupleSvc(), "FILE1/tof1");
    if ( nt9 ) m_tuple9 = nt9;
    else {
      m_tuple9 = ntupleSvc()->book ("FILE1/tof1", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple9 )    {
	status = m_tuple9->addItem ("ptrk",   m_ptot_btof1);
	status = m_tuple9->addItem ("cntr",   m_cntr_btof1);
	status = m_tuple9->addItem ("ph",  m_ph_btof1);
	status = m_tuple9->addItem ("zhit", m_zhit_btof1);
	status = m_tuple9->addItem ("qual", m_qual_btof1);
	status = m_tuple9->addItem ("te",   m_te_btof1);
	status = m_tuple9->addItem ("tmu",   m_tmu_btof1);
	status = m_tuple9->addItem ("tpi",   m_tpi_btof1);
	status = m_tuple9->addItem ("tk",   m_tk_btof1);
	status = m_tuple9->addItem ("tp",   m_tp_btof1);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple9) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // check Tof:barrel inner Tof 


  if(m_checkTof == 1) {
    NTuplePtr nt10(ntupleSvc(), "FILE1/tof2");
    if ( nt10 ) m_tuple10 = nt10;
    else {
      m_tuple10 = ntupleSvc()->book ("FILE1/tof2", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple10 )    {
	status = m_tuple10->addItem ("ptrk",   m_ptot_btof2);
	status = m_tuple10->addItem ("cntr",   m_cntr_btof2);
	status = m_tuple10->addItem ("ph",  m_ph_btof2);
	status = m_tuple10->addItem ("zhit", m_zhit_btof2);
	status = m_tuple10->addItem ("qual", m_qual_btof2);
	status = m_tuple10->addItem ("te",   m_te_btof2);
	status = m_tuple10->addItem ("tmu",   m_tmu_btof2);
	status = m_tuple10->addItem ("tpi",   m_tpi_btof2);
	status = m_tuple10->addItem ("tk",   m_tk_btof2);
	status = m_tuple10->addItem ("tp",   m_tp_btof2);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple10) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // check Tof:barrel outter Tof


  NTuplePtr nt11(ntupleSvc(), "FILE1/pid");
  if ( nt11 ) m_tuple11 = nt11;
  else {
    m_tuple11 = ntupleSvc()->book ("FILE1/pid", CLID_ColumnWiseTuple, "ks N-Tuple example");
    if ( m_tuple11 )    {
      status = m_tuple11->addItem ("ptrk",   m_ptrk_pid);
      status = m_tuple11->addItem ("cost",   m_cost_pid);
      status = m_tuple11->addItem ("dedx",   m_dedx_pid);
      status = m_tuple11->addItem ("tof1",   m_tof1_pid);
      status = m_tuple11->addItem ("tof2",   m_tof2_pid);
      status = m_tuple11->addItem ("prob",   m_prob_pid);
    }
    else    { 
      log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple11) << endmsg;
      return StatusCode::FAILURE;
    }
  }


  //
  //--------end of book--------
  //

  log << MSG::INFO << "successfully return from initialize()" <<endmsg;
  return StatusCode::SUCCESS;

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode Rhopi::execute() {
  
  std::cout << "execute()" << std::endl;

  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in execute()" << endreq;

  SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
  int runNo=eventHeader->runNumber();
  int event=eventHeader->eventNumber();
  log << MSG::DEBUG <<"run, evtnum = "
      << runNo << " , "
      << event <<endreq;
  cout<<"event "<<event<<endl;
  Ncut0++;

  SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);
  //  log << MSG::INFO << "get event tag OK" << endreq;
    log << MSG::DEBUG <<"ncharg, nneu, tottks = " 
      << evtRecEvent->totalCharged() << " , "
      << evtRecEvent->totalNeutral() << " , "
      << evtRecEvent->totalTracks() <<endreq;

  SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(),  EventModel::EvtRec::EvtRecTrackCol);
  //
  // check x0, y0, z0, r0
  // suggest cut: |z0|<5 && r0<1
  //
  Vint iGood, ipip, ipim;
  iGood.clear();
  ipip.clear();
  ipim.clear();
  Vp4 ppip, ppim;
  ppip.clear();
  ppim.clear();

  int nCharge = 0;

  Hep3Vector xorigin(0,0,0);
  
  //if (m_reader.isRunNumberValid(runNo)) {
   IVertexDbSvc*  vtxsvc;
  Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
  if(vtxsvc->isVertexValid()){
  double* dbv = vtxsvc->PrimaryVertex(); 
  double*  vv = vtxsvc->SigmaPrimaryVertex();  
//    HepVector dbv = m_reader.PrimaryVertex(runNo);
//    HepVector vv = m_reader.SigmaPrimaryVertex(runNo);
    xorigin.setX(dbv[0]);
    xorigin.setY(dbv[1]);
    xorigin.setZ(dbv[2]);
  }

  for(int i = 0; i < evtRecEvent->totalCharged(); i++){
    EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;
    if(!(*itTrk)->isMdcTrackValid()) continue;
    RecMdcTrack *mdcTrk = (*itTrk)->mdcTrack();
    double pch=mdcTrk->p();
    double x0=mdcTrk->x();
    double y0=mdcTrk->y();
    double z0=mdcTrk->z();
    double phi0=mdcTrk->helix(1);
    double xv=xorigin.x();
    double yv=xorigin.y();
    double Rxy=(x0-xv)*cos(phi0)+(y0-yv)*sin(phi0);
    m_vx0 = x0;
    m_vy0 = y0;
    m_vz0 = z0;
    m_vr0 = Rxy;

    HepVector a = mdcTrk->helix();
    HepSymMatrix Ea = mdcTrk->err();
    HepPoint3D point0(0.,0.,0.);   // the initial point for MDC recosntruction
    HepPoint3D IP(xorigin[0],xorigin[1],xorigin[2]); 
    VFHelix helixip(point0,a,Ea); 
    helixip.pivot(IP);
    HepVector vecipa = helixip.a();
    double  Rvxy0=fabs(vecipa[0]);  //the nearest distance to IP in xy plane
    double  Rvz0=vecipa[3];         //the nearest distance to IP in z direction
    double  Rvphi0=vecipa[1];
    m_rvxy0=Rvxy0;
    m_rvz0=Rvz0;
    m_rvphi0=Rvphi0;

    m_tuple1->write();
//    if(fabs(z0) >= m_vz0cut) continue;
//    if(fabs(Rxy) >= m_vr0cut) continue;
    
    if(fabs(Rvz0) >= 10.0) continue;
    if(fabs(Rvxy0) >= 1.0) continue;
    
    iGood.push_back(i);
    nCharge += mdcTrk->charge();
  }
  
  //
  // Finish Good Charged Track Selection
  //
  int nGood = iGood.size();
  log << MSG::DEBUG << "ngood, totcharge = " << nGood << " , " << nCharge << endreq;
  if((nGood != 2)||(nCharge!=0)){
    return StatusCode::SUCCESS;
  }
  Ncut1++;

  Vint iGam;
  iGam.clear();
  for(int i = evtRecEvent->totalCharged(); i< evtRecEvent->totalTracks(); i++) {
    EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;
    if(!(*itTrk)->isEmcShowerValid()) continue;
    RecEmcShower *emcTrk = (*itTrk)->emcShower();
    Hep3Vector emcpos(emcTrk->x(), emcTrk->y(), emcTrk->z());
    // find the nearest charged track
    double dthe = 200.;
    double dphi = 200.;
    double dang = 200.; 
    for(int j = 0; j < evtRecEvent->totalCharged(); j++) {
      EvtRecTrackIterator jtTrk = evtRecTrkCol->begin() + j;
      if(!(*jtTrk)->isExtTrackValid()) continue;
      RecExtTrack *extTrk = (*jtTrk)->extTrack();
      if(extTrk->emcVolumeNumber() == -1) continue;
      Hep3Vector extpos = extTrk->emcPosition();
      //      double ctht = extpos.cosTheta(emcpos);
      double angd = extpos.angle(emcpos);
      double thed = extpos.theta() - emcpos.theta();
      double phid = extpos.deltaPhi(emcpos);
      thed = fmod(thed+CLHEP::twopi+CLHEP::twopi+pi, CLHEP::twopi) - CLHEP::pi;
      phid = fmod(phid+CLHEP::twopi+CLHEP::twopi+pi, CLHEP::twopi) - CLHEP::pi;
      if(angd < dang){
        dang = angd;
        dthe = thed;
        dphi = phid;
      }
    }
    if(dang>=200) continue;
    double eraw = emcTrk->energy();
    dthe = dthe * 180 / (CLHEP::pi);
    dphi = dphi * 180 / (CLHEP::pi);
    dang = dang * 180 / (CLHEP::pi);
    m_dthe = dthe;
    m_dphi = dphi;
    m_dang = dang;
    m_eraw = eraw;
    m_tuple2->write();
    if(eraw < m_energyThreshold) continue;
//    if((fabs(dthe) < m_gammaThetaCut) && (fabs(dphi)<m_gammaPhiCut) ) continue;
    if(fabs(dang) < m_gammaAngleCut) continue;
    //
    // good photon cut will be set here
    //
    iGam.push_back(i);
  }
  
  //
  // Finish Good Photon Selection
  //
  int nGam = iGam.size();

  log << MSG::DEBUG << "num Good Photon " << nGam  << " , " <<evtRecEvent->totalNeutral()<<endreq;
  if(nGam<2){
    return StatusCode::SUCCESS;
  }
  Ncut2++;



  //
  //
  // check dedx infomation
  //
  //
  
  if(m_checkDedx == 1) {
    for(int i = 0; i < nGood; i++) {
      EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
      if(!(*itTrk)->isMdcTrackValid()) continue;
      if(!(*itTrk)->isMdcDedxValid())continue;
      RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack();
      RecMdcDedx* dedxTrk = (*itTrk)->mdcDedx();
      m_ptrk = mdcTrk->p();

      m_chie = dedxTrk->chiE();
      m_chimu = dedxTrk->chiMu();
      m_chipi = dedxTrk->chiPi();
      m_chik = dedxTrk->chiK();
      m_chip = dedxTrk->chiP();
      m_ghit = dedxTrk->numGoodHits();
      m_thit = dedxTrk->numTotalHits();
      m_probPH = dedxTrk->probPH();
      m_normPH = dedxTrk->normPH();
      m_tuple7->write();
    }
  }

  //
  // check TOF infomation
  //


  if(m_checkTof == 1) {
    for(int i = 0; i < nGood; i++) {
      EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
      if(!(*itTrk)->isMdcTrackValid()) continue;
      if(!(*itTrk)->isTofTrackValid()) continue;

      RecMdcTrack * mdcTrk = (*itTrk)->mdcTrack();
      SmartRefVector<RecTofTrack> tofTrkCol = (*itTrk)->tofTrack();

      double ptrk = mdcTrk->p();

      SmartRefVector<RecTofTrack>::iterator iter_tof = tofTrkCol.begin();
      for(;iter_tof != tofTrkCol.end(); iter_tof++ ) { 
        TofHitStatus *status = new TofHitStatus; 
        status->setStatus((*iter_tof)->status());
        if(!(status->is_barrel())){//endcap
          if( !(status->is_counter()) ) continue; // ? 
          if( status->layer()!=0 ) continue;//layer1
          double path=(*iter_tof)->path(); // ? 
          double tof  = (*iter_tof)->tof();
          double ph   = (*iter_tof)->ph();
          double rhit = (*iter_tof)->zrhit();
          double qual = 0.0 + (*iter_tof)->quality();
          double cntr = 0.0 + (*iter_tof)->tofID();
          double texp[5];
          for(int j = 0; j < 5; j++) {
            double gb = ptrk/xmass[j];
            double beta = gb/sqrt(1+gb*gb);
            texp[j] = 10 * path /beta/velc;
          }
          m_cntr_etof  = cntr;
          m_ptot_etof = ptrk;
          m_ph_etof   = ph;
          m_rhit_etof  = rhit;
          m_qual_etof  = qual;
          m_te_etof    = tof - texp[0];
          m_tmu_etof   = tof - texp[1];
          m_tpi_etof   = tof - texp[2];
          m_tk_etof    = tof - texp[3];
          m_tp_etof    = tof - texp[4];
          m_tuple8->write();
        }
        else {//barrel
          if( !(status->is_counter()) ) continue; // ? 
          if(status->layer()==1){ //layer1
            double path=(*iter_tof)->path(); // ? 
            double tof  = (*iter_tof)->tof();
            double ph   = (*iter_tof)->ph();
            double rhit = (*iter_tof)->zrhit();
            double qual = 0.0 + (*iter_tof)->quality();
            double cntr = 0.0 + (*iter_tof)->tofID();
            double texp[5];
            for(int j = 0; j < 5; j++) {
              double gb = ptrk/xmass[j];
              double beta = gb/sqrt(1+gb*gb);
              texp[j] = 10 * path /beta/velc;
            }
 
            m_cntr_btof1  = cntr;
            m_ptot_btof1 = ptrk;
            m_ph_btof1   = ph;
            m_zhit_btof1  = rhit;
            m_qual_btof1  = qual;
            m_te_btof1    = tof - texp[0];
            m_tmu_btof1   = tof - texp[1];
            m_tpi_btof1   = tof - texp[2];
            m_tk_btof1    = tof - texp[3];
            m_tp_btof1    = tof - texp[4];
            m_tuple9->write();
          }

          if(status->layer()==2){//layer2
            double path=(*iter_tof)->path(); // ? 
            double tof  = (*iter_tof)->tof();
            double ph   = (*iter_tof)->ph();
            double rhit = (*iter_tof)->zrhit();
            double qual = 0.0 + (*iter_tof)->quality();
            double cntr = 0.0 + (*iter_tof)->tofID();
            double texp[5];
            for(int j = 0; j < 5; j++) {
              double gb = ptrk/xmass[j];
              double beta = gb/sqrt(1+gb*gb);
              texp[j] = 10 * path /beta/velc;
            }
 
            m_cntr_btof2  = cntr;
            m_ptot_btof2 = ptrk;
            m_ph_btof2   = ph;
            m_zhit_btof2  = rhit;
            m_qual_btof2  = qual;
            m_te_btof2    = tof - texp[0];
            m_tmu_btof2   = tof - texp[1];
            m_tpi_btof2   = tof - texp[2];
            m_tk_btof2    = tof - texp[3];
            m_tp_btof2    = tof - texp[4];
            m_tuple10->write();
          } 
        }

        delete status; 
      } 
    } // loop all charged track
  }  // check tof


  //
  // Assign 4-momentum to each photon
  // 

  Vp4 pGam;
  pGam.clear();
  for(int i = 0; i < nGam; i++) {
    EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + iGam[i]; 
    RecEmcShower* emcTrk = (*itTrk)->emcShower();
    double eraw = emcTrk->energy();
    double phi = emcTrk->phi();
    double the = emcTrk->theta();
    HepLorentzVector ptrk;
    ptrk.setPx(eraw*sin(the)*cos(phi));
    ptrk.setPy(eraw*sin(the)*sin(phi));
    ptrk.setPz(eraw*cos(the));
    ptrk.setE(eraw);

//    ptrk = ptrk.boost(-0.011,0,0);// boost to cms

    pGam.push_back(ptrk);
  }
  cout<<"before pid"<<endl;
  //
  // Assign 4-momentum to each charged track
  //
  ParticleID *pid = ParticleID::instance();
  for(int i = 0; i < nGood; i++) {
    EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + iGood[i]; 
    //    if(pid) delete pid;
    pid->init();
    pid->setMethod(pid->methodProbability());
//    pid->setMethod(pid->methodLikelihood());  //for Likelihood Method  

    pid->setChiMinCut(4);
    pid->setRecTrack(*itTrk);
    pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTofE()); // use PID sub-system
    pid->identify(pid->onlyPion() | pid->onlyKaon());    // seperater Pion/Kaon
    //    pid->identify(pid->onlyPion());
    //  pid->identify(pid->onlyKaon());
    pid->calculate();
    if(!(pid->IsPidInfoValid())) continue;
    RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack();
    m_ptrk_pid = mdcTrk->p();
    m_cost_pid = cos(mdcTrk->theta());
    m_dedx_pid = pid->chiDedx(2);
    m_tof1_pid = pid->chiTof1(2);
    m_tof2_pid = pid->chiTof2(2);
    m_prob_pid = pid->probPion();
    m_tuple11->write();

//  if(pid->probPion() < 0.001 || (pid->probPion() < pid->probKaon())) continue;
    if(pid->probPion() < 0.001) continue;
//    if(pid->pdf(2)<pid->pdf(3)) continue; //  for Likelihood Method(0=electron 1=muon 2=pion 3=kaon 4=proton) 

    RecMdcKalTrack* mdcKalTrk = (*itTrk)->mdcKalTrack();//After ParticleID, use RecMdcKalTrack substitute RecMdcTrack
    RecMdcKalTrack::setPidType  (RecMdcKalTrack::pion);//PID can set to electron, muon, pion, kaon and proton;The default setting is pion

    if(mdcKalTrk->charge() >0) {
      ipip.push_back(iGood[i]);
      HepLorentzVector ptrk;
      ptrk.setPx(mdcKalTrk->px());
      ptrk.setPy(mdcKalTrk->py());
      ptrk.setPz(mdcKalTrk->pz());
      double p3 = ptrk.mag();
      ptrk.setE(sqrt(p3*p3+mpi*mpi));

//      ptrk = ptrk.boost(-0.011,0,0);//boost to cms

      ppip.push_back(ptrk);
    } else {
      ipim.push_back(iGood[i]);
      HepLorentzVector ptrk;
      ptrk.setPx(mdcKalTrk->px());
      ptrk.setPy(mdcKalTrk->py());
      ptrk.setPz(mdcKalTrk->pz());
      double p3 = ptrk.mag();
      ptrk.setE(sqrt(p3*p3+mpi*mpi));

//      ptrk = ptrk.boost(-0.011,0,0);//boost to cms

      ppim.push_back(ptrk);
    }
  }

/*
  for(int i = 0; i < nGood; i++) {//for rhopi without PID
    EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + iGood[i];
    RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack(); 
    if(mdcTrk->charge() >0) {
      ipip.push_back(iGood[i]);
      HepLorentzVector ptrk;
      ptrk.setPx(mdcTrk->px());
      ptrk.setPy(mdcTrk->py());
      ptrk.setPz(mdcTrk->pz());
      double p3 = ptrk.mag();
      ptrk.setE(sqrt(p3*p3+mpi*mpi));
      ppip.push_back(ptrk);
    } else {
      ipim.push_back(iGood[i]);
      HepLorentzVector ptrk;
      ptrk.setPx(mdcTrk->px());
      ptrk.setPy(mdcTrk->py());
      ptrk.setPz(mdcTrk->pz());
      double p3 = ptrk.mag();
      ptrk.setE(sqrt(p3*p3+mpi*mpi));
      ppim.push_back(ptrk);
    }
  }// without PID
*/

  int npip = ipip.size();
  int npim = ipim.size();
  if(npip*npim != 1) return SUCCESS;

  Ncut3++;


  //
  // Loop each gamma pair, check ppi0 and pTot 
  //

  HepLorentzVector pTot;
  for(int i = 0; i < nGam - 1; i++){
    for(int j = i+1; j < nGam; j++) {
      HepLorentzVector p2g = pGam[i] + pGam[j];
      pTot = ppip[0] + ppim[0];
      pTot += p2g;
      m_m2gg = p2g.m();
      m_etot = pTot.e();
      m_tuple3 -> write();

    }
  }
  
  
  RecMdcKalTrack *pipTrk = (*(evtRecTrkCol->begin()+ipip[0]))->mdcKalTrack();
  RecMdcKalTrack *pimTrk = (*(evtRecTrkCol->begin()+ipim[0]))->mdcKalTrack();

  WTrackParameter wvpipTrk, wvpimTrk;
  wvpipTrk = WTrackParameter(mpi, pipTrk->getZHelix(), pipTrk->getZError());
  wvpimTrk = WTrackParameter(mpi, pimTrk->getZHelix(), pimTrk->getZError());

/* Default is pion, for other particles:
  wvppTrk = WTrackParameter(mp, pipTrk->getZHelixP(), pipTrk->getZErrorP());//proton
  wvmupTrk = WTrackParameter(mmu, pipTrk->getZHelixMu(), pipTrk->getZErrorMu());//muon
  wvepTrk = WTrackParameter(me, pipTrk->getZHelixE(), pipTrk->getZErrorE());//electron
  wvkpTrk = WTrackParameter(mk, pipTrk->getZHelixK(), pipTrk->getZErrorK());//kaon
*/
  //
  //    Test vertex fit
  //

  HepPoint3D vx(0., 0., 0.);
  HepSymMatrix Evx(3, 0);
  double bx = 1E+6;
  double by = 1E+6;
  double bz = 1E+6;
  Evx[0][0] = bx*bx;
  Evx[1][1] = by*by;
  Evx[2][2] = bz*bz;

  VertexParameter vxpar;
  vxpar.setVx(vx);
  vxpar.setEvx(Evx);
  
  VertexFit* vtxfit = VertexFit::instance();
  vtxfit->init();
  vtxfit->AddTrack(0,  wvpipTrk);
  vtxfit->AddTrack(1,  wvpimTrk);
  vtxfit->AddVertex(0, vxpar,0, 1);
  if(!vtxfit->Fit(0)) return SUCCESS;
  vtxfit->Swim(0);
  
  WTrackParameter wpip = vtxfit->wtrk(0);
  WTrackParameter wpim = vtxfit->wtrk(1);

  //KinematicFit * kmfit = KinematicFit::instance();
  KalmanKinematicFit * kmfit = KalmanKinematicFit::instance();

  //
  //  Apply Kinematic 4C fit
  // 
  cout<<"before 4c"<<endl;
  if(m_test4C==1) {
//    double ecms = 3.097;
    HepLorentzVector ecms(0.034,0,0,3.097);

    double chisq = 9999.;
    int ig1 = -1;
    int ig2 = -1;
    for(int i = 0; i < nGam-1; i++) {
      RecEmcShower *g1Trk = (*(evtRecTrkCol->begin()+iGam[i]))->emcShower();
      for(int j = i+1; j < nGam; j++) {
	RecEmcShower *g2Trk = (*(evtRecTrkCol->begin()+iGam[j]))->emcShower();
	kmfit->init();
	kmfit->AddTrack(0, wpip);
	kmfit->AddTrack(1, wpim);
	kmfit->AddTrack(2, 0.0, g1Trk);
	kmfit->AddTrack(3, 0.0, g2Trk);
	kmfit->AddFourMomentum(0, ecms);
	bool oksq = kmfit->Fit();
	if(oksq) {
	  double chi2 = kmfit->chisq();
	  if(chi2 < chisq) {
	    chisq = chi2;
	    ig1 = iGam[i];
	    ig2 = iGam[j];
	  }
	}
      }
    }
    
    if(chisq < 200) { 

      RecEmcShower *g1Trk = (*(evtRecTrkCol->begin()+ig1))->emcShower();
      RecEmcShower *g2Trk = (*(evtRecTrkCol->begin()+ig2))->emcShower();
      kmfit->init();
      kmfit->AddTrack(0, wpip);
      kmfit->AddTrack(1, wpim);
      kmfit->AddTrack(2, 0.0, g1Trk);
      kmfit->AddTrack(3, 0.0, g2Trk);
      kmfit->AddFourMomentum(0, ecms);
      bool oksq = kmfit->Fit();
      if(oksq) {
	HepLorentzVector ppi0 = kmfit->pfit(2) + kmfit->pfit(3);
	m_mpi0 = ppi0.m();
	m_chi1 = kmfit->chisq();
	m_tuple4->write();
        Ncut4++;
      }
    }
  }
  
  //
  //  Apply Kinematic 5C Fit
  //

  // find the best combination over all possible pi+ pi- gamma gamma pair
  if(m_test5C==1) {
//    double ecms = 3.097;
    HepLorentzVector ecms(0.034,0,0,3.097);
    double chisq = 9999.;
    int ig1 = -1;
    int ig2 = -1;
    for(int i = 0; i < nGam-1; i++) {
      RecEmcShower *g1Trk = (*(evtRecTrkCol->begin()+iGam[i]))->emcShower();
      for(int j = i+1; j < nGam; j++) {
	RecEmcShower *g2Trk = (*(evtRecTrkCol->begin()+iGam[j]))->emcShower();
	kmfit->init();
	kmfit->AddTrack(0, wpip);
	kmfit->AddTrack(1, wpim);
	kmfit->AddTrack(2, 0.0, g1Trk);
	kmfit->AddTrack(3, 0.0, g2Trk);
	kmfit->AddResonance(0, 0.135, 2, 3);
	kmfit->AddFourMomentum(1, ecms);
	if(!kmfit->Fit(0)) continue;
	if(!kmfit->Fit(1)) continue;
	bool oksq = kmfit->Fit();
	if(oksq) {
	  double chi2 = kmfit->chisq();
	  if(chi2 < chisq) {
	    chisq = chi2;
	    ig1 = iGam[i];
	    ig2 = iGam[j];
	  }
	}
      }
    }
  

    log << MSG::INFO << " chisq = " << chisq <<endreq;

    if(chisq < 200) {
      RecEmcShower* g1Trk = (*(evtRecTrkCol->begin()+ig1))->emcShower();
      RecEmcShower* g2Trk = (*(evtRecTrkCol->begin()+ig2))->emcShower();

      kmfit->init();
      kmfit->AddTrack(0, wpip);
      kmfit->AddTrack(1, wpim);
      kmfit->AddTrack(2, 0.0, g1Trk);
      kmfit->AddTrack(3, 0.0, g2Trk);
      kmfit->AddResonance(0, 0.135, 2, 3);
      kmfit->AddFourMomentum(1, ecms);
      bool oksq = kmfit->Fit();
      if(oksq){
	HepLorentzVector ppi0 = kmfit->pfit(2) + kmfit->pfit(3);
	HepLorentzVector prho0 = kmfit->pfit(0) + kmfit->pfit(1);
	HepLorentzVector prhop = ppi0 + kmfit->pfit(0);
	HepLorentzVector prhom = ppi0 + kmfit->pfit(1);
	
	m_chi2  = kmfit->chisq();
	m_mrh0 = prho0.m();
	m_mrhp = prhop.m();
	m_mrhm = prhom.m();
	double eg1 = (kmfit->pfit(2)).e();
	double eg2 = (kmfit->pfit(3)).e();
	double fcos = abs(eg1-eg2)/ppi0.rho();
	m_tuple5->write();
        Ncut5++;
	// 
	//  Measure the photon detection efficiences via
	//          J/psi -> rho0 pi0
	//
	if(fabs(prho0.m()-0.770)<0.150) {  
	  if(fabs(fcos)<0.99) {
	    m_fcos = (eg1-eg2)/ppi0.rho();
	    m_elow =  (eg1 < eg2) ? eg1 : eg2;
	    m_tuple6->write();
            Ncut6++;
	  }
	} // rho0 cut
      }  //oksq
    } 
  }
  return StatusCode::SUCCESS;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode Rhopi::finalize() {
  cout<<"total number:         "<<Ncut0<<endl;
  cout<<"nGood==2, nCharge==0: "<<Ncut1<<endl;
  cout<<"nGam>=2:              "<<Ncut2<<endl;
  cout<<"Pass Pid:             "<<Ncut3<<endl;
  cout<<"Pass 4C:              "<<Ncut4<<endl;
  cout<<"Pass 5C:              "<<Ncut5<<endl;
  cout<<"J/psi->rho0 pi0:      "<<Ncut6<<endl;
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in finalize()" << endmsg;
  return StatusCode::SUCCESS;
}

