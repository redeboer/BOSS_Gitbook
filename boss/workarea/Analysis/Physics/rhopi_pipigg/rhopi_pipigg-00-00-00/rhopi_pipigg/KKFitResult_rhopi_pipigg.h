#ifndef Physics_Analysis_KKFitResult_rhopi_pipigg_H
#define Physics_Analysis_KKFitResult_rhopi_pipigg_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/KKFitResult.h"
	#include "TrackSelector/TrackSelector.h"
	#include "McTruth/McParticle.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_objects
/// @{

	/// Derived class for a container that contains important fit results of the `KalmanKinematicFit`, including masses.
	/// @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date     December 14th, 2018
	class KKFitResult_rhopi_pipigg : public KKFitResult
	{
	public:
		/// @name Constructor
		///@{
			KKFitResult_rhopi_pipigg() {}
			KKFitResult_rhopi_pipigg(KalmanKinematicFit* kkmfit);
			KKFitResult_rhopi_pipigg(Event::McParticle* photon1, Event::McParticle* photon2, Event::McParticle* pionNeg, Event::McParticle* pionPos);
		///@}


		/// @name Parameters computed from fit
			///@{
			double fM_pi0;      ///< Current computed mass of the \f$\pi^0\f$ candidate.
			double fM_rho0;     ///< Current computed mass of the \f$\rho^0\f$ candidate.
			double fM_rhom;     ///< Current computed mass of the \f$\rho^-\f$ candidate.
			double fM_rhop;     ///< Current computed mass of the \f$\rho^+\f$ candidate.
			double fM_JpsiRho0; ///< Current computed mass of the \f$J/\psi \rightarrow \rho^0\pi^0\f$ candidate.
			double fM_JpsiRhom; ///< Current computed mass of the \f$J/\psi \rightarrow \rho^-\pi^+\f$ candidate.
			double fM_JpsiRhop; ///< Current computed mass of the \f$J/\psi \rightarrow \rho^+\pi^-\f$ candidate.
			///@}


	private:
		/// @name Setters
			///@{
			void SetValues(const HepLorentzVector &pPhoton1, const HepLorentzVector &pPhoton2, const HepLorentzVector &pPionNeg, const HepLorentzVector &pPionPos);
			void SetValues(Event::McParticle *photon1, Event::McParticle *photon2, Event::McParticle *pionPos, Event::McParticle *pionPos);
			///@}


	};


/// @}
#endif