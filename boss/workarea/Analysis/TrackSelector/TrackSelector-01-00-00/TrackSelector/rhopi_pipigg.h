#ifndef Physics_Analysis_rhopi_pipigg_H
#define Physics_Analysis_rhopi_pipigg_H

/// @addtogroup BOSS
/// @{

/**
 * @brief    Analyse \f$ J/\psi \rightarrow \rho^{0,\pm}\pi^{0,\mp} \rightarrow \pi^-\pi^+\\gamma\gamma \f$ events.
 * @details  Based on the original `RhopiAlg` package, but now making use of the `TrackSelector` base algorithm.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 27th, 2018
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

protected:
	// * Maps, vectors, and iterators * //
		std::vector<EvtRecTrack*> fGamma;   //!< Vector that contains a selection of pointers to neutral tracks identified as \f$\gamma\f$.
		std::vector<EvtRecTrack*> fPionNeg; //!< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^-\f$.
		std::vector<EvtRecTrack*> fPionPos; //!< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^+\f$.
		std::vector<EvtRecTrack*>::iterator fGamma1Iter;  //!< Iterator for looping over the collection of gamma's (1st occurence).
		std::vector<EvtRecTrack*>::iterator fGamma2Iter;  //!< Iterator for looping over the collection of gamma's (2st occurence).
		std::vector<EvtRecTrack*>::iterator fPionNegIter; //!< Iterator for looping over the collection of negative pions.
		std::vector<EvtRecTrack*>::iterator fPionPosIter; //!< Iterator for looping over the collection of positive pions.

	// * Maps of Ntuples *
		/// `NTuple`s are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each `NTuple::Item` represents its leaves. The name of the leaves is determined when calling `NTuple::addItem`.
		/// Note that the `NTuple::Items` have to be added to the NTuple during the `TrackSelector::initialize()` step, otherwise they cannot be used as values! This is also the place where you name these variables, so make sure that the structure here is reflected there!
		bool fWrite_mult_select; //!< Package property that determines whether or not to write the multiplicities <i>of the selected particles</i>.
		bool fWrite_photon; //!< Package property that determines whether or not to write angle info from the photons.
		bool fWrite_fit4c_all;  //!< Package property that determines whether or not to write results of the `4C` fit <i>for all combinations</i>.
		bool fWrite_fit4c_best; //!< Package property that determines whether or not to write results of the `4C` fit for the combination closest to the expected masses.
		bool fWrite_fit5c_all;  //!< Package property that determines whether or not to write results of the `5C` fit <i>for all combinations</i>.
		bool fWrite_fit5c_best; //!< Package property that determines whether or not to write results of the `5C` fit for the combination closest to the expected masses.
		std::map<std::string, NTuple::Item<double> > fMap_photon;     //!< Container for the `"gamma"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_dedx_pi;    //!< Container for the `"dedx_pi"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_fit4c_all;  //!< Container for the `"fit4c_all"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_fit4c_best; //!< Container for the `"fit4c_best"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_fit5c_all;  //!< Container for the `"fit5c_all"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_fit5c_best; //!< Container for the `"fit5c_best"` branch.

	// * Cut parameters * //
		/// Here, you can define data members that you use to define cuts. The values for these cuts should be set in the `TrackSelector::TrackSelector` constructor (see `.cxx` file).
		double fCut_GammaPhi_max;   //!< Maximum \f$\phi\f$ angle between the photon and the nearest charged track <i>in radians</i>.
		double fCut_GammaTheta_max; //!< Maximum \f$\theta\f$ angle between the photon and the nearest charged track <i>in radians</i>.
		double fCut_GammaAngle_max; //!< Maximum angle between the photon and the nearest charged track <i>in degrees</i>.

private:
	// * Private data members (values)
	double fE_gamma1;   //!< Energy of the first photon.
	double fE_gamma2;   //!< Energy of the second photon.
	double fCosGamma;   //!< Cosine of the angle between the two photons.
	double fM_pi0;      //!< Invariant mass of \f$\pi^0 \rightarrow \gamma\gamma\f$.
	double fM_rho0;     //!< Invariant mass of \f$\rho^0 \rightarrow \pi^-\pi^+\f$.
	double fM_rhom;     //!< Invariant mass of \f$\rho^- \rightarrow \pi^0\pi^-\f$.
	double fM_rhop;     //!< Invariant mass of \f$\rho^+ \rightarrow \pi^0\pi^+\f$.
	double fM_JpsiRho0; //!< Invariant mass of \f$J/\psi \rightarrow \rho^0\pi^0\f$.
	double fM_JpsiRhom; //!< Invariant mass of \f$J/\psi \rightarrow \rho^+\pi^-\f$.
	double fM_JpsiRhop; //!< Invariant mass of \f$J/\psi \rightarrow \rho^-\pi^+\f$.

	double fSmallestAngle; //!< Current smallest angle between the photons and the charged tracks.
	double fSmallestPhi;   //!< Current smallest \f$\phi\f$ angle between the photons and the charged tracks.
	double fSmallestTheta; //!< Current smallest \f$\theta\f$ angle between the photons and the charged tracks.
	
	// * Private methods * //
	HepLorentzVector ComputeGammaVector(EvtRecTrack* track);
	double MeasureForBestFit4c();
	double MeasureForBestFit5c();
	void BookNtupleItemsFit(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle);
	void ComputeInvariantMasses(KalmanKinematicFit *kkmfit);
	void SetSmallestAngles(std::vector<EvtRecTrack*>::iterator &iter, std::vector<EvtRecTrack*> &vec, Hep3Vector &emcpos);
	void WriteFitResults(KalmanKinematicFit *kkmfit, std::map<std::string, NTuple::Item<double> > &map, const char *tupleName);


};

/// @}
// end of Doxygen group BOSS

#endif