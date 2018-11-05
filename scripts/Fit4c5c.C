/**
 * @brief    Simple script that applies some fits to `fit4c` and `fit5c` invariant mass info from output of the `RhopiAlg` analysis job.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 * @details
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

	// * Load files and their TTree *
		// ! Choose a file that was generated using hadd
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

	// * Write results *
		std::ofstream out;
		out.open(outputFileName);
		out << "Number of iterations: " << nTimes << std::endl;
		out << "Number of events:     " << chainNEntries << std::endl;
		out << "Times for hadd file:" << std::endl;
		out << "  total (s):         " << timeHadd << std::endl;
		out << "  average (s):       " << timeHadd/nTimes << std::endl;
		out << "  per event (ns):    " << timeHadd/nTimes/chainNEntries*1e9 << std::endl;
		out << "Times for chain:" << std::endl;
		out << "  total (s):         " << timeChain << std::endl;
		out << "  average (s):       " << timeChain/nTimes << std::endl;
		out << "  per event (ns):    " << timeChain/nTimes/chainNEntries*1e9;
		out.close();
	// * Print results *
		std::cout << std::endl;
		std::cout << "Number of iterations: " << nTimes << std::endl;
		std::cout << "Number of events:     " << chainNEntries << std::endl;
		std::cout << "\033[1;4mTimes for hadd file\033[0m" << std::endl;
		std::cout << "  total (s):          " << timeHadd << std::endl;
		std::cout << "  average (s):        " << timeHadd/nTimes << std::endl;
		std::cout << "  per event (ns):     " << timeHadd/nTimes/chainNEntries*1e9 << std::endl;
		std::cout << "\033[1;4mTimes for chain\033[0m" << std::endl;
		std::cout << "  total (s):          " << timeChain << std::endl;
		std::cout << "  average (s):        " << timeChain/nTimes << std::endl;
		std::cout << "  per event (ns):     " << timeChain/nTimes/chainNEntries*1e9 << std::endl;
	// * Terminate ROOT *
		gApplication->Terminate();

}