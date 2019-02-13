#ifndef Physics_Analysis_PipiJpsi_H
#define Physics_Analysis_PipiJpsi_H 

/// @addtogroup BOSS_packages
/// @{

/**
 * @brief    psi'--> J/psi pion pion, J/psi --> di-leptons
 * @author   Kai Zhu (zhuk@ihep.ac.cn)
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/Algorithm.h"
	#include "GaudiKernel/NTuple.h"
	#include "trkInfo.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class PipiJpsi : public Algorithm {
public:
	// * Constructor and destructors *
	PipiJpsi(const std::string& name, ISvcLocator* pSvcLocator);

	// * Algorithm steps *
	StatusCode initialize();
	StatusCode execute();
	StatusCode finalize();  

private:

	// ! ------- DECLARE CUTS HERE ------- ! //
		// Here, you can define data members that you use to define cuts. The values for these cuts should be set in the `RhopiAlg::RhopiAlg` constructor (see `.cxx` file).

		// * Declare r0, z0 cut for charged tracks *
		double m_vr0cut;
		double m_vz0cut;

		// * Declare whether or not to check success of Particle Identification *
		bool m_checkDedx;
		bool m_checkTof;
		bool m_eventrate;
		int  m_chan_det;

		// * Declare the track angle cut and track's transverse momentum *
		double m_cosThetaCut;  

		// * Declare the cut angle between two pions to drop gamma conversion *
		double m_pipi_dang_cut;

		// * Declare whether pick up sub-sample *
		bool m_subsample_flag;
		bool m_trigger_flag;

		// * Declare energy/moentum to distinguish e and muon *
		double m_distin_emuon;

	// ! ------- DECLARE NTUPLES HERE ------- ! //
		// NTuples are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each NTuple::Item represents its leaves. The name of the leaves is determined when calling NTuple::addItem.
		// Note that the NTuple::Items are added to the NTuple during the RhopiAlg::initialize() step. This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Vertex information of the charged tracks *
		NTuple::Tuple*  m_tuple1;
			NTuple::Item<double>  m_vx0;
			NTuple::Item<double>  m_vy0;
			NTuple::Item<double>  m_vz0;
			NTuple::Item<double>  m_vr0;

		// * 'Fake' photon (angles) *
		NTuple::Tuple*  m_tuple2;
			NTuple::Item<double>  m_dthe;
			NTuple::Item<double>  m_dphi;
			NTuple::Item<double>  m_dang;
			NTuple::Item<double>  m_eraw;
			NTuple::Item<long>  m_nGam;

		// * Energy loss dE/dx info *
		NTuple::Tuple* m_tuple3;
			NTuple::Item<double> m_ptrk;
			NTuple::Item<double> m_chie;
			NTuple::Item<double> m_chimu;
			NTuple::Item<double> m_chipi;
			NTuple::Item<double> m_chik;
			NTuple::Item<double> m_chip;
			NTuple::Item<double> m_probPH;
			NTuple::Item<double> m_normPH;
			NTuple::Item<double> m_ghit;
			NTuple::Item<double> m_thit;

		// * Endcap ToF *
		NTuple::Tuple* m_tuple4;
			NTuple::Item<double> m_ptot_etof;
			NTuple::Item<double> m_cntr_etof;
			NTuple::Item<double> m_path_etof;
			NTuple::Item<double> m_tof_etof;
			NTuple::Item<double> m_te_etof;
			NTuple::Item<double> m_tmu_etof;
			NTuple::Item<double> m_tpi_etof;
			NTuple::Item<double> m_tk_etof;
			NTuple::Item<double> m_tp_etof;
			NTuple::Item<double> m_ph_etof;
			NTuple::Item<double> m_rhit_etof;
			NTuple::Item<double> m_qual_etof;

		// * Barrel inner ToF *
		NTuple::Tuple* m_tuple5;
			NTuple::Item<double> m_ptot_btof1;
			NTuple::Item<double> m_cntr_btof1;
			NTuple::Item<double> m_path_btof1;
			NTuple::Item<double> m_tof_btof1;
			NTuple::Item<double> m_te_btof1;
			NTuple::Item<double> m_tmu_btof1;
			NTuple::Item<double> m_tpi_btof1;
			NTuple::Item<double> m_tk_btof1;
			NTuple::Item<double> m_tp_btof1;
			NTuple::Item<double> m_ph_btof1;
			NTuple::Item<double> m_zhit_btof1;
			NTuple::Item<double> m_qual_btof1;

		// * Barrel outer ToF *
		NTuple::Tuple* m_tuple6;
			NTuple::Item<double> m_ptot_btof2;
			NTuple::Item<double> m_cntr_btof2;
			NTuple::Item<double> m_path_btof2;
			NTuple::Item<double> m_tof_btof2;
			NTuple::Item<double> m_te_btof2;
			NTuple::Item<double> m_tmu_btof2;
			NTuple::Item<double> m_tpi_btof2;
			NTuple::Item<double> m_tk_btof2;
			NTuple::Item<double> m_tp_btof2;
			NTuple::Item<double> m_ph_btof2;
			NTuple::Item<double> m_zhit_btof2;
			NTuple::Item<double> m_qual_btof2;
	
		// * With the method of momentum selection *
		NTuple::Tuple* m_tuple8; 
			NTuple::Item<double> m_mom_lepm;		    
			NTuple::Item<double> m_mom_lepp;	    
			NTuple::Item<double> m_mom_pionp;		    
			NTuple::Item<double> m_mom_pionm;		    
			NTuple::Item<double> m_pipi_dang;		    
			NTuple::Item<double> m_cms_lepp;		    
			NTuple::Item<double> m_cms_lepm;
			NTuple::Item<double> m_mass_twopi;       
			NTuple::Item<double> m_mass_jpsi;   
			NTuple::Item<double> m_mass_recoil;   
			NTuple::Item<double> m_inv_mass;   
			NTuple::Item<double> m_tot_e;   
			NTuple::Item<double> m_tot_px;   
			NTuple::Item<double> m_tot_py;   
			NTuple::Item<double> m_tot_pz;   
			NTuple::Item<double> m_ep_ratio;
			NTuple::Item<long>   m_event_flag;
				// 3 or 4 tracks, 4=>4 tracks, 0=> miss pi+, 1=> miss pi-, 2=> miss lepton+, 3=> miss lepton-
			NTuple::Item<double> m_trans_ratio_lepm;		    
			NTuple::Item<double> m_trans_ratio_lepp;	    
			NTuple::Item<double> m_trans_ratio_pionp;		    
			NTuple::Item<double> m_trans_ratio_pionm;	    
			NTuple::Item<long> m_run;
			NTuple::Item<long> m_event;
			NTuple::Item<long> m_index;
			NTuple::Array<double> m_cos_theta;
			NTuple::Array<double> m_phi;
			NTuple::Matrix<double> m_four_mom;
	
		// * We don't need smear, actually, assume one track of pi pi is missing, the recoil mass is actually the invariant of pair-lepton *
			NTuple::Item<long> m_pion_matched;
			NTuple::Item<long> m_lep_matched;
		
		// * Book MCtruth *
			NTuple::Item<long>   m_idxmc;
			NTuple::Array<long>  m_pdgid;
			NTuple::Array<long>  m_motheridx;
			NTuple::Item<double> m_true_pionp;
				// pions' momentum from MC truth   
			NTuple::Item<double> m_true_pionm;   

};


#endif 
