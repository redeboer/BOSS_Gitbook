#ifndef Physics_Analysis_SimplifiedTree_H
#define Physics_Analysis_SimplifiedTree_H

/**
 * @brief    A container class for a `TTree` that allows easy access to its branches. Addresses are set automatically upon construction.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 21st, 2018
 */

// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "TList.h"
	#include "TObject.h"
	#include "TTree.h"
	#include <iostream>
	#include <unordered_map>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
	class SimplifiedTree
	{
	public:

		// * Constructors and destructors
		SimplifiedTree() : fTree(nullptr) {}
		SimplifiedTree(TTree* tree, bool print = true);

		// * Getters
		TTree* Get() { return fTree; }
		Char_t&    Get_B(const char* name) { return fMap_B.at(name); }
		UChar_t&   Get_b(const char* name) { return fMap_b.at(name); }
		Short_t&   Get_S(const char* name) { return fMap_S.at(name); }
		UShort_t&  Get_s(const char* name) { return fMap_s.at(name); }
		Int_t&     Get_I(const char* name) { return fMap_I.at(name); }
		UInt_t&    Get_i(const char* name) { return fMap_i.at(name); }
		Float_t&   Get_F(const char* name) { return fMap_F.at(name); }
		Double_t&  Get_D(const char* name) { return fMap_D.at(name); }
		Long64_t&  Get_L(const char* name) { return fMap_L.at(name); }
		ULong64_t& Get_l(const char* name) { return fMap_l.at(name); }
		Bool_t&    Get_O(const char* name) { return fMap_O.at(name); }

		// * Getters
		void Print();

	private:

		// * Data members
		TTree* fTree;
		std::unordered_map<std::string, Char_t>    fMap_B; //!< Map of addresses for Char_t (8 bit signed integer).
		std::unordered_map<std::string, UChar_t>   fMap_b; //!< Map of addresses for UChar_t (8 bit unsigned integer).
		std::unordered_map<std::string, Short_t>   fMap_S; //!< Map of addresses for Short_t (16 bit signed integer).
		std::unordered_map<std::string, UShort_t>  fMap_s; //!< Map of addresses for UShort_t (16 bit unsigned integer).
		std::unordered_map<std::string, Int_t>     fMap_I; //!< Map of addresses for Int_t (32 bit signed integer).
		std::unordered_map<std::string, UInt_t>    fMap_i; //!< Map of addresses for UInt_t (32 bit unsigned integer).
		std::unordered_map<std::string, Float_t>   fMap_F; //!< Map of addresses for Float_t (32 bit floating point).
		std::unordered_map<std::string, Double_t>  fMap_D; //!< Map of addresses for Double_t (64 bit floating point).
		std::unordered_map<std::string, Long64_t>  fMap_L; //!< Map of addresses for Long64_t (64 bit signed integer).
		std::unordered_map<std::string, ULong64_t> fMap_l; //!< Map of addresses for ULong64_t (64 bit unsigned integer).
		std::unordered_map<std::string, Bool_t>    fMap_O; //!< Map of addresses for Bool_t (boolean).

		// * Private methods
		template<typename TYPE> void SetAddress(TObject* obj, std::unordered_map<std::string, TYPE> &map);
		template<typename TYPE> void SetAddressSafe(TObject* obj, std::unordered_map<std::string, TYPE> &map);

	};



// * =========================================== * //
// * ------- CONSTRUCTORS AND DESTRUCTORS ------ * //
// * =========================================== * //

	/**
	 * @brief Automatically book addresses for all branches of a `TTree`. The branches are accessible by name (use `Get_<type>`, though you have to the `type` to use this).
	 * 
	 * @param tree The `TTree` that you want to load.
	 * @param print Whether or not to print the `TTree` names and its branches+types. Give `true` to the `BOSSRootFile` constructor for debugging purposes in particular.
	 */
	SimplifiedTree::SimplifiedTree(TTree* tree, bool print) : fTree(tree)
	{
		if(!fTree) return;
		TIter next(fTree->GetListOfBranches());
		TObject *obj  = nullptr;
		if(print) std::cout << "  Tree \"" << tree->GetName() << "\" has branches:" << std::endl;
		while((obj = next())) {
			std::string type(obj->GetTitle()); /// The data type of a branch can be determined from the last character of its title. See https://root.cern.ch/doc/master/classTTree.html for the labels of each type.
			if(print) std::cout << "    " << type << std::endl;
			switch(type.back()) {
				case 'B' : SetAddress(obj, fMap_B); break;
				case 'b' : SetAddress(obj, fMap_b); break;
				case 'S' : SetAddress(obj, fMap_S); break;
				case 's' : SetAddress(obj, fMap_s); break;
				case 'I' : SetAddress(obj, fMap_I); break;
				case 'i' : SetAddress(obj, fMap_i); break;
				case 'F' : SetAddress(obj, fMap_F); break;
				case 'D' : SetAddress(obj, fMap_D); break;
				case 'L' : SetAddress(obj, fMap_L); break;
				case 'l' : SetAddress(obj, fMap_l); break;
				case 'O' : SetAddress(obj, fMap_O); break;
				default :
					std::cout << "ERROR: Unable to book address for branch \"" << type << "\"" << std::endl;
			}
		}
	}



// * =========================================== * //
// * ------- CONSTRUCTORS AND DESTRUCTORS ------ * //
// * =========================================== * //
	/**
	 * @brief
	 */



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //

	/**
	 * @brief
	 */
	template<typename TYPE>
	void SimplifiedTree::SetAddress(TObject* obj, std::unordered_map<std::string, TYPE> &map)
	{
		fTree->SetBranchAddress(obj->GetName(), &map[obj->GetName()]);
	}

	/**
	 * @brief
	 */
	template<typename TYPE>
	void SimplifiedTree::SetAddressSafe(TObject* obj, std::unordered_map<std::string, TYPE> &map)
	{
		if(fTree) SetAddress(obj, map);
	}



#endif