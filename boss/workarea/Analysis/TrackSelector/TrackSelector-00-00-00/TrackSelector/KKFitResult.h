#ifndef Analysis_KKFitResult_H
#define Analysis_KKFitResult_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "VertexFit/KalmanKinematicFit.h"
	#include <float.h> // for DBL_MAX



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_objects
/// @{

	/// Base class for a container that contains important fit results of the `KalmanKinematicFit`.
	/// This object is necessary, because `KalmanKinematicFit` allows only the existance of one instance (see for instance <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classKalmanKinematicFit.html#67eb34f5902be7b16ce75e60513a995a">here</a>  and the fact that `operator=` of `KalmanKinematicFit` has been made `private`). For each derived algorithm of `TrackSelector`, you should create a derived class of `KKFitResult` where you define how to compute invariant masses and define the measure for the best fit.
	/// @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date     December 31st, 2018
	class KKFitResult
	{
	public:
		/// @name Constructor
			///@{
			KKFitResult(KalmanKinematicFit* kkmfit=nullptr);
			///@}


		/// @name Boolean tests
			///@{
			bool HasResults() { return fHasResults; }
			explicit operator bool() const { return fHasResults; }
			virtual bool IsBetter() const;
			///@}


		/// @name Setters
			///@{
			void ResetBestCompareValue(const double value=DBL_MAX) { fBestCompareValue = value; };
			///@}


		/// @name Parameters computed from fit
			///@{
			double fChiSquared; ///< \f$\chi^2\f$ of <i>this</i> fit (hence, not `static`).
			///@}


	protected:
		/// @name Boolean tests
			///@{
			bool IsBetter(const double &value, double &bestvalue) const;
			///@}


		/// @name Fit characteristics
			///@{
			double fFitMeasure; ///< Measure that can be used to compare fits. @remark Usage depends completely on your implementation in the derived class.
			static double fBestCompareValue; ///< Current best value for the comparison. Note that it is automatically reset to the value set here when all (derived) instances of `KKFitResult` are destroyed.
			KalmanKinematicFit* fFit;
			bool fHasResults; ///< You will only want to write the fit results if they have been properly computed. This bit is set to `true` if either this object has been constructed using a non-empty `KalmanKinematicFit` pointer or has been constructed with (see derived classes for implementation)
			///@}


	};


/// @}
#endif