#ifndef Physics_Analysis_RhopiRootNamespace_H
#define Physics_Analysis_RhopiRootNamespace_H

/**
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     October 25th, 2018
 * @remark   @b DEVELOPMENTAL
 *
 * @details  This header contains addresses enca
 */


// * ========================= * //
// * ------- LIBRARIES ------- * //
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
		double mrho0; //!< Invariant mass \f$ M_{\pi^+ \pi^-} \f$ (\f$ \rho^0 \f$).
		double mrhop; //!< Invariant mass \f$ M_{\pi^0 \pi^+} \f$ (\f$ \rho^+ \f$).
		double mrhom; //!< Invariant mass \f$ M_{\pi^0 \pi^-} \f$ (\f$ \rho^- \f$).
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

#endif