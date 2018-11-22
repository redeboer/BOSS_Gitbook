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
	#include "../inc/CommonFunctions.h"
	#include "../inc/RhopiRootFile.h"
	#include "TH1D.h"
	#include <iostream>
	using namespace RooFit;
	using namespace AnaBranches;
	using namespace Settings;
	using namespace CommonFunctions;


// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //
/**
 * @brief Main function used when compiling and executing in `ROOT`.
 * @param inputFileName 
 */
void FitInvMassSignal()
{
	// * Open RhoPi input file * //
		RhopiRootFile file("../data/root/ana_rhopi_data.root");
		if(file.IsZombie()) return;

	// * Particles to reconstruct * //
		ReconstructedParticle pi0 ( 111, "#gamma#gamma"); // neutral rho meson
		ReconstructedParticle rho0( 113, "#pi^{+}#pi^{-}"); // neutral rho meson
		ReconstructedParticle rhop( 213, "#pi^{+}#pi^{0}"); // positive rho meson
		ReconstructedParticle rhom(-213, "#pi^{-}#pi^{0}"); // negative rho meson

	// * Create invariant mass histogram * //
		TH1D hist_pi0  = CreateInvariantMassHistogram(pi0,  400);
		TH1D hist_rho0 = CreateInvariantMassHistogram(rho0, 200);
		TH1D hist_rhop = CreateInvariantMassHistogram(rhop, 200);
		TH1D hist_rhom = CreateInvariantMassHistogram(rhom, 200);

	// * Loop the tree to fill inv mass spectrums * //
		auto fit4c_lambda = [] (TH1D& pi0) {
			pi0.Fill(fit4c::mpi0);
		};
		auto fit5c_lambda = [] (TH1D& rho0, TH1D& rhop, TH1D& rhom) {
			rho0.Fill(fit5c::mrho0);
			rhop.Fill(fit5c::mrhop);
			rhom.Fill(fit5c::mrhom);
		};
		LoopTree(file.FindTree("fit4c"), fit4c_lambda, hist_pi0);
		LoopTree(file.FindTree("fit5c"), fit5c_lambda, hist_rho0, hist_rhop, hist_rhom);

	// * Fit double gaussian * //
		FitDoubleGaussian(hist_pi0,  pi0);
		FitDoubleGaussian(hist_rho0, rho0, 2);
		FitDoubleGaussian(hist_rhop, rhop, 2);
		FitDoubleGaussian(hist_rhom, rhom, 2);

	// // * Fit Breit-Wigner * //
		FitBreitWigner(hist_pi0,  pi0);
		FitBreitWigner(hist_rho0, rho0, 2);
		FitBreitWigner(hist_rhop, rhop, 2);
		FitBreitWigner(hist_rhom, rhom, 2);

	// // * Fit Breit-Wigner convoluted with double Gaussian * //
		FitBWDoubleGaussianConvolution(hist_pi0,  pi0);
		FitBWDoubleGaussianConvolution(hist_rho0, rho0, 2);
		FitBWDoubleGaussianConvolution(hist_rhop, rhop, 2);
		FitBWDoubleGaussianConvolution(hist_rhom, rhom, 2);

}

/**
 * @brief Main function that is called when executing the executable compiled using e.g. `g++`.
 */
int main()
{
	FitInvMassSignal();
	return 0;
}