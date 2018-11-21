#ifndef Physics_Analysis_CommonFunctions_H
#define Physics_Analysis_CommonFunctions_H

/**
 * @brief       Header that is used to collect functions that can be used by different scripts.
 * @description The idea of sharing these functions is to standardise output.
 * @author      Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date        November 20th, 2018
 * @remark      @b DEVELOPMENTAL
 */


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "../inc/FrameworkSettings.h"
	#include "../inc/Particle.h"
	#include "../inc/ReconstructedParticle.h"
	#include "RooAddPdf.h"
	#include "RooBreitWigner.h"
	#include "RooDataHist.h"
	#include "RooFFTConvPdf.h"
	#include "RooFitResult.h"
	#include "RooGaussian.h"
	#include "RooPlot.h"
	#include "RooRealVar.h"
	#include "TCanvas.h"
	#include "TH1.h"
	#include "TH1D.h"
	#include "TH2D.h"
	#include "TString.h"
	#include "TSystem.h"
	#include "TTree.h"


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
	 * @brief Create a histogram object.
	 */
	void SetInvariantMassHistogram(TH1D& hist, const ReconstructedParticle& particle, const int nBins = 200)
	{
		hist = TH1D(
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
	 * @brief Fit the sum of two Gaussian functions on a invariant mass distrubution. The mean of the two Gaussian is in both cases taken to be the mass of the particle to be reconstructed.
	 * @brief For a pure particle signal, that is, without backround <b>and</b> without a physical particle width, the width of the two Gaussians characterises the resolution of the detector.
	 * @details See https://root.cern.ch/roofit-20-minutes for an instructive tutorial.
	 * @param hist Invariant mass histogram that you would like to fit
	 * @param particle Hypothesis particle: which particle are you reconstructing? All analysis parameters, such as estimates for Gaussian widths, are contained within this object.
	 */
	void FitDoubleGaussian(TH1D &hist, const ReconstructedParticle& particle)
	{
		
		// * The `RooFit` method * //
			RooRealVar invMassVar("invMassVar",
				Form("#it{M}_{%s} (GeV/#it{c}^{2})", particle.GetDaughterLabel()),
				particle.PlotFrom(),
				particle.PlotUntil()
			);

		// * Import histogram to Roofit * //
			RooDataHist invMassDistribution(
				Form("%scandidate_RooDataHist", particle.GetName()), hist.GetTitle(),
				invMassVar, RooFit::Import(hist));

		// * Create Gaussian functions * //
			RooRealVar mean(
				Form("m_{%s}", particle.GetNameLaTeX()),
				Form("%s mass", particle.GetNameLaTeX()),
				particle.GetMass(), particle.GetLowerMass(), particle.GetUpperMass());
			RooRealVar s1("#sigma_{1}", Form("%s width 1", particle.GetNameLaTeX()),
				particle.GetGaussianSmallWidth(), 0., 10.*particle.GetGaussianSmallWidth());
			RooRealVar s2("#sigma_{2}", Form("%s width 2", particle.GetNameLaTeX()),
				particle.GetGaussianWideWidth(), 0., 10.*particle.GetGaussianWideWidth());
			RooGaussian gauss1("gauss1",
				"Gaussian PDF 1 for #it{M}_{#gamma#gamma} distribution",
				invMassVar, mean, s1);
			RooGaussian gauss2("gauss2",
				"Gaussian PDF 2 for #it{M}_{#gamma#gamma} distribution",
				invMassVar, mean, s2);

		// * Add the components and fit * //
			RooRealVar ratio("n_{gaus1} / n_{gaus2}", "Ratio between the two Gaussian pdfs", .8, 0., 1.);
			RooAddPdf  doublegauss("double_gaussian", "Double gaussian",
				RooArgList(gauss1, gauss2), RooArgList(ratio));
			doublegauss.fitTo(
				invMassDistribution,
				RooFit::Range(particle.FitFrom(), particle.FitUntil()));

		// * Plot results and save * //
			RooPlot *frame = invMassVar.frame(); // create a frame to draw
			frame->SetAxisRange(particle.PlotFrom(), particle.PlotUntil());
			invMassDistribution.plotOn(frame, // draw distribution
				RooFit::LineWidth(2), RooFit::LineColor(kBlue+2), RooFit::LineWidth(1),
				RooFit::MarkerColor(kBlue+2), RooFit::MarkerSize(.5));
			doublegauss.plotOn(frame, RooFit::LineWidth(2), RooFit::LineColor(kBlack)); // draw sig+bck fit
			doublegauss.plotOn(frame, RooFit::Components(gauss1), // draw gauss 1
				RooFit::LineWidth(1), RooFit::LineColor(kRed-4));
			doublegauss.plotOn(frame, RooFit::Components(gauss2), // draw gauss 2
				RooFit::LineWidth(1), RooFit::LineColor(kBlue-4));
			doublegauss.paramOn(frame, RooFit::Layout(.56, .98, .92));

		// * Write fitted histograms * //
			TCanvas c;
			c.SetBatch();
			frame->Draw();
			const TString outputDir = Form("%s/%s", Settings::Output::PlotOutputDir.Data(), __BASE_FILE__);
			gSystem->mkdir(outputDir.Data());
			c.SaveAs(Form("%s/DoubleGauss_%s.%s", outputDir.Data(), particle.GetName(), Settings::Output::Extension));
			c.Close();

	}

	/**
	 * @brief Fit the sum of two Gaussian functions on a invariant mass distrubution. The mean of the two Gaussian is in both cases taken to be the mass of the particle to be reconstructed.
	 * @brief For a pure particle signal, that is, without backround <b>and</b> without a physical particle width, the width of the two Gaussians characterises the resolution of the detector.
	 * @details See https://root.cern.ch/roofit-20-minutes for an instructive tutorial.
	 * @param hist Invariant mass histogram that you would like to fit
	 * @param particle Hypothesis particle: which particle are you reconstructing? All analysis parameters, such as estimates for Gaussian widths, are contained within this object.
	 */
	void FitBreitWigner(TH1D &hist, const ReconstructedParticle& particle)
	{

		// * The `RooFit` method * //
			RooRealVar invMassVar("invMassVar",
				Form("#it{M}_{%s} (GeV/#it{c}^{2})", particle.GetDaughterLabel()),
				particle.PlotFrom(),
				particle.PlotUntil()
			);

		// * Import histogram to Roofit * //
			RooDataHist invMassDistribution(
				Form("%scandidate_RooDataHist", particle.GetName()), hist.GetTitle(),
				invMassVar, RooFit::Import(hist));

		// * Create Breit-Wigner function and fit * //
			RooRealVar mean(
				Form("m_{%s}", particle.GetNameLaTeX()),
				Form("%s mass", particle.GetNameLaTeX()),
				particle.GetMass(), particle.GetLowerMass(), particle.GetUpperMass());
			RooRealVar width("width", Form("%s width", particle.GetNameLaTeX()),
				particle.GetBWPureWidth(), 0., 100.*particle.GetBWPureWidth());
			RooGaussian bw("breitwigner",
				Form("Breit-Wigner PDF for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
				invMassVar, mean, width);
			bw.fitTo(
				invMassDistribution,
				RooFit::Range(particle.FitFrom(), particle.FitUntil()));

		// * Plot results and save * //
			RooPlot *frame = invMassVar.frame(); // create a frame to draw
			frame->SetAxisRange(particle.PlotFrom(), particle.PlotUntil());
			invMassDistribution.plotOn(frame, // draw distribution
				RooFit::LineWidth(2), RooFit::LineColor(kBlue+2), RooFit::LineWidth(1),
				RooFit::MarkerColor(kBlue+2), RooFit::MarkerSize(.5));
			bw.plotOn(frame, RooFit::LineWidth(2), RooFit::LineColor(kBlack));
			bw.paramOn(frame, RooFit::Layout(.56, .98, .92));

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
	 */
	void FitBWDoubleGaussianConvolution(TH1D &hist, const ReconstructedParticle& particle)
	{
		
		// * The `RooFit` method * //
			RooRealVar invMassVar("invMassVar",
				Form("#it{M}_{%s} (GeV/#it{c}^{2})", particle.GetDaughterLabel()),
				particle.PlotFrom(),
				particle.PlotUntil()
			);

		// * Import histogram to Roofit * //
			RooDataHist invMassDistribution(
				Form("%scandidate_RooDataHist", particle.GetName()), hist.GetTitle(),
				invMassVar, RooFit::Import(hist));

		// * Create Gaussian functions * //
			RooRealVar m0("GaussianMeanZero", "GaussianMeanZero", 0.);
			RooRealVar s1("#sigma_{1}", Form("%s width 1", particle.GetNameLaTeX()), particle.GetGaussianSmallWidth());
			RooRealVar s2("#sigma_{2}", Form("%s width 2", particle.GetNameLaTeX()), particle.GetGaussianWideWidth());
			RooGaussian gauss1("gauss1",
				Form("Gaussian PDF 1 for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
				invMassVar, m0, s1);
			RooGaussian gauss2("gauss2",
				Form("Gaussian PDF 2 for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
				invMassVar, m0, s2);

		// * Add the Gaussian components * //
			RooRealVar ratio("n_{gaus1} / n_{gaus2}", "Ratio between the two Gaussian pdfs", .8, 0., 1.);
			RooAddPdf  doublegauss("double_gaussian", "Double gaussian",
				RooArgList(gauss1, gauss2), RooArgList(ratio));
			doublegauss.fitTo(
				invMassDistribution,
				RooFit::Range(particle.FitFrom(), particle.FitUntil()));

		// * Create Breit-Wigner/Cauchy distribution * //
			RooRealVar mean(
				Form("m_{%s}", particle.GetNameLaTeX()),
				Form("%s mass", particle.GetNameLaTeX()),
				particle.GetMass(), particle.GetLowerMass(), particle.GetUpperMass());
			RooRealVar width("#sigma_{BW}", Form("%s BW width", particle.GetNameLaTeX()),
				particle.GetBWConvolutedWidth(), 0., 10.*particle.GetBWConvolutedWidth());
			RooGaussian bw("breitwigner",
				Form("Breit-Wigner PDF for #it{M}_{%s} distribution", particle.GetDaughterLabel()),
				invMassVar, mean, width);

		// * Convolute and fit * //
			RooFFTConvPdf signal("signal", "signal", invMassVar, bw, doublegauss);
			signal.fitTo(invMassDistribution,
				RooFit::Range(particle.FitFrom(), particle.FitUntil()));

		// * Plot results and save * //
			RooPlot *frame = invMassVar.frame(); // create a frame to draw
			frame->SetAxisRange(particle.PlotFrom(), particle.PlotUntil());
			invMassDistribution.plotOn(frame, // draw distribution
				RooFit::LineWidth(2), RooFit::LineColor(kBlue+2), RooFit::LineWidth(1),
				RooFit::MarkerColor(kBlue+2), RooFit::MarkerSize(.5));
			signal.plotOn(frame, RooFit::LineWidth(2), RooFit::LineColor(kBlack));
			signal.paramOn(frame, RooFit::Layout(.56, .98, .92));

		// * Write fitted histograms * //
			TCanvas c;
			c.SetBatch();
			frame->Draw();
			const TString outputDir = Form("%s/%s", Settings::Output::PlotOutputDir.Data(), __BASE_FILE__);
			gSystem->mkdir(outputDir.Data());
			c.SaveAs(Form("%s/Convolution_%s.%s", outputDir.Data(), particle.GetName(), Settings::Output::Extension));
			c.Close();

	}
}

#endif