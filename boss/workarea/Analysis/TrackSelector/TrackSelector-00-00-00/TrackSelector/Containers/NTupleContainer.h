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

	/// Container object that is to facilitate the booking procedure for `NTuple::Tuple`s and their `NTuple::Item`s.
	/// See <a href="https://besiii.gitbook.io/boss/the-boss-afterburner/initial/motivation#problems-with-booking-procedures">this page</a> for the motivation. This function also automatically declares a corresponding `JobSwitch` property.
	/// @remark This object can only be used within a derived class off the Gaudi `Algorithm` class because its `fTuple` requires to be booked by it (using e.g. `TrackSelector::BookNTuples`).
	/// @todo Upgrade using templates. Currently only accepts `double`s!
	/// @todo Think about a solution for indexed items.
	/// @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date   February 19th, 2019
	class NTupleContainer : public Container_base
	{
	public:
		/// @name Constructors and destructors
			///@{
			NTupleContainer(const std::string &name, const std::string &description="", const bool write=true);
			~NTupleContainer();
			///@}


		/// @name Instance handlers
			///@{
			static std::map<std::string, NTupleContainer*> instances;
			static NTupleContainer& Get(const std::string &tuple_name) { return *instances.at(tuple_name); }
			static void PrintTuplesAndItems();
			static void PrintFilledTuples();
			///@}


		/// @name Setters
			///@{
			void SetTuplePtr(NTuplePtr& data) { fTuple = data.ptr(); }
			///@}


		/// @name Getters
			///@{
			NTuple::Item<double>& operator[] (const std::string &key) { return fItems[key]; }
			NTuple::Item<double>& at(const std::string &key) { return fItems.at(key); }
			const std::map<std::string, NTuple::Item<double> >* GetItems() { return &fItems; }
			const ULong_t GetEntries() const { return fEntries; } /// Read access to `fEntries`.
			///@}


		/// @name NTuple handlers and members
			///@{
			void AddItem(const std::string &item_name);
			void Write();
			///@}


		/// @name JobSwitch handlers
			///@{
			const bool DoWrite() const { return (bool)fWrite; } /// Return `true` if `fTuple` pointer exists and if `fWrite` property has been set to `true`.
			void SetWriteSwitch(const bool val=true) { fWrite.SetValue(val); }
			///@}


	protected:
		/// @name Data members
			///@{
			JobSwitch fWrite;   ///< Boolean job property that determines whether or not to write data stored to this `NTuple` to a `TTree` (property name starts with `"write_"` by default).
			NTuple::Tuple *fTuple; ///< Pointer to the encapsulated `NTuple::Tuple`.
			ULong_t fEntries;
			std::map<std::string, NTuple::Item<double> > fItems; ///< Inventory of added items.
			///@}
	};


/// @}
#endif