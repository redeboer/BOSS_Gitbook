//*****************************************************************************
//
// EvtDecay.cxx
//
// EvtDecay algorithm takes generated event from transient store, and decay
// particles, including weak decays of its secondary particles.
// EvtDecay can be used as a TopAlg in conjunction with algorithms Pythia, 
// KKMC or a SingleParticleGun.
//
// History:
// Original LHCb code by Witold Pokorski and Patric Robbe.
// August 2002: Malte Muller, adopted for ATHENA to work inside algorithm PythiaBModule (only private version within 3.0.0)  
// Sept 2003: James Catmore, adopted for 6.0.3 (not reeased in 6.0.3 ony private version) to work inside PythiaBModule.
// Nov 2003: M.Smizanska,  rewritten a) to be standalone EvtDecay algorithm so it can be combined 
// with any type of Pythia generator b) to match to new LHCb(CDF) code dedicated to LHC, c) to work in 7.3.0.
// EvtDecay first time released in 7.3.0   20.Nov. 2003
// Dec 2003: M.Smizanska: define user's input - decay table 
// Feb 2004 J Catmore, addition of random seed facility. TEMPORARY FIX
// Oct 2005 A. Zhemchugov, adapted for BES3  
// May 2006 K.L He, set spin density matrix for e+e- -> V 
// Sept.2007, R.G.Ping, change to the BesEvtGen          
// Jan. 2008, R.G.Ping, to print McTruth table to the file DecayTopology when only doing generation, not simulation
// Feb. 2008, R.G.Ping, add an option to only run the BesEvtGen 
// Mar. 2008, R.G. Ping, user options "DecayDecDir" and "PdtTableDir" are changed. 
// Mar. 2008-03, R.G. Ping, dump the user decay card to the bosslog file
//*****************************************************************************

// Header for this module:-
#include "EvtDecay.h"
#include "ReadME.h"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtDecayTag.hh"
#include "EvtGen.hh"
#include "EvtGenBase/EvtRandomEngine.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenModels/EvtLundCharm.hh"
#include "EvtGenModels/EvtPsi3Sdecay.hh"
#include "EvtGlobalSet.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenModels/EvtCalHelAmp.hh"
#include "EvtGenModels/EvtConExc.hh"

// Framework Related Headers:-
#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "EventModel/EventHeader.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "DataInfoSvc/IDataInfoSvc.h"
#include "DataInfoSvc/DataInfoSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/DataSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IPartPropSvc.h"

#include "BesKernel/IBesRndmGenSvc.h" 
#include "GeneratorObject/McGenEvent.h"
#include "CLHEP/Random/Ranlux64Engine.h"
#include "CLHEP/Random/RandFlat.h" 
#include "EvtGenBase/EvtEulerAngles.hh"

#include <stdlib.h>
#include <string.h>

static string mydecayrec;
static double _amps_max;
int nchr   = 0;
int nchr_e = 0;
int nchr_mu= 0;
int nchr_pi= 0;
int nchr_k = 0;
int nchr_p = 0;
int N_gamma=0;
int N_gammaFSR = 0;
int fst[50];
int EvtDecay::m_runNo=0;
double EvtConExc::SetMthr=0;

EvtDecay::EvtDecay(const string& name, ISvcLocator* pSvcLocator):Algorithm( name, pSvcLocator )
{


 
  // these can be used to specify alternative locations or filenames
  // for the EvtGen particle and channel definition files.

  declareProperty("DecayDecDir", m_DecayDec             = "");
  declareProperty("PdtTableDir", m_PdtTable             = "");
  declareProperty("userDecayTableName", userDecFileName = "NONE");
  declareProperty("DecayTopology", m_DecayTop           = "");    // output decay topology to a file specified by user    
  declareProperty("DecayRecord", m_DecayRec             = "");    // output decay topology of one particle specified by user to a file                   
  declareProperty("ParentParticle", m_ParentPart        = "NONE");// Mothor particle name in pdt.table for only running BesEvtGen
  declareProperty("Multiplicity", m_Ncharge             = false); // output ncharge number of an event
  declareProperty("NutupleFile",m_NtupleFile            = false); // output Ntuple file 
  declareProperty("mDIY",_mDIY= false);                           // mDIY model 
  declareProperty("FDPdata",m_SB3File                   = "");    // Fit Dalitz Plot (FDP) data file name (*.root) 
  declareProperty("FDPHisT",m_SB3HT                     = "");    // histogram title of Dalitz plot in  data file  
  declareProperty("FDPpart",m_FDPparticle               ="");     //to assign the FDP parent particle name (string)
  declareProperty("TruthFile",m_truthFile               =""); 
  declareProperty("TruthPart",m_truthPart               =""); 
  declareProperty("Psi3SopenCharm",m_Psi4040OpenCharm=false);
  declareProperty("Psi2openCharm", m_Psi2openCharm=false);
  declareProperty("SetMthrForConExc",m_SetMthr=0.0);
  declareProperty("statDecays",m_statDecays=false);
  declareProperty("TagLundCharmModel", m_tagLundModel=false);
  m_mystring.clear();
  declareProperty("FileForTrackGen", m_mystring);
  //for polarized charmonium production
  m_polarization.clear();//= diag{1+Pe, 0, 1-Pe} with electron polarization Pe
  declareProperty("polarization", m_polarization);
  declareProperty("eBeamPolarization", m_eBeamPolarization=-1);
  m_cluster0.clear();m_wind0.clear();
  m_cluster1.clear();m_wind1.clear();
  m_cluster2.clear();m_wind2.clear();
  declareProperty("cluster0",m_cluster0);
  declareProperty("cluster1",m_cluster1);
  declareProperty("cluster2",m_cluster2);
  declareProperty("masswin0",m_wind0);
  declareProperty("masswin1",m_wind1);
  declareProperty("masswin2",m_wind2);
  declareProperty("OutputP4",m_outputp4="");
  declareProperty("ReadMeasuredEcms", m_RdMeasuredEcms = false);
  declareProperty("beamEnergySpread", m_beamEnergySpread = 0);
  m_ReadTruth.clear();
  declareProperty("ReadTruth",m_ReadTruth); 
  //ReadTruth={{"ParentName"},{"i0","i1","i2"},{"j0","j1","j2","j3"}}, where the first part. is Parent->getDaug(i0)->getDaug(i1)->getDaug(i2),
  //and second particle is Parent ->getDaug(j0)->getDaug(j1)->getDaug(j2)->getDaug(j3);
  declareProperty("RvalueTag",_RvalueTag=false);
  declareProperty("PrintFSFor",m_printfs="");
}


StatusCode EvtDecay::initialize(){

  MsgStream log(messageService(), name());
  system("cat $BESEVTGENROOT/share/phokhara_9.1.fferr>phokhara_9.1.fferr");
  system("cat $BESEVTGENROOT/share/phokhara_9.1.ffwarn>phokhara_9.1.ffwarn");

  if(m_truthFile!=""){truth.open(m_truthFile.c_str());}
  log << MSG::INFO << "EvtDecay initialize" << endreq;
  static const bool CREATEIFNOTTHERE(true);
  StatusCode RndmStatus = service("BesRndmGenSvc", p_BesRndmGenSvc, CREATEIFNOTTHERE);
  if (!RndmStatus.isSuccess() || 0 == p_BesRndmGenSvc)
  {
    log << MSG::ERROR << " Could not initialize Random Number Service" << endreq;
    return RndmStatus;
  }

  EvtGlobalSet::SV.clear();
  for(int i=0;i<m_mystring.size();i++){EvtGlobalSet::SV.push_back(m_mystring[i]);}

  EvtGlobalSet::iVV.clear();
  EvtGlobalSet::dVV.clear();
  std::vector<std::vector<int> >myivv;
  std::vector<std::vector<double> >mydvv;

  EvtConExc::SetMthr=m_SetMthr;

  myivv.push_back(m_cluster0);
  myivv.push_back(m_cluster1);
  myivv.push_back(m_cluster2);

  mydvv.push_back(m_wind0);
  mydvv.push_back(m_wind1);
  mydvv.push_back(m_wind2);

  for(int i=0;i<myivv.size();i++){
    std::vector<int> theivv;
    for(int j=0;j<myivv[i].size();j++){
      theivv.push_back(myivv[i][j]);
    }
    if(theivv.size()) EvtGlobalSet::iVV.push_back(theivv);
  }

  for(int i=0;i<mydvv.size();i++){
    std::vector<double> thedvv;
    for(int j=0;j<mydvv[i].size();j++){
      thedvv.push_back(mydvv[i][j]);
    }
    if(thedvv.size()) EvtGlobalSet::dVV.push_back(thedvv);
  }


  if(m_eBeamPolarization>1) {std::cout<<"The e-beam polaziation should be in 0 to 1"<<std::endl;abort();}
  m_numberEvent=0; 
  first = true; 
  m_ampscalflag = true;
  // Save the random number seeds in the event
  CLHEP::HepRandomEngine* engine  = p_BesRndmGenSvc->GetEngine("EVTGEN");
  const long    s  = engine->getSeed();
  p_BesRndmGenSvc->setGenseed(s+1);

  m_seeds.clear();
  m_seeds.push_back(s);
  totalChannels = 0;

 // open an interface to EvtGen

  if ( m_DecayDec == "") {  //use default DECAY.DEC and pdt.table if not defined by user
   m_DecayDec=getenv("BESEVTGENROOT");
   m_DecayDec +="/share/DECAY.DEC";
   }

  if ( m_PdtTable == "") {
   m_PdtTable =getenv("BESEVTGENROOT");
   m_PdtTable +="/share/pdt.table";
   }  
  
  char catcmd[300];  //output user decay cards to log file
  std::cout<<"===================== user decay card ========================"<<std::endl;
  strcpy(catcmd, "cat ");
  strcat(catcmd, userDecFileName.c_str());
  system(catcmd);
  
   IDataInfoSvc *tmpInfoSvc;
   DataInfoSvc* dataInfoSvc;
  
  // write decay cards to the root file: pingrg@2009-09-09
  StatusCode status;
  status = service("DataInfoSvc",tmpInfoSvc);
  if (status.isSuccess()) {
    log << MSG::INFO << "got the DataInfoSvc" << endreq;
    dataInfoSvc=dynamic_cast<DataInfoSvc *>(tmpInfoSvc);
    dataInfoSvc->setDecayCard(userDecFileName);
  } else { 
    log << MSG::ERROR << "could not get the DataInfoSvc" << endreq;
    return StatusCode::FAILURE;
  }

  

  m_RandomEngine  = new EvtBesRandom(engine);
  m_Gen = new EvtGen( m_DecayDec.c_str(), m_PdtTable.c_str(), m_RandomEngine );
      
  if(userDecFileName =="NONE")  log << MSG::INFO << "EvtDecay User did not define his Decay table EvtGen will use standart" << endreq;   
  if(!(userDecFileName =="NONE")) m_Gen->readUDecay(userDecFileName.c_str());

 if(!(m_DecayTop==" ")) {
   outfile.open(m_DecayTop.c_str()); 
  }

 //for output Ntuple file, pingrg-2009-09-07


  if(m_NtupleFile) {
       NTuplePtr nt1(ntupleSvc(),"MYROOTFILE/Trk");
       if(nt1) {m_tuple=nt1;}
       else {
          m_tuple = ntupleSvc()->book ("MYROOTFILE/Trk", CLID_ColumnWiseTuple, "Generator-trk-Ntuple");
          if(m_tuple){
          status = m_tuple->addItem ("TotNumTrk", TotNumTrk, 0,100);
          status = m_tuple->addIndexedItem ("Trk_index", TotNumTrk, m_Trk_index);    
          status = m_tuple->addIndexedItem ("m_px_trk",  TotNumTrk, m_px_trk); 
          status = m_tuple->addIndexedItem ("m_py_trk",  TotNumTrk, m_py_trk); 
          status = m_tuple->addIndexedItem ("m_pz_trk",  TotNumTrk, m_pz_trk); 
          status = m_tuple->addIndexedItem ("m_en_trk",  TotNumTrk, m_en_trk); 
          status = m_tuple->addIndexedItem ("FST",       TotNumTrk, m_fst); 

          status = m_tuple->addItem ("nchr",     m_nchr); 
          status = m_tuple->addItem ("nchr_e",   m_nchr_e); 
          status = m_tuple->addItem ("nchr_mu",  m_nchr_mu); 
          status = m_tuple->addItem ("nchr_pi",  m_nchr_pi); 
          status = m_tuple->addItem ("nchr_k",   m_nchr_k); 
          status = m_tuple->addItem ("nchr_p",   m_nchr_p); 
          status = m_tuple->addItem ("N_gamma",  m_gamma);
          status = m_tuple->addItem ("N_gammaFSR",  m_gammaFSR); 
          status = m_tuple->addItem ("Flag1",  m_flag1); 
	  } else {   
 	           log << MSG::ERROR << "    Cannot book N-tuple:"<< long(m_tuple) << endmsg;  
 	           return StatusCode::FAILURE;
	  }    
       }

       NTuplePtr nt2(ntupleSvc(),"MYROOTFILE/mass");
       if(nt2) {mass_tuple=nt2;}
       else {
          mass_tuple = ntupleSvc()->book ("MYROOTFILE/mass", CLID_ColumnWiseTuple, "Generator-mass-Ntuple");
          if(mass_tuple){
          status = mass_tuple->addItem ("m12", m_m12);
          status = mass_tuple->addItem ("m13", m_m13);
          status = mass_tuple->addItem ("m23", m_m23);
          status = mass_tuple->addItem ("m1", m_m1);
          status = mass_tuple->addItem ("m2", m_m2);
          status = mass_tuple->addItem ("m3", m_m3);
          status = mass_tuple->addItem ("costheta1", m_cos1);
          status = mass_tuple->addItem ("costheta2", m_cos2);
          status = mass_tuple->addItem ("costheta3", m_cos3);
          status = mass_tuple->addItem ("ichannel", m_ich);
	  } else {   
 	           log << MSG::ERROR << "    Cannot book N-tuple:"<< long(m_tuple) << endmsg;  
 	           return StatusCode::FAILURE;
	  }    
       }

       NTuplePtr nt3(ntupleSvc(),"MYROOTFILE/massGen");
       if(nt3) {massgen_tuple=nt3;}
       else {
          massgen_tuple = ntupleSvc()->book ("MYROOTFILE/massGen", CLID_ColumnWiseTuple, "Generator-mass-Ntuple");
          if(massgen_tuple){
          status = massgen_tuple->addItem ("m12", _m12);
          status = massgen_tuple->addItem ("m13", _m13);
          status = massgen_tuple->addItem ("m23", _m23);
          status = massgen_tuple->addItem ("m1", _m1);
          status = massgen_tuple->addItem ("m2", _m2);
          status = massgen_tuple->addItem ("m3", _m3);
          status = massgen_tuple->addItem ("costheta1", _cos1);
          status = massgen_tuple->addItem ("costheta2", _cos2);
          status = massgen_tuple->addItem ("costheta3", _cos3);
          status = massgen_tuple->addItem ("ichannel", _ich);
	  } else {   
 	           log << MSG::ERROR << "    Cannot book N-tuple:"<< long(m_tuple) << endmsg;  
 	           return StatusCode::FAILURE;
	  }    
       }


  }
  for(int i=0;i<500;i++){br[i]=0;}
  if(!(m_SB3File=="" && m_SB3HT=="")) {
    const char *filename, *histitle;
    filename=(char*)m_SB3File.c_str();
    histitle=(char*)m_SB3HT.c_str();
    SuperBody3decay.setFile(filename);
    SuperBody3decay.setHTitle(histitle);
    SuperBody3decay.init();
  }

  return StatusCode::SUCCESS;
}

StatusCode EvtDecay::execute() 
{ 

  MsgStream log(messageService(), name());
  // log << MSG::INFO << "EvtDecay executing" << endreq;
  //------------------
  string   key = "GEN_EVENT";
  // retrieve event from Transient Store (Storegate)
  SmartDataPtr<McGenEventCol> McEvtColl(eventSvc(), "/Event/Gen");	

  m_numberEvent += 1;  
  writeFlag = 0;
  //std::cout<<"EvtNumber= "<<m_numberEvent<<std::endl;
  if ( McEvtColl == 0 ) 
  {
    HepMC::GenEvent* evt=new GenEvent();  
    evt->set_event_number(m_numberEvent);

    //read Ecms from the database
    SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
    int runNo=eventHeader->runNumber();
    int event=eventHeader->eventNumber();
    bool newRunFlag=0;
    if(runNo != 0 && runNo != m_runNo){m_runNo=runNo;newRunFlag = true;}else{newRunFlag=false;}
    if(m_RdMeasuredEcms&& newRunFlag){// using cms energy of beam read from database
      runNo = std::abs(runNo);
      ReadME theME(runNo);
      if(theME.isRunNoValid()){
	dbEcms=theME.getEcms();
	std::cout<<"Read Ecms= "<<dbEcms<<std::endl;
	if(dbEcms!=0){
	  std::cout<<"INFO: Read the MeasuredEcms"<<std::endl;
	}
	else{
	  std::cout<<"ERROR: Can not read the MeasuredEcms, try to turn off the ReadEcmsDB"<<std::endl;
	  return StatusCode::FAILURE;
	}
      }
    }
    //end of read Ecms 

    callBesEvtGen( evt );
    if(!(m_DecayTop==""))  evt->print(outfile);  

    //create a Transient store 
    McGenEventCol *mcColl = new McGenEventCol;
    McGenEvent* mcEvent = new McGenEvent(evt);
    mcColl->push_back(mcEvent);
    StatusCode sc = eventSvc()->registerObject("/Event/Gen",mcColl);
    if(sc != SUCCESS)  return StatusCode::FAILURE;    

  } else  //  (McEvtColl != 0) 
 {
  McGenEventCol::iterator mcItr;
  for( mcItr = McEvtColl->begin(); mcItr != McEvtColl->end(); mcItr++ )   
  {
     HepMC::GenEvent* hEvt = (*mcItr)->getGenEvt();
    // MeVToGeV( hEvt );

     callEvtGen( hEvt );

     if(!(m_DecayTop==""))  hEvt->print(outfile); 
    // GeVToMeV( hEvt );
     //   if(!(m_DecayRec=="")) outfile2<<std::endl;
  if(!(m_DecayRec=="")) std::cout<<std::endl;
  };
 }
  if( m_NtupleFile ){ m_tuple->write();}
  return StatusCode::SUCCESS; 
}

// main procedure, loops over all particles in given event,
// converts them to EvtGenParticles,
// feeds them to EvtGen,
// converts back to HepMC particles and puts them in the event.

StatusCode EvtDecay::callEvtGen( HepMC::GenEvent* hepMCevt )
{
  MsgStream log(messageService(), name());
   nchr   = 0;
   nchr_e = 0;
   nchr_mu= 0;
   nchr_pi= 0;
   nchr_k = 0;
   nchr_p = 0;
   N_gamma=0;
   N_gammaFSR = 0;
   SmartDataPtr<Event::EventHeader>  eventHeader(eventSvc(),"/Event/EventHeader");
  
  for (int i=0;i<13;i++) fst[i]=0;
  HepMC::GenEvent::particle_const_iterator itp;
  AllTrk_index=0;
  for( itp=hepMCevt->particles_begin(); itp!=hepMCevt->particles_end(); ++itp )   
  {
    // This is the main loop.
    // We iterate over particles and we look for ones that
    // should be decayed with EvtGen.
    //
    // status == 1     - undecayed Pythia particle (also for particles that are not supposed to decay)
    // status == 999   - particle already decayed by EvtGen
    // status == 899   - particle was supposed to decay by EvtGen - but found no model
    //                   this may be also intentional - such events are then excluded from
    //                   being written into persistent output.
    // ISTHEP(IHEP):
    // status code for entry IHEP, with the following meanings:
    // 	
    //  = 0 : null entry. 
    //  = 1 : an existing entry, which has not decayed or fragmented. This is the main class of entries, which represents the `final state' given by the generator. 
    //  = 2 : an entry which has decayed or fragmented and is therefore not appearing in the final state, but is retained for event history information. 
    //  = 3 : a documentation line, defined separately from the event history. This could include the two incoming reacting particles, etc. 
    //  = 4 - 10 : undefined, but reserved for future standards. 
    //  = 11 - 200 : at the disposal of each model builder for constructs specific to his program, but equivalent to a null line in the context of any other program. 
    //  = 201 - : at the disposal of users, in particular for event tracking in the detector. 

    HepMC::GenParticle* hepMCpart=*itp;
    int stat = hepMCpart->status();


    if ( stat != 1 ) continue;

  loop_to_select_eventsB:
    int id = hepMCpart->pdg_id();
    parentPDGcode=id;
    hepMCpart->set_status(899);
    EvtId eid=EvtPDL::evtIdFromStdHep(id);
    string parentName=EvtPDL::name(eid);
    
    double en =(hepMCpart->momentum()).e();
    double pex=(hepMCpart->momentum()).px();
    double pey=(hepMCpart->momentum()).py();
    double pez=(hepMCpart->momentum()).pz();
	
    EvtVector4R p_init(en,pex,pey,pez);
    parentMass=p_init.mass();
    EvtPDL::reSetMass(eid,parentMass);

    EvtParticle* part=EvtParticleFactory::particleFactory(eid,p_init);

    // set spin density matrix for e+ e- -> V
    bool parentFlag=false;
    if(  writeFlag==0 && part->getSpinType() == EvtSpinType::VECTOR) {
      if(m_polarization.size()>0) {
	part->setPolarizedSpinDensity(m_polarization[0],m_polarization[1],m_polarization[2]);
      }else if(m_eBeamPolarization>0){
	part->setPolarizedSpinDensity(1+ m_eBeamPolarization,0,1- m_eBeamPolarization);
      } else{
	part->setVectorSpinDensity();
      }
      parentFlag=true;  
      writeFlag++;
    } else {
      int id = hepMCpart->pdg_id();
      if( id == 22 ) {N_gamma ++;fst[0]++;}  //fst[0]:photon
      if( id == -22 ){N_gammaFSR ++;}
      if( id == 11 ) {fst[1]++;} else if(id == -11){fst[2]++;} // fst[1]: electron  ; fst[]: positron
      else if(id ==  13){fst[3]++;} // fst[3]: mu-
      else if(id == -13){fst[4]++;} // fst[4]: mu+
      else if(id == 211){fst[5]++;} // fst[5]: pi+
      else if(id ==-211){fst[6]++;} // fst[6]: pi-
      else if(id == 321){fst[7]++;} // fst[7]: K+
      else if(id ==-321){fst[8]++;} // fst[8]: K-
      else if(id ==2212){fst[9]++;} // fst[9]: pronton
      else if(id ==-2212){fst[10]++;} // fst[10]: anti-proton
      else if(id == 2112){fst[11]++;} // fst[11]: nucleon
      else if(id ==-2112){fst[12]++;} // fst[12]: ant-nucleon
      if( fabs(id) == 11)   {nchr_e++;}
      if( fabs(id) == 13)   {nchr_mu++;}
      if( fabs(id) == 211)  {nchr_pi++;}
      if( fabs(id) == 321)  {nchr_k++;}
      if( fabs(id) == 2212) {nchr_p++;}
      
      //std::cout<<"id= "<<id<<" AllTrk_index= "<<AllTrk_index<<std::endl;
      if( m_NtupleFile==true ){
	m_Trk_index[AllTrk_index]=id;
	m_px_trk[AllTrk_index]=pex;
	m_py_trk[AllTrk_index]=pey;
	m_pz_trk[AllTrk_index]=pez;
	m_en_trk[AllTrk_index]=en ;
	
	AllTrk_index++;
	Trk_index[AllTrk_index]=0;
      }
      
      
    } 
    
 // for SuperBody3decay generator
 //    EvtVector4R pd1,pd2,pd3;
    EvtVector4R fdp_init;
    EvtId fdp_ppid;
    if(m_FDPparticle!=""){
      findPart(part);
      fdp_ppid = FDP_id;
      fdp_init = FDP_init;
    } else{
      fdp_ppid = eid;
      fdp_init = p_init;
    }

    if(!(m_SB3File=="" || m_SB3HT=="") && parentFlag &&  first ){
      SuperBody3decay_make(fdp_ppid, fdp_init );
     }

  loop_to_select_eventsA:
    m_Gen->generateDecay(part);
    if(m_truthFile!="" && m_truthPart!=""){
      if(EvtPDL::name(part->getId())==m_truthPart ){
	int ndaug = part->getNDaug();
	for(int id=0;id<ndaug;id++){
	  EvtParticle* sonp=part->getDaug(id);
	  EvtVector4R  son=sonp->getP4();
	  truth<<son.get(0)<<" "<<son.get(1)<<" "<<son.get(2)<<" "<<son.get(3)<<std::endl;
	}
      }
    }
    double ratio,rdm;
    if(_mDIY && parentFlag &&  m_ampscalflag ) _amps_max=CalAmpsMax(part);
    if(_mDIY && parentFlag) {
    rdm=EvtRandom::Flat(0.0, 1.0);
    double amps=CalAmpsMDIY( part );
    ratio=amps/_amps_max;
    }
    if(_mDIY && parentFlag && ratio<=rdm){ goto   loop_to_select_eventsA;}
    
    //-------- For superbody3------------------------------
    bool accept; 
   if(m_FDPparticle==""){FDP_part=part;}
   if(!(m_SB3File=="" || m_SB3HT=="") && parentFlag            ){ accept=SuperBody3decay_judge(  FDP_part); }
   if(!(m_SB3File=="" || m_SB3HT=="") && parentFlag  && !accept){ 
     part->deleteTree();
     writeFlag=0;
     goto loop_to_select_eventsB; 
   }else if(!(m_SB3File=="" || m_SB3HT=="") && parentFlag  && accept){
     countChannel(FDP_part);
   }
   //-------- for psi4040 open charm inclusive decay
    if((m_Psi2openCharm || m_Psi4040OpenCharm) && parentFlag ){
      if(getModel(part) == "OPENCHARM"){
	std::cout<<"OPENCHARM model need to set Psi2openCharm and Psi3SopenCharm to be false!"<<std::endl;
	abort();
      }
     EvtPsi3Sdecay opencharm(en, part);
     bool theDecay = opencharm.choseDecay();
     if(!theDecay ) {
     part->deleteTree();
     writeFlag=0;
     goto loop_to_select_eventsB;
     }
   }
    //------------- dump the decay tree to the event model
      // if(Nchannel>=0) part->dumpTree();
      // part->printTree();
   
      if(parentFlag){
       EvtDecayTag theTag(part);
       unsigned int modeTag, multiplicityTag;
       modeTag = theTag.getModeTag();
       if( getModel(part) == "OPENCHARM"||getModel(part) == "LUNDCHARM" && m_tagLundModel){ 
	 multiplicityTag = decayType(part);
	 //debugging
	 // std::cout<<"Opencharm Mode: "<<decayType(part)<<std::endl;
         } else {
	 multiplicityTag = theTag.getMultTag() + decayType(part);
       }
      
       //std::cout<<"StdHep= "<<EvtPDL::getStdHep(part->getId())<<" getChannel "<<part->getChannel()<<" flag1,flag2= "<<modeTag<<" "<<multiplicityTag<<std::endl;
       if(eventHeader == 0) std::cout << "event header unavailable: " << std::endl;
       	if (eventHeader != 0) {
	    eventHeader->setFlag1(modeTag);
	    eventHeader->setFlag2(multiplicityTag);
	    //std::cout<<"flag1,flag2= "<<modeTag<<" "<<multiplicityTag<<std::endl;
	} 
      }
   
     if(!(m_DecayRec=="")) mydecayrec=part->writeTreeRec(m_DecayRec);
    //-- decay statistics
     if(m_statDecays && parentFlag ) countChannel(part);
    // ----------- pingrg 2008-03-25 ---------

    if ( log.level() <= MSG::DEBUG ) part->printTree() ;
    log << MSG::DEBUG << "Converting particles to HepMC" << endreq;

    makeHepMC(part, hepMCevt, hepMCpart);
    if(part->getNDaug()!=0)  
	hepMCpart->set_status(999);  

    //debug
    
    if(part->getId()==EvtPDL::getId(m_outputp4)){
      int nds=part->getNDaug(); 
      for(int i=0;i<nds;i++){
	if(EvtPDL::name(part->getDaug(i)->getId())=="gammaFSR") continue;
	EvtVector4R vp1=part->getDaug(i)->getP4Lab();
	std::cout<<"vvpp "<<vp1.get(0)<<" "<<vp1.get(1)<<" "<<vp1.get(2)<<" "<<vp1.get(3)<<std::endl;
      }
    }

    if(m_ReadTruth.size())ReadTruth(part,m_ReadTruth);      
    //if(part->getId()==EvtPDL::getId("psi(4260)")) std::cout<<"mydebug "<<part->getP4().mass()<<std::endl;
    part->deleteTree();
  };

  for( itp=hepMCevt->particles_begin(); itp!=hepMCevt->particles_end(); ++itp )   
  {
      if (!(*itp)->end_vertex() && ( (*itp)->status()==899 || (*itp)->status()==999 ))
        (*itp)->set_status(1);
  } 
  //nchr = nchr_e + nchr_mu + nchr_pi + nchr_k +nchr_p;
  nchr = nchr_pi + nchr_k +nchr_p;
  if(m_Ncharge == true ) {std::cout<<"Multiplicity: TotoalCharge_e_mu_pi_K_p_gamma_gammaFSR: "
                                   <<nchr     <<" "
                                   <<nchr_e   <<" "
                                   << nchr_mu <<" "
                                   << nchr_pi <<" "
				   << nchr_k  <<" "
				   <<nchr_p   <<" "
				   <<N_gamma  <<" "
				   <<N_gammaFSR<<endl;} 
  if(m_Ncharge == true  ){std::cout<<"FinalStates: "
				  <<fst[0]<<" "
				  <<fst[1]<<" "
				  <<fst[2]<<" "
				  <<fst[3]<<" "
				  <<fst[4]<<" "
				  <<fst[5]<<" "
				  <<fst[6]<<" "
				  <<fst[7]<<" "
				  <<fst[8]<<" "
				  <<fst[9]<<" "
				  <<fst[10]<<" "
				  <<fst[11]<<" "
				  <<fst[12]<<std::endl;}   
   if( m_NtupleFile ){ 
     //    TotNumTrk=500;  
    m_nchr   = nchr;
    m_nchr_e = nchr_e;
    m_nchr_mu = nchr_mu;
    m_nchr_pi = nchr_pi;
    m_nchr_k = nchr_k;
    m_nchr_p = nchr_p;
    m_gamma=N_gamma;
    m_gammaFSR=N_gammaFSR;
    TotNumTrk = AllTrk_index;//  nchr + N_gamma + N_gammaFSR;  
   }
   //   std::cout<<"Flag= "<<eventHeader->flag1()<<"  "<<eventHeader->flag2()<<std::endl;

  return StatusCode::SUCCESS;
}

//****** CallBesEvtGen ************
// This part is responsible for only ruuing BesEvtGen.  //pingrg Feb. 3,2008
//***************************************************
StatusCode EvtDecay::callBesEvtGen( HepMC::GenEvent* hepMCevt )
{
  MsgStream log(messageService(), name());

   nchr   = -1;
   nchr_e = 0;
   nchr_mu= 0;
   nchr_pi= 0;
   nchr_k = 0;
   nchr_p = 0;
   N_gamma=0;
   N_gammaFSR = 0;
   SmartDataPtr<Event::EventHeader>  eventHeader(eventSvc(),"/Event/EventHeader");  
  for (int i=0;i<13;i++) fst[i]=0;
  HepMC::GenEvent::particle_const_iterator itp;
    // This is the main loop.
    // We iterate over particles and we look for ones that
    // should be decayed with EvtGen.
    //
    // status == 1     - undecayed Pythia particle (also for particles that are not supposed to decay)
    // status == 999   - particle already decayed by EvtGen
    // status == 899   - particle was supposed to decay by EvtGen - but found no model
    //                   this may be also intentional - such events are then excluded from
    //                   being written into persistent output.
    // ISTHEP(IHEP):
    // status code for entry IHEP, with the following meanings:
    // 	
    //  = 0 : null entry. 
    //  = 1 : an existing entry, which has not decayed or fragmented. This is the main class of entries, which represents the `final state' given by the generator. 
    //  = 2 : an entry which has decayed or fragmented and is therefore not appearing in the final state, but is retained for event history information. 
    //  = 3 : a documentation line, defined separately from the event history. This could include the two incoming reacting particles, etc. 
    //  = 4 - 10 : undefined, but reserved for future standards. 
    //  = 11 - 200 : at the disposal of each model builder for constructs specific to his program, but equivalent to a null line in the context of any other program. 
    //  = 201 - : at the disposal of users, in particular for event tracking in the detector. 

 loop_to_select_eventsB:  
    EvtId ppid=EvtPDL::getId(m_ParentPart); //parent particle name
    double ppmass=0;
    if(m_RdMeasuredEcms && m_beamEnergySpread==0) { //read Ecms, no energy spread
      EvtPDL::reSetMass(ppid,dbEcms);
      ppmass=EvtPDL::getMass(ppid);
    }else if(m_RdMeasuredEcms && m_beamEnergySpread!=0){ //read Ecms, with nergy spread
      double cmssig=sqrt(2.)*m_beamEnergySpread;
      double myppmass = energySpread(dbEcms, cmssig);
      EvtPDL::reSetMass(ppid,myppmass);
      ppmass=EvtPDL::getMass(ppid);
    }else if( (!m_RdMeasuredEcms) && m_beamEnergySpread!=0){//not read Ecms, with energy spread
      if(m_numberEvent==1 )dbEcms = EvtPDL::getMass(ppid);
      double cmssig=sqrt(2.)*m_beamEnergySpread;
      double myppmass = energySpread(dbEcms, cmssig);
      EvtPDL::reSetMass(ppid,myppmass);   
      ppmass=EvtPDL::getMass(ppid);  
    }else if((!m_RdMeasuredEcms) && m_beamEnergySpread==0 ){//not read Ecms, no energy spread
      ppmass=EvtPDL::getMass(ppid);
    }else {std::cout<<"EvtDecay::callBesEvtGen:: bad option"<<std::endl; abort();}

    // std::cout<<"testMass  "<<ppmass<<std::endl;
    //using Ecms from data base, for XYZ data sets
    parentMass=ppmass;
    int pid=EvtPDL::getStdHep(ppid);
    parentPDGcode=pid;

    EvtVector4R p_init(ppmass,0.0,0.0,0.0);
 
    EvtParticle* part=EvtParticleFactory::particleFactory(ppid,p_init);
    HepMC::GenParticle* hepMCpart=new GenParticle(HepLorentzVector(0,0,0,ppmass),pid,3);

    bool parentFlag = false;
    // set spin density matrix for e+ e- -> V
    if(writeFlag ==0 && part->getSpinType() == EvtSpinType::VECTOR) {
      if(m_polarization.size()>0) { 
	part->setPolarizedSpinDensity(m_polarization[0],m_polarization[1],m_polarization[2]);
      } else if(m_eBeamPolarization>0){
	part->setPolarizedSpinDensity(1+ m_eBeamPolarization,0,1- m_eBeamPolarization);
      } else{
	part->setVectorSpinDensity();
      }
      parentFlag = true;
      writeFlag++;
    }
 
 // for SuperBody3decay generator
    EvtVector4R fdp_init;
    EvtId fdp_ppid;
    if(m_FDPparticle!=""){
      findPart(part);
      fdp_ppid = FDP_id;
      fdp_init = FDP_init;
    } else{
      fdp_ppid = ppid;
      fdp_init = p_init;
    }

    if(!(m_SB3File=="" || m_SB3HT=="") &&  first ){ SuperBody3decay_make( fdp_ppid, fdp_init);}
 // -----------------------------------
  
  loop_to_select_events:
    m_Gen->generateDecay(part); if(m_numberEvent<=1){ std::cout<<"after m_Gen->decay "<<std::endl; part->printTree();}

    double ratio,rdm;
    if(_mDIY &&  m_ampscalflag ) _amps_max=CalAmpsMax(part);

    if(_mDIY ) {
      for(int myloop=0;myloop<100;myloop++){
	m_Gen->generateDecay(part);
	double amps=CalAmpsMDIY( part);
	ratio=amps/_amps_max;
	rdm=EvtRandom::Flat(0.0, 1.0);
	if( !isNumber(amps) || !isNumber(_amps_max) || ratio<=0 ) {
	  part->deleteTree();
	  part=EvtParticleFactory::particleFactory(ppid,p_init);
	  continue;
	}else if( rdm<=ratio) {
	  break;
	}
      }
    }

    if(m_truthFile!="" && m_truthPart!=""){
      if(EvtPDL::name(part->getId())==m_truthPart ){
	int ndaug = part->getNDaug();
	for(int id=0;id<ndaug;id++){
	  EvtParticle* sonp=part->getDaug(id);
	  EvtVector4R son=sonp->getP4();
	  truth<<son.get(0)<<" "<<son.get(1)<<" "<<son.get(2)<<" "<<son.get(3)<<std::endl;
	}
      }
    }
//--------- For superbody3
    bool accept; 
    if(m_FDPparticle==""){FDP_part=part;}
   if(!(m_SB3File=="" || m_SB3HT=="")){
     accept=SuperBody3decay_judge( FDP_part);
      }
   if(!(m_SB3File=="" || m_SB3HT=="") && !accept) {
       delete hepMCpart;
       part->deleteTree();
     goto loop_to_select_eventsB;
   }else  if(!(m_SB3File=="" || m_SB3HT=="") && parentFlag  && accept){
     countChannel(FDP_part);
   }

      int Nchannel=part->getChannel();

     if(m_statDecays && parentFlag) countChannel(part);
 //------------- dump the decay tree to the event model
    //     int Nchannel=part->getChannel();
    //     if(Nchannel>=0) part->dumpTree();
     
     if(parentFlag){
       EvtDecayTag theTag(part);
       unsigned int modeTag, multiplicityTag;
       modeTag = theTag.getModeTag();
       if( getModel(part) == "OPENCHARM"|| getModel(part) == "LUNDCHARM" && m_tagLundModel ){ 
	 multiplicityTag = decayType(part);
	 //debugging
	 // std::cout<<"Opencharm Mode: "<<decayType(part)<<std::endl;
       }else if(_RvalueTag){
	 multiplicityTag = decayType(part);
       } else {
	 multiplicityTag = theTag.getMultTag() + decayType(part);
       }
       if(eventHeader == 0) std::cout << "event header unavailable: " << std::endl;
       if (eventHeader != 0) {
	 eventHeader->setFlag1(modeTag);
	 eventHeader->setFlag2(multiplicityTag);
       } 
       //debugg
       // std::cout<<modeTag<<" "<<multiplicityTag<<std::endl;
       if(m_NtupleFile)m_flag1 = modeTag;
       //std::cout<<"Flag1 "<<modeTag<<std::endl;
     }
     
     if(!(m_DecayRec=="")) {mydecayrec=part->writeTreeRec(m_DecayRec);std::cout<<std::endl;};
      // ----------- pingrg 2008-03-25 ---------
      
      if ( log.level() <= MSG::DEBUG ) part->printTree() ;
      log << MSG::DEBUG << "Converting particles to HepMC" << endreq;
      
      makeHepMC(part, hepMCevt, hepMCpart);
      
      if(part->getNDaug()!=0) hepMCpart->set_status(999); 
      
    //-------------
   if(EvtPDL::getId(m_printfs)==part->getId()) {
      int myd=part->getNDaug();
      std::cout<<"_FinalState "<<myd;
      for(int ii=0;ii<myd;ii++) {
       std::cout<<" "<<EvtPDL::name(part->getDaug(ii)->getId());
       }
      std::cout<<std::endl;
    }

    AllTrk_index=0;
  for( itp=hepMCevt->particles_begin(); itp!=hepMCevt->particles_end(); ++itp )   
  {
      if (!(*itp)->end_vertex() && ( (*itp)->status()==899 || (*itp)->status()==999 ))
        (*itp)->set_status(1);
 
      HepMC::GenParticle* hepMCpart=*itp;
      int stat = hepMCpart->status();
      int id = hepMCpart->pdg_id();
      if(abs(id)==411 ||abs(id)==413 ) { (*itp)->set_status(999);stat=999;}

       if ( stat != 1 ) continue;
       if( id == 22 ) {N_gamma ++;fst[0]++;}
       if( id == -22 ){N_gammaFSR ++;}
            if(id == 11 ) {fst[1]++;}
       else if(id == -11) {fst[2]++;} 
       else if(id == 13 ) {fst[3]++;} 
       else if(id ==-13)  {fst[4]++;}
       else if(id==211)   {fst[5]++;}
       else if(id==-211)  {fst[6]++;}
       else if(id== 321)  {fst[7]++;} 
       else if(id ==-321) {fst[8]++;} 
       else if(id ==2212) {fst[9]++;}
       else if(id==-2212) {fst[10]++;} 
       else if(id == 2112){fst[11]++;} 
       else if(id==-2112) {fst[12]++;}
       if( fabs(id) == 11)   {nchr_e++;}
       if( fabs(id) == 13)   {nchr_mu++;}
       if( fabs(id) == 211)  {nchr_pi++;}
       if( fabs(id) == 321)  {nchr_k++;}
       if( fabs(id) == 2212) {nchr_p++;}

       //for Nuple
        double en =(hepMCpart->momentum()).e();
        double pex=(hepMCpart->momentum()).px();
        double pey=(hepMCpart->momentum()).py();
        double pez=(hepMCpart->momentum()).pz();
	
        if( m_NtupleFile==true && id!=0){
 	  m_Trk_index[AllTrk_index]=id;
	  m_px_trk[AllTrk_index]=pex;
	  m_py_trk[AllTrk_index]=pey;
	  m_pz_trk[AllTrk_index]=pez;
	  m_en_trk[AllTrk_index]=en ; /*
	  std::cout<<"trk# "   <<AllTrk_index
                   <<" PID:"   <<id
		   <<" px: "   <<pex
		   <<" py: "   <<pey
		   <<" pz: "   <<pez
		   <<" E:  "   <<en<<std::endl; */ 
         AllTrk_index++;
	}
     
  } 

  itp=hepMCevt->particles_begin(); // parent decaying particle status set
    (*itp)->set_status(2);

    //nchr = nchr_e + nchr_mu + nchr_pi + nchr_k +nchr_p;
    nchr =  nchr_pi + nchr_k +nchr_p;
  if(m_Ncharge == true ) {std::cout<<"Multiplicity: TotoalCharge_e_mu_pi_K_p_gamma_gammaFSR: "
                                   <<nchr<<" "
                                   <<nchr_e<<" "
                                   << nchr_mu <<" "
                                   << nchr_pi <<" "
                                   << nchr_k <<" "
                                   <<nchr_p<<" "
                                   <<N_gamma<<" "
                                   <<N_gammaFSR<<endl;}   
  if(m_Ncharge == true  ){std::cout<<"FinalStates: "
                                  <<fst[0]<<" "
                                  <<fst[1]<<" "
                                  <<fst[2]<<" "
                                  <<fst[3]<<" "
                                  <<fst[4]<<" "
                                  <<fst[5]<<" "
                                  <<fst[6]<<" "
                                  <<fst[7]<<" "
                                  <<fst[8]<<" "
                                  <<fst[9]<<" "
                                  <<fst[10]<<" "
                                  <<fst[11]<<" "
                                  <<fst[12]<<std::endl;}  
  
  //if(nchr==3) part->printTree();
   if( m_NtupleFile ){ 
    m_nchr   = nchr;
    m_nchr_e = nchr_e;
    m_nchr_mu = nchr_mu;
    m_nchr_pi = nchr_pi;
    m_nchr_k = nchr_k;
    m_nchr_p = nchr_p;
    m_gamma=N_gamma;
    m_gammaFSR=N_gammaFSR;
    TotNumTrk = AllTrk_index; //nchr + N_gamma + N_gammaFSR;  
   }

    //debug
   if(m_ReadTruth.size())ReadTruth(part,m_ReadTruth);


   //if(part->getId()==EvtPDL::getId("psi(4260)")) std::cout<<"mydebug "<<part->getP4().mass()<<std::endl;

    part->deleteTree();
    return StatusCode::SUCCESS;
}
 
//************ end of callBesEvtGen *********************
StatusCode EvtDecay::finalize() 
{

  if(EvtCalHelAmp::nevt>0){
    double H2=EvtCalHelAmp::_H2;
    double H1=EvtCalHelAmp::_H1;
    double H0=EvtCalHelAmp::_H0;
    double H2err=EvtCalHelAmp::_H2err;
    double H1err=EvtCalHelAmp::_H1err;
    double H0err=EvtCalHelAmp::_H0err;
    int nd = EvtCalHelAmp::nevt;
    std::cout<<"Calculated helicity amplitude sqaured are:"<<std::endl;
    std::cout<<"|H_{2}|^2=|H_{-2}|^2= "<<H2/nd<<" +/- "<<sqrt(H2err/nd)<<endl;
    std::cout<<"|H_{1}|^2=|H_{-1}|^2= "<<H1/nd<<" +/- "<<sqrt(H1err/nd)<<endl;
    //std::cout<<"|H_{0}|^2= "<<H0/nd           <<" +/- "<<sqrt(H0err/nd)<<endl;
  }
  MsgStream log(messageService(), name());
  truth.close();
  log << MSG::INFO << "EvtDecay finalized" << endreq;
   fstream Forfile;
   Forfile.open("fort.16",ios::in);
   char delcmd[300];
   strcpy(delcmd,"rm -f ");
   strcat(delcmd,"fort.16");
   // if(Forfile)system(delcmd);

   fstream Forfile2;
   Forfile2.open("fort.22",ios::in);
   strcpy(delcmd,"rm -f ");
   strcat(delcmd,"fort.22");
   // if(Forfile2)system(delcmd);

   //   To get the branching fraction of the specified mode in Lundcharm Model
   /*    
   EvtLundCharm lundcharmEvt;
   int nevt=lundcharmEvt.getTotalEvt();
   std::cout<<"The total number of the specified mode is  "<<nevt<<std::endl;
   */ 
   int totalEvt=0;
   if(!(m_SB3File=="" || m_SB3HT=="")){
   for(int i=0;i<500;i++){totalEvt=totalEvt+br[i];}
   for(int i=0;i<500;i++){
     double bi=1.0*br[i]/totalEvt;
     std::cout<<"Branching fraction of channel "<<i<<" is: "<<bi<<std::endl;
     if(br[i]==0) break;
   }
   }

   if(m_statDecays){
     int totalEvt=0;
     for(int i=0;i<=totalChannels;i++){totalEvt=totalEvt+br[i];}
     std::cout<<"==================Statistical first chain decay ==============================="<<std::endl;
     std::cout<<"i-th channel      Events Generated         Branching fraction generated        "<<std::endl;
     for(int i=0;i<=totalChannels;i++){
      std::cout<<"|  "<<i<<"              "<<br[i]<<"                        "<<br[i]*1.0/totalEvt<<std::endl;      

     }
     std::cout<<"--------------------------------------------------------------------------------"<<std::endl;
     std::cout<<"  sum:            "<<totalEvt<<"                     "<<"1"<<std::endl;
     std::cout<<"================== End of statistical first chain decay ========================"<<std::endl;
   }

   return StatusCode::SUCCESS;
}


StatusCode EvtDecay::makeHepMC(EvtParticle* part, HepMC::GenEvent* hEvt, 
                               HepMC::GenParticle* hPart)
{  
  MsgStream log(messageService(), name());

  if(part->getNDaug()!=0)
    {  
      double ct=(part->getDaug(0)->get4Pos()).get(0);
      double x=(part->getDaug(0)->get4Pos()).get(1);
      double y=(part->getDaug(0)->get4Pos()).get(2);
      double z=(part->getDaug(0)->get4Pos()).get(3);
         
      HepMC::GenVertex* end_vtx = new HepMC::GenVertex(HepLorentzVector(x,y,z,ct));
      hEvt->add_vertex( end_vtx );
      end_vtx->add_particle_in(hPart);

      int ndaug=part->getNDaug();
            
      for(int it=0;it<ndaug;it++)
	{
                
	  double e=(part->getDaug(it)->getP4Lab()).get(0);
	  double px=(part->getDaug(it)->getP4Lab()).get(1);
	  double py=(part->getDaug(it)->getP4Lab()).get(2);
	  double pz=(part->getDaug(it)->getP4Lab()).get(3);
	  int id=EvtPDL::getStdHep(part->getDaug(it)->getId());
	  int status=1;

	  if(part->getDaug(it)->getNDaug()!=0) status=999;

	  HepMC::GenParticle* prod_part = new HepMC::GenParticle(HepLorentzVector(px,py,pz,e),id,status);
	  end_vtx->add_particle_out(prod_part); 
	  makeHepMC(part->getDaug(it),hEvt,prod_part);

      
     
	  if( log.level()<MSG::INFO )
	   prod_part->print();
	}
    }

  return StatusCode::SUCCESS;
}


void EvtDecay::MeVToGeV     (HepMC::GenEvent* evt)
{
  for ( HepMC::GenEvent::particle_iterator p = evt->particles_begin(); p != evt->particles_end(); ++p ) {
    //              cout << " PDG, BAR " << (*p)->pdg_id() << " " << (*p)->barcode() << endl;
  //  (*p)->set_momentum( (*p)->momentum() / 1000. );

                                 HepMC::FourVector tmpfv((*p)->momentum().x()/1000.,
                                 (*p)->momentum().y()/1000.,
                                 (*p)->momentum().z()/1000.,
                                 (*p)->momentum().t()/1000.
                                                                                                                                );

         (*p)->set_momentum( tmpfv );
  }
}

void EvtDecay::GeVToMeV     (HepMC::GenEvent* evt)
{
  for ( HepMC::GenEvent::particle_iterator p = evt->particles_begin(); p != evt->particles_end(); ++p ) {
    //              cout << " PDG, BAR " << (*p)->pdg_id() << " " << (*p)->barcode() << endl;
   // (*p)->set_momentum( (*p)->momentum() * 1000. );
                                 HepMC::FourVector tmpfv((*p)->momentum().x()*1000.,
                                 (*p)->momentum().y()*1000.,
                                 (*p)->momentum().z()*1000.,
                                 (*p)->momentum().t()*1000.
                                                                                                                                );

         (*p)->set_momentum( tmpfv );
  }
}
 

double EvtDecay::CalAmpsMax( EvtParticle* part )
{
  double amps_max=0;
  for(int i=0;i<100000;i++){
  m_Gen->generateDecay(part);
  double amps=CalAmpsMDIY(part);
  if(amps > amps_max) amps_max=amps;
  }
  amps_max=amps_max*1.05;
  m_ampscalflag = false;
  return amps_max;
}

// EvtBesRandom class implementation, basically BesRandomSvc -> EvtGen random engine interface

EvtBesRandom::EvtBesRandom(HepRandomEngine* engine)
{
 m_engine = engine;
 m_engine->showStatus();
}

EvtBesRandom::~EvtBesRandom()
{}

double EvtBesRandom::random()
{ 
  //double  rdm=EvtRandom::Flat(0.0, 1.0);;
  double rdm= RandFlat::shoot(m_engine);  
  // std::cout<<"rdm= "<<rdm<<std::endl;
  return rdm;
  //return RandFlat::shoot(m_engine);
}


void EvtDecay::SuperBody3decay_make(EvtId ppid,  EvtVector4R p_init ){
    double xmass2,ymass2;
    bool accept;
    EvtVector4R pd1,pd2,pd3;

    //---- find out the pdg codes of final state and count the identical particles
      FinalState_make( ppid, p_init); 

    //begin to loop with events
      for(int i=0;i<100000;i++){ 
      regen:
       EvtParticle* part=EvtParticleFactory::particleFactory(ppid,p_init);
       HepMC::GenParticle* hepMCpart=new GenParticle(HepLorentzVector(0,0,0,parentMass),parentPDGcode,3);  //this line make the decay to select different channel

       if(part->getSpinType() == EvtSpinType::VECTOR) {
	  part->setVectorSpinDensity();
        }
 	m_Gen->generateDecay(part);
       

	FinalState_sort(part);
        pd1=son0;
	pd2=son1;
	pd3=son2;

	//    std::cout<<"multi, pdg, pi= "<<multi<<" "<<pdg0<<" "<<pdg1<<" "<<pdg2<<" "<<son0<<son1<<son2<<std::endl;

	xmass2=(pd1+pd2).mass2(); // Dalitz plot m12^2 ~ m13^2 
	ymass2=(pd1+pd3).mass2();
        double xlow=SuperBody3decay.getXlow();
        double xup=SuperBody3decay.getXup();
        double ylow=SuperBody3decay.getYlow();
        double yup=SuperBody3decay.getYup();
        if(xmass2<xlow || xmass2>xup || ymass2<ylow || ymass2>yup) {  part->deleteTree();goto regen;}
        SuperBody3decay.HFill(xmass2,ymass2);

	if( m_NtupleFile ){
        m_ich=part->getChannel();
        m_m1=pd1.mass(); 
        m_m2=pd2.mass();
        m_m3=pd3.mass();
        m_m12=(pd1+pd2).mass();
        m_m23=(pd2+pd3).mass();
        m_m13=(pd1+pd3).mass();
        m_cos1=pd1.get(3)/pd1.d3mag();
        m_cos2=pd2.get(3)/pd2.d3mag();
        m_cos3=pd3.get(3)/pd3.d3mag();
	mass_tuple->write();
	}
	double m1=pd1.mass(); double m2=pd2.mass();double m3=pd3.mass();
	 double mj=(pd2+pd3).mass();
	 //	  std::cout<<"mass 1 2 3 chicj <<=="<<m1<<" "<<m2<<" "<<m3<<" "<<mj<<std::endl;
	 //	 delete hepMCpart;
      }
  
      SuperBody3decay.HReweight(); //reweighting Dalitz plot
      SuperBody3decay.setZmax();   // find out the maximum value after reweighting
      first=false;
}

bool  EvtDecay::SuperBody3decay_judge(EvtParticle* part){
    double xmass2,ymass2;
    bool accept;
    EvtVector4R pd1,pd2,pd3;


     	FinalState_sort( part);

        pd1=son0;
	pd2=son1;
	pd3=son2;


      xmass2=(pd1+pd2).mass2(); // Dalitz plot m12^2 ~ m13^2 
      ymass2=(pd1+pd3).mass2();
      accept=SuperBody3decay.AR(xmass2,ymass2); 

      //debugging
     if(accept && m_NtupleFile) {
     _ich=part->getChannel();
     _m1=pd1.mass(); 
     _m2=pd2.mass();
     _m3=pd3.mass();
     _m12=(pd1+pd2).mass();
     _m23=(pd2+pd3).mass();
     _m13=(pd1+pd3).mass();
     _cos1=pd1.get(3)/pd1.d3mag();
     _cos2=pd2.get(3)/pd2.d3mag();
     _cos3=pd3.get(3)/pd3.d3mag();
     massgen_tuple->write();
     }
     //     std::cout<<"mass 1 2 3 chicj>> "<<_m1<<" "<<_m2<<" "<<_m3<<" "<<_m23<<std::endl;

      return accept;
}


void EvtDecay:: FinalState_make(EvtId ppid,  EvtVector4R p_init){
  // get the final state pdg cond and count the identicle particle
  multi=1;
  identical_flag=true;

  for(int i=1;i<10000;i++){ //sigle out the final state
   EvtParticle* part=EvtParticleFactory::particleFactory(ppid,p_init);
   HepMC::GenParticle* hepMCpart=new GenParticle(HepLorentzVector(0,0,0,parentMass),parentPDGcode,3); 

   m_Gen->generateDecay(part);
   int nson=part->getNDaug();

   if(nson == 2) {continue;} else 
   if(nson ==3){
   EvtId xid0,xid1,xid2;
   xid0=part->getDaug(0)->getId();
   xid1=part->getDaug(1)->getId();
   xid2=part->getDaug(2)->getId();
   
//-- pdg code 
   pdg0=EvtPDL::getStdHep(xid0);
   pdg1=EvtPDL::getStdHep(xid1);
   pdg2=EvtPDL::getStdHep(xid2);

   if(pdg0==pdg1 && pdg1==pdg2) {multi=3;} 
   else if(pdg0==pdg1 ){multi=2;}  // two identical particle list as 0,1 index
   else if(pdg0==pdg2 ){multi=2;pdg=pdg1; pdg1=pdg2; pdg2=pdg;} 
   else if(pdg1==pdg2) {multi=2;pdg=pdg0; pdg0=pdg1; pdg1=pdg2; pdg2=pdg;} 
   else {multi=1;}
   break;
   } else{
     std::cout<<"No direct three body decay channel found in decay card, I stop run"<<std::endl;
     ::abort();
   }   
  }
  //  std::cout<<"pdg_i "<<pdg0<<" "<<pdg1<<" "<<pdg2<<std::endl;
  //  std::cout<<"identical particle "<<multi<<std::endl;
}

void EvtDecay::FinalState_sort(EvtParticle* part){
  // sort the momentum to aon0, son1, son2
   EvtVector4R pd0,pd1,pd2;
   EvtId xid0,xid1,xid2;
   int id0,id1,id2;

    int nson=part->getNDaug();
    if(nson==2){
      pd0=part->getDaug(0)->getP4Lab();
      pd1=part->getDaug(1)->getDaug(0)->getP4Lab();
      pd2=part->getDaug(1)->getDaug(1)->getP4Lab();

      xid0=part->getDaug(0)->getId();
      xid1=part->getDaug(1)->getDaug(0)->getId();
      xid2=part->getDaug(1)->getDaug(1)->getId();	 

     } else if(nson==3){
      pd0=part->getDaug(0)->getP4Lab();
      pd1=part->getDaug(1)->getP4Lab();
      pd2=part->getDaug(2)->getP4Lab();

      xid0=part->getDaug(0)->getId();
      xid1=part->getDaug(1)->getId();
      xid2=part->getDaug(2)->getId();
     }
      //-- pdg code
      id0=EvtPDL::getStdHep(xid0);
      id1=EvtPDL::getStdHep(xid1);
      id2=EvtPDL::getStdHep(xid2);

      //      std::cout<<"pid before match: "<<id0<<" "<<id1<<" "<<id2<<std::endl; 
      //-- assign sons momentum
      if(multi==1){
	assign_momentum(id0,pd0);
	assign_momentum(id1,pd1);
	assign_momentum(id2,pd2);
      } else if(multi==2){
	assign_momentum2(id0,pd0);
	assign_momentum2(id1,pd1);
	assign_momentum2(id2,pd2);
       if(son0.get(0)>son1.get(0)){son=son0;son0=son1;son1=son;} // change into E_0 < E_1
      } else if(multi==3){    // sort sons according to energy E_0 < E_1 < E_2
        double En0=pd0.get(0);
	double En1=pd1.get(0);
	double En2=pd2.get(0);
	if(En0 < En1 && En1 < En2) {son0=pd0;son1=pd1;son2=pd2;}
	if(En0 < En2 && En2 < En1) {son0=pd0;son1=pd2;son2=pd1;}
	if(En1 < En0 && En0 < En2) {son0=pd1;son1=pd0;son2=pd2;}
	if(En1 < En2 && En2 < En0) {son0=pd1;son1=pd2;son2=pd0;}
	if(En2 < En0 && En0 < En1) {son0=pd2;son1=pd0;son2=pd1;}
	if(En2 < En1 && En1 < En0) {son0=pd2;son1=pd1;son2=pd0;}
      }
 
}


void EvtDecay::assign_momentum(int id0, EvtVector4R pd0){
      if(id0==pdg0){son0=pd0;}
 else if(id0==pdg1){son1=pd0;}
 else if(id0==pdg2){son2=pd0;}
      else {std::cout<<"PID= "<<id0<<" not machted, I stop"<<std::endl; ::abort();}
}

void EvtDecay::assign_momentum2(int id0, EvtVector4R pd0){ // for two identicle particle case
      if(id0==pdg0 && identical_flag){son0=pd0;identical_flag=false;}
 else if(id0==pdg1){son1=pd0;identical_flag=true;}
 else if(id0==pdg2){son2=pd0;}
      else {std::cout<<"PID not machted, I stop"<<std::endl; ::abort();}
}

void EvtDecay::findPart(EvtParticle* part){
  FDP_id=EvtPDL::getId(m_FDPparticle);
  int FDP_pdg=EvtPDL::getStdHep(FDP_id);

  m_Gen->generateDecay(part);
  int dn=part->getNDaug();

  if(dn!=0){
    for(int i=0;i<dn;i++){
      EvtParticle* part1=part->getDaug(i);
      EvtId sonid=part1->getId();
      int son_pdg = EvtPDL::getStdHep(sonid);
      if(son_pdg == FDP_pdg){
	FDP_init=part1->getP4Lab();
	FDP_part=part1;
	return;
      } else{
      findPart(part1);
      }
    }
  } //if (dn block

}

void EvtDecay::countChannel(EvtParticle* part){
  int ich=part->getChannel();

  //debuging
  //if(getModel(part,ich)=="OPENCHARM" &&EvtPDL::name( part->getId() )=="psi(4260)") ich = part->getGeneratorFlag();
  //std::cout<<"the channel is "<<ich<<std::endl;

  br[ich]++;
  if(ich>totalChannels) totalChannels = ich;
  //std::cout<<"EVENT IN br_i "<<br[ich]<<std::endl;
}

bool EvtDecay::isCharmonium(EvtId xid){
EvtId psi4415 = EvtPDL::getId(std::string("psi(4415)"));
EvtId psi4160 = EvtPDL::getId(std::string("psi(4160)"));
EvtId psi4040 = EvtPDL::getId(std::string("psi(4040)"));
EvtId psi3770 = EvtPDL::getId(std::string("psi(3770)"));
EvtId psiprim = EvtPDL::getId(std::string("psi(2S)"));
EvtId jpsi    = EvtPDL::getId(std::string("J/psi"));
EvtId etac    = EvtPDL::getId(std::string("eta_c"));
EvtId etac2s  = EvtPDL::getId(std::string("eta_c(2S)"));
EvtId hc = EvtPDL::getId(std::string("h_c"));
EvtId chic0   = EvtPDL::getId(std::string("chi_c0"));
EvtId chic1   = EvtPDL::getId(std::string("chi_c1"));
EvtId chic2   = EvtPDL::getId(std::string("chi_c2"));
EvtId chic0p  = EvtPDL::getId(std::string("chi_c0p"));
EvtId chic1p  = EvtPDL::getId(std::string("chi_c1p"));
EvtId chic2p  = EvtPDL::getId(std::string("chi_c1p"));
 std::vector<EvtId> Vid; Vid.clear();
 Vid.push_back(psi4415);
 Vid.push_back(psi4160);
 Vid.push_back(psi4040);
 Vid.push_back(psi3770);
 Vid.push_back(psiprim);
 Vid.push_back(jpsi);
 Vid.push_back(etac);
 Vid.push_back(etac2s);
 Vid.push_back(hc);
 Vid.push_back(chic0);
 Vid.push_back(chic1);
 Vid.push_back(chic2);
 Vid.push_back(chic0p);
 Vid.push_back(chic1p);
 Vid.push_back(chic2p);

 bool flag=true;
 for(int i=0;i<Vid.size();i++){ if(xid == Vid[i]) return flag;}
 return false;
}


bool EvtDecay::isCharm(EvtId xid){
EvtId d0    = EvtPDL::getId(std::string("D0"));
EvtId d0bar = EvtPDL::getId(std::string("anti-D0"));
EvtId dp    = EvtPDL::getId(std::string("D+"));
EvtId dm    = EvtPDL::getId(std::string("D-"));
EvtId d0h   = EvtPDL::getId(std::string("D0H"));
EvtId d0l    = EvtPDL::getId(std::string("D0L"));
EvtId dstp   = EvtPDL::getId(std::string("D*+"));
EvtId dstm   = EvtPDL::getId(std::string("D*-"));
EvtId ds0    = EvtPDL::getId(std::string("D*0"));
EvtId ds0bar = EvtPDL::getId(std::string("anti-D*0"));
EvtId dsp    = EvtPDL::getId(std::string("D_s+"));
EvtId dsm    = EvtPDL::getId(std::string("D_s-"));
EvtId dsstp  = EvtPDL::getId(std::string("D_s*+"));
EvtId dsstm  = EvtPDL::getId(std::string("D_s*-"));
EvtId ds0stp = EvtPDL::getId(std::string("D_s0*+"));
EvtId ds0stm = EvtPDL::getId(std::string("D_s0*-"));

 std::vector<EvtId> Vid; Vid.clear();
 Vid.push_back(d0);
 Vid.push_back(d0bar);
 Vid.push_back(dp);
 Vid.push_back(dm);
 Vid.push_back(d0h);
 Vid.push_back(d0l);
 Vid.push_back(dstp);
 Vid.push_back(dstm);
 Vid.push_back(ds0);
 Vid.push_back(ds0bar );
 Vid.push_back(dsp );
 Vid.push_back(dsm );
 Vid.push_back(dsstp );
 Vid.push_back(dsstm );
 Vid.push_back(ds0stp );
 Vid.push_back(ds0stm );

 bool flag=true;
 for(int i=0;i<Vid.size();i++){ if(xid == Vid[i]) return flag;}
 return false;
}

bool EvtDecay::isRadecay(EvtParticle *par){
  int ndg = par->getNDaug();
  for(int i=0;i<ndg;i++){
    EvtId xid = par->getDaug(i)->getId();
    if(EvtPDL::getStdHep(xid) == 22){return true;}
  }
  return false;
}

int EvtDecay::decayType(EvtParticle *par){
 /*************************************
  * 0: gamma light_hadrons
  * 1: gamma charmonium
  * 2: DDbar (D0 D0bar or D+D-)
  * 3: ligh_hadrons
  * 4: others
  *************************************/
  int Nchannel=par->getChannel();
  int ndg = par->getNDaug();
  int nfsr=0;

  if(_RvalueTag){return Nchannel;}
  //  std::cout<<"decayType= "<<Nchannel<<std::endl;

  std::string model = getModel(par,Nchannel);
  if( model == "OPENCHARM" || model == "LUNDCHARM" && m_tagLundModel){ // lundcharm model tag
    int ldm = par->getGeneratorFlag();
    // std::cout<<"LundCharmFlag = "<<ldm<<std::endl;
    return ldm;
    //return 9;
  }

  for(int i=0;i<ndg;i++){ //remove the FSR photon
    EvtId xid =par->getDaug(i)->getId(); 
    if(EvtPDL::getStdHep(xid) == -22 ){nfsr++;}
      }

  if( isRadecay(par) ){
    for(int i=0;i<ndg;i++){
      EvtId xid = par->getDaug(i)->getId();
      if( isCharmonium(xid) ) return 1;
      }
    return 0;
  } else{
    if(ndg-nfsr ==2 ){
    int ndg = par->getNDaug();
      EvtId xd1 = par->getDaug(0)->getId();
      EvtId xd2 = par->getDaug(1)->getId();
      if( isCharm(xd1) && isCharm(xd2) ) {return 2;} else 
      if( !isCharmonium(xd1) && !isCharmonium(xd2) && !isCharm(xd1) && !isCharm(xd2) ){
	return 3;
      }
    } else{  // ndg>=3
      bool flag = false;
      for(int i=0;i<ndg;i++){
        EvtId xid = par->getDaug(i)->getId();
	if( isCharmonium(xid) ) {flag = true; break;}
	if( isCharm(xid)      ) {flag = true; break;}
      } // for_i block
      if( !flag ){return 3;} else {return 4;}
    }// if_ndg block
  }  
 
}


std::string EvtDecay::getModel(EvtParticle *par, int mode){
  EvtDecayBase* thedecay = EvtDecayTable::gettheDecay(par->getId(),mode);
  return thedecay->getModelName();
}

std::string EvtDecay::getModel(EvtParticle *par){
  int mode = par->getChannel();
  EvtDecayBase* thedecay = EvtDecayTable::gettheDecay(par->getId(),mode);
  return thedecay->getModelName();
}
 

void EvtDecay::ReadTruth(EvtParticle* part,std::vector<std::vector<string> > mylist){
  if(mylist.size()<2) {std::cout<<" Empty list"<<std::endl;abort();}
  EvtVector4R vp1;
  for(int k=0;k<mylist[0].size();k++){
    if(part->getId()==EvtPDL::getId(mylist[0][k])){
      if(part->getDaug(1)->getId()==EvtPDL::getId("vhdr")){part=part->getDaug(1);continue;}
      if(part->getDaug(0)->getId()==EvtPDL::getId("vgam")){part=part->getDaug(1);continue;}
      for(int i=1;i<mylist.size();i++){
	EvtParticle* mypart=part;
	for(int j=0;j<mylist[i].size();j++){
	  mypart=mypart->getDaug(atoi(mylist[i][j].c_str()));
	  //std::cout<<atoi(mylist[i][j].c_str());
	}
	//std::cout<<std::endl;
	std::cout<<EvtPDL::name(mypart->getId())<<std::endl;
	vp1=mypart->getP4Lab();
	if( !isNumber(vp1.get(0)) ) {part->printTree(); abort();}
	std::cout<<"truth "<<vp1.get(0)<<" "<<vp1.get(1)<<" "<<vp1.get(2)<<" "<<vp1.get(3)<<std::endl;
      }
    }
  }
}

int EvtDecay::isNumber(double d){return (d==d);}//if d=nan, return 0, otherwise, return 1


double EvtDecay::energySpread(double mu,double sigma){
  //mu: mean value in Gaussian
  //sigma: variance in Gaussian 
 rloop:
  int n=12;
  double ri=0;
  for(int i=0;i<n;i++){
    double pm= EvtRandom::Flat(0.,1);
    ri += pm;
  }
  double eta=sqrt(n*12.0)*(ri/12-0.5);
  double xsig= sigma*eta+mu;
  double mx0=EvtConExc::mlow;
  double mx1=EvtConExc::mup;
  if(xsig<mx0 || xsig>mx1) goto rloop;
  return xsig;
}

#include "../user/Lenu.inc"
