#ifndef Physics_Analysis_KKFitResult_D0phi_KpiKK_H
#define Physics_Analysis_KKFitResult_D0phi_KpiKK_H

/// @addtogroup BOSS_objects
/// @{

/**
 * @brief    Derived class for a container that contains important fit results of the `KalmanKinematicFit`, including masses.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 14th, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TrackSelector/KKFitResult.h"
	#include "TrackSelector/TrackSelector.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class KKFitResult_D0phi_KpiKK : public KKFitResult
{
public:
	// * CONSTRUCTORS * //
		KKFitResult_D0phi_KpiKK() : fFitMeasure(1e9) {}
		KKFitResult_D0phi_KpiKK(KalmanKinematicFit* kkmfit);

	// * INFORMATION * //
		bool IsBetter() const;

	// * PUBLIC DATA MEMBERS * //
		double fM_D0;   //!< Current computed mass of the \f$D^0\f$ candidate.
		double fM_Jpsi; //!< Current computed mass of the \f$J/\psi\f$ candidate.
		double fM_phi;  //!< Current computed mass of the \f$\phi\f$ candidate.
		double fP_D0;   //!< Current computed 3-momentum of \f$D^0\f$ candidate.
		double fP_phi;  //!< Current computed 3-momentum of \f$D^0\f$ candidate.
		double fFitMeasure;

private:
	void SetValues(KalmanKinematicFit* kkmfit);

};

/// @}


#endif