#ifndef Physics_Analysis_rhopi_pipigg_H
#define Physics_Analysis_rhopi_pipigg_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TrackSelector/TrackSelector.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_packages
/// @{

	/**
	 * @brief   <b>WIP</b> Analyse \f$ J/\psi \rightarrow \rho^{0,\pm}\pi^{0,\mp} \rightarrow \pi^-\pi^+\gamma\gamma \f$ events.
	 * @details Based on the original `RhopiAlg` package, but now making use of the `TrackSelector` base algorithm.
	 * @author  Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date    December 27th, 2018
	 */
	class rhopi_pipigg : public TrackSelector
	{
	public:
		/// @name Constructors
			///@{
			rhopi_pipigg(const std::string &name, ISvcLocator* pSvcLocator);
			///@}

		/// @name Derived Algorithm steps
			///@{
			StatusCode initialize_rest();
			StatusCode execute_rest();
			StatusCode finalize_rest();
			///@}


	protected:
		/// @name Track collections and iterators
			///@{
			std::vector<EvtRecTrack*> fGamma;   //!< Vector that contains a selection of pointers to neutral tracks identified as \f$\gamma\f$.
			std::vector<EvtRecTrack*> fPionNeg; //!< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^-\f$.
			std::vector<EvtRecTrack*> fPionPos; //!< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^+\f$.
			std::vector<EvtRecTrack*>::iterator fGamma1Iter;  //!< Iterator for looping over the collection of gamma's (1st occurence).
			std::vector<EvtRecTrack*>::iterator fGamma2Iter;  //!< Iterator for looping over the collection of gamma's (2st occurence).
			std::vector<EvtRecTrack*>::iterator fPionNegIter; //!< Iterator for looping over the collection of negative pions.
			std::vector<EvtRecTrack*>::iterator fPionPosIter; //!< Iterator for looping over the collection of positive pions.
			///@}


		/// @name Control switches
			///@{
			bool fWrite_mult_select; //!< Package property that determines whether or not to write the multiplicities <i>of the selected particles</i>.
			bool fWrite_photon;      //!< Package property that determines whether or not to write angle info from the photons.
			bool fWrite_fit4c_all;   //!< Package property that determines whether or not to write results of the `4C` fit <i>for all combinations</i>.
			bool fWrite_fit4c_best;  //!< Package property that determines whether or not to write results of the `4C` fit for the combination closest to the expected masses.
			bool fWrite_fit5c_all;   //!< Package property that determines whether or not to write results of the `5C` fit <i>for all combinations</i>.
			bool fWrite_fit5c_best;  //!< Package property that determines whether or not to write results of the `5C` fit for the combination closest to the expected masses.
			///@}


		// ! NTuple data members ! //
		/// @name Maps of NTuples
			///@{
			std::map<std::string, NTuple::Item<double> > fMap_photon;     //!< Container for the `"gamma"` branch.
			std::map<std::string, NTuple::Item<double> > fMap_dedx_pi;    //!< Container for the `"dedx_pi"` branch.
			std::map<std::string, NTuple::Item<double> > fMap_fit4c_all;  //!< Container for the `"fit4c_all"` branch.
			std::map<std::string, NTuple::Item<double> > fMap_fit4c_best; //!< Container for the `"fit4c_best"` branch.
			std::map<std::string, NTuple::Item<double> > fMap_fit5c_all;  //!< Container for the `"fit5c_all"` branch.
			std::map<std::string, NTuple::Item<double> > fMap_fit5c_best; //!< Container for the `"fit5c_best"` branch.
			///@}

		/// @name Counters and cut objects
			///@{
			double fCut_GammaPhi;   //!< Cut on \f$\phi\f$ angle between the photon and the nearest charged track <i>in radians</i>.
			double fCut_GammaTheta; //!< Cut on \f$\theta\f$ angle between the photon and the nearest charged track <i>in radians</i>.
			double fCut_GammaAngle; //!< Cut on angle between the photon and the nearest charged track <i>in degrees</i>.
			///@}

	private:
		/// @name Invariant masses
			///@{
			double fM_pi0;      //!< Invariant mass of \f$\pi^0 \rightarrow \gamma\gamma\f$.
			double fM_rho0;     //!< Invariant mass of \f$\rho^0 \rightarrow \pi^-\pi^+\f$.
			double fM_rhom;     //!< Invariant mass of \f$\rho^- \rightarrow \pi^0\pi^-\f$.
			double fM_rhop;     //!< Invariant mass of \f$\rho^+ \rightarrow \pi^0\pi^+\f$.
			double fM_JpsiRho0; //!< Invariant mass of \f$J/\psi \rightarrow \rho^0\pi^0\f$.
			double fM_JpsiRhom; //!< Invariant mass of \f$J/\psi \rightarrow \rho^+\pi^-\f$.
			double fM_JpsiRhop; //!< Invariant mass of \f$J/\psi \rightarrow \rho^-\pi^+\f$.
			///@}

		/// @name Other stored values
			///@{
			double fE_gamma1;   //!< Energy of the first photon.
			double fE_gamma2;   //!< Energy of the second photon.
			double fCosGamma;   //!< Cosine of the angle between the two photons.
			double fSmallestAngle; //!< Current smallest angle between the photons and the charged tracks.
			double fSmallestPhi;   //!< Current smallest \f$\phi\f$ angle between the photons and the charged tracks.
			double fSmallestTheta; //!< Current smallest \f$\theta\f$ angle between the photons and the charged tracks.
			///@}
		
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


#endif