#ifndef Analysis_JobProperty_H
#define Analysis_JobProperty_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include <list>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_objects
/// @{

	/**
	 * @brief This class is the base class for any property that is declared using `Algorithm::declareProperty()`. It is currently quite simple and only contains a name and a description.
	 *
	 * @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date   February 15th, 2019
	 */
	class JobProperty
	{
	public:
		JobProperty(const std::string &cutname, const std::string &cutdescription="") : name(cutname), description(cutdescription) {}
		~JobProperty() {}
		void SetDescription(const double &str) { description = str; }
		void SetName(const double &str) { name = str; }
		std::string description;
		std::string name;
	};


/// @}
#endif