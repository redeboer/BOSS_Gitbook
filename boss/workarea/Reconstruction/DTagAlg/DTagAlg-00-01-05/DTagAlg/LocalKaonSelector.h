#ifndef LOCAL_KAON_SELECTOR_H
#define LOCAL_KAON_SELECTOR_H

#include "BesDChain/CDChargedKaon.h"
#include "DecayChain/Function/DCSelectionFunction.h"

class LocalKaonSelector : public DCSelectionFunction<CDChargedKaon>
{
   public :

      LocalKaonSelector();


      bool operator() (CDChargedKaon& aKaon);
      void setpidtype(int type){m_pidtype=type;}      

   private :

      LocalKaonSelector( const LocalKaonSelector& );
      const LocalKaonSelector& operator= ( const LocalKaonSelector& );

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
      bool   m_rejectPion;
      bool   m_rejectProton;

      bool m_likelihood;
      bool m_neuronNetwork;
      std::vector<double> m_neuronValCut;

};

extern LocalKaonSelector kaonSelector;

#endif
