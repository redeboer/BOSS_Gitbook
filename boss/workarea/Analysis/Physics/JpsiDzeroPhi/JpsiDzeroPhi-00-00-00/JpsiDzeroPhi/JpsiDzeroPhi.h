#ifndef Physics_Analysis_JpsiDzeroPhi_H
#define Physics_Analysis_JpsiDzeroPhi_H

/**
 * @brief    Analyse \f$ J/\psi \rightarrow D^0 \phi \f$ events.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 14th, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/Algorithm.h"
	#include "GaudiKernel/NTuple.h"
	// #include "VertexFit/ReadBeamParFromDb.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class JpsiDzeroPhi : public Algorithm
{
public:
	// * Constructor and destructors *
	JpsiDzeroPhi(const std::string& name, ISvcLocator* pSvcLocator);

	// * Algorithm steps *
	StatusCode initialize();
	StatusCode execute();
	StatusCode finalize();

private:

	// * Reader for beam info * //
	// ReadBeamParFromDb m_reader;

	// ! ------- DECLARE CUTS HERE ------- ! //
		// Here, you can define data members that you use to define cuts. The values for these cuts should be set in the `JpsiDzeroPhi::JpsiDzeroPhi` constructor (see `.cxx` file).

		// * Declare r0, z0 cut for charged tracks *
		double m_vr0cut;
		double m_vz0cut;
		double m_rvz0cut;
		double m_rvxy0cut;

		// * Declare energy, dphi, dthe cuts for fake gamma's *
		double m_energyThreshold;
		double m_gammaPhiCut;
		double m_gammaThetaCut;
		double m_gammaAngleCut;

		// * Declare invariant mass window cut *
		double m_dmrho0; // width of inv. mass window around mrho0

		// * Declare whether to test the success of the 4- and 5-constraint fits *
		bool m_test4C;
		bool m_test5C;
		double m_maxChiSq;

		// * Declare whether or not to check success of Particle Identification *
		bool m_checkDedx;
		bool m_checkTof;


	// ! ------- DECLARE NTUPLES HERE ------- ! //
		// NTuples are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each NTuple::Item represents its leaves. The name of the leaves is determined when calling NTuple::addItem.
		// Note that the NTuple::Items are added to the NTuple during the JpsiDzeroPhi::initialize() step. This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Vertex information of the charged tracks *
		NTuple::Tuple* m_tuple_vxyz;
			NTuple::Item<double> m_vx0;
			NTuple::Item<double> m_vy0;
			NTuple::Item<double> m_vz0;
			NTuple::Item<double> m_vr0;
			NTuple::Item<double> m_rvxy0;
			NTuple::Item<double> m_rvz0;
			NTuple::Item<double> m_rvphi0;

		// * 'Fake' photon (angles) *
		NTuple::Tuple* m_tuple_ang;
			NTuple::Item<double> m_dthe;
			NTuple::Item<double> m_dphi;
			NTuple::Item<double> m_dang;
			NTuple::Item<double> m_eraw;

		// * Raw invariant mass of the two gammas and their total energy *
		NTuple::Tuple* m_tuple_mgg;
			NTuple::Item<double> m_m2gg;
			NTuple::Item<double> m_etot;

		// * 4-constraint (4C) fit information *
		NTuple::Tuple* m_tuple_fit4c;
			NTuple::Item<double> m_chi1;
			NTuple::Item<double> m_mpi0;

		// * 5-constraint (5C) fit information *
		NTuple::Tuple* m_tuple_fit5c;
			NTuple::Item<double> m_chi2;
			NTuple::Item<double> m_mrho0;
			NTuple::Item<double> m_mrhop;
			NTuple::Item<double> m_mrhom;

		// * Photons *
		NTuple::Tuple* m_tuple_photon;
			NTuple::Item<double> m_fcos;
			NTuple::Item<double> m_elow;

		// * Energy loss dE/dx *
		NTuple::Tuple* m_tuple_dedx;
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

		// * End cap ToF decector *
		NTuple::Tuple* m_tuple_tof_ec;
			NTuple::Item<double> m_ptot_etof;
			NTuple::Item<double> m_path_etof;
			NTuple::Item<double> m_tof_etof;
			NTuple::Item<double> m_cntr_etof;
			NTuple::Item<double> m_te_etof;
			NTuple::Item<double> m_tmu_etof;
			NTuple::Item<double> m_tpi_etof;
			NTuple::Item<double> m_tk_etof;
			NTuple::Item<double> m_tp_etof;
			NTuple::Item<double> m_ph_etof;
			NTuple::Item<double> m_rhit_etof;
			NTuple::Item<double> m_qual_etof;

		// * Inner barrel ToF decector *
		NTuple::Tuple* m_tuple_tof_ib;
			NTuple::Item<double> m_ptot_btof1;
			NTuple::Item<double> m_path_btof1;
			NTuple::Item<double> m_tof_btof1;
			NTuple::Item<double> m_cntr_btof1;
			NTuple::Item<double> m_te_btof1;
			NTuple::Item<double> m_tmu_btof1;
			NTuple::Item<double> m_tpi_btof1;
			NTuple::Item<double> m_tk_btof1;
			NTuple::Item<double> m_tp_btof1;
			NTuple::Item<double> m_ph_btof1;
			NTuple::Item<double> m_zhit_btof1;
			NTuple::Item<double> m_qual_btof1;

		// * Outer barrel ToF decector *
		NTuple::Tuple* m_tuple_tof_ob;
			NTuple::Item<double> m_ptot_btof2;
			NTuple::Item<double> m_path_btof2;
			NTuple::Item<double> m_tof_btof2;
			NTuple::Item<double> m_cntr_btof2;
			NTuple::Item<double> m_te_btof2;
			NTuple::Item<double> m_tmu_btof2;
			NTuple::Item<double> m_tpi_btof2;
			NTuple::Item<double> m_tk_btof2;
			NTuple::Item<double> m_tp_btof2;
			NTuple::Item<double> m_ph_btof2;
			NTuple::Item<double> m_zhit_btof2;
			NTuple::Item<double> m_qual_btof2;

		// * Particle ID info *
		NTuple::Tuple* m_tuple_pid;
			NTuple::Item<double> m_ptrk_pid;
			NTuple::Item<double> m_cost_pid;
			NTuple::Item<double> m_dedx_pid;
			NTuple::Item<double> m_tof1_pid;
			NTuple::Item<double> m_tof2_pid;
			NTuple::Item<double> m_prob_pid;

};

#endif