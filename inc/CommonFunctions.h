#ifndef BOSS_Afterburner_CommonFunctions_H
#define BOSS_Afterburner_CommonFunctions_H

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "FrameworkSettings.h"
	#include "Particle.h"
	#include "ReconstructedParticle.h"
	#include "RooDataHist.h"
	#include "RooRealVar.h"
	#include "TCanvas.h"
	#include "TH1.h"
	#include "TH1D.h"
	#include "TH1F.h"
	#include "TH2D.h"
	#include "TString.h"
	#include "TTree.h"
	#include "TVirtualPad.h"
	#include <string>



// * ============================ * //
// * ------- DECLARATIONS ------- * //
// * ============================ * //
/// @addtogroup BOSS_Afterburner_functions
/// @{


	/**
	 * @brief   A namespace that is used to bundle functions that can be used by different analysis scripts.
	 * @details The idea of sharing these functions is to standardise output. For instance, if a pdf of a histogram is saved through a function defined here, the pdf output can be centrally 'beautified' in this header file.
	 * 
	 * @author  Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date    November 20th, 2018
	 */
	namespace CommonFunctions
	{
		/**
		 * @brief Namespace containing functions related to drawing and saving plots.
		 */
		namespace Draw
		{
			template<class... ARGS>
			void DrawAndSaveRecursion(Option_t* opt, ARGS&&... args);
				//!< Start recursion for `DrawAndSaveRecursion`.
			template<class TYPE, class... ARGS>
			void DrawAndSaveRecursion(Option_t* opt, TYPE first, ARGS... args);
			template<> inline
			void DrawAndSaveRecursion(Option_t* opt) {};
				//!< End recursion for `DrawAndSaveRecursion`. @todo Impossible to compile in `make` framework?

			template<class ...ARGS>
			void DrawAndSave(const char* saveas, Option_t* opt, const char* logScale, ARGS... args);
			void DrawAndSave(TH1 &hist, const char* saveas, Option_t* opt, TString logScale="");
			void DrawAndSave(TH1D &hist, const char* saveas, TString logScale="");
			void DrawAndSave(TH2D &hist, const char* saveas, TString logScale="");
			void DrawAndSave(TTree* tree, const char* varexp, const char* selection, Option_t* opt, TString logScale="");
			void SaveCanvas(const char *saveas, TVirtualPad *pad=gPad, TString logScale="");
			void SetLogScale(TString logScale="", TVirtualPad *pad=gPad);
		}
		/**
		 * @brief Namespace containing functions related to debugging and making macro's handle errors.
		 */
		namespace Error
		{
			bool IsEmptyPtr(void* ptr);
		}
		/**
		 * @brief Namespace containing functions related to file naming and file I/O.
		 */
		namespace File
		{
			const char* GetOutputFilename(const char* filenameWithoutExt);
			const char* SetOutputFilename(const char* filenameWithoutExt);
		}
		/**
		 * @brief Namespace containing functions related to performing fits on histograms (in particular related to `RooFit`).
		 */
		namespace Fit
		{
			RooDataHist CreateRooFitInvMassDistr(TH1F *hist, const RooRealVar &var, const ReconstructedParticle& particle);
			RooRealVar CreateRooFitInvMassVar(const ReconstructedParticle& particle);
			void FitBWGaussianConvolution(TH1F *hist, const ReconstructedParticle& particle, const UChar_t numPolynomials = 0, TString logScale="");
			void FitBWDoubleGaussianConvolution(TH1F *hist, const ReconstructedParticle& particle, const UChar_t numPolynomials = 0, TString logScale="");
			void FitBreitWigner(TH1F *hist, const ReconstructedParticle& particle, const UChar_t numPolynomials = 0);
			void FitDoubleGaussian(TH1F *hist, const ReconstructedParticle& particle, const UChar_t numPolynomials = 0, TString logScale="");
		}
		/**
		 * @brief Namespace containing functions related to generating and modifying histograms.
		 */
		namespace Hist
		{
			TH1D CreateInvariantMassHistogram(const ReconstructedParticle& particle, const int nBins = 200);
			void SetAxisTitles(TH1* hist, const char* xAxis, const char* yAxis=nullptr, const char* zAxis=nullptr, bool update=true);
		}
		/**
		 * @brief Namespace containing functions related to looping over objects, for instance over the entries in a `TTree`. Functions here either use a `Lambda` as argument or are defined recursively. This is why support for `C++11` is required for the <i>BOSS Afterburner</i>.
		 */
		namespace Loop
		{
			template<typename FUNCTOR, typename ...Rest>
			void LoopTree(TTree* tree, FUNCTOR&& lambda, Rest&&... args);
		}
		/**
		 * @brief Namespace containing functions related to terminal output.
		 */
		namespace Print
		{
			template<typename TYPE>
			std::string CommaFormattedString(TYPE number);
		}

	}



/// @}
// * ================================== * //
// * ------- SUB-NAMESPACE DRAW ------- * //
// * ================================== * //


	/**
	 * @brief The `DrawAndSaveRecursion` functions are necessary for `DrawAndSave`, which is a <i>variadic</i> template function.
	 * 
	 * @tparam TYPE The type of objects that you want to draw. @todo Should ideally be `TObject`s.
	 * @tparam ARGS The type of the rest of the objects that you want to draw. The type is actually inferred from `TYPE`.
	 * @param opt Draw options.
	 * @param first The first object that you want to plot.
	 * @param args The objects that you want to plot.
	 */
	template<class TYPE, class... ARGS> inline
	void CommonFunctions::Draw::DrawAndSaveRecursion(Option_t* opt, TYPE first, ARGS... args)

	{
		/// Some useful information for defining template functions in the `.cxx` file:
		/// - https://isocpp.org/wiki/faq/templates#template-specialization-speed
		/// - https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl
		/// - https://isocpp.org/wiki/faq/templates#separate-template-fn-defn-from-decl-export-keyword
		auto obj = dynamic_cast<TObject*>(first);
		if(obj) obj->Draw(opt);
		DrawAndSaveRecursion(opt, args...); // continue recursion
	}


	/**
	 * @brief Function that allows you to draw and save any set of `TObject`s.
	 * 
	 * @tparam ARGS The type of objects that you want to draw. @todo Should ideally be `TObject`s.
	 * @param saveas Filename that the output file name should have. See `CommonFunctions::CommonFunctions::File::SetOutputFilename` for more information.
	 * @param opt Draw options.
	 * @param logScale If this argument contains an `'x'`, the \f$x\f$-scale will be set to log scale (same for `'y'` and `'z'`).
	 * @param args The objects that you want to plot.
	 */
	template<class ...ARGS> inline
	void CommonFunctions::Draw::DrawAndSave(const char* saveas, Option_t* opt, const char* logScale, ARGS... args)
	{
		// * Create canvas in batch mode * //
		TCanvas c;
		c.SetBatch();
		// * Draw objects * //
		DrawAndSaveRecursion(opt, args...);
		// * Save canvas * //
		SaveCanvas(saveas, &c, logScale);
	}



// * ================================== * //
// * ------- SUB-NAMESPACE LOOP ------- * //
// * ================================== * //

	/**
	 * @brief Generalisation of the procedure when looping over a `TTree`.
	 */
	template<typename FUNCTOR, typename ...Rest> inline
	void CommonFunctions::Loop::LoopTree(TTree* tree, FUNCTOR&& lambda, Rest&&... args)
	{
		if(CommonFunctions::Error::IsEmptyPtr(tree)) return;
		auto nEntries = tree->GetEntries();
		std::cout << "Looping over " << nEntries << " events in the \"" << tree->GetName() << "\" tree" << std::flush;
		for(auto i = 0; i < nEntries; ++i) {
			tree->GetEntry(i);
			lambda(args...);
		}
		std::cout << "\rSuccesfully looped over " << nEntries << " events in the \"" << tree->GetName() << "\" tree" << std::endl;
	}



// * =================================== * //
// * ------- SUB-NAMESPACE PRINT ------- * //
// * =================================== * //


	/**
	 * @brief Create a std::string from a number that has a number between each 
	 * @param number Number that you want to format.
	 * @return Formatted std::string.
	 */
	template<typename TYPE> inline
	std::string CommonFunctions::Print::CommaFormattedString(TYPE number)
	{
		std::string output = std::to_string(number);
		int insertPosition = output.length()-3;
		while (insertPosition > 0) {
			output.insert(insertPosition, ",");
			insertPosition -= 3;
		}
		return output;
	}



#endif