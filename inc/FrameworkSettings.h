#ifndef Physics_Analysis_Settings_H
#define Physics_Analysis_Settings_H

/// @addtogroup BOSS_Afterburner
/// @{

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

#include "Particle.h"
#include "TString.h"
#include <iostream>

/**
 * @brief    Helper namespace in case of `C++11` compilation. The `std::make_unique` method of `std::unique_ptr` has not been defined by default and needs to be defined seperately, hence the '`_fix`' of `std`.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     October 25th, 2018
 */

// * In case of C++ 2011 compilation *
namespace std_fix {
	template <typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args)
	{
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
} // namespace std_fix

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
		const double TofUnitCm = 29.9792458; //!< Time of Flight path unit in cm.
		const double TofUnitMm = 299.792458; //!< Time of Flight path unit in mm.
	}
	namespace Fit
	{
		const double gSigmaScaleFactorLow = 0.; //!< This `double` parametr will be used when constructing a `RooRealVar` object for the sigma's of the double Gaussian. The lower boundary of this `RooRealVar` sigma object will be set to this factor times the estimated sigma).
		const double gSigmaScaleFactorUp = 10.; //!< This `double` parameter will be used when constructing a `RooRealVar` object for the sigma's of the double Gaussian. The upper boundary of this `RooRealVar` sigma object will be set to this factor times the estimated sigma).
	}
	namespace Output
	{
		const char* Extension = "pdf"; //!< Default extension of output plots (e.g. `png`, `eps`, `jpg`, etc.).
		const TString PlotOutputDir = Form("/mnt/c/IHEP/besfs/users/deboer/BOSS_Afterburner/plots/%s", Extension); //!< Default output directory for plots.
	}
	/**
	 * @brief List of relevant particles used in this analysis.
	 * @details Particles are generated using the `ROOT` PDG database. For a list of PDG codes, see http://home.fnal.gov/~mrenna/lutp0613man2/node44.html
	 */
	namespace Particles
	{
		const Particle Em  (11);   //!< Electron (\f$ e^- \f$).
		const Particle Mum (13);   //!< Muon (\f$ \mu^- \f$).
		const Particle Pip (211);  //!< Positive pion (\f$ \pi^+ \f$).
		const Particle Pi0 (111);  //!< Neutral pion (\f$ \pi^0 \f$).
		const Particle Rhop(213);  //!< Positive rho meson (\f$ \rho^\pm \f$).
		const Particle Rho0(113);  //!< Neutral rho meson (\f$ \rho^0 \f$).
		const Particle K   (321);  //!< Positive kaon (\f$ K^\pm \f$).
		const Particle K0  (311);  //!< Neutral kaon (\f$ K^\pm \f$).
		const Particle P   (2212); //!< Proton (\f$ p \f$).
	}
}

/// @}
// end of Doxygen group BOSS_Afterburner

#endif