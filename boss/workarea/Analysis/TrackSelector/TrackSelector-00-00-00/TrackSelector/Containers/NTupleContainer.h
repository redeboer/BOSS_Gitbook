#ifndef Analysis_NTupleContainer_H
#define Analysis_NTupleContainer_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "GaudiKernel/NTuple.h"
	#include "TrackSelector/Containers/Container_base.h"
	#include "TrackSelector/Containers/JobSwitch.h"
	#include <map>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_objects
/// @{

	/**
	 * @brief Container object that is to facilitate the booking procedure for `NTuple::Tuple`s and their `NTuple::Item`s. See <a href="https://besiii.gitbook.io/boss/the-boss-afterburner/initial/motivation#problems-with-booking-procedures">this page</a> for the motivation. This function also automatically declares a corresponding `JobSwitch` property.
	 *
	 * @todo Upgrade using templates. Currently only accepts `double`s!
	 * @todo Think about a solution for indexed items.
	 * 
	 * @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date   February 19th, 2019
	 */
	class NTupleContainer : public Container_base
	{
	public:
		// * Constructors and destructors * //
			NTupleContainer(const std::string &name, const std::string &description="");
			~NTupleContainer();

		// * Instance handlers * //
			static std::map<std::string, NTupleContainer*> instances;
			static NTupleContainer& Get(const std::string &tuple_name) { return *instances.at(tuple_name); }

		// * Getters * //
			NTuple::Item<double>& operator[] (const std::string &key) { return items[key]; }
			NTuple::Item<double>& at(const std::string &key) { return items.at(key); }

		// * NTuple handlers and members * //
			void AddItem(const std::string &item_name);
			void Write() const;
			NTuple::Tuple *tuple;

		// * JobSwitch handlers * //
			explicit operator bool() const { return (bool)perform; }
			bool operator! () const { return !perform; }
			JobSwitch perform; //!< Boolean job property that can be used as a switch for performing a loop to fill this `NTuple` (property name starts with `"do"` by default).
			JobSwitch write; //!< Boolean job property that determines whether or not to write data stored to this `NTuple` to a `TTree` (property name starts with `"write_"` by default).

	private:
		std::map<std::string, NTuple::Item<double> > items;
		void SetSwitch(JobSwitch &obj, const std::string &prepend);
	};


/// @}
#endif