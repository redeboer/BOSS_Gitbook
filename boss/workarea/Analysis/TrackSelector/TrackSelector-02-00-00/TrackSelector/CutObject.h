#ifndef Analysis_CutObject_H
#define Analysis_CutObject_H

/// @addtogroup BOSS_objects
/// @{

/**
 * @brief This object contains all information and tools for a cut you want to apply.
 * @details It contains `min` and `max` value of the cut parameter, a counter for the number of events or tracks that passed this cut test, as well as a name and description for the cut.
 *
 * @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date   January 3rd, 2019
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TString.h"
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
	bool FailsCut(const double &value) { if(value>=max || value<=min) return true; ++counter; return false; }
	bool FailsMax(const double &value) { if(value>=max) return true; ++counter; return false; }
	bool FailsMin(const double &value) { if(value<=min) return true; ++counter; return false; }
	bool operator< (const double &value) const { return value<max; }
	bool operator==(const double &value) const { return value<max && value>min; }
	bool operator> (const double &value) const { return value>min; }
	void operator++() { ++counter; }
	template<typename T> void operator+=(T incr) { counter += incr; }
	const char* NameMax() { return Form("cut_%s_max", name.data()); }
	const char* NameMin() { return Form("cut_%s_min", name.data()); }
	void SetDescription(const double &str) { description = str; }
	void SetName(const double &str) { name = str; }
	void Print(const int wname=0, const int wmin=0, const int wmax=0, const int wcounter=0) const;

	double max;
	double min;
	std::string description;
	std::string name;
	unsigned long long counter;

	// * STATIC MEMBERS * //
	static void PrintAll();
	static std::list<CutObject*> instances;

};

/// @}


#endif