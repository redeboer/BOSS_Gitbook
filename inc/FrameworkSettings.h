#ifndef BOSS_Afterburner_Settings_H
#define BOSS_Afterburner_Settings_H

// * ========================================= * //
// * ------- DOXYGEN GROUP DEFINITIONS ------- * //
// * ========================================= * //

	/**
	 * @defgroup BOSS_Afterburner BOSS Afterburner
	 * @brief <b>Classes, namespaces, and functions for the <i>BOSS Afterburner</i></b>. The core of these classes is the `BOSSOutputLoader` class. This class allows you to automatically load a set of ROOT files that have been procuded during the event selection in BOSS. Supporting functions, parameters and other classes are to be found in for instance the `CommonFunctions.h` header file.
	 */

	/**
	 * @defgroup BOSS_Afterburner_objects Objects
	 * @ingroup BOSS_Afterburner
	 * @brief Base and derived __classes__ that represent the structure of BOSS output files.
	 */

	/**
	 * @defgroup BOSS_Afterburner_functions Functions
	 * @ingroup BOSS_Afterburner
	 * @brief Header files containing helper __functions__ that can be used in the eventual scripts.
	 */

	/**
	 * @defgroup BOSS_Afterburner_settings Settings
	 * @ingroup BOSS_Afterburner
	 * @brief Global __parameters__ that represent default settings of the <i>BOSS Afterburner</i>.
	 */

	/**
	 * @defgroup BOSS_Afterburner_scripts Scripts
	 * @ingroup BOSS_Afterburner
	 * @brief Analysis __scripts__ of the <i>BOSS Afterburner</i>. These are contain the eventual analysis code of the BOSS output files and make use of the objects defined under `inc`.
	 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "Particle.h"
	#include "TString.h"

	// * For backward compatibility with ROOT5 * //
	#ifdef __CINT__
		#include <TLatex.h>
		#define GET_PTR(VARIABLE) VARIABLE //!< Definition of pointer in case of ROOT5.
		#define SHARED_PTR(TYPE) TYPE*     //!< Definition of shared pointer in case of ROOT5.
		#define UNIQUE_PTR(TYPE) TYPE*     //!< Definition of unique pointer in case of ROOT5.
	#else
		#define GET_PTR(VARIABLE) (VARIABLE).get()     //!< Definition of pointer in case of ROOT6 or higher.
		#define SHARED_PTR(TYPE) std::shared_ptr<TYPE> //!< Definition of shared pointer in case of ROOT6 or higher.
		#define UNIQUE_PTR(TYPE) std::unique_ptr<TYPE> //!< Definition of unique pointer in case of ROOT6 or higher.
	#endif



// * ============================ * //
// * ------- DECLARATIONS ------- * //
// * ============================ * //
/// @addtogroup BOSS_Afterburner_settings
/// @{


	/**
	 * @brief    Helper namespace in case of `C++11` compilation. The `std::make_unique` method of `std::unique_ptr` has not been defined by default and needs to be defined seperately, hence the '`_fix`' of `std`.
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     October 25th, 2018
	 * @todo     This version of the header contains the definitions as well (as `inline`) to avoid multiple definitions. But it should be possible to use the old version (`.old` and `.cpp`), which use `extern` and `inline`, without running into multiple definition problems.
	 */
	namespace std_fix {
		template <typename T, typename... Args> inline
		std::unique_ptr<T> make_unique(Args&&... args)
		{
			return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
		}
	}


	/**
	 * @brief    Header with a namespace `Settings` that contains <i>all</i> settings for the analysis framework.
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     October 25th, 2018
	 *
	 * @details  The `Settings` namespace has been split up into several sub-namespaces.
	 */
	namespace Settings
	{
		namespace Detector
		{
			inline constexpr double TofUnitCm = 29.9792458;
			inline constexpr double TofUnitMm = 299.792458;
		}
		namespace File
		{
			inline constexpr const char* gConfigs = "configs";
			inline constexpr const char* gExtension = "pdf";
			inline constexpr const char* gPlotOutputDir = "plots";
		}
		namespace Fit
		{
			inline constexpr double gSigmaScaleFactorLow = 0.;
			inline constexpr double gSigmaScaleFactorUp = 10.;
		}
		/**
		 * @brief List of relevant particles used in this analysis.
		 * @details Particles are generated using the `ROOT` PDG database. For a list of PDG codes, see http://home.fnal.gov/~mrenna/lutp0613man2/node44.html
		 */
		namespace Particles
		{
			inline const Particle Em(11);   //!< Electron (\f$ e^- \f$). 
			inline const Particle Mum(13);   //!< Muon (\f$ \mu^- \f$).
			inline const Particle Pip(211);  //!< Positive pion (\f$ \pi^+ \f$).
			inline const Particle Pi0(111);  //!< Neutral pion (\f$ \pi^0 \f$).
			inline const Particle Rhop(213);  //!< Positive rho meson (\f$ \rho^\pm \f$).
			inline const Particle Rho0(113);  //!< Neutral rho meson (\f$ \rho^0 \f$).
			inline const Particle K(321);  //!< Positive kaon (\f$ K^\pm \f$).  
			inline const Particle K0(311);  //!< Neutral kaon (\f$ K^\pm \f$). 
			inline const Particle P(2212); //!< Proton (\f$ p \f$).  
		}
	}


/// @}
#endif