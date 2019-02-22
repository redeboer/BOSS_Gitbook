#ifndef Physics_Analysis_D0phi_KpiKK_H
#define Physics_Analysis_D0phi_KpiKK_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/TrackSelector.h"
	#include "D0phi_KpiKK/KKFitResult_D0phi_KpiKK.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_packages
/// @{

	/// Analyse \f$ J/\psi \rightarrow D^0\phi \rightarrow K^-\pi^+ \pi^-\pi^+ \f$ events.
	/// @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date     December 14th, 2018
	class D0phi_KpiKK : public TrackSelector
	{
	public:
		/// @name Constructors
			///@{
			D0phi_KpiKK(const std::string &name, ISvcLocator* pSvcLocator);
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
			std::vector<Event::McParticle*> fMcKaonNeg; ///< Vector containing true \f$K^-\f$.
			std::vector<Event::McParticle*> fMcKaonPos; ///< Vector containing true \f$K^+\f$.
			std::vector<Event::McParticle*> fMcPionPos; ///< Vector containing true \f$\pi^+\f$.
			std::vector<Event::McParticle*>::iterator fMcKaonNeg1Iter; ///< Iterator for looping over the MC collection of negative kaons (1st occurence).
			std::vector<Event::McParticle*>::iterator fMcKaonNeg2Iter; ///< Iterator for looping over the MC collection of negative kaons (2st occurence).
			std::vector<Event::McParticle*>::iterator fMcKaonPosIter;  ///< Iterator for looping over the MC collection of positive kaons.
			std::vector<Event::McParticle*>::iterator fMcPionPosIter;  ///< Iterator for looping over the MC collection of positive pions.
			std::vector<EvtRecTrack*> fKaonNeg; ///< Vector that contains a selection of pointers to charged tracks identified as \f$K^-\f$. @todo Decide if this can be formulated in terms of some `fEvtRecTrackMap`.
			std::vector<EvtRecTrack*> fKaonPos; ///< Vector that contains a selection of pointers to charged tracks identified as \f$K^+\f$.
			std::vector<EvtRecTrack*> fPionPos; ///< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^+\f$.
			std::vector<EvtRecTrack*>::iterator fKaonNeg1Iter; ///< Iterator for looping over the collection of negative kaons (1st occurence).
			std::vector<EvtRecTrack*>::iterator fKaonNeg2Iter; ///< Iterator for looping over the collection of negative kaons (2st occurence).
			std::vector<EvtRecTrack*>::iterator fKaonPosIter;  ///< Iterator for looping over the collection of positive kaons.
			std::vector<EvtRecTrack*>::iterator fPionPosIter;  ///< Iterator for looping over the collection of positive pions.
			///@}


		/// @name NTuples (eventual TTrees)
			///@{
			NTupleContainer fNTuple_dedx_K;     ///< `NTuple::Tuple` container for the \f$dE/dx\f$ of kaons.
			NTupleContainer fNTuple_dedx_pi;    ///< `NTuple::Tuple` container for the \f$dE/dx\f$ of pions.
			NTupleContainer fNTuple_fit4c_all;  ///< `NTuple::Tuple` container for the 4-constraint fit branch containing <i>all</i> combinations.
			NTupleContainer fNTuple_fit4c_best; ///< `NTuple::Tuple` container for the 4-constraint fit branch containing only the <i>best</i> combination.
			NTupleContainer fNTuple_fit_mc;     ///< `NTuple::Tuple` container for the 4-constraint fit of MC truth.
			///@}


	private:
		/// @name NTuple methods
			///@{
			void AddNTupleItems_Fit(NTupleContainer &tuple);
			void SetFitNTuple(KKFitResult *fitresults, NTupleContainer &tuple);
			///@}


	};



/// @}
#endif