#ifndef Physics_NTupleMap_H
#define Physics_NTupleMap_H

/**
 * @brief    Analyse \f$ J/\psi \rightarrow D^0 \phi \f$ events.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 14th, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/Algorithm.h"
	#include "GaudiKernel/NTuple.h"
	#include <map>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class NTupleMap : public Algorithm
{
public:
	// * Constructor and destructors *
	NTupleMap(const std::string& name, ISvcLocator* pSvcLocator) : Algorithm(name, pSvcLocator) {};

protected:
	// * Private data members * //
	NTuplePtr BookNTuple(const char* tupleName);
	std::map<std::string, NTuplePtr> fNTupleMap;
		//!< Map for `NTuplePtr`s. The string identifier should be the name of the `NTuple` and of the eventual `TTree`.

};

#endif