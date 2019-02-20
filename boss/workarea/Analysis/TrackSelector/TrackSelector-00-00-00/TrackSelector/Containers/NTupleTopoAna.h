#ifndef Physics_Analysis_NTupleTopoAna_H
#define Physics_Analysis_NTupleTopoAna_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "GaudiKernel/NTuple.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_packages
/// @{

	/**
	 * @brief Struct object that represents the `NTuple` containing MC truth necessary for the `topoana` package.
	 * @remark February 18th, 2019
	 * @author Remco de Boer 雷穆克
	 */
	struct NTupleTopoAna
	{
		NTuple::Tuple *tuple;      //!< `NTuple` that will be the eventual `"MctruthForTopoAna"` tree.
		// NTuple::Item<int>  iEvt;   //!< <b>Counter</b> for number of events (not the ID!)
		NTuple::Item<int>  runID;  //!< Run number ID.
		NTuple::Item<int>  evtID;  //!< Rvent number ID.
		NTuple::Item<int>  nItems; //!< Number of MC particles stored for this event. This one is necessary for loading following two items, because they are arrays.
		NTuple::Array<int> particle; //!< PDG code for the particle in this array.
		NTuple::Array<int> mother; //!< Track index of the mother particle.
		explicit operator bool() const { return tuple; }
		void Write() { if(tuple) tuple->write(); }
	};

/// @}
#endif