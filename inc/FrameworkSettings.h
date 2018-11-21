#ifndef Physics_Analysis_Settings_H
#define Physics_Analysis_Settings_H

/**
 * @brief    Header with a namespace `Settings` that contains <i>all</i> settings for the analysis framework.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     October 25th, 2018
 * @remark   @b DEVELOPMENTAL
 *
 * @details  The `Settings` namespace has been split up into several sub-namespaces.
 */

#include "Particle.h"
#include "TString.h"

namespace Settings
{
	namespace Output
	{
		const char* Extension = "pdf"; //!< Default extension of output plots (e.g. `png`, `eps`, `jpg`, etc.).
		const TString PlotOutputDir = Form("../plots/%s", Extension); //!< Default output directory for plots.
	}
	namespace Math
	{
		const double TwoPi = 6.2831853; //!< Mathematical constant \f$ 2 pi\f$.
		const double Pi    = 3.1415927; //!< Mathematical constant \f$ pi\f$.
	}
	namespace Detector
	{
		const double TofUnitCm = 29.9792458; //!< Time of Flight path unit in cm.
		const double TofUnitMm = 299.792458; //!< Time of Flight path unit in mm.
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

#endif