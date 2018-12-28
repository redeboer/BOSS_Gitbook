/**
 * @brief    Simple script that allows to quickly investigate the contents of a ROOT file that has been produced with BOSS.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 */



// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "../inc/BOSSRootFile.h"
	#include "TStyle.h"
	#include <iostream>
	using namespace CommonFunctions::Fit;
	using namespace RooFit;
	using namespace Settings;



// ! ============================== ! //
// ! ------- MACRO SETTINGS ------- ! //
// ! ============================== ! //

	// * INPUT FILE * //
	const char* filename = "/mnt/c/IHEP/scratchfs/bes/deboer/data/root/ana_DzeroPhi_mc_resultTrackSelector.root";

	// * TERMINAL OUTPUT * //
	const bool print = false; //!< Whether or not to use the precisely set histogram ranges.

	// * PLOT STYLE * //
	const bool setranges = false; //!< Whether or not to precisely set histogram ranges.
	const bool plotstats = true; //!< Whether or not to draw the legend in the upper right corner with histogram statistics.

	// * WHICH BRANCHES TO PLOT * //
	const bool pureplot    = true; //!< Whether or not to plot histograms of branches <i>without fit</i>.
	const bool draw_mult   = false; //!< Whether or not to draw the multiplicity branches.
	const bool draw_vertex = false; //!< Whether or not to draw the `"vertex"` branch.
	const bool draw_tof    = false; //!< Whether or not to draw the `"tof*"` branches.
	const bool draw_fit    = true; //!< Whether or not to draw the `"fit"` branches.

	// * FIT SETTINGS * //
	const bool fitplots  = false; //!< Whether or not to produce invariant mass fits.
	const bool do_gauss  = true; //!< Whether or not to produce perform a double Gaussian fit.
	const bool do_conv_s = true; //!< Whether or not to produce perform a Breit-Wigner convoluted with a <i>single</i> Gaussian.
	const bool do_conv_d = false; //!< Whether or not to produce perform a Breit-Wigner convoluted with a <i>double</i> Gaussian.



// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //
/**
 * @brief Main function used when compiling and executing in `ROOT`.
 */
void FitInvMassSignal()
{


	// * OPEN INPUT FILE * //
		BOSSRootFile file(filename, print); /// To investigate the contents of the ROOT file, you first need to know which `TTree`s and branches it contains. If you simply construct the `BOSSRootFile` by giving it a file name, all `TTree`s will be loaded automatically as well as addresses for each of their branches. Five the constructer `true` as its second argument, and the names of these `TTree`s, their branches, and the types of these branches (behind the slash `/` after the name) will be printed to the terminal. <b>Do this if your macro throws an exception, because this probably means that you use the wrong names for the trees and or the branches further on in the macro.</b>
		if(file.IsZombie()) return;
		if(plotstats) gStyle->SetOptStat(0);


	// * PLOT BRANCHES WITHOUT FITS * //
		if(pureplot) {
			if(draw_mult) {
				for(auto tree : file.GetSimplifiedTrees()) {
					TString name(tree.second.Get()->GetName());
					if(name.BeginsWith("mult")) tree.second.DrawAndSaveAllMultiplicityBranches();
				}
			}
			if(draw_vertex) {
				if(setranges) {
					file.DrawBranches("vertex", "vy0:vx0", "colz");
					file.DrawBranches("vertex", "vz0:vx0", "colz");
					file.DrawBranches("vertex", "vz0:vy0", "colz");
				} else {
					file.DrawBranches("vertex", "vx0", "vy0", 60, -.154, -.146,  40,  .08,   .122, "colz");
					file.DrawBranches("vertex", "vx0", "vz0", 60, -.4,    .5,    60,  .08,   .122, "colz");
					file.DrawBranches("vertex", "vy0", "vz0", 60, -.4,    .5,    40, -.154, -.146, "colz");
				}
			}
			if(draw_tof) {
				if(setranges) {
					file.DrawBranches("ToFIB", "p", "tof", 120, 2., 15., 80, 0., 1.5, "colz", "z");
					file.DrawBranches("ToFOB", "p", "tof", 120, 2., 15., 80, 0., 1.5, "colz", "z");
				} else {
					file.DrawBranches("ToFIB", "tof:p", "colz");
					file.DrawBranches("ToFOB", "tof:p", "colz");
				}
			}
			if(draw_fit) {
				if(setranges) {
					file.DrawBranches("fit4c_all",  "mD0",    500,  .7,    2.,      "E1", "y");
					file.DrawBranches("fit4c_best", "mD0",    500,  .7,    2.,      "E1", "y");
					file.DrawBranches("fit4c_all",  "mphi",   500,  .97,   1.7,     "E1", "y");
					file.DrawBranches("fit4c_best", "mphi",   500,  .97,   1.7,     "E1", "y");
					file.DrawBranches("fit4c_all",  "mJpsi", 2000, 3.0967, 3.09685, "E1", "y");
					file.DrawBranches("fit4c_best", "mJpsi", 2000, 3.0967, 3.09685, "E1", "y");
				} else {
					file.DrawBranches("fit4c_all",  "mD0",   "E1", "y");
					file.DrawBranches("fit4c_best", "mD0",   "E1", "y");
					file.DrawBranches("fit4c_all",  "mphi",  "E1", "y");
					file.DrawBranches("fit4c_best", "mphi",  "E1", "y");
					file.DrawBranches("fit4c_all",  "mJpsi", "E1", "y");
					file.DrawBranches("fit4c_best", "mJpsi", "E1", "y");
				}
			}
		}


	// * PERFORM FITS * //
		if(fitplots) {
		// * Particles to reconstruct
			ReconstructedParticle D0  (421, "K^{-}#pi^{+}");
			ReconstructedParticle phi (333, "K^{+}K^{-}");
			ReconstructedParticle Jpsi(443, "#D^{0}#phi");

		// * Create invariant mass histogram
			TH1F* hist_D0   = file["fit4c"].GetInvariantMassHistogram("mD0",   D0,   500);
			TH1F* hist_phi  = file["fit4c"].GetInvariantMassHistogram("mphi",  phi,  500);
			// TH1F* hist_Jpsi = file["fit4c"].GetInvariantMassHistogram("mJpsi", Jpsi, 2000); //! useless plot

		// * Fit double gaussian
			if(do_gauss) {
				FitDoubleGaussian(hist_D0,   D0,   0, "y");
				FitDoubleGaussian(hist_phi,  phi,  2, "y");
				// FitDoubleGaussian(hist_Jpsi, Jpsi, 0, "y"); //! useless plot
			}

		// * Fit Breit-Wigner convoluted with singe Gaussian
			if(do_conv_s) {
				FitBWGaussianConvolution(hist_D0,   D0,   0, "y");
				FitBWGaussianConvolution(hist_phi,  phi,  2, "y");
				// FitBWGaussianConvolution(hist_Jpsi, Jpsi, 2, "y"); //! useless plot
			}

		// * Fit Breit-Wigner convoluted with double Gaussian
			if(do_conv_s) {
				FitBWDoubleGaussianConvolution(hist_D0,   D0,   0, "y");
				FitBWDoubleGaussianConvolution(hist_phi,  phi,  2, "y");
				// FitBWDoubleGaussianConvolution(hist_Jpsi, Jpsi, 2, "y"); //! useless plot
			}

		}

}

/**
 * @brief Main function that is called when executing the executable compiled using e.g. `g++`.
 */
int main()
{
	FitInvMassSignal();
	return 0;
}