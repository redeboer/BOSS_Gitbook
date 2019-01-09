#ifndef Physics_Analysis_D0phi_KpiKK_H
#define Physics_Analysis_D0phi_KpiKK_H

/// @addtogroup BOSS
/// @{

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
		bool fWrite_fit4c_all;     //!< Package property that determines whether or not to write results of the `4C` fit <i>for all combinations</i>.
		bool fWrite_fit4c_best;    //!< Package property that determines whether or not to write results of the `4C` fit for the combination closest to <i>both</i> \f$m_{D^0}\f$ and \f$m_\phi\f$.
		bool fWrite_fit4c_best_D0;  //!< Package property that determines whether or not to write results of the `4C` fit for the combination closest to \f$m_{D^0}\f$ only.
		bool fWrite_fit4c_best_phi; //!< Package property that determines whether or not to write results of the `4C` fit for the combination closest to \f$m_\phi\f$ only.
		std::map<std::string, NTuple::Item<double> > fMap_dedx_K;  //!< Container for the `"dedx_k"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_dedx_pi; //!< Container for the `"dedx_pi"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_fit4c_all;     //!< Container for the `"fit4c_all"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_fit4c_best;    //!< Container for the `"fit4c_best"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_fit4c_best_D0;  //!< Container for the `"fit4c_best_D0"` branch.
		std::map<std::string, NTuple::Item<double> > fMap_fit4c_best_phi; //!< Container for the `"fit4c_best_phi"` branch.

private:
	// * Private methods
	void BookNtupleItemsFit(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle);
	void WriteFitResults(KalmanKinematicFit *kkmfit, std::map<std::string, NTuple::Item<double> > &map, const char *tupleName);

};


/**
 * @brief Derived class for a container that contains important fit results of the `KalmanKinematicFit`, including masses.
 */
class KKFitResult_D0phi_KpiKK : public KKFitResult
{
	KKFitResult_D0phi_KpiKK() {}
	KKFitResult_D0phi_KpiKK(KalmanKinematicFit* kkmfit) : KKFitResult(kkmfit) {}

	// * Public data members * //
	static double fBestCompareValue_D0  = 1e9; //!< Current best value for the comparison with regard to \f$D^0\f$. See `KKFitResult::fBestCompareValue`.
	static double fBestCompareValue_phi = 1e9; //!< Current best value for the comparison with regard to \f$\phi\f$. See `KKFitResult::fBestCompareValue`.
	double fM_D0;   //!< Current computed mass of \f$D^0\f$.
	double fM_Jpsi; //!< Current computed mass of \f$J/\psi\f$.
	double fM_phi;  //!< Current computed mass of \f$\phi\f$.
	double fFitMeasure;
	double fFitMeasure_D0;
	double fFitMeasure_phi;
	bool IsBetter_D0();
	bool IsBetter_phi();

private:
	// void SetValues_rest();
};

/// @}
// end of Doxygen group BOSS

#endif