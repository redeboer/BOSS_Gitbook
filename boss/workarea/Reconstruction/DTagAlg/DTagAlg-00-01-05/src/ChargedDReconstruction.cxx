//
// All D+ decay modes Reconstruction 
//
#include <fstream>
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"


#include "EventModel/EventHeader.h"
#include "EventModel/Event.h"
#include "EventModel/EventModel.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecDTag.h"
#include "EvtRecEvent/EvtRecPi0.h"
#include "EvtRecEvent/EvtRecEtaToGG.h"
#include "EvtRecEvent/EvtRecVeeVertex.h"

#include "BesDChain/CDChargedPionList.h"
#include "BesDChain/CDChargedKaonList.h"
#include "BesDChain/CDPhotonList.h"
#include "BesDChain/CDPi0List.h"
#include "BesDChain/CDEtaList.h"
#include "BesDChain/CDKsList.h"
#include "BesDChain/CDDecayList.h"

#include "McTruth/McParticle.h"
#include "ParticleID/ParticleID.h"
#include "VertexFit/VertexFit.h"
#include "VertexFit/SecondVertexFit.h"
#include "VertexFit/IVertexDbSvc.h"

#include "DTagAlg/ChargedDReconstruction.h"
#include "DTagAlg/LocalKaonSelector.h"
#include "DTagAlg/LocalPionSelector.h"
#include "DTagAlg/LocalPhotonSelector.h"
#include "DTagAlg/LocalKsSelector.h"
#include "DTagAlg/LocalPi0Selector.h"
#include "DTagAlg/LocalEtatoGGSelector.h"
#include "DTagAlg/LocalEptoPiPiEtaSelector.h"
#include "DTagAlg/LocalEptoRhoGamSelector.h"
#include "DTagAlg/LocalRhotoPiPiSelector.h"
#include "DTagAlg/ChargedDSelector.h"


#include "DTagAlg/utility.h"

using namespace Event;

//*******************************************************************************************
ChargedDReconstruction::ChargedDReconstruction(const std::string& name, ISvcLocator* pSvcLocator):
Algorithm(name, pSvcLocator)  {
  //Declare the properties
  declareProperty( "debug",           m_debug = false );
  declareProperty( "ReadBeamEFromDB", m_ReadBeamEFromDB = false );
  declareProperty( "UseCalibBeamE",   m_usecalibBeamE = false );
  declareProperty( "UseVertexfit",    m_usevertexfit = false );
  declareProperty( "BeamE",           m_beamE = 1.8865 ); 
  declareProperty( "DpList",          m_decaylist = "test.txt" );
}

//******************************************************************************************
StatusCode ChargedDReconstruction::initialize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in initialize()" <<endreq;
  
  m_irun=-100;
  m_beta.setX(0.011);
  m_beta.setY(0);
  m_beta.setZ(0);
  chanlist=getlist(m_decaylist);

  return StatusCode::SUCCESS;
}

//********************************************************************************************
StatusCode ChargedDReconstruction::finalize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in finalize()" << endreq;

  chanlist.clear();
  
  return StatusCode::SUCCESS;
}

StatusCode ChargedDReconstruction::registerEvtRecDTagCol(
    EvtRecDTagCol* aNewEvtRecDTagCol, MsgStream& log) {
  StatusCode sc = eventSvc()->registerObject("/Event/EvtRec/EvtRecDTagCol",
                                             aNewEvtRecDTagCol);
  if (sc != StatusCode::SUCCESS) {
    log << MSG::FATAL << "Could not register EvtRecDTagCol in TDS!" << endreq;
  }
  return sc;
}

//*********************************************************************************************
StatusCode ChargedDReconstruction::execute() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in execute()" << endreq;
  
  StatusCode sc;

  //////////////////
  // Read REC data
  /////////////////
  SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
  int event= eventHeader->eventNumber();
  // if ( m_debug || ( (event & 0x3FF) == 0 ) )
  //std::cout << "event: " << event << std::endl;

  SmartDataPtr<EvtRecEvent> recEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);
  SmartDataPtr<EvtRecTrackCol> recTrackCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol);
  log << MSG::DEBUG << "run and event = " << eventHeader->runNumber()
                                        << " " << eventHeader->eventNumber() << endreq;
  log << MSG::DEBUG <<"ncharg, nneu, tottks = "
          << recEvent->totalCharged() << " , "
          << recEvent->totalNeutral() << " , "
      << recEvent->totalTracks() <<endreq;
  
  EvtRecTrackIterator charged_begin = recTrackCol->begin();
  EvtRecTrackIterator charged_end = charged_begin + recEvent->totalCharged();

  EvtRecTrackIterator neutral_begin = recTrackCol->begin()+recEvent->totalCharged();
  EvtRecTrackIterator neutral_end = recTrackCol->begin()+recEvent->totalTracks();

  
  SmartDataPtr<EvtRecPi0Col> recPi0Col(eventSvc(), "/Event/EvtRec/EvtRecPi0Col");
  if ( ! recPi0Col ) {
    log << MSG::FATAL << "Could not find EvtRecPi0Col" << endreq;
    return StatusCode::FAILURE;
  }


  SmartDataPtr<EvtRecEtaToGGCol> recEtaToGGCol(eventSvc(), "/Event/EvtRec/EvtRecEtaToGGCol");
  if ( ! recEtaToGGCol ) {
    log << MSG::FATAL << "Could not find EvtRecEtaToGGCol" << endreq;
    return StatusCode::FAILURE;
  }
  

  SmartDataPtr<EvtRecVeeVertexCol> recVeeVertexCol(eventSvc(), "/Event/EvtRec/EvtRecVeeVertexCol");
   if ( ! recVeeVertexCol ) {
     log << MSG::FATAL << "Could not find EvtRecVeeVertexCol" << endreq;
     return StatusCode::FAILURE;
   }
   

  SmartDataPtr<EvtRecDTagCol> recDTagCol(eventSvc(), EventModel::EvtRec::EvtRecDTagCol);
  if (!recDTagCol) {
    log << MSG::FATAL << "EvtRecDTagCol is not registered yet" << endreq;
    return StatusCode::FAILURE;
  }

  

  //get primary vertex from db
  Hep3Vector xorigin(0,0,0);
  
  IVertexDbSvc*  vtxsvc;
  Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
  if (vtxsvc->isVertexValid()) {

    //vertex[0] = vx; vertex[1]= vy; vertex[2] = vz;
    double* vertex = vtxsvc->PrimaryVertex();
    xorigin.setX(vertex[0]);
    xorigin.setY(vertex[1]);
    xorigin.setZ(vertex[2]);
  }
  
  utility util;
  

  //registered in DTag.cxx
  /*
  if (!recDTagCol) {
    recDTagCol = new EvtRecDTagCol;
    sc = registerEvtRecDTagCol(recDTagCol, log);
    if (sc != StatusCode::SUCCESS) {
      return sc;
    }
  }
  */
  
  /////////////////////////////
  //reconstruct particle lists
  /////////////////////////////
  pionSelector.setpidtype(0);
  kaonSelector.setpidtype(0);
  CDChargedPionList pionList(charged_begin, charged_end, pionSelector);
  CDChargedKaonList kaonList(charged_begin, charged_end, kaonSelector);

  CDKsList          ksList(ksSelector);
  dc_fill(ksList,  recVeeVertexCol->begin(), recVeeVertexCol->end());

  // do a secondary vertex fit and cut on the results
  map<EvtRecVeeVertex*, vector< double > > fitinfo;
  for( CDKsList::iterator ksit = ksList.particle_begin(); ksit != ksList.particle_end(); ++ksit ){
    EvtRecVeeVertex* ks = const_cast<EvtRecVeeVertex*>( (*ksit).particle().navKshort() );
    fitinfo[ks] = util.SecondaryVFit(ks, vtxsvc);
  }
  
  CDPi0List         pi0List(pi0Selector);
  dc_fill(pi0List, recPi0Col->begin(), recPi0Col->end());
  
  CDEtaList         etaList(etatoGGSelector);
  dc_fill(etaList, recEtaToGGCol->begin(), recEtaToGGCol->end());

  //pion/kaon list with PID
  pionSelector.setpidtype(1);
  kaonSelector.setpidtype(1);
  CDChargedPionList pionList_tight(charged_begin, charged_end, pionSelector);
  CDChargedKaonList kaonList_tight(charged_begin, charged_end, kaonSelector);

  

  int run = eventHeader->runNumber();
  m_ievt  = eventHeader->eventNumber();
  m_nChrg = recEvent->totalCharged();
  m_nNeu  = recEvent->totalNeutral();
  m_nPion = pionList.size();
  m_nKaon = kaonList.size();
  m_nPi0  = pi0List.size();
  m_nKs   = ksList.size();
  
  
  ///////////////////////
  // get beam energy and beta
  ///////////////////////
  

  if(m_ReadBeamEFromDB && m_irun!=run){
    m_irun=run;
    if(m_usecalibBeamE)
      m_readDb.setcalib(true);
    m_beamE=m_readDb.getbeamE(m_irun,m_beamE);
    if(run>0)
      m_beta=m_readDb.getbeta();
    // cout<<"use beam E from data base:"<<m_beamE<<endl;
  }
  double ebeam=m_beamE;
  

  //////////////////////////////
  //reconstruct decay lists
  /////////////////////////////
  
  
  for(int list=0;list<chanlist.size();list++){
    
    string channel=chanlist[list];
    vector<int> numchan;
    chargedDSelector.setebeam(ebeam);
    chargedDSelector.setbeta(m_beta);
    CDDecayList decaylist(chargedDSelector);

    //K+/-: 1, Pi+/-:2,  Pi0:3, Eta: 4, Ks:5
    //the fist element of the vector stands for decay mode,
    //the rest will be particles, and size of the vector minus 1 will be number of daughers.
    
    if(channel=="DptoKPiPi") {
      numchan.push_back( EvtRecDTag::kDptoKPiPi );
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=kaonList.minus()* pionList.plus()* pionList.plus();
    }
    else if(channel=="DptoKPiPiPi0") {
      numchan.push_back( EvtRecDTag::kDptoKPiPiPi0 );
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=kaonList.minus()* pionList.plus()* pionList.plus()* pi0List;
    }
    else if(channel=="DptoKsPi") {
      numchan.push_back( EvtRecDTag::kDptoKsPi );
      numchan.push_back(5);
      numchan.push_back(2);
      decaylist=ksList* pionList.plus();
    }
    else if(channel=="DptoKsPiPi0") {
      numchan.push_back( EvtRecDTag::kDptoKsPiPi0 );
      numchan.push_back(5);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=ksList* pionList.plus()* pi0List;
    }
    else if(channel=="DptoKsPiPiPi") {
      numchan.push_back( EvtRecDTag::kDptoKsPiPiPi );
      numchan.push_back(5);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=ksList* pionList.plus()* pionList.plus()* pionList.minus();
    }
    else if(channel=="DptoKKPi") {
      numchan.push_back( EvtRecDTag::kDptoKKPi );
      numchan.push_back(1);
      numchan.push_back(1);
      numchan.push_back(2);
      decaylist=kaonList.minus()* kaonList.plus()* pionList.plus();
    }
    else if(channel=="DptoPiPi0") {
      numchan.push_back( EvtRecDTag::kDptoPiPi0 );
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=pionList.plus()* pi0List;
    }
    else if(channel=="DptoKPi0") {
      numchan.push_back( EvtRecDTag::kDptoKPi0 );
      numchan.push_back(1);
      numchan.push_back(3);
      decaylist=kaonList.plus()* pi0List;
    }
    else if(channel=="DptoKsK") {
      numchan.push_back( EvtRecDTag::kDptoKsK );
      numchan.push_back(5);
      numchan.push_back(1);
      decaylist=ksList* kaonList.plus();
    }
    else if(channel=="DptoPiPiPi") {
      numchan.push_back( EvtRecDTag::kDptoPiPiPi );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=pionList.plus()* pionList.plus()* pionList.minus() ;
    }
    else if(channel=="DptoPiPi0Pi0") {
      numchan.push_back( EvtRecDTag::kDptoPiPi0Pi0 );
      numchan.push_back(2);
      numchan.push_back(3);
      numchan.push_back(3);
      decaylist=pionList.plus()* pi0List* pi0List;
    }
    else if(channel=="DptoKsKsPi") {
      numchan.push_back( EvtRecDTag::kDptoKsKsPi );
      numchan.push_back(5);
      numchan.push_back(5);
      numchan.push_back(2);
      decaylist=ksList* ksList* pionList.plus();
    }
    else if(channel=="DptoKsKPi0") {
      numchan.push_back( EvtRecDTag::kDptoKsKPi0 );
      numchan.push_back(5);
      numchan.push_back(1);
      numchan.push_back(3);
      decaylist=ksList* kaonList.plus()* pi0List;
    }
    else if(channel=="DptoKsKsK") {
      numchan.push_back( EvtRecDTag::kDptoKsKsK );
      numchan.push_back(5);
      numchan.push_back(5);
      numchan.push_back(1);
      decaylist=ksList* ksList* kaonList.plus();
    }
    else if(channel=="DptoPiPiPiPi0") {
      numchan.push_back( EvtRecDTag::kDptoPiPiPiPi0 );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=pionList.plus()* pionList.plus()* pionList.minus()* pi0List;
    }
    else if(channel=="DptoKsPiPi0Pi0") {
      numchan.push_back( EvtRecDTag::kDptoKsPiPi0Pi0 );
      numchan.push_back(5);
      numchan.push_back(2);
      numchan.push_back(3);
      numchan.push_back(3);
      decaylist=ksList* pionList.plus()* pi0List* pi0List;
    }
    else if(channel=="DptoKsKplusPiPi") {
      numchan.push_back( EvtRecDTag::kDptoKsKplusPiPi );
      numchan.push_back(5);
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=ksList* kaonList.plus()* pionList.plus()* pionList.minus();
    }
    else if(channel=="DptoKsKminusPiPi") {
      numchan.push_back( EvtRecDTag::kDptoKsKminusPiPi );
      numchan.push_back(5);
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=ksList* kaonList.minus()* pionList.plus()* pionList.plus();
    }
    else if(channel=="DptoKKPiPi0") {
      numchan.push_back( EvtRecDTag::kDptoKKPiPi0 );
      numchan.push_back(1);
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=kaonList.minus()* kaonList.plus()* pionList.plus()* pi0List;
    }
    else if(channel=="DptoPiPiPiPiPi") {
      numchan.push_back( EvtRecDTag::kDptoPiPiPiPiPi );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=pionList.plus()* pionList.plus()* pionList.plus()* pionList.minus()* pionList.minus();
    }
    else if(channel=="DptoKPiPiPiPi") {
      numchan.push_back( EvtRecDTag::kDptoKPiPiPiPi );
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=kaonList.minus()* pionList.plus()* pionList.plus()* pionList.plus()* pionList.minus();
    }
    else if(channel=="DptoPiEta") {
      numchan.push_back( EvtRecDTag::kDptoPiEta );
      numchan.push_back(2);
      numchan.push_back(4);
      decaylist=pionList.plus()* etaList;
    }
    else if(channel=="DptoKsPiEta") {
      numchan.push_back( EvtRecDTag::kDptoKsPiEta );
      numchan.push_back(5);
      numchan.push_back(2);
      numchan.push_back(4);
      decaylist=ksList* pionList.plus()* etaList;
    }
    
    CDDecayList::iterator D_begin =decaylist.particle_begin();
    CDDecayList::iterator D_end   =decaylist.particle_end();

    for ( CDDecayList::iterator it = D_begin; it != D_end; it++ ) {
      
      EvtRecDTag* recDTag = new EvtRecDTag;
      recDTag->setdecayMode( (EvtRecDTag::DecayMode)numchan[0] );

      vector<int> trackid, showerid;
      vector<int> kaonid, pionid;
      int numofchildren=numchan.size()-1;

      for(int i=0; i< numofchildren;i++){
	
	const CDCandidate& daughter=(*it).particle().child(i);
	
	if(numchan[i+1]==1){
	  const EvtRecTrack* track=daughter.track();
	  trackid.push_back(track->trackId());
	  kaonid.push_back(track->trackId());
	}
	else if(numchan[i+1]==2){
	  const EvtRecTrack* track=daughter.track();
	  trackid.push_back(track->trackId());
	  pionid.push_back(track->trackId());
	}
	else if ( numchan[i+1]==3){
	  const EvtRecTrack* hiEnGamma=daughter.navPi0()->hiEnGamma();
	  const EvtRecTrack* loEnGamma=daughter.navPi0()->loEnGamma();
	  showerid.push_back(hiEnGamma->trackId());
	  showerid.push_back(loEnGamma->trackId());
	}
	else if ( numchan[i+1]==4){
	  const EvtRecTrack* hiEnGamma=daughter.navEta()->hiEnGamma();
	  const EvtRecTrack* loEnGamma=daughter.navEta()->loEnGamma();
	  showerid.push_back(hiEnGamma->trackId());
	  showerid.push_back(loEnGamma->trackId());
	}
	else if ( numchan[i+1]==5){
	  EvtRecVeeVertex* aKsCand = const_cast<EvtRecVeeVertex*>( daughter.navKshort() );
	  // fill fit info
	  recDTag->addToFitInfo(aKsCand->mass(),fitinfo[aKsCand][0],fitinfo[aKsCand][1],fitinfo[aKsCand][2]);
	  // fill tracks
	  EvtRecTrack* pion1Trk = aKsCand->daughter(0);
	  EvtRecTrack* pion2Trk = aKsCand->daughter(1);    
	  trackid.push_back(pion1Trk->trackId());
	  trackid.push_back(pion2Trk->trackId());
	}
	
      }//end of filling track and shower ids
      
      
      saveDpInfo(it, ebeam, numofchildren, recDTag);
      savetrack(trackid,showerid,charged_begin,charged_end,neutral_begin,neutral_end,recDTag);
      pidtag(kaonid,pionid,kaonList_tight, pionList_tight,recDTag);
      
      if(m_usevertexfit){
        HepLorentzVector p4change_vfit=util.vfit(channel, kaonid,  pionid, xorigin, charged_begin);
        recDTag->setp4(recDTag->p4()+p4change_vfit);
      }


      trackid.clear();
      showerid.clear();
      kaonid.clear();
      pionid.clear();
      
      //write dtag object out
      recDTagCol->push_back(recDTag);
      
    }//end of decaylist iterator

    numchan.clear();

  }//end of reconstrucing all D+ decay lists

  return StatusCode::SUCCESS;
}


void  ChargedDReconstruction::saveDpInfo(CDDecayList::iterator it, double ebeam, int numofchildren, EvtRecDTag* recDTag){
  
  double mass = (*it).particle().mass();
  int charge= (*it).particle().charge();
  HepLorentzVector p4((*it).particle().momentum(), (*it).particle().energy());
  recDTag->setp4(p4);
  
  p4.boost(-m_beta);
  double mbc2_CMS = ebeam*ebeam - p4.v().mag2();
  double mbc_CMS = mbc2_CMS > 0 ? sqrt( mbc2_CMS ) : -10;
  double deltaE_CMS = p4.t() - ebeam;
  
  if((*it).particle().userTag()==1)
    recDTag->settype( EvtRecDTag::Tight );
  else
    recDTag->settype( EvtRecDTag::Loose );
  recDTag->setcharge(charge);
  recDTag->setcharm(charge);
  recDTag->setnumOfChildren(numofchildren);
  recDTag->setmass(mass);
  recDTag->setmBC(mbc_CMS);
  recDTag->setbeamE(ebeam);
  recDTag->setdeltaE(deltaE_CMS);
  
}

void  ChargedDReconstruction::savetrack(vector<int> trackid, vector<int> showerid, EvtRecTrackIterator charged_begin, EvtRecTrackIterator charged_end,
				      EvtRecTrackIterator neutral_begin, EvtRecTrackIterator neutral_end,EvtRecDTag* recDTag){

  vector<EvtRecTrackIterator> trktemp;
  vector<EvtRecTrackIterator> shrtemp;
 
  //fill tracks
  for(EvtRecTrackIterator trk=charged_begin; trk<charged_end;trk++){

    bool isothertrack=true;
    for(int i=0; i<trackid.size(); i++){
      if( (*trk)->trackId()==trackid[i] ){
	trktemp.push_back(trk);
	isothertrack=false;
	break;
      }
    }
    if(isothertrack)
      recDTag->addOtherTrack(*trk);
  }
  for(int i=0; i<trackid.size();i++){
    for(int j=0; j<trktemp.size(); j++){
      EvtRecTrackIterator trk=trktemp[j];
      if( (*trk)->trackId()==trackid[i])
	recDTag->addTrack(*trktemp[j]);
    }
  }

  
  //fill showers
  for(EvtRecTrackIterator shr=neutral_begin; shr<neutral_end;shr++){
    bool isothershower=true;
    for(int i=0; i<showerid.size(); i++){
      if( (*shr)->trackId()==showerid[i] ){
	shrtemp.push_back(shr);
	isothershower=false;
	break;
      }
    }
    if(isothershower)
      recDTag->addOtherShower(*shr);
  }

  for(int i=0; i<showerid.size();i++){
    for(int j=0; j<shrtemp.size(); j++){
      EvtRecTrackIterator shr=shrtemp[j];
      if( (*shr)->trackId()==showerid[i])
	recDTag->addShower(*shrtemp[j]);
    }
  }
  
  
}

void ChargedDReconstruction::pidtag(vector<int> kaonid, vector<int> pionid, CDChargedKaonList& kaonList, CDChargedPionList& pionList,EvtRecDTag* recDTag){

  bool iskaon=false,ispion=false;
  
  
  // save track ids which passed pion/kaon cuts

  for (CDChargedKaonList::iterator kit = kaonList.particle_begin(); kit != kaonList.particle_end(); kit++) {
    recDTag->addKaonId( (*kit).particle().track() );
  }

  for (CDChargedPionList::iterator pit = pionList.particle_begin(); pit != pionList.particle_end(); pit++) {
    recDTag->addPionId( (*pit).particle().track() );
  }
  
  
  /*
  for(int i=0; i<kaonid.size(); i++){
    bool ithkaon=false;
    for (CDChargedKaonList::iterator kit = kaonList.particle_begin(); kit != kaonList.particle_end(); kit++) {
      if((*kit).particle().track()->trackId()==kaonid[i]){
	ithkaon=true;
	break;
      }
    }
    if(!ithkaon) break;
    if(i==kaonid.size()-1)
      iskaon=true;
  }

  for(int i=0; i<pionid.size(); i++){
    bool ithpion=false;
    for (CDChargedPionList::iterator pit = pionList.particle_begin(); pit != pionList.particle_end(); pit++) {
      if((*pit).particle().track()->trackId()==pionid[i]){
	ithpion=true;
	break;
      }
    }
    if(!ithpion) break;
    if(i==pionid.size()-1)
      ispion=true;
  }
  

  if( iskaon && ispion)
    recDTag->settype( EvtRecDTag::Tight );
  else if( (kaonid.size()==0 && ispion) || (pionid.size()==0 && iskaon))
    recDTag->settype( EvtRecDTag::Tight );
  else if( kaonid.size()==0 && pionid.size()==0 )
    recDTag->settype( EvtRecDTag::Tight );
  */

}



vector<string> ChargedDReconstruction::getlist(string& filename ){

  string channel;
  vector<string> temp;
  
  ifstream inFile;
 
  inFile.open(filename.c_str());
  if (!inFile) {
    cout << "Unable to open decay list file";
    exit(1); // terminate with error
  }
    
  while (inFile >> channel) {
    temp.push_back(channel);
  }
    
  inFile.close();
  
  return temp;
  
}
