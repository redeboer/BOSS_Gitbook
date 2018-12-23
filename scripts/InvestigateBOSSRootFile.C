/**
 * @brief    Simple script that allows to quickly investigate the contents of a ROOT file that has been produced with BOSS.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 */


// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "../inc/CommonFunctions.h"
	#include "../inc/BOSSRootFile.h"
	#include "../inc/FrameworkSettings.h"
	#include "../inc/ReconstructedParticle.h"
	#include "TH1D.h"
	#include <iostream>
	using namespace CommonFunctions;
	using namespace RooFit;
	using namespace Settings;


// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //
/**
 * @brief Main function used when compiling and executing in `ROOT`.
 */
void FitInvMassSignal()
{
	// * Open DzeroPhi input file * //
		BOSSRootFile file("../data/root/ana_DzeroPhi_mc_result3.root", true); /// To investigate the contents of the ROOT file, you first need to know which `TTree`s and branches it contains. If you simply construct the `BOSSRootFile` by giving it a file name, all `TTree`s will be loaded automatically as well as addresses for each of their branches. Five the constructer `true` as its second argument, and the names of these `TTree`s, their branches, and the types of these branches (behind the slash `/` after the name) will be printed to the terminal. <b>Do this if your macro throws an exception, because this probably means that you use the wrong names for the trees and or the branches further on in the macro.</b>
		if(file.IsZombie()) return;
		// file.PlotAllBranches();

	// * Draw useful primary vertex plots
		file.PlotDistribution("mult", "NKaonNeg", "E1");
		file.PlotDistribution("mult", "NKaonPos", "E1");
		file.PlotDistribution("mult", "NPionPos", "E1");
		file.PlotDistribution("mult", "Ncharge",  "E1");
		file.PlotDistribution("mult", "Ngood",    "E1");
		file.PlotDistribution("mult", "Nmdc",     "E1");
		file.PlotDistribution("mult", "Nneutral", "E1");
		file.PlotDistribution("mult", "Ntotal",   "E1");

	// * Draw useful primary vertex plots
		file.PlotDistribution("vertex", "vx0:vy0", "colz");
		file.PlotDistribution("vertex", "vx0:vz0", "colz");
		file.PlotDistribution("vertex", "vy0:vz0", "colz");

	// * Draw useful ToF plots
		file.PlotDistribution("tof1", "tof:ptrk", "colz");
		file.PlotDistribution("tof2", "tof:ptrk", "colz");

	// * Draw useful dEdx plots
		file.PlotDistribution("pid", "dedx:ptrk", "colz");
		file.PlotDistribution("pid", "tof1:ptrk", "colz");
		file.PlotDistribution("pid", "tof2:ptrk", "colz");

	// * Draw useful fit4c plots
		file.PlotDistribution("fit4c", "mD0");
		file.PlotDistribution("fit4c", "mphi");
		file.PlotDistribution("fit4c", "mJpsi");

	// * Particles to reconstruct * //
		// ReconstructedParticle D0  ( 421, "K^{-}#pi^{+}"); // neutral D meson
		// ReconstructedParticle phi ( 333, "K^{+}K^{-}");   // neutral phi meson
		// ReconstructedParticle Jpsi( 443, "#D^{0}#phi");   // J/psi meson

	// * Create invariant mass histogram * //
		// TH1D hist_D0   = CreateInvariantMassHistogram(D0,   200);
		// TH1D hist_phi  = CreateInvariantMassHistogram(phi,  200);
		// TH1D hist_Jpsi = CreateInvariantMassHistogram(Jpsi, 200);

	// * Loop the tree to fill inv mass spectrums * //
		// auto fit_lambda = [] (TH1D& D0, TH1D& phi, TH1D& Jpsi) {
		// 	D0.Fill  (fit4c::mD0);
		// 	phi.Fill (fit4c::mphi);
		// 	Jpsi.Fill(fit4c::mJpsi);
		// };
		// LoopTree(file.FindTree("fit4c"), fit_lambda, hist_D0, hist_phi, hist_Jpsi);
		// LoopTree(file.FindTree("fit6c"), fit_lambda, hist_D0, hist_phi, hist_Jpsi);

	// * Fit double gaussian * //
		// DrawAndSave(hist_D0, "testD0.pdf");
		// FitDoubleGaussian(hist_phi,  phi,  2);
		// FitDoubleGaussian(hist_Jpsi, Jpsi, 2);

	// * Fit Breit-Wigner * //
		// FitBreitWigner(hist_D0,   D0);
		// FitBreitWigner(hist_phi,  phi, 2);
		// FitBreitWigner(hist_Jpsi, Jpsi, 2);

	// * Fit Breit-Wigner convoluted with double Gaussian * //
		// FitBWDoubleGaussianConvolution(hist_D0,   D0);
		// FitBWDoubleGaussianConvolution(hist_phi,  phi,  2);
		// FitBWDoubleGaussianConvolution(hist_Jpsi, Jpsi, 2);

}

/**
 * @brief Main function that is called when executing the executable compiled using e.g. `g++`.
 */
int main()
{
	FitInvMassSignal();
	return 0;
}