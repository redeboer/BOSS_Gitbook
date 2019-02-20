// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/JobSwitch.h"



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	JobSwitch::JobSwitch(const std::string &name, const std::string &description, const bool val) :
		Container_base(name, description),
		value(val)
	{
		gJobSwitches.push_back(this);
	}

	JobSwitch::~JobSwitch() {
		gJobSwitches.remove(this);
	}

	std::list<JobSwitch*> JobSwitch::gJobSwitches;