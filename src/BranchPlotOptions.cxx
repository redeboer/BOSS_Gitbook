// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "BranchPlotOptions.h"



// * =========================== * //
// * ------- CONSTRUCTORS ------ * //
// * =========================== * //

	/**
	 * @brief Constructor that opens a `TFile` and unordered_maps its contents (using <b>number of bins</b>).
	 */
	BranchPlotOptions::BranchPlotOptions(const char* branchName, const int nBins, const double from, const double to, const char* option, const char* cut) :
		fBranchName(branchName)
	{
		/// <ol>
		/// <li> Abort if arguments are weird.
			if(nBins <= 0) return;
			if(to <= from) return;

		/// <li> Import arguments from constructor parameters.
			fCuts         = cut;
			fDrawOption   = option;
			fNumberOfBins = nBins;
			fPlotFrom     = from;
			fPlotTo       = to;
			fBinWidth     = (fPlotTo - fPlotFrom) / fNumberOfBins;

		/// </ol>
	}

	/**
	 * @brief Constructor that opens a `TFile` and unordered_maps its contents (using bin <b>width</b>).
	 */
	BranchPlotOptions::BranchPlotOptions(const char* branchName, const double binWidth, const double from, const double to, const char* option, const char* cut) :
		fBranchName(branchName)
	{
		/// <ol>
		/// <li> Abort if arguments are weird.
			if(binWidth <= 0) return;
			if(to <= from) return;

		/// <li> <b>WARNING</b> Abort if number of bins is larger than 10.000!
			fNumberOfBins = (fPlotTo - fPlotFrom) / fBinWidth;
			if(fNumberOfBins >= 10000) return;

		/// <li> Import arguments from constructor parameters.
			fCuts         = cut;
			fDrawOption   = option;
			fPlotFrom     = from;
			fPlotTo       = to;

		/// </ol>
	}