/* * ------- CLASS DESCRIPTION ------- *
! Modify these according to your ROOT file
	AUTHOR: Remco de Boer 雷穆克
	EMAIL:
		r.e.deboer@students.uu.nl
		remco.de.boer@ihep.ac.cn
	DATE: October 19th, 2018
	STATUS: Developmental
	DESCRIPTION:
		The RhopiRootFile is derived from the general BOSSRootFile class. It can be used to easily load a ROOT file generated in the jopOptions_ana (analysis) step of the BOSS framework. As opposed to the more BOSSRootFile class, the structure of this class needs to match that of the analysis you are working with.
*/

#ifndef Physics_Analysis_RhopiRootFile_H
#define Physics_Analysis_RhopiRootFile_H

// * ------- LIBRARIES ------- *
	#include "BOSSRootFile.h"
	#include "TTree.h"

// * ------- CLASS DEFINITION ------- *
class RhopiRootFile : public BOSSRootFile
{

public:
	// * Constructor and destructors *
	RhopiRootFile() : BOSSRootFile() { Initialize(); }
	RhopiRootFile(const char*);
	~RhopiRootFile();

	// * Getters *
	TTree* vxyz()   { return fVxyz; }
	TTree* photon() { return fPhoton; }
	TTree* dedx()   { return fDedx; }
	TTree* tof1()   { return fTof1; }
	TTree* tof2()   { return fTof2; }
	TTree* pid()    { return fPid; }
	TTree* etot()   { return fEtot; }
	TTree* fit4c()  { return fFit4c; }
	TTree* fit5c()  { return fFit5c; }
	TTree* geff()   { return fGeff; }
	TTree* tofe()   { return fTofe; }


private:
	// * Data members *
	TTree* fVxyz;
	TTree* fPhoton;
	TTree* fDedx;
	TTree* fTof1;
	TTree* fTof2;
	TTree* fPid;
	TTree* fEtot;
	TTree* fFit4c;
	TTree* fFit5c;
	TTree* fGeff;
	TTree* fTofe;

	// * Private methods *
	void Initialize();
	void Destruct();
};


// * ------- CONSRUCTORS AND DESTRUCTORS ------- * //
	RhopiRootFile::RhopiRootFile(const char* filename) : BOSSRootFile(filename)
	{
		if(!(fVxyz   = GetTree("vxyz")))   Destruct();
		if(!(fPhoton = GetTree("photon"))) Destruct();
		if(!(fDedx   = GetTree("dedx")))   Destruct();
		if(!(fTof1   = GetTree("tof1")))   Destruct();
		if(!(fTof2   = GetTree("tof2")))   Destruct();
		if(!(fPid    = GetTree("pid")))    Destruct();
		if(!(fEtot   = GetTree("etot")))   Destruct();
		if(!(fFit4c  = GetTree("fit4c")))  Destruct();
		if(!(fFit5c  = GetTree("fit5c")))  Destruct();
		if(!(fGeff   = GetTree("geff")))   Destruct();
		if(!(fTofe   = GetTree("tofe")))   Destruct();
	}
	RhopiRootFile::~RhopiRootFile()
	{
		Destruct();
	}

// * ------- GETTERS ------- * //


// * ------- PRIVATE METHODS ------- * //
	void RhopiRootFile::Initialize()
	{
		BOSSRootFile::Initialize();
		fVxyz   = NULL;
		fPhoton = NULL;
		fDedx   = NULL;
		fTof1   = NULL;
		fTof2   = NULL;
		fPid    = NULL;
		fEtot   = NULL;
		fFit4c  = NULL;
		fFit5c  = NULL;
		fGeff   = NULL;
		fTofe   = NULL;
	}
	void RhopiRootFile::Destruct()
	{
		// BOSSRootFile::Destruct();
		Initialize();
	}


#endif