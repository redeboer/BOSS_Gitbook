#ifndef LOCAL_PION_SELECTOR_H
#define LOCAL_PION_SELECTOR_H

#include "BesDChain/CDChargedPion.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalPionSelector : public DCSelectionFunction<CDChargedPion>
{
   public :

      LocalPionSelector();

      bool operator() (CDChargedPion& aPion);
      void setpidtype(int type){m_pidtype=type;}  

   private :

      LocalPionSelector( const LocalPionSelector& );
      const LocalPionSelector& operator= ( const LocalPionSelector& );

      int m_pidtype;


      double m_VrCut;
      double m_VzCut;
      double m_CosThetaCut;

      bool   m_useSimplePID;
      bool   m_useDedx;
      bool   m_useTof1;
      bool   m_useTof2;
      bool   m_useTofE;
      bool   m_useTofQ;
      bool   m_useEmc;
      bool   m_useMuc;

      bool   m_probability;
      double m_PidProbCut;
      bool   m_rejectKaon;
      bool   m_rejectProton;

      bool m_likelihood;
      bool m_neuronNetwork;
      std::vector<double> m_neuronValCut;
};

extern LocalPionSelector pionSelector;

#endif
