#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/PropertyMgr.h"
#include "EventModel/EventModel.h"
#include "EventModel/Event.h"
#include "EvtRecEvent/EvtRecDTag.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "DTagAlg/DTag.h"
#include <vector>


//**************************************************************************************
DTag::DTag(const std::string& name, ISvcLocator* pSvcLocator) :
  Algorithm(name, pSvcLocator) {
  //Declare the properties
  declareProperty("NeutralDReconstruction",  m_recD0 = true);
  declareProperty("ChargedDReconstruction",  m_recDp = true);
  declareProperty("DsReconstruction",        m_recDs = true);
  // declareProperty("RawDstOnly",              m_rawdstonly = true);
}

// ***********************************************************************************
StatusCode DTag::initialize() {

  MsgStream log(msgSvc(), name());
  
  log << MSG::INFO << "creating DTag sub Algorithm" << endreq;

  StatusCode sc;




  if(m_recD0) { 
    sc = createSubAlgorithm("NeutralDReconstruction", "NeutralDReconstruction", m_NeutralDReconstruction);
    if(sc.isFailure()) {
      log << MSG::ERROR << "Error while creating D0Reconstruction" << endreq;
      return StatusCode::FAILURE;
    }
  }

  if(m_recDp) { 
    sc = createSubAlgorithm("ChargedDReconstruction", "ChargedDReconstruction", m_ChargedDReconstruction);
    if(sc.isFailure()) {
      log << MSG::ERROR << "Error while creating D+Reconstruction" << endreq;
      return StatusCode::FAILURE;
    }
  }

  if(m_recDs) { 
    sc = createSubAlgorithm("DsReconstruction", "DsReconstruction", m_DsReconstruction);
    if(sc.isFailure()) {
      log << MSG::ERROR << "Error while creating DsReconstruction" << endreq;
      return StatusCode::FAILURE;
    }
  }

  log << MSG::INFO << "successfully return from initialize()" <<endmsg;
  return StatusCode::SUCCESS;

}

StatusCode DTag::registerParent(MsgStream& log) {
  DataObject *aEvtRecEvent;
  eventSvc()->findObject("/Event/EvtRec",aEvtRecEvent);
  if (aEvtRecEvent == NULL) {
    aEvtRecEvent = new EvtRecEvent();
    StatusCode sc = eventSvc()->registerObject("/Event/EvtRec",aEvtRecEvent);
    if (sc != StatusCode::SUCCESS) {
      log << MSG::FATAL << "Could not register EvtRecEvent" <<endreq;
      return StatusCode::FAILURE;
    }
  }
  return StatusCode::SUCCESS;
}

StatusCode DTag::clearEvtRecDTagCol(MsgStream& log) {
  StatusCode sc = registerParent(log);
  if (sc != StatusCode::SUCCESS) {
    return sc;
  }
  
  DataObject* aEvtRecDTagCol;
  eventSvc()->findObject("/Event/EvtRec/EvtRecDTagCol",aEvtRecDTagCol);
  if (aEvtRecDTagCol != NULL) {
    //IDataManagerSvc* dataManSvc = dynamic_cast<IDataManagerSvc*>(eventSvc());
    SmartIF<IDataManagerSvc> dataManSvc(eventSvc());
    dataManSvc->clearSubTree("/Event/EvtRec/EvtRecDTagCol");
    eventSvc()->unregisterObject("/Event/EvtRec/EvtRecDTagCol");
  } 
  
  SmartDataPtr<EvtRecDTagCol> evtRecDTagCol(eventSvc(), EventModel::EvtRec::EvtRecDTagCol);
  if( evtRecDTagCol != NULL){
    //IDataManagerSvc* dataManSvc = dynamic_cast<IDataManagerSvc*>(eventSvc());
    SmartIF<IDataManagerSvc> dataManSvc(eventSvc());
    dataManSvc->clearSubTree("/Event/EvtRec/EvtRecDTagCol");
    eventSvc()->unregisterObject("/Event/EvtRec/EvtRecDTagCol");
  } 
  
  return StatusCode::SUCCESS;
}

void DTag::registerEvtRecDTagCol(MsgStream& log) {
  EvtRecDTagCol* aNewEvtRecDTagCol = new EvtRecDTagCol;
  StatusCode sc = eventSvc()->registerObject("/Event/EvtRec/EvtRecDTagCol",
                                             aNewEvtRecDTagCol);
  if (sc != StatusCode::SUCCESS) {
    log << MSG::FATAL << "Could not register EvtRecDTagCol in TDS!" << endreq;
  }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode DTag::execute() {
  

  //check if DTagList exists
  //if true, will exit the program
  SmartDataPtr<EvtRecDTagCol> evtRecDTagCol(eventSvc(), EventModel::EvtRec::EvtRecDTagCol);
  if( evtRecDTagCol != NULL ){
    if ( evtRecDTagCol->size()>0 ) {
      cout << "************************************************"<<endl;
      cout << "EvtRecDTagCol already exists in this file!" << endl;
      cout << " Please run this program on a raw dst file!" << endl;
      cout << " *** This job is now being killed. ***" << endl;
      cout << "************************************************"<<endl;
      return StatusCode::FAILURE;
    }  /// End of "evtRecDTagCol->size()>0" IF
  }  /// End of "evtRecDTagCol != NULL" IF

    
  MsgStream log(msgSvc(), name());
  StatusCode sc = clearEvtRecDTagCol(log);
  if (sc != StatusCode::SUCCESS) {
    return sc;
  }

  DataObject* aEvtRecDTagCol;
  eventSvc()->findObject("/Event/EvtRec/EvtRecDTagCol", aEvtRecDTagCol);
  if (aEvtRecDTagCol == NULL) {
    registerEvtRecDTagCol(log);
  }
  
  std::vector<Algorithm*>::const_iterator it  = subAlgorithms()->begin();
  std::vector<Algorithm*>::const_iterator end = subAlgorithms()->end();
  for(; it != end; it++) {
    sc = (*it)->execute();
    if(sc.isFailure()) {
      log << "Error executing selection "  << (*it)->name() << endreq;
    }
  }


  return StatusCode::SUCCESS;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode DTag::finalize() {

  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in finalize()" << endmsg;
  return StatusCode::SUCCESS;
}

