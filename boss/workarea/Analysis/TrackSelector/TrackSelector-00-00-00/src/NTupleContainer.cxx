// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/NTupleContainer.h"
	#include "TString.h" // for Form
	#include <iostream>
	#include <utility> // for make_pair



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/**
	 * @brief The constructor attempts to add the `NTuple::Tuple` to the `instances` mapping.
	 */
	NTupleContainer::NTupleContainer(const std::string &name, const std::string &description) :
		Container_base(name, description), perform(name), write(name)
	{
		/// <ol>
		/// <li> Abort if this `NTuple` key name already exists in the `instances` mapping.
			if(instances.find(name) != instances.end()) {
				std::cout << "FATAL ERROR: NTuple \"" << name << "\" already exists!" << std::endl;
				std::terminate(); /// @warning Contains fatal `terminate` statement when double booking! Consider turning into a `return` statement.
			}
		/// <li> Insert a key to the `instances` mapping.
			instances.insert(make_pair(name, this));
		/// <li> Set name for corresponding "do" `JobSwitch`.
			std::string new_name(name);
			new_name.insert(0, "do_");
			perform.SetName(new_name, description);
		/// <li> Set name for corresponding "write" `JobSwitch`.
			std::string new_name(name);
			new_name.insert(0, "write_");
			write.SetName(new_name, description);
		/// </ol>
	}

	/**
	 * @brief The destructor removes this instances from the `instances` inventory.
	 */
	NTupleContainer::~NTupleContainer()
	{
		instances.erase(Name());
	}

	std::map<std::string, NTupleContainer*> NTupleContainer::instances;



// * =============================== * //
// * ------- NTUPLE HANDLERS ------- * //
// * =============================== * //

	void NTupleContainer::Write() const
	{
		if(!fTuple || !write) return;
		fTuple->write();
	}



// * ================================== * //
// * ------- BOOKING PROCEDURES ------- * //
// * ================================== * //

	/**
	 * @brief Easier and expanded version of `NTuple::Tuple::addItem`.
	 */
	void NTupleContainer::AddItem(const std::string &item_name)
	{
		/// <ol>
		/// <li> <b>Abort</b> if `fTuple` has not been booked.
		if(!fTuple) {
			std::cout << "WARNING: NTuple \"" << Name() << "\" has not been booked, so cannot add item \"" << item_name << "\"" << std::endl;
			return;
		}
		/// <li> <b>Abort</b> if the `perform` job switch property has been set to `false`.
		if(!perform) return;
		/// <li> Create an `NTuple::Item` using the `items` mapping.
		items[item_name];
		/// <li> Create an `NTuple::Item` using the `items` mapping to the `fTuple` and to the map of `items`, if allowed by the `perform` job switch.
		fTuple->addItem(item_name, items.at(item_name));
		/// </ol>
	}