#ifndef Physics_Analysis_Rhopi_H
#define Physics_Analysis_Rhopi_H

#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"
//#include "VertexFit/ReadBeamParFromDb.h"


class KGamma : public Algorithm {

public:
	KGamma(const std::string& name, ISvcLocator* pSvcLocator);
	StatusCode initialize();
	StatusCode execute();
	StatusCode finalize();

private:

	// ReadBeamParFromDb m_reader;

	// * ------- DECLARE CUTS HERE ------- * //

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


	// * ------- DECLARE NTUPLES HERE ------- * //

		// * Charged track vertex *
			NTuple::Tuple* m_tuple1;
			NTuple::Item<double> m_vx0;
			NTuple::Item<double> m_vy0;
			NTuple::Item<double> m_vz0;
			NTuple::Item<double> m_vr0;
			NTuple::Item<double> m_rvxy0;
			NTuple::Item<double> m_rvz0;
			NTuple::Item<double> m_rvphi0;

		// * Fake photon *
			NTuple::Tuple* m_tuple2;
			NTuple::Item<double> m_dthe;
			NTuple::Item<double> m_dphi;
			NTuple::Item<double> m_dang;
			NTuple::Item<double> m_eraw;

		// * KGamma: raw mgg, etot *
			NTuple::Tuple* m_tuple3;
			NTuple::Item<double> m_m2gg;
			NTuple::Item<double> m_etot;

		// * KGamma fit4C *
			NTuple::Tuple* m_tuple4;
			NTuple::Item<double> m_chi1;
			NTuple::Item<double> m_mpi0;

		// * KGamma fit5C *
			NTuple::Tuple* m_tuple5;
			NTuple::Item<double> m_chi2;
			NTuple::Item<double> m_mrho0;
			NTuple::Item<double> m_mrhop;
			NTuple::Item<double> m_mrhom;

		// * Photons *
			NTuple::Tuple* m_tuple6;
			NTuple::Item<double> m_fcos;
			NTuple::Item<double> m_elow;

		// * Energy loss dE/dx *
			NTuple::Tuple* m_tuple7;
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

		// * End cap ToF *
			NTuple::Tuple* m_tuple8;
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

		// * Barrel inner ToF *
			NTuple::Tuple* m_tuple9;
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

		// * Barrel outer ToF *
			NTuple::Tuple* m_tuple10;
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
			NTuple::Tuple* m_tuple11;
			NTuple::Item<double> m_ptrk_pid;
			NTuple::Item<double> m_cost_pid;
			NTuple::Item<double> m_dedx_pid;
			NTuple::Item<double> m_tof1_pid;
			NTuple::Item<double> m_tof2_pid;
			NTuple::Item<double> m_prob_pid;

};

#endif