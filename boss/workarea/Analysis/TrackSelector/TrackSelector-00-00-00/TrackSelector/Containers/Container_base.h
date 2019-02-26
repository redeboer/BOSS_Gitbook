#ifndef Analysis_Container_base_H
#define Analysis_Container_base_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_objects
/// @{

	/// This class is the base class for objects requiring a name and a description. This can be useful for instance for defining derived containers containing properties and `NTuple`s. See <a href="https://besiii.gitbook.io/boss/the-boss-afterburner/initial/motivation#problems-with-booking-procedures">this page</a> for the motivation.
	/// @todo Encapsulate in `namespace` structure.
	/// @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date   February 15th, 2019
	class Container_base
	{
	public:
		Container_base(const std::string &name, const std::string &desc="") : fName(name), fDescription(desc) {} ///< Only construct the name and description strings
		virtual void SetName(const std::string &name);
		virtual void SetName(const std::string &name, const std::string &desc);
		virtual void SetDescription(const std::string &desc);
		const std::string& Name() const { return fName; } ///< Get a pointer to the `C` string (!) of the name.
		const std::string& Description() const { return fDescription; } ///< Get a pointer to the `C` string (!) of the description.
	private:
		std::string fName; ///< Name/identifier of the container object.
		std::string fDescription; ///< Title or description of the container object.
	};


/// @}
#endif