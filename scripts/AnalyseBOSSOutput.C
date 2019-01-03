/**
 * @brief    Simple script that allows to quickly investigate the contents of a ROOT file that has been produced with BOSS.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 */



// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "../inc/BOSSOutputLoader.h"
	#include "TStyle.h"
	#include <iostream>
	#include <utility>
	#include <fstream>
	#include <sstream>
	using namespace CommonFunctions::Draw;
	using namespace CommonFunctions::Fit;
	using namespace RooFit;
	using namespace Settings;
	using namespace std;



// * ================================ * //
// * ------- DEFAULT SETTINGS ------- * //
// * ================================ * //

	// * STRUCT DEFINITION * //
	template<typename TYPE>
	struct ArgPair {
		string name;
		TYPE value;
		void SetValue(const string &input) {
			istringstream ss(input);
			ss >> value;
		}
		void operator=(const TYPE &val) { value = val; }
		bool operator==(const string &str) const { return !name.compare(str); }
		bool operator!=(const string &str) const { return  name.compare(str); }
		void print() { cout << "  " << name << " = " << value << endl; }
	};

	// * FILE NAME * //
	ArgPair<string> gFilename { "InputFile", "/mnt/d/IHEP/root/D0phi_KpiKK_data" };

	// * TERMINAL OUTPUT * //
	ArgPair<bool> gPrint { "Print branches", false };

	// * PLOT STYLE * //
	ArgPair<bool> gSetranges   { "set plot ranges", true  }; //!< Whether or not to precisely set histogram ranges.
	ArgPair<bool> gPlotstats   { "gPlotstats",   false }; //!< Whether or not to draw the legend in the upper right corner with histogram statistics.

	// * WHICH BRANCHES TO PLOT * //
	ArgPair<bool> gPureplot    { "gPureplot",    true  }; //!< Whether or not to plot histograms of branches <i>without fit</i>.
	ArgPair<bool> gDraw_mult   { "gDraw_mult",   false }; //!< Whether or not to draw the multiplicity branches.
	ArgPair<bool> gDraw_vertex { "gDraw_vertex", false }; //!< Whether or not to draw the `"vertex"` branch.
	ArgPair<bool> gDraw_tof    { "gDraw_tof",    false }; //!< Whether or not to draw the `"tof*"` branches.
	ArgPair<bool> gDraw_fit    { "gDraw_fit",    true  }; //!< Whether or not to draw the `"fit"` branches.

	// * FIT SETTINGS * //
	ArgPair<bool> gFitplots    { "gFitplots",    false }; //!< Whether or not to produce invariant mass fits.
	ArgPair<bool> gDo_gauss    { "gDo_gauss",    true  }; //!< Whether or not to produce perform a double Gaussian fit.
	ArgPair<bool> gDo_conv_s   { "gDo_conv_s",   false }; //!< Whether or not to produce perform a Breit-Wigner convoluted with a <i>single</i> Gaussian.
	ArgPair<bool> gDo_conv_d   { "gDo_conv_d",   false }; //!< Whether or not to produce perform a Breit-Wigner convoluted with a <i>double</i> Gaussian.



// * ===================================== * //
// * ------- FUNCTION DECLARATIONS ------- * //
// * ===================================== * //

	int LoadConfiguration(const char* filename);
	string GetParameterName(string line);
	string GetParameterValue(string line);
	void DrawDifference(TH1F *histToDraw, TH1F *histToSubtract, const char* setLog="");
	void RemoveLeading(string &line, const char c=' ');
	void RemoveTrailing(string &line, const char c=' ');
	void Trim(string &line, const char c=' ');



// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //


	/**
	 * @brief Main function used when compiling and executing in `ROOT`.
	 */
	void AnalyseBOSSOutput()
	{


		// * OPEN INPUT FILE * //
			BOSSOutputLoader file(gFilename.value.data(), gPrint.value); /// To investigate the contents of the ROOT file, you first need to know which `TTree`s and branches it contains. If you simply construct the `BOSSOutputLoader` by giving it a file name, all `TTree`s will be loaded automatically as well as addresses for each of their branches. Five the constructer `true` as its second argument, and the names of these `TTree`s, their branches, and the types of these branches (behind the slash `/` after the name) will be printed to the terminal. <b>Do this if your macro throws an exception, because this probably means that you use the wrong names for the trees and or the branches further on in the macro.</b>
			if(file.IsZombie()) return;
			LoadConfiguration("configs/D0phi_KpiKK_MC.txt");
return;
			if(!gPlotstats.value) gStyle->SetOptStat(0);
			file.PrintCutFlow();

		// * PLOT BRANCHES WITHOUT FITS * //
			if(gPureplot.value) {
				if(gDraw_mult.value) {
					for(auto tree = file.GetChains().begin(); tree != file.GetChains().end(); ++tree) {
						TString name(tree->second.GetChain().GetName());
						if(name.BeginsWith("mult")) tree->second.DrawAndSaveAllMultiplicityBranches("", "");
					}
				}
				if(gDraw_vertex.value) {
					if(gSetranges.value) {
						file.DrawBranches("vertex", "vy0:vx0", "colz");
						file.DrawBranches("vertex", "vz0:vx0", "colz");
						file.DrawBranches("vertex", "vz0:vy0", "colz");
					} else {
						file.DrawBranches("vertex", "vx0", "vy0", 60, -.154, -.146,  40,  .08,   .122, "colz");
						file.DrawBranches("vertex", "vx0", "vz0", 60, -.4,    .5,    60,  .08,   .122, "colz");
						file.DrawBranches("vertex", "vy0", "vz0", 60, -.4,    .5,    40, -.154, -.146, "colz");
					}
				}
				if(gDraw_tof.value) {
					if(gSetranges.value) {
						file.DrawBranches("ToFIB", "p", "tof", 120, 2., 15., 80, 0., 1.5, "colz", "z");
						file.DrawBranches("ToFOB", "p", "tof", 120, 2., 15., 80, 0., 1.5, "colz", "z");
					} else {
						file.DrawBranches("ToFIB", "tof:p", "colz");
						file.DrawBranches("ToFOB", "tof:p", "colz");
					}
				}
				if(gDraw_fit.value) {
					if(gSetranges.value) {
						auto fit4c_all_mD0        = (TH1F*)(file.DrawBranches("fit4c_all",      "mD0",   100,  .7,    2.,      "E1", "")->Clone("fit4c_all/mD0_inv"));
						auto fit4c_all_mphi       = (TH1F*)(file.DrawBranches("fit4c_all",      "mphi",  100,  .97,   1.7,     "E1", "")->Clone("fit4c_all/mphi_inv"));
						auto fit4c_all_mJpsi      = (TH1F*)(file.DrawBranches("fit4c_all",      "mJpsi", 100, 3.0967, 3.09685, "E1", "")->Clone("fit4c_all/mJpsi_inv"));
						auto fit4c_best_mD0       = (TH1F*)(file.DrawBranches("fit4c_best",     "mD0",   100,  .7,    2.,      "E1", "")->Clone("fit4c_best/mD0_inv"));
						auto fit4c_best_mphi      = (TH1F*)(file.DrawBranches("fit4c_best",     "mphi",  100,  .97,   1.7,     "E1", "")->Clone("fit4c_best/mphi_inv"));
						auto fit4c_best_mJpsi     = (TH1F*)(file.DrawBranches("fit4c_best",     "mJpsi", 100, 3.0967, 3.09685, "E1", "")->Clone("fit4c_best/mJpsi_inv"));
						auto fit4c_best_D0_mD0    = (TH1F*)(file.DrawBranches("fit4c_best_D0",  "mD0",   100,  .7,    2.,      "E1", "")->Clone("fit4c_best_D0/mD0_inv"));
						auto fit4c_best_D0_mphi   = (TH1F*)(file.DrawBranches("fit4c_best_D0",  "mphi",  100,  .97,   1.7,     "E1", "")->Clone("fit4c_best_D0/mphi_inv"));
						auto fit4c_best_D0_mJpsi  = (TH1F*)(file.DrawBranches("fit4c_best_D0",  "mJpsi", 100, 3.0967, 3.09685, "E1", "")->Clone("fit4c_best_D0/mJpsi_inv"));
						auto fit4c_best_phi_mD0   = (TH1F*)(file.DrawBranches("fit4c_best_phi", "mD0",   100,  .7,    2.,      "E1", "")->Clone("fit4c_best_phi/mD0_inv"));
						auto fit4c_best_phi_mphi  = (TH1F*)(file.DrawBranches("fit4c_best_phi", "mphi",  100,  .97,   1.7,     "E1", "")->Clone("fit4c_best_phi/mphi_inv"));
						auto fit4c_best_phi_mJpsi = (TH1F*)(file.DrawBranches("fit4c_best_phi", "mJpsi", 100, 3.0967, 3.09685, "E1", "")->Clone("fit4c_best_phi/mJpsi_inv"));
						DrawDifference(fit4c_best_mD0,       fit4c_all_mD0);
						DrawDifference(fit4c_best_mphi,      fit4c_all_mphi);
						DrawDifference(fit4c_best_mJpsi,     fit4c_all_mJpsi);
						DrawDifference(fit4c_best_D0_mD0,    fit4c_all_mD0);
						DrawDifference(fit4c_best_D0_mphi,   fit4c_all_mphi);
						DrawDifference(fit4c_best_D0_mJpsi,  fit4c_all_mJpsi);
						DrawDifference(fit4c_best_phi_mD0,   fit4c_all_mD0);
						DrawDifference(fit4c_best_phi_mphi,  fit4c_all_mphi);
						DrawDifference(fit4c_best_phi_mJpsi, fit4c_all_mJpsi);
					} else {
						file.DrawBranches("fit4c_all",      "mD0",   "E1", "y");
						file.DrawBranches("fit4c_best",     "mD0",   "E1", "y");
						file.DrawBranches("fit4c_best_D0",  "mD0",   "E1", "y");
						file.DrawBranches("fit4c_best_phi", "mD0",   "E1", "y");
						file.DrawBranches("fit4c_all",      "mphi",  "E1", "y");
						file.DrawBranches("fit4c_best",     "mphi",  "E1", "y");
						file.DrawBranches("fit4c_best_D0",  "mphi",  "E1", "y");
						file.DrawBranches("fit4c_best_phi", "mphi",  "E1", "y");
						file.DrawBranches("fit4c_all",      "mJpsi", "E1", "y");
						file.DrawBranches("fit4c_best",     "mJpsi", "E1", "y");
						file.DrawBranches("fit4c_best_D0",  "mJpsi", "E1", "y");
						file.DrawBranches("fit4c_best_phi", "mJpsi", "E1", "y");
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
					FitDoubleGaussian(hist_D0,   D0,   0, "y");
					FitDoubleGaussian(hist_phi,  phi,  2, "y");
					// FitDoubleGaussian(hist_Jpsi, Jpsi, 0, "y"); //! useless plot
				}

			// * Fit Breit-Wigner convoluted with singe Gaussian
				if(gDo_conv_s.value) {
					FitBWGaussianConvolution(hist_D0,   D0,   0, "y");
					FitBWGaussianConvolution(hist_phi,  phi,  2, "y");
					// FitBWGaussianConvolution(hist_Jpsi, Jpsi, 2, "y"); //! useless plot
				}

			// * Fit Breit-Wigner convoluted with double Gaussian
				if(gDo_conv_s.value) {
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
		AnalyseBOSSOutput();
		return 0;
	}



// * ==================================== * //
// * ------- FUNCTION DEFINITIONS ------- * //
// * ==================================== * //

template<typename TYPE>
void SetParameter(ArgPair<TYPE> &par, const string &parname, const string &parvalue)
{
	if(par != parname) return;
	par.SetValue(parvalue);
	par.print();
}
void SetParameter(ArgPair<string> &par, const string &parname, const string &parvalue)
{
	if(par != parname) return;
	par = parvalue;
	par.print();
}
void SetParameter(ArgPair<bool> &par, const string &parname, const string &parvalue)
{
	if(par != parname) return;
	if(!parvalue.compare("false") || parvalue.front()=='0') par = false;
	else par = true;
	par.print();
}


	int LoadConfiguration(const char* filename)
	{
		/// -# Create file stream (`ifstream`) of config `txt` file.
		ifstream file(filename);
		if(!file.is_open()) return -1;
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
				SetParameter(gFilename,    parname, parvalue);
				SetParameter(gPrint,       parname, parvalue);
				SetParameter(gSetranges,   parname, parvalue);
				SetParameter(gPlotstats,   parname, parvalue);
				SetParameter(gPureplot,    parname, parvalue);
				SetParameter(gDraw_mult,   parname, parvalue);
				SetParameter(gDraw_vertex, parname, parvalue);
				SetParameter(gDraw_tof,    parname, parvalue);
				SetParameter(gDraw_fit,    parname, parvalue);
				SetParameter(gFitplots,    parname, parvalue);
				SetParameter(gDo_gauss,    parname, parvalue);
				SetParameter(gDo_conv_s,   parname, parvalue);
				SetParameter(gDo_conv_d,   parname, parvalue);
			/// </ul>
		}
		cout << endl;
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
		Trim(line, 34); // 34 is ASCII for double quoation mark "
		/// @return Parameter value as infered from line
		return line;
	}


	/**
	 * @brief *TEMPORARY* function that serves as a fix for the bug that causes the wrong best pair to be stored.
	 */
	void DrawDifference(TH1F *histToDraw, TH1F *histToSubtract, const char* setLog)
	{
		if(!gPad) return;
		gPad->Clear();
		histToDraw->Scale(-1.);
		histToDraw->Add(histToSubtract);
		histToDraw->Draw("E1");
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