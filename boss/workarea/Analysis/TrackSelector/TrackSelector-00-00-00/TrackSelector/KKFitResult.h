#ifndef Analysis_KKFitResult_H
#define Analysis_KKFitResult_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "VertexFit/KalmanKinematicFit.h"



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
			KKFitResult() : fChiSquared(1e9), fFitMeasure(1e9) {}
			KKFitResult(KalmanKinematicFit* kkmfit);
			///@}


		/// @name Boolean tests
			///@{
			explicit operator bool() const { return fFit; }
			bool operator!() const { return !fFit; }
			virtual bool IsBetter() const;
			///@}


		/// @name Fit characteristics
			///@{
			double fChiSquared; ///< \f$\chi^2\f$ of <i>this</i> fit (hence, not `static`).
			double fFitMeasure; ///< Measure that can be used to compare fits. @remark Usage depends completely on your implementation in the derived class.
			static double fBestCompareValue; ///< Current best value for the comparison. Note that it is automatically reset to the value set here when all (derived) instances of `KKFitResult` are destroyed.
			///@}


	protected:
		/// @name Boolean tests
			///@{
			bool IsBetter(const double &value, double &bestvalue) const;
			///@}


		/// @name Setters
			///@{
			void SetValues();
			///@}


		/// @name Fit characteristics
			///@{
			KalmanKinematicFit* fFit;
			///@}


	};


/// @}
#endif