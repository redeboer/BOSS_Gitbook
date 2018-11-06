/**
 * @brief    Simple script that applies some fits to `fit4c` and `fit5c` invariant mass info from output of the `RhopiAlg` analysis job.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 * @details  Makes use of one `hadd`ed file, not of a `TChain`.
 */


// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "TApplication.h"
	#include "TChain.h"
	#include "TFile.h"
	#include "TString.h"
	#include "TTree.h"
	#include "TStopwatch.h"
	#include <iostream>


// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //
/**
 * @brief Main function used when compiling and executing in `ROOT`.
 * @param fileName 
 */
void Fit4c5c(const char* fileName = "../data/root/ana_rhopi.root")
{

	// * Load the file and its trees *
		TFile *file = new TFile(fileName);
		TTree *treeFit4c = dynamic_cast<TTree*>(file->Get("fit4c"));
		TTree *treeFit5c = dynamic_cast<TTree*>(file->Get("fit5c"));
		// ! Pick one of its branches
		const char* treeName = "tof1";
		const char* branchName = "tpi";
		// ! Set proper branch names
		double haddRead;
		double chainRead;
		tree ->SetBranchAddress(branchName, &haddRead);
		chain->SetBranchAddress(branchName, &chainRead);

	// * Get number of entries *
	Long64_t haddNEntries  = tree ->GetEntries();
	Long64_t chainNEntries = chain->GetEntries();
	std::cout << "Number of entries in the combined ROOT file:  " <<  haddNEntries << std::endl;
	std::cout << "Number of entries in the chain of ROOT files: " << chainNEntries << std::endl;
	if(haddNEntries != chainNEntries) {
		std::cout << "--> ERROR: Number of entries is not the same!" << std::endl;
	} else {
		std::cout << "--> OK" << std::endl;
	}

	// * Start stopwatch *
		double time = 0.;
		double timeChain = 0.;
		double timeHadd  = 0.;
		TStopwatch stopwatch;

	// * Loop multiple times over *
		std::cout << std::endl;
		for(int iteration = 0; iteration < nTimes; ++iteration) {
			// * User output
			std::cout << "\rPerforming iteration " << iteration+1 << "/" << nTimes << std::flush;
			// * Loop over hadded file
			time = stopwatch.RealTime();
			stopwatch.Continue();
			for(Long64_t i = 0; i < haddNEntries; ++i) {
				tree->GetEntry(i);
			}
			timeHadd += (stopwatch.RealTime() - time);
			// * Loop over chain
			time = stopwatch.RealTime();
			stopwatch.Continue();
			for(Long64_t i = 0; i < chainNEntries; ++i) {
				chain->GetEntry(i);
			}
			timeChain += (stopwatch.RealTime() - time);
		}
		std::cout << std::endl;

	// * Terminate ROOT *
		file->Close();
		gApplication->Terminate();

}