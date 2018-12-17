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
	#include "DzeroPhiRootNamespace.h"
	#include "TTree.h"



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
	TTree* vxyz()   { return fVxyz; }
	TTree* dedx()   { return fDedx; }
	TTree* tof1()   { return fTof1; }
	TTree* tof2()   { return fTof2; }
	TTree* pid()    { return fPid; }
	TTree* fit4c()  { return fFit4c; }
	TTree* fit6c()  { return fFit6c; }
	TTree* tofe()   { return fTofe; }


private:
	// * Data members *
	TTree* fVxyz;
	//!< Branch for primary vertex information.
	TTree* fDedx;
	//!< Branch for PID using energy loss (\f$ dE/dx \f$).
	TTree* fTof1;
	//!< Branch for Time of Flight registered by the <b>inner</b> barrel.
	TTree* fTof2;
	//!< Branch for Time of Flight registered by the <b>outer</b> barrel.
	TTree* fPid;
	//!< Branch for particle identification information.
	TTree* fFit4c;
	//!< Branch for neutral pion (\f$ \pi^0 \f$) invariant mass.
	TTree* fFit6c;
	//!< Branch for charged and neutral rho (\f$ \rho^0, \rho^-, \rho^+ \f$) invariant mass.
	TTree* fTofe;
	//!< Branch for Time of Flight registered by the <b>end cap</b>.

	// * Private methods *
	void PrintDistribution1D();

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
		if(!(fVxyz   = FindTree("vxyz")))   Destruct(); // Vertex position
		if(!(fDedx   = FindTree("dedx")))   Destruct(); // dE/dx PID information
		if(!(fTof1   = FindTree("tof1")))   Destruct(); // ToF inner barrel information
		if(!(fTof2   = FindTree("tof2")))   Destruct(); // ToF outer barrel information
		if(!(fPid    = FindTree("pid")))    Destruct(); // Particle Identification
		if(!(fFit4c  = FindTree("fit4c")))  Destruct(); // Fit pi^0 information from EMCal
		if(!(fFit6c  = FindTree("fit6c")))  Destruct(); // Fit rho^0, rho^+, rho^- inv. mass from EMCal
		if(!(fTofe   = FindTree("tofe")))   Destruct(); // ToF end cap information
	// * Set branching addresses *
	// ! Modify these according to your ROOT file
		// * Vertex position (vxyz / m_tuple1)
			SetBranchAddress(fVxyz,   "vx0",    AnaBranches::vxyz::vx0);    // primary x-vertex as determined by MDC
			SetBranchAddress(fVxyz,   "vy0",    AnaBranches::vxyz::vy0);    // primary y-vertex as determined by MDC
			SetBranchAddress(fVxyz,   "vz0",    AnaBranches::vxyz::vz0);    // primary z-vertex as determined by MDC
			SetBranchAddress(fVxyz,   "vr0",    AnaBranches::vxyz::vr0);    // distance from origin in xy-plane
			SetBranchAddress(fVxyz,   "rvxy0",  AnaBranches::vxyz::rvxy0);  // nearest distance to IP in xy plane
			SetBranchAddress(fVxyz,   "rvz0",   AnaBranches::vxyz::rvz0);   // nearest distance to IP in z direction
			SetBranchAddress(fVxyz,   "rvphi0", AnaBranches::vxyz::rvphi0); // angle in the xy-plane (?)
		// * dE/dx PID information (dedx / m_tuple7)
			SetBranchAddress(fDedx,   "ptrk",   AnaBranches::dedx::ptrk);   // momentum of the track
			SetBranchAddress(fDedx,   "chie",   AnaBranches::dedx::chie);   // chi2 in case of electron
			SetBranchAddress(fDedx,   "chimu",  AnaBranches::dedx::chimu);  // chi2 in case of muon
			SetBranchAddress(fDedx,   "chipi",  AnaBranches::dedx::chipi);  // chi2 in case of pion
			SetBranchAddress(fDedx,   "chik",   AnaBranches::dedx::chik);   // chi2 in case of kaon
			SetBranchAddress(fDedx,   "chip",   AnaBranches::dedx::chip);   // chi2 in case of proton
			SetBranchAddress(fDedx,   "probPH", AnaBranches::dedx::probPH); // most probable pulse height from truncated mean
			SetBranchAddress(fDedx,   "normPH", AnaBranches::dedx::normPH); // normalized pulse height
			SetBranchAddress(fDedx,   "ghit",   AnaBranches::dedx::ghit);   // number of good hits
			SetBranchAddress(fDedx,   "thit",   AnaBranches::dedx::thit);   // total number of hits
		// * ToF inner barrel information (tof1 / m_tuple9)
			SetBranchAddress(fTof1,   "ptrk",   AnaBranches::tof1::ptrk);   // momentum of the track as reconstructed by MDC
			SetBranchAddress(fTof1,   "path",   AnaBranches::tof1::path);   // Path length
			SetBranchAddress(fTof1,   "tof",    AnaBranches::tof1::tof);    // Time of flight
			SetBranchAddress(fTof1,   "cntr",   AnaBranches::tof1::cntr);   // ToF counter ID
			SetBranchAddress(fTof1,   "ph",     AnaBranches::tof1::ph);     // ToF pulse height
			SetBranchAddress(fTof1,   "zhit",   AnaBranches::tof1::zhit);   // track extrapolate Z or R Hit position
			SetBranchAddress(fTof1,   "qual",   AnaBranches::tof1::qual);   // data quality of reconstruction
			SetBranchAddress(fTof1,   "te",     AnaBranches::tof1::te);     // difference with ToF in electron hypothesis
			SetBranchAddress(fTof1,   "tmu",    AnaBranches::tof1::tmu);    // difference with ToF in muon hypothesis
			SetBranchAddress(fTof1,   "tpi",    AnaBranches::tof1::tpi);    // difference with ToF in charged pion hypothesis
			SetBranchAddress(fTof1,   "tk",     AnaBranches::tof1::tk);     // difference with ToF in charged kaon hypothesis
			SetBranchAddress(fTof1,   "tp",     AnaBranches::tof1::tp);     // difference with ToF in proton hypothesis
		// * ToF outer barrel information (tof2 / m_tuple10)
			SetBranchAddress(fTof2,   "ptrk",   AnaBranches::tof2::ptrk);   // momentum of the track as reconstructed by MDC
			SetBranchAddress(fTof2,   "path",   AnaBranches::tof2::path);   // Path length
			SetBranchAddress(fTof2,   "tof",    AnaBranches::tof2::tof);    // Time of flight
			SetBranchAddress(fTof2,   "cntr",   AnaBranches::tof2::cntr);   // ToF counter ID
			SetBranchAddress(fTof2,   "ph",     AnaBranches::tof2::ph);     // ToF pulse height
			SetBranchAddress(fTof2,   "zhit",   AnaBranches::tof2::zhit);   // track extrapolate Z or R Hit position
			SetBranchAddress(fTof2,   "qual",   AnaBranches::tof2::qual);   // data quality of reconstruction
			SetBranchAddress(fTof2,   "te",     AnaBranches::tof2::te);     // difference with ToF in electron hypothesis
			SetBranchAddress(fTof2,   "tmu",    AnaBranches::tof2::tmu);    // difference with ToF in muon hypothesis
			SetBranchAddress(fTof2,   "tpi",    AnaBranches::tof2::tpi);    // difference with ToF in charged pion hypothesis
			SetBranchAddress(fTof2,   "tk",     AnaBranches::tof2::tk);     // difference with ToF in charged kaon hypothesis
			SetBranchAddress(fTof2,   "tp",     AnaBranches::tof2::tp);     // difference with ToF in proton hypothesis
		// * Particle Identification (pid / m_tuple11)
			SetBranchAddress(fPid,    "ptrk",   AnaBranches::pid::ptrk);    // momentum of the track
			SetBranchAddress(fPid,    "cost",   AnaBranches::pid::cost);    // theta angle of the track
			SetBranchAddress(fPid,    "dedx",   AnaBranches::pid::dedx);    // Chi squared of the dedx of the track
			SetBranchAddress(fPid,    "tof1",   AnaBranches::pid::tof1);    // Chi squared of the inner barrel ToF of the track
			SetBranchAddress(fPid,    "tof2",   AnaBranches::pid::tof2);    // Chi squared of the outer barrel ToF of the track
			SetBranchAddress(fPid,    "prob",   AnaBranches::pid::prob);    // probability that it is a pion
		// * Fit pi^0 information from EMCal (fit4c / m_tuple4)
			SetBranchAddress(fFit4c,  "mD0",   AnaBranches::fit4c::mD0);   // Invariant mass \f$ M_{K^-pi^+} \f$ (\f$ D^0    \f$)
			SetBranchAddress(fFit4c,  "mphi",  AnaBranches::fit4c::mphi);  // Invariant mass \f$ M_{K^+K^+ } \f$ (\f$ \phi   \f$)
			SetBranchAddress(fFit4c,  "mJpsi", AnaBranches::fit4c::mJpsi); // Invariant mass \f$ M_{D^0phi } \f$ (\f$ J/\psi \f$)
			SetBranchAddress(fFit4c,  "chi2",  AnaBranches::fit4c::chi2);  // Chi squared of the Kalman kinematic fit.
		// * Fit rho^0, rho^+, rho^- inv. mass from EMCal (fit6c / m_tuple5)
			SetBranchAddress(fFit6c,  "mD0",   AnaBranches::fit6c::mD0);   // Invariant mass \f$ M_{K^-pi^+} \f$ (\f$ D^0    \f$)
			SetBranchAddress(fFit6c,  "mphi",  AnaBranches::fit6c::mphi);  // Invariant mass \f$ M_{K^+K^+ } \f$ (\f$ \phi   \f$)
			SetBranchAddress(fFit6c,  "mJpsi", AnaBranches::fit6c::mJpsi); // Invariant mass \f$ M_{D^0phi } \f$ (\f$ J/\psi \f$)
			SetBranchAddress(fFit6c,  "chi2",  AnaBranches::fit6c::chi2);  // Chi squared of the Kalman kinematic fit.
		// * ToF end cap information (tofe / m_tuple8)
			SetBranchAddress(fTofe,   "ptrk",   AnaBranches::tofe::ptrk);   // momentum of the track as reconstructed by MDC
			SetBranchAddress(fTofe,   "path",   AnaBranches::tofe::path);   // Path length
			SetBranchAddress(fTofe,   "tof",    AnaBranches::tofe::tof);    // Time of flight
			SetBranchAddress(fTofe,   "cntr",   AnaBranches::tofe::cntr);   // ToF counter ID
			SetBranchAddress(fTofe,   "ph",     AnaBranches::tofe::ph);     // ToF pulse height
			SetBranchAddress(fTofe,   "rhit",   AnaBranches::tofe::rhit);   // track extrapolate Z or R Hit position
			SetBranchAddress(fTofe,   "qual",   AnaBranches::tofe::qual);   // data quality of reconstruction
			SetBranchAddress(fTofe,   "te",     AnaBranches::tofe::te);     // difference with ToF in electron hypothesis
			SetBranchAddress(fTofe,   "tmu",    AnaBranches::tofe::tmu);    // difference with ToF in muon hypothesis
			SetBranchAddress(fTofe,   "tpi",    AnaBranches::tofe::tpi);    // difference with ToF in charged pion hypothesis
			SetBranchAddress(fTofe,   "tk",     AnaBranches::tofe::tk);     // difference with ToF in charged kaon hypothesis
			SetBranchAddress(fTofe,   "tp",     AnaBranches::tofe::tp);     // difference with ToF in proton hypothesis
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
	BOSSRootFile::Destruct(); // needed?
	Initialize();
}

/**
 * @brief Auxiliar function for constructor (so that it can be called by all constructor types and other methods). Only sets sets all pointer type data members to nullpointers.
 */
void DzeroPhiRootFile::Initialize()
{
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