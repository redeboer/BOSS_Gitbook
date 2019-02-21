#ifndef Physics_Analysis_NTupleTopoAna_H
#define Physics_Analysis_NTupleTopoAna_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/NTupleContainer.h"
	#include "GaudiKernel/NTuple.h"



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
			///@}


		/// @name NTuple handlers and members
			///@{
			NTuple::Tuple* GetNTuple() { return fTuple; } ///< Access to `fNTuple` (as opposed to `NTupleContainer`). @todo Develop template `NTupleContainer::AddItem` and `AddIndexedItem` procedure that removes the need to access `fTuple`.
			///@}


		/// @name Specific NTuple::Items and NTuple::Arrays
			///@{
			// NTuple::Item<int>  iEvt;     ///< @b Counter for number of events (not the ID!)
			NTuple::Item<int>  runID;    ///< Run number ID.
			NTuple::Item<int>  evtID;    ///< Rvent number ID.
			NTuple::Item<int>  nItems;   ///< Number of MC particles stored for this event. This one is necessary for loading following two items, because they are arrays.
			NTuple::Array<int> particle; ///< PDG code for the particle in this array.
			NTuple::Array<int> mother;   ///< Track index of the mother particle.
			///@}
	};

/// @}
#endif