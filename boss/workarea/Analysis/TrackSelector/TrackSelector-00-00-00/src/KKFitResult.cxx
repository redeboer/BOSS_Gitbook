// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Vector/LorentzVector.h"
	#include "TrackSelector/KKFitResult.h"
	using CLHEP::HepLorentzVector;



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	double KKFitResult::fBestCompareValue = DBL_MAX;

	/// Constructor that immediately instantiates all its members from a `KalmanKinematic` fit result.
	KKFitResult::KKFitResult(KalmanKinematicFit* kkmfit) :
		fChiSquared(DBL_MAX),
		fFitMeasure(DBL_MAX),
		fFit(kkmfit),
		fHasResults(false)
	{
		if(!fFit) return;
		fChiSquared = fFit->chisq();
	}



// * =================================== * //
// * -------- PROTECTED METHODS -------- * //
// * =================================== * //


	/// Comparison method.
	bool KKFitResult::IsBetter(const double &value, double &bestvalue) const
	{
		/// -# Returns `false` if the object does not contain a `KalmanKinematicFit`.
		if(!fFit) return false;
		/// -# Returns `false` if its own `fCompareValue` worse (`>`) than `fBestCompareValue`.
		if(value > bestvalue) return false;
		/// -# If not, update `fBestCompareValue`.
		bestvalue = value;
		return true;
	}


	/// Default function of `KKFitResult::IsBetter(const double &value, double &bestvalue)`.
	/// Overwrite with your own implementation if necessary.
	bool KKFitResult::IsBetter() const
	{
		return KKFitResult::IsBetter(fFitMeasure, fBestCompareValue);
	}