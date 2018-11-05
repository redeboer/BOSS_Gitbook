/**
 * @brief    Simple script that compares the speed of looping over a `TChain` of ROOT files and over one  combined ROOT file (where all the ROOT files have been combined using `hadd`).
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 * @remark   @b DEVELOPMENTAL
 *
 * @details  This macro does the same loop several times as to ensure a fair comparison between the two loop modes.
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
 * @details
 */
void CompareChainVsHadd()
{

	// * Load files and their TTree *
		// ! Set number of times you want to loop over the trees
		int nTimes = 100;
		// ! Pick one of its branches
		const char* treeName = "tof1";
		const char* branchName = "tof1";
		// ! Choose a file that was generated using hadd
		TFile *file = new TFile("../data/root/ana_rhopi.root");
		TTree *tree = dynamic_cast<TTree*>(file->Get(treeName));
		// ! Set these file names
		TChain *chain = new TChain(treeName);
		chain->Add("../data/root/ana_rhopi_0.root");
		chain->Add("../data/root/ana_rhopi_1.root");
		chain->Add("../data/root/ana_rhopi_2.root");
		chain->Add("../data/root/ana_rhopi_3.root");
		chain->Add("../data/root/ana_rhopi_4.root");
		chain->Add("../data/root/ana_rhopi_5.root");
		chain->Add("../data/root/ana_rhopi_6.root");
		chain->Add("../data/root/ana_rhopi_7.root");
		chain->Add("../data/root/ana_rhopi_8.root");
		chain->Add("../data/root/ana_rhopi_9.root");
		chain->Add("../data/root/ana_rhopi_10.root");
		chain->Add("../data/root/ana_rhopi_11.root");
		chain->Add("../data/root/ana_rhopi_12.root");
		chain->Add("../data/root/ana_rhopi_13.root");
		chain->Add("../data/root/ana_rhopi_14.root");
		chain->Add("../data/root/ana_rhopi_15.root");
		chain->Add("../data/root/ana_rhopi_16.root");
		chain->Add("../data/root/ana_rhopi_17.root");
		chain->Add("../data/root/ana_rhopi_18.root");
		chain->Add("../data/root/ana_rhopi_19.root");
		chain->Add("../data/root/ana_rhopi_20.root");
		chain->Add("../data/root/ana_rhopi_21.root");
		chain->Add("../data/root/ana_rhopi_22.root");
		chain->Add("../data/root/ana_rhopi_23.root");
		chain->Add("../data/root/ana_rhopi_24.root");
		// ! Set proper branch names
		double haddRead;
		double chainRead;
		haddTree->SetBranchAddress("tpi", haddRead);
		chain   ->SetBranchAddress("tpi", chainRead);

	// * Get number of entries *
	Long64_t haddNEntries  = haddTree->GetEntries();
	Long64_t chainNEntries = chain   ->GetEntries();
	std::cout << "Number of entries in the combined ROOT file:  " <<  haddNEntries << std::endl;
	std::cout << "Number of entries in the chain of ROOT files: " << chainNEntries << std::endl;
	if(haddNEntries != chainNEntries) {
		std::cout << "--> ERROR: Number of entries is not the same!" << std::endl;
	}

	// * Start stopwatch *
		double time = 0.;
		double timeChain = 0.;
		double timeHadd  = 0.;
		TStopwatch stopwatch;

	// * Loop multiple times over *
		for(int iteration = 0; iteration < ; ++iteration) {
			// * Loop over hadded file
			time = stopwatch.RealTime();
			stopwatch.Continue();
			for(Long64_t i = 0; i < haddNEntries; ++i) {
				haddTree->GetEntry(i);
			}
			timeHadd += (stopwatch.RealTime() - time1);
			// * Loop over chain
			time = stopwatch.RealTime();
			stopwatch.Continue();
			for(Long64_t i = 0; i < chainNEntries; ++i) {
				chain->GetEntry(i);
			}
			timeChain += (stopwatch.RealTime() - time1);
		}

	// * Print results *
		std::cout << "Times for hadd file:" << std::endl;
		std::cout << "  total:   " << timeHadd        << std::endl;
		std::cout << "  average: " << timeHadd/nTimes << std::endl;
		std::cout << "Times for chain:" << std::endl;
		std::cout << "  total:   " << timeChain        << std::endl;
		std::cout << "  average: " << timeChain/nTimes << std::endl;
	// * Terminate ROOT *
		gApplication->Terminate();

}