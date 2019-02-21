#ifndef BOSS_Afterburner_FitObject_H
#define BOSS_Afterburner_FitObject_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "FrameworkSettings.h"
	#include "ReconstructedParticle.h"
	#include "RooAddPdf.h"
	#include "RooDataHist.h"
	#include "RooRealVar.h"
	#include "TH1D.h"
	#include <iostream>
	#include <utility>


// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_Afterburner
/// @{


	/// This is a base class for fit specific objects that you use for fitting invariant mass spectra.
	/// It provides its derived classes with the members and methods necessary to load the spectrum and the `ReconstructedParticle` object of the particle that you want to reconstruct. Basic `RooFit` objects such as `RooDataHist` that are needed when fitting whatever the function you are fitting are initialised through this class as well.
	///
	/// @author  Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date    November 26th, 2018
	/// @remark  @b DEVELOPMENTAL
	class FitObject
	{
	public:
		// * Constructor and destructors *
		FitObject(TH1D& hist, const ReconstructedParticle& particle, const UChar_t nPol = 0);
		~FitObject();

		// * Setters *
		void AddPolynomialBackground(UChar_t nPol = 2);
		void PerformFit();

		// * Getters *
		bool IsLoaded();

	protected:
		// * Data members *
		ReconstructedParticle fParticle; ///< The particle that you are trying to reconstruct. <b>All fitting parameters should be described in this object!</b>
		std::unique_ptr<RooDataHist> fRooDataHist; ///< `RooFit` distribution as constructed from the 'imported' histogram.
		RooRealVar fRooRealVar; ///< Variable used by `RooFit` to perform fit on.
		RooArgList fSigArgs;
		RooArgList fBckParameters;
		RooArgList fBckArgs;
		RooArgList fComponents;
		RooArgList fNContributions;
		std::unique_ptr<RooAddPdf>     fFullShape; ///< Background and signal added up.
		std::unique_ptr<RooGaussian>   fGaussian1; ///< Narrow Gaussian.
		std::unique_ptr<RooGaussian>   fGaussian2; ///< Wide Gaussian.
		std::unique_ptr<RooPolynomial> fPolBackground;
		std::unique_ptr<RooRealVar>    fMean; ///< Mean of the <b>two</b> Gaussians when centered around the mass
		std::unique_ptr<RooRealVar>    fMean0; ///< Mean of the <b>two</b> Gaussians when centered around \f$ M_{\mathrm{inv}} = 0 \f$. This is useful in a convolution.
		std::unique_ptr<RooRealVar>    fNGauss1;
		std::unique_ptr<RooRealVar>    fNGauss2;
		std::unique_ptr<RooRealVar>    fSigToBckRatio;
		std::unique_ptr<RooRealVar>    fSigma1; ///< Width of the (prferably) narrow Gaussian of the two.
		std::unique_ptr<RooRealVar>    fSigma2; ///< Width of the (prferably) wide Gaussian of the two.

		UChar_t fNPolynomial;

		// * Private methods *
		void Initialize(TH1D& hist);
		void SetRooRealVar();
		void SetInvMassDistr(TH1D& hist);
		void SetSignalArguments();
		void BuildFullShape();
		void BuildDoubleGaussian();
		void AddPolynomialBackground(const UChar_t nPol = 2);

	};



/// @}
// * =========================================== * //
// * ------- CONSTRUCTORS AND DESTRUCTORS ------ * //
// * =========================================== * //

	/// Construct particle based on its code in the PDG.
	FitObject::FitObject(TH1D& hist, const ReconstructedParticle& particle, const UChar_t nPol) :
		fParticle(particle), fNPolynomial(nPol)
	{
		Initialize(hist);
		AddPolynomialBackground(nPol);
		BuildFullShape();
	}



// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //

	/// Perform fit if `RooFit` objects have been loaded.
	void FitObject::PerformFit()
	{
		if(fFullShape) {
			fFullShape->fitTo(
				fRooDataHist,
				RooFit::Range(fParticle.FitFrom(), fParticle.FitUntil()));
		}
	}

	void FitObject::AddPolynomialBackground(UChar_t nPol)
	{
		for(UChar_t i = 0; i <= fNPolynomial; ++i) {
			fBckParameters.addClone(RooRealVar(Form("p%u", i), Form("p%u", i), 0., -1e6, 1e6));
		}
		fPolBackground = std::make_unique<RooPolynomial>("polBkg",
			Form("Polynomial-%u background", numPolynomials),
			*fRooRealVar, fBckParameters);
		fSigToBckRatio = std_fix::make_unique<RooRealVar>(
			Form("N_{pol%u}", numPolynomials),
			Form("N_{pol%u}", numPolynomials),
			0., 0., 1e5);
		if(numPolynomials) {
			fComponents.add(*fPolBackground);
			fNContributions.add(*fSigToBckRatio);
		}
	}

	/// Build a `RooFit` function that adds up the signal and the background.
	void FitObject::BuildFullShape()
	{
		fFullShape = std_fix::make_unique<RooAddPdf>("full_shape", "Double gaussian + background", fComponents, fNContributions);
		fFullShape->fitTo(
			*fRooDataHist,
			RooFit::Range(fParticle.FitFrom(), fParticle.FitUntil()));
	}



// * ======================= * //
// * ------- GETTERS ------- * //
// * ======================= * //

	/// Check if a particle has been loaded into the `FitObject`.
	bool FitObject::IsLoaded()
	{
		return fParticle.IsLoaded();
	}


// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //


	/// Create and set `RooRealVar` data member specifically for resonstructing a certain particle (`ReconstructedParticle`). This data member will be used to as a variable in the `RooFit` procedure
	void FitObject::SetRooRealVar() {
		if(fParticle.IsLoaded()) {
			fRooRealVar = RooRealVar(
				Form("#it{M}_{%s}", fParticle.GetDaughterLabel()),
				Form("#it{M}_{%s} (GeV/#it{c}^{2})", fParticle.GetDaughterLabel()),
				fParticle.PlotFrom(),
				fParticle.PlotUntil()
			);
		}
	}


	/// Create a `RooDataHist` specifically for resonstructing a certain particle (`ReconstructedParticle`).
	/// @param hist Invariant mass histogram to bne analysed.
	void FitObject::SetInvMassDistr(TH1D& hist) {
		if(fParticle.IsLoaded()) {
			RooDataHist distr(
				Form("%scandidate_RooDataHist", fParticle.GetName()),
				histGetTitle(), fRooRealVar, RooFit::Import(hist));
		}
	}


	/// Create a `RooFit` component for a double gaussian, with it's center around the reconstructed particle mass.
	void FitObject::BuildDoubleGaussian()
	{
		fMean = std_fix::make_unique<RooRealVar>(
			Form("m_{%s}", fParticle.GetNameLaTeX()),
			Form("%s mass", fParticle.GetNameLaTeX()),
			fParticle.GetMass(), fParticle.GetLowerMass(), fParticle.GetUpperMass());
		fSigma1 = std_fix::make_unique<RooRealVar>("#sigma_{1}",
			Form("%s width 1", fParticle.GetNameLaTeX()),
			fParticle.GetGaussianSmallWidth(),
			Settings::Fit::gSigmaScaleFactorLow * fParticle.GetGaussianSmallWidth(),
			Settings::Fit::gSigmaScaleFactorUp  * fParticle.GetGaussianSmallWidth());
		fSigma2 = std_fix::make_unique<RooRealVar>("#sigma_{2}",
			Form("%s width 2", fParticle.GetNameLaTeX()),
			fParticle.GetGaussianWideWidth(),
			Settings::Fit::gSigmaScaleFactorLow * fParticle.GetGaussianWideWidth(),
			Settings::Fit::gSigmaScaleFactorUp  * fParticle.GetGaussianWideWidth());
		fGaussian1 = std_fix::make_unique<RooGaussian>("gauss1",
			Form("Gaussian PDF 1 for #it{M}_{%s} distribution", fParticle.GetDaughterLabel()),
			*fRooRealVar, *fMean, *fSigma1);
		fGaussian2 = std_fix::make_unique<RooGaussian>("gauss2",
			Form("Gaussian PDF 2 for #it{M}_{%s} distribution", fParticle.GetDaughterLabel()),
			*fRooRealVar, *fMean, *fSigma2);
		fNGauss1 = std_fix::make_unique<RooRealVar>("N_{gaus1}", "N_{gaus1}", 1e2, 0., 1e6);
		fNGauss2 = std_fix::make_unique<RooRealVar>("N_{gaus2}", "N_{gaus2}", 1e4, 0., 1e6);
		fComponents.add(*fGaussian1);
		fComponents.add(*fGaussian2);
		fNContributions.add(*fNGauss1);
		fNContributions.add(*fNGauss2);
	}


	/// Auxiliary function that allows you to share functionality among constructors.
	void FitObject::Initialize(TH1D& hist)
	{
		SetRooRealVar();
		SetInvMassDistr(hist);
		fSigArgs.takeOwnership();
		fBckArgs.takeOwnership();
	}



#endif