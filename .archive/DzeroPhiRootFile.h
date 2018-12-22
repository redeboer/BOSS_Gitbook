#ifndef Physics_Analysis_DzeroPhiRootFile_H
#define Physics_Analysis_DzeroPhiRootFile_H

/**
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     October 25th, 2018
 * @remark   @b DEVELOPMENTAL
 *
 * @details  The `DzeroPhiRootTree` is derived from the general `BOSSRootTree` class. It can be used to easily load a ROOT file generated in the jopOptions_ana (analysis) step of the BOSS framework. As opposed to the more `BOSSRootTree` class, the structure of this class needs to match that of the analysis you are working with.
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "BOSSRootFile.h"
	#include "TTree.h"



// * ========================== * //
// * ------- NAMESPACES ------- * //
// * ========================== * //
// ! Modify these according to your ROOT file
/**
 * @brief
 * @details
 */
namespace DzeroPhiBranches
{
	namespace mult { //!< Primary vertex info and multiplicities.
		int    Ntotal;   //!< Total number of events per track.
		int    Ncharge;  //!< Number of charged tracks.
		int    Nneutral; //!< Number of charged tracks.
		double vx0;      //!< Primary \f$x\f$ coordinate of the collision point.
		double vy0;      //!< Primary \f$y\f$ coordinate of the collision point.
		double vz0;      //!< Primary \f$z\f$ coordinate of the collision point.
		int    Ngood;    //!< Number of 'good' charged tracks.
		int    Nmdc;     //!< Number of charged tracks in MDC. @todo Test whether this parameter really makes sense.
		int    NKaonPos; //!< Number of \f$K^+\f$.
		int    NKaonNeg; //!< Number of \f$K^-\f$.
		int    NPionPos; //!< Number of \f$\pi^-\f$.
	};
	namespace vxyz { //!< Vertex information of each track.
		double vx;    //!< Primary \f$x\f$ coordinate of the vertex as determined by MDC.
		double vy;    //!< Primary \f$y\f$ coordinate of the vertex as determined by MDC.
		double vz;    //!< Primary \f$z\f$ coordinate of the vertex as determined by MDC.
		double vr;    //!< Distance from origin in \f$xy\f$ plane.
		double rvxy;  //!< Nearest distance to IP in \f$xy\f$ plane.
		double rvz;   //!< Nearest distance to IP in \f$z\f$ direction.
		double rvphi; //!< Angle in the \f$xy\f$plane (?). @todo
		double phi;   //!< Helix angle of the particle (?). @todo
		double p;     //!< Momentum \f$p\f$ of the track.
	};
	namespace fit4c { //!< Invariant masses and \f$\chi_\mathrm{red}^2\f$ as determined by 4-constraint Kalman kinematic fit.
		double mD0;   //!< Invariant mass for \f$ K^- pi^+ \f$ (\f$ D^0 \f$).
		double mphi;  //!< Invariant mass for \f$ K^+ K^+  \f$ (\f$ \phi \f$).
		double mJpsi; //!< Invariant mass for \f$ D^0 \phi \f$ (\f$ J/\psi \f$).
		double chi2;  //!< Chi squared of the Kalman kinematic fit.
	}
	namespace fit6c { //!< Invariant masses and \f$\chi_\mathrm{red}^2\f$ as determined by 6-constraint Kalman kinematic fit.
		double mD0;   //!< Invariant mass for \f$ M_{K^-pi^+} \f$ (\f$ D^0    \f$)
		double mphi;  //!< Invariant mass for \f$ M_{K^+K^+ } \f$ (\f$ \phi   \f$)
		double mJpsi; //!< Invariant mass for \f$ M_{D^0phi } \f$ (\f$ J/\psi \f$)
		double chi2;  //!< Chi squared of the Kalman kinematic fit.
	}
	namespace dedx { //!< Energy loss (\f$ dE/dx \f$) PID branch.
		double ptrk;   //!< Momentum of the track.
		double chie;   //!< Chi squared in case of electron.
		double chimu;  //!< Chi squared in case of muon.
		double chipi;  //!< Chi squared in case of pion.
		double chik;   //!< Chi squared in case of kaon.
		double chip;   //!< Chi squared in case of proton.
		double probPH; //!< Most probable pulse height from truncated mean.
		double normPH; //!< Normalized pulse height.
		double ghit;   //!< Number of good hits.
		double thit;   //!< Total number of hits.
	}
	namespace tofe { //!< Time of Flight end cap branch.
		double tof;  //!< Time of flight.
		double ptrk; //!< Momentum of the track as reconstructed by MDC.
		double path; //!< Path length.
		double cntr; //!< ToF counter ID.
		double ph;   //!< ToF pulse height.
		double rhit; //!< Track extrapolate \f$Z\f$ or \f$R\f$ Hit position.
		double qual; //!< Data quality of reconstruction.
		double te;   //!< Difference with ToF in electron hypothesis.
		double tmu;  //!< Difference with ToF in muon hypothesis.
		double tpi;  //!< Difference with ToF in charged pion hypothesis.
		double tk;   //!< Difference with ToF in charged kaon hypothesis.
		double tp;   //!< Difference with ToF in proton hypothesis.
	}
	namespace tof1 { //!< ToF <b>inner</b> barrel branch.
		double tof;  //!< Time of flight.
		double ptrk; //!< Momentum of the track as reconstructed by MDC.
		double path; //!< Path length.
		double cntr; //!< ToF counter ID.
		double ph;   //!< ToF pulse height.
		double zhit; //!< Track extrapolate \f$Z\f$ or \f$R\f$ Hit position.
		double qual; //!< Data quality of reconstruction.
		double te;   //!< Difference with ToF in electron hypothesis.
		double tmu;  //!< Difference with ToF in muon hypothesis.
		double tpi;  //!< Difference with ToF in charged pion hypothesis.
		double tk;   //!< Difference with ToF in charged kaon hypothesis.
		double tp;   //!< Difference with ToF in proton hypothesis.
	}
	namespace tof2 { //!< ToF <b>outer</b> barrel branch.
		double tof;  //!< Time of flight.
		double ptrk; //!< Momentum of the track as reconstructed by MDC.
		double path; //!< Path length.
		double cntr; //!< ToF counter ID.
		double ph;   //!< ToF pulse height.
		double zhit; //!< Track extrapolate \f$Z\f$ or \f$R\f$ Hit position.
		double qual; //!< Data quality of reconstruction.
		double te;   //!< Difference with ToF in electron hypothesis.
		double tmu;  //!< Difference with ToF in muon hypothesis.
		double tpi;  //!< Difference with ToF in charged pion hypothesis.
		double tk;   //!< Difference with ToF in charged kaon hypothesis.
		double tp;   //!< Difference with ToF in proton hypothesis.
	}
	namespace pid { //!< Track PID information.
		double ptrk; //!< Momentum of the track.
		double cost; //!< Theta angle of the track.
		double dedx; //!< Chi squared of the dedx of the track.
		double tof1; //!< Chi squared of the inner barrel ToF of the track.
		double tof2; //!< Chi squared of the outer barrel ToF of the track.
		double prob; //!< Probability that it is a pion.
	}
}



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class DzeroPhiRootFile : public BOSSRootFile
{

public:
	// * Constructor and destructors *
		DzeroPhiRootFile() : BOSSRootFile() { Initialize(); }
		DzeroPhiRootFile(const char* filename);
		~DzeroPhiRootFile();

	// * Getters *
		TTree* mult()   { return fMult; }
		TTree* vxyz()   { return fVxyz; }
		TTree* fit4c()  { return fFit4c; }
		TTree* fit6c()  { return fFit6c; }
		TTree* dedx()   { return fDedx; }
		TTree* tof1()   { return fTof1; }
		TTree* tof2()   { return fTof2; }
		TTree* tofe()   { return fTofe; }
		TTree* pid()    { return fPid; }


private:
	// * Data members *
		TTree* fMult;  //!< Branch for primary vertex information and multiplicities.
		TTree* fVxyz;  //!< Branch for track vertex information.
		TTree* fFit4c; //!< Branch for neutral pion (\f$ \pi^0 \f$) invariant mass.
		TTree* fFit6c; //!< Branch for charged and neutral rho (\f$ \rho^0, \rho^-, \rho^+ \f$) invariant mass.
		TTree* fDedx;  //!< Branch for PID using energy loss (\f$ dE/dx \f$).
		TTree* fTof1;  //!< Branch for Time of Flight registered by the <b>inner</b> barrel.
		TTree* fTof2;  //!< Branch for Time of Flight registered by the <b>outer</b> barrel.
		TTree* fTofe;  //!< Branch for Time of Flight registered by the <b>end cap</b>.
		TTree* fPid;   //!< Branch for particle identification information.

	// * Constructor/destructor helpers *
		void Initialize();
		void Destruct();

};



// * =========================================== * //
// * ------- CONSRUCTORS AND DESTRUCTORS ------- * //
// * =========================================== * //
	/**
	 * @brief Constructor. Loads all trees and sets branch addresses of the default DzeroPhi ROOT file.
	 * @param filename File name of the ROOT file that you want to load.
	 */
	DzeroPhiRootFile::DzeroPhiRootFile(const char* filename) : BOSSRootFile(filename)
	{
		// * Load trees *
			// ! Modify these according to your ROOT file
			if(!(fMult   = FindTree("mult")))   Destruct();
			if(!(fVxyz   = FindTree("vxyz")))   Destruct();
			if(!(fFit4c  = FindTree("fit4c")))  Destruct();
			if(!(fFit6c  = FindTree("fit6c")))  Destruct();
			if(!(fDedx   = FindTree("dedx")))   Destruct();
			if(!(fTofe   = FindTree("tofe")))   Destruct();
			if(!(fTof1   = FindTree("tof1")))   Destruct();
			if(!(fTof2   = FindTree("tof2")))   Destruct();
			if(!(fPid    = FindTree("pid")))    Destruct();

		// * Set branching addresses *
			// ! Modify these according to your ROOT file
			// * Primary vertex info and multiplicities (mult)
				SetBranchAddress(fMult, "Ntotal",   DzeroPhiBranches::mult::Ntotal);
				SetBranchAddress(fMult, "Ncharge",  DzeroPhiBranches::mult::Ncharge);
				SetBranchAddress(fMult, "Nneutral", DzeroPhiBranches::mult::Nneutral);
				SetBranchAddress(fMult, "vx0",      DzeroPhiBranches::mult::vx0);
				SetBranchAddress(fMult, "vy0",      DzeroPhiBranches::mult::vy0);
				SetBranchAddress(fMult, "vz0",      DzeroPhiBranches::mult::vz0);
				SetBranchAddress(fMult, "Ngood",    DzeroPhiBranches::mult::Ngood);
				SetBranchAddress(fMult, "Nmdc",     DzeroPhiBranches::mult::Nmdc);
				SetBranchAddress(fMult, "fEvent_NKaonPos", DzeroPhiBranches::mult::NKaonPos);
				SetBranchAddress(fMult, "fEvent_NKaonNeg", DzeroPhiBranches::mult::NKaonNeg);
				SetBranchAddress(fMult, "fEvent_NPionPos", DzeroPhiBranches::mult::NPionPos);

			// * Vertex information of each track (vxyz)
				SetBranchAddress(fVxyz, "vx",    DzeroPhiBranches::vxyz::vx);
				SetBranchAddress(fVxyz, "vy",    DzeroPhiBranches::vxyz::vy);
				SetBranchAddress(fVxyz, "vz",    DzeroPhiBranches::vxyz::vz);
				SetBranchAddress(fVxyz, "vr",    DzeroPhiBranches::vxyz::vr);
				SetBranchAddress(fVxyz, "rvxy",  DzeroPhiBranches::vxyz::rvxy);
				SetBranchAddress(fVxyz, "rvz",   DzeroPhiBranches::vxyz::rvz);
				SetBranchAddress(fVxyz, "rvphi", DzeroPhiBranches::vxyz::rvphi);
				SetBranchAddress(fVxyz, "phi",   DzeroPhiBranches::vxyz::phi);
				SetBranchAddress(fVxyz, "p",     DzeroPhiBranches::vxyz::p);

			// * Fit pi^0 information from EMCal (fit4c)
				SetBranchAddress(fFit4c,  "mD0",   DzeroPhiBranches::fit4c::mD0);
				SetBranchAddress(fFit4c,  "mphi",  DzeroPhiBranches::fit4c::mphi);
				SetBranchAddress(fFit4c,  "mJpsi", DzeroPhiBranches::fit4c::mJpsi);
				SetBranchAddress(fFit4c,  "chi2",  DzeroPhiBranches::fit4c::chi2);

			// * Fit rho^0, rho^+, rho^- inv. mass from EMCal (fit6c)
				SetBranchAddress(fFit6c,  "mD0",   DzeroPhiBranches::fit6c::mD0);
				SetBranchAddress(fFit6c,  "mphi",  DzeroPhiBranches::fit6c::mphi);
				SetBranchAddress(fFit6c,  "mJpsi", DzeroPhiBranches::fit6c::mJpsi);
				SetBranchAddress(fFit6c,  "chi2",  DzeroPhiBranches::fit6c::chi2);

			// * dE/dx PID information (dedx)
				SetBranchAddress(fDedx,   "ptrk",   DzeroPhiBranches::dedx::ptrk);
				SetBranchAddress(fDedx,   "chie",   DzeroPhiBranches::dedx::chie);
				SetBranchAddress(fDedx,   "chimu",  DzeroPhiBranches::dedx::chimu);
				SetBranchAddress(fDedx,   "chipi",  DzeroPhiBranches::dedx::chipi);
				SetBranchAddress(fDedx,   "chik",   DzeroPhiBranches::dedx::chik);
				SetBranchAddress(fDedx,   "chip",   DzeroPhiBranches::dedx::chip);
				SetBranchAddress(fDedx,   "probPH", DzeroPhiBranches::dedx::probPH);
				SetBranchAddress(fDedx,   "normPH", DzeroPhiBranches::dedx::normPH);
				SetBranchAddress(fDedx,   "ghit",   DzeroPhiBranches::dedx::ghit);
				SetBranchAddress(fDedx,   "thit",   DzeroPhiBranches::dedx::thit);

			// * ToF end cap information (tofe)
				SetBranchAddress(fTofe, "tof",  DzeroPhiBranches::tofe::tof);
				SetBranchAddress(fTofe, "ptrk", DzeroPhiBranches::tofe::ptrk);
				SetBranchAddress(fTofe, "path", DzeroPhiBranches::tofe::path);
				SetBranchAddress(fTofe, "cntr", DzeroPhiBranches::tofe::cntr);
				SetBranchAddress(fTofe, "ph",   DzeroPhiBranches::tofe::ph);
				SetBranchAddress(fTofe, "rhit", DzeroPhiBranches::tofe::rhit);
				SetBranchAddress(fTofe, "qual", DzeroPhiBranches::tofe::qual);
				SetBranchAddress(fTofe, "te",   DzeroPhiBranches::tofe::te);
				SetBranchAddress(fTofe, "tmu",  DzeroPhiBranches::tofe::tmu);
				SetBranchAddress(fTofe, "tpi",  DzeroPhiBranches::tofe::tpi);
				SetBranchAddress(fTofe, "tk",   DzeroPhiBranches::tofe::tk);
				SetBranchAddress(fTofe, "tp",   DzeroPhiBranches::tofe::tp);

			// * ToF inner barrel information (tof1)
				SetBranchAddress(fTof1, "tof",  DzeroPhiBranches::tof1::tof);
				SetBranchAddress(fTof1, "ptrk", DzeroPhiBranches::tof1::ptrk);
				SetBranchAddress(fTof1, "path", DzeroPhiBranches::tof1::path);
				SetBranchAddress(fTof1, "cntr", DzeroPhiBranches::tof1::cntr);
				SetBranchAddress(fTof1, "ph",   DzeroPhiBranches::tof1::ph);
				SetBranchAddress(fTof1, "zhit", DzeroPhiBranches::tof1::zhit);
				SetBranchAddress(fTof1, "qual", DzeroPhiBranches::tof1::qual);
				SetBranchAddress(fTof1, "te",   DzeroPhiBranches::tof1::te);
				SetBranchAddress(fTof1, "tmu",  DzeroPhiBranches::tof1::tmu);
				SetBranchAddress(fTof1, "tpi",  DzeroPhiBranches::tof1::tpi);
				SetBranchAddress(fTof1, "tk",   DzeroPhiBranches::tof1::tk);
				SetBranchAddress(fTof1, "tp",   DzeroPhiBranches::tof1::tp);

			// * ToF outer barrel information (tof2)
				SetBranchAddress(fTof2, "tof",  DzeroPhiBranches::tof2::tof);
				SetBranchAddress(fTof2, "ptrk", DzeroPhiBranches::tof2::ptrk);
				SetBranchAddress(fTof2, "path", DzeroPhiBranches::tof2::path);
				SetBranchAddress(fTof2, "cntr", DzeroPhiBranches::tof2::cntr);
				SetBranchAddress(fTof2, "ph",   DzeroPhiBranches::tof2::ph);
				SetBranchAddress(fTof2, "zhit", DzeroPhiBranches::tof2::zhit);
				SetBranchAddress(fTof2, "qual", DzeroPhiBranches::tof2::qual);
				SetBranchAddress(fTof2, "te",   DzeroPhiBranches::tof2::te);
				SetBranchAddress(fTof2, "tmu",  DzeroPhiBranches::tof2::tmu);
				SetBranchAddress(fTof2, "tpi",  DzeroPhiBranches::tof2::tpi);
				SetBranchAddress(fTof2, "tk",   DzeroPhiBranches::tof2::tk);
				SetBranchAddress(fTof2, "tp",   DzeroPhiBranches::tof2::tp);

			// * Particle Identification (pid)
				SetBranchAddress(fPid,    "ptrk",   DzeroPhiBranches::pid::ptrk);
				SetBranchAddress(fPid,    "cost",   DzeroPhiBranches::pid::cost);
				SetBranchAddress(fPid,    "dedx",   DzeroPhiBranches::pid::dedx);
				SetBranchAddress(fPid,    "tof1",   DzeroPhiBranches::pid::tof1);
				SetBranchAddress(fPid,    "tof2",   DzeroPhiBranches::pid::tof2);
				SetBranchAddress(fPid,    "prob",   DzeroPhiBranches::pid::prob);
	}

	/**
	 * @brief Default destructor. Merely calls the destructor helper.
	 */
	DzeroPhiRootFile::~DzeroPhiRootFile()
	{
		Destruct();
	}

	/**
	 * @brief Auxiliary function for the destructor (so that the destructor functionality can also be called by other methods). Merely sets pointers to null.
	 */
	void DzeroPhiRootFile::Destruct()
	{
		BOSSRootFile::Destruct(); // ? needed?
		Initialize();
	}

	/**
	 * @brief Auxiliar function for constructor (so that it can be called by all constructor types and other methods). Only sets sets all pointer type data members to nullpointers.
	 */
	void DzeroPhiRootFile::Initialize()
	{
		fMult   = nullptr;
		fVxyz   = nullptr;
		fDedx   = nullptr;
		fTof1   = nullptr;
		fTof2   = nullptr;
		fPid    = nullptr;
		fFit4c  = nullptr;
		fFit6c  = nullptr;
		fTofe   = nullptr;
	}

#endif