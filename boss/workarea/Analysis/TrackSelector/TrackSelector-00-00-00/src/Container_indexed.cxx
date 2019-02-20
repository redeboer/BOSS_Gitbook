// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Container_indexed.h"
	#include <iostream>
	#include <utility> // for make_pair



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/**
	 * @brief The constructor, in addition to setting the name and description, attempts to add `Insert` an instance of itself to the `instances` mapping.
	 */
	Container_indexed::Container_indexed(const std::string &name, const std::string &description) :
		Container_base(name, description)
	{
		Insert(name);
	}

	/**
	 * @brief The destructor removes this instances from the `instances` mapping.
	 */
	Container_indexed::~Container_indexed()
	{
		instances.erase(Name());
	}

	std::map<std::string, Container_indexed*> Container_indexed::instances;



// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //

	/**
	 * @brief A overload of `Container_base::SetName` that in addition to setting the name, safely redefines its corresponding key in `instances`.
	 */
	void Container_indexed::SetName(const std::string &name)
	{
		Insert(name);
		Container_base::SetName(name);
	}

	/**
	 * @brief Extension of `SetName` where you can also set the description.
	 */
	void Container_indexed::SetName(const std::string &name, const std::string &desc)
	{
		Container_indexed::SetName(name);
		Container_base::SetDescription(name);
	}



// * ================================= * //
// * ------- PROTECTED METHODS ------- * //
// * ================================= * //

	/**
	 * @brief Helper method that allows you to to check whether a key already exists in . This method uses a `template` and has been made `protected` as to share its functionality for derived classes.
	 * @todo Use error handling for existing keys, not `terminate`.
	 * @return Currently redudant, because of the `terminate`, but returns `false` at the end.
	 */
	template<class TYPE>
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
	template<class TYPE>
	void Container_indexed::Insert(const std::map<std::string, TYPE*> mapping, const std::string &key, TYPE *value)
	{
		if(KeyExists(mapping, key)) return;
		mapping.insert(make_pair(key, value));
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //

	/**
	 * @brief Helper method that allows you to to check whether a key already exists in `instances` mapping.
	 */
	bool Container_indexed::KeyExists(const std::string &key) const
	{
		return KeyExists(instances, key);
	}

	/**
	 * @brief Helper method that allows you to safely (using `KeyExists`) insert a `key` with corresponding `value` into the `instances` mapping.
	 */
	void Container_indexed::Insert(const std::string &key)
	{
		Insert(instances, key, this);
	}