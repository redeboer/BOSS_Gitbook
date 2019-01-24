// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "ChainLoader.h"
	#include "CommonFunctions.h"
	#include "FrameworkSettings.h"
	#include "TList.h"
	#include <iomanip>
	#include <iostream>



// * ============================ * //
// * ------- CONSTRUCTORS ------- * //
// * ============================ * //


	/**
	 * @brief Automatically book addresses for all branches of the underlying `TChain`. The branches are accessible by name (use method `Get_<type>`, though you have to the `type` to use this).
	 * 
	 * @param print Whether or not to print the `TChain` names and its branches+types. Use `true` for debugging purposes in particular.
	 */
	void ChainLoader::BookAddresses(bool print)
	{
		if(!fChain.GetNbranches()) return;
		/// -# Get list of `TBranches`.
			TIter next(fChain.GetListOfBranches());
			TObject *obj  = nullptr;
			if(print) {
				std::cout << "Tree \"" << fChain.GetName() << "\" (" << fChain.GetEntries() << " entries)" << std::endl;
				std::cout << fChain.GetTitle() << std::endl;
				std::cout << "   "
					<< std::setw(18) << std::left  << "BRANCH NAME"
					<< std::setw(12) << std::right << "MEAN" << std::endl;
			}
		/// -# Loop over list of `TBranches` and determine `typename` of the branch. The data type of a branch can be determined from the last character of its title. See <a href="https://root.cern.ch/doc/master/classTTree.html#a8a2b55624f48451d7ab0fc3c70bfe8d7">`TTree`</a> for the labels of each type.
			while((obj = next())) {
				std::string type(obj->GetTitle());
				if(print) std::cout << "   "
					<< std::setw(18) << std::left  << type
					<< std::setw(12) << std::right << ComputeMean(&fChain, obj->GetName()) << std::endl;
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
			if(print) std::cout << std::endl;
	}



// * =========================== * //
// * ------- INFORMATION ------- * //
// * =========================== * //

	/**
	 * @brief Draw a distribution of one of the branches in the file.
	 *
	 * @param branchX Branch that you want to plot. You may use a formula.
	 * @param nBinx Number of bins to use on the \f$x\f$-axis.
	 * @param x1 Lower limit on the \f$x\f$-axis.
	 * @param x2 Upper limit on the \f$x\f$-axis.
	 * @param save Set to `false` if you do not want to save the histogram that has been drawn.
	 * @param option Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	TH1F* ChainLoader::Draw(const char* branchX, const Int_t nBinx, const double x1, const double x2, Option_t *option, const bool save, const TString &logScale, const char* cut)
	{
		// * Draw histogram * //
		const char* histName = Form("hist_%s", branchX);
		Draw(Form("%s>>%s(%i,%f,%f)", branchX, histName, nBinx, x1, x2), cut, option, save, logScale);
		// * Modify histogram * //
		TH1F *hist = (TH1F*)gDirectory->Get(histName);
		hist->SetTitle(Form("#it{%s} branch of the \"%s\" tree", branchX, fChain.GetName()));
		if(!logScale.Contains("y") && !logScale.Contains("Y")) {// NOTE: cannot set y log scale if min value is 0
			auto maxbin = hist->GetMaximumBin();
			hist->GetYaxis()->SetRangeUser(0., hist->GetBinContent(maxbin)+1.5*hist->GetBinErrorUp(maxbin));
		}
		CommonFunctions::Hist::SetAxisTitles(hist, branchX, Form("count / %g", hist->GetBinWidth(1)));
		// * Save and return histogram * //
		if(save) CommonFunctions::Draw::SaveCanvas(Form("%s/%s", fChain.GetName(), branchX), gPad, logScale);
		return hist;
	}

	/**
	 * @brief Create a histogram object especially for invariant mass analysis.
	 * 
	 * @param branchName Names of the branche that you want to plot.
	 * @param particle Hypothesis particle: which particle are you reconstructing? All analysis parameters, such as estimates for Gaussian widths, are contained within this object.
	 * @param nBins Number of bins to use on the \f$x\f$-axis.
	 * @param option Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	TH1F* ChainLoader::GetInvariantMassHistogram(const char* branchName, const ReconstructedParticle& particle, const int nBins, Option_t *option, const TString &logScale)
	{
		// * Check input arguments * //
		if(*branchName != 'm') {
			std::cout << "ERROR: branch name \"" << branchName <<  "\" does not start with 'm'" << std::endl;
			return nullptr;
		}
		// * Get histogram and modify
		TH1F* hist = Draw(branchName, nBins, particle.PlotFrom(), particle.PlotUntil(), option, false, logScale);
		hist->SetTitle(Form("Invariant mass for %s candidate", particle.GetNameLaTeX()));
		CommonFunctions::Hist::SetAxisTitles(hist,
			Form("#it{M}_{%s} (GeV/#it{c}^{2})", particle.GetDaughterLabel()),
			Form("count / %g", hist->GetYaxis()->GetBinWidth(1)));
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
	 * @param option Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	TH2F* ChainLoader::Draw(const char* branchX, const char* branchY, const Int_t nBinx, const double x1, const double x2, const Int_t nBiny, const double y1, const double y2, Option_t *option, const bool save, const TString &logScale, const char* cut)
	{
		// * Draw histogram * //
		const char* histName = Form("hist_%s_%s", branchX, branchY);
		Draw(Form("%s:%s>>%s(%i,%f,%f,%i,%f,%f)", branchX, branchY, histName, nBinx, x1, x2, nBiny, y1, y2), cut, option, save, logScale);
		// * Modify histogram * //
		TH2F *hist = (TH2F*)gDirectory->Get(histName);
		hist->SetTitle(Form("\"%s\" tree: #it{%s} vs #it{%s}", fChain.GetName(), branchX, branchY));
		CommonFunctions::Hist::SetAxisTitles(
			hist, branchX, branchY,
			Form("count / (%gx%g)",
				hist->GetXaxis()->GetBinWidth(1),
				hist->GetYaxis()->GetBinWidth(1)
			));
		// * Save and return histogram * //
		if(save) CommonFunctions::Draw::SaveCanvas(Form("%s/%s:%s", fChain.GetName(), branchY, branchX), gPad, logScale);
		return hist;
	}


	/**
	 * @brief Compute mean value of a branch in the `TChain`.
	 * 
	 * @param chain `TChain` that you want to check.
	 * @param branchName Name of the branch that you want to check.
	 */
	double ChainLoader::ComputeMean(TChain* chain, const char* branchName)
	{
		if(!chain) return -1e6;
		chain->Draw(branchName);
		TH1F *htemp = (TH1F*)gPad->GetPrimitive("htemp");
		if(!htemp) return -1e6;
		double mean = htemp->GetMean();
		delete htemp;
		return mean;
	}


	/**
	 * @brief Draw the distributions of all branches of the underlying `TChain`.
	 * 
	 * @param option Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	void ChainLoader::DrawAndSaveAllBranches(Option_t *option, const TString& logScale)
	{
		if(!fChain.GetNtrees()) return;
		TIter next(fChain.GetListOfBranches());
		TObject *obj  = nullptr;
		while((obj = next())) Draw(obj->GetName(), "", option, true, logScale);
	}


	/**
	 * @brief Draw the distributions of all branches of the underlying `TChain` if its name starts with `"mult"`.
	 * @details This function additionally ensures that the bin width is set to 1 and that the histograms are drawn in `"E1"` mode (see https://root.cern.ch/doc/master/classTHistPainter.html).
	 * 
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 * @param option Draw options.
	 */
	void ChainLoader::DrawAndSaveAllMultiplicityBranches(const TString& logScale, Option_t *option)
	{
		if(!fChain.GetNtrees()) return;
		TString name(fChain.GetName());
		if(!name.BeginsWith("mult")) return;
		TIter next(fChain.GetListOfBranches());
		TObject *obj  = nullptr;
		while((obj = next())) {
			const char* name = obj->GetName();
			int max = fChain.GetMaximum(name);
			if(!max) continue;
			Draw(name, max, 0, max, option, true, logScale);
		}
	}

	/**
	 * @brief Draw a distribution of one of the branches in the file.
	 *
	 * @param branchNames Names of the branches that you want to plot. See https://root.cern.ch/doc/master/classTTree.html#a8a2b55624f48451d7ab0fc3c70bfe8d7 for how this works.
	 * @param save Set to `false` if you do not want to save the histogram that has been drawn.
	 * @param option Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 */
	void ChainLoader::Draw(const char* branchNames, const char* cut, Option_t *option, const bool save, const TString& logScale)
	{
		/// <ol>
		/// <li> Check if `TChain` contains entries.
		if(!fChain.GetEntries()) return;
		/// <li> Remove possible output options from `branchNames` parameter.
		TString outputName(branchNames);
		if(outputName.Contains(">")) outputName.Resize(outputName.First('>'));
		/// <li> Draw histogram and save
		fChain.Draw(branchNames, cut, option);
		if(save) CommonFunctions::Draw::SaveCanvas(Form("%s/%s", fChain.GetName(), outputName.Data()), gPad, logScale);
		/// </ol>
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //

	/**
	 * @brief Set a memory address for one of the branches of `fChain`.
	 * 
	 * @tparam TYPE The `typename` of the address. It is determined by the `map` you feed it.
	 * @param obj The object from which you want to load the address. This is usually a `TBranch` object.
	 * @param map The type of address you a want to assign (`double`, `int`, etc) is determined by the `map` you feed it. This map should be one of the `fMap_*` data members of this class.
	 */
	template<typename TYPE>
	void ChainLoader::SetAddress(TObject* obj, std::unordered_map<std::string, TYPE> &map)
	{
		fChain.SetBranchAddress(obj->GetName(), &map[obj->GetName()]);
	}

	/**
	 * @brief Set a memory address for one of the branches of `fChain`. Only do this if `fChain` exists.
	 * 
	 * @tparam TYPE The `typename` of the address. It is determined by the `map` you feed it.
	 * @param obj The object from which you want to load the address. This is usually a `TBranch` object.
	 * @param map The type of address you a want to assign (`double`, `int`, etc) is determined by the `map` you feed it. This map should be one of the `fMap_*` data members of this class.
	 */
	template<typename TYPE>
	void ChainLoader::SetAddressSafe(TObject* obj, std::unordered_map<std::string, TYPE> &map)
	{
		if(fChain.GetNbranches()) SetAddress(obj, map);
	}