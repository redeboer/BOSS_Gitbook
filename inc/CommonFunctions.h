#ifndef Physics_Analysis_CommonFunctions_H
#define Physics_Analysis_CommonFunctions_H

/**
 * @brief   Header that is used to collect functions that can be used by different scripts.
 * @details The idea of sharing these functions is to standardise output.
 * @author  Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date    November 20th, 2018
 * @remark  @b DEVELOPMENTAL
 */


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "../inc/FrameworkSettings.h"
	#include "../inc/Particle.h"
	#include "../inc/ReconstructedParticle.h"
	#include "RooAddPdf.h"
	#include "RooBreitWigner.h"
	#include "RooCmdArg.h"
	#include "RooDataHist.h"
	#include "RooFFTConvPdf.h"
	#include "RooFitResult.h"
	#include "RooGaussian.h"
	#include "RooLinkedList.h"
	#include "RooPlot.h"
	#include "RooPolynomial.h"
	#include "RooRealVar.h"
	#include "TCanvas.h"
	#include "TH1.h"
	#include "TH1D.h"
	#include "TH2D.h"
	#include "TString.h"
	#include "TSystem.h"
	#include "TTree.h"
	#include <utility> // for shared pointers

// * ========================= * //
// * ------- FUNCTIONS ------- * //
// * ========================= * //
namespace CommonFunctions //!< Namespace that contains functions that you want to be accessible in any of the scripts.
{
	/**
	 * @brief Auxiliary function that you can used in other functions to check if a pointer that you fed it is a nullpointer.
	 */
	bool CheckIfNullPtr(void* ptr)
	{
		if(!ptr) {
			std::cout << "ERROR: fed a nullpointer" << std::endl;
			return true;
		}
		return false;
	}

	/**
	 * @brief Helper function that 
	 * @param c Canvas of which you want to set the logarithmic scales.
	 * @param logScale Which axes do you want to set? Just mention e.g. `"xy"` if you want to have the x and y axes in logaritmic scale. This option is not capital sensitive.
	 */
	void SetLogScale(TCanvas& c, TString logScale)
	{
		logScale.ToLower();
		if(logScale.Contains("x")) c.SetLogx();
		if(logScale.Contains("y")) c.SetLogy();
		if(logScale.Contains("z")) c.SetLogz();
	}

	/**
	 * @brief Generalisation of the procedure when looping over a `TTree`.
	 */
	template<typename FUNCTOR, typename ...Rest>
	void LoopTree(TTree* tree, FUNCTOR&& lambda, Rest&&... args)
	{
		if(CheckIfNullPtr(tree)) return;
		auto nEntries = tree->GetEntries();
		std::cout << "Looping over " << nEntries << " events in the \"" << tree->GetName() << "\" tree" << std::flush;
		for(auto i = 0; i < nEntries; ++i) {
			tree->GetEntry(i);
			lambda(args...);
		}
		std::cout << "\rSuccesfully looped over " << nEntries << " events in the \"" << tree->GetName() << "\" tree" << std::endl;
	}

	/**
	 * @brief Create a histogram object especially for invariant mass analysis.
	 */
	TH1D CreateInvariantMassHistogram(const ReconstructedParticle& particle, const int nBins = 200)
	{
		return TH1D(
			Form("hist_%s", particle.GetName()),
			Form("Invariant mass for %s candidate;#it{M}_{%s} (GeV/#it{c}^{2});counts",
				particle.GetNameLaTeX(), particle.GetDaughterLabel()),
			nBins, particle.PlotFrom(), particle.PlotUntil());
	}

	/**
	 * @brief Function that allows you to create and save a quick sketch of a `TTree` branch.
	 */
	void DrawAndSave(TTree* tree, const char* varexp, const char* selection, Option_t* option, TString logScale = "")
	{
		TCanvas c;
		SetLogScale(c, logScale);
		tree->Draw(varexp, selection, option);
		c.SaveAs(Form("%s/%s_%s", Settings::Output::PlotOutputDir.Data(), tree->GetName(), varexp));
	}

	/**
	 * @brief Auxiliary function that is used by the more specific `DrawAndSave` functions for `TH1D` and `TH2D`.
	 */
	void DrawAndSave(TH1 &hist, const char* saveas, Option_t* opt, TString logScale = "")
	{
		TCanvas c;
		SetLogScale(c, logScale);
		hist.Draw(opt);
		c.SaveAs(Form("%s/%s", Settings::Output::PlotOutputDir.Data(), saveas));
	}

	/**
	 * @brief Draw and save a 1D distribution (output folder is determined from `FrameworkSettings.h`).
	 */
	void DrawAndSave(TH1D &hist, const char* saveas, TString logScale = "")
	{
		DrawAndSave(hist, saveas, "ep", logScale);
	}

	/**
	 * @brief Draw and save a 2D distribution (output folder is determined from `FrameworkSettings.h`).
	 */
	void DrawAndSave(TH2D &hist, const char* saveas, TString logScale = "")
	{
		DrawAndSave(hist, saveas, "colz", logScale);
	}

	/**
	 * @brief Create a `RooRealVar` specifically for resonstructing a certain particle (`ReconstructedParticle`).
	 */
	RooRealVar CreateRooFitInvMassVar(const ReconstructedParticle& particle) {
		RooRealVar var(
			Form("#it{M}_{%s}", particle.GetDaughterLabel()),
			Form("#it{M}_{%s} (GeV/#it{c}^{2})", particle.GetDaughterLabel()),
			particle.PlotFrom(),
			particle.PlotUntil()
		);
		return var;
	}

	/**
	 * @brief Create a `RooDataHist` specifically for resonstructing a certain particle (`ReconstructedParticle`).
	 */
	RooDataHist CreateRooFitInvMassDistr(TH1D &hist, const RooRealVar &var, const ReconstructedParticle& particle) {
		RooDataHist distr(
			Form("%scandidate_RooDataHist", particle.GetName()),
			hist.GetTitle(), var, RooFit::Import(hist));
		return distr;
	}

	/**
	 * @brief The `DrawAndSaveRecursion` functions are necessary for `DrawAndSave`, which is a variadic template function.
	 */
	template<class... ARGS> void DrawAndSaveRecursion(Option_t* option, ARGS&&... args); // start recursion
	template<class TYPE, class... ARGS>
	void DrawAndSaveRecursion(Option_t* option, TYPE first, ARGS... args)
	{
		auto obj = dynamic_cast<TObject*>(first);
		if(obj) obj->Draw(option);
		DrawAndSaveRecursion(option, args...); // continue recursion
	}
	template<> void DrawAndSaveRecursion(Option_t* option) {} // end recursion

	/**
	 * @brief Function that allows you to draw and save any set of `TObject`s.
	 */
	template<class ...ARGS>
	void DrawAndSave(const char* filename, Option_t* option, const char* logScale, ARGS... args)
	{
		// * Create canvas * //
			TCanvas c;
			SetLogScale(c, logScale);
			c.SetBatch();
		// * Draw objects * //
			DrawAndSaveRecursion(option, args...);
		// * Save canvas * //
			const TString outputDir = Form("%s/%s", Settings::Output::PlotOutputDir.Data(), __BASE_FILE__);
			gSystem->mkdir(outputDir.Data());
			c.SaveAs(Form("%s/%s.%s", outputDir.Data(), filename, Settings::Output::Extension));
	}

	/**
	 * @brief Fit the sum of two Gaussian functions on a invariant mass distrubution. The mean of the two Gaussian is in both cases taken to be the mass of the particle to be reconstructed.
	 * @brief For a pure particle signal, that is, without backround <b>and</b> without a physical particle width, the width of the two Gaussians characterises the resolution of the detector.
	 * @details See https://root.cern.ch/roofit-20-minutes for an instructive tutorial.
	 * 
	 * @param hist Invariant mass histogram that you would like to fit
	 * @param particle Hypothesis particle: which particle are you reconstructing? All analysis parameters, such as estimates for Gaussian widths, are contained within this object.
	 * @param numPolynomials The degree of the polynomial that describes the background.
	 */
	void FitDoubleGaussian(TH1D &hist, const ReconstructedParticle& particle, const UChar_t numPolynomials = 0)
	{
		// * Data members * //
			// * FitObject
				std::unique_ptr<RooDataHist> fRooDataHist;
				std::unique_ptr<RooRealVar>  fRooRealVar;
			// * FitObjectDoubleGaussian
				RooArgList fBckParameters;
				RooArgList fComponents;
				RooArgList fNContributions;
				std::unique_ptr<RooAddPdf>     fFullShape;
				std::unique_ptr<RooGaussian>   fGaussian1;
				std::unique_ptr<RooGaussian>   fGaussian2;
				std::unique_ptr<RooPolynomial> fPolBackground;
				std::unique_ptr<RooRealVar>    fMean;
				std::unique_ptr<RooRealVar>    fNGauss1;
				std::unique_ptr<RooRealVar>    fNGauss2;
				std::unique_ptr<RooRealVar>    fSigToBckRatio;
				std::unique_ptr<RooRealVar>    fSigma1;
				std::unique_ptr<RooRealVar>    fSigma2;

		if(true) {
			// * Create RooFit variable and data distribution * //
				fRooRealVar  = std_fix::make_unique<RooRealVar>(CreateRooFitInvMassVar(particle));
				fRooDataHist = std_fix::make_unique<RooDataHist>(CreateRooFitInvMassDistr(hist, *fRooRealVar, particle));

			// * Create Gaussian functions * //
				fMean = std_fix::make_unique<RooRealVar>(
					Form("m_{%s}", particle.GetNameLaTeX()),
					Form("%s mass", particle.GetNameLaTeX()),
					particle.GetMass(), particle.GetLowerMass(), particle.GetUpperMass());
				fSigma1 = std_fix::make_unique<RooRealVar>("#sigma_{1}",
					Form("%s width 1", particle.GetNameLaTeX()),
					particle.GetGaussianSmallWidth(),
					Settings::Fit::fSigmaScaleFactorLow * particle.GetGaussianSmallWidth(),
					Settings::Fit::fSigmaScaleFactorUp  * particle.GetGaussianSmallWidth());
				fSigma2 = std_fix::make_unique<RooRealVar>("#sigma_{2}",
					Form("%s width 2", particle.GetNameLaTeX()),
					particle.GetGaussianWideWidth(),
					Settings::Fit::fSigmaScaleFactorLow * particle.GetGaussianWideWidth(),
					Settings::Fit::fSigmaScaleFactorUp  * particle.GetGaussianWideWidth());
				fGaussian1 = std_fix::make_unique<RooGaussian>("gauss1",
					Form("Gaussian PDF 1 for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
					*fRooRealVar, *fMean, *fSigma1);
				fGaussian2 = std_fix::make_unique<RooGaussian>("gauss2",
					Form("Gaussian PDF 2 for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
					*fRooRealVar, *fMean, *fSigma2);
				fNGauss1 = std_fix::make_unique<RooRealVar>("N_{gaus1}", "N_{gaus1}", 1e2, 0., 1e6);
				fNGauss2 = std_fix::make_unique<RooRealVar>("N_{gaus2}", "N_{gaus2}", 1e4, 0., 1e6);
				fComponents.add(*fGaussian1);
				fComponents.add(*fGaussian2);
				fNContributions.add(*fNGauss1);
				fNContributions.add(*fNGauss2);

			// * Add polynomial background if required * //
				for(UChar_t i = 0; i <= numPolynomials; ++i) {
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

			// * Add the components and fit * //
				fFullShape = std_fix::make_unique<RooAddPdf>("full_shape", "Double gaussian + background", fComponents, fNContributions);
				fFullShape->fitTo(
					*fRooDataHist,
					RooFit::Range(particle.FitFrom(), particle.FitUntil()));

			// * Plot results and save * //
				auto frame = fRooRealVar->frame(); // create a frame to draw
				frame->SetAxisRange(particle.PlotFrom(), particle.PlotUntil());
				fRooDataHist->plotOn(frame, // draw distribution
					RooFit::LineWidth(2), RooFit::LineColor(kBlue+2), RooFit::LineWidth(1),
					RooFit::MarkerColor(kBlue+2), RooFit::MarkerSize(.5));
				fFullShape->plotOn(frame, RooFit::LineWidth(2), RooFit::LineColor(kBlack)); // draw sig+bck fit
				fFullShape->plotOn(frame, RooFit::Components(*fComponents.at(0)), // draw gauss 1
					RooFit::LineWidth(1), RooFit::LineColor(kRed-4));
				fFullShape->plotOn(frame, RooFit::Components(*fComponents.at(1)), // draw gauss 2
					RooFit::LineWidth(1), RooFit::LineColor(kBlue-4));
				if(numPolynomials) {
					fFullShape->plotOn(frame, RooFit::Components(*fPolBackground), // draw background
						RooFit::LineStyle(kDashed), RooFit::LineWidth(1), RooFit::LineColor(kGray));
				}
				fFullShape->paramOn(frame, RooFit::Layout(.56, .98, .92));

			// * Write fitted histograms * //
				DrawAndSave(
					Form("DoubleGauss_%s", particle.GetName()), "", "",
					frame);
				delete frame;
		}

	}

	/**
	 * @brief Fit the sum of two Gaussian functions on a invariant mass distrubution. The mean of the two Gaussian is in both cases taken to be the mass of the particle to be reconstructed.
	 * @brief For a pure particle signal, that is, without backround <b>and</b> without a physical particle width, the width of the two Gaussians characterises the resolution of the detector.
	 * @details See https://root.cern.ch/roofit-20-minutes for an instructive tutorial.
	 * 
	 * @param hist Invariant mass histogram that you would like to fit
	 * @param particle Hypothesis particle: which particle are you reconstructing? All analysis parameters, such as estimates for Gaussian widths, are contained within this object.
	 * @param numPolynomials The degree of the polynomial that describes the background.
	 */
	void FitBreitWigner(TH1D &hist, const ReconstructedParticle& particle, const UChar_t numPolynomials = 0)
	{

		// * Create RooFit variable and data distribution * //
			RooRealVar fRooRealVar = CreateRooFitInvMassVar(particle);
			RooDataHist fRooDataHist = CreateRooFitInvMassDistr(hist, fRooRealVar, particle);

		// * Create Breit-Wigner function and fit * //
			RooRealVar mean(
				Form("m_{%s}", particle.GetNameLaTeX()),
				Form("%s mass", particle.GetNameLaTeX()),
				particle.GetMass(), particle.GetLowerMass(), particle.GetUpperMass());
			RooRealVar width("width", Form("%s width", particle.GetNameLaTeX()),
				particle.GetBWPureWidth(), 0., 100.*particle.GetBWPureWidth());
			RooGaussian signal("breitwigner",
				Form("Breit-Wigner PDF for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
				fRooRealVar, mean, width);
			RooRealVar n("N_{BW}", "N_{BW}", 1e2, 0., 1e6);
			RooArgList fComponents(signal);
			RooArgList fNContributions(n);

		// * Add polynomial background if required * //
			RooArgList fBckParameters;
			for(UChar_t i = 0; i <= numPolynomials; ++i) {
				auto p = new RooRealVar(Form("p%u", i), Form("p%u", i), 0., -1e6, 1e6);
				fBckParameters.add(*p);
			}
			RooPolynomial fPolBackground("polBkg",
				Form("Polynomial-%u background", numPolynomials),
				fRooRealVar, fBckParameters);
			RooRealVar fSigToBckRatio(
				Form("N_{pol%u}", numPolynomials),
				Form("N_{pol%u}", numPolynomials),
				0., 0., 1e5);
			if(numPolynomials) {
				fComponents.add(fPolBackground);
				fNContributions.add(fSigToBckRatio);
			}

		// * Add the components and fit * //
			RooAddPdf fFullShape("full_shape", "Breit-Wigner + background", fComponents, fNContributions);
			fFullShape.fitTo(
				fRooDataHist,
				RooFit::Range(particle.FitFrom(), particle.FitUntil()));

		// * Plot results and save * //
			RooPlot *frame = fRooRealVar.frame(); // create a frame to draw
			frame->SetAxisRange(particle.PlotFrom(), particle.PlotUntil());
			fRooDataHist.plotOn(frame, // draw distribution
				RooFit::LineWidth(2), RooFit::LineColor(kBlue+2), RooFit::LineWidth(1),
				RooFit::MarkerColor(kBlue+2), RooFit::MarkerSize(.5));
			fFullShape.plotOn(frame, RooFit::LineWidth(2), RooFit::LineColor(kBlack));
			if(numPolynomials) {
				fFullShape.plotOn(frame, RooFit::Components(signal), // draw Breit-Wigner
					RooFit::LineWidth(1), RooFit::LineColor(kRed-4));
				fFullShape.plotOn(frame, RooFit::Components(fPolBackground), // draw background
					RooFit::LineStyle(kDashed), RooFit::LineWidth(1), RooFit::LineColor(kGray));
			}
			fFullShape.paramOn(frame, RooFit::Layout(.56, .98, .92));

		// * Write fitted histograms * //
			TCanvas c;
			c.SetBatch();
			frame->Draw();
			const TString outputDir = Form("%s/%s", Settings::Output::PlotOutputDir.Data(), __BASE_FILE__);
			gSystem->mkdir(outputDir.Data());
			c.SaveAs(Form("%s/BreitWigner_%s.%s", outputDir.Data(), particle.GetName(), Settings::Output::Extension));
			c.Close();

	}

	/**
	 * @brief Fit the sum of two Gaussian functions on a invariant mass distrubution. The mean of the two Gaussian is in both cases taken to be the mass of the particle to be reconstructed.
	 * @brief For a pure particle signal, that is, without backround <b>and</b> without a physical particle width, the width of the two Gaussians characterises the resolution of the detector.
	 * @details See https://root.cern.ch/roofit-20-minutes for an instructive tutorial.
	 * @param hist Invariant mass histogram that you would like to fit
	 * @param particle Hypothesis particle: which particle are you reconstructing? All analysis parameters, such as estimates for Gaussian widths, are contained within this object.
	 * @param numPolynomials The degree of the polynomial that describes the background.
	 */
	void FitBWDoubleGaussianConvolution(TH1D &hist, const ReconstructedParticle& particle, const UChar_t numPolynomials = 0)
	{

		// * Create RooFit variable and data distribution * //
			RooRealVar fRooRealVar = CreateRooFitInvMassVar(particle);
			RooDataHist fRooDataHist = CreateRooFitInvMassDistr(hist, fRooRealVar, particle);

		// * Create double Gaussian function * //
			RooRealVar m0("GaussianMeanZero", "GaussianMeanZero", 0.);
			RooRealVar fSigma1("#sigma_{1}", Form("%s width 1", particle.GetNameLaTeX()),
				particle.GetGaussianSmallWidth()); //! width is fixed
			RooRealVar fSigma2("#sigma_{2}", Form("%s width 2", particle.GetNameLaTeX()),
				particle.GetGaussianWideWidth()); //! width is fixed
			RooGaussian fGaussian1("gauss1",
				Form("Gaussian PDF 1 for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
				fRooRealVar, m0, fSigma1);
			RooGaussian fGaussian2("gauss2",
				Form("Gaussian PDF 2 for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
				fRooRealVar, m0, fSigma2);
			RooRealVar fNGauss1("N_{gaus1}", "N_{gaus1}", 1e2, 0., 1e6);
			RooRealVar fNGauss2("N_{gaus2}", "N_{gaus2}", 1e4, 0., 1e6);
			RooRealVar ratio("N_{gaus1} / N_{gaus2}", "Ratio between the two Gaussian pdfs", .8, 0., 1.);
			RooAddPdf doublegauss("double_gaussian", "Double gaussian",
				RooArgList(fGaussian1, fGaussian2), RooArgList(ratio));

		// * Add the Gaussian components * //

		// * Create Breit-Wigner/Cauchy distribution * //
			RooRealVar mean(
				Form("m_{%s}", particle.GetNameLaTeX()),
				Form("%s mass", particle.GetNameLaTeX()),
				particle.GetMass(), particle.GetLowerMass(), particle.GetUpperMass());
			RooRealVar width("#sigma_{BW}", Form("%s BW width", particle.GetNameLaTeX()),
				particle.GetBWConvolutedWidth(), 0., 10.*particle.GetBWConvolutedWidth());
			RooGaussian bw("breitwigner",
				Form("Breit-Wigner PDF for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
				fRooRealVar, mean, width);

		// * Convolute * //
			RooFFTConvPdf signal("convolution", "convolution", fRooRealVar, bw, doublegauss);
			RooRealVar n("N_{gaus1}", "N_{gaus1}", 1e2, 0., 1e6);
			RooArgList fComponents(signal);
			RooArgList fNContributions(n);

		// * Add polynomial background if required * //
			RooArgList fBckParameters;
			for(UChar_t i = 0; i <= numPolynomials; ++i) {
				auto p = new RooRealVar(Form("p%u", i), Form("p%u", i), 0., -1e6, 1e6);
				fBckParameters.add(*p);
			}
			RooPolynomial fPolBackground("polBkg",
				Form("Polynomial-%u background", numPolynomials),
				fRooRealVar, fBckParameters);
			RooRealVar fSigToBckRatio(
				Form("N_{pol%u}", numPolynomials),
				Form("N_{pol%u}", numPolynomials),
				0., 0., 1e5);
			if(numPolynomials) {
				fComponents.add(fPolBackground);
				fNContributions.add(fSigToBckRatio);
			}

		// * Add the components and fit * //
			RooAddPdf fFullShape("full_shape", "Double gaussian + background", fComponents, fNContributions);
			fFullShape.fitTo(
				fRooDataHist,
				RooFit::Range(particle.FitFrom(), particle.FitUntil()));

		// * Plot results and save * //
			RooPlot *frame = fRooRealVar.frame(); // create a frame to draw
			frame->SetAxisRange(particle.PlotFrom(), particle.PlotUntil());
			fRooDataHist.plotOn(frame, // draw distribution
				RooFit::LineWidth(2), RooFit::LineColor(kBlue+2), RooFit::LineWidth(1),
				RooFit::MarkerColor(kBlue+2), RooFit::MarkerSize(.5));
			fFullShape.plotOn(frame, RooFit::LineWidth(2), RooFit::LineColor(kBlack));
			if(numPolynomials) {
				fFullShape.plotOn(frame, RooFit::Components(signal), // draw signal
					RooFit::LineWidth(1), RooFit::LineColor(kRed-4));
				fFullShape.plotOn(frame, RooFit::Components(fPolBackground), // draw background
					RooFit::LineStyle(kDashed), RooFit::LineWidth(1), RooFit::LineColor(kGray));
			}
			fFullShape.paramOn(frame, RooFit::Layout(.56, .98, .92));

		// * Write fitted histograms * //
			TCanvas c;
			c.SetBatch();
			frame->Draw();
			const TString outputDir = Form("%s/%s", Settings::Output::PlotOutputDir.Data(), __BASE_FILE__);
			gSystem->mkdir(outputDir.Data());
			c.SaveAs(Form("%s/Convolution_%s.%s", outputDir.Data(), particle.GetName(), Settings::Output::Extension));
			c.Close();

	}

} // namespace CommonFunctions

#endif