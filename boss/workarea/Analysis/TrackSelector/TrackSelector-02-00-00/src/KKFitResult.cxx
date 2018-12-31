// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Vector/LorentzVector.h"
	#include "TrackSelector/KKFitResult.h"
	using CLHEP::HepLorentzVector;



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	double KKFitResult::fBestCompareValue = 1e9;

	/**
	 * @brief
	 */
	KKFitResult::KKFitResult(KalmanKinematicFit* kkmfit) :
		fChiSquared(1e9),
		fFit(kkmfit)
	{
		SetValues(fFit);
	}



// * =================================== * //
// * -------- PROTECTED METHODS -------- * //
// * =================================== * //

	/**
	 * @brief Constructof for the `KKFitResult` base class.
	 */
	void KKFitResult::SetValues(KalmanKinematicFit* kkmfit)
	{
		if(!kkmfit) return;
		fChiSquared = fFit->chisq();
	}


	/**
	 * @brief Comparison method.
	 */
	bool KKFitResult::IsBetter(double &value, double &bestvalue)
	{
		/// # Returns `false` if the object does not contain a `KalmanKinematicFit`.
		if(!fFit) return false;
		/// # Returns `false` if its own `fCompareValue` worse than `fBestCompareValue`.
		if(value > bestvalue) return false;
		/// # If not, update `fBestCompareValue`
		bestvalue = value;
		return true;
	}