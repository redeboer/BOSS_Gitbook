// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/NTupleContainer.h"
	#include "TString.h"
	#include <cmath>
	#include <iomanip>
	#include <iostream>
	#include <utility> // for make_pair



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //


	/// The constructor attempts to add the `NTuple::Tuple` to the `instances` mapping.
	NTupleContainer::NTupleContainer(const std::string &name, const std::string &description, const bool write) :
		Container_base(name, description),
		fEntries(0),
		fTuple(nullptr),
		fWrite(Form("write_%s", name.c_str()), description, write)
	{
		/// -# Abort if this `NTuple` key name already exists in the `instances` mapping.
			if(instances.find(name) != instances.end()) {
				std::cout << "FATAL ERROR: NTuple \"" << name << "\" already exists!" << std::endl;
				std::terminate(); /// @warning Contains fatal `terminate` statement when double booking! Consider turning into a `return` statement.
			}
		/// -# Insert a key to the `instances` mapping.
			instances.insert(make_pair(name, this));
	}


	/// The destructor removes this instances from the `instances` inventory.
	NTupleContainer::~NTupleContainer()
	{
		instances.erase(Name());
	}

	std::map<std::string, NTupleContainer*> NTupleContainer::instances;



// * =============================== * //
// * ------- NTUPLE HANDLERS ------- * //
// * =============================== * //


	/// Write `NTuple` if `"write_"` `JobSwitch` has been set to `true`.
	void NTupleContainer::Write()
	{
		if(!fTuple) return;
		if(!DoWrite()) return;
		++fEntries;
		fTuple->write();
	}


	/// Print table of `NTuple`s existing in `instances` with their entries.
	void NTupleContainer::PrintFilledTuples()
	{
		/// -# @b Abort if no `NTuple`s exist in `instances`.
			if(!instances.size()) return;

		/// -# Get name widths and `fEntries` widths.
			std::string h_name("TUPLE NAME"), h_entries("nENTRIES");
			size_t w_namemax(h_name.size()), w_valmax(h_entries.size()), nentries(0);
			std::map<std::string, NTupleContainer*>::iterator it;
			for(it = instances.begin(); it != instances.end(); ++it) {
				if(!it->second->GetEntries()) continue;
				++nentries;
				size_t w_val(ceil(log10(it->second->GetEntries())));
				if(it->first.size() > w_namemax) w_namemax = it->first.size();
				if(w_val > w_valmax) w_valmax = w_val;
			}
			++w_namemax;

		/// -# @b Abort if there are no entries that have been filled.
			if(!nentries) return;

		/// -# Print table header
			std::cout << std::endl;
			std::cout << std::setw(w_namemax) << h_name << " | ";
			std::cout << std::setw(w_valmax) << h_entries << std::endl;

		/// -# Print a row for each `NTupleContainer` in `instances` with name and number of entries
			for(it = instances.begin(); it != instances.end(); ++it) {
				if(!it->second->GetEntries()) continue;
				std::cout << std::setw(w_namemax) << it->first << " | ";
				std::cout << std::setw(w_valmax) << it->second->GetEntries();
				std::cout << std::endl;
			}
			std::cout << std::endl;
	}


	/// Print list of `NTuple`s existing in `instances` with their items.
	void NTupleContainer::PrintTuplesAndItems()
	{
		/// <ol>
		/// <li> @b Abort if no `NTuple`s exist in `instances`.
			if(!instances.size()) return;
		/// <li> Print header line.
			std::cout << std::endl << std::endl << "THERE ARE " << instances.size() << " BOOKED NTUPLES" << std::endl;
		/// <li> Print a row for each `NTupleContainer` in `instances`.
			std::map<std::string, NTupleContainer*>::iterator it;
			for(it = instances.begin(); it != instances.end(); ++it) {
			/// <ol>
			/// <li> Print name and description.
				std::cout << std::endl << "NTuple \"" << it->first << "\" (write = ";
			/// <li> Print write `JobSwitch` and number of entries if available.
				std::cout << it->second->DoWrite();
				if(it->second->GetEntries()) std::cout << " | entries = " << it->second->GetEntries();
				std::cout << "):" << std::endl;
			/// <li> Print name and description.
				std::cout << it->second->Description() << std::endl;
			/// <li> Print list of added items.
				int count(0);
				std::map<std::string, NTuple::Item<double> >::const_iterator item = it->second->GetItems()->begin();
				for(item; item != it->second->GetItems()->end(); ++item) {
					std::cout << "  " << count << ". " << item->first << std::endl;
					++count;
				}
			/// </ol>
			}
		/// </ol>
	}



// * ================================== * //
// * ------- BOOKING PROCEDURES ------- * //
// * ================================== * //


	/// Easier and expanded version of `NTuple::Tuple::addItem`.
	void NTupleContainer::AddItem(const std::string &item_name)
	{
		/// -# @b Abort if `fTuple` has not been booked.
			if(!fTuple) {
				std::cout << "WARNING: NTuple \"" << Name() << "\" has not been booked, so cannot add item \"" << item_name << "\"" << std::endl;
				return;
			}
		/// -# @b Abort if the `"write_"` job switch property has been set to `false`.
			if(!DoWrite()) return;
		/// -# Create an `NTuple::Item` using the `items` mapping.
			fItems[item_name];
		/// -# Create an `NTuple::Item` using the `items` mapping to the `fTuple` and to the map of `items`, if allowed by the `perform` job switch.
			fTuple->addItem(item_name, fItems.at(item_name));
	}