#ifndef Analysis_Container_indexed_H
#define Analysis_Container_indexed_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/Container_base.h"
	#include <string>
	#include <map>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_objects
/// @{

	/**
	 * @brief This is an extension of the `Container_base` base class that in addition keeps track of all instanecs in a static mapping of all its instances.
	 *
	 * @todo See if static `instances` member is shared in derived classes or whether they are independent.
	 *
	 * @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date   February 20th, 2019
	 */
	class Container_indexed : public Container_base
	{
	public:
		Container_indexed(const std::string &name, const std::string &desccription="");
		~Container_indexed();
		static std::map<std::string, Container_indexed*> instances; //!< Mapping of instances of this oject.
		void SetName(const std::string &name);
		void SetName(const std::string &name, const std::string &desc);
	protected:
		template<class TYPE> bool KeyExists(const std::map<std::string, TYPE*> mapping, const std::string &key) const;
		template<class TYPE> void Insert(const std::map<std::string, TYPE*> mapping, const std::string &key, TYPE* value);
	private:
		void Insert(const std::string &key);
		bool KeyExists(const std::string &key) const;
	};


/// @}
#endif