#ifndef Physics_Analysis_D0phi_KpiKK_H
#define Physics_Analysis_D0phi_KpiKK_H

/**
 * @brief    Analyse \f$ J/\psi \rightarrow D^0\phi \rightarrow K^-\pi^+ \pi^-\pi^+ \f$ events.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 14th, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TrackSelector/TrackSelector.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class D0phi_KpiKK : public TrackSelector
{
public:
	// * Constructor and destructors *
	D0phi_KpiKK(const std::string &name, ISvcLocator* pSvcLocator);

	// * Algorithm steps *
	StatusCode initialize_rest();
	StatusCode execute_rest();
	StatusCode finalize_rest();


protected:
	// * Maps, vectors, and iterators * //
		std::vector<EvtRecTrack*> fKaonNeg; //!< Vector that contains a selection of pointers to charged tracks identified as \f$K^-\f$. @todo Decide if this can be formulated in terms of some `fEvtRecTrackMap`.
		std::vector<EvtRecTrack*> fKaonPos; //!< Vector that contains a selection of pointers to charged tracks identified as \f$K^+\f$.
		std::vector<EvtRecTrack*> fPionPos; //!< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^+\f$.
		std::vector<EvtRecTrack*>::iterator fKaonNeg1Iter; //!< Iterator for looping over the collection of negative kaons (1st occurence).
		std::vector<EvtRecTrack*>::iterator fKaonNeg2Iter; //!< Iterator for looping over the collection of negative kaons (2st occurence).
		std::vector<EvtRecTrack*>::iterator fKaonPosIter; //!< Iterator for looping over the collection of positive kaons.
		std::vector<EvtRecTrack*>::iterator fPionPosIter; //!< Iterator for looping over the collection of positive pions.

	// * Maps of Ntuples *
		/// `NTuple`s are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each `NTuple::Item` represents its leaves. The name of the leaves is determined when calling `NTuple::addItem`.
		/// Note that the `NTuple::Items` have to be added to the NTuple during the `TrackSelector::initialize()` step, otherwise they cannot be used as values! This is also the place where you name these variables, so make sure that the structure here is reflected there!
		bool fDo_mult_select; //!< Package property that determines whether or not to write the multiplicities <i>of the selected particles</i>.
		bool fDo_fit4c_all;   //!< Package property that determines whether or not to write results of the `4C` fit <i>for all combinations</i>.
		bool fDo_fit4c_best;  //!< Package property that determines whether or not to write results of the `4C` fit for the combination closest to the expected masses.
		std::map<std::string, NTuple::Item<double> > fMult_select; //!< Container for the `"dedx_k"` branch.
		std::map<std::string, NTuple::Item<double> > fDedx_K;      //!< Container for the `"dedx_k"` branch.
		std::map<std::string, NTuple::Item<double> > fDedx_pi;     //!< Container for the `"dedx_pi"` branch.
		std::map<std::string, NTuple::Item<double> > fFit4c_all;   //!< Container for the `"fit4c_all"` branch.
		std::map<std::string, NTuple::Item<double> > fFit4c_best;  //!< Container for the `"fit4c_best"` branch.

private:
	// * Private data members (values)
	double fM_D0;   //!< Current computed mass of \f$D^0\f$.
	double fM_Jpsi; //!< Current computed mass of \f$J/\psi\f$.
	double fM_phi;  //!< Current computed mass of \f$\phi\f$.

	// * Private methods
	double MeasureForBestFit();
	void ComputeInvariantMasses(KalmanKinematicFit *kkmfit);
	void WriteFitResults(KalmanKinematicFit *kkmfit, std::map<std::string, NTuple::Item<double> > &map, const char *tupleName);

};

#endif