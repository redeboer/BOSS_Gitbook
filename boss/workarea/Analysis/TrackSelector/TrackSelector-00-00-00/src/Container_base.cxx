// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/Container_base.h"



// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //

	void Container_base::SetName(const std::string &name)
		/// Rename the object.
		/// Gives controlled access to the `private` `fName` member.
	{
		fName = name;
	}

	void Container_base::SetName(const std::string &name, const std::string &description)
		/// Rename the object and give it a new description.
		/// Gives controlled access to the `private` `fName` and `fDescription` members.
	{
		SetName(name);
		SetDescription(description);
	}

	void Container_base::SetDescription(const std::string &desc)
		/// Give a new description to the object.
		/// Gives controlled access to the `private` `fDescription` member.
	{
		fDescription = desc;
	}