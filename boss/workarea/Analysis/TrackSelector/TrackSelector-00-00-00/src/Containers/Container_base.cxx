// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/Container_base.h"



// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //

	/**
	 * @brief Rename the object. Gives controlled access to the `private` `fName` member.
	 */
	void Container_base::SetName(const std::string &new_name)
	{
		fName = new_name;
	}

	/**
	 * @brief Give a new description to the object. Gives controlled access to the `private` `fDescription` member.
	 */
	void Container_base::SetName(const std::string &new_desc)
	{
		fDescription = new_desc;
	}

	/**
	 * @brief Rename the object and give it a new description. Gives controlled access to the `private` `fName` and `fDescription` members.
	 */
	void Container_base::SetName(const std::string &new_name, const std::string &new_desc)
	{
		SetName(new_name);
		SetDescription(new_desc);
	}