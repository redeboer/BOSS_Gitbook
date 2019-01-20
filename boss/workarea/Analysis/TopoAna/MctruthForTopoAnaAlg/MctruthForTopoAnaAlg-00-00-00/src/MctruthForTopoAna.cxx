// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "GaudiKernel/SmartDataPtr.h"
	#include "EventModel/EventHeader.h"
	#include "McTruth/McParticle.h"
	#include "MctruthForTopoAnaAlg/MctruthForTopoAna.h"    



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/**
	 * @brief Constructor for this class is emptry, but needs to be defined, because it inherits from `Algorithm`.
	 */
	MctruthForTopoAna::MctruthForTopoAna(const std::string & name,ISvcLocator * pSvcLocator): Algorithm(name,pSvcLocator) {}



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //


	/**
	 * @brief Book `NTuple` for MC truth.
	 * @details The contents of this `NTuple` are required for the `topoana` package to function properly.
	 */
	StatusCode MctruthForTopoAna::initialize()
	{
		/// <ol>
		/// <li> Create log stream (`MsgStream` object).
			MsgStream log(msgSvc(),name());
			log << MSG::INFO << "in initialize()" << endmsg;
		/// <li> Book `NTuple` called `"MctruthForTopoAna"`. A selection of MC truth data will be written to this `TTree`. This tree contains:
			NTuplePtr nt(ntupleSvc(),"FILE1/MctruthForTopoAna");
			if(nt) m_tuple=nt;
			else {
				m_tuple=ntupleSvc()->book("FILE1/MctruthForTopoAna",CLID_ColumnWiseTuple,"Mctruth For Topology Analysis");
				if(m_tuple) {
				/// <ul>
					m_tuple->addItem("Ievt",  m_ievt);  /// <li> `"Ievt"`: <b>counter</b> for number of events (not the ID!).
					m_tuple->addItem("Runid", m_runid); /// <li> `"Runid"`: run number ID.
					m_tuple->addItem("Evtid", m_evtid); /// <li> `"Evtid"`: event number ID.
					m_tuple->addItem("Nmcps", m_nmcps, 0, 100); /// <li> `"Nmcps"`: number of MC particles stored for this event. This one is necessary for loading following two items, because they are arrays.
					m_tuple->addIndexedItem("Pid",  m_nmcps, m_pid); /// <li> `"Pid"` (array): PDG code for the particle in this array.
					m_tuple->addIndexedItem("Midx", m_nmcps, m_midx); /// <li> `"Midx"` (array): track index of the mother particle (corrected with `rootIndex`).
				/// </ul>
				} else {
					log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple) << endmsg;
					/// <li> Abort if booking the `NTuple` has failed.
					return StatusCode::FAILURE;
				}
			}
		/// <li> Reset counter for number of events.
			m_ievt = 0;
		/// </ol>
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Write MC truth properties <i>for each event</i>.
	 */
	int ievt=0;
	StatusCode MctruthForTopoAna::execute() 
	{
		/// <ol>
		/// <li> Create log stream object and increase counter for number of events.
			MsgStream log(msgSvc(), name());
			++m_ievt;
			// std::cout << std::endl << std::endl;
			// std::cout << "ievt = " << ievt << std::endl;

		/// <li> Load event from DST file and get run and event number.
			SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
			m_runid=eventHeader->runNumber();
			m_evtid=eventHeader->eventNumber();
			// std::cout << "RunNo=" << m_runid << std::endl;
			// std::cout << "EvtNo=" << m_evtid << std::endl;

		/// <li> Attempt to get 'Monte Carlo truth'.
			SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(),"/Event/MC/McParticleCol");
			if(!mcParticleCol) {
				std::cout << "Could not retrieve McParticelCol" << std::endl;
				return(StatusCode::FAILURE);  
			}

		/// <li> Attempt to get 'Monte Carlo truth'.
			/// <ul>
			m_nmcps = 0; /// <li> Reset the `m_nmcps` counter for the number of MC particles.
			const int incPid1 = 91; /// <li> `91` is the PDG code of cluster.
			const int incPid2 = 92; /// <li> `92` is the PDG code of string.
			bool incPdcy(false);
			int rootIndex(-1);
			/// </ul>

		/// <li> Loop over collection of MC particle objects. An array containing a selection of MC truth particles is created here.
			/// <ul>
			Event::McParticleCol::iterator it = mcParticleCol->begin();
			for(; it!=mcParticleCol->end(); it++) {
				// std::cout << "idx=" << m_nmcps << "\t" << "pid=" << (*it)->particleProperty() << "\t" << "midx=" << ((*it)->mother()).trackIndex() << std::endl;
				/// <li> Only decayed particles are of interest to the `topoana` package.
				if((*it)->primaryParticle()) continue;
				if(!(*it)->decayFromGenerator()) continue;
				/// <li> The `rootIndex` variable is used to skip particles like the \f$Z\f$ boson in the decay chain after the \f$e^+e^-\f$ collision. It has to be stored once and untill it is stored, all MC truth particles are rejected (which is what the `incPdcy` boolean is for).
				if(
					(*it)->particleProperty() == incPid1 ||
					(*it)->particleProperty() == incPid2 ) {
					incPdcy = true;
					rootIndex = (*it)->trackIndex();
				}
				/// <li> Skip if this particle is the initial cluster.
				if(!incPdcy ||
					(*it)->particleProperty() == incPid1 ||
					(*it)->particleProperty() == incPid2 ) continue;
				/// <li> Store PDG code of this MC particle.
				m_pid[m_nmcps] = (*it)->particleProperty();
				/// <li> Store track ID of the mother. This ID is `-1` if it is the initial cluster (e.g. \f$J/\psi\f$).
				if(
					((*it)->mother()).particleProperty() == incPid1 ||
					((*it)->mother()).particleProperty() == incPid2 ) m_midx[m_nmcps] = -1;
				else m_midx[m_nmcps] = ((*it)->mother()).trackIndex()-rootIndex-1;
				m_nmcps++;
			}
			/// </ul>

		/// <li> <b>Write</b> info stored in the loop to the `"MctruthForTopoAna"` `TTree`.
			m_tuple->write();

		/// </ol>
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Print number of events.
	 */
	StatusCode MctruthForTopoAna::finalize()
	{
		MsgStream log(msgSvc(), name());
		std::cout << "ievt=" << ievt << std::endl;
		log << MSG::INFO << "in finalize()" << endmsg;
	}