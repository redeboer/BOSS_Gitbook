// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/JobSwitch.h"
	#include <cmath>
	#include <iostream>
	#include <iomanip>



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	JobSwitch::JobSwitch(const std::string &name, const std::string &description, const bool val) :
		Container_base(name, description),
		fValue(val)
	{
		gJobSwitches.push_back(this);
	}

	JobSwitch::~JobSwitch() {
		gJobSwitches.remove(this);
	}

	std::list<JobSwitch*> JobSwitch::gJobSwitches;



// * =========================== * //
// * ------- INFORMATION ------- * //
// * =========================== * //


	/// Static function for debugging purposes.
	/// Print the name and value of each `JobSwitch` in the instances stored in `gJobSwitches`.
	void JobSwitch::PrintSwitches()
	{
		/// -# @b Abort if there is no instance of any `JobSwitch` object.
			if(!gJobSwitches.size()) {
				std::cout << "There are no JobSwitches" << std::endl;
				return;
			}
		/// -# Print header line.
			std::cout << "List of " << gJobSwitches.size() << " job switches:" << std::endl;
		/// -# Get width for columns.
			std::list<JobSwitch*>::iterator job;
			size_t width(0);
			for(job = gJobSwitches.begin(); job != gJobSwitches.end(); ++job)
				if((*job)->Name().size() > width) width = (*job)->Name().size();
			width += 2;
		/// -# Print table rows.
			for(job = gJobSwitches.begin(); job != gJobSwitches.end(); ++job)
				std::cout << std::setw(width) << (*job)->Name() << " = " << (*job)->Value() << std::endl;
	}