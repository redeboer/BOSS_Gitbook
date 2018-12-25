#ifndef Physics_Analysis_rhopi_pipigg_H
#define Physics_Analysis_rhopi_pipigg_H

/**
 * @brief    Modified header of the stock BOSS package `rhopi_pipigg` (version `00-00-23`).
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 24th, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TrackSelector/TrackSelector.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class rhopi_pipigg : public TrackSelector
{
public:
	// * Constructor and destructors *
	rhopi_pipigg(const std::string &name, ISvcLocator* pSvcLocator);

	// * Algorithm steps *
	StatusCode initialize_rest();
	StatusCode execute_rest();
	StatusCode finalize_rest();

private:
	// * Private methods * //
	void SetSmallestAngles(std::vector<EvtRecTrack*>::iterator &iter, std::vector<EvtRecTrack*> &vec, Hep3Vector &emcpos);

	// * Private data members * //
		double fMpi;  //!< Mass of \f$\pi^\pm\f$.
		double fMrho; //!< Mass of \f$\rho\f$.
		double fSmallestAngle; //!< Current angle (in a loop) between the nearest neutral track and a charged track.
		double fSmallestTheta; //!< Current \f$\theta\f$ angle (in a loop) between the nearest neutral track and a charged track.
		double fSmallestPhi;   //!< Current \f$\phi\f$ angle (in a loop) between the nearest neutral track and a charged track.
		std::vector<EvtRecTrack*> fGamma; //!< Vector that contains a selection of pointers to neutral tracks identified as \f$\gamma\f$.
		std::vector<EvtRecTrack*> fPionNeg; //!< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^-\f$.
		std::vector<EvtRecTrack*> fPionPos; //!< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^+\f$.
		std::vector<EvtRecTrack*>::iterator gGamma1Iter; //!< Iterator for looping over the collection of gamma's (1st occurence).
		std::vector<EvtRecTrack*>::iterator gGamma2Iter; //!< Iterator for looping over the collection of gamma's (2st occurence).
		std::vector<EvtRecTrack*>::iterator fPionNegIter; //!< Iterator for looping over the collection of negative pions.
		std::vector<EvtRecTrack*>::iterator fPionPosIter; //!< Iterator for looping over the collection of positive pions.


	// ! Cut parameters ! //
		/// Here, you can define data members that you use to define cuts. The values for these cuts should be set in the `TrackSelector::TrackSelector` constructor (see `.cxx` file).

		double fDeltaMrho0;    //!< Cut on width of invariant mass window around \f$m_{\rho^0}\f$.
		double fMaxChiSq;      //!< Maximum \f$\chi_\mathrm{red}^2\f$ of the kinematic Kalman fits.
		double fMaxGammaPhi;   //!< Maximum \f$\phi\f$ angle between the photon and the nearest charged track <i>in radians</i>.
		double fMaxGammaTheta; //!< Maximum \f$\theta\f$ angle between the photon and the nearest charged track <i>in radians</i>.
		double fMaxGammaAngle; //!< Maximum angle between the photon and the nearest charged track <i>in degrees</i>.


	// ! NTuple data members ! //
		/// `NTuple`s are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each `NTuple::Item` represents its leaves. The name of the leaves is determined when calling `NTuple::addItem`.
		/// Note that the `NTuple::Items` have to be added to the NTuple during the `TrackSelector::initialize()` step, otherwise they cannot be used as values! This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Maps of Ntuples *
			bool fDo_photon; //!< Package property that determines whether or not to write angle info from the photons.
			bool fDo_fit4c;  //!< Package property that determines whether or not to perform and record a 4-constraint Kalman kinematic fit.
			bool fDo_fit5c;  //!< Package property that determines whether or not to perform and record a 5-constraint Kalman kinematic fit.
			std::map<std::string, NTuple::Item<double> > fGammaInfo; //!< Container for the `"gamma"` branch.
			std::map<std::string, NTuple::Item<double> > fDedx_pi;   //!< Container for the `"dedx_pi"` branch.
			std::map<std::string, NTuple::Item<double> > fFit4c;     //!< Container for the `"fit4c"` branch.
			std::map<std::string, NTuple::Item<double> > fFit5c;     //!< Container for the `"fit5c"` branch.

};

#endif