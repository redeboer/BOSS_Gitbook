/// @addtogroup BOSS_Afterburner_scripts
/// @{
	
/**
 * @brief    Simple script that allows to quickly investigate the contents of a ROOT file that has been produced with BOSS.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 */



// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "CommonFunctions.h"
	#include "BOSSOutputLoader.h"
	#include "ArgPair.h"
	#include "TStyle.h"
	#include <iostream>
	#include <utility>
	#include <fstream>
	#include <sstream>
	#include <list>
	using namespace CommonFunctions::Draw;
	using namespace CommonFunctions::Fit;
	using namespace RooFit;
	using namespace Settings;
	using namespace std;



// * ================================ * //
// * ------- DEFAULT SETTINGS ------- * //
// * ================================ * //

	// * FILE NAME * //
	ArgPair<string> gFilename { "Input file or directory", "NOFILE" };

	// * TERMINAL OUTPUT * //
	ArgPair<bool> gPrint { "Print branches", false };

	// * PLOT STYLE * //
	ArgPair<string> gLogY    { "Use y log scale", "y"   }; //!< Whether to draw the \f$y\f$ axis of the `TH1F` in log scale.
	ArgPair<string> gLogZ    { "Use z log scale", "z"   }; //!< Whether to draw the \f$z\f$ axis of the `TH2F` in log scale.
	ArgPair<bool> gSetranges { "Set plot ranges", true  }; //!< Whether or not to precisely set histogram ranges.
	ArgPair<bool> gPlotstats { "Plot statistics", false }; //!< Whether or not to draw the legend in the upper right corner with histogram statistics.

	// * WHICH BRANCHES TO PLOT * //
	ArgPair<bool> gPureplot    { "Plot raw data",      true  }; //!< Whether or not to plot histograms of branches <i>without fit</i>.
	ArgPair<bool> gDraw_mctruth { "Draw mctruth",      false }; //!< Whether or not to draw the MC truth parameters.
	ArgPair<bool> gDraw_mult   { "Draw multiplicites", false }; //!< Whether or not to draw the multiplicity branches.
	ArgPair<bool> gDraw_vertex { "Draw vertex",        false }; //!< Whether or not to draw the `"vertex"` branch.
	ArgPair<bool> gDraw_tof    { "Draw ToF",           false }; //!< Whether or not to draw the `"tof*"` branches.
	ArgPair<bool> gDraw_fit    { "Draw fit branches",  true  }; //!< Whether or not to draw the `"fit"` branches.

	// * FIT SETTINGS * //
	ArgPair<bool> gFitplots  { "Perform fits",          false }; //!< Whether or not to produce invariant mass fits.
	ArgPair<bool> gDo_gauss  { "Do Gaussian",           true  }; //!< Whether or not to produce perform a double Gaussian fit.
	ArgPair<bool> gDo_conv_s { "Do single convolution", false }; //!< Whether or not to produce perform a Breit-Wigner convoluted with a <i>single</i> Gaussian.
	ArgPair<bool> gDo_conv_d { "Do double convolution", false }; //!< Whether or not to produce perform a Breit-Wigner convoluted with a <i>double</i> Gaussian.



// * ===================================== * //
// * ------- FUNCTION DECLARATIONS ------- * //
// * ===================================== * //

	int LoadConfiguration(const char* filename);
	string GetParameterName(string line);
	string GetParameterValue(string line);
	void DrawDifference(TH1 *histToDraw, TH1 *histToSubtract, Option_t* opt="E1", const char* setLog="");
	void RemoveLeading(string &line, const char c=' ');
	void RemoveTrailing(string &line, const char c=' ');
	void Trim(string &line, const char c=' ');



// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //


	/**
	 * @brief Main function used when compiling and executing in `ROOT`.
	 */
	void AnalyseBOSSOutput(TString configuration_file="configs/D0phi_KpiKK_quick.txt")
	{

		// * OPEN INPUT FILE * //
			LoadConfiguration(configuration_file.Data());
			BOSSOutputLoader file(gFilename.value.data(), gPrint.value); /// To investigate the contents of the ROOT file, you first need to know which `TTree`s and branches it contains. If you simply construct the `BOSSOutputLoader` by giving it a file name, all `TTree`s will be loaded automatically as well as addresses for each of their branches. Five the constructer `true` as its second argument, and the names of these `TTree`s, their branches, and the types of these branches (behind the slash `/` after the name) will be printed to the terminal. <b>Do this if your macro throws an exception, because this probably means that you use the wrong names for the trees and or the branches further on in the macro.</b>
			if(file.IsZombie()) return;
			if(!gPlotstats.value) gStyle->SetOptStat(0);
			file.PrintCutFlow();

		// * PLOT BRANCHES WITHOUT FITS * //
			if(gPureplot.value) {
				if(gDraw_mult.value) {
					for(auto tree = file.GetChains().begin(); tree != file.GetChains().end(); ++tree) {
						TString name(tree->second.GetChain().GetName());
						if(name.BeginsWith("mult")) tree->second.DrawAndSaveAllMultiplicityBranches(gLogY.value.data(), "");
					}
				}
				if(gDraw_mctruth.value) {
					if(gSetranges.value) {
						file.DrawBranches("mctruth", "E", 150, 0., 3.5, "E1", gLogY.value.data());
						file.DrawBranches("mctruth", "p", 150, 0., 1.25, "E1", gLogY.value.data());
					} else {
						file.DrawBranches("mctruth", "E", "E1", gLogY.value.data());
						file.DrawBranches("mctruth", "p", "E1", gLogY.value.data());
					}
				}
				if(gDraw_vertex.value) {
					if(gSetranges.value) {
						file.DrawBranches("vertex", "vy0:vx0", "colz", gLogZ.value.data());
						file.DrawBranches("vertex", "vz0:vx0", "colz", gLogZ.value.data());
						file.DrawBranches("vertex", "vz0:vy0", "colz", gLogZ.value.data());
					} else {
						file.DrawBranches("vertex", "vx0", "vy0", 60, -.154, -.146,  40,  .08,   .122, "colz", gLogZ.value.data());
						file.DrawBranches("vertex", "vx0", "vz0", 60, -.4,    .5,    60,  .08,   .122, "colz", gLogZ.value.data());
						file.DrawBranches("vertex", "vy0", "vz0", 60, -.4,    .5,    40, -.154, -.146, "colz", gLogZ.value.data());
					}
				}
				if(gDraw_tof.value) {
					if(gSetranges.value) {
						file.DrawBranches("ToFIB", "p", "tof", 120, 2., 15., 80, 0., 1.5, "colz", gLogZ.value.data());
						file.DrawBranches("ToFOB", "p", "tof", 120, 2., 15., 80, 0., 1.5, "colz", gLogZ.value.data());
					} else {
						file.DrawBranches("ToFIB", "tof:p", "colz", gLogZ.value.data());
						file.DrawBranches("ToFOB", "tof:p", "colz", gLogZ.value.data());
					}
				}
				if(gDraw_fit.value) {
					if(gSetranges.value) {
						/// -# Draw main invariant mass distributions
						auto fit4c_all_dalitz  = (TH1F*)(file.DrawBranches("fit4c_all",  "mphi", "mD0", 60, .7, 2., 40, .9, 2.1, "colz", gLogZ.value.data())->Clone("fit4c_best_D0/mD0_mphi_inv"));
						auto fit4c_all_mD0     = (TH1F*)(file.DrawBranches("fit4c_all",  "mD0",   100,  .7,    2.,      "E1", gLogY.value.data())->Clone("fit4c_all/mD0_inv"));
						auto fit4c_all_mJpsi   = (TH1F*)(file.DrawBranches("fit4c_all",  "mJpsi", 100, 3.0967, 3.09685, "E1", gLogY.value.data())->Clone("fit4c_all/mJpsi_inv"));
						auto fit4c_all_mphi    = (TH1F*)(file.DrawBranches("fit4c_all",  "mphi",  100,  .97,   1.7,     "E1", gLogY.value.data())->Clone("fit4c_all/mphi_inv"));
						auto fit4c_best_dalitz = (TH1F*)(file.DrawBranches("fit4c_best", "mphi", "mD0", 60, .7, 2., 40, .9, 2.1, "colz", gLogZ.value.data())->Clone("fit4c_best_D0/mD0_mphi_inv"));
						auto fit4c_best_mD0    = (TH1F*)(file.DrawBranches("fit4c_best", "mD0",   100,  .7,    2.,      "E1", gLogY.value.data())->Clone("fit4c_best/mD0_inv"));
						auto fit4c_best_mJpsi  = (TH1F*)(file.DrawBranches("fit4c_best", "mJpsi", 100, 3.0967, 3.09685, "E1", gLogY.value.data())->Clone("fit4c_best/mJpsi_inv"));
						auto fit4c_best_mphi   = (TH1F*)(file.DrawBranches("fit4c_best", "mphi",  100,  .97,   1.7,     "E1", gLogY.value.data())->Clone("fit4c_best/mphi_inv"));
						/// -# Draw difference between distributions for all combinations and the best combination
						DrawDifference(fit4c_best_mD0,    fit4c_all_mD0,    "E1",   gLogY.value.data());
						DrawDifference(fit4c_best_mphi,   fit4c_all_mphi,   "E1",   gLogY.value.data());
						DrawDifference(fit4c_best_mJpsi,  fit4c_all_mJpsi,  "E1",   gLogY.value.data());
						DrawDifference(fit4c_best_dalitz, fit4c_all_dalitz, "colz", gLogZ.value.data());
						/// -# Draw invariant mass distributions with cuts applied on the other candidate
						DrawAndSave(&file["fit4c_all"].GetChain(), "mphi", "mD0>1.5",  "E1", gLogY.value.data());
						DrawAndSave(&file["fit4c_all"].GetChain(), "mD0",  "mphi<1.1", "E1", gLogY.value.data());
						DrawAndSave(&file["fit4c_all"].GetChain(), "mD0:mphi",  "mD0>1.5&&mphi<1.1", "colz", gLogZ.value.data());
						/// -# Draw 3-momentum distributions
						file.DrawBranches("fit4c_all",  "pD0",  "E1", gLogY.value.data());
						file.DrawBranches("fit4c_all",  "pphi", "E1", gLogY.value.data());
						file.DrawBranches("fit4c_best", "pD0",  "E1", gLogY.value.data());
						file.DrawBranches("fit4c_best", "pphi", "E1", gLogY.value.data());
						/// -# Draw invariant mass versus 3-momentum
						file.DrawBranches("fit4c_all",  "mD0:pD0",   "colz", gLogY.value.data());
						file.DrawBranches("fit4c_all",  "mphi:pphi", "colz", gLogY.value.data());
						file.DrawBranches("fit4c_best", "mD0:pD0",   "colz", gLogY.value.data());
						file.DrawBranches("fit4c_best", "mphi:pphi", "colz", gLogY.value.data());
					} else {
						file.DrawBranches("fit4c_all",  "mD0",   "E1", gLogY.value.data());
						file.DrawBranches("fit4c_all",  "mJpsi", "E1", gLogY.value.data());
						file.DrawBranches("fit4c_all",  "mphi",  "E1", gLogY.value.data());
						file.DrawBranches("fit4c_best", "mD0",   "E1", gLogY.value.data());
						file.DrawBranches("fit4c_best", "mJpsi", "E1", gLogY.value.data());
						file.DrawBranches("fit4c_best", "mphi",  "E1", gLogY.value.data());
					}
				}
			}

		// * PERFORM FITS * //
			if(gFitplots.value) {
			// * Particles to reconstruct
				ReconstructedParticle D0  (421, "K^{-}#pi^{+}");
				ReconstructedParticle phi (333, "K^{+}K^{-}");
				ReconstructedParticle Jpsi(443, "#D^{0}#phi");

			// * Create invariant mass histogram
				TH1F* hist_D0  = file["fit4c"].GetInvariantMassHistogram("mD0",   D0,   500);
				TH1F* hist_phi = file["fit4c"].GetInvariantMassHistogram("mphi",  phi,  500);
				// TH1F* hist_Jpsi = file["fit4c"].GetInvariantMassHistogram("mJpsi", Jpsi, 2000); //! useless plot

			// * Fit double gaussian
				if(gDo_gauss.value) {
					FitDoubleGaussian(hist_D0,   D0,   0, gLogY.value.data());
					FitDoubleGaussian(hist_phi,  phi,  2, gLogY.value.data());
					// FitDoubleGaussian(hist_Jpsi, Jpsi, 0, gLogY.value.data()); //! useless plot
				}

			// * Fit Breit-Wigner convoluted with singe Gaussian
				if(gDo_conv_s.value) {
					FitBWGaussianConvolution(hist_D0,   D0,   0, gLogY.value.data());
					FitBWGaussianConvolution(hist_phi,  phi,  2, gLogY.value.data());
					// FitBWGaussianConvolution(hist_Jpsi, Jpsi, 2, gLogY.value.data()); //! useless plot
				}

			// * Fit Breit-Wigner convoluted with double Gaussian
				if(gDo_conv_s.value) {
					FitBWDoubleGaussianConvolution(hist_D0,   D0,   0, gLogY.value.data());
					FitBWDoubleGaussianConvolution(hist_phi,  phi,  2, gLogY.value.data());
					// FitBWDoubleGaussianConvolution(hist_Jpsi, Jpsi, 2, gLogY.value.data()); //! useless plot
				}

			}

	}


	/**
	 * @brief Main function that is called when executing the executable compiled using e.g. `g++`.
	 */
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



// * ==================================== * //
// * ------- FUNCTION DEFINITIONS ------- * //
// * ==================================== * //


	int LoadConfiguration(const char* filename)
	{
		/// -# Create file stream (`ifstream`) of config `txt` file.
		ifstream file(filename);
		if(!file.is_open()) {
			cout << "WARNING: Could not load configuration file \"" << filename << "\"" << endl;
			return -1;
		}
		/// -# Print configuration title.
		cout << endl << "LOADING CONFIGURATION FROM \"" << filename << "\"" << endl;
		/// -# Loop over lines.
		string line;
		while(getline(file, line)) {
			/// <ul>
			/// <li> Remove weird characters like EOF.
				if(line.back()<' ') line.pop_back();
			/// <li> Skip line if it does <i>not</i> contain spaces or an equal sign.
				if(line.find('=') == string::npos && line.find(' ') == string::npos) continue;
			/// <li> Remove leading spaces and tabs.
				RemoveLeading(line);
				RemoveLeading(line, '\t');
			/// <li> Skip line if it is a comment.
				string beginning = line.substr(0,2);
				if(!beginning.compare("//")) continue; // C++ line comment
				if(!beginning.compare("/*")) continue; // C++ begin block comment
				if(beginning.front() == '*') continue; // C++ continue block comment
				if(beginning.front() == '#') continue; // bash comment
				if(beginning.front() == '%') continue; // LaTeX comment
			/// <li> Get paramter name and value.
				string parname  { GetParameterName (line) };
				string parvalue { GetParameterValue(line) };
			/// <li> Load value if it compares to one of the parameters.
				ArgPair_base::SetParameters(parname, parvalue);
			/// </ul>
		}
		/// -# Print loaded values in table form.
		ArgPair_base::PrintAll();
		/// @return Number of valid loaded arguments
	}


	string GetParameterName(string line)
	{
		/// -# If `line` contains an equal sign, remove everything after it.
		if(line.find('=') != string::npos) line.resize(line.find_first_of('='));
		/// -# If not, remove everything after first space.
		else if(line.find(' ') != string::npos) line.resize(line.find_first_of(' '));
		/// -# Remove trailing spaces and tabs.
		RemoveTrailing(line);
		RemoveTrailing(line, '\t');
		/// @return Parameter name as deduced from input `line`
		return line;
	}


	string GetParameterValue(string line)
	{
		/// -# If `line` contains equal sign, get everything before it.
		if(line.find('=') != string::npos) line = line.substr(line.find_first_of('=')+1);
		/// -# If not, remove everything before first space.
		else if(line.find(' ') != string::npos) line = line.substr(line.find_first_of(' ')+1);
		/// -# Remove leading spaces and tabs.
		RemoveLeading(line);
		RemoveLeading(line, '\t');
		/// -# Remove quotation marks.
		if(!line.compare("\"\"")) line = ""; // if empty string
		else Trim(line, 34); // 34 is ASCII for double quoation mark "
		/// @return Parameter value as infered from line
		return line;
	}


	/**
	 * @brief *TEMPORARY* function that serves as a fix for the bug that causes the wrong best pair to be stored.
	 */
	void DrawDifference(TH1 *histToDraw, TH1 *histToSubtract, Option_t* opt, const char* setLog)
	{
		if(!gPad) return;
		gPad->Clear();
		histToDraw->Scale(-1.);
		histToDraw->Add(histToSubtract);
		histToSubtract->SetLineColor(kWhite);
		// histToSubtract->SetMarkerColor(kWhite);
		TString option(opt);
		if(dynamic_cast<TH1F*>(histToDraw)) {
			histToSubtract->Draw(option.Data());
			option += "";
		}
		histToDraw->Draw(option.Data());
		SaveCanvas(Form("%s", histToDraw->GetName()), gPad, setLog);
	}


	void RemoveLeading(string &line, const char c)
	{
		if(line.front() == c) line = line.substr(line.find_first_not_of(c));
	}


	void RemoveTrailing(string &line, const char c)
	{
		if(line.back() == c) line.resize(line.find_last_not_of(c)+1);
	}


	void Trim(string &line, const char c)
	{
		RemoveLeading (line, c);
		RemoveTrailing(line, c);
	}

/// @}
