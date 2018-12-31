// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Vector/LorentzVector.h"
	#include "TrackSelector/TrackSelector.h"
	#include "D0phi_KpiKK/KKFitResult_D0phi_KpiKK.h"

	using CLHEP::HepLorentzVector;
	using namespace TSGlobals;



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	double KKFitResult_D0phi_KpiKK::fBestCompareValue_D0  = 1e9;
	double KKFitResult_D0phi_KpiKK::fBestCompareValue_phi = 1e9;

	/**
	 * @brief
	 */
	KKFitResult_D0phi_KpiKK::KKFitResult_D0phi_KpiKK(KalmanKinematicFit* kkmfit) :
		fFitMeasure(1e9),
		fFitMeasure_D0(1e9),
		fFitMeasure_phi(1e9),
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
		/// -# Test whether `KalmanKinematicFit` pointer exists.
		if(!fFit) return;
		/// -# Get Lorentz vectors of the decay products:
		HepLorentzVector pKaonNeg1 =  fFit->pfit(0); /// * \f$K^-\f$ (first occurrence)
		HepLorentzVector pKaonNeg2 =  fFit->pfit(1); /// * \f$K^-\f$ (second occurrence)
		HepLorentzVector pKaonPos  =  fFit->pfit(2); /// * \f$K^+\f$
		HepLorentzVector pPionPos  =  fFit->pfit(3); /// * \f$\pi^+\f$
		/// -# Compute Lorentz vectors of the particles to be reconstructed:
		HepLorentzVector pD0   = pKaonNeg1 + pPionPos; /// * \f$D^0 \rightarrow K^-\pi^+\f$
		HepLorentzVector pphi  = pKaonNeg2 + pKaonPos; /// * \f$\phi \rightarrow K^-K^+\f$
		HepLorentzVector pJpsi = pD0 + pphi;           /// * \f$J/\psi \rightarrow D^0\phi\f$
		/// -# Compute invariant masses:
		fM_D0   = pD0.m();    /// * \f$M_{K^-\pi^+}\f$
		fM_phi  = pphi.m();   /// * \f$M_{K^-K^+}\f$
		fM_Jpsi = pJpsi.m();  /// * \f$M_{D^0\phi}\f$
		/// -# Compute measures for best fit:
		fFitMeasure_D0  = (fM_D0  - gM_D0 ) / gM_D0;    /// * `fFitMeasure_D0`  = \f$\frac{M_{K^-\pi^+} - m_{D^0} }{m_{D^0}   }\f$ (procentual difference with \f$m_{D^0}\f$)
		fFitMeasure_phi = (fM_phi - gM_phi) / gM_phi;   /// * `fFitMeasure_phi` = \f$\frac{M_{K^-K^+}   - m_{\phi}}{m_{\phi}  }\f$ (procentual difference with \f$m_{\phi}\f$)
		fFitMeasure = fFitMeasure_D0 * fFitMeasure_phi; /// * `fFitMeasure` = `fFitMeasure_D0 * fFitMeasure_phi` (product of procentual differences)
	}


	bool KKFitResult_D0phi_KpiKK::IsBetter()
	{
		return KKFitResult::IsBetter(fFitMeasure, fBestCompareValue);
	}


	bool KKFitResult_D0phi_KpiKK::IsBetter_D0()
	{
		return KKFitResult::IsBetter(fFitMeasure_D0, fBestCompareValue_D0);
	}


	bool KKFitResult_D0phi_KpiKK::IsBetter_phi()
	{
		return KKFitResult::IsBetter(fFitMeasure_D0, fBestCompareValue_D0);
	}