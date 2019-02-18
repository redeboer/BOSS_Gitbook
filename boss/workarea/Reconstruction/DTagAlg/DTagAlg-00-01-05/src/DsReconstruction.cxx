//
// All Ds decay modes Reconstruction 
//
#include <fstream>
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/PropertyMgr.h"

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


#include "DTagAlg/DsReconstruction.h"
#include "DTagAlg/LocalKaonSelector.h"
#include "DTagAlg/LocalPionSelector.h"
#include "DTagAlg/LocalPhotonSelector.h"
#include "DTagAlg/LocalKsSelector.h"
#include "DTagAlg/LocalPi0Selector.h"
#include "DTagAlg/LocalEtatoGGSelector.h"
#include "DTagAlg/LocalEtatoPiPiPi0Selector.h"
#include "DTagAlg/LocalEptoPiPiEtaSelector.h"
#include "DTagAlg/LocalEptoPiPiEta3PiSelector.h"
#include "DTagAlg/LocalEptoRhoGamSelector.h"
#include "DTagAlg/LocalRhotoPiPiSelector.h"
#include "DTagAlg/DsSelector.h"

#include "DTagAlg/utility.h"

using namespace Event;

//*******************************************************************************************
DsReconstruction::DsReconstruction(const std::string& name, ISvcLocator* pSvcLocator):
  Algorithm(name, pSvcLocator)  {
  //Declare the properties
  declareProperty( "debug",  m_debug = false );
  declareProperty( "ReadBeamEFromDB", m_ReadBeamEFromDB = false );
  declareProperty( "UseCalibBeamE",   m_usecalibBeamE = false );
  declareProperty( "UseVertexfit",    m_usevertexfit = false );
  declareProperty( "BeamE",  m_beamE=2.015 );
  declareProperty( "DsList",  m_decaylist = "test.txt" );
}

//******************************************************************************************
StatusCode DsReconstruction::initialize() {
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
StatusCode DsReconstruction::finalize() {
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in finalize()" << endreq;

  chanlist.clear();
  
  return StatusCode::SUCCESS;
}

StatusCode DsReconstruction::registerEvtRecDTagCol(
						   EvtRecDTagCol* aNewEvtRecDTagCol, MsgStream& log) {
  StatusCode sc = eventSvc()->registerObject("/Event/EvtRec/EvtRecDTagCol",
                                             aNewEvtRecDTagCol);
  if (sc != StatusCode::SUCCESS) {
    log << MSG::FATAL << "Could not register EvtRecDTagCol in TDS!" << endreq;
  }
  return sc;
}

//*********************************************************************************************
StatusCode DsReconstruction::execute() {
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
  CDPhotonList      photonList(neutral_begin, neutral_end, photonSelector);

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
    //cout<<"use beam E from data base:"<<m_beamE<<endl;
  }
  double ebeam=m_beamE;

  //////////////////////////////
  //reconstruct decay lists
  /////////////////////////////


  for(int list=0;list<chanlist.size();list++){

    string channel=chanlist[list];
    vector<int> numchan;
    dsSelector.setebeam(ebeam);
    dsSelector.setbeta(m_beta);
    CDDecayList decaylist(dsSelector);

    //K+/-: 1, Pi+/-:2,  Pi0:3,
    //Eta: 4, Ks:5,
    //eta'(pipieta):	6,
    //eta'(rhogamma):	7
    //eta'(pipieta(pipipi0)): 8
    //eta(pipipi0):		9,
    //the fist element of the vector stands for decay mode,
    //the rest will be particles, and size of the vector minus 1 will be number of daughers.

    if(channel=="DstoKsK") {
      numchan.push_back( EvtRecDTag::kDstoKsK );
      numchan.push_back(5);
      numchan.push_back(1);
      decaylist=ksList* kaonList.plus();
    }
    else if(channel=="DstoKKPi") {
      numchan.push_back( EvtRecDTag::kDstoKKPi );
      numchan.push_back(1);
      numchan.push_back(1);
      numchan.push_back(2);
      decaylist=kaonList.minus()* kaonList.plus()* pionList.plus();
    }
    else if(channel=="DstoKsKPi0") {
      numchan.push_back( EvtRecDTag::kDstoKsKPi0 );
      numchan.push_back(5);
      numchan.push_back(1);
      numchan.push_back(3);
      decaylist=ksList* kaonList.plus()* pi0List;
    }
    else if(channel=="DstoKsKsPi") {
      numchan.push_back( EvtRecDTag::kDstoKsKsPi );
      numchan.push_back(5);
      numchan.push_back(5);
      numchan.push_back(2);
      decaylist=ksList* ksList* pionList.plus();
    }
    else if(channel=="DstoKKPiPi0") {
      numchan.push_back( EvtRecDTag::kDstoKKPiPi0 );
      numchan.push_back(1);
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=kaonList.minus()* kaonList.plus()* pionList.plus()* pi0List;
    }
    else if(channel=="DstoKsKplusPiPi") {
      numchan.push_back( EvtRecDTag::kDstoKsKplusPiPi );
      numchan.push_back(5);
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=ksList* kaonList.plus()* pionList.plus()* pionList.minus();
    }
    else if(channel=="DstoKsKminusPiPi") {
      numchan.push_back( EvtRecDTag::kDstoKsKminusPiPi );
      numchan.push_back(5);
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=ksList* kaonList.minus()* pionList.plus()* pionList.plus();
    }
    else if(channel=="DstoKKPiPiPi") {
      numchan.push_back( EvtRecDTag::kDstoKKPiPiPi );
      numchan.push_back(1);
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=kaonList.minus()* kaonList.plus()* pionList.plus()* pionList.plus()* pionList.minus();
    }
    else if(channel=="DstoPiPi0") {
      numchan.push_back( EvtRecDTag::kDstoPiPi0 );
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=pionList.plus()* pi0List;
    }
    else if(channel=="DstoPiPiPi") {
      numchan.push_back( EvtRecDTag::kDstoPiPiPi );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=pionList.plus()* pionList.plus()* pionList.minus();
    }
    else if(channel=="DstoPiPiPiPi0") {
      numchan.push_back( EvtRecDTag::kDstoPiPiPiPi0 );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=pionList.plus()* pionList.plus()* pionList.minus()* pi0List;
    }
    else if(channel=="DstoPiPiPiPiPi") {
      numchan.push_back( EvtRecDTag::kDstoPiPiPiPiPi );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=pionList.plus()* pionList.plus()* pionList.plus()* pionList.minus()* pionList.minus();
    }
    else if(channel=="DstoPiPiPiPiPiPi0") {
      numchan.push_back( EvtRecDTag::kDstoPiPiPiPiPiPi0 );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=pionList.plus()* pionList.plus()* pionList.plus()* pionList.minus()* pionList.minus()* pi0List;
    }
    else if(channel=="DstoPiPiPiPi0Pi0") { // <------------- NEW MODE
      numchan.push_back( EvtRecDTag::kDstoPiPiPiPi0Pi0 );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(3);
      numchan.push_back(3);
      decaylist=pionList.plus()* pionList.plus()* pionList.minus()* pi0List* pi0List;
    }
    else if(channel=="DstoPiEta") {
      numchan.push_back( EvtRecDTag::kDstoPiEta );
      numchan.push_back(2);
      numchan.push_back(4);
      decaylist=pionList.plus()* etaList;
    }
    else if(channel=="DstoPiEtaPiPiPi0") { // <------------- NEW MODE
      numchan.push_back( EvtRecDTag::kDstoPiEtaPiPiPi0 );
      numchan.push_back(2);
      numchan.push_back(9);
      CDDecayList EtaList(etatoPiPiPi0Selector);
      EtaList=pionList.plus()* pionList.minus()* pi0List;
      decaylist=pionList.plus()* EtaList;
    }
    else if(channel=="DstoPiPi0Eta") {
      numchan.push_back( EvtRecDTag::kDstoPiPi0Eta );
      numchan.push_back(2);
      numchan.push_back(3);
      numchan.push_back(4);
      decaylist=pionList.plus()* pi0List* etaList;
    }
    else if(channel=="DstoPiPi0EtaPiPiPi0") { // <---------- NEW MODE
      numchan.push_back( EvtRecDTag::kDstoPiPi0EtaPiPiPi0 );
      numchan.push_back(2);
      numchan.push_back(3);
      numchan.push_back(9);
      CDDecayList EtaList(etatoPiPiPi0Selector);
      EtaList=pionList.plus()* pionList.minus()* pi0List;
      decaylist=pionList.plus()* pi0List* EtaList;
    }
    else if(channel=="DstoPiPiPiEta") {
      numchan.push_back( EvtRecDTag::kDstoPiPiPiEta );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(4);
      decaylist=pionList.plus()* pionList.plus()* pionList.minus()* etaList;
    }
    else if(channel=="DstoPiPiPiEtaPiPiPi0") { // <---------- NEW MODE
      numchan.push_back( EvtRecDTag::kDstoPiPiPiEtaPiPiPi0 );
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(9);
      CDDecayList EtaList(etatoPiPiPi0Selector);
      EtaList=pionList.plus()* pionList.minus()* pi0List;
      decaylist=pionList.plus()* pionList.plus()* pionList.minus()* EtaList;
    }
    else if(channel=="DstoPiEPPiPiEta") {
      numchan.push_back( EvtRecDTag::kDstoPiEPPiPiEta );
      numchan.push_back(2);
      numchan.push_back(6);
      CDDecayList epList(eptoPiPiEtaSelector);
      epList=pionList.plus()* pionList.minus()* etaList;
      decaylist=pionList.plus()* epList;
    }
    else if(channel=="DstoPiPi0EPPiPiEta") {
      numchan.push_back( EvtRecDTag::kDstoPiPi0EPPiPiEta );
      numchan.push_back(2);
      numchan.push_back(3);
      numchan.push_back(6);
      CDDecayList epList(eptoPiPiEtaSelector);
      epList=pionList.plus()* pionList.minus()* etaList;
      decaylist=pionList.plus()* pi0List* epList;
    }
    else if(channel=="DstoPiEPPiPiEtaPiPiPi0") {// <------------- New mode: 470 eta'(pipieta, eta->pipipi0)
      numchan.push_back( EvtRecDTag::kDstoPiEPPiPiEtaPiPiPi0 );
      numchan.push_back(2);
      numchan.push_back(8);
      CDDecayList EtaList(etatoPiPiPi0Selector);
      EtaList=pionList.plus()* pionList.minus()* pi0List;
      CDDecayList EtapList(eptoPiPiEta3PiSelector);
      EtapList=pionList.plus()* pionList.minus()* EtaList; 
      decaylist=pionList.plus()* EtapList;
    }
    else if(channel=="DstoPiPi0EPPiPiEtaPiPiPi0") { //<------------ New mode: 471 eta'(pipieta, eta->pipipi0)
      numchan.push_back( EvtRecDTag::kDstoPiPi0EPPiPiEtaPiPiPi0 );
      numchan.push_back(2);
      numchan.push_back(3);
      numchan.push_back(8);
      CDDecayList EtaList(etatoPiPiPi0Selector);
      EtaList=pionList.plus()* pionList.minus()* pi0List;
      CDDecayList EtapList(eptoPiPiEta3PiSelector);
      EtapList=pionList.plus()* pionList.minus()* EtaList;
      decaylist=pionList.plus()* pi0List* EtapList;
    }

    else if(channel=="DstoPiEPRhoGam") {
      numchan.push_back( EvtRecDTag::kDstoPiEPRhoGam );
      numchan.push_back(2);
      numchan.push_back(7);
      CDDecayList rhoList(rhotoPiPiSelector);
      rhoList=pionList.plus()* pionList.minus();
      CDDecayList epList(eptoRhoGamSelector);
      epList=rhoList* photonList;
      decaylist=pionList.plus()* epList;
    }
    else if(channel=="DstoPiPi0EPRhoGam") {
      numchan.push_back( EvtRecDTag::kDstoPiPi0EPRhoGam );
      numchan.push_back(2);
      numchan.push_back(3);
      numchan.push_back(7);
      CDDecayList rhoList(rhotoPiPiSelector);
      rhoList=pionList.plus()* pionList.minus();
      CDDecayList epList(eptoRhoGamSelector);
      epList=rhoList* photonList;
      decaylist=pionList.plus()* pi0List* epList;
    }
    else if(channel=="DstoKsPi") {
      numchan.push_back( EvtRecDTag::kDstoKsPi );
      numchan.push_back(5);
      numchan.push_back(2);
      decaylist=ksList* pionList.plus();
    }
    else if(channel=="DstoKsPiPi0") {
      numchan.push_back( EvtRecDTag::kDstoKsPiPi0 );
      numchan.push_back(5);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=ksList* pionList.plus()* pi0List;
    }
    else if(channel=="DstoKPiPi") {
      numchan.push_back( EvtRecDTag::kDstoKPiPi );
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      decaylist=kaonList.plus()* pionList.plus()* pionList.minus();
    }
    else if(channel=="DstoKPiPiPi0") {
      numchan.push_back( EvtRecDTag::kDstoKPiPiPi0 );
      numchan.push_back(1);
      numchan.push_back(2);
      numchan.push_back(2);
      numchan.push_back(3);
      decaylist=kaonList.plus()* pionList.plus()* pionList.minus()* pi0List;
    }
    else if(channel=="DstoKKK") {
      numchan.push_back( EvtRecDTag::kDstoKKK );
      numchan.push_back(1);
      numchan.push_back(1);
      numchan.push_back(1);
      decaylist=kaonList.minus()* kaonList.plus()* kaonList.plus();
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
	else if (numchan[i+1]==6){
	  const CDCandidate& apion = daughter.decay().child(0);
	  const CDCandidate& spion = daughter.decay().child(1);
	  const CDCandidate& eta = daughter.decay().child(2);
	  const EvtRecTrack* apiontrk = apion.track();
	  const EvtRecTrack* spiontrk = spion.track();
	  const EvtRecTrack* hiEnGamma=eta.navEta()->hiEnGamma();
	  const EvtRecTrack* loEnGamma=eta.navEta()->loEnGamma();

	  trackid.push_back(apiontrk->trackId());
	  trackid.push_back(spiontrk->trackId());
	  showerid.push_back(hiEnGamma->trackId());
	  showerid.push_back(loEnGamma->trackId());

	}
	else if (numchan[i+1]==7){
	  const CDCandidate& rho = daughter.decay().child(0);
	  const CDCandidate& apion = rho.decay().child(0);
	  const CDCandidate& spion = rho.decay().child(1);
	  const CDCandidate& gamma = daughter.decay().child(1);

	  const EvtRecTrack* apiontrk = apion.track();
	  const EvtRecTrack* spiontrk = spion.track();
	  const EvtRecTrack* gammatrk = gamma.photon();


	  trackid.push_back(apiontrk->trackId());
	  trackid.push_back(spiontrk->trackId());
	  showerid.push_back(gammatrk->trackId());

	}
	else if (numchan[i+1]==8){ // <------------ NEW PART FOR ETAP TO PIPIETA TO PIPIPI0
	  const CDCandidate& apion  = daughter.decay().child(0);
	  const CDCandidate& spion  = daughter.decay().child(1);
	  const CDCandidate& eta    = daughter.decay().child(2);
	  const CDCandidate& e0pion = eta.decay().child(0);
	  const CDCandidate& e1pion = eta.decay().child(1);
	  const CDCandidate& pi0    = eta.decay().child(2);

	  const EvtRecTrack* apiontrk  = apion.track();
	  const EvtRecTrack* spiontrk  = spion.track();
	  const EvtRecTrack* e0piontrk = e0pion.track();
	  const EvtRecTrack* e1piontrk = e1pion.track();

	  const EvtRecTrack* hiEnGamma=pi0.navPi0()->hiEnGamma();
	  const EvtRecTrack* loEnGamma=pi0.navPi0()->loEnGamma();

	  trackid.push_back(apiontrk->trackId());
	  trackid.push_back(spiontrk->trackId());
	  trackid.push_back(e0piontrk->trackId());
	  trackid.push_back(e1piontrk->trackId());
	  showerid.push_back(hiEnGamma->trackId());
	  showerid.push_back(loEnGamma->trackId());

	}
	else if (numchan[i+1]==9){ // <------- NEW PART FOR ETA TO PIPIPI0
	  const CDCandidate& apion = daughter.decay().child(0);
	  const CDCandidate& spion = daughter.decay().child(1);
	  const CDCandidate& pi0 = daughter.decay().child(2);
	  const EvtRecTrack* apiontrk = apion.track();
	  const EvtRecTrack* spiontrk = spion.track();
	  const EvtRecTrack* hiEnGamma=pi0.navPi0()->hiEnGamma();
	  const EvtRecTrack* loEnGamma=pi0.navPi0()->loEnGamma();

	  trackid.push_back(apiontrk->trackId());
	  trackid.push_back(spiontrk->trackId());
	  showerid.push_back(hiEnGamma->trackId());
	  showerid.push_back(loEnGamma->trackId());

	}


      }//end of filling track and shower ids


      saveDsInfo(it, ebeam, numofchildren, recDTag);
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


void  DsReconstruction::saveDsInfo(CDDecayList::iterator it, double ebeam, int numofchildren, EvtRecDTag* recDTag){

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

void  DsReconstruction::savetrack(vector<int> trackid, vector<int> showerid, EvtRecTrackIterator charged_begin, EvtRecTrackIterator charged_end,
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

void DsReconstruction::pidtag(vector<int> kaonid, vector<int> pionid, CDChargedKaonList& kaonList, CDChargedPionList& pionList,EvtRecDTag* recDTag){

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


vector<string> DsReconstruction::getlist(string& filename ){

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
