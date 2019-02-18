#ifndef DTagAlg_ReadBeamInfFromDb_H
#define DTagAlg_ReadBeamInfFromDb_H

#include "CLHEP/Vector/ThreeVector.h"
#include "DatabaseSvc/IDatabaseSvc.h"
#include <iostream>
#include <mysql.h>


class ReadBeamInfFromDb {

 public:
  
  ReadBeamInfFromDb();
  ~ReadBeamInfFromDb() {}
  
  bool isRunValid(int run);
  double getbeamE(int run, double defaultbeamE);
  inline  CLHEP::Hep3Vector getbeta(){return m_beta;}
  inline bool setcalib(bool calib){m_usecbE=calib;}

 private:
  MYSQL* OpenDb() const;
  double ReadDb(int run);
  void CloseDb(MYSQL* mysql) const;
  
  IDatabaseSvc* m_dbsvc;
  int m_run;
  bool m_isRunValid;
  double m_beamE;
  bool m_usecbE;
  CLHEP::Hep3Vector m_beta;
};


#endif
