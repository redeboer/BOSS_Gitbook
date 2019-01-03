// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/CutObject.h"



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	CutObject::CutObject(const std::string &cutname, const std::string &cutdescription) :
		min(-DBL_MAX),
		max(DBL_MAX),
		name(cutname),
		description(cutdescription),
		counter(0)
	{
		instances.push_back(this);
	}
	CutObject::~CutObject() {
		instances.remove(this);
	}