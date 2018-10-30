#ifndef Physics_Analysis_RhopiRootFile_H
#define Physics_Analysis_RhopiRootFile_H

/**
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     October 25th, 2018
 * @remark   @b DEVELOPMENTAL
 *
 * @details  The `RhopiRootChain` is derived from the general `BOSSRootChain` class. It can be used to easily load a ROOT file generated in the jopOptions_ana (analysis) step of the BOSS framework. As opposed to the more `BOSSRootChain` class, the structure of this class needs to match that of the analysis you are working with.
 */


// * ========================= * //
// * ------- LIBRARIES ------- *
// * ========================= * //
#include "BOSSRootChain.h"
#include "TChain.h"



// * ========================== * //
// * ------- NAMESPACES ------- * //
// * ========================== * //
// ! Modify these according to your ROOT file
/**
 * @brief
 * @details
 */
namespace AnaBranches
{
	/**
	 * @brief Primary vertex position.
	 * @details This namespace represents the branch structure of the `vxyz` tree. Its leaves describe the position of the primary vertex of each event.
	 */
	namespace vxyz {
		double vx0;    //!< Primary x-vertex as determined by MDC
		double vy0;    //!< Primary y-vertex as determined by MDC
		double vz0;    //!< Primary z-vertex as determined by MDC
		double vr0;    //!< Distance from origin in xy-plane
		double rvxy0;  //!< Nearest distance to IP in xy plane
		double rvz0;   //!< Nearest distance to IP in z direction
		double rvphi0; //!< Angle in the xy-plane (?)
	};

	/**
	 * @brief Photon kinematics.
	 * @details
	 */
	namespace photon {
		double dthe; //!< Theta difference with nearest charged track (degrees).`
		double dphi; //!< Phi difference with nearest charged track (degrees).`
		double dang; //!< Angle difference with nearest charged track.`
		double eraw; //!< Energy of the photon.`
	}

	/**
	 * @brief Energy loss (\f$ dE/dx \f$) PID branch.
	 * @details
	 */
	namespace dedx {
		double ptrk;   //!< Momentum of the track.
		double chie;   //!< Chi2 in case of electron.
		double chimu;  //!< Chi2 in case of muon.
		double chipi;  //!< Chi2 in case of pion.
		double chik;   //!< Chi2 in case of kaon.
		double chip;   //!< Chi2 in case of proton.
		double probPH; //!< Most probable pulse height from truncated mean.
		double normPH; //!< Normalized pulse height.
		double ghit;   //!< Number of good hits.
		double thit;   //!< Total number of hits.
	}

	/**
	 * @brief ToF <b>inner</b> barrel branch.
	 * @details
	 */
	namespace tof1 {
		double ptrk; //!< Momentum of the track as reconstructed by MDC.
		double path; //!< Path length.
		double tof;  //!< Time of flight.
		double cntr; //!< ToF counter ID.
		double ph;   //!< ToF pulse height.
		double zhit; //!< Track extrapolate Z or R Hit position.
		double qual; //!< Data quality of reconstruction.
		double te;   //!< Difference with ToF in electron hypothesis.
		double tmu;  //!< Difference with ToF in muon hypothesis.
		double tpi;  //!< Difference with ToF in charged pion hypothesis.
		double tk;   //!< Difference with ToF in charged kaon hypothesis.
		double tp;   //!< Difference with ToF in proton hypothesis.
	}

	/**
	 * @brief ToF <b>outer</b> barrel branch.
	 * @details
	 */
	namespace tof2 {
		double ptrk; //!< Momentum of the track as reconstructed by MDC.
		double path; //!< Path length.
		double tof;  //!< Time of flight.
		double cntr; //!< ToF counter ID.
		double ph;   //!< ToF pulse height.
		double zhit; //!< Track extrapolate Z or R Hit position.
		double qual; //!< Data quality of reconstruction.
		double te;   //!< Difference with ToF in electron hypothesis.
		double tmu;  //!< Difference with ToF in muon hypothesis.
		double tpi;  //!< Difference with ToF in charged pion hypothesis.
		double tk;   //!< Difference with ToF in charged kaon hypothesis.
		double tp;   //!< Difference with ToF in proton hypothesis.
	}

	/**
	 * @brief Track PID information.
	 * @details
	 */
	namespace pid {
		double ptrk; //!< Momentum of the track.
		double cost; //!< Theta angle of the track.
		double dedx; //!< Chi2 of the dedx of the track.
		double tof1; //!< Chi2 of the inner barrel ToF of the track.
		double tof2; //!< Chi2 of the outer barrel ToF of the track.
		double prob; //!< Probability that it is a pion.
	}

	/**
	 * @brief Energy branch.
	 * @details
	 */
	namespace etot {
		double m2gg; //!< Invariant mass of the two gammas.
		double etot; //!< Total energy of pi^+, pi^ and the two gammas.
	}

	/**
	 * @brief Neutral pion (\f$ \pi^0 \f$) invariant mass fit branch.
	 * @details
	 */
	namespace fit4c {
		double mpi0; //!< Invariant pi0 mass according to Kalman kinematic fit.
		double chi2; //!< Chi square of the Kalman kinematic fit.
	}

	/**
	 * @brief Charged and neutral rho (\f$ \rho^0, \rho^-, \rho^+ \f$) invariant mass fit branch.
	 * @details
	 */
	namespace fit5c {
		double chi2;  //!< Chi squared of the Kalman kinematic fit.
		double mrho0; //!< Invariant mass \f$ M_{\pi^+pi^-} \f$ (\f$ \rho^0 \f$).
		double mrhop; //!< Invariant mass \f$ M_{\pi^0pi^+} \f$ (\f$ \rho^+ \f$).
		double mrhom; //!< Invariant mass \f$ M_{\pi^0pi^-} \f$ (\f$ \rho^- \f$).
	}

	/**
	 * @brief Photon detection efficiences.
	 * @details
	 */
	namespace geff {
		double fcos; //!< E/p ratio for pi^0 candidate.
		double elow; //!< Lowest energy of the two gammas.
	}

	/**
	 * @brief Time of Flight end cap branch.
	 * @details
	 */
	namespace tofe {
		double ptrk; //!< Momentum of the track as reconstructed by MDC.
		double path; //!< Path length.
		double tof;  //!< Time of flight.
		double cntr; //!< ToF counter ID.
		double ph;   //!< ToF pulse height.
		double rhit; //!< Track extrapolate Z or R Hit position.
		double qual; //!< Data quality of reconstruction.
		double te;   //!< Difference with ToF in electron hypothesis.
		double tmu;  //!< Difference with ToF in muon hypothesis.
		double tpi;  //!< Difference with ToF in charged pion hypothesis.
		double tk;   //!< Difference with ToF in charged kaon hypothesis.
		double tp;   //!< Difference with ToF in proton hypothesis.
	}
}

// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class RhopiRootChain : public BOSSRootChain
{

public:
	// * Constructor and destructors *
	RhopiRootChain() : BOSSRootChain() { Initialize(); }
	RhopiRootChain(const char* directoryPath, const char* analysisName);
	~RhopiRootChain();

	// * Getters *
	TChain* vxyz()   { return fVxyz; }
	TChain* photon() { return fPhoton; }
	TChain* dedx()   { return fDedx; }
	TChain* tof1()   { return fTof1; }
	TChain* tof2()   { return fTof2; }
	TChain* pid()    { return fPid; }
	TChain* etot()   { return fEtot; }
	TChain* fit4c()  { return fFit4c; }
	TChain* fit5c()  { return fFit5c; }
	TChain* geff()   { return fGeff; }
	TChain* tofe()   { return fTofe; }


private:
	// * Data members *
	TChain* fVxyz;
	//!< Branch for primary vertex information.
	TChain* fPhoton;
	//!< Branch for photon kinematics.
	TChain* fDedx;
	//!< Branch for PID using energy loss (\f$ dE/dx \f$).
	TChain* fTof1;
	//!< Branch for Time of Flight registered by the <b>inner</b> barrel.
	TChain* fTof2;
	//!< Branch for Time of Flight registered by the <b>outer</b> barrel.
	TChain* fPid;
	//!< Branch for particle identification information.
	TChain* fEtot;
	//!< Branch for total energy and energy of the \f$ \gamma \f$'s.
	TChain* fFit4c;
	//!< Branch for neutral pion (\f$ \pi^0 \f$) invariant mass.
	TChain* fFit5c;
	//!< Branch for charged and neutral rho (\f$ \rho^0, \rho^-, \rho^+ \f$) invariant mass.
	TChain* fGeff;
	//!< Branch for photon detection efficiences.
	TChain* fTofe;
	//!< Branch for Time of Flight registered by the <b>end cap</b>.

	// * Private methods *
	void Initialize();
	void Destruct();
	void SetChain(TChain*& chainPtr, const char* chainName);
};


// * =========================================== * //
// * ------- CONSRUCTORS AND DESTRUCTORS ------- * //
// * =========================================== * //
RhopiRootChain::RhopiRootChain(const char* directoryPath, const char* analysisName) :
	BOSSRootChain(directoryPath, analysisName)
{
	// * Load chains *
	// ! Modify these according to your ROOT file
		SetChain(fVxyz,   "vxyz");   // Vertex position
		SetChain(fPhoton, "photon"); // Photon information
		SetChain(fDedx,   "dedx");   // dE/dx PID information
		SetChain(fTof1,   "tof1");   // ToF inner barrel information
		SetChain(fTof2,   "tof2");   // ToF outer barrel information
		SetChain(fPid,    "pid");    // Particle Identification
		SetChain(fEtot,   "etot");   // Total energy
		SetChain(fFit4c,  "fit4c");  // Fit pi^0 information from EMCal
		SetChain(fFit5c,  "fit5c");  // Fit rho^0, rho^+, rho^- inv. mass from EMCal
		SetChain(fGeff,   "geff");   // Photon detection efficiency info
		SetChain(fTofe,   "tofe");   // ToF end cap information
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
		// * Photon information (photon / m_tuple2)
			SetBranchAddress(fPhoton, "dthe",   AnaBranches::photon::dthe); // theta difference with nearest charged track (degrees)
			SetBranchAddress(fPhoton, "dphi",   AnaBranches::photon::dphi); // phi difference with nearest charged track (degrees)
			SetBranchAddress(fPhoton, "dang",   AnaBranches::photon::dang); // angle difference with nearest charged track
			SetBranchAddress(fPhoton, "eraw",   AnaBranches::photon::eraw); // energy of the photon
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
			// SetBranchAddress(fTof1,   "path",   AnaBranches::tof1::path);   // Path length
			// SetBranchAddress(fTof1,   "tof",    AnaBranches::tof1::tof);    // Time of flight
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
			// SetBranchAddress(fTof2,   "path",   AnaBranches::tof2::path);   // Path length
			// SetBranchAddress(fTof2,   "tof",    AnaBranches::tof2::tof);    // Time of flight
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
		// * Total energy (etot / m_tuple3)
			SetBranchAddress(fEtot,   "m2gg",   AnaBranches::etot::m2gg);   // invariant mass of the two gammas
			SetBranchAddress(fEtot,   "etot",   AnaBranches::etot::etot);   // total energy of pi^+, pi^ and the two gammas
		// * Fit pi^0 information from EMCal (fit4c / m_tuple4)
			SetBranchAddress(fFit4c,  "mpi0",   AnaBranches::fit4c::mpi0);  // invariant pi0 mass according to Kalman kinematic fit
			SetBranchAddress(fFit4c,  "chi2",   AnaBranches::fit4c::chi2);  // chi square of the Kalman kinematic fit
		// * Fit rho^0, rho^+, rho^- inv. mass from EMCal (fit5c / m_tuple5)
			SetBranchAddress(fFit5c,  "chi2",   AnaBranches::fit5c::chi2);  // chi squared of the Kalman kinematic fit
			SetBranchAddress(fFit5c,  "mrh0",   AnaBranches::fit5c::mrho0); // inv. mass pi^+ pi^- (rho^0)
			SetBranchAddress(fFit5c,  "mrhp",   AnaBranches::fit5c::mrhop); // inv. mass pi^0 pi^+ (rho^+)
			SetBranchAddress(fFit5c,  "mrhm",   AnaBranches::fit5c::mrhom); // inv. mass pi^0 pi^- (rho^-)
		// * Photon detection efficiency info (geff / m_tuple6)
			SetBranchAddress(fGeff,   "fcos",   AnaBranches::geff::fcos);   // E/p ratio for pi^0 candidate
			SetBranchAddress(fGeff,   "elow",   AnaBranches::geff::elow);   // lowest energy of the two gammas
		// * ToF end cap information (tofe / m_tuple8)
			SetBranchAddress(fTofe,   "ptrk",   AnaBranches::tofe::ptrk);   // momentum of the track as reconstructed by MDC
			// SetBranchAddress(fTofe,   "path",   AnaBranches::tofe::path);   // Path length
			// SetBranchAddress(fTofe,   "tof",    AnaBranches::tofe::tof);    // Time of flight
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
RhopiRootChain::~RhopiRootChain()
{
	Destruct();
}



// * ======================= * //
// * ------- GETTERS ------- * //
// * ======================= * //




// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //
void RhopiRootChain::Initialize()
{
	fVxyz   = nullptr;
	fPhoton = nullptr;
	fDedx   = nullptr;
	fTof1   = nullptr;
	fTof2   = nullptr;
	fPid    = nullptr;
	fEtot   = nullptr;
	fFit4c  = nullptr;
	fFit5c  = nullptr;
	fGeff   = nullptr;
	fTofe   = nullptr;
}
void RhopiRootChain::Destruct()
{
	BOSSRootChain::Destruct(); // needed?
	Initialize();
}
void RhopiRootChain::SetChain(TChain*& chainPtr, const char* chainName)
{
	chainPtr = FindChain(chainName);
	if(!chainPtr) {
		std::cout << "WARNING: Chain \"" << chainName << "\" does not exist and has not been loaded" << std::endl;
	}
}

#endif