// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/Container_indexed.h"



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/// The constructor, in addition to setting the name and description, attempts to add `Insert` an instance of itself to the `fInstances` mapping.
	Container_indexed::Container_indexed(const std::string &name, const std::string &description) :
		Container_base(name, description)
	{
		Insert(name);
	}

	/// The destructor removes this fInstances from the `fInstances` mapping.
	Container_indexed::~Container_indexed()
	{
		fInstances.erase(Name());
	}

	std::map<std::string, Container_indexed*> Container_indexed::fInstances;



// * ======================= * //
// * ------- SETTERS ------- * //
// * ======================= * //

	/// A overload of `Container_base::SetName` that in addition to setting the name, safely redefines its corresponding key in `fInstances`.
	void Container_indexed::SetName(const std::string &name)
	{
		Insert(name);
		Container_base::SetName(name);
	}

	/// Extension of `SetName` where you can also set the description.
	void Container_indexed::SetName(const std::string &name, const std::string &desc)
	{
		Container_indexed::SetName(name);
		Container_base::SetDescription(name);
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //

	/// Helper method that allows you to to check whether a key already exists in `fInstances` mapping.
	bool Container_indexed::KeyExists(const std::string &key) const
	{
		return KeyExists(fInstances, key);
	}

	/// Helper method that allows you to safely (using `KeyExists`) insert a `key` with corresponding `value` into the `fInstances` mapping.
	void Container_indexed::Insert(const std::string &key)
	{
		Insert(fInstances, key, this);
	}