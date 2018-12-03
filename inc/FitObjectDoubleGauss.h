#ifndef Physics_Analysis_FitObjectDoubleGauss_H
#define Physics_Analysis_FitObjectDoubleGauss_H

/**
 * @brief    Container for objects necessary for fitting invariant mass spectra.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 26th, 2018
 * @details  This object only needs an invariant mass spectrum and a `TReconstructedParticle` object of the particle that you want to reconstruct. `RooFit` objects will then be automatically loaded in the constructor.
 * @remark   @b DEVELOPMENTAL
 */


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TString.h"
	#include "../inc/FitObject.h"
	#include "RooGaussian.h"
	#include "RooPolynomial.h"
	#include "RooRealVar.h"
	#include <iostream>


// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class FitObjectDoubleGauss : public FitObject
{
public:
	// * Constructor and destructors *
	FitObjectDoubleGauss(TH1D& hist, const ReconstructedParticle& particle);

	// * Setters *

	// * Getters *

protected:
	// * Data members *
	std::unique_ptr<RooRealVar> fMean; //!< Mean of the <b>two</b> Gaussians when centered around the mass
	std::unique_ptr<RooRealVar> fMean0; //!< Mean of the <b>two</b> Gaussians when centered around \f M_\text{inv} = 0 \f. This is useful in a convolution.
	std::unique_ptr<RooRealVar> fSigma1; //!< Width of the (prferably) narrow Gaussian of the two.
	std::unique_ptr<RooRealVar> fSigma2; //!< Width of the (prferably) wide Gaussian of the two.
	std::unique_ptr<RooGaussian> fGaussian1; //!< Narrow Gaussian.
	std::unique_ptr<RooGaussian> fGaussian2; //!< Wide Gaussian.

	// * Private methods *
	std::unique_ptr<RooRealVar> BuildSigmaVariable(const UChar_t number, const double& width);
	std::unique_ptr<RooGaussian> BuildGaussianFunction(const bool wide, const bool mean0 = false);
	void Initialize();
	void SetMassParameter();
	void SetMassParameterAroundZero();
	void SetSignalArguments();

};


// * =========================================== * //
// * ------- CONSTRUCTORS AND DESTRUCTORS ------ * //
// * =========================================== * //

	/**
	 * @brief Construct particle based on its code in the PDG.
	 */
	FitObjectDoubleGauss::FitObjectDoubleGauss(TH1D& hist, const ReconstructedParticle& particle) :
		FitObject(hist, particle)
	{
		Initialize();
	}


// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //




// * ======================= * //
// * ------- GETTERS ------- * //
// * ======================= * //




// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //

	/**
	 * @brief Build a `RooRealVar` object that resembles a sigma of a Gaussian shape (i.e. has the proper name, title, and boundaries).
	 */
	std::unique_ptr<RooRealVar> FitObjectDoubleGauss::BuildSigmaVariable(const UChar_t num, const double& width)
	{
		return std_fix::make_unique<RooRealVar>(
			Form("#sigma_{%u}", num),
			Form("%s width %u", fParticle.GetNameLaTeX(), num),
			fParticle.GetGaussianSmallWidth(),
			width * Settings::Fit::fSigmaScaleFactorLow,
			width * Settings::Fit::fSigmaScaleFactorUp);
	}

	void FitObjectDoubleGauss::SetMassParameter()
	{
		fMean = std_fix::make_unique<RooRealVar>(
			Form("m_{%s}", fParticle.GetNameLaTeX()),
			Form("%s mass", fParticle.GetNameLaTeX()),
			fParticle.GetMass(), fParticle.GetLowerMass(), fParticle.GetUpperMass());
	}

	void FitObjectDoubleGauss::SetMassParameterAroundZero()
	{
		fMean0 = std_fix::make_unique<RooRealVar>("double_gauss_mean", "Double Gaussian mean set to 0", 0.);
	}

	std::unique_ptr<RooGaussian> FitObjectDoubleGauss::BuildGaussianFunction(const bool wide, const bool mean0)
	{
		// * Check existense of all objects * //
		if(!fMean || !fMean || !fSigma1 || !fSigma2 ) return nullptr;
		// * Construct parameters based on boolean input * //
		UChar_t num = 1;
		auto mean  = fMean.get();
		auto sigma = fSigma1.get();
		if(mean0) mean = fMean0.get();
		if(wide) {
			num = 2;
			sigma = fSigma2.get();
		}
		// * Return properly formatted RooGaussian object * //
		return std_fix::make_unique<RooGaussian>(
			Form("gauss%u", num),
			Form("Gaussian PDF %u for #it{M}_{%s} distribution", num, fParticle.GetDaughterLabel()),
			fRooRealVar, *mean, *sigma);
	}

	/**
	 * @brief Encapsulation of what any constructor for this object needs to do.
	 */
	void FitObjectDoubleGauss::Initialize()
	{
		SetMassParameter();
		SetMassParameterAroundZero();
		fSigma1 = BuildSigmaVariable(1, fParticle.GetGaussianSmallWidth());
		fSigma2 = BuildSigmaVariable(2, fParticle.GetGaussianWideWidth());
		fGaussian1 = BuildGaussianFunction(false); // narrow
		fGaussian2 = BuildGaussianFunction(true);  // wide
		fComponents.add(fGaussian1);
		fComponents.add(fGaussian2);
		fRatios.addClone(n1);
		fRatios.addClone(n2);
		RooRealVar n1("N_{gaus1}", "N_{gaus1}", 1e2, 0., 1e6);
		RooRealVar n2("N_{gaus2}", "N_{gaus2}", 1e4, 0., 1e6);
	}
			RooRealVar n1("N_{gaus1}", "N_{gaus1}", 1e2, 0., 1e6);
			RooRealVar n2("N_{gaus2}", "N_{gaus2}", 1e4, 0., 1e6);
			RooArgList compon(g1, g2);
			RooArgList ratios(n1, n2);

		// * Add polynomial background if required * //
			RooArgList parameters;
			for(UChar_t i = 0; i <= numPolynomials; ++i) {
				auto p = new RooRealVar(Form("p%u", i), Form("p%u", i), 0., -1e6, 1e6);
				parameters.add(*p);
			}
			RooPolynomial polBackground("polBkg",
				Form("Polynomial-%u background", numPolynomials),
				invMassVar, parameters);
			RooRealVar sbratio(
				Form("N_{pol%u}", numPolynomials),
				Form("N_{pol%u}", numPolynomials),
				0., 0., 1e5);
			if(numPolynomials) {
				compon.add(polBackground);
				ratios.add(sbratio);
			}

		// * Add the components and fit * //
			RooAddPdf fullShape("full_shape", "Double gaussian + background", compon, ratios);
			fullShape.fitTo(
				invMassDistribution,
				RooFit::Range(particle.FitFrom(), particle.FitUntil()));

		// * Plot results and save * //
			RooPlot *frame = invMassVar.frame(); // create a frame to draw
			frame->SetAxisRange(particle.PlotFrom(), particle.PlotUntil());
			invMassDistribution.plotOn(frame, // draw distribution
				RooFit::LineWidth(2), RooFit::LineColor(kBlue+2), RooFit::LineWidth(1),
				RooFit::MarkerColor(kBlue+2), RooFit::MarkerSize(.5));
			fullShape.plotOn(frame, RooFit::LineWidth(2), RooFit::LineColor(kBlack)); // draw sig+bck fit
			fullShape.plotOn(frame, RooFit::Components(g1), // draw gauss 1
				RooFit::LineWidth(1), RooFit::LineColor(kRed-4));
			fullShape.plotOn(frame, RooFit::Components(g2), // draw gauss 2
				RooFit::LineWidth(1), RooFit::LineColor(kBlue-4));
			if(numPolynomials) {
				fullShape.plotOn(frame, RooFit::Components(polBackground), // draw background
					RooFit::LineStyle(kDashed), RooFit::LineWidth(1), RooFit::LineColor(kGray));
			}
			fullShape.paramOn(frame, RooFit::Layout(.56, .98, .92));

		// * Write fitted histograms * //
			TCanvas c;
			c.SetBatch();
			frame->Draw();
			const TString outputDir = Form("%s/%s", Settings::Output::PlotOutputDir.Data(), __BASE_FILE__);
			gSystem->mkdir(outputDir.Data());
			c.SaveAs(Form("%s/DoubleGauss_%s.%s", outputDir.Data(), particle.GetName(), Settings::Output::Extension));
			c.Close();

	}

#endif