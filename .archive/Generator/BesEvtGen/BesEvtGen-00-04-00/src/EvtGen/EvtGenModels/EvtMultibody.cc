#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtResonance.hh"
#include "EvtGenBase/EvtResonance2.hh"
#include "EvtGenModels/EvtMultibody.hh"
#include "EvtGenBase/EvtConst.hh"
#include "EvtGenBase/EvtdFunction.hh"
#include "EvtGenBase/EvtKine.hh"

EvtMultibody::~EvtMultibody() 
{
    if( _decayTree != NULL ) delete _decayTree;
    _decayTree=NULL;
}

void EvtMultibody::getName(std::string& model_name)
{
    model_name = "D_MULTIBODY";
}

EvtDecayBase* EvtMultibody::clone()
{
    return new EvtMultibody;
}

void EvtMultibody::init()
{
    int N = getNArg();
 
    _decayTree = new EvtMTree( getDaugs(), getNDaug() );
 
    for(int i=0; i<N-1; ++i) {
        if(getArgStr( i )=="RESONANCE") {
            _decayTree->addtree( getArgStr( ++i ) );
        } else {
            report(ERROR,"EvtGen")
                << "Syntax error at " << getArgStr( i ) << std::endl;
            ::abort();
        }
    }
}

// Set the maximum probability amplitude - if function is left blank then the
// program will search for it.  This however is not deterministic and therefore
// in the release cannot be in place.
void EvtMultibody::initProbMax()
{
    // setProbMax(1.0);
}

void EvtMultibody::decay( EvtParticle *p )
{
    // Initialize the phase space before doing anything else!
    p->initializePhaseSpace(getNDaug(),getDaugs());
    std::vector<EvtVector4R> product;

    for(int i=0; i<getNDaug(); ++i) 
        product.push_back(p->getDaug(i)->getP4Lab());

    EvtSpinAmp amp = _decayTree->amplitude( product );

    int * ilist = new int[amp.rank()];
    
    // Set up the rotation matrix for the root particle
    EvtSpinDensity R = p->rotateToHelicityBasis();

    EvtSpinType::spintype type = EvtPDL::getSpinType(getParentId());
    int twospin = EvtSpinType::getSpin2( EvtPDL::getSpinType(getParentId()) );
    
    std::vector<EvtSpinType::spintype> types(2, type);
    EvtSpinAmp newamp( types, EvtComplex(0.0, 0.0) );
    std::vector<int> index = newamp.iterallowedinit();
    do {
        newamp(index) = R.Get((index[0]+twospin)/2,(index[1]+twospin)/2);
    } while( newamp.iterateallowed( index ) );

    newamp.extcont(amp, 1, 0);
    amp=newamp;

    index = amp.iterallowedinit();
    std::vector<int> spins = amp.dims();
    
    do {
        for( int i=0; i<index.size(); ++i ) {
            ilist[i]=index[i]+spins[i];
        }
        
        vertex( ilist, amp( index ) );
    } while( amp.iterateallowed( index ) );

    delete [] ilist;
}
