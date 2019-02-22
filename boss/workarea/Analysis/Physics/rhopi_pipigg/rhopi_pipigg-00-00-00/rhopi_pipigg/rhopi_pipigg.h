#ifndef Physics_Analysis_rhopi_pipigg_H
#define Physics_Analysis_rhopi_pipigg_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/TrackSelector.h"
	#include "rhopi_pipigg/KKFitResult_rhopi_pipigg.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_packages
/// @{

	/// @b WIP Analyse \f$ J/\psi \rightarrow \rho^{0,\pm}\pi^{0,\mp} \rightarrow \pi^-\pi^+\gamma\gamma \f$ events.
	/// Based on the original `RhopiAlg` package, but now making use of the `TrackSelector` base algorithm.
	/// @author  Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date    December 27th, 2018
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
			std::vector<Event::McParticle*> fMcPhotons; ///< Vector containing true \f$\gamma\f$'s.
			std::vector<Event::McParticle*> fMcPionNeg; ///< Vector containing true \f$\pi^-\f$.
			std::vector<Event::McParticle*> fMcPionPos; ///< Vector containing true \f$\pi^+\f$.
			std::vector<Event::McParticle*>::iterator fMcPhoton1Iter; ///< Iterator for looping over the MC collection of photons (1st occurence).
			std::vector<Event::McParticle*>::iterator fMcPhoton2Iter; ///< Iterator for looping over the MC collection of photons (2st occurence).
			std::vector<Event::McParticle*>::iterator fMcPionNegIter; ///< Iterator for looping over the MC collection of negative pions.
			std::vector<Event::McParticle*>::iterator fMcPionPosIter; ///< Iterator for looping over the MC collection of positive pions.
			std::vector<EvtRecTrack*> fPhotons; ///< Vector that contains a selection of pointers to neutral tracks identified as \f$\gamma\f$.
			std::vector<EvtRecTrack*> fPionNeg; ///< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^-\f$.
			std::vector<EvtRecTrack*> fPionPos; ///< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^+\f$.
			std::vector<EvtRecTrack*>::iterator fPhoton1Iter; ///< Iterator for looping over the collection of photons (1st occurence).
			std::vector<EvtRecTrack*>::iterator fPhoton2Iter; ///< Iterator for looping over the collection of photons (2st occurence).
			std::vector<EvtRecTrack*>::iterator fPionNegIter; ///< Iterator for looping over the collection of negative pions.
			std::vector<EvtRecTrack*>::iterator fPionPosIter; ///< Iterator for looping over the collection of positive pions.
			///@}


		/// @name NTuples (eventual TTrees)
			///@{
			NTupleContainer fNTuple_dedx_pi; ///< `NTuple::Tuple` container for the \f$dE/dx\f$ of pions.
			NTupleContainer fNTuple_fit4c;   ///< `NTuple::Tuple` container for the 4-constraint fit branch containing <i>all</i> combinations.
			NTupleContainer fNTuple_fit5c;   ///< `NTuple::Tuple` container for the 5-constraint fit branch containing <i>all</i> combinations.
			NTupleContainer fNTuple_fit_mc;  ///< `NTuple::Tuple` container for the 4-constraint fit of MC truth.
			NTupleContainer fNTuple_photon;  ///< `NTuple::Tuple` container for the photon branch.
			///@}


		/// @name Counters and cut objects
			///@{
			CutObject fCut_GammaPhi;   ///< Cut on \f$\phi\f$ angle between the photon and the nearest charged track <i>in radians</i>.
			CutObject fCut_GammaTheta; ///< Cut on \f$\theta\f$ angle between the photon and the nearest charged track <i>in radians</i>.
			CutObject fCut_GammaAngle; ///< Cut on angle between the photon and the nearest charged track <i>in degrees</i>.
			///@}


	private:
		/// @name NTuple methods
			///@{
			void AddNTupleItems_Fit(NTupleContainer &tuple);
			void SetFitNTuple(KKFitResult *fitresults, NTupleContainer &tuple);
			///@}


		/// @name Computational
			///@{
			HepLorentzVector ComputeGammaVector(EvtRecTrack* track);
			void SetSmallestAngles(std::vector<EvtRecTrack*>::iterator &iter, std::vector<EvtRecTrack*> &vec, Hep3Vector &emcpos);
			///@}


		/// @name Invariant masses
			///@{
			double fM_pi0;      ///< Invariant mass of \f$\pi^0 \rightarrow \gamma\gamma\f$.
			double fM_rho0;     ///< Invariant mass of \f$\rho^0 \rightarrow \pi^-\pi^+\f$.
			double fM_rhom;     ///< Invariant mass of \f$\rho^- \rightarrow \pi^0\pi^-\f$.
			double fM_rhop;     ///< Invariant mass of \f$\rho^+ \rightarrow \pi^0\pi^+\f$.
			double fM_JpsiRho0; ///< Invariant mass of \f$J/\psi \rightarrow \rho^0\pi^0\f$.
			double fM_JpsiRhom; ///< Invariant mass of \f$J/\psi \rightarrow \rho^+\pi^-\f$.
			double fM_JpsiRhop; ///< Invariant mass of \f$J/\psi \rightarrow \rho^-\pi^+\f$.
			///@}


		/// @name Other stored values
			///@{
			double fE_gamma1;   ///< Energy of the first photon.
			double fE_gamma2;   ///< Energy of the second photon.
			double fCosGamma;   ///< Cosine of the angle between the two photons.
			double fSmallestAngle; ///< Current smallest angle between the photons and the charged tracks.
			double fSmallestPhi;   ///< Current smallest \f$\phi\f$ angle between the photons and the charged tracks.
			double fSmallestTheta; ///< Current smallest \f$\theta\f$ angle between the photons and the charged tracks.
			///@}


	};

/// @}


#endif