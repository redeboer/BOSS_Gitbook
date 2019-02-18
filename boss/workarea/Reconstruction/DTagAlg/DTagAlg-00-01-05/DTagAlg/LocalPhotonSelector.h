#ifndef LOCAL_PHOTON_SELECTOR_H
#define LOCAL_PHOTON_SELECTOR_H

#include "BesDChain/CDPhoton.h"
#include "BesDChain/BesDCSelector.h"

class LocalPhotonSelector : public BesDCSelector<CDPhoton>
{
   public :

      LocalPhotonSelector();

      bool operator() (CDPhoton& aPhoton);


   private :

      LocalPhotonSelector( const LocalPhotonSelector& );
      const LocalPhotonSelector& operator= ( const LocalPhotonSelector& );

      double m_minEnergy;
 
      bool   m_useBarrelEndcap;
      double m_maxCosThetaBarrel;
      double m_minCosThetaEndcap;
      double m_maxCosThetaEndcap;
      double m_minEndcapEnergy;
 
      bool   m_applyTimeCut;
      double m_minTime;
      double m_maxTime;
      double m_deltaTime;
 
      bool m_applyDangCut;
      double m_minDang;
};

extern LocalPhotonSelector photonSelector;

#endif
