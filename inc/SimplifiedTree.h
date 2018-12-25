#ifndef Physics_Analysis_SimplifiedTree_H
#define Physics_Analysis_SimplifiedTree_H

/**
 * @brief    A container class for a `TTree` that allows easy access to its branches. Addresses are set automatically upon construction.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 21st, 2018
 */

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "../inc/FrameworkSettings.h"
	#include "../inc/CommonFunctions.h"
	#include "TList.h"
	#include "TObject.h"
	#include "TTree.h"
	#include "TH1F.h"
	#include "TH2F.h"
	#include <iostream>
	#include <unordered_map>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
	class SimplifiedTree
	{
	public:

		// * CONSTRUCTORS AND DESTRUCTORS * //
		SimplifiedTree() : fTree(nullptr) {}
		SimplifiedTree(TTree* fTree, bool print = true);

		// * GETTERS * //
			TTree* Get() { return fTree; }
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

		// * INFORMATION * //
			TH1F* DrawBranches(const char* branchX, const Int_t nBinx, const double x1, const double x2, const bool save=true, Option_t* opt="", const TString &logScale="");
			TH1F* GetInvariantMassHistogram(const char* branchName, const ReconstructedParticle& particle, const int nBins=100, Option_t *opt="", const TString &logScale="");
			TH2F* DrawBranches(const char* branchX, const char* branchY, const Int_t nBinx, const double x1, const double x2, const Int_t nBiny, const double y1, const double y2, const bool save=true, Option_t* opt="", const TString &logScale="");
			void DrawAndSaveAllBranches(Option_t* opt="", const TString &logScale="");
			void DrawBranches(const char* branchNames, const bool save=true, Option_t* opt="", const TString &logScale="");

	private:

		// * DATA MEMBERS * //
			TTree* fTree;
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
			template<typename TYPE> void SetAddress(TObject* obj, std::unordered_map<std::string, TYPE> &map);
			template<typename TYPE> void SetAddressSafe(TObject* obj, std::unordered_map<std::string, TYPE> &map);

	};



// * ============================ * //
// * ------- CONSTRUCTORS ------- * //
// * ============================ * //

	/**
	 * @brief Automatically book addresses for all branches of a `TTree`. The branches are accessible by name (use `Get_<type>`, though you have to the `type` to use this).
	 * 
	 * @param fTree The `TTree` that you want to load.
	 * @param print Whether or not to print the `TTree` names and its branches+types. Give `true` to the `SimplifiedTree` constructor for debugging purposes in particular.
	 */
	SimplifiedTree::SimplifiedTree(TTree* fTree, bool print) : fTree(fTree)
	{
		if(!fTree) return;
		TIter next(fTree->GetListOfBranches());
		TObject *obj  = nullptr;
		if(print) std::cout << "  Tree \"" << fTree->GetName() << "\" (" << fTree->GetEntries() << " entries) has branches:" << std::endl;
		while((obj = next())) {
			std::string type(obj->GetTitle()); /// The data type of a branch can be determined from the last character of its title. See https://root.cern.ch/doc/master/classTTree.html#a8a2b55624f48451d7ab0fc3c70bfe8d7 for the labels of each type.
			if(print) std::cout << "    " << type << std::endl;
			switch(type.back()) {
				case 'B' : SetAddress(obj, fMap_B); break;
				case 'b' : SetAddress(obj, fMap_b); break;
				case 'S' : SetAddress(obj, fMap_S); break;
				case 's' : SetAddress(obj, fMap_s); break;
				case 'I' : SetAddress(obj, fMap_I); break;
				case 'i' : SetAddress(obj, fMap_i); break;
				case 'F' : SetAddress(obj, fMap_F); break;
				case 'D' : SetAddress(obj, fMap_D); break;
				case 'L' : SetAddress(obj, fMap_L); break;
				case 'l' : SetAddress(obj, fMap_l); break;
				case 'O' : SetAddress(obj, fMap_O); break;
				default :
					std::cout << "ERROR: Unable to book address for branch \"" << type << "\"" << std::endl;
			}
		}
	}



// * =========================== * //
// * ------- INFORMATION ------- * //
// * =========================== * //

	/**
	 * @brief Draw a distribution of one of the branches in the file.
	 *
	 * @param branchNames Names of the branches that you want to plot. See https://root.cern.ch/doc/master/classTTree.html#a8a2b55624f48451d7ab0fc3c70bfe8d7 for how this works.
	 * @param save Set to `false` if you do not want to save the histogram that has been drawn.
	 * @param opt Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	void SimplifiedTree::DrawBranches(const char* branchNames, const bool save, Option_t* opt, const TString& logScale)
	{
		// * Check input arguments * //
		if(!fTree) return;
		TString outputName(branchNames);
		if(outputName.Contains(">")) outputName.Resize(outputName.First('>'));
		// * Draw histogram and save * //
		fTree->Draw(branchNames, "", opt);
		if(save) CommonFunctions::Draw::SaveCanvas(Form("%s/%s", fTree->GetName(), outputName.Data()), gPad, logScale);
	}

	/**
	 * @brief Draw a distribution of one of the branches in the file.
	 *
	 * @param branchX Branch that you want to plot. You may use a formula.
	 * @param nBinx Number of bins to use on the \f$x\f$-axis.
	 * @param x1 Lower limit on the \f$x\f$-axis.
	 * @param x2 Upper limit on the \f$x\f$-axis.
	 * @param save Set to `false` if you do not want to save the histogram that has been drawn.
	 * @param opt Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	TH1F* SimplifiedTree::DrawBranches(const char* branchX, const Int_t nBinx, const double x1, const double x2, const bool save, Option_t* opt, const TString &logScale)
	{
		// * Draw histogram * //
		const char* histName = Form("hist_%s", branchX);
		DrawBranches(Form("%s>>%s(%i,%f,%f)", branchX, histName, nBinx, x1, x2), false, opt, logScale);
		// * Modify histogram * //
		TH1F *hist = (TH1F*)gDirectory->Get(histName);
		hist->SetTitle(Form("#it{%s} branch of the \"%s\" tree", branchX, fTree->GetName()));
		CommonFunctions::Hist::SetAxisTitles(hist, branchX, Form("count / %g", hist->GetBinWidth(1)));
		// * Save and return histogram * //
		if(save) CommonFunctions::Draw::SaveCanvas(Form("%s/%s", fTree->GetName(), branchX), gPad, logScale);
		return hist;
	}

	/**
	 * @brief Draw a distribution of one of the branches in the file.
	 *
	 * @param branchX Branch that you want to plot on the \f$x\f$-axis. You may use a formula.
	 * @param branchY Branch that you want to plot on the \f$y\f$-axis. You may use a formula.
	 * @param nBinx Number of bins to use on the \f$x\f$-axis.
	 * @param x1 Lower limit on the \f$x\f$-axis.
	 * @param x2 Upper limit on the \f$x\f$-axis.
	 * @param nBiny Number of bins to use on the \f$y\f$-axis.
	 * @param y1 Lower limit on the \f$y\f$-axis.
	 * @param y2 Upper limit on the \f$y\f$-axis.
	 * @param save Set to `false` if you do not want to save the histogram that has been drawn.
	 * @param opt Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	TH2F* SimplifiedTree::DrawBranches(const char* branchX, const char* branchY, const Int_t nBinx, const double x1, const double x2, const Int_t nBiny, const double y1, const double y2, const bool save, Option_t* opt, const TString &logScale)
	{
		// * Draw histogram * //
		const char* histName = Form("hist_%s_%s", branchX, branchY);
		DrawBranches(Form("%s:%s>>%s(%i,%f,%f,%i,%f,%f)", branchX, branchY, histName, nBinx, x1, x2, nBiny, y1, y2), false, opt, logScale);
		// * Modify histogram * //
		TH2F *hist = (TH2F*)gDirectory->Get(histName);
		hist->SetTitle(Form("\"%s\" tree: #it{%s} vs #it{%s}", fTree->GetName(), branchX, branchY));
		CommonFunctions::Hist::SetAxisTitles(
			hist, branchX, branchY,
			Form("count / (%gx%g)",
				hist->GetXaxis()->GetBinWidth(1),
				hist->GetYaxis()->GetBinWidth(1)
			));
		// * Save and return histogram * //
		if(save) CommonFunctions::Draw::SaveCanvas(Form("%s/%s:%s", fTree->GetName(), branchY, branchX), gPad, logScale);
		return hist;
	}


	/**
	 * @brief Draw the distributions of all branches of the underlying `TTree`.
	 * 
	 * @param opt Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	void SimplifiedTree::DrawAndSaveAllBranches(Option_t* opt, const TString& logScale)
	{
		if(!fTree) return;
		TIter next(fTree->GetListOfBranches());
		TObject *obj  = nullptr;
		while((obj = next())) DrawBranches(obj->GetName(), true, opt, logScale);
	}

	/**
	 * @brief Create a histogram object especially for invariant mass analysis.
	 * 
	 * @param branchName Names of the branche that you want to plot.
	 * @param particle Hypothesis particle: which particle are you reconstructing? All analysis parameters, such as estimates for Gaussian widths, are contained within this object.
	 * @param nBins Number of bins to use on the \f$x\f$-axis.
	 * @param opt Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	TH1F* SimplifiedTree::GetInvariantMassHistogram(const char* branchName, const ReconstructedParticle& particle, const int nBins, Option_t *opt, const TString &logScale)
	{
		// * Check input arguments * //
		if(*branchName != 'm') {
			std::cout << "ERROR: branch name \"" << branchName <<  "\" does not start with 'm'" << std::endl;
			return nullptr;
		}
		// * Get histogram and modify
		TH1F* hist = DrawBranches(branchName, nBins, particle.PlotFrom(), particle.PlotUntil(), false, opt, logScale);
		hist->SetTitle(Form("Invariant mass for %s candidate", particle.GetNameLaTeX()));
		CommonFunctions::Hist::SetAxisTitles(hist,
			Form("#it{M}_{%s} (GeV/#it{c}^{2})", particle.GetDaughterLabel()),
			Form("count / %g", hist->GetYaxis()->GetBinWidth(1)));
		return hist;
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //

	/**
	 * @brief Set a memory address for one of the branches of `fTree`.
	 * 
	 * @tparam TYPE The `typename` of the address. It is determined by the `map` you feed it.
	 * @param obj The object from which you want to load the address. This is usually a `TBranch` object.
	 * @param map The type of address you a want to assign (`double`, `int`, etc) is determined by the `map` you feed it. This map should be one of the `fMap_*` data members of this class.
	 */
	template<typename TYPE>
	void SimplifiedTree::SetAddress(TObject* obj, std::unordered_map<std::string, TYPE> &map)
	{
		fTree->SetBranchAddress(obj->GetName(), &map[obj->GetName()]);
	}

	/**
	 * @brief Set a memory address for one of the branches of `fTree`. Only do this if `fTree` exists.
	 * 
	 * @tparam TYPE The `typename` of the address. It is determined by the `map` you feed it.
	 * @param obj The object from which you want to load the address. This is usually a `TBranch` object.
	 * @param map The type of address you a want to assign (`double`, `int`, etc) is determined by the `map` you feed it. This map should be one of the `fMap_*` data members of this class.
	 */
	template<typename TYPE>
	void SimplifiedTree::SetAddressSafe(TObject* obj, std::unordered_map<std::string, TYPE> &map)
	{
		if(fTree) SetAddress(obj, map);
	}



#endif