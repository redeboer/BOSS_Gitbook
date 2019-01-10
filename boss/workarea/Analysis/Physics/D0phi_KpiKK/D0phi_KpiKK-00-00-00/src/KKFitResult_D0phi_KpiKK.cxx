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

	/**
	 * @brief
	 */
	KKFitResult_D0phi_KpiKK::KKFitResult_D0phi_KpiKK(KalmanKinematicFit* kkmfit) :
		fFitMeasure(1e9),
		KKFitResult(kkmfit)
	{
		SetValues(fFit);
	}



// * ============================ * //
// * ------- KKFITRESULTS ------- * //
// * ============================ * //

	/**
	 * @brief Constructof for the `KKFitResult` base class.
	 */
	void KKFitResult_D0phi_KpiKK::SetValues(KalmanKinematicFit* kkmfit)
	{
		/// <ol>
		/// <li> Test whether `KalmanKinematicFit` pointer exists.
		if(!fFit) return;
		/// <li> Get Lorentz vectors of the decay products:
			/// <ol>
			HepLorentzVector pKaonNeg1 =  fFit->pfit(0); /// <li> \f$K^-\f$ (first occurrence)
			HepLorentzVector pKaonNeg2 =  fFit->pfit(1); /// <li> \f$K^-\f$ (second occurrence)
			HepLorentzVector pKaonPos  =  fFit->pfit(2); /// <li> \f$K^+\f$
			HepLorentzVector pPionPos  =  fFit->pfit(3); /// <li> \f$\pi^+\f$
			/// </ol>
		/// <li> Compute Lorentz vectors of the particles to be reconstructed:
			/// <ol>
			HepLorentzVector pD0   = pKaonNeg1 + pPionPos; /// <li> \f$D^0 \rightarrow K^-\pi^+\f$
			HepLorentzVector pphi  = pKaonNeg2 + pKaonPos; /// <li> \f$\phi \rightarrow K^-K^+\f$
			HepLorentzVector pJpsi = pD0 + pphi;           /// <li> \f$J/\psi \rightarrow D^0\phi\f$
			/// </ol>
		/// <li> Compute invariant masses and momentum:
			fM_D0   = pD0.m();    /// <li> `fM_D0`   = \f$M_{K^-\pi^+}\f$
			fM_phi  = pphi.m();   /// <li> `fM_phi`  = \f$M_{K^-K^+}\f$
			fM_Jpsi = pJpsi.m();  /// <li> `fM_Jpsi` = \f$M_{D^0\phi}\f$
			fP_D0   = std::sqrt(pD0 .px()*pD0 .px() + pD0 .py()*pD0 .py() + pD0 .pz()*pD0 .pz());  /// <li> `fP_D0`  = \f$|\vec{p}_{K^-\pi^+}|\f$
			fP_phi  = std::sqrt(pphi.px()*pphi.px() + pphi.py()*pphi.py() + pphi.pz()*pphi.pz());  /// <li> `fP_phi` = \f$|\vec{p}_{K^-K^+}|\f$
		/// <li> Compute measure for best fit: `fFitMeasure` := \f$M_{K^-K^+} - m_{\phi}\f$
		fFitMeasure = std::abs(fM_phi - gM_phi);
		/// </ol>
	}


	bool KKFitResult_D0phi_KpiKK::IsBetter() const
	{
		return KKFitResult::IsBetter(fFitMeasure, fBestCompareValue);
	}