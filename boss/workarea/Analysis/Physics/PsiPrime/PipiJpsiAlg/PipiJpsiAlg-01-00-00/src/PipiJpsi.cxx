// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "DstEvent/TofHitStatus.h"
	#include "EventModel/Event.h"
	#include "EventModel/EventHeader.h"
	#include "EventModel/EventModel.h"
	#include "EvtRecEvent/EvtRecEvent.h"
	#include "EvtRecEvent/EvtRecTrack.h"
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/Bootstrap.h"
	#include "GaudiKernel/IDataProviderSvc.h"
	#include "GaudiKernel/IHistogramSvc.h"
	#include "GaudiKernel/INTupleSvc.h"
	#include "GaudiKernel/ISvcLocator.h"
	#include "GaudiKernel/MsgStream.h"
	#include "GaudiKernel/NTuple.h"
	#include "GaudiKernel/PropertyMgr.h"
	#include "GaudiKernel/SmartDataPtr.h"
	#include "McTruth/McParticle.h"
	#include "MdcRecEvent/RecMdcKalTrack.h"
	#include "ParticleID/ParticleID.h"
	#include "PipiJpsiAlg/PipiJpsi.h"
	#include "TMath.h"
	#include "TrigEvent/TrigData.h"
	#include "TrigEvent/TrigEvent.h"
	#include "VertexFit/KinematicFit.h"
	#include "VertexFit/SecondVertexFit.h"
	#include "VertexFit/VertexFit.h"
	#include "VertexFit/WTrackParameter.h"
	#include <vector>
	#ifndef ENABLE_BACKWARDS_COMPATIBILITY
		typedef HepGeom::Point3D<double> HepPoint3D;
	#endif
	using CLHEP::Hep3Vector;
	using CLHEP::Hep2Vector;
	using CLHEP::HepLorentzVector;



// * ==================================== * //
// * ------- GLOBALS AND TYPEDEFS ------- * //
// * ==================================== * //

	// * Constants * //
		const double me  = 0.000511;
		const double mpi = 0.13957;
		const double mproton = 0.938272;
		const double mmu = 0.105658;
		const double mpsip = 3.686;
		const double xmass[5] = {
			0.000511, // electron
			0.105658, // muon
			0.139570, // charged pion
			0.493677, // charged kaon
			0.938272  // proton
		};
		const double velc = 29.9792458;  // tof_path unit in cm.
		const double PI = 3.1415926;


	// * Typedefs * //
		typedef std::vector<int> Vint;
		typedef std::vector<HepLorentzVector> Vp4;


	// * Counters for efficiency * //
		static long m_cout_all(0);
		static long m_cout_col(0);
		static long m_cout_charge(0);
		static long m_cout_nGood(0);
		static long m_cout_mom(0);
		static long m_cout_recoil(0);
		static long m_cout_everat(0);



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //
	/**
	 * @brief Constructor for the `PipiJpsi` algorithm.
	 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `RhopiAlg`), and if required a documentation string. Note that you should define the paramters themselves in the header (RhopiAlg/RhopiAlg.h) and that you should assign the values in `share/jopOptions_RhopiAlg.txt`. Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html.
	 */
	PipiJpsi::PipiJpsi(const std::string& name, ISvcLocator* pSvcLocator) :
		Algorithm(name, pSvcLocator)
	{
		// * Declare the properties * //
		declareProperty("Vr0cut", m_vr0cut=1.0);
		declareProperty("Vz0cut", m_vz0cut=5.0);
		declareProperty("TrackCosThetaCut", m_cosThetaCut=0.93);
		declareProperty("PipiDangCut", m_pipi_dang_cut=0.98);

		declareProperty("CheckDedx", m_checkDedx = true);
		declareProperty("CheckTof",  m_checkTof = true);

		declareProperty("Subsample",   m_subsample_flag=false);
		declareProperty("Trigger",     m_trigger_flag=false);
		declareProperty("DistinEMuon", m_distin_emuon=2.0);

		declareProperty("EventRate", m_eventrate=false);
		declareProperty("ChanDet",   m_chan_det=1);
	}



// * ========================== * //
// * ------- INITIALIZE ------- * //
// * ========================== * //
	/**
	 * @brief (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning. <b>Define and load `NTuple`s here.</b>
	 */
	StatusCode PipiJpsi::initialize()
	{
		// * Log stream and status code * //
			MsgStream log(msgSvc(), name());
			log << MSG::INFO << "in initialize()" << endmsg;

		/// <table>
		/// <tr><td colspan="2"><b>`NTuple "vxyz"`:   Vertex information of the charged tracks</b></td></tr>
			NTuplePtr nt1(ntupleSvc(), "FILE1/vxyz");
			if(nt1) m_tuple1 = nt1;
			else {
				m_tuple1 = ntupleSvc()->book("FILE1/vxyz", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(m_tuple1) {
					m_tuple1->addItem("vx0", m_vx0); ///<tr><td>`"vx0"`</td><td>Primary \f$x\f$-vertex as determined by MDC</td></tr>
					m_tuple1->addItem("vy0", m_vy0); ///<tr><td>`"vy0"`</td><td>Primary \f$y\f$-vertex as determined by MDC</td></tr>
					m_tuple1->addItem("vz0", m_vz0); ///<tr><td>`"vz0"`</td><td>Primary \f$z\f$-vertex as determined by MDC</td></tr>
					m_tuple1->addItem("vr0", m_vr0); ///<tr><td>`"vr0"`</td><td>Distance from origin in \f$xy\f$-plane</td></tr>
				} else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple1) << endmsg;
					return StatusCode::FAILURE;
				}
			}

		/// <tr><td colspan="2"><b>`NTuple "photon"`: 'Fake' photon (angles)</b></td></tr>
			NTuplePtr nt2(ntupleSvc(), "FILE1/photon");
			if(nt2) m_tuple2 = nt2;
			else {
				m_tuple2 = ntupleSvc()->book("FILE1/photon", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(m_tuple2) {
					m_tuple2->addItem("dthe", m_dthe); ///<tr><td>`"dthe"`</td><td>\f$\theta\f$ angle difference with nearest charged track (degrees)</td></tr>
					m_tuple2->addItem("dphi", m_dphi); ///<tr><td>`"dphi"`</td><td>\f$\phi\f$ angle difference with nearest charged track (degrees)</td></tr>
					m_tuple2->addItem("dang", m_dang); ///<tr><td>`"dang"`</td><td>Angle difference with nearest charged track</td></tr>
					m_tuple2->addItem("eraw", m_eraw); ///<tr><td>`"eraw"`</td><td>Energy of the photon</td></tr>
					m_tuple2->addItem("nGam", m_nGam); ///<tr><td>`"nGam"`</td><td></td></tr>
				} else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple2) << endmsg;
					return StatusCode::FAILURE;
				}
			}

		/// <tr><td colspan="2"><b>`NTuple "dedx"`:   Energy loss dE/dx info</b></td></tr>
			if(m_checkDedx) {
				NTuplePtr nt3(ntupleSvc(), "FILE1/dedx");
				if(nt3) m_tuple3 = nt3;
				else {
					m_tuple3 = ntupleSvc()->book("FILE1/dedx", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(m_tuple3) {
						m_tuple3->addItem("ptrk",   m_ptrk);   ///<tr><td>`"ptrk"`  </td><td>Momentum of the track</td></tr>
						m_tuple3->addItem("chie",   m_chie);   ///<tr><td>`"chie"`  </td><td>\f$\chi^2\f$ in case of electron </td></tr>
						m_tuple3->addItem("chimu",  m_chimu);  ///<tr><td>`"chimu"` </td><td>\f$\chi^2\f$ in case of muon </td></tr>
						m_tuple3->addItem("chipi",  m_chipi);  ///<tr><td>`"chipi"` </td><td>\f$\chi^2\f$ in case of pion </td></tr>
						m_tuple3->addItem("chik",   m_chik);   ///<tr><td>`"chik"`  </td><td>\f$\chi^2\f$ in case of kaon </td></tr>
						m_tuple3->addItem("chip",   m_chip);   ///<tr><td>`"chip"`  </td><td>\f$\chi^2\f$ in case of proton </td></tr>
						m_tuple3->addItem("probPH", m_probPH); ///<tr><td>`"probPH"`</td><td>Most probable pulse height from truncated mean</td></tr>
						m_tuple3->addItem("normPH", m_normPH); ///<tr><td>`"normPH"`</td><td>Normalized pulse height</td></tr>
						m_tuple3->addItem("ghit",   m_ghit);   ///<tr><td>`"ghit"`  </td><td>Number of good hits</td></tr>
						m_tuple3->addItem("thit",   m_thit);   ///<tr><td>`"thit"`  </td><td>Total number of hits</td></tr>
					} else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple3) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			} // check dE/dx

		/// <tr><td colspan="2"><b>`NTuple "tofe"`:   Endcap ToF</b></td></tr>
			if(m_checkTof) {
				NTuplePtr nt4(ntupleSvc(), "FILE1/tofe");
				if(nt4) m_tuple4 = nt4;
				else {
					m_tuple4 = ntupleSvc()->book("FILE1/tofe", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(m_tuple4) {
						m_tuple4->addItem("ptrk", m_ptot_etof); ///<tr><td>`"ptrk"`</td><td>Momentum of the track as reconstructed by MDC</td></tr>
						m_tuple4->addItem("path", m_path_etof); ///<tr><td>`"path"`</td><td>Path length</td></tr>
						m_tuple4->addItem("tof",  m_tof_etof);  ///<tr><td>`"tof"` </td><td>Time of flight</td></tr>
						m_tuple4->addItem("cntr", m_cntr_etof); ///<tr><td>`"cntr"`</td><td>ToF counter ID</td></tr>
						m_tuple4->addItem("ph",   m_ph_etof);   ///<tr><td>`"ph"`  </td><td>ToF pulse height</td></tr>
						m_tuple4->addItem("rhit", m_rhit_etof); ///<tr><td>`"rhit"`</td><td>Track extrapolate Z or R Hit position</td></tr>
						m_tuple4->addItem("qual", m_qual_etof); ///<tr><td>`"qual"`</td><td>Data quality of reconstruction</td></tr>
						m_tuple4->addItem("te",   m_te_etof);   ///<tr><td>`"te"`  </td><td>Difference with ToF in electron hypothesis</td></tr>
						m_tuple4->addItem("tmu",  m_tmu_etof);  ///<tr><td>`"tmu"` </td><td>Difference with ToF in muon hypothesis</td></tr>
						m_tuple4->addItem("tpi",  m_tpi_etof);  ///<tr><td>`"tpi"` </td><td>Difference with ToF in charged pion hypothesis</td></tr>
						m_tuple4->addItem("tk",   m_tk_etof);   ///<tr><td>`"tk"`  </td><td>Difference with ToF in charged kaon hypothesis</td></tr>
						m_tuple4->addItem("tp",   m_tp_etof);   ///<tr><td>`"tp"`  </td><td>Difference with ToF in proton hypothesis</td></tr>
					} else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple4) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			} // check Tof: endcap

		/// <tr><td colspan="2"><b>`NTuple "tof1"`:   Barrel inner ToF</b></td></tr>
			if(m_checkTof) {
				NTuplePtr nt5(ntupleSvc(), "FILE1/tof1");
				if(nt5) m_tuple5 = nt5;
				else {
					m_tuple5 = ntupleSvc()->book("FILE1/tof1", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(m_tuple5) {
						m_tuple5->addItem("ptrk", m_ptot_btof1); ///<tr><td>`"ptrk"`</td><td>Momentum of the track as reconstructed by MDC</td></tr>
						m_tuple5->addItem("path", m_path_btof1); ///<tr><td>`"path"`</td><td>Path length</td></tr>
						m_tuple5->addItem("tof",  m_tof_btof1);  ///<tr><td>`"tof"` </td><td>Time of flight</td></tr>
						m_tuple5->addItem("cntr", m_cntr_btof1); ///<tr><td>`"cntr"`</td><td>ToF counter ID</td></tr>
						m_tuple5->addItem("ph",   m_ph_btof1);   ///<tr><td>`"ph"`  </td><td>ToF pulse height</td></tr>
						m_tuple5->addItem("zhit", m_zhit_btof1); ///<tr><td>`"zhit"`</td><td>Track extrapolate Z or R Hit position</td></tr>
						m_tuple5->addItem("qual", m_qual_btof1); ///<tr><td>`"qual"`</td><td>Data quality of reconstruction</td></tr>
						m_tuple5->addItem("te",   m_te_btof1);   ///<tr><td>`"te"`  </td><td>Difference with ToF in electron hypothesis</td></tr>
						m_tuple5->addItem("tmu",  m_tmu_btof1);  ///<tr><td>`"tmu"` </td><td>Difference with ToF in muon hypothesis</td></tr>
						m_tuple5->addItem("tpi",  m_tpi_btof1);  ///<tr><td>`"tpi"` </td><td>Difference with ToF in charged pion hypothesis</td></tr>
						m_tuple5->addItem("tk",   m_tk_btof1);   ///<tr><td>`"tk"`  </td><td>Difference with ToF in charged kaon hypothesis</td></tr>
						m_tuple5->addItem("tp",   m_tp_btof1);   ///<tr><td>`"tp"`  </td><td>Difference with ToF in proton hypothesis</td></tr>
					} else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple5) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			} // check Tof: barrel inner Tof

		/// <tr><td colspan="2"><b>`NTuple "tof2"`:   Barrel outer ToF</b></td></tr>
			if(m_checkTof) {
				NTuplePtr nt6(ntupleSvc(), "FILE1/tof2");
				if(nt6) m_tuple6 = nt6;
				else {
					m_tuple6 = ntupleSvc()->book("FILE1/tof2", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(m_tuple6) {
						m_tuple6->addItem("ptrk", m_ptot_btof2); ///<tr><td>`"ptrk"`</td><td>Momentum of the track as reconstructed by MDC</td></tr>
						m_tuple6->addItem("path", m_path_btof2); ///<tr><td>`"path"`</td><td>Path length</td></tr>
						m_tuple6->addItem("tof",  m_tof_btof2);  ///<tr><td>`"tof"` </td><td>Time of flight</td></tr>
						m_tuple6->addItem("cntr", m_cntr_btof2); ///<tr><td>`"cntr"`</td><td>ToF counter ID</td></tr>
						m_tuple6->addItem("ph",   m_ph_btof2);   ///<tr><td>`"ph"`  </td><td>ToF pulse height</td></tr>
						m_tuple6->addItem("zhit", m_zhit_btof2); ///<tr><td>`"zhit"`</td><td>Track extrapolate Z or R Hit position</td></tr>
						m_tuple6->addItem("qual", m_qual_btof2); ///<tr><td>`"qual"`</td><td>Data quality of reconstruction</td></tr>
						m_tuple6->addItem("te",   m_te_btof2);   ///<tr><td>`"te"`  </td><td>Difference with ToF in electron hypothesis</td></tr>
						m_tuple6->addItem("tmu",  m_tmu_btof2);  ///<tr><td>`"tmu"` </td><td>Difference with ToF in muon hypothesis</td></tr>
						m_tuple6->addItem("tpi",  m_tpi_btof2);  ///<tr><td>`"tpi"` </td><td>Difference with ToF in charged pion hypothesis</td></tr>
						m_tuple6->addItem("tk",   m_tk_btof2);   ///<tr><td>`"tk"`  </td><td>Difference with ToF in charged kaon hypothesis</td></tr>
						m_tuple6->addItem("tp",   m_tp_btof2);   ///<tr><td>`"tp"`  </td><td>Difference with ToF in proton hypothesis</td></tr>
					} else {
			log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple6) << endmsg;
			return StatusCode::FAILURE;
					}
				}
			} // check Tof: barrel outter Tof

		/// <tr><td colspan="2"><b>`NTuple "infmom"`: Information with the method of momentum selection</b> @attention This is the important part of the `PipiJpsi` algorithm!</td></tr>
			NTuplePtr nt8(ntupleSvc(), "FILE1/infmom");
			if(nt8) m_tuple8 = nt8;
			else {
				m_tuple8 = ntupleSvc()->book("FILE1/infmom", CLID_ColumnWiseTuple, "information with momentum method");
				if(m_tuple8) {
					// * Momenta * //
						m_tuple8->addItem("momlepmm", m_mom_lepm);  ///<tr><td>`"momlepmm"`</td><td>4-momentum of the identified negative lepton</td></tr>
						m_tuple8->addItem("momlepp",  m_mom_lepp);  ///<tr><td>`"momlepp"` </td><td>4-momentum of the positive pion</td></tr>
						m_tuple8->addItem("mompionm", m_mom_pionm); ///<tr><td>`"mompionm"`</td><td>4-momentum of the identified negative lepton</td></tr>
						m_tuple8->addItem("mompionp", m_mom_pionp); ///<tr><td>`"mompionp"`</td><td>4-momentum of the positive pion</td></tr>
					// * Geometry * //
						m_tuple8->addItem("pipidang", m_pipi_dang); ///<tr><td>`"pipidang"`</td><td>Angle between the two pions</td></tr>
						m_tuple8->addItem("cmslepp",  m_cms_lepp);  ///<tr><td>`"cmslepp"` </td><td>Boosted \f$|\vec{p}|\f$ of the positive lepton</td></tr>
						m_tuple8->addItem("cmslepm",  m_cms_lepm);  ///<tr><td>`"cmslepm"` </td><td>Boosted \f$|\vec{p}|\f$ of the negative lepton</td></tr>
						m_tuple8->addIndexedItem("costhe",  m_index, m_cos_theta); ///<tr><td>`"costhe"` </td><td>\f$\cos(\theta)\f$ of the lorentz vector in the cylindrical coordinate system</td></tr>
						m_tuple8->addIndexedItem("phi",     m_index, m_phi);       ///<tr><td>`"phi"`    </td><td>\f$\phi\f$ angle of the lorentz vector in the cylindrical coordinate system</td></tr>
						m_tuple8->addIndexedItem("fourmom", m_index, 4, m_four_mom); ///<tr><td>`"fourmom"`</td><td>\f$E\f$, \f$p_x\f$, \f$p_y\f$, and \f$p_z\f$ for each lepton/pion (hence a \f$4\times4\f$ matrix)</td></tr>
					// * Invariant masses * //
						m_tuple8->addItem("invtwopi", m_mass_twopi);  ///<tr><td>`"invtwopi"`</td><td>Invariant mass of the two pions.</td></tr>
						m_tuple8->addItem("invjpsi",  m_mass_jpsi);   ///<tr><td>`"invjpsi"` </td><td>Invariant mass of the \f$J/\psi\f$ (sum of the two leptons)</td></tr>
						m_tuple8->addItem("recoil",   m_mass_recoil); ///<tr><td>`"recoil"`  </td><td>Invariant mass of the lab 4-momentum minus the 4-momenta of the two pions</td></tr>
						m_tuple8->addItem("invmass",  m_inv_mass);    ///<tr><td>`"invmass"` </td><td>Invariant mass of the total system (\f$J/\psi\f$ plus the two pions)</td></tr>
					// * Energies and momenta * //
						m_tuple8->addItem("totene", m_tot_e);  ///<tr><td>`"totene"`</td><td>Total energy of the \f$J/\psi \pi\pi\f$ system</td></tr>
						m_tuple8->addItem("totpx",  m_tot_px); ///<tr><td>`"totpx"` </td><td>Total \f$p_x\f$ of the \f$J/\psi \pi\pi\f$ system</td></tr>
						m_tuple8->addItem("totpy",  m_tot_py); ///<tr><td>`"totpy"` </td><td>Total \f$p_y\f$ of the \f$J/\psi \pi\pi\f$ system</td></tr>
						m_tuple8->addItem("totpz",  m_tot_pz); ///<tr><td>`"totpz"` </td><td>Total \f$p_z\f$ of the \f$J/\psi \pi\pi\f$ system</td></tr>
					// * Ratios * //
						m_tuple8->addItem("epratio",      m_ep_ratio);          ///<tr><td>`"epratio"`     </td><td>Total energy of the EMC showers</td></tr>
						m_tuple8->addItem("eveflag",      m_event_flag);        ///<tr><td>`"eveflag"`     </td><td>Characterisation of the event: 3 or 4 tracks, 4=>4 tracks, 0=> miss pi+, 1=> miss pi-, 2=> miss lepton+, 3=> miss lepton-</td></tr>
						m_tuple8->addItem("tplepratiom",  m_trans_ratio_lepm);  ///<tr><td>`"tplepratiom"` </td><td>Ratio of the 4-momentum axial distance (\f$\rho\f$ in the cylindrical coordinate system) versus \f$|\vec{p}|\f$ for the negative lepton.</td></tr>
						m_tuple8->addItem("tplepratiop",  m_trans_ratio_lepp);  ///<tr><td>`"tplepratiop" `</td><td>Ratio of the 4-momentum axial distance (\f$\rho\f$ in the cylindrical coordinate system) versus \f$|\vec{p}|\f$ for the positive lepton.</td></tr>
						m_tuple8->addItem("tppionratiom", m_trans_ratio_pionm); ///<tr><td>`"tppionratiom"`</td><td>Ratio of the 4-momentum axial distance (\f$\rho\f$ in the cylindrical coordinate system) versus \f$|\vec{p}|\f$ for the negative pion.</td></tr>
						m_tuple8->addItem("tppionratiop", m_trans_ratio_pionp); ///<tr><td>`"tppionratiop"`</td><td>Ratio of the 4-momentum axial distance (\f$\rho\f$ in the cylindrical coordinate system) versus \f$|\vec{p}|\f$ for the positive pion.</td></tr>
					// * Indices * //
						m_tuple8->addItem("run",    m_run);         ///<tr><td>`"run"`   </td><td>Run number</td></tr>
						m_tuple8->addItem("event",  m_event);       ///<tr><td>`"event"` </td><td>Event number</td></tr>
						m_tuple8->addItem("ntrack", m_index, 0, 4); ///<tr><td>`"ntrack"`</td><td>For indexed item. Ranges from `0` to `3`: for \f$\pi^+\f$, \f$\pi^-\f$, \f$l^+\f$, and \f$l^-\f$</td></tr>
					// * Matches * //
						m_tuple8->addItem("pionmat", m_pion_matched); ///<tr><td>`"pionmat"`</td><td>Number of identified pions</td></tr>
						m_tuple8->addItem("lepmat",  m_lep_matched);  ///<tr><td>`"lepmat"` </td><td>Number of identified leptons (actually non-pions)</td></tr>
					// * Monte Carlo truth * //
						m_tuple8->addItem("indexmc", m_idxmc, 0, 100); ///<tr><td>`"indexmc"`</td><td>Total number of (useful) MC particles in this event (used for the arrays)</td></tr>
						m_tuple8->addIndexedItem("pdgid",     m_idxmc, m_pdgid);     ///<tr><td>`"pdgid"`</td><td>PDG code of the MC truth particle</td></tr>
						m_tuple8->addIndexedItem("motheridx", m_idxmc, m_motheridx); ///<tr><td>`"motheridx"`</td><td>PDG code of the mother of the MC particle</td></tr>
						m_tuple8->addItem("truepp", m_true_pionp); ///<tr><td>`"truepp"`</td><td>True \f$|\vec{p}|\f$ of the positive pion</td></tr>
						m_tuple8->addItem("truepm", m_true_pionm); ///<tr><td>`"truepm"`</td><td>True \f$|\vec{p}|\f$ of the negative pion</td></tr>
				} else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple8) << endmsg;
					return StatusCode::FAILURE;
				}
			}

		/// </table>
		log << MSG::INFO << "Successfully return from initialize()" << endmsg;
		return StatusCode::SUCCESS;

	}



// * ========================= * //
// * -------- EXECUTE -------- * //
// * ========================= * //
	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode PipiJpsi::execute()
	{
		/// <ol>
		/// <li> Log stream and status code
			MsgStream log(msgSvc(), name());
			log << MSG::INFO << "in execute()" << endmsg;
			StatusCode sc = StatusCode::SUCCESS;

		/// <li> Event counter and set new file
			// * Save the events passed selection to a new file *
			++m_cout_all;
			setFilterPassed(false);

			// * Load DST file info *
			SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
			if(!eventHeader) {
				log << MSG::ERROR << "EventHeader not found" << endmsg;
				return StatusCode::SUCCESS;
			}
			int run(eventHeader->runNumber());
			int event(eventHeader->eventNumber());
			if(event%1000==0) cout << "run: " << run << " event: " << event << endl;

			// * Load event information and track collection and test *
				/*
				http://bes3.to.infn.it/Boss/7.0.2/html/namespaceEventModel_1_1EvtRec.html (namespace)
				http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecEvent.html (class)
				http://bes3.to.infn.it/Boss/7.0.2/html/EvtRecTrack_8h.html (typedef EvtRecTrackCol)
				*/
			SmartDataPtr<EvtRecEvent>    evtRecEvent (eventSvc(), EventModel::EvtRec::EvtRecEvent);
			SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol);
			if(!evtRecEvent) {
				log << MSG::ERROR << "EvtRecEvent not found" << endmsg;
				return StatusCode::SUCCESS;
			}
			if(!evtRecTrkCol) {
				log << MSG::ERROR << "EvtRecTrackCol" << endmsg;
				return StatusCode::SUCCESS;
			}

			// * Log number of events *
			log << MSG::DEBUG << "ncharg, nneu, tottks = "
				<< evtRecEvent->totalCharged() << " , "
				<< evtRecEvent->totalNeutral() << " , "
				<< evtRecEvent->totalTracks() << endmsg;


		/// <li> Print trigger information once
			if(m_trigger_flag) {
				SmartDataPtr<TrigData> trigData(eventSvc(),EventModel::Trig::TrigData);
				if(!trigData) {
					log << MSG::FATAL << "Could not find Trigger Data for physics analysis" << endmsg;
					return StatusCode::FAILURE;
				}

				log << MSG::DEBUG << "Trigger conditions: " << endmsg;
				for(int i=0; i < 48; ++i) {
					log << MSG::DEBUG << "i:" << i << "  name:" << trigData->getTrigCondName(i) << "  cond:" << trigData->getTrigCondition(i) << endmsg;
				}

				// * Test event rate *
				int m_trig_tot(0);
				int m_trig_which(-1);
				if(m_eventrate) {
					for(int j=0; j<16; j++) {
						if(trigData->getTrigChannel(j)) {
							++m_trig_tot;
							m_trig_which = j+1;
						}
					}
					if(m_trig_tot==1 && m_trig_which==m_chan_det) m_cout_everat++;
					return sc;
				}
			}

			++m_cout_col;
			if(
				evtRecEvent->totalCharged()<3 ||
				evtRecTrkCol->size()<3 ||
				evtRecEvent->totalTracks()>99 ||
				evtRecTrkCol->size()>99) return StatusCode::SUCCESS;
			++m_cout_charge;

		/// <li> Asign four-momentum with KalmanTrack
			/// <ul>
			/// <li> Declare vectors etc.
				Vint iGood;
				// Number of different particles: \f$\pi^+\f$, \f$\pi^-\f$, \f$l^+\f$, \f$l^-\f$
				int m_num[4] = {0, 0, 0, 0};
				int nCharge = 0;
				m_pion_matched = 0;
				m_lep_matched = 0;
				HepLorentzVector m_lv_pionp;
				HepLorentzVector m_lv_pionm;
				HepLorentzVector m_lv_lepp;
				HepLorentzVector m_lv_lepm;
				HepLorentzVector m_lv_ele;
				HepLorentzVector m_lv_pos;
				HepLorentzVector m_lv_mum;
				HepLorentzVector m_lv_mup;

			/// <li> Create selection of 'good' charged tracks
				/// <ul>
				for(int i = 0; i < evtRecEvent->totalCharged(); ++i) {
					/// <li> Get MDC Kalman track for this charged track
						EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + i;
						if(!(*itTrk)->isMdcKalTrackValid()) continue;
						RecMdcKalTrack* mdcTrk = (*itTrk)->mdcKalTrack();

					/// <li> Get \f$xyz\f$, \f$r\f$ geometry
						m_vx0 = mdcTrk->x();
						m_vy0 = mdcTrk->y();
						m_vz0 = mdcTrk->z();
						m_vr0 = mdcTrk->r();

					/// <li> Apply vertex cuts
						if(fabs(m_vz0) >= m_vz0cut) continue;
						if(m_vr0 >= m_vr0cut) continue;

					/// <li> <b>Store this track as 'good'</b>
						iGood.push_back(i);
						nCharge += mdcTrk->charge();

					/// <li> Count if either pion or lepton. The track is considered to be pion if the <i>probability to be an electron</i> is less than 1.
						if((*itTrk)->isEmcShowerValid()) {
							if(mdcTrk->p()<1.) ++m_pion_matched;
							else               ++m_lep_matched;
						}

					/// <li> Store 4-momenta of the four different cases (pion or lepton and negative or positive)
						if(mdcTrk->charge() > 0) {
							if(mdcTrk->p() < 1.0) { // pi+
								mdcTrk->setPidType(RecMdcKalTrack::pion);
								// @warning If you do not modify the `DstMdcKalTrack` package, the `RecMdcKalTrack::p4()` function cannot be used. You should get the momentum from `MdcKalTrack`, then calculate the energy by yourself.
								m_lv_pionp = mdcTrk->p4(xmass[2]);
								++m_num[0];
							} else { // l+
								mdcTrk->setPidType(RecMdcKalTrack::electron);
								m_lv_pos = mdcTrk->p4(xmass[0]);
								mdcTrk->setPidType(RecMdcKalTrack::muon);
								m_lv_mup = mdcTrk->p4(xmass[1]);
								++m_num[2];
							}
						} else {
							if(mdcTrk->p() < 1.0) { // pi-
								mdcTrk->setPidType(RecMdcKalTrack::pion);
								m_lv_pionm = mdcTrk->p4(xmass[2]);
								++m_num[1];
							} else { // l-
								mdcTrk->setPidType(RecMdcKalTrack::electron);
								m_lv_ele = mdcTrk->p4(xmass[0]);
								mdcTrk->setPidType(RecMdcKalTrack::muon);
								m_lv_mum = mdcTrk->p4(xmass[1]);
								++m_num[3];
							}
						}
				}
				/// </ul>

			/// <li> Test selections (apply cuts)
				/// <ul>
				/// <li> Apply photon cut: only 3 or 4 photons
					int nGood = iGood.size();
					log << MSG::DEBUG << "With KalmanTrack, ngood, totcharge = " << nGood << " , " << nCharge << endmsg;
					if(nGood<3 || nGood>4) return sc;
					++m_cout_nGood;

				/// <li> Compute total energy
					m_ep_ratio = 0;
					for(int i=0; i< evtRecEvent->totalTracks(); ++i) {
						EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;
						if(!(*itTrk)->isEmcShowerValid()) continue;
						RecEmcShower *emcTrk = (*itTrk)->emcShower();
						m_ep_ratio += emcTrk->energy();
					}

				/// <li> Store vectors in either muon or electron case
					if(m_ep_ratio > m_distin_emuon) { // if electron
						m_lv_lepp = m_lv_pos;
						m_lv_lepm = m_lv_ele;
					} else { // if muon
						m_lv_lepp = m_lv_mup;
						m_lv_lepm = m_lv_mum;
					}

				/// <li> Apply \f$\cos(\theta)\f$ cuts
					HepLorentzVector m_lv_lab(0.04, 0, 0, 3.686);
					if(nGood==4) {
						if(nCharge) return sc;
						m_event_flag = 4;
					} else {
						if(m_num[0]>1 || m_num[1]>1 || m_num[2]>1 || m_num[3]>1) return sc;
						if(m_num[0]==0) {
							if(nCharge != -1) return StatusCode::SUCCESS;
							m_lv_pionp = m_lv_lab - m_lv_pionm - m_lv_lepp - m_lv_lepm;
							if(m_lv_pionp.vect().cosTheta()>m_cosThetaCut) return StatusCode::SUCCESS;
							m_event_flag = 0;
						}
						if(m_num[1]==0) {
							if(nCharge != 1) return StatusCode::SUCCESS;
							m_lv_pionm = m_lv_lab - m_lv_pionp - m_lv_lepp - m_lv_lepm;
							if(m_lv_pionm.vect().cosTheta()>m_cosThetaCut) return StatusCode::SUCCESS;
							m_event_flag = 1;
						}
						if(m_num[2]==0) {
							if(nCharge != -1) return StatusCode::SUCCESS;
							m_lv_lepp = m_lv_lab - m_lv_pionp - m_lv_pionm - m_lv_lepm;
							if(m_lv_lepp.vect().cosTheta()>m_cosThetaCut) return StatusCode::SUCCESS;
							m_event_flag = 2;
						}
						if(m_num[3]==0) {
							if(nCharge != 1) return StatusCode::SUCCESS;
							m_lv_lepm = m_lv_lab - m_lv_pionp - m_lv_pionm - m_lv_lepp;
							if(m_lv_lepm.vect().cosTheta()>m_cosThetaCut) return StatusCode::SUCCESS;
							m_event_flag = 3;
						}
					}
					++m_cout_mom;
				/// </ul>
			/// </ul>

		/// <li> With momentum method calculate the invariant mass of Jpsi actually we use the recoil mass
			HepLorentzVector m_lv_recoil;
			HepLorentzVector m_lv_jpsi;
			m_lv_recoil = m_lv_lab - m_lv_pionp - m_lv_pionm;
			m_lv_jpsi = m_lv_lepp + m_lv_lepm;

			m_mass_twopi = (m_lv_pionp + m_lv_pionm).m();
			m_mass_recoil = m_lv_recoil.m();
			m_mass_jpsi = m_lv_jpsi.m();

		/// <li> \f$J/\psi\f$ mass cut
			if( m_mass_recoil < 3.05 || m_mass_recoil > 3.15) return sc;
			if( m_mass_jpsi < 3.0 || m_mass_jpsi > 3.2) return sc;
			++m_cout_recoil;

			HepLorentzVector m_ttm(m_lv_jpsi + m_lv_pionp + m_lv_pionm);
			if(m_ttm.m()>4 || m_ttm.m()<3) return sc;

		/// <li> Angle difference between pions, suppress gamma convertion. @attention This is an important part of the `PipiJpsi` algorithm!
			/// <ul>
			/// <li> Compute angle between pions
				m_pipi_dang = m_lv_pionp.vect().cosTheta(m_lv_pionm.vect());

			/// <li> Magnitude of the 3-momenta
				m_mom_pionp = m_lv_pionp.vect().mag();
				m_mom_pionm = m_lv_pionm.vect().mag();
				m_mom_lepp  = m_lv_lepp .vect().mag();
				m_mom_lepm  = m_lv_lepm .vect().mag();

			/// <li> Compute ratios
				m_trans_ratio_lepp  = m_lv_lepp .vect().perp() / m_lv_lepp.vect().mag();
				m_trans_ratio_lepm  = m_lv_lepm .vect().perp() / m_lv_lepm.vect().mag();
				m_trans_ratio_pionp = m_lv_pionp.vect().perp() / m_lv_pionp.vect().mag();
				m_trans_ratio_pionm = m_lv_pionm.vect().perp() / m_lv_pionm.vect().mag();

			/// <li> Apply boost
				Hep3Vector m_boost_jpsi(m_lv_recoil.boostVector());
				HepLorentzVector m_lv_cms_lepp(boostOf(m_lv_lepp, -m_boost_jpsi));
				HepLorentzVector m_lv_cms_lepm(boostOf(m_lv_lepm, -m_boost_jpsi));
				m_cms_lepm = m_lv_cms_lepm.vect().mag();
				m_cms_lepp = m_lv_cms_lepp.vect().mag();
				log << MSG::DEBUG << "jpsi four momentum in cms " << m_lv_cms_lepp + m_lv_cms_lepm << endmsg;

			/// <li> Compute total masses, energies, and momenta
				m_inv_mass = m_ttm.m();
				m_tot_e    = m_ttm.e();
				m_tot_px   = m_ttm.px();
				m_tot_py   = m_ttm.py();
				m_tot_pz   = m_ttm.pz();

			/// <li> Store indices
				m_run = run;
				m_event = event;

			/// <li> Store momenta in matrix when one track is assumed missing
				HepLorentzVector m_lv_book(0,0,0,0); // assume one track is missing
				for(m_index=0; m_index<4; m_index++) {
					switch(m_index) {
						case 0: m_lv_book = m_lv_pionp; break;
						case 1: m_lv_book = m_lv_pionm; break;
						case 2: m_lv_book = m_lv_lepp;  break;
						case 3: m_lv_book = m_lv_lepm;  break;
						default: m_lv_book.setE(2008);
					}
					m_cos_theta[m_index] = m_lv_book.vect().cosTheta();
					m_phi[m_index] = m_lv_book.vect().phi();
					m_four_mom[m_index][0] = m_lv_book.e();
					m_four_mom[m_index][1] = m_lv_book.px();
					m_four_mom[m_index][2] = m_lv_book.py();
					m_four_mom[m_index][3] = m_lv_book.pz();
				}

			/// <li> Set filter passed
				if(m_subsample_flag) setFilterPassed(true);
				else if(
					m_mass_recoil > 3.08 &&
					m_mass_recoil < 3.12 &&
					m_mass_jpsi   > 3.0  &&
					m_mass_jpsi   < 3.2  &&
					m_cms_lepp    < 1.7  &&
					m_cms_lepp    > 1.4  &&
					m_cms_lepm    < 1.7  &&
					m_cms_lepm    > 1.4  &&
					m_event_flag == 4 &&
					m_pipi_dang < m_pipi_dang_cut) setFilterPassed(true);

			/// </ul>

		/// <li> Monte Carlo information. @attention This is an important part of the `PipiJpsi` algorithm!
			SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");
			if(m_run<0) {
				int m_numParticle(0), m_true_pid(0);
				if(!mcParticleCol) {
					log << MSG::ERROR << "Could not retrieve McParticelCol" << endmsg;
					return StatusCode::FAILURE;
				} else {
					bool psipDecay(false);
					int rootIndex(-1);
					Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
					for (; iter_mc != mcParticleCol->end(); iter_mc++) {
						if((*iter_mc)->primaryParticle()) continue;
						if(!(*iter_mc)->decayFromGenerator()) continue;
						// if(((*iter_mc)->mother()).trackIndex()<3) continue;
						if((*iter_mc)->particleProperty()==100443) {
							psipDecay = true;
							rootIndex = (*iter_mc)->trackIndex();
						}
						if(!psipDecay) continue;
						int mcidx = ((*iter_mc)->mother()).trackIndex() - rootIndex;
						int pdgid = (*iter_mc)->particleProperty();
						m_pdgid[m_numParticle] = pdgid;
						m_motheridx[m_numParticle] = mcidx;
						++m_numParticle;

						// if(!(*iter_mc)->leafParticle()) continue;
						if((*iter_mc)->particleProperty() == 211)
							m_true_pionp = (*iter_mc)->initialFourMomentum().vect().mag();
						if((*iter_mc)->particleProperty() == -211)
							m_true_pionm = (*iter_mc)->initialFourMomentum().vect().mag();
					}
					m_idxmc = m_numParticle;
				}
			}

			m_tuple1->write();
			m_tuple8->write();

		/// <li> Selection of 'good' photons. Note that these aren't really used, as the process \f$\psi' \rightarrow \pi\pi J/\psi \rightarrow \pi\pi ll\f$ doesn't involve photons.
			Vint iGam;  iGam.clear();
			for(int i = evtRecEvent->totalCharged(); i< evtRecEvent->totalTracks(); ++i) {
				EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;
				if(!(*itTrk)->isEmcShowerValid()) continue;
				RecEmcShower *emcTrk = (*itTrk)->emcShower();
				Hep3Vector emcpos(emcTrk->x(), emcTrk->y(), emcTrk->z()); // find the nearest charged track
				double dthe = 200.;
				double dphi = 200.;
				double dang = 200.;
				for(int j = 0; j < evtRecEvent->totalCharged(); j++) {
					EvtRecTrackIterator jtTrk = evtRecTrkCol->begin() + j;
					if(!(*jtTrk)->isExtTrackValid()) continue;
					RecExtTrack *extTrk = (*jtTrk)->extTrack();
					if(extTrk->emcVolumeNumber() == -1) continue;
					Hep3Vector extpos = extTrk->emcPosition();
					    //  double ctht = extpos.cosTheta(emcpos);
					double angd = extpos.angle(emcpos);
					double thed = extpos.theta() - emcpos.theta();
					double phid = extpos.deltaPhi(emcpos);
					thed = fmod(thed+CLHEP::twopi+CLHEP::twopi+pi, CLHEP::twopi) - CLHEP::pi;
					phid = fmod(phid+CLHEP::twopi+CLHEP::twopi+pi, CLHEP::twopi) - CLHEP::pi;

					if(fabs(thed) < fabs(dthe)) dthe = thed;
					if(fabs(phid) < fabs(dphi)) dphi = phid;
					if(angd < dang) dang = angd;
				}

				// * Photon angle cut * //
				if(dang>=200) continue;
				double eraw = emcTrk->energy();
				dthe = dthe * 180 / (CLHEP::pi);
				dphi = dphi * 180 / (CLHEP::pi);
				dang = dang * 180 / (CLHEP::pi);
				m_dthe = dthe;
				m_dphi = dphi;
				m_dang = dang;
				m_eraw = eraw;

				// * Good photon cut set here * //
				// if(eraw < m_energyThreshold) continue;
				// if((fabs(dthe) < m_gammaThetaCut) && (fabs(dphi)<m_gammaPhiCut)) continue;
				iGam.push_back((*itTrk)->trackId());

			}

		/// <li> Finish Good Photon Selection
			m_nGam = iGam.size();
			log << MSG::DEBUG << "num Good Photon " << m_nGam  << " , " << evtRecEvent->totalNeutral() << endmsg;
			m_tuple2->write();

		/// <li> Check dedx infomation
			if(m_checkDedx) {
				int m_dedx_cout(0);
				for(int i = 0; i < nGood; ++i) {
					EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + iGood[i];
					if(!(*itTrk)->isMdcDedxValid())continue;
					RecMdcKalTrack *mdcTrk = (*itTrk)->mdcKalTrack();
					RecMdcDedx *dedxTrk = (*itTrk)->mdcDedx();

					m_ptrk   = mdcTrk->p();
					m_chie   = dedxTrk->chiE();
					m_chimu  = dedxTrk->chiMu();
					m_chipi  = dedxTrk->chiPi();
					m_chik   = dedxTrk->chiK();
					m_chip   = dedxTrk->chiP();
					m_ghit   = dedxTrk->numGoodHits();
					m_thit   = dedxTrk->numTotalHits();
					m_probPH = dedxTrk->probPH();
					m_normPH = dedxTrk->normPH();

					m_tuple3->write();
				}
			}

		/// <li> Check TOF infomation
			if(m_checkTof) {
				int m_endcap_cout(0), m_layer1_cout(0), m_layer2_cout(0);
				for(int i = 0; i < nGood; ++i) {
					EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
					if(!(*itTrk)->isTofTrackValid()) continue;

					RecMdcKalTrack *mdcTrk = (*itTrk)->mdcKalTrack();
					SmartRefVector<RecTofTrack> tofTrkCol = (*itTrk)->tofTrack();

					double ptrk = mdcTrk->p();

					for( SmartRefVector<RecTofTrack>::iterator iter_tof = tofTrkCol.begin() ;iter_tof != tofTrkCol.end(); iter_tof++) {
						TofHitStatus *status = new TofHitStatus;
						status->setStatus((*iter_tof)->status());
						if(!(status->is_barrel())) { // endcap
							if( !(status->is_counter())) continue; // ?
							if( status->layer()!=0) continue; // layer1
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

							m_cntr_etof = cntr;
							m_ptot_etof = ptrk;
							m_path_etof = path;
							m_ph_etof   = ph;
							m_rhit_etof = rhit;
							m_qual_etof = qual;
							m_tof_etof  = tof;
							m_te_etof   = tof - texp[0];
							m_tmu_etof  = tof - texp[1];
							m_tpi_etof  = tof - texp[2];
							m_tk_etof   = tof - texp[3];
							m_tp_etof   = tof - texp[4];

							m_tuple4->write();
						} else { // barrel
							if( !(status->is_counter())) continue; // ?
							if(status->layer()==1) { // layer1
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

								m_cntr_btof1 = cntr;
								m_ptot_btof1 = ptrk;
								m_path_btof1 = path;
								m_ph_btof1   = ph;
								m_zhit_btof1 = rhit;
								m_qual_btof1 = qual;
								m_tof_btof1  = tof;
								m_te_btof1   = tof - texp[0];
								m_tmu_btof1  = tof - texp[1];
								m_tpi_btof1  = tof - texp[2];
								m_tk_btof1   = tof - texp[3];
								m_tp_btof1   = tof - texp[4];

								m_tuple5->write();
							}

							if(status->layer()==2) {//layer2
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

								m_cntr_btof2 = cntr;
								m_ptot_btof2 = ptrk;
								m_path_btof2 = path;
								m_ph_btof2   = ph;
								m_zhit_btof2 = rhit;
								m_qual_btof2 = qual;
								m_tof_btof2  = tof;
								m_te_btof2   = tof - texp[0];
								m_tmu_btof2  = tof - texp[1];
								m_tpi_btof2  = tof - texp[2];
								m_tk_btof2   = tof - texp[3];
								m_tp_btof2   = tof - texp[4];

								m_tuple6->write();
							}
						}

						delete status;
					}
				} // loop all charged track
			}  // check tof

		/// </ol>
		return StatusCode::SUCCESS;
	}



// * ========================== * //
// * -------- FINALIZE -------- * //
// * ========================== * //
	/**
	 * @brief Inherited `finalize` method of `Algorithm`. This function is only called once, after running over all events.
	 * @details Prints the flow chart to the terminal, so make sure you save this output!
	 */
	StatusCode PipiJpsi::finalize()
	{
		MsgStream log(msgSvc(), name());
		log << MSG::INFO << "in finalize()" << endmsg;

		// * Print flow chart * //
			if(m_eventrate) cout
				<< "all event: " << m_cout_all << endl
				<< "only channel " << m_chan_det << ": " << m_cout_everat << endl;

			cout << "all event: " << m_cout_all << endl
				<< "all collection point is OK: " << m_cout_col << endl
				<< "charged tracks >=3: " << m_cout_charge << endl
				<< "good charged tracks [3,4]: " << m_cout_nGood << endl
				<< "after momentum assign: " << m_cout_mom << endl
				<< "after recoild mass cut: " << m_cout_recoil << endl;

		return StatusCode::SUCCESS;
	}