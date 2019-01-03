#ifndef Analysis_CutObject_H
#define Analysis_CutObject_H

/**
 * @brief
 * @details
 *
 * @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date   January 3rd, 2019
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TString.h"
	#include "GaudiKernel/NTuple.h"
	#include <list>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class CutObject
{
public:
	CutObject(const std::string &name, const std::string &description="");
	~CutObject();
	bool operator< (const double &value) const { return value<max; }
	bool operator==(const double &value) const { return value<max && value>min; }
	bool operator> (const double &value) const { return value>min; }
	void operator++() { ++counter; }
	const char* NameMax() { return Form("cut_%s_max", name.data()); }
	const char* NameMin() { return Form("cut_%s_min", name.data()); }
	double& Max() { return max; }
	double& Min() { return min; }
	static void Write() { if(ntuple) ntuple->write(); }
	void SetDescription(const double &str) { description = str; }
	void SetName(const double &str) { name = str; }

	double max;
	double min;
	static std::list<CutObject*> instances;
	static NTuplePtr ntuple;
	std::string description;
	std::string name;
	unsigned long long counter;

};

#endif