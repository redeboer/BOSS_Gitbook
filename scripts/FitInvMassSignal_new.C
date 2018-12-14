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
	#include "../inc/FitObjectDoubleGauss.h"
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
 */
void FitInvMassSignal()
{
	// * Open RhoPi input file * //
		RhopiRootFile file("../data/root/181111/ana_rhopi_data.root");
		if(file.IsZombie()) return;

	// * Particles to reconstruct * //
		ReconstructedParticle pi0(111, "#gamma#gamma");

	// * Create invariant mass histogram * //
		TH1D hist_pi0 = CreateInvariantMassHistogram(pi0,  400);

	// * Loop the tree to fill inv mass spectrums * //
		auto fit4c_lambda = [] (TH1D& pi0) {
			pi0.Fill(fit4c::mpi0);
		};
		LoopTree(file.FindTree("fit4c"), fit4c_lambda, hist_pi0);

	// * Fit double gaussian * //
		FitObjectDoubleGauss fit_pi0(hist_pi0, pi0);
		fit_pi0.PerformFit();


}

/**
 * @brief Main function that is called when executing the executable compiled using e.g. `g++`.
 */
int main()
{
	FitInvMassSignal();
	return 0;
}