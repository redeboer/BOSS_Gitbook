// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/INTupleSvc.h"
	#include "NTupleMap/NTupleMap.h"
	#include "TString.h"
	#include <map>
	#include <string>

// * ================================= * //
// * -------- PRIVATE METHODS -------- * //
// * ================================= * //
/**
 * @brief   Helper function that allows you to create pair of a string with a `NTuplePtr`.
 * @details This function first attempts to see if there is already an `NTuple` in the output file. If not, it will book an `NTuple` of 
 *
 * @param tupleName This will not only be the name of your `NTuple`, but also the name of the `TTree` in the output ROOT file when you use `NTuple::write()`. The name used here is also used as identifier in `NTuplePtr` map `fNTupleMap`. In other words, you can get any of the `NTuplePtr`s in this map by using `fNTupleMap[<tuple name>]`. If there is no `NTuple` of this name, it will return a `nullptr`.
 */
NTuplePtr NTupleMap::BookNTuple(const char* tupleName) {
	const char* bookName = Form("FILE1/%s", tupleName);
	NTuplePtr nt(ntupleSvc(), bookName); // attempt to get from file
	if(!nt) { // if not available in file, book a new one
		nt = ntupleSvc()->book(bookName, CLID_ColumnWiseTuple, "ks N-Tuple example");
		if(!nt) log << MSG::ERROR << "    Cannot book N-tuple:" << long(fNTupleMap[tupleName]) << endmsg;
	}
	fNTupleMap[tupleName] = nt;
	return nt;
}