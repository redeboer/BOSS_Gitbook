#ifndef Analysis_Container_indexed_H
#define Analysis_Container_indexed_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/Container_base.h"
	#include <iostream>
	#include <map>
	#include <string>
	#include <utility> // for make_pair



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_objects
/// @{

	/// This is an extension of the `Container_base` base class that in addition keeps track of all instanecs in a static mapping of all its instances.
	/// @todo Consider implementing <a href="https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern">Curiously Recurring Template Pattern</a> for independent objects of `fInstances` in derived classes.
	///
	/// @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date   February 20th, 2019
	class Container_indexed : public Container_base
	{
	public:
		Container_indexed(const std::string &name, const std::string &desccription="");
		~Container_indexed();
		void SetName(const std::string &name);
		void SetName(const std::string &name, const std::string &desc);
		static const std::map<std::string, Container_indexed*>& Instances() { return fInstances; } /// Read access to `fInstances` mapping. @todo `Instances` function necessary?
	protected:
		template<class TYPE> inline
		bool KeyExists(const std::map<std::string, TYPE*> mapping, const std::string &key) const;
		template<class TYPE> inline
		void Insert(std::map<std::string, TYPE*> &mapping, const std::string &key, TYPE* value);
	private:
		void Insert(const std::string &key);
		bool KeyExists(const std::string &key) const;
		static std::map<std::string, Container_indexed*> fInstances; //!< Mapping of instances of this oject.
	};


/// @}



// * ================================= * //
// * ------- PROTECTED METHODS ------- * //
// * ================================= * //

	/**
	 * @brief Helper method that allows you to to check whether a key already exists in . This method uses a `template` and has been made `protected` as to share its functionality for derived classes.
	 * @todo Use error handling for existing keys, not `terminate`.
	 * @return Currently redudant, because of the `terminate`, but returns `false` at the end.
	 */
	template<class TYPE> inline
	bool Container_indexed::KeyExists(const std::map<std::string, TYPE*> mapping, const std::string &key) const
	{
		if(mapping.find(key) != mapping.end()) {
			std::cout << "FATAL ERROR: key \"" << key << "\" already exists in mapping" << std::endl;
			std::terminate(); /// @warning Contains fatal `terminate` statement when double booking! Consider turning into a `return` statement.
		}
		return false;
	}

	/**
	 * @brief Helper method that allows you to safely (using `KeyExists`) insert a `key` with corresponding `value` into a `mapping`. This method uses a `template` and has been made `protected` as to share its functionality for derived classes.
	 */
	template<class TYPE> inline
	void Container_indexed::Insert(std::map<std::string, TYPE*> &mapping, const std::string &key, TYPE *value)
	{
		if(KeyExists(mapping, key)) return;
		mapping.insert(typename std::map<std::string, TYPE*>::value_type(key, value));
	}



#endif