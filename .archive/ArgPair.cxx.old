// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "ArgPair.h"
	#include <iomanip>


// * ============================ * //
// * ------- CONSTRUCTORS ------- * //
// * ============================ * //


	std::list<ArgPair_base*> ArgPair_base::instances;



// * =========================== * //
// * ------- INFORMATION ------- * //
// * =========================== * //


	/**
	 * @brief Method that defines the first part of the derived `ArgPair::Print` method.
	 * @details Since the behavior is the same for each type of parameter, this method has been defined in the `ArgPair_base` base clase.
	 * @param width Use this parameter if you want to print the `name` of the parameter in a certain column width. This can be useful when printing a table using `PrintAll`.
	 */
	void ArgPair_base::PrintName(int width)
	{
		std::cout << "  "; /// An indent is printed by default.
		if(width) std::cout << std::right << std::setw(width);
		std::cout << name << " = ";
	}

	void ArgPair_base::PrintAll()
	{
		/// -# Get string length of the parameter with the longest name.
		size_t maxwidth = 0;
		for(auto item : instances) {
			if(item->name.length()>maxwidth) maxwidth = item->name.length();
		}
		/// -# Print all items.
		for(auto item : instances) item->Print(maxwidth);
		std::cout << std::endl;
	}


// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //


	void ArgPair_base::SetParameters(const std::string &parname, const std::string &parvalue, bool output)
	{
		for(auto item : ArgPair_base::instances) {
			item->SetParameter(parname, parvalue, output);
		}
	}