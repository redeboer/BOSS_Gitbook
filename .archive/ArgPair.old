#ifndef BOSS_Afterburner_ArgPair_H
#define BOSS_Afterburner_ArgPair_H

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include <list>
	#include <string>
	#include <sstream>
	#include <iostream>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_Afterburner_objects
/// @{


	/**
	 * @brief    Base class for `ArgPair`.
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     January 7th, 2019
	 */
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


	/**
	 * @brief    Class that can contain parameters and values of a loaded configuration file.
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     January 7th, 2019
	 */
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



/// @}
// * =============================== * //
// * ------- SPECIALISATIONS ------- * //
// * =============================== * //
// ! This list needs to be expended if you start using more typenames
	template class ArgPair<std::string>;
	template class ArgPair<bool>;


// * ============================ * //
// * ------- CONSTRUCTORS ------- * //
// * ============================ * //

	template<typename TYPE> inline
	ArgPair<TYPE>::ArgPair(const std::string &input, const TYPE &val) :
		value(val), ArgPair_base(input)
	{
		ArgPair_base::instances.push_back(this);
	}



// * =========================== * //
// * ------- INFORMATION ------- * //
// * =========================== * //

	/**
	 * @brief Print a `name` and `value` this parameter object.
	 * @param width Use this parameter if you want to print the `name` of the parameter in a certain column width. This can be useful when printing a table using `PrintAll`.
	 */
	template<typename TYPE> inline
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
	template<typename TYPE> inline
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


// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //


	template<typename TYPE> inline
	void ArgPair<TYPE>::SetValue(const std::string &input) {
		std::istringstream ss(input);
		ss >> value;
	}


	template<typename TYPE> inline
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


#endif