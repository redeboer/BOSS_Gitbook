/**
 * @brief    Simple script that shows how to fit a double gaussian on an invariant mass spectrum using `RooFit`.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 * @details  Note that this script makes use of one `hadd`ed file, not of a `TChain`. It is currently assumed that the invariant mass plot to be fitted comes from the `mpi0` branch in the `fit6c` that is produced by the `DzeroPhiAlg` analysis job. But this can be modified in the sections that have been marked by `// ! ... ! //`.
 */


// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "../inc/CommonFunctions.h"
	#include "../inc/DzeroPhiRootFile.h"
	#include "../inc/FrameworkSettings.h"
	#include "../inc/ReconstructedParticle.h"
	#include "TH1D.h"
	#include <iostream>
	using namespace CommonFunctions;
	using namespace DzeroPhiBranches;
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
		DzeroPhiRootFile file("../data/root/ana_DzeroPhi_mc_result2.root");
		if(file.IsZombie()) return;
TCanvas c;
file.FindTree("fit4c")->Draw("mD0");   c.SaveAs("mD0.pdf");
file.FindTree("fit4c")->Draw("mJpsi"); c.SaveAs("mJpsi.pdf");
file.FindTree("fit4c")->Draw("mphi");  c.SaveAs("mphi.pdf");
file.FindTree("fit4c")->Draw("chi2");  c.SaveAs("chi2.pdf");

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