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
			template<class... ARGS> extern
			void DrawAndSaveRecursion(Option_t* opt, ARGS&&... args);
				//!< Start recursion for `DrawAndSaveRecursion`.
			template<class TYPE, class... ARGS> extern
			void DrawAndSaveRecursion(Option_t* opt, TYPE first, ARGS... args);
			template<> inline
			void DrawAndSaveRecursion(Option_t* opt) {};
				//!< End recursion for `DrawAndSaveRecursion`. @todo Impossible to compile in `make` framework?

			template<class ...ARGS> extern
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
			template<typename FUNCTOR, typename ...Rest> extern
			void LoopTree(TTree* tree, FUNCTOR&& lambda, Rest&&... args);
		}
		/**
		 * @brief Namespace containing functions related to terminal output.
		 */
		namespace Print
		{
			template<typename TYPE> extern
			std::string CommaFormattedString(TYPE number);
		}

	}



/// @}
#endif