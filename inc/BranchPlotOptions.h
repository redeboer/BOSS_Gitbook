#ifndef BOSS_Afterburner_BranchPlotOptions_H
#define BOSS_Afterburner_BranchPlotOptions_H

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "CommonFunctions.h"
	#include <string>



// * ================================= * //
// * ------- CLASS DEFINITIONS ------- * //
// * ================================= * //
/// @addtogroup BOSS_Afterburner_objects
/// @{


	/**
	 * @brief    Container object for a <i>BOSS Afterburner</i> analysis config file.
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     January 24th, 2018
	 *
	 * @details  Give this object a path to the configuration text file for the analysis you want to perform, and all settings will be automatically loaded from this file. The syntax for this file is of course determined by this object.
	 */
	class BranchPlotOptions
	{
	public:
		/// @name Constructors
			///@{
			BranchPlotOptions(const char* branchName, const int nBins=100, const double from=-DBL_MAX, const double to=DBL_MAX, const char* option="", const char* cut="");
			BranchPlotOptions(const char* branchName, const double binWidth, const double from=-DBL_MAX, const double to=DBL_MAX, const char* option="", const char* cut="");
			///@}

	private:
		std::string fBranchName;
		std::string fCuts;
		std::string fDrawOption;

		/// @name Binnings
			///@{
			int fNumberOfBins;
			double fBinWidth;
			double fPlotFrom;
			double fPlotTo;
			///@}

	};


/// @}
#endif