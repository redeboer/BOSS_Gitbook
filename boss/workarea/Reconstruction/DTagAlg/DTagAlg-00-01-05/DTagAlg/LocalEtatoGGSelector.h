#ifndef LOCAL_ETATOGG_SELECTOR_H
#define LOCAL_ETATOGG_SELECTOR_H

#include "BesDChain/CDEta.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalEtatoGGSelector : public DCSelectionFunction<CDEta>
{
   public :

      LocalEtatoGGSelector();

      bool operator() (CDEta& aEta);


   private :

      LocalEtatoGGSelector( const LocalEtatoGGSelector& );
      const LocalEtatoGGSelector& operator= ( const LocalEtatoGGSelector& );

      double m_minMass;
      double m_maxMass; 
      double m_maxChisq;

};

extern LocalEtatoGGSelector etatoGGSelector;

#endif
