#ifndef BOSS_Afterburner_ChainLoader_H
#define BOSS_Afterburner_ChainLoader_H

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "ReconstructedParticle.h"
	#include "TChain.h"
	#include "TH1F.h"
	#include "TH2F.h"
	#include "TObject.h"
	#include "TTree.h"
	#include <string>
	#include <unordered_map>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_Afterburner_objects
/// @{


	/**
	 * @brief    A container class for a `TChain` that allows easy access to its branches. Addresses are set automatically upon construction.
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     December 21st, 2018
	 */
	class ChainLoader
	{
	public:

		// * CONSTRUCTORS AND DESTRUCTORS * //
		ChainLoader() {}
		ChainLoader(TTree* tree) : fChain(tree->GetName(), tree->GetTitle()) {}

		// * GETTERS * //
			TChain& GetChain() { return fChain; }
			std::unordered_map<std::string, Char_t>&    Get_B() { return fMap_B; }
			std::unordered_map<std::string, UChar_t>&   Get_b() { return fMap_b; }
			std::unordered_map<std::string, Short_t>&   Get_S() { return fMap_S; }
			std::unordered_map<std::string, UShort_t>&  Get_s() { return fMap_s; }
			std::unordered_map<std::string, Int_t>&     Get_I() { return fMap_I; }
			std::unordered_map<std::string, UInt_t>&    Get_i() { return fMap_i; }
			std::unordered_map<std::string, Float_t>&   Get_F() { return fMap_F; }
			std::unordered_map<std::string, Double_t>&  Get_D() { return fMap_D; }
			std::unordered_map<std::string, Long64_t>&  Get_L() { return fMap_L; }
			std::unordered_map<std::string, ULong64_t>& Get_l() { return fMap_l; }
			std::unordered_map<std::string, Bool_t>&    Get_O() { return fMap_O; }
			Char_t&    Get_B(const char* name) { return fMap_B.at(name); }
			UChar_t&   Get_b(const char* name) { return fMap_b.at(name); }
			Short_t&   Get_S(const char* name) { return fMap_S.at(name); }
			UShort_t&  Get_s(const char* name) { return fMap_s.at(name); }
			Int_t&     Get_I(const char* name) { return fMap_I.at(name); }
			UInt_t&    Get_i(const char* name) { return fMap_i.at(name); }
			Float_t&   Get_F(const char* name) { return fMap_F.at(name); }
			Double_t&  Get_D(const char* name) { return fMap_D.at(name); }
			Long64_t&  Get_L(const char* name) { return fMap_L.at(name); }
			ULong64_t& Get_l(const char* name) { return fMap_l.at(name); }
			Bool_t&    Get_O(const char* name) { return fMap_O.at(name); }
			Long64_t   GetEntries() const { return fChain.GetEntries(); }

		// * SETTERS * //
			Int_t Add(const char* filename) { return fChain.Add(filename); }

		// * INFORMATION * //
			TH1F* Draw(const char* branchX, const Int_t nBinx, const double x1, const double x2, Option_t *option="", const bool save=true, const TString &logScale="", const char* cut="");
			TH1F* GetInvariantMassHistogram(const char* branchName, const ReconstructedParticle& particle, const int nBins=100, Option_t *option="", const TString &logScale="");
			TH2F* Draw(const char* branchX, const char* branchY, const Int_t nBinx, const double x1, const double x2, const Int_t nBiny, const double y1, const double y2, Option_t *option="", const bool save=true, const TString &logScale="", const char* cut="");
			double ComputeMean(TChain* tree, const char* branchName);
			void BookAddresses(bool print=false);
			void Draw(const char* branchNames, const char* cut="", Option_t *option="", const bool save=true, const TString &logScale="");
			void DrawAndSaveAllBranches(Option_t *option="", const TString &logScale="");
			void DrawAndSaveAllMultiplicityBranches(const TString &logScale="", Option_t *option="E1");

	private:

		// * DATA MEMBERS * //
			TChain fChain; //!< `TChain` object to which the `TFile`s are added.
			std::unordered_map<std::string, Char_t>    fMap_B; //!< Map of addresses for Char_t (8 bit signed integer).
			std::unordered_map<std::string, UChar_t>   fMap_b; //!< Map of addresses for UChar_t (8 bit unsigned integer).
			std::unordered_map<std::string, Short_t>   fMap_S; //!< Map of addresses for Short_t (16 bit signed integer).
			std::unordered_map<std::string, UShort_t>  fMap_s; //!< Map of addresses for UShort_t (16 bit unsigned integer).
			std::unordered_map<std::string, Int_t>     fMap_I; //!< Map of addresses for Int_t (32 bit signed integer).
			std::unordered_map<std::string, UInt_t>    fMap_i; //!< Map of addresses for UInt_t (32 bit unsigned integer).
			std::unordered_map<std::string, Float_t>   fMap_F; //!< Map of addresses for Float_t (32 bit floating point).
			std::unordered_map<std::string, Double_t>  fMap_D; //!< Map of addresses for Double_t (64 bit floating point).
			std::unordered_map<std::string, Long64_t>  fMap_L; //!< Map of addresses for Long64_t (64 bit signed integer).
			std::unordered_map<std::string, ULong64_t> fMap_l; //!< Map of addresses for ULong64_t (64 bit unsigned integer).
			std::unordered_map<std::string, Bool_t>    fMap_O; //!< Map of addresses for Bool_t (boolean).

		// * PRIVATE METHODS * //
			template<typename TYPE>
			void SetAddress(TObject* obj, std::unordered_map<std::string, TYPE> &map);
			template<typename TYPE>
			void SetAddressSafe(TObject* obj, std::unordered_map<std::string, TYPE> &map);

	};


/// @}
#endif