#ifndef Physics_Analysis_NTupleTopoAna_H
#define Physics_Analysis_NTupleTopoAna_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "GaudiKernel/NTuple.h"
	#include "McTruth/McParticle.h"
	#include "TrackSelector/Containers/NTupleContainer.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_packages
/// @{

	/// Extension of `NTupleContainer` that represents the `NTuple` containing MC truth necessary for the `topoana` package.
	/// @author Remco de Boer 雷穆克
	/// @date February 18th, 2019
	class NTupleTopoAna : public NTupleContainer
	{
	public:
		/// @name Constructors and destructors
			///@{
			NTupleTopoAna(const std::string &name, const std::string &desc) : NTupleContainer(name, desc) {} /// @remark Here you can define the name and title of the branch for the TopoAna pacakge!
			static bool IsInitialCluster(Event::McParticle *mcparticle) {
				return (
					mcparticle->particleProperty() == 91 ||
					mcparticle->particleProperty() == 92 );
			} ///< Test whether an MC truth particle is `91` (the PDG code of a 'cluster') or `92` (the PDG code of a string). This function is used to characterised the initial cluster, e.g. \f$J/\psi\f$
			///@}


		/// @name NTuple handlers and members
			///@{
			NTuple::Tuple* GetNTuple() { return fTuple; } ///< Access to `fNTuple` (as opposed to `NTupleContainer`). @todo Develop template `NTupleContainer::AddItem` and `AddIndexedItem` procedure that removes the need to access `fTuple`.
			///@}


		/// @name Specific NTuple::Items and NTuple::Arrays
			///@{
			// NTuple::Item<int>  iEvt;     ///< @b Counter for number of events (not the ID!)
			NTuple::Item<int>  runID;    ///< Run number ID.
			NTuple::Item<int>  evtID;    ///< Event number ID.
			NTuple::Item<int>  index;    ///< Number of MC particles stored for this event. This one is necessary for loading following two items, because they are arrays. @remark It is called `index`, because you use it as an index in the `WriteMcTruthForTopoAna` loop when filling the `particle` and `mother` arrays. Of course, when you exit the loop, this index also gives the number of items.
			NTuple::Array<int> particle; ///< PDG code for the particle in this array.
			NTuple::Array<int> mother;   ///< Array index that points to which particle in the `particle` array is the mother particle. Is a `-1` if the mother is a cluster (not stored in the `particle` array).
			///@}
	};

/// @}
#endif