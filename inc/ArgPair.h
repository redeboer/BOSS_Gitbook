#ifndef Physics_Analysis_ArgPair_H
#define Physics_Analysis_ArgPair_H

/// @addtogroup BOSS_Afterburner
/// @{

/**
 * @brief    Class that can contain parameters and values of a loaded configuration file.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     January 7th, 2019
 */

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include <iomanip>
	#include <iostream>
	#include <list>
	#include <sstream>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
	// * BASE CLASS * //
	class ArgPair_base {
	public:
		ArgPair_base(const std::string &input) : name(input) {}
		static void PrintAll();
		static void SetParameters(const std::string &parname, const std::string &parvalue, bool output=false);
		virtual void Print(int width=0) = 0;
		virtual void SetParameter(const std::string &parname, const std::string &parvalue, bool output=false) = 0;
		std::string name;
	protected:
		static std::list<ArgPair_base*> instances;
		void PrintName(int width=0);
	};

	// * TEMPLATE CLASS * //
	template<typename TYPE>
	class ArgPair : public ArgPair_base {
	public:
		ArgPair(const std::string &input, const TYPE &val);
		void SetParameter(const std::string &parname, const std::string &parvalue, bool output=false);
		void operator=(const TYPE &val) { value = val; }
		bool operator==(const std::string &data) const { return !name.compare(data); }
		bool operator!=(const std::string &data) const { return  name.compare(data); }
		void Print(int width=0);
		TYPE value;
	private:
		void PrintValue();
		void SetValue(const std::string &input);
	};

	// * SPECIALIZATIONS * //
	template class ArgPair<std::string>;
	template class ArgPair<bool>;


// * ============================ * //
// * ------- CONSTRUCTORS ------- * //
// * ============================ * //

	template<typename TYPE>
	ArgPair<TYPE>::ArgPair(const std::string &input, const TYPE &val) :
		value(val), ArgPair_base(input)
	{
		ArgPair_base::instances.push_back(this);
	}


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

	/**
	 * @brief Print a `name` and `value` this parameter object.
	 * @param width Use this parameter if you want to print the `name` of the parameter in a certain column width. This can be useful when printing a table using `PrintAll`.
	 */
	template<typename TYPE>
	void ArgPair<TYPE>::Print(int width)
	{
		PrintName(width);
		PrintValue();
		std::cout << std::endl;
	}


	/** 
	 * @brief General template method that prints the value of this paramter object.
	 * @details This method has been defined to allow for specialisations.
	 */
	template<typename TYPE>
	void ArgPair<TYPE>::PrintValue()
	{
		std::cout << value;
	}


	/** 
	 * @brief Specialisation `ArgPair::Print` in the case of a `string`.
	 * @details This method prints quotation marks around the parameter value.
	 */
	template<> void ArgPair<std::string>::PrintValue()
	{
		std::cout << "\"" << value << "\"";
	}


	/** 
	 * @brief Specialisation `ArgPair::Print` in the case of a `bool`ean.
	 * @details This method prints `false` if the value is `0` and `true` if otherwise.
	 */
	template<> void ArgPair<bool>::PrintValue()
	{
		if(value) std::cout << "true";
		else      std::cout << "false";
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


	template<typename TYPE>
	void ArgPair<TYPE>::SetValue(const std::string &input) {
		std::istringstream ss(input);
		ss >> value;
	}


	template<typename TYPE>
	void ArgPair<TYPE>::SetParameter(const std::string &parname, const std::string &parvalue, bool output)
	{
		if(name.compare(parname)) return;
		SetValue(parvalue);
		if(output) Print();
	}


	template<> void ArgPair<std::string>::SetParameter(const std::string &parname, const std::string &parvalue, bool output)
	{
		if(name.compare(parname)) return;
		value = parvalue;
		if(output) Print();
	}


	template<> void ArgPair<bool>::SetParameter(const std::string &parname, const std::string &parvalue, bool output)
	{
		if(name.compare(parname)) return;
		if(!parvalue.compare("false") || parvalue.front()=='0') value = false;
		else value = true;
		if(output) Print();
	}


	void ArgPair_base::SetParameters(const std::string &parname, const std::string &parvalue, bool output)
	{
		for(auto item : ArgPair_base::instances) {
			item->SetParameter(parname, parvalue, output);
		}
	}

/// @}
// end of Doxygen group BOSS_Afterburner


#endif