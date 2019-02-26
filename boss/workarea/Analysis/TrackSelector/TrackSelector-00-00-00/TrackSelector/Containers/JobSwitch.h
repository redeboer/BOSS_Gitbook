#ifndef Analysis_JobSwitch_H
#define Analysis_JobSwitch_H


// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "TrackSelector/Containers/Container_base.h"
	#include "TString.h"
	#include <list>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_objects
/// @{

	/// This simple object encapsulates a `bool`ean job option.
	/// Options like these are usually used as switches in your algorithm that can be set to `true` or `false` in the job options. The constructor only needs a name for this switch. It is convention to start this name with e.g.:
	/// * `"write_"` if this determines whether or not to write a certain `TTree`/`NTuple`.
	/// @author Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	/// @date   January 3rd, 2019
	class JobSwitch : public Container_base
	{
	public:
		JobSwitch(const std::string &name, const std::string &description="", const bool val=true);
		~JobSwitch();
		static std::list<JobSwitch*> gJobSwitches;
		static void PrintSwitches();
		explicit operator bool() const { return fValue; }   /// Convertor for `bool`ean arithmatic.
		const bool Value() const { return fValue; }         /// Read access to `fValue` data member.
		void SetValue(const bool value) { fValue = value; } /// Write access to `fValue` data member.
	// private:
		bool fValue; /// The actual switch. Not set to `private` because of `Algorithm::declareProperty` needs it in `TrackSelector`.
	};


/// @}
#endif