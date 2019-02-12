//psi'--> J/psi pion pion, J/psi --> di-leptons
//Kai Zhu (zhuk@ihep.ac.cn)
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"
#include "EventModel/Event.h"
#include "TrigEvent/TrigEvent.h"
#include "TrigEvent/TrigData.h"
#include "McTruth/McParticle.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "DstEvent/TofHitStatus.h"

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
#include "VertexFit/KinematicFit.h"
#include "VertexFit/VertexFit.h"
#include "VertexFit/SecondVertexFit.h"
#include "VertexFit/WTrackParameter.h"
#include "ParticleID/ParticleID.h" 
#include "MdcRecEvent/RecMdcKalTrack.h"

#include "PipiJpsiAlg/PipiJpsi.h"

#include <vector>
//const double twopi = 6.2831853;

const double me  = 0.000511;
const double mpi = 0.13957;
const double mproton = 0.938272;
const double mmu = 0.105658;
const double mpsip = 3.686;
const double xmass[5] = {0.000511, 0.105658, 0.139570,0.493677, 0.938272};
const double velc = 29.9792458;  // tof_path unit in cm.
const double PI = 3.1415926;
// const double velc = 299.792458;   // tof path unit in mm
typedef std::vector<int> Vint;
typedef std::vector<HepLorentzVector> Vp4;

// counter for efficiency
static long m_cout_all(0), m_cout_col(0), m_cout_charge(0), m_cout_nGood(0), m_cout_mom(0), m_cout_recoil(0), m_cout_everat(0);
/////////////////////////////////////////////////////////////////////////////

PipiJpsi::PipiJpsi(const std::string& name, ISvcLocator* pSvcLocator) :
  Algorithm(name, pSvcLocator) {
  //Declare the properties  
  declareProperty("Vr0cut", m_vr0cut=1.0);
  declareProperty("Vz0cut", m_vz0cut=5.0);
  declareProperty("TrackCosThetaCut",m_cosThetaCut=0.93);
  declareProperty("PipiDangCut",m_pipi_dang_cut=0.98);

  declareProperty("CheckDedx", m_checkDedx = true);
  declareProperty("CheckTof",  m_checkTof = true);

  declareProperty("Subsample", m_subsample_flag=false); 
  declareProperty("Trigger", m_trigger_flag=false); 
  declareProperty("DistinEMuon", m_distin_emuon=2.0);

  declareProperty("EventRate", m_eventrate=false);
  declareProperty("ChanDet", m_chan_det=1);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode PipiJpsi::initialize(){
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
      status = m_tuple2->addItem ("nGam",   m_nGam);
    }
    else    { 
      log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple2) << endmsg;
      return StatusCode::FAILURE;
    }
  }


  if(m_checkDedx) {
    NTuplePtr nt3(ntupleSvc(), "FILE1/dedx");
    if ( nt3 ) m_tuple3 = nt3;
    else {
      m_tuple3 = ntupleSvc()->book ("FILE1/dedx", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple3 )    {
	status = m_tuple3->addItem ("ptrk",   m_ptrk);
	status = m_tuple3->addItem ("chie",   m_chie);
	status = m_tuple3->addItem ("chimu",   m_chimu);
	status = m_tuple3->addItem ("chipi",   m_chipi);
	status = m_tuple3->addItem ("chik",   m_chik);
	status = m_tuple3->addItem ("chip",   m_chip);
	status = m_tuple3->addItem ("probPH",   m_probPH);
	status = m_tuple3->addItem ("normPH",   m_normPH);
	status = m_tuple3->addItem ("ghit",   m_ghit);
	status = m_tuple3->addItem ("thit",   m_thit);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple3) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // check dE/dx

  if(m_checkTof) {
    NTuplePtr nt4(ntupleSvc(), "FILE1/tofe");
    if ( nt4 ) m_tuple4 = nt4;
    else {
      m_tuple4 = ntupleSvc()->book ("FILE1/tofe",CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple4 )    {
	status = m_tuple4->addItem ("ptrk",   m_ptot_etof);
	status = m_tuple4->addItem ("cntr",   m_cntr_etof);
	status = m_tuple4->addItem ("path",   m_path_etof);
	status = m_tuple4->addItem ("ph",  m_ph_etof);
	status = m_tuple4->addItem ("rhit", m_rhit_etof);
	status = m_tuple4->addItem ("qual", m_qual_etof);
	status = m_tuple4->addItem ("tof",   m_tof_etof);
	status = m_tuple4->addItem ("te",   m_te_etof);
	status = m_tuple4->addItem ("tmu",   m_tmu_etof);
	status = m_tuple4->addItem ("tpi",   m_tpi_etof);
	status = m_tuple4->addItem ("tk",   m_tk_etof);
	status = m_tuple4->addItem ("tp",   m_tp_etof);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple4) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // check Tof:endcap



  if(m_checkTof) {
    NTuplePtr nt5(ntupleSvc(), "FILE1/tof1");
    if ( nt5 ) m_tuple5 = nt5;
    else {
      m_tuple5 = ntupleSvc()->book ("FILE1/tof1", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple5 )    {
	status = m_tuple5->addItem ("ptrk",   m_ptot_btof1);
	status = m_tuple5->addItem ("cntr",   m_cntr_btof1);
	status = m_tuple5->addItem ("path",   m_path_btof1);
	status = m_tuple5->addItem ("ph",  m_ph_btof1);
	status = m_tuple5->addItem ("zhit", m_zhit_btof1);
	status = m_tuple5->addItem ("qual", m_qual_btof1);
	status = m_tuple5->addItem ("tof",   m_tof_btof1);
	status = m_tuple5->addItem ("te",   m_te_btof1);
	status = m_tuple5->addItem ("tmu",   m_tmu_btof1);
	status = m_tuple5->addItem ("tpi",   m_tpi_btof1);
	status = m_tuple5->addItem ("tk",   m_tk_btof1);
	status = m_tuple5->addItem ("tp",   m_tp_btof1);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple5) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // check Tof:barrel inner Tof 


  if(m_checkTof) {
    NTuplePtr nt6(ntupleSvc(), "FILE1/tof2");
    if ( nt6 ) m_tuple6 = nt6;
    else {
      m_tuple6 = ntupleSvc()->book ("FILE1/tof2", CLID_ColumnWiseTuple, "ks N-Tuple example");
      if ( m_tuple6 )    {
	status = m_tuple6->addItem ("ptrk",   m_ptot_btof2);
	status = m_tuple6->addItem ("cntr",   m_cntr_btof2);
	status = m_tuple6->addItem ("path",   m_path_btof2);
	status = m_tuple6->addItem ("ph",  m_ph_btof2);
	status = m_tuple6->addItem ("zhit", m_zhit_btof2);
	status = m_tuple6->addItem ("qual", m_qual_btof2);
	status = m_tuple6->addItem ("tof",   m_tof_btof2);
	status = m_tuple6->addItem ("te",   m_te_btof2);
	status = m_tuple6->addItem ("tmu",   m_tmu_btof2);
	status = m_tuple6->addItem ("tpi",   m_tpi_btof2);
	status = m_tuple6->addItem ("tk",   m_tk_btof2);
	status = m_tuple6->addItem ("tp",   m_tp_btof2);
      }
      else    { 
	log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple6) << endmsg;
	return StatusCode::FAILURE;
      }
    }
  } // check Tof:barrel outter Tof

 NTuplePtr nt8(ntupleSvc(), "FILE1/infmom");
  if ( nt8 ) m_tuple8 = nt8;
  else {
    m_tuple8 = ntupleSvc()->book ("FILE1/infmom", CLID_ColumnWiseTuple, "information with momentum method");
    if ( m_tuple8 )    {
      status = m_tuple8->addItem ("momlepp", m_mom_lepp );
      status = m_tuple8->addItem ("momlepmm", m_mom_lepm );
      status = m_tuple8->addItem ("mompionm", m_mom_pionm );
      status = m_tuple8->addItem ("mompionp", m_mom_pionp );
      status = m_tuple8->addItem ("pipidang", m_pipi_dang );
      status = m_tuple8->addItem ("cmslepp", m_cms_lepp );
      status = m_tuple8->addItem ("cmslepm", m_cms_lepm );
      status = m_tuple8->addItem ("invtwopi", m_mass_twopi );
      status = m_tuple8->addItem ("invjpsi", m_mass_jpsi );
      status = m_tuple8->addItem ("recoil", m_mass_recoil);
      status = m_tuple8->addItem ("invmass", m_inv_mass );
      status = m_tuple8->addItem ("totene", m_tot_e );
      status = m_tuple8->addItem ("totpx", m_tot_px );
      status = m_tuple8->addItem ("totpy", m_tot_py );
      status = m_tuple8->addItem ("totpz", m_tot_pz );
      status = m_tuple8->addItem ("epratio", m_ep_ratio );
      status = m_tuple8->addItem ("eveflag", m_event_flag );
      status = m_tuple8->addItem ("tplepratiom", m_trans_ratio_lepm );
      status = m_tuple8->addItem ("tplepratiop", m_trans_ratio_lepp );
      status = m_tuple8->addItem ("tppionratiom", m_trans_ratio_pionm );
      status = m_tuple8->addItem ("tppionratiop", m_trans_ratio_pionp );
      status = m_tuple8->addItem ("run", m_run );
      status = m_tuple8->addItem ("event", m_event );
      status = m_tuple8->addItem ("ntrack", m_index, 0, 4 );
      status = m_tuple8->addIndexedItem ("costhe", m_index, m_cos_theta );
      status = m_tuple8->addIndexedItem ("phi", m_index, m_phi );
      status = m_tuple8->addIndexedItem ("fourmom", m_index, 4, m_four_mom);
      status = m_tuple8->addItem ("pionmat", m_pion_matched);
      status = m_tuple8->addItem ("lepmat", m_lep_matched);
      //MCtruth
      status = m_tuple8->addItem("indexmc",    m_idxmc, 0, 100);
      status = m_tuple8->addIndexedItem("pdgid",     m_idxmc, m_pdgid);
      status = m_tuple8->addIndexedItem("motheridx", m_idxmc, m_motheridx);
      status = m_tuple8->addItem("truepp", m_true_pionp);
      status = m_tuple8->addItem("truepm", m_true_pionm);
    }
    else    { 
      log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple8) << endmsg;
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
StatusCode PipiJpsi::execute() {
  
  //std::cout << "execute()" << std::endl;

  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in execute()" << endreq;
  m_cout_all ++;
  StatusCode sc=StatusCode::SUCCESS;
  //save the events passed selection to a new file
  setFilterPassed(false);

  SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
  if(!eventHeader){
    log << MSG::ERROR << "EventHeader not found" << endreq;
    return StatusCode::SUCCESS;
  }
  int run(eventHeader->runNumber());
  int event(eventHeader->eventNumber());
  if(event%1000==0) cout << "run: " << run << " event: " << event << endl;

  SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), "/Event/EvtRec/EvtRecEvent");
  if(!evtRecEvent){
    log << MSG::ERROR << "EvtRecEvent not found" << endreq;
    return StatusCode::SUCCESS;
  }
    log << MSG::DEBUG <<"ncharg, nneu, tottks = " 
      << evtRecEvent->totalCharged() << " , "
      << evtRecEvent->totalNeutral() << " , "
      << evtRecEvent->totalTracks() <<endreq;
    
  SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(), "/Event/EvtRec/EvtRecTrackCol");
  if(!evtRecTrkCol){
    log << MSG::ERROR << "EvtRecTrackCol" << endreq;
    return StatusCode::SUCCESS;
  }

  if(m_trigger_flag){
    SmartDataPtr<TrigData> trigData(eventSvc(),EventModel::Trig::TrigData);
    if (!trigData) {
      log << MSG::FATAL << "Could not find Trigger Data for physics analysis" << endreq;
      return StatusCode::FAILURE;
    }
    /// Print trigger information once:
    log << MSG::DEBUG << "Trigger conditions: " << endreq;
    for(int i=0; i < 48; i++){
      log << MSG::DEBUG << "i:" << i << "  name:" << trigData->getTrigCondName(i) << "  cond:" << trigData->getTrigCondition(i) << endreq;
    }
    // test event rate
    int m_trig_tot(0), m_trig_which(-1);
    if(m_eventrate){
      for(int j=0; j<16; j++){
	if(trigData->getTrigChannel(j)){
	  m_trig_tot ++;
	  m_trig_which = j+1;
	}
      }
      if(m_trig_tot==1 && m_trig_which==m_chan_det) m_cout_everat++;
      return sc;
    }
  }

  m_cout_col ++;
  if(evtRecEvent->totalCharged()<3 || evtRecTrkCol->size()<3 || evtRecEvent->totalTracks()>99 || evtRecTrkCol->size()>99) return StatusCode::SUCCESS;
  m_cout_charge ++;

  // Asign four-momentum with KalmanTrack
  Vint iGood; iGood.clear();
  int m_num[4]={0,0,0,0}; // number of different particles: pi+, pi-, l+, l-
  int nCharge = 0;
  m_pion_matched = 0; m_lep_matched = 0;
  HepLorentzVector m_lv_pionp, m_lv_pionm, m_lv_lepp, m_lv_lepm, m_lv_ele, m_lv_pos, m_lv_mum, m_lv_mup;

  for(int i = 0; i < evtRecEvent->totalCharged(); i++){
    EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;
    if(!(*itTrk)->isMdcKalTrackValid()) continue;
    RecMdcKalTrack* mdcTrk = (*itTrk)->mdcKalTrack(); 
  
    m_vx0 = mdcTrk->x();
    m_vy0 = mdcTrk->y();
    m_vz0 = mdcTrk->z();
    m_vr0 = mdcTrk->r();
    if(fabs(m_vz0) >= m_vz0cut) continue;
    if(m_vr0 >= m_vr0cut) continue;
    iGood.push_back(i);
    nCharge += mdcTrk->charge();
    if(mdcTrk->p()<1.0){ if((*itTrk)->isEmcShowerValid()) m_pion_matched ++; }
    else{ if((*itTrk)->isEmcShowerValid()) m_lep_matched ++; }

    if(mdcTrk->charge()>0){
      if(mdcTrk->p()<1.0){
	mdcTrk->setPidType(RecMdcKalTrack::pion);
	// Warning: for ones who do not modify the DstMdcKalTrack package, the following p4() function cannot be used, you should get momentum from MdcKalTrack, then calculate the energy by yourself
	m_lv_pionp = mdcTrk->p4(xmass[2]);  
	m_num[0] ++;
      }
      else{
	mdcTrk->setPidType(RecMdcKalTrack::electron);
	m_lv_pos = mdcTrk->p4(xmass[0]);
	mdcTrk->setPidType(RecMdcKalTrack::muon);
	m_lv_mup = mdcTrk->p4(xmass[1]);
	m_num[2] ++;
      }
    }
    else{
      if(mdcTrk->p()<1.0){
	mdcTrk->setPidType(RecMdcKalTrack::pion);
	m_lv_pionm = mdcTrk->p4(xmass[2]); 
	m_num[1] ++;
      }
      else{
	mdcTrk->setPidType(RecMdcKalTrack::electron);
	m_lv_ele = mdcTrk->p4(xmass[0]); 
	mdcTrk->setPidType(RecMdcKalTrack::muon);
	m_lv_mum = mdcTrk->p4(xmass[1]); 
	m_num[3] ++;
      }
    }
  }
  
  int nGood = iGood.size();
  log << MSG::DEBUG << "With KalmanTrack, ngood, totcharge = " << nGood << " , " << nCharge << endreq;
  if(nGood<3 || nGood>4) return sc;
  m_cout_nGood ++;

  m_ep_ratio = 0;
  for(int i=0; i< evtRecEvent->totalTracks(); i++){
    EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;
    if(!(*itTrk)->isEmcShowerValid()) continue;
    RecEmcShower *emcTrk = (*itTrk)->emcShower();
    m_ep_ratio += emcTrk->energy();
  }

  if(m_ep_ratio > m_distin_emuon){
    m_lv_lepp = m_lv_pos;
    m_lv_lepm = m_lv_ele;
  }
  else{
    m_lv_lepp = m_lv_mup;
    m_lv_lepm = m_lv_mum;
  }

  HepLorentzVector m_lv_lab(0.04,0,0,3.686);
  if(nGood==4){
    if(nCharge) return sc;
    m_event_flag = 4;
  }
  else{ 
    if(m_num[0]>1 || m_num[1]>1 || m_num[2]>1 || m_num[3]>1) return sc;
    if(m_num[0]==0){
      if(nCharge != -1) return StatusCode::SUCCESS;
      m_lv_pionp = m_lv_lab - m_lv_pionm - m_lv_lepp - m_lv_lepm;
      if(m_lv_pionp.vect().cosTheta()>m_cosThetaCut)return StatusCode::SUCCESS;
      m_event_flag = 0;
    }
    if(m_num[1]==0){
      if(nCharge != 1) return StatusCode::SUCCESS;
      m_lv_pionm = m_lv_lab - m_lv_pionp - m_lv_lepp - m_lv_lepm;
      if(m_lv_pionm.vect().cosTheta()>m_cosThetaCut)return StatusCode::SUCCESS;
      m_event_flag = 1;
    }
    if(m_num[2]==0){
      if(nCharge != -1) return StatusCode::SUCCESS;
      m_lv_lepp = m_lv_lab - m_lv_pionp - m_lv_pionm - m_lv_lepm;
      if(m_lv_lepp.vect().cosTheta()>m_cosThetaCut)return StatusCode::SUCCESS;
      m_event_flag = 2;
    }
    if(m_num[3]==0){
       if(nCharge != 1) return StatusCode::SUCCESS;
       m_lv_lepm = m_lv_lab - m_lv_pionp - m_lv_pionm - m_lv_lepp;
      if(m_lv_lepm.vect().cosTheta()>m_cosThetaCut)return StatusCode::SUCCESS;
      m_event_flag = 3;
    }
  }
  m_cout_mom ++;

  // With momentum method calculate the invariant mass of Jpsi
  // actually we use the recoil mass
  HepLorentzVector m_lv_recoil, m_lv_jpsi;
  m_lv_recoil = m_lv_lab - m_lv_pionp - m_lv_pionm;
  m_lv_jpsi = m_lv_lepp + m_lv_lepm;

  m_mass_twopi = (m_lv_pionp + m_lv_pionm).m();
  m_mass_recoil = m_lv_recoil.m();
  m_mass_jpsi = m_lv_jpsi.m();

  // Jpsi mass cut
  if( m_mass_recoil < 3.05 || m_mass_recoil > 3.15 ) return sc;
  if( m_mass_jpsi < 3.0 || m_mass_jpsi > 3.2 ) return sc;
  m_cout_recoil ++;

  HepLorentzVector m_ttm(m_lv_jpsi + m_lv_pionp + m_lv_pionm);
  if(m_ttm.m()>4 || m_ttm.m()<3) return sc;
 
  // dangle between pions, suppress gamma convertion
  m_pipi_dang = m_lv_pionp.vect().cosTheta(m_lv_pionm.vect());

  m_mom_pionp = m_lv_pionp.vect().mag();
  m_mom_pionm = m_lv_pionm.vect().mag();
  m_mom_lepp = m_lv_lepp.vect().mag();
  m_mom_lepm = m_lv_lepm.vect().mag();
  m_trans_ratio_lepp = m_lv_lepp.vect().perp()/m_lv_lepp.vect().mag();
  m_trans_ratio_lepm = m_lv_lepm.vect().perp()/m_lv_lepm.vect().mag();
  m_trans_ratio_pionp = m_lv_pionp.vect().perp()/m_lv_pionp.vect().mag();
  m_trans_ratio_pionm = m_lv_pionm.vect().perp()/m_lv_pionm.vect().mag();

  Hep3Vector m_boost_jpsi(m_lv_recoil.boostVector());
  HepLorentzVector m_lv_cms_lepp(boostOf(m_lv_lepp,-m_boost_jpsi));
  HepLorentzVector m_lv_cms_lepm(boostOf(m_lv_lepm,-m_boost_jpsi));
  m_cms_lepm = m_lv_cms_lepm.vect().mag();
  m_cms_lepp = m_lv_cms_lepp.vect().mag();
  log << MSG::DEBUG << "jpsi four momentum in cms " << m_lv_cms_lepp + m_lv_cms_lepm <<endreq;

  m_inv_mass = m_ttm.m();
  m_tot_e = m_ttm.e();
  m_tot_px = m_ttm.px();
  m_tot_py = m_ttm.py();
  m_tot_pz = m_ttm.pz();
  m_run = run;
  m_event = event;
  HepLorentzVector m_lv_book(0,0,0,0); // assume one track is missing
  for(m_index=0; m_index<4; m_index++){
    switch(m_index){
    case 0: m_lv_book = m_lv_pionp; 
      break;
    case 1: m_lv_book = m_lv_pionm; 
      break;
    case 2: m_lv_book = m_lv_lepp;
      break;
    case 3: m_lv_book = m_lv_lepm;
      break;
    default: m_lv_book.setE(2008);
    }
    m_cos_theta[m_index] = m_lv_book.vect().cosTheta();
    m_phi[m_index] = m_lv_book.vect().phi();
    m_four_mom[m_index][0] = m_lv_book.e();
    m_four_mom[m_index][1] = m_lv_book.px();
    m_four_mom[m_index][2] = m_lv_book.py();
    m_four_mom[m_index][3] = m_lv_book.pz();
  }

  if(m_subsample_flag) setFilterPassed(true);
  else if(m_mass_recoil>3.08 && m_mass_recoil<3.12 && m_mass_jpsi>3.0 && m_mass_jpsi<3.2 && m_cms_lepp<1.7 && m_cms_lepp>1.4 && m_cms_lepm<1.7 && m_cms_lepm>1.4 && m_event_flag==4 && m_pipi_dang<m_pipi_dang_cut) setFilterPassed(true);
  //cout << "passed" << endl;

  //MC information
  SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");
  if(m_run<0){
    int m_numParticle(0), m_true_pid(0);
    if(!mcParticleCol){
      log << MSG::ERROR << "Could not retrieve McParticelCol" << endreq;
      return StatusCode::FAILURE;
    }
    else{
      bool psipDecay(false);
      int rootIndex(-1);
      Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
      for (; iter_mc != mcParticleCol->end(); iter_mc++){
        if ((*iter_mc)->primaryParticle()) continue;
        if (!(*iter_mc)->decayFromGenerator()) continue;
        //if ( ((*iter_mc)->mother()).trackIndex()<3 ) continue;
        if ((*iter_mc)->particleProperty()==100443){
          psipDecay = true;
          rootIndex = (*iter_mc)->trackIndex();
        }
        if (!psipDecay) continue;
        int mcidx = ((*iter_mc)->mother()).trackIndex() - rootIndex;
        int pdgid = (*iter_mc)->particleProperty();
        m_pdgid[m_numParticle] = pdgid;
        m_motheridx[m_numParticle] = mcidx;
        m_numParticle ++;    

	//if(!(*iter_mc)->leafParticle()) continue;
	if((*iter_mc)->particleProperty() == 211) m_true_pionp = (*iter_mc)->initialFourMomentum().vect().mag();
	if((*iter_mc)->particleProperty() == -211) m_true_pionm = (*iter_mc)->initialFourMomentum().vect().mag();
      }
      m_idxmc = m_numParticle;
    }
  }

  m_tuple1->write();
  m_tuple8->write();


  // next is good photon selection
  Vint iGam;  iGam.clear();
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

      if(fabs(thed) < fabs(dthe)) dthe = thed;
      if(fabs(phid) < fabs(dphi)) dphi = phid;
      if(angd < dang) dang = angd;
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
    // if(eraw < m_energyThreshold) continue;
    // if((fabs(dthe) < m_gammaThetaCut) && (fabs(dphi)<m_gammaPhiCut) ) continue;
    // good photon cut will be set here
    iGam.push_back((*itTrk)->trackId());
  }
  // Finish Good Photon Selection
  m_nGam = iGam.size();
  log << MSG::DEBUG << "num Good Photon " << m_nGam  << " , " <<evtRecEvent->totalNeutral()<<endreq;
  m_tuple2->write();

  //
  // check dedx infomation
  //
  if(m_checkDedx) {
    int m_dedx_cout(0);
    for(int i = 0; i < nGood; i++) {
      EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
      if(!(*itTrk)->isMdcDedxValid())continue;
      RecMdcKalTrack *mdcTrk = (*itTrk)->mdcKalTrack();
      RecMdcDedx *dedxTrk = (*itTrk)->mdcDedx();
  
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

      m_tuple3->write();
    }
  }

  //
  // check TOF infomation
  //
  if(m_checkTof) {
    int m_endcap_cout(0), m_layer1_cout(0), m_layer2_cout(0);
    for(int i = 0; i < nGood; i++) {
      EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
      if(!(*itTrk)->isTofTrackValid()) continue;

      RecMdcKalTrack *mdcTrk = (*itTrk)->mdcKalTrack();
      SmartRefVector<RecTofTrack> tofTrkCol = (*itTrk)->tofTrack();

      double ptrk = mdcTrk->p();

      for( SmartRefVector<RecTofTrack>::iterator iter_tof = tofTrkCol.begin() ;iter_tof != tofTrkCol.end(); iter_tof++ ) { 
        TofHitStatus *status = new TofHitStatus; 
        status->setStatus((*iter_tof)->status());
        if(!(status->is_barrel())){//endcap
          if( !(status->is_counter()) ) continue; // ? 
          if( status->layer()!=0 ) continue;//layer1
          double path = (*iter_tof)->path(); // ? the unit is cm
          double tof  = (*iter_tof)->tof();  // the unit is ns/100
          double ph   = (*iter_tof)->ph();
          double rhit = (*iter_tof)->zrhit();
          double qual = 0.0 + (*iter_tof)->quality();
          double cntr = 0.0 + (*iter_tof)->tofID();
          double texp[5];
          for(int j = 0; j < 5; j++) {
            double gb = xmass[j]/ptrk;
            double beta = sqrt(1+gb*gb);
            texp[j] = path*beta/velc; // the unit here is ns
          }
          m_cntr_etof  = cntr;
          m_ptot_etof  = ptrk;
	  m_path_etof = path;
          m_ph_etof    = ph;
          m_rhit_etof  = rhit;
          m_qual_etof  = qual;
	  m_tof_etof = tof;
          m_te_etof    = tof - texp[0];
          m_tmu_etof   = tof - texp[1];
          m_tpi_etof   = tof - texp[2];
          m_tk_etof    = tof - texp[3];
          m_tp_etof    = tof - texp[4];

          m_tuple4->write();
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
            double gb = xmass[j]/ptrk;
            double beta = sqrt(1+gb*gb);
            texp[j] = path*beta/velc;
            }
 
            m_cntr_btof1  = cntr;
            m_ptot_btof1 = ptrk;
	    m_path_btof1 = path;
            m_ph_btof1   = ph;
            m_zhit_btof1  = rhit;
            m_qual_btof1  = qual;
	    m_tof_btof1 = tof;
            m_te_btof1    = tof - texp[0];
            m_tmu_btof1   = tof - texp[1];
            m_tpi_btof1   = tof - texp[2];
            m_tk_btof1    = tof - texp[3];
            m_tp_btof1    = tof - texp[4];

            m_tuple5->write();
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
            double gb = xmass[j]/ptrk;
            double beta = sqrt(1+gb*gb);
            texp[j] = path*beta/velc;
            }
 
            m_cntr_btof2  = cntr;
            m_ptot_btof2 = ptrk;
	    m_path_btof2 = path;
            m_ph_btof2   = ph;
            m_zhit_btof2  = rhit;
            m_qual_btof2  = qual;
	    m_tof_btof2 = tof;
            m_te_btof2    = tof - texp[0];
            m_tmu_btof2   = tof - texp[1];
            m_tpi_btof2   = tof - texp[2];
            m_tk_btof2    = tof - texp[3];
            m_tp_btof2    = tof - texp[4];

	    m_tuple6->write();
          } 
        }

        delete status; 
      } 
    } // loop all charged track
  }  // check tof
 

    return StatusCode::SUCCESS;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode PipiJpsi::finalize() {

  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in finalize()" << endmsg;
  if(m_eventrate) cout << "all event: " << m_cout_all << endl << "only channel " << m_chan_det << ": " << m_cout_everat << endl;
  cout << "all event: " << m_cout_all << endl << "all collection point is OK: " << m_cout_col << endl << "charged tracks >=3: " << m_cout_charge << endl << "good charged tracks [3,4]: " << m_cout_nGood << endl << "after momentum assign: " << m_cout_mom << endl << "after recoild mass cut: " << m_cout_recoil << endl;
  return StatusCode::SUCCESS;
}

 
