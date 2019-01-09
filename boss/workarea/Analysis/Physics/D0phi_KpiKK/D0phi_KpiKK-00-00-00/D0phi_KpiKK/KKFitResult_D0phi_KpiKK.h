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
		KKFitResult_D0phi_KpiKK() : fFitMeasure(1e9), fFitMeasure_D0(1e9), fFitMeasure_phi(1e9) {}
		KKFitResult_D0phi_KpiKK(KalmanKinematicFit* kkmfit);
	// * INFORMATION * //
		bool IsBetter();
		bool IsBetter_D0();
		bool IsBetter_phi();

	// * STATIC DATA MEMBERS * //
		static double fBestCompareValue_D0; //!< Current best value for the comparison with regard to \f$D^0\f$. See `KKFitResult::fBestCompareValue`.
		static double fBestCompareValue_phi; //!< Current best value for the comparison with regard to \f$\phi\f$. See `KKFitResult::fBestCompareValue`.

	// * PUBLIC DATA MEMBERS * //
		double fM_D0;   //!< Current computed mass of \f$D^0\f$.
		double fM_Jpsi; //!< Current computed mass of \f$J/\psi\f$.
		double fM_phi;  //!< Current computed mass of \f$\phi\f$.
		double fFitMeasure;
		double fFitMeasure_D0;
		double fFitMeasure_phi;

private:
	void SetValues(KalmanKinematicFit* kkmfit);

};

/// @}


#endif