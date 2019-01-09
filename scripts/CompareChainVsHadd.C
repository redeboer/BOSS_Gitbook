/// @addtogroup BOSS_Afterburner_scripts
/// @{
	
/**
 * @brief    Simple script that compares the speed of looping over a `TChain` of ROOT files and over one  combined ROOT file (where all the ROOT files have been combined using `hadd`).
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     November 5th, 2018
 * @details  This macro does the same loop several times (`nTimes` with default 10) as to ensure a fair comparison between the two loop modes. The computation times for both the `TChain` and the `hadd` method are printed and written out to a log file. Note that the reading operation is quite simple: only one branch address is loaded.
 */


// * ======================================== * //
// * ------- LIBRARIES AND NAMESPACES ------- * //
// * ======================================== * //
	#include "TChain.h"
	#include "TFile.h"
	#include "TString.h"
	#include "TTree.h"
	#include "TStopwatch.h"
	#include <iostream>
	#include <fstream> // for writing to a file
	#include <sstream> // needed to import arguments in `main` function


// * ======================= * //
// * ------- GLOBALS ------- * //
// * ======================= * //
	const char* gDefaultOutputFileName = "comparison.log";
	const int   gDefaultNTimes         = 10;
	// ! Customise these values ! //
	const char* gFileToLoad  = "../data/root/ana_rhopi.root";
	const char* gTreeName    = "tof1";
	const char* gBrancheName = "tpi";


// * ============================= * //
// * ------- MAIN FUNCTION ------- * //
// * ============================= * //
/**
 * @brief Main function used when compiling and executing in `ROOT`.
 */
void CompareChainVsHadd(int nTimes = gDefaultNTimes, const char* outputFileName = gDefaultOutputFileName)
{

	// * Attempt to load file * //
		TFile *file = new TFile(gFileToLoad);
		if(!file || file->IsZombie()) {
			if(file) file->Close();
			return;
		}

	// * Attempt to load TTree from file * //
		TTree *tree = dynamic_cast<TTree*>(file->Get(gTreeName));
		if(!tree) {
			std::cout << "FATAL ERROR: File \"" << gFileToLoad << "\" does not contain a tree \"" << gTreeName << "\"" << std::endl;
			file->Close();
			return;
		}

	// * Load tchains * //
		TChain *chain = new TChain(gTreeName);
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

	// * Set branch addresses * //
		double haddRead;
		double chainRead;
		tree ->SetBranchAddress(gBrancheName, &haddRead);
		chain->SetBranchAddress(gBrancheName, &chainRead);

	// * Get number of entries *
		Long64_t haddNEntries  = tree ->GetEntries();
		Long64_t chainNEntries = chain->GetEntries();
		std::cout << "Number of entries in the combined ROOT file:  " <<  haddNEntries << std::endl;
		std::cout << "Number of entries in the chain of ROOT files: " << chainNEntries << std::endl;
		if(haddNEntries != chainNEntries) {
			std::cout << "--> ERROR: Number of entries is not the same!" << std::endl;
			file->Close();
			return;
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

}

/**
 * @brief Main function when directly compiling using e.g. `g++`.
 */
int main(int argc, char *argv[])
{
	// * Default arguments * //
	int nTimes = gDefaultNTimes;
	TString outputFileName(gDefaultOutputFileName);

	//* Import arguments if available * //
	if(argc > 1) { // if at least one argument is given
		std::stringstream str(argv[1]);
		str >> nTimes; // convert argument to integer
	}
	if(argc > 2) { // if at least two arguments are given
		outputFileName = argv[2];
	}

	// * Execute function * //
	CompareChainVsHadd(nTimes, outputFileName.Data());

	// * Default return value * //
	return 0;
}

/// @}
