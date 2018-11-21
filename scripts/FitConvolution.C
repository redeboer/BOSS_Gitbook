/**
 * @brief    Simple script that shows how to fit a double gaussian convoluted with a Breit-Wigner / Cauchy function on an invariant mass spectrum using `RooFit`.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 * @details  Note that this script makes use of one `hadd`ed file, not of a `TChain`. It is currently assumed that the invariant mass plot to be fitted comes from the `mpi0` branche in the `fit4c` that is produced by the `RhopiAlg` analysis job. But this can be modified in the section `GLOBALS`.
 * @remark   This script was designed to show that fitting in `RooFit` is not that straightforward: if you apply the straightforward method that is applied here, the fit result is not as good as the ordinary `TH1::Fit` procedure (compare the `FitDoubleGaussian_TH1Fit.C` script).
 */


// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "RooAddPdf.h"
	#include "RooBreitWigner.h"
	#include "RooDataHist.h"
	#include "RooFFTConvPdf.h"
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


// * ======================= * //
// * ------- GLOBALS ------- * //
// * ======================= * //
	// ! Do not change these parametes ! //
	const TDatabasePDG gPDG;
		//!< A `ROOT` `TDatabasePDG` object that contains all info of particles. Has to be constructed once, which is why it is a global.
	// ! Customise these values ! //
	const TParticlePDG* gParticlePDG = gPDG.GetParticle(111);
		//!< The particle that you want to analyse. (111 is the PDG code for pi0.
	const char* gDefaultFileToLoad = "../data/root/ana_rhopi_data_0.root";
		//!< Location of the ROOT file that you want to analyse.
	const char* gTreeToLoad        = "fit4c";
		//!< `TTree` that you want to loop over.
	const char* gBrancheToLoad     = "mpi0";
		//!< Branche in the `TTree` that you want to use to create the invariant mass plot.
	const char* gCandidate         = "#pi^{0}";
		//!< Decay particle string that will be used in the invariant mass plot labels. Use LaTeX here (see <a href="https://root.cern.ch/doc/master/classTLatex.html">`TLaTeX`</a>).
	const char* gDaughters         = "#gamma#gamma";
		//!< Daughter particle string that will be used in the invariant mass plot labels. Use LaTeX here (see <a href="https://root.cern.ch/doc/master/classTLatex.html">`TLaTeX`</a>).
	const char* gOutputExtension   = "pdf";
		//!< Extension that is used to save the output plots.
		//!< !! Note that it has to be set before the `gOutputDir` !!
	const char* gOutputDir         = Form("../plots/%s/script_output/%s", gOutputExtension, __FILE__);
		//!< Directory to which the resulting output files will be written (format determined by `gOutputExtension`). Preferably write to the `plots` folder in this repository, as the contents will be ignored through `.gitignore`.
		//!< !! No not end this string with a "/" !!


// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //
/**
 * @brief Main function used when compiling and executing in `ROOT`.
 * @param inputFileName 
 */
void FitDoubleGaussian_RooFit(const char* inputFileName = gDefaultFileToLoad)
{
	// * Output file name declared here to avoid `Form` bug later * //
		// ! becareful with the `/` sign
		gSystem->mkdir(gOutputDir, kTRUE);
		const TString outputFileName = Form("%s/%s_RooFit.%s", gOutputDir, gBrancheToLoad, gOutputExtension);

	// * Attempt to load the file * //
		TFile file(inputFileName);
		if(file.IsZombie()) return;

	// * Attempt to load trees * //
		TTree *tree = dynamic_cast<TTree*>(file.Get(gTreeToLoad));
		if(!tree) {
			std::cout << "FATAL ERROR: File \"" << inputFileName << "\" does not contain a tree \"" << gTreeToLoad << "\"" << std::endl;
			file.Close();
			return;
		}
		Long64_t nEntries = tree->GetEntries();

	// * Get branch addresses * //
		double invmass;
		if(tree->SetBranchAddress(gBrancheToLoad, &invmass)) {
			std::cout << "ERROR: Branch \"" << gBrancheToLoad << "\" not properly loaded" << std::endl;
			file.Close();
			return;
		}

	// * Create invariant mass histogram * //
			TH1D hist(Form("%s_%s", gTreeToLoad, gBrancheToLoad),
				Form("Invariant mass as determined with %s (%s candidate);#it{M}_{%s} (GeV/#it{c}^{2});counts", gTreeToLoad, gCandidate, gDaughters),
				500, 0., .25
			);

	// * Loop the tree to fill inv mass spectrums * //
		std::cout << "Looping over " << nEntries << " events in the \"" << gTreeToLoad << "\" tree (\"" << gBrancheToLoad << "\")" << std::flush;
		for(Long64_t i = 0; i < nEntries; ++i) {
			tree->GetEntry(i);
			hist.Fill(invmass);
		}
		std::cout << "\rSuccesfully looped over " << nEntries << " events in the \"" << gTreeToLoad << "\" tree (\"" << gBrancheToLoad << "\")" << std::endl;

	// * The `RooFit` method * //
		const double xmin = hist.GetXaxis()->GetXmin();
		const double xmax = hist.GetXaxis()->GetXmax();
		RooRealVar invMassVar("invMassVar", Form("#it{M}_{%s} (GeV/#it{c}^{2})", gDaughters), xmin, xmax);

	// * Import histogram to Roofit * //
		RooDataHist invMassDistribution(
			Form("%s_%s_RooDataHist", gTreeToLoad, gBrancheToLoad), hist.GetTitle(),
			invMassVar, RooFit::Import(hist));

	// * Create Gaussian functions * //
		RooRealVar m1("mean1",  "#pi^{0} mass", 0.);
		RooRealVar s1("sigma1", "#pi^{0} width", 0.03343); // should be extracted from pure double Gaussian fit
		RooGaussian gauss1("gauss1",
			"Gaussian PDF 1 for #it{M}_{#gamma#gamma} distribution",
			invMassVar, m1, s1);

		RooRealVar m2("mean2",  "#pi^{0} mass", 0.);
		RooRealVar s2("sigma2", "#pi^{0} width", 0.005509); // should be extracted from pure double Gaussian fit
		RooGaussian gauss2("gauss2",
			"Gaussian PDF 2 for #it{M}_{#gamma#gamma} distribution",
			invMassVar, m2, s2);

	// * Add the two Gaussian components * //
		double boundary = 5e5;
		RooRealVar n1("ngauss1", "Number of events in Gaussian 1", 8e3, 0., boundary);
		RooRealVar n2("ngauss2", "Number of events in Gaussian 2", 1e5, 0., boundary);
		RooAddPdf doublegauss("double_gaussian", "Double gaussian",
			RooArgList(gauss1, gauss2),
			RooArgList(n1, n2));

	// * Create a Breit-Wigner/Cauchy distribution * //
		RooRealVar bwm1("mean",  "#pi^{0} mass",
			gParticlePDG->Mass(), .95*gParticlePDG->Mass(), 1.05*gParticlePDG->Mass());
		RooRealVar bws1("bw-width", "#pi^{0} width", .01, 0., 1.);
		RooBreitWigner bw("breitwigner",
			"Breit-Wigner PDF for the #it{M}_{#gamma#gamma} distribution",
			invMassVar, bwm1, bws1);

	// * Convolute Breit-Wigner with double Gaussian * //
		RooFFTConvPdf signal("signal", "signal", invMassVar, bw, doublegauss);
		RooFitResult* result = signal.fitTo(invMassDistribution);

	// * Fit, plot results, and save * //
		RooPlot *frame = invMassVar.frame(); // create a frame to draw
		frame->SetAxisRange(.1, .169);
		invMassDistribution.plotOn(frame, // draw distribution
			LineWidth(2), LineColor(kBlue+2), LineWidth(1),
			MarkerColor(kBlue+2), MarkerSize(.5));
		signal.plotOn(frame, LineWidth(2), LineColor(kBlack), FillColor(kBlack)); // draw sig+bck fit
		signal.plotOn(frame, Components(doublegauss), // draw double Gaussian
			LineWidth(1), LineColor(kRed-4));
		signal.plotOn(frame, Components(bw), // draw Breit-Wigner
			LineWidth(1), LineColor(kBlue-4));
		signal.paramOn(frame, Layout(.56, .98, .92));

	// * Write fitted histograms * //
		TCanvas c;
		c.SetBatch();
		frame->Draw();
		c.SaveAs(outputFileName.Data());
		c.Close();

	// * Finalise macro *
		file.Close();

}

/**
 * @brief Main function that is called when executing the executable compiled using e.g. `g++`.
 */
int main(int argc, char *argv[])
{
	// * Default arguments * //
	TString inputFileName(gDefaultFileToLoad);

	//* Import arguments if available * //
	if(argc > 1) {
		inputFileName = argv[1];
	}

	// * Execute function * //
	FitDoubleGaussian_RooFit(inputFileName.Data());

	// * Default return value * //
	return 0;
}