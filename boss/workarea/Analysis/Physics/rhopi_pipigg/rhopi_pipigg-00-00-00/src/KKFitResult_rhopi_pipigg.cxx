// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Vector/LorentzVector.h"
	#include "TrackSelector/TrackSelector.h"
	#include "rhopi_pipigg/KKFitResult_rhopi_pipigg.h"
	#include <cmath>

	using CLHEP::HepLorentzVector;
	using namespace TSGlobals;



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //


	/// Construct a `KKFitResult_rhopi_pipigg` object based on a pointer to a `KalmanKinematicFit` object.
	KKFitResult_rhopi_pipigg::KKFitResult_rhopi_pipigg(KalmanKinematicFit* kkmfit) :
		KKFitResult(kkmfit)
	{
		SetValues(fFit);
	}


	/// Construct a `KKFitResult_rhopi_pipigg` object based on a pointer to a `KalmanKinematicFit` object.
	KKFitResult_rhopi_pipigg::KKFitResult_rhopi_pipigg(Event::McParticle* photon1, Event::McParticle* photon2, Event::McParticle* pionNeg, Event::McParticle* pionPos)
	{
		SetValues(photon1, photon2, pionNeg, pionPos);
	}



// * ============================ * //
// * ------- KKFITRESULTS ------- * //
// * ============================ * //


	/// Further specification of `KKFitResult::SetValues()` and a helper function for the constructor (hence `private` method).
	void KKFitResult_rhopi_pipigg::SetValues(KalmanKinematicFit* kkmfit)
	{
		/// <ol>
		/// <li> Test whether `KalmanKinematicFit` pointer exists.
			if(!fFit) return;
		/// <li> Get Lorentz vectors of the decay products using `KalmanKinematicFit::pfit`:
			SetValues(
				/// <ol>
				fFit->pfit(0), /// <li> \f$\pi^-\f$
				fFit->pfit(1), /// <li> \f$\pi^+\f$
				fFit->pfit(2), /// <li> \f$\gamma\f$ (first occurrence)
				fFit->pfit(3)  /// <li> \f$\gamma\f$ (second occurrence)
				/// </ol>
			);
		/// </ol>
	}


	/// Further specification of `KKFitResult::SetValues()` and a helper function for the constructor (hence `private` method). This one is used for the loop over MC truth.
	void KKFitResult_rhopi_pipigg::SetValues(
		Event::McParticle *photon1, 
		Event::McParticle *photon2, 
		Event::McParticle *pionNeg, 
		Event::McParticle *pionPos)
	{
		/// <ol>
		/// <li> Test whether all `Event::McParticle` pointers exist.
			if(!photon1) return;
			if(!photon2) return;
			if(!pionNeg) return;
			if(!pionPos) return;
		/// <li> Apply `SetValues` to the `initialFourMomentum` of these `Event::McParticle` pointers.
			SetValues(
				pionNeg->initialFourMomentum(),
				pionPos->initialFourMomentum(),
				photon1->initialFourMomentum(),
				photon2->initialFourMomentum()
			);
		/// </ol>
	}


	/// Helper function for the constructor (hence `private` method).
	void KKFitResult_rhopi_pipigg::SetValues(
		const HepLorentzVector &pPhoton1, 
		const HepLorentzVector &pPhoton2, 
		const HepLorentzVector &pPionMin, 
		const HepLorentzVector &pPionPos)
	{
		/// <ol>
		/// <li> Compute Lorentz vectors of the particles to be reconstructed:
			/// <ul>
			HepLorentzVector ppi0  = pPhoton1 + pPhoton2;  /// <li> \f$\pi^0 \rightarrow \gamma\gamma\f$
			HepLorentzVector prho0 = pPionMin + pPionPos;  /// <li> \f$\rho^+ \rightarrow \pi^-\pi^+\f$
			HepLorentzVector prhom = ppi0 + pPionMin;      /// <li> \f$\rho^- \rightarrow \pi^0\pi^-\f$
			HepLorentzVector prhop = ppi0 + pPionPos;      /// <li> \f$\rho^+ \rightarrow \pi^0\pi^+\f$
			HepLorentzVector pJpsiRho0 = prho0 + ppi0;     /// <li> \f$J/\psi \rightarrow \rho^0\pi^0\f$
			HepLorentzVector pJpsiRhom = prhom + pPionPos; /// <li> \f$J/\psi \rightarrow \rho^-\pi^+\f$
			HepLorentzVector pJpsiRhop = prhop + pPionMin; /// <li> \f$J/\psi \rightarrow \rho^+\pi^-\f$
			/// </ul>
		/// <li> Compute invariant masses and momentum.
			fM_pi0      = ppi0     .m();
			fM_rho0     = prho0    .m();
			fM_rhom     = prhom    .m();
			fM_rhop     = prhop    .m();
			fM_JpsiRho0 = pJpsiRho0.m();
			fM_JpsiRhom = pJpsiRhom.m();
			fM_JpsiRhop = pJpsiRhop.m();
		/// </ol>
	}