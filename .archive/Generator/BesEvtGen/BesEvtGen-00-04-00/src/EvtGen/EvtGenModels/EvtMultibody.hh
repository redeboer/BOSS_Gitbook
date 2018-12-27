#ifndef EVTMULTIBODY_HH
#define EVTMULTIBODY_HH

#include "EvtGenBase/EvtMTree.hh"
#include "EvtGenBase/EvtDecayAmp.hh"
#include "EvtGenBase/EvtSpinAmp.hh"

class EvtParticle;

class EvtMultibody:public EvtDecayAmp
{
    public:
        EvtMultibody() { _decayTree = NULL; }
        virtual ~EvtMultibody();

        void getName(std::string& name);
        EvtDecayBase* clone();

        void init();
        void initProbMax();

        void decay(EvtParticle *p); 

    private:
        EvtMTree * _decayTree;
};

#endif
