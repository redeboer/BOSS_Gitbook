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
	void SetDaughterLabel(const char* daughters);

	// * Getters *
	const double GetLowerMass() const;
	const double GetUpperMass() const;
	const double GetMassOffsetPercentage() const;
	const double FitFrom() const;
	const double FitUntil() const;
	const double PlotFrom() const;
	const double PlotUntil() const;
	const double GetBWPureWidth() const;
	const double GetBWConvolutedWidth() const;
	const double GetGaussianSmallWidth() const;
	const double GetGaussianWideWidth() const;
	const std::pair<double, double> GetDoubleGaussianWidths() const;
	const std::pair<double, double> GetDoubleGaussianRatio() const;
	const std::pair<double, double> GetFitRange() const;
	const std::pair<double, double> GetPlotRange() const;
	void GetDoubleGaussianWidths(double& from, double& to) const;
	void GetFitRange(double& from, double& to) const;
	void GetPlotRange(double& from, double& to) const;
	const char* GetDaughterLabel() const;

protected:
	// * Data members *
	TString fDaughterLabels; //!< LaTeX formatted string for decay particles.
	double fMassOffset; //!< Percentage (divided by 100) that the mean (namely, the mass) may vary.
	double fBWPureWidth; //!< Estimate for the width of the Breit-Wigner function when fitting BW only.
	double fBWConvolutedWidth; //!< Estimate for the width of the Breit-Wigner function when convoluted with a double Gaussian.
	std::pair<double, double> fDoubleGaussianWidths; //!< Pair of two sigma values. You can use that as estimates of the widths for the double gaussian that you plan to fit. These sigmas are supposed to characterise the resolution of the detector. For consistency in naming, the first one should be smaller than the second.
	std::pair<double, double> fFitRange; //!< Invariant mass range over which you fit a function (double Gaussian, Crystal ball, Breit-Wigner, etc.).
	std::pair<double, double> fPlotRange; //!< Invariant mass range that you plot.

	// * Private methods *
	void Initialize();
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
	 * @brief Encapsulation of what any constructor for this object needs to do.
	 */
	void FitObjectDoubleGauss::Initialize()
	{
	}

	/**
	 * @brief Fit the sum of two Gaussian functions on a invariant mass distrubution. The mean of the two Gaussian is in both cases taken to be the mass of the particle to be reconstructed.
	 * @brief For a pure particle signal, that is, without backround <b>and</b> without a physical particle width, the width of the two Gaussians characterises the resolution of the detector.
	 * @details See https://root.cern.ch/roofit-20-minutes for an instructive tutorial.
	 * @param hist Invariant mass histogram that you would like to fit
	 * @param particle Hypothesis particle: which particle are you reconstructing? All analysis parameters, such as estimates for Gaussian widths, are contained within this object.
	 */
	void FitObjectDoubleGauss::SetSignalArguments()
	{

		// * Create function of two added Gaussian functions * //
			RooRealVar mean(
				Form("m_{%s}", fParticle.GetNameLaTeX()),
				Form("%s mass", fParticle.GetNameLaTeX()),
				fParticle.GetMass(), fParticle.GetLowerMass(), fParticle.GetUpperMass());
			RooRealVar s1("#sigma_{1}", Form("%s width 1", fParticle.GetNameLaTeX()),
				fParticle.GetGaussianSmallWidth(), 0., 10.*fParticle.GetGaussianSmallWidth());
			RooRealVar s2("#sigma_{2}", Form("%s width 2", fParticle.GetNameLaTeX()),
				fParticle.GetGaussianWideWidth(), 0., 10.*fParticle.GetGaussianWideWidth());
			RooGaussian g1("gauss1",
				Form("Gaussian PDF 1 for #it{M}_{%s} distribution", fParticle.GetDaughterLabel()),
				fRooRealVar, mean, s1);
			RooGaussian g2("gauss2",
				Form("Gaussian PDF 2 for #it{M}_{%s} distribution", fParticle.GetDaughterLabel()),
				fRooRealVar, mean, s2);
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