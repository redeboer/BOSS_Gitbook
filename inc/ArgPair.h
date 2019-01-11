#ifndef BOSS_Afterburner_ArgPair_H
#define BOSS_Afterburner_ArgPair_H

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include <list>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_Afterburner_objects
/// @{


	/**
	 * @brief    Class that can contain parameters and values of a loaded configuration file.
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


	/**
	 * @brief Specialisation of class `ArgPair` for `typename` `std::string`.
	 */
	template class ArgPair<std::string>;


	/**
	 * @brief Specialisation of class `ArgPair` for `typename` `bool`.
	 */
	template class ArgPair<bool>;


/// @}
#endif