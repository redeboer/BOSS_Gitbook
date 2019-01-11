// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "FrameworkSettings.h"
	#include <utility>


// * ======================= * //
// * ------- SDT_FIX ------- * //
// * ======================= * //

	template <typename T, typename... Args>
	std::unique_ptr<T> std_fix::make_unique(Args&&... args)
	{
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}


// * ======================== * //
// * ------- DETECTOR ------- * //
// * ======================== * //

	const double Settings::Detector::TofUnitCm = 29.9792458; //!< Time of Flight path unit in cm.
	const double Settings::Detector::TofUnitMm = 299.792458; //!< Time of Flight path unit in mm.


// * =================== * //
// * ------- FIT ------- * //
// * =================== * //

	const double Settings::Fit::gSigmaScaleFactorLow = 0.; //!< This `double` parametr will be used when constructing a `RooRealVar` object for the sigma's of the double Gaussian. The lower boundary of this `RooRealVar` sigma object will be set to this factor times the estimated sigma).
	const double Settings::Fit::gSigmaScaleFactorUp = 10.; //!< This `double` parameter will be used when constructing a `RooRealVar` object for the sigma's of the double Gaussian. The upper boundary of this `RooRealVar` sigma object will be set to this factor times the estimated sigma).


// * =================== * //
// * ------- FIT ------- * //
// * =================== * //

	const char* Settings::Output::Extension = "pdf"; //!< Default extension of output plots (e.g. `png`, `eps`, `jpg`, etc.).
	const TString Settings::Output::PlotOutputDir = Form("/mnt/c/IHEP/besfs/users/deboer/BOSS_Afterburner/plots/%s", Extension); //!< Default output directory for plots.


// * ========================= * //
// * ------- PARTICLES ------- * //
// * ========================= * //

	const Particle Settings::Particles::Em  (11);   //!< Electron (\f$ e^- \f$).
	const Particle Settings::Particles::Mum (13);   //!< Muon (\f$ \mu^- \f$).
	const Particle Settings::Particles::Pip (211);  //!< Positive pion (\f$ \pi^+ \f$).
	const Particle Settings::Particles::Pi0 (111);  //!< Neutral pion (\f$ \pi^0 \f$).
	const Particle Settings::Particles::Rhop(213);  //!< Positive rho meson (\f$ \rho^\pm \f$).
	const Particle Settings::Particles::Rho0(113);  //!< Neutral rho meson (\f$ \rho^0 \f$).
	const Particle Settings::Particles::K   (321);  //!< Positive kaon (\f$ K^\pm \f$).
	const Particle Settings::Particles::K0  (311);  //!< Neutral kaon (\f$ K^\pm \f$).
	const Particle Settings::Particles::P   (2212); //!< Proton (\f$ p \f$).