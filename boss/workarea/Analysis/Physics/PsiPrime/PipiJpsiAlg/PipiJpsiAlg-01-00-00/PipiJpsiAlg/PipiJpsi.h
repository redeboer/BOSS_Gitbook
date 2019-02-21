#ifndef Physics_Analysis_PipiJpsi_H
#define Physics_Analysis_PipiJpsi_H


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
/// @addtogroup BOSS_packages
/// @{


	/// Modified header of the stock BOSS package `PipiJpsi` (version `00-00-03`). Analyse \f$ \psi' \rightarrow J/\psi \pi \pi \f$ and \f$ J/\psi \rightarrow \mathrm{di\-leptons} \f$ events. This example package particularly teaches the use of Monte Carlo truth.
	/// @author   Kai Zhu (zhuk@ihep.ac.cn)
	/// @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date     February 13th, 2019
	class PipiJpsi : public Algorithm {
	public:
		/// @name Constructors
			///@{
			PipiJpsi(const std::string& name, ISvcLocator* pSvcLocator);
			///@}

		/// @name Gaudi Algorithm steps
			///@{
			StatusCode initialize();
			StatusCode execute();
			StatusCode finalize();
			///@}

	private:
	// ! ================================= ! //
	// ! ------- DECLARE CUTS HERE ------- ! //
	// ! ================================= ! //
		/// @name Cut parameters. Here, you can define data members that you use to define cuts. The values for these cuts should be set in the `PipiJpsi::PipiJpsi` constructor (see `.cxx` file)
			///@{
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

			// * Distinguish electron versus muon *
				double m_distin_emuon; ///< Value of \f$E/|\vec{p}|\f$ that is used to distinguish electron versus muon 

			///@}


	// ! ==================================== ! //
	// ! ------- DECLARE NTUPLES HERE ------- ! //
	// ! ==================================== ! //
		/// @name NTuples (eventual TTrees and their branches). NTuples are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each NTuple::Item represents its leaves. The name of the leaves is determined when calling NTuple::addItem. Note that the NTuple::Items are added to the NTuple during the PipiJpsi::initialize() step. This is also the place where you name these variables, so make sure that the structure here is reflected there!
			///@{
			NTuple::Tuple* m_tuple1; ///< Vertex information of the charged tracks
				NTuple::Item<double>  m_vx0; ///< Primary \f$x\f$-vertex as determined by MDC
				NTuple::Item<double>  m_vy0; ///< Primary \f$y\f$-vertex as determined by MDC
				NTuple::Item<double>  m_vz0; ///< Primary \f$z\f$-vertex as determined by MDC
				NTuple::Item<double>  m_vr0; ///< Distance from origin in \f$xy\f$-plane

			NTuple::Tuple* m_tuple2; ///< 'Fake' photon (angles)
				NTuple::Item<double> m_dthe; ///< \f$\theta\f$ angle difference with nearest charged track (degrees)
				NTuple::Item<double> m_dphi; ///< \f$\phi\f$ angle difference with nearest charged track (degrees)
				NTuple::Item<double> m_dang; ///< Angle difference with nearest charged track
				NTuple::Item<double> m_eraw; ///< Energy of the photon
				NTuple::Item<long>   m_nGam; ///< Number of photons

			NTuple::Tuple* m_tuple3; ///< Energy loss dE/dx info
				NTuple::Item<double> m_ptrk;   ///< Momentum of the track
				NTuple::Item<double> m_chie;   ///< \f$\chi^2\f$ in case of electron
				NTuple::Item<double> m_chimu;  ///< \f$\chi^2\f$ in case of muon
				NTuple::Item<double> m_chipi;  ///< \f$\chi^2\f$ in case of pion
				NTuple::Item<double> m_chik;   ///< \f$\chi^2\f$ in case of kaon
				NTuple::Item<double> m_chip;   ///< \f$\chi^2\f$ in case of proton
				NTuple::Item<double> m_probPH; ///< Most probable pulse height from truncated mean
				NTuple::Item<double> m_normPH; ///< Normalized pulse height
				NTuple::Item<double> m_ghit;   ///< Number of good hits
				NTuple::Item<double> m_thit;   ///< Total number of hits

			NTuple::Tuple* m_tuple4; ///< Endcap ToF
				NTuple::Item<double> m_ptot_etof;   ///< Momentum of the track as reconstructed by MDC
				NTuple::Item<double> m_path_etof;   ///< Path length
				NTuple::Item<double> m_tof_etof;    ///< Time of flight
				NTuple::Item<double> m_cntr_etof;   ///< ToF counter ID
				NTuple::Item<double> m_ph_etof;     ///< ToF pulse height
				NTuple::Item<double> m_rhit_etof;   ///< Track extrapolate Z or R Hit position
				NTuple::Item<double> m_qual_etof;   ///< Data quality of reconstruction
				NTuple::Item<double> m_te_etof;     ///< Difference with ToF in electron hypothesis
				NTuple::Item<double> m_tmu_etof;    ///< Difference with ToF in muon hypothesis
				NTuple::Item<double> m_tpi_etof;    ///< Difference with ToF in charged pion hypothesis
				NTuple::Item<double> m_tk_etof;     ///< Difference with ToF in charged kaon hypothesis
				NTuple::Item<double> m_tp_etof;     ///< Difference with ToF in proton hypothesis

			NTuple::Tuple* m_tuple5; ///< Barrel inner ToF
				NTuple::Item<double> m_ptot_btof1;  ///< Momentum of the track as reconstructed by MDC
				NTuple::Item<double> m_path_btof1;  ///< Path length
				NTuple::Item<double> m_tof_btof1;   ///< Time of flight
				NTuple::Item<double> m_cntr_btof1;  ///< ToF counter ID
				NTuple::Item<double> m_ph_btof1;    ///< ToF pulse height
				NTuple::Item<double> m_zhit_btof1;  ///< Track extrapolate Z or R Hit position
				NTuple::Item<double> m_qual_btof1;  ///< Data quality of reconstruction
				NTuple::Item<double> m_te_btof1;    ///< Difference with ToF in electron hypothesis
				NTuple::Item<double> m_tmu_btof1;   ///< Difference with ToF in muon hypothesis
				NTuple::Item<double> m_tpi_btof1;   ///< Difference with ToF in charged pion hypothesis
				NTuple::Item<double> m_tk_btof1;    ///< Difference with ToF in charged kaon hypothesis
				NTuple::Item<double> m_tp_btof1;    ///< Difference with ToF in proton hypothesis

			NTuple::Tuple* m_tuple6; ///< Barrel outer ToF
				NTuple::Item<double> m_ptot_btof2;  ///< Momentum of the track as reconstructed by MDC
				NTuple::Item<double> m_path_btof2;  ///< Path length
				NTuple::Item<double> m_tof_btof2;   ///< Time of flight
				NTuple::Item<double> m_cntr_btof2;  ///< ToF counter ID
				NTuple::Item<double> m_ph_btof2;    ///< ToF pulse height
				NTuple::Item<double> m_zhit_btof2;  ///< Track extrapolate Z or R Hit position
				NTuple::Item<double> m_qual_btof2;  ///< Data quality of reconstruction
				NTuple::Item<double> m_te_btof2;    ///< Difference with ToF in electron hypothesis
				NTuple::Item<double> m_tmu_btof2;   ///< Difference with ToF in muon hypothesis
				NTuple::Item<double> m_tpi_btof2;   ///< Difference with ToF in charged pion hypothesis
				NTuple::Item<double> m_tk_btof2;    ///< Difference with ToF in charged kaon hypothesis
				NTuple::Item<double> m_tp_btof2;    ///< Difference with ToF in proton hypothesis

			NTuple::Tuple* m_tuple8; ///< Information for method of momentum selection
				// * Momenta * //
					NTuple::Item<double> m_mom_lepm;  ///< 4-momentum of the identified negative lepton
					NTuple::Item<double> m_mom_lepp;  ///< 4-momentum of the positive pion
					NTuple::Item<double> m_mom_pionm; ///< 4-momentum of the identified negative lepton
					NTuple::Item<double> m_mom_pionp; ///< 4-momentum of the positive pion
				// * Geometry * //
					NTuple::Item<double>   m_pipi_dang; ///< Angle between the two pions
					NTuple::Item<double>   m_cms_lepp;  ///< Boosted \f$|\vec{p}|\f$ of the positive lepton
					NTuple::Item<double>   m_cms_lepm;  ///< Boosted \f$|\vec{p}|\f$ of the negative lepton
					NTuple::Array<double>  m_cos_theta; ///< \f$\cos(\theta)\f$ of the lorentz vector in the cylindrical coordinate system
					NTuple::Array<double>  m_phi;       ///< \f$\phi\f$ angle of the lorentz vector in the cylindrical coordinate system
					NTuple::Matrix<double> m_four_mom;  ///< \f$E\f$, \f$p_x\f$, \f$p_y\f$, and \f$p_z\f$ for each lepton/pion (hence a \f$4\times4\f$ matrix)
				// * Invariant masses * //
					NTuple::Item<double> m_mass_twopi;  ///< Invariant mass of the two pions.
					NTuple::Item<double> m_mass_jpsi;   ///< Invariant mass of the \f$J/\psi\f$ (sum of the two leptons)
					NTuple::Item<double> m_mass_recoil; ///< Invariant mass of the lab 4-momentum minus the 4-momenta of the two pions
					NTuple::Item<double> m_inv_mass;    ///< Invariant mass of the total system (\f$J/\psi\f$ plus the two pions)
				// * Energies and momenta * //
					NTuple::Item<double> m_tot_e;  ///< Total energy of the \f$J/\psi \pi\pi\f$ system
					NTuple::Item<double> m_tot_px; ///< Total \f$p_x\f$ of the \f$J/\psi \pi\pi\f$ system
					NTuple::Item<double> m_tot_py; ///< Total \f$p_y\f$ of the \f$J/\psi \pi\pi\f$ system
					NTuple::Item<double> m_tot_pz; ///< Total \f$p_z\f$ of the \f$J/\psi \pi\pi\f$ system
				// * Ratios * //
					NTuple::Item<double> m_ep_ratio;          ///< Total energy of the EMC showers
					NTuple::Item<long>   m_event_flag;        ///< Characterisation of the event: 3 or 4 tracks, 4=>4 tracks, 0=> miss pi+, 1=> miss pi-, 2=> miss lepton+, 3=> miss lepton-
					NTuple::Item<double> m_trans_ratio_lepm;  ///< Ratio of the 4-momentum axial distance (\f$\rho\f$ in the cylindrical coordinate system) versus \f$|\vec{p}|\f$ for the negative lepton.
					NTuple::Item<double> m_trans_ratio_lepp;  ///< Ratio of the 4-momentum axial distance (\f$\rho\f$ in the cylindrical coordinate system) versus \f$|\vec{p}|\f$ for the positive lepton.
					NTuple::Item<double> m_trans_ratio_pionm; ///< Ratio of the 4-momentum axial distance (\f$\rho\f$ in the cylindrical coordinate system) versus \f$|\vec{p}|\f$ for the negative pion.
					NTuple::Item<double> m_trans_ratio_pionp; ///< Ratio of the 4-momentum axial distance (\f$\rho\f$ in the cylindrical coordinate system) versus \f$|\vec{p}|\f$ for the positive pion.
				// * Indices * //
					NTuple::Item<long> m_run;   ///< Run number
					NTuple::Item<long> m_event; ///< Event number
					NTuple::Item<long> m_index; ///< For indexed item. Ranges from `0` to `3`: for \f$\pi^+\f$, \f$\pi^-\f$, \f$l^+\f$, and \f$l^-\f$
				// * Matches * //
					NTuple::Item<long> m_pion_matched; ///< Number of identified pions. We don't need smear, actually, assume one track of pi pi is missing, the recoil mass is actually the invariant of pair-lepton.
					NTuple::Item<long> m_lep_matched;  ///< Number of identified leptons (actually non-pions). We don't need smear, actually, assume one track of pi pi is missing, the recoil mass is actually the invariant of pair-lepton.
				// * Monte Carlo truth * //
					NTuple::Item<long>   m_idxmc;      ///< Total number of (useful) MC particles in this event (used for the arrays)
					NTuple::Array<long>  m_pdgid;      ///< PDG code of the MC truth particle
					NTuple::Array<long>  m_motheridx;  ///< PDG code of the mother of the MC particle
					NTuple::Item<double> m_true_pionp; ///< True \f$|\vec{p}|\f$ of the positive pion
					NTuple::Item<double> m_true_pionm; ///< True \f$|\vec{p}|\f$ of the negative pion

			///@}

	};


/// @}
#endif