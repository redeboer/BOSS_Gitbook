/// @addtogroup BOSS_Afterburner_scripts
/// @{
	
/**
/// Simple script that allows to quickly investigate the contents of a ROOT file that has been produced with BOSS.
/// @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
/// @date     November 5th, 2018
 */



// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "ConfigLoader.h"
	#include "BOSSOutputLoader.h"
	#include "CommonFunctions.h"
	#include "TStyle.h"
	#include <fstream>
	#include <iostream>

	using namespace CommonFunctions::Draw;
	using namespace CommonFunctions::Fit;
	using namespace RooFit;
	using namespace Settings;
	using namespace std;



// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //


	/// Main function used when compiling and executing in `ROOT`.
	void AnalyseBOSSOutput(const char* configuration_file="configs/D0phi_KpiKK_quick.txt")
	{

		// * OPEN INPUT FILE * //
			ConfigLoader config(configuration_file);
			BOSSOutputLoader file(config.fInputFilename.value.data(), config.fPrintBranches.value); /// To investigate the contents of the ROOT file, you first need to know which `TTree`s and branches it contains. If you simply construct the `BOSSOutputLoader` by giving it a file name, all `TTree`s will be loaded automatically as well as addresses for each of their branches. Five the constructer `true` as its second argument, and the names of these `TTree`s, their branches, and the types of these branches (behind the slash `/` after the name) will be printed to the terminal. <b>Do this if your macro throws an exception, because this probably means that you use the wrong names for the trees and or the branches further on in the macro.</b>
			if(file.IsZombie()) return;
			if(!config.fPlotstats.value) gStyle->SetOptStat(0);
			file.PrintCutFlow();

		// * PLOT BRANCHES WITHOUT FITS * //
			if(config.fPureplot.value) {
				if(config.fDraw_mult.value) {
					for(auto tree = file.GetChains().begin(); tree != file.GetChains().end(); ++tree) {
						TString name(tree->second.GetChain().GetName());
						if(name.BeginsWith("mult")) tree->second.DrawAndSaveAllMultiplicityBranches(config.fLogY.value.data(), "");
					}
				}
				if(config.fDraw_mctruth.value) {
					if(config.fSetranges.value) {
						file.Draw("mctruth", "E", 150, 0., 3.5, "E1", config.fLogY.value.data());
						file.Draw("mctruth", "p", 150, 0., 1.25, "E1", config.fLogY.value.data());
					} else {
						file.Draw("mctruth", "E", "E1", config.fLogY.value.data());
						file.Draw("mctruth", "p", "E1", config.fLogY.value.data());
					}
				}
				if(config.fDraw_vertex.value) {
					if(config.fSetranges.value) {
						file.Draw("vertex", "vy0:vx0", "colz", config.fLogZ.value.data());
						file.Draw("vertex", "vz0:vx0", "colz", config.fLogZ.value.data());
						file.Draw("vertex", "vz0:vy0", "colz", config.fLogZ.value.data());
					} else {
						file.Draw("vertex", "vx0", "vy0", 60, -.154, -.146,  40,  .08,   .122, "colz", config.fLogZ.value.data());
						file.Draw("vertex", "vx0", "vz0", 60, -.4,    .5,    60,  .08,   .122, "colz", config.fLogZ.value.data());
						file.Draw("vertex", "vy0", "vz0", 60, -.4,    .5,    40, -.154, -.146, "colz", config.fLogZ.value.data());
					}
				}
				if(config.fDraw_tof.value) {
					if(config.fSetranges.value) {
						file.Draw("ToFIB", "p", "tof", 120, 2., 15., 80, 0., 1.5, "colz", config.fLogZ.value.data());
						file.Draw("ToFOB", "p", "tof", 120, 2., 15., 80, 0., 1.5, "colz", config.fLogZ.value.data());
					} else {
						file.Draw("ToFIB", "tof:p", "colz", config.fLogZ.value.data());
						file.Draw("ToFOB", "tof:p", "colz", config.fLogZ.value.data());
					}
				}
				if(config.fDraw_fit.value) {
					if(config.fSetranges.value) {
						/// -# Draw main invariant mass distributions
						file.Draw("fit4c_all",  "mphi", "mD0", 240, .7, 2., 160, .9, 2.1, "colz", config.fLogZ.value.data());
						file.Draw("fit4c_all",  "mD0",   400,  .7,    2.,      "E1", config.fLogY.value.data());
						file.Draw("fit4c_all",  "mJpsi", 400, 3.0967, 3.09685, "E1", config.fLogY.value.data());
						file.Draw("fit4c_all",  "mphi",  400,  .97,   1.7,     "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "mphi", "mD0", 240, .7, 2., 160, .9, 2.1, "colz", config.fLogZ.value.data());
						file.Draw("fit4c_best", "mD0",   400,  .7,    2.,      "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "mJpsi", 400, 3.0967, 3.09685, "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "mphi",  400,  .97,   1.7,     "E1", config.fLogY.value.data());
						/// -# Draw invariant mass distributions with cuts applied on the other candidate
						DrawAndSave(&file["fit4c_all"].GetChain(), "mphi", "mD0>1.5",  "E1", config.fLogY.value.data());
						DrawAndSave(&file["fit4c_all"].GetChain(), "mD0",  "mphi<1.1", "E1", config.fLogY.value.data());
						DrawAndSave(&file["fit4c_all"].GetChain(), "mD0:mphi",  "mD0>1.5&&mphi<1.1", "colz", config.fLogZ.value.data());
						/// -# Draw 3-momentum distributions
						file.Draw("fit4c_all",  "pD0",  400, .4, 1.15, "E1", config.fLogY.value.data());
						file.Draw("fit4c_all",  "pphi", 400, .4, 1.15, "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "pD0",  400, .4, 1.15, "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "pphi", 400, .4, 1.15, "E1", config.fLogY.value.data());
						/// -# Draw invariant mass versus 3-momentum
						file.Draw("fit4c_all",  "mD0",  "pD0",  240, .4, 1.15, 160, .97, 1.7, "colz", config.fLogZ.value.data());
						file.Draw("fit4c_all",  "mphi", "pphi", 240, .4, 1.15, 160,  .7,  2., "colz", config.fLogZ.value.data());
						file.Draw("fit4c_best", "mD0",  "pD0",  240, .4, 1.15, 160, .97, 1.7, "colz", config.fLogZ.value.data());
						file.Draw("fit4c_best", "mphi", "pphi", 240, .4, 1.15, 160,  .7,  2., "colz", config.fLogZ.value.data());
						file.Draw("fit4c_all",  "mD0:pD0",   "colz", config.fLogZ.value.data());
						file.Draw("fit4c_all",  "mphi:pphi", "colz", config.fLogZ.value.data());
						file.Draw("fit4c_best", "mD0:pD0",   "colz", config.fLogZ.value.data());
						file.Draw("fit4c_best", "mphi:pphi", "colz", config.fLogZ.value.data());
					} else {
						/// -# Draw main invariant mass distributions
						file.Draw("fit4c_all",  "mD0",   "E1", config.fLogY.value.data());
						file.Draw("fit4c_all",  "mJpsi", "E1", config.fLogY.value.data());
						file.Draw("fit4c_all",  "mphi",  "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "mD0",   "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "mJpsi", "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "mphi",  "E1", config.fLogY.value.data());
						/// -# Draw invariant mass distributions with cuts applied on the other candidate
						DrawAndSave(&file["fit4c_all"].GetChain(), "mphi", "mD0>1.5",  "E1", config.fLogY.value.data());
						DrawAndSave(&file["fit4c_all"].GetChain(), "mD0",  "mphi<1.1", "E1", config.fLogY.value.data());
						DrawAndSave(&file["fit4c_all"].GetChain(), "mD0:mphi",  "mD0>1.5&&mphi<1.1", "colz", config.fLogZ.value.data());
						/// -# Draw 3-momentum distributions
						file.Draw("fit4c_all",  "pD0",  "E1", config.fLogY.value.data());
						file.Draw("fit4c_all",  "pphi", "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "pD0",  "E1", config.fLogY.value.data());
						file.Draw("fit4c_best", "pphi", "E1", config.fLogY.value.data());
						/// -# Draw invariant mass versus 3-momentum
						file.Draw("fit4c_all",  "mD0:pD0",   "colz", config.fLogZ.value.data());
						file.Draw("fit4c_all",  "mphi:pphi", "colz", config.fLogZ.value.data());
						file.Draw("fit4c_best", "mD0:pD0",   "colz", config.fLogZ.value.data());
						file.Draw("fit4c_best", "mphi:pphi", "colz", config.fLogZ.value.data());
					}
				}
			}

		// * PERFORM FITS * //
			if(config.fFitplots.value) {
			// * Particles to reconstruct
				ReconstructedParticle D0  (421, "K^{-}#pi^{+}");
				ReconstructedParticle phi (333, "K^{+}K^{-}");
				ReconstructedParticle Jpsi(443, "#D^{0}#phi");

			// * Create invariant mass histogram
				TH1F* hist_D0  = file["fit4c"].GetInvariantMassHistogram("mD0",   D0,   500);
				TH1F* hist_phi = file["fit4c"].GetInvariantMassHistogram("mphi",  phi,  500);
				// TH1F* hist_Jpsi = file["fit4c"].GetInvariantMassHistogram("mJpsi", Jpsi, 2000); //! useless plot

			// * Fit double gaussian
				if(config.fDo_gauss.value) {
					FitDoubleGaussian(hist_D0,   D0,   0, config.fLogY.value.data());
					FitDoubleGaussian(hist_phi,  phi,  2, config.fLogY.value.data());
					// FitDoubleGaussian(hist_Jpsi, Jpsi, 0, gLogY.value.data()); //! useless plot
				}

			// * Fit Breit-Wigner convoluted with singe Gaussian
				if(config.fDo_conv_s.value) {
					FitBWGaussianConvolution(hist_D0,   D0,   0, config.fLogY.value.data());
					FitBWGaussianConvolution(hist_phi,  phi,  2, config.fLogY.value.data());
					// FitBWGaussianConvolution(hist_Jpsi, Jpsi, 2, config.fLogY.value.data()); //! useless plot
				}

			// * Fit Breit-Wigner convoluted with double Gaussian
				if(config.fDo_conv_s.value) {
					FitBWDoubleGaussianConvolution(hist_D0,   D0,   0, config.fLogY.value.data());
					FitBWDoubleGaussianConvolution(hist_phi,  phi,  2, config.fLogY.value.data());
					// FitBWDoubleGaussianConvolution(hist_Jpsi, Jpsi, 2, config.fLogY.value.data()); //! useless plot
				}

			}

	}


	/// Main function that is called when executing the executable compiled using e.g. `g++`.
	int main(int argc, char *argv[])
	{
		if(argc > 2) {
			cout << "FATAL ERROR: Cannot run this macro with more than one argument" << endl;
			return 1;
		}
		if(argc==1) AnalyseBOSSOutput();
		else        AnalyseBOSSOutput(argv[1]);
		return 0;
	}


/// @}