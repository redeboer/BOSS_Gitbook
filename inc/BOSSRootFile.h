/* * ------- CLASS DESCRIPTION ------- *
	AUTHOR: Remco de Boer 雷穆克
	EMAIL:
		r.e.deboer@students.uu.nl
		remco.de.boer@ihep.ac.cn
	DATE: October 19th, 2018
	STATUS: Developmental
	DESCRIPTION:
		The BOSSRootFile class describes a ROOT file generated in the jopOptions_ana (analysis) step of the BOSS framework in the most general way, that is, it only loads a TFile and creates a list of pointers to all TTrees contained within the main directory of this TFile.
		You can create a derived class which maps the list of TTrees to a structure with particular names that match the file with which you are working.
*/

#ifndef Physics_Analysis_BOSSRootFile_H
#define Physics_Analysis_BOSSRootFile_H

// * ------- LIBRARIES ------- *
	#include <list>
	#include "TFile.h"
	#include "TKey.h"
	#include "TList.h"
	#include "TObject.h"
	#include "TString.h"
	#include "TTree.h"


// * ------- CLASS DEFINITION ------- *
class BOSSRootFile
{

public:
	// * Constructor and destructors *
	BOSSRootFile();
	BOSSRootFile(const char*);
	~BOSSRootFile();
	void CloseFile();

	// * Information *
	void Print();

	// * Getters *
	std::list<TTree*>& GetListOfTTrees();
	TTree* GetTree(const char*);

protected:
	// * Data members *
	TFile*            fFile;
	std::list<TTree*> fTTrees;

	// * Private methods *
	bool OpenFile(const char*);
	void Initialize();
	void Destruct();
	void LoadTTrees();

};


// * ------- CONSRUCTORS AND DESTRUCTORS ------- * //
	BOSSRootFile::BOSSRootFile() : fFile(NULL)
	{
		Initialize();
	}
	BOSSRootFile::BOSSRootFile(const char* filename) : fFile(NULL)
	{
		// * Initialize members (empty them) *
		Initialize();
		// * Open file *
		OpenFile(filename);
		// * Load TTrees *
		LoadTTrees();
	}
	BOSSRootFile::~BOSSRootFile()
	{
		Destruct();
	}
	void BOSSRootFile::CloseFile()
	{
		if(fFile) fFile->Close();
		fFile = NULL;
		fTTrees.clear();
	}

// * ------- GETTERS ------- * //
	std::list<TTree*>& BOSSRootFile::GetListOfTTrees()
	{
		return fTTrees;
	}
	TTree* BOSSRootFile::GetTree(const char* treeName)
	{
		TTree* tree = NULL;
		// * Loop over list of TTrees *
		std::list<TTree*>::iterator it = fTTrees.begin();
		while(it != fTTrees.end()) {
			TString name = (*it)->GetName();
			if(name.EqualTo(treeName)) {
				tree = *it;
				break;
			}
			++it;
		}
		// * Pass TTree pointer if the name matches
		if(!tree) {
			std::cout << "ERROR: TTree \"" << treeName << "\" does not exist in file \"" << fFile->GetName() << "\"" << std::endl;
		}
		return tree;
	}


// * ------- INFORMATION ------- * //
	void BOSSRootFile::Print()
	{
		// * Check if class has been constructed *
		if(!fFile) {
			std::cout << std::endl << "File not loaded" << std::endl;
			return;
		}
		// * If so, print all 
		std::cout << std::endl << "File name: \"" << fFile->GetName() << "\"" << std::endl;
		if(fTTrees.size()) {
			std::cout << "  contains " << fTTrees.size() << " TTrees:" << std::endl;
			std::list<TTree*>::iterator it = fTTrees.begin();
			while(it != fTTrees.end()) {
				std::cout << "    \"" << (*it)->GetName() << "\"\t\"" << (*it)->GetTitle() << "\"" << std::endl;
				++it;
			}
		} else {
			std::cout << "  does not contain any TTrees" << std::endl;
		}
		std::cout << "  Output of fFile->ls():" << std::endl;
		std::cout << "  ----------------------" << std::endl;
		fFile->ls();
		std::cout << std::endl;
	}


// * ------- PRIVATE METHODS ------- * //
	void BOSSRootFile::Initialize()
	{
		fFile = NULL;
		fTTrees.clear();
	}
	bool BOSSRootFile::OpenFile(const char* filename)
	{
		// * Close file if open *
		CloseFile();
		// * Attempt to open file *
		fFile = new TFile(filename);
		if(!fFile) {
			std::cout << "ERROR: File \"" << filename << "\" does not exist" << std::endl;
			return false;
		}
		if(fFile->IsZombie()) {
			std::cout << "ERROR: File \"" << filename << "\" is zombie" << std::endl;
			return false;
		}
		return true; // if successful
	}
	void BOSSRootFile::Destruct()
	{
		CloseFile();
		Initialize();
	}
	void BOSSRootFile::LoadTTrees()
	{
		// * Only perform if file has been loaded *
		if(!fFile) return;

		// * Go through keys in the file and add to list of TTrees *
		TIter next(fFile->GetListOfKeys());
		TObject *obj  = NULL;
		TKey    *key  = NULL;
		TTree   *tree = NULL;
		while((obj = next())) {
			if((key = dynamic_cast<TKey*>(obj))) {
				if((tree = dynamic_cast<TTree*>(key->ReadObj()))) {
					// * Add to list of TTrees
					fTTrees.push_back(tree);
				}
			}
		}

		// * Error output *
		std::cout << "Loaded " << fTTrees.size() << " TTrees from file \"" << fFile->GetName() << "\"" << std::endl;
	}

#endif