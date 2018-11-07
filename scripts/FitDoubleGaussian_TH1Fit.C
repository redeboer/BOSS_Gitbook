/**
 * @brief    Simple script that shows how to fit a double gaussian on an invariant mass spectrum using the standard ROOT `TH1::Fit` method.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 7th, 2018
 * @details  Note that this script makes use of one `hadd`ed file, not of a `TChain`. It is currently assumed that the invariant mass plot to be fitted comes from the `invmass` branche in the `fit4c` that is produced by the `RhopiAlg` analysis job. But this can be modified in the section `GLOBALS`.
 */


// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "TCanvas.h"
	#include "TText.h"
	#include "TDatabasePDG.h"
	#include "TF1.h"
	#include "TFile.h"
	#include "TFitResult.h"
	#include "TFitResultPtr.h"
	#include "TH1D.h"
	#include "TLine.h"
	#include "TParticlePDG.h"
	#include "TPaveText.h"
	#include "TString.h"
	#include "TStyle.h"
	#include "TSystem.h"
	#include "TTree.h"
	#include <iostream>

// * ======================= * //
// * ------- GLOBALS ------- * //
// * ======================= * //
	// ! Do not change these parametes ! //
	const TDatabasePDG gPDG;
		//!< A `ROOT` `TDatabasePDG` object that contains all info of particles. Has to be constructed once, which is why it is a global.
	// ! Customise these values ! //
	const TParticlePDG* gParticlePDG = gPDG.GetParticle(111);
		//!< The particle that you want to analyse. (111 is the PDG code for pi0.
	const char* gDefaultFileToLoad = "../data/root/ana_rhopi.root";
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
void FitDoubleGaussian_TH1Fit(const char* inputFileName = gDefaultFileToLoad)
{
	// * Output file name declared here to avoid `Form` bug later * //
		// ! becareful with the `/` sign
		gSystem->mkdir(gOutputDir, kTRUE);
		const TString outputFileName = Form("%s/%s_TH1Fit.%s", gOutputDir, gBrancheToLoad, gOutputExtension);

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

	// * The ROOT `TH1::Fit` method * //
	/*
		See https://root.cern.ch/root/htmldoc/guides/users-guide/FittingHistograms.html
	*/
		const double xmin = hist.GetXaxis()->GetXmin();
		const double xmax = hist.GetXaxis()->GetXmax();
		TF1 doublegauss("double gaussian", "gaus(0)+gaus(3)", xmin, xmax);
		doublegauss.SetLineColor(kBlack); // black
		// * height of each Gaussian
			doublegauss.SetParameter(0, .5*hist.GetBinContent(hist.GetMaximumBin()));
			doublegauss.SetParameter(3, .5*hist.GetBinContent(hist.GetMaximumBin()));
		// * mean of each Gaussian (fixed!)
			doublegauss.FixParameter(1, gParticlePDG->Mass());
			doublegauss.FixParameter(4, gParticlePDG->Mass());
		// * width of each Gaussian
			doublegauss.SetParameter(2, .005);
			doublegauss.SetParameter(5, .0005);
		TFitResultPtr result = hist.Fit(&doublegauss, "SQ");
		result->Print();

	// * Extract the parameters * //
		// * Gaussian 1
		TF1 gaus1("gaus1", "gaus", xmin, xmax);
		gaus1.SetLineColor(kRed-4); // light red
		// gaus1.SetLineStyle(2);      // dashed
		gaus1.SetLineWidth(1);      // thinest
		gaus1.FixParameter(0, doublegauss.GetParameter(0)); // height 1
		gaus1.FixParameter(1, doublegauss.GetParameter(1)); // mean 1
		gaus1.FixParameter(2, doublegauss.GetParameter(2)); // width 1
		// * Gaussian 2
		TF1 gaus2("gaus2", "gaus", xmin, xmax);
		gaus2.SetLineColor(kBlue-4); // light blue
		// gaus2.SetLineStyle(2);       // dashed
		gaus2.SetLineWidth(1);       // thinest
		gaus2.FixParameter(0, doublegauss.GetParameter(3)); // height 2
		gaus2.FixParameter(1, doublegauss.GetParameter(4)); // mean 2
		gaus2.FixParameter(2, doublegauss.GetParameter(5)); // width 2

	// * Write main histogram (do this first to you can extract Y-ranges) * //
		TCanvas c;
		hist.SetAxisRange(.1, .17); // decrease range of the X-axis
		hist.Draw("ep1");
		c.Update(); // see here https://root.cern.ch/root/roottalk/roottalk02/4222.html

	// * Beatify the eventual plot with some lines * //
		// * Vertical mass line
		TLine massLine(
			gParticlePDG->Mass(), gPad->GetUymin(),
			gParticlePDG->Mass(), gPad->GetUymax()
		);
		massLine.SetLineColor(kGray+2); // darker gray
		massLine.SetLineStyle(2);
		// * Lines that indicate width
		double leftSide1  = gParticlePDG->Mass() - gaus1.GetParameter(2);
		double leftSide2  = gParticlePDG->Mass() - gaus2.GetParameter(2);
		double rightSide1 = gParticlePDG->Mass() + gaus1.GetParameter(2);
		double rightSide2 = gParticlePDG->Mass() + gaus2.GetParameter(2);
		TLine  leftWidth1( leftSide1, 0.,  leftSide1, gaus1( leftSide1));
		TLine  leftWidth2( leftSide2, 0.,  leftSide2, gaus2( leftSide2));
		TLine rightWidth1(rightSide1, 0., rightSide1, gaus1(rightSide1));
		TLine rightWidth2(rightSide2, 0., rightSide2, gaus2(rightSide2));
		leftWidth1 .SetLineColor(kRed -4); // light red
		leftWidth2 .SetLineColor(kBlue-4); // light blue
		rightWidth1.SetLineColor(kRed -4); // light red
		rightWidth2.SetLineColor(kBlue-4); // light blue
		leftWidth1 .SetLineStyle(2);
		leftWidth2 .SetLineStyle(2);
		rightWidth1.SetLineStyle(2);
		rightWidth2.SetLineStyle(2);

	// * TPave with statistics * //
		int    ndf     = doublegauss.GetNDF();
		double chi2    = doublegauss.GetChisquare();
		double chi2red = chi2/ndf;
		double fixmean = gParticlePDG->Mass();
		double sigma1  = gaus1.GetParameter(2);
		double sigma2  = gaus2.GetParameter(2);
		TPaveText stats(.7, .6, .88, .88, "NDC");
		stats.AddText("Fit statistics")->SetTextFont(22);
		stats.AddText(Form("#it{n}_{events} = %lld",     nEntries))->SetTextFont(132);
		stats.AddText(Form("NDF = %d",                   ndf))     ->SetTextFont(132);
		stats.AddText(Form("#it{#chi}^{2} = %.1f",       chi2))    ->SetTextFont(132);
		stats.AddText(Form("#it{#chi}_{red}^{2} = %.2f", chi2red)) ->SetTextFont(132);
		stats.AddLine(0., .37, 1., .37);
		stats.AddText(Form("#sigma_{1} = %.3f MeV/#it{c}^{2}", 1e3*sigma1))->SetTextFont(132);
		stats.AddText(Form("#sigma_{2} = %.3f MeV/#it{c}^{2}", 1e3*sigma2))->SetTextFont(132);
		stats.AddText(Form("fixed mean: %.2f MeV/#it{c}^{2}", 1e3*fixmean))->SetTextFont(132);

	// * Write non-fitted histograms * //
		gStyle->SetOptStat(0);
		gaus1.Draw("same");
		gaus2.Draw("same");
		massLine.Draw("same");
		leftWidth1 .Draw("same");
		leftWidth2 .Draw("same");
		rightWidth1.Draw("same");
		rightWidth2.Draw("same");
		stats.Draw();
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
	FitDoubleGaussian_TH1Fit(inputFileName.Data());

	// * Default return value * //
	return 0;
}