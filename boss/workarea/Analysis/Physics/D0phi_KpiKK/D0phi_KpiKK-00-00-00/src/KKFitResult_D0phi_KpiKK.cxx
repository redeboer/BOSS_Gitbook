// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Vector/LorentzVector.h"
	#include "TrackSelector/TrackSelector.h"
	#include "D0phi_KpiKK/KKFitResult_D0phi_KpiKK.h"
	#include <cmath>

	using CLHEP::HepLorentzVector;
	using namespace TSGlobals;



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/// Construct a `KKFitResult_D0phi_KpiKK` object based on a pointer to a `KalmanKinematicFit` object.
	KKFitResult_D0phi_KpiKK::KKFitResult_D0phi_KpiKK(KalmanKinematicFit* kkmfit) :
		KKFitResult(kkmfit)
	{
		SetValues(fFit);
	}

	/// Construct a `KKFitResult_D0phi_KpiKK` object based on a pointer to a `KalmanKinematicFit` object.
	KKFitResult_D0phi_KpiKK::KKFitResult_D0phi_KpiKK(Event::McParticle* kaonNeg1, Event::McParticle* kaonNeg2, Event::McParticle* kaonPos, Event::McParticle* pionPos)
	{
		SetValues(kaonNeg1, kaonNeg2, kaonPos, pionPos);
	}



// * ============================ * //
// * ------- KKFITRESULTS ------- * //
// * ============================ * //

	/// Helper function for the constructor (hence `private` method).
	void KKFitResult_D0phi_KpiKK::SetValues(KalmanKinematicFit* kkmfit)
	{
		/// <ol>
		/// <li> Test whether `KalmanKinematicFit` pointer exists.
			if(!fFit) return;
		/// <li> Get Lorentz vectors of the decay products using `KalmanKinematicFit::pfit`:
			/// <ol>
			SetValues(
				fFit->pfit(0), /// <li> \f$K^-\f$ (first occurrence)
				fFit->pfit(1), /// <li> \f$K^-\f$ (second occurrence)
				fFit->pfit(2), /// <li> \f$K^+\f$
				fFit->pfit(3)  /// <li> \f$\pi^+\f$
			);
			/// </ol>
		/// </ol>
	}

	/// Helper function for the constructor (hence `private` method).
	void KKFitResult_D0phi_KpiKK::SetValues(
		Event::McParticle *kaonNeg1, 
		Event::McParticle *kaonNeg2, 
		Event::McParticle *kaonPos, 
		Event::McParticle *pionPos)
	{
		/// <ol>
		/// <li> Test whether all `Event::McParticle` pointers exist.
		if(!kaonNeg1) return;
		if(!kaonNeg2) return;
		if(!kaonPos)  return;
		if(!pionPos)  return;
		/// <li> Apply `SetValues` to the `initialFourMomentum` of these `Event::McParticle` pointers.
		SetValues(
			kaonNeg1->initialFourMomentum(),
			kaonNeg2->initialFourMomentum(),
			kaonPos ->initialFourMomentum(),
			pionPos ->initialFourMomentum());
		/// </ol>
	}

	/// Helper function for the constructor (hence `private` method).
	void KKFitResult_D0phi_KpiKK::SetValues(
		const HepLorentzVector &pKaonNeg1, 
		const HepLorentzVector &pKaonNeg2, 
		const HepLorentzVector &pKaonPos, 
		const HepLorentzVector &pPionPos)
	{
		/// <ol>
		/// <li> Compute Lorentz vectors of the particles to be reconstructed:
			/// <ul>
			HepLorentzVector pD0   = pKaonNeg1 + pPionPos; /// <li> \f$D^0 \rightarrow K^-\pi^+\f$
			HepLorentzVector pphi  = pKaonNeg2 + pKaonPos; /// <li> \f$\phi \rightarrow K^-K^+\f$
			HepLorentzVector pJpsi = pD0 + pphi;           /// <li> \f$J/\psi \rightarrow D^0\phi\f$
			/// </ul>
		/// <li> Compute invariant masses and momentum:
			/// <ul>
			fM_D0   = pD0.m();    /// <li> `fM_D0`   = \f$M_{K^-\pi^+}\f$
			fM_phi  = pphi.m();   /// <li> `fM_phi`  = \f$M_{K^-K^+}\f$
			fM_Jpsi = pJpsi.m();  /// <li> `fM_Jpsi` = \f$M_{D^0\phi}\f$
			fP_D0   = std::sqrt(pD0 .px()*pD0 .px() + pD0 .py()*pD0 .py() + pD0 .pz()*pD0 .pz());  /// <li> `fP_D0`  = \f$|\vec{p}_{K^-\pi^+}|\f$
			fP_phi  = std::sqrt(pphi.px()*pphi.px() + pphi.py()*pphi.py() + pphi.pz()*pphi.pz());  /// <li> `fP_phi` = \f$|\vec{p}_{K^-K^+}|\f$
			/// </ul>
		/// <li> Compute measure for best fit: `fFitMeasure` := \f$M_{K^-K^+} - m_{\phi}\f$
		fFitMeasure = std::abs(fM_phi - gM_phi);
		/// </ol>
	}