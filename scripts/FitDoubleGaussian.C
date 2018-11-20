/**
 * @brief    Simple script that shows how to fit a double gaussian on an invariant mass spectrum using `RooFit`.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 * @details  Note that this script makes use of one `hadd`ed file, not of a `TChain`. It is currently assumed that the invariant mass plot to be fitted comes from the `mpi0` branch in the `fit5c` that is produced by the `RhopiAlg` analysis job. But this can be modified in the sections that have been marked by `// ! ... ! //`.
 */


// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "../inc/FrameworkSettings.h"
	#include "../inc/ReconstructedParticle.h"
	#include "../inc/RhopiRootFile.h"
	#include "RooAddPdf.h"
	#include "RooDataHist.h"
	#include "RooGaussian.h"
	#include "RooPlot.h"
	#include "RooRealVar.h"
	#include "TCanvas.h"
	#include "TDatabasePDG.h"
	#include "TFile.h"
	#include "TH1D.h"
	#include "TLine.h"
	#include "TParticlePDG.h"
	#include "TPaveText.h"
	#include "TString.h"
	#include "TStyle.h"
	#include "TSystem.h"
	#include "TText.h"
	#include "TTree.h"
	#include <iostream>
	using namespace RooFit;
	using namespace AnaBranches;
	using namespace Settings;


// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //
/**
 * @brief Main function used when compiling and executing in `ROOT`.
 * @param inputFileName 
 */
void FitDoubleGaussian()
{
	// * Output file name declared here to avoid `Form` bug later * //
	// ! Set your parameters here ! //
		const char* inputFileName = "../data/root/ana_rhopi_data_0.root";
		const char* treeName = "fit5c";
		ReconstructedParticle particle(
			213, // the PDG code for pi0 is 111
			"#gamma#gamma"); // decay channel (use LaTeX!)
		if(!particle.GetParticlePDG()) return;
		const int numberOfBins = 500;

	// * Prepare input and output * //
		RhopiRootFile file(inputFileName);
		if(file.IsZombie()) return;
		const TString outputDirectory = Form("%s/%s", Output::PlotOutputDir, __FILE__);
		const TString outputFileName  = Form("%s/%s.%s", outputDirectory.Data(), particle.GetName(), Output::Extension);
		gSystem->mkdir(outputDirectory.Data(), kTRUE);

	// * Create invariant mass histogram * //
		TH1D hist("invmass_hist",
			Form("Invariant mass for %s candidate;#it{M}_{%s} (GeV/#it{c}^{2});counts",
				particle.GetNameLaTeX(), particle.GetDaughterLabel()),
			numberOfBins, particle.FitFrom(), particle.FitUntil()
		);

	// * Loop the tree to fill inv mass spectrums * //
	// ! You choose your branch and mass in this loop ! //
		TTree* tree = file.FindTree(treeName);
		if(!tree) return;
		Long64_t nEntries = tree->GetEntries();
		std::cout << "Looping over " << nEntries << " events in the \"" << treeName << "\" tree" << std::flush;
		for(Long64_t i = 0; i < nEntries; ++i) {
			tree->GetEntry(i);
			hist.Fill(fit5c::mrhop); //! set mass branch here
		}
		std::cout << "\rSuccesfully looped over " << nEntries << " events in the \"" << treeName << "\" tree" << std::endl;

	// * The `RooFit` method * //
	// NOTE: See https://root.cern.ch/roofit-20-minutes
		RooRealVar invMassVar("invMassVar",
			Form("#it{M}_{%s} (GeV/#it{c}^{2})", particle.GetDaughterLabel()),
			particle.FitFrom(),
			particle.FitUntil()
		);

	// * Import histogram to Roofit * //
		RooDataHist invMassDistribution(
			Form("%s_%s_RooDataHist", treeName, particle.GetName()), hist.GetTitle(),
			invMassVar, Import(hist));

	// * Create Gaussian functions * //
		RooRealVar mean(
			Form("m_{%s}", particle.GetNameLaTeX()),
			Form("%s mass", particle.GetNameLaTeX()),
			particle.GetMass(), particle.GetLowerMass(), particle.GetUpperMass());
		RooRealVar s1("#sigma_{1}", Form("%s width 1", particle.GetNameLaTeX()),
			particle.GetGaussianSmallWidth(), 0., 100.*particle.GetGaussianSmallWidth());
		RooRealVar s2("#sigma_{2}", Form("%s width 2", particle.GetNameLaTeX()),
			particle.GetGaussianWideWidth(), 0., 100.*particle.GetGaussianWideWidth());
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
		RooFitResult* result = doublegauss.fitTo(invMassDistribution);

	// * Fit, plot results, and save * //
		RooPlot *frame = invMassVar.frame(); // create a frame to draw
		frame->SetAxisRange(particle.PlotFrom(), particle.PlotUntil());
		invMassDistribution.plotOn(frame, // draw distribution
			LineWidth(2), LineColor(kBlue+2), LineWidth(1),
			MarkerColor(kBlue+2), MarkerSize(.5));
		doublegauss.plotOn(frame, LineWidth(2), LineColor(kBlack)); // draw sig+bck fit
		doublegauss.plotOn(frame, Components(gauss1), // draw gauss 1
			LineWidth(1), LineColor(kRed-4));
		doublegauss.plotOn(frame, Components(gauss2), // draw gauss 2
			LineWidth(1), LineColor(kBlue-4));
		doublegauss.paramOn(frame, Layout(.56, .98, .92));

	// * Write fitted histograms * //
		TCanvas c;
		c.SetBatch();
		frame->Draw();
		c.SaveAs(outputFileName.Data());
		c.Close();

}

/**
 * @brief Main function that is called when executing the executable compiled using e.g. `g++`.
 */
int main()
{
	// * Execute function * //
	FitDoubleGaussian();

	// * Default return value * //
	return 0;
}