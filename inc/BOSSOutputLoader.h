#ifndef BOSS_Afterburner_BOSSOutputLoader_H
#define BOSS_Afterburner_BOSSOutputLoader_H

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "ChainLoader.h"
	#include "TChain.h"
	#include "TString.h"
	#include <list>
	#include <string>
	#include <unordered_map>



// * ================================= * //
// * ------- CLASS DEFINITIONS ------- * //
// * ================================= * //
/// @addtogroup BOSS_Afterburner_objects
/// @{


	/// General container for BOSS analysis output
	/// This object only needs a path of a directory containin ROOT files from the initial analysis or of one such root file. All `TTree`s and the branches in these files will be loaded automatically, so you don't have to worry about names of those trees. This object also comes with several methods that faciliate e.g. plotting of the `TTree` contents.
	/// @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date     October 24th, 2018
	class BOSSOutputLoader
	{
	public:
		/// @name Constructors
			///@{
			BOSSOutputLoader() {}
			BOSSOutputLoader(const char* directoryPath, bool print_branches = true, bool print_averages = false);
			///@}

		/// @name Information functions
			///@{
			TH1F* Draw(const char* treeName, const char* branchX, const Int_t nBinx, const double x1, const double x2, Option_t *option="", const TString &logScale="", const char* cut="");
			TH2F* Draw(const char* treeName, const char* branchX, const char* branchY, const Int_t nBinx, const double x1, const double x2, const Int_t nBiny, const double y1, const double y2, Option_t *option="", const TString &logScale="", const char* cut="");
			bool IsZombie();
			void Draw(const char* treeName, const char* branchNames, const char* cut="", Option_t *option="", const TString &logScale="");
			void DrawAndSaveAllBranches(const char* treeName, Option_t *option="", const TString &logScale="");
			void Print();
			void Print(const char* nameOfTree, Option_t *option = "toponly");
			void PrintCutFlow();
			void PrintCuts();
			void PrintTrees(Option_t *option="");
			void QuickDrawAndSaveAll(Option_t *option="");
			///@}

		/// @name Getters
			///@{
			Long64_t GetEntries(const char* treeName);
			Long64_t GetLargestEntries() const;
			ChainLoader& operator[](const char* name) { return fChains.at(name); }
			TChain& FindTree(const char* treeName);
			std::unordered_map<std::string, ChainLoader>& GetChains() { return fChains; }
			///@}

	protected:
		/// @name Data members
			///@{
			TString fDirectoryPath; ///< The path of the directory that was was used to load the ROOT files and generate the `TChain`s in this object.
			std::list<TString> fFileNames; ///< List of filenames that were loaded from the directory.
			std::unordered_map<std::string, ChainLoader> fChains; ///< Map of `TChain`s of the loaded ROOT files (contained withing `ChainLoader`).
			///@}

		/// @name Helper methods
			///@{
			std::list<ChainLoader*> CreateOrderedMap();
			void LoadChains(bool print_branches=true, bool print_averages=false);
			///@}

	};


/// @}
#endif