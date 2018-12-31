#ifndef Analysis_KKFitResult_H
#define Analysis_KKFitResult_H

/**
 * @brief    Base class for a container that contains important fit results of the `KalmanKinematicFit`.
 * @details  This object is necessary, because `KalmanKinematicFit` allows only the existance of one instance (see for instance <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classKalmanKinematicFit.html#67eb34f5902be7b16ce75e60513a995a">here</a>  and the fact that `operator=` of `KalmanKinematicFit` has been made `private`). For each derived algorithm of `TrackSelector`, you should create a derived class of `KKFitResult` where you define how to compute invariant masses and define the measure for the best fit.
 *
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 31st, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "VertexFit/KalmanKinematicFit.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class KKFitResult
{
public:
	// * CONSTRUCTORS * //
		KKFitResult() : fChiSquared(1e9) {}
		KKFitResult(KalmanKinematicFit* kkmfit);

	// * INFORMATION * //
		bool HasFit() { return fFit; }
		virtual bool IsBetter() = 0;

	// * PUBLIC DATA MEMBERS * //
		double fChiSquared;

	// * STATIC DATA MEMBERS * //
		static double fBestCompareValue; //!< Current best value for the comparison. Note that it is automatically reset to the value set here when all (derived) instances of `KKFitResult` are destroyed.

protected:
	// * PROTECTED METHODS * //
		bool IsBetter(double &value, double &bestvalue);
		void SetValues(KalmanKinematicFit* kkmfit);

	// * PROTECTED DATA MEMBERS * //
		KalmanKinematicFit* fFit;
};

#endif