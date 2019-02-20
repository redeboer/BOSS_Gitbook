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

	/**
	 * @brief This class is the base class for objects requiring a name and a description. This can be useful for instance for defining derived containers containing properties and `NTuple`s. See <a href="https://besiii.gitbook.io/boss/the-boss-afterburner/initial/motivation#problems-with-booking-procedures">this page</a> for the motivation.
	 * @todo Encapsulate in `namespace` structure.
	 *
	 * @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date   February 15th, 2019
	 */
	class Container_base
	{
	public:
		Container_base(const std::string &obj_name, const std::string &obj_desc="") : fName(obj_name), fDescription(obj_desc) {}
		void SetName(const std::string &new_name);
		void SetName(const std::string &new_name, const std::string &new_desc);
		void SetDescription(const std::string &new_desc);
		const std::string Name() const { return fName; };
		const std::string Description() const { return fDescription; };
	private:
		std::string fDescription; //!< Title or description of the container object.
		std::string fName; //!< Name/identifier of the container object.
	};


/// @}
#endif