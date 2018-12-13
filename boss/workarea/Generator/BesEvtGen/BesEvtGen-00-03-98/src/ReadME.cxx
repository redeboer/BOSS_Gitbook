//======= Lianjin WU <wulj@ihep.ac.cn> ======
#include "ReadME.h"
int ReadME::preRunNo=-1;
void ReadME::readMeasuredEcms(int runNo){
  m_sampleName=0; m_ecms=0; m_ecmsErr=0;
  m_aveEcms=0; m_aveEcmsErr=0; m_runFrom=0;
  m_runTo=0; m_runNoValid=false;
  m_px=0; m_py=0; m_pz=0;

  std::cout<<"MeasuredEcmsSvc begin read database MeasuredEcmsSvc"<<std::endl;
  int runNum=std::abs(runNo);
  if(runNo<0) std::cout<<"MeasuredEcmsSvc for MC events"<<std::endl;
  else std::cout<<"MeasuredEcmsSvc for DT events"<<std::endl;

  char stmt[400];
  snprintf(stmt, 1024, "select sample, RunFrom, RunTo, Ecms, Ecms_err, Ave_Ecms, Ave_Ecms_err from MeasuredEcms2 where RunFrom <= %d and RunTo >= %d", runNum, runNum);

  DatabaseRecordVector res;
  int rowNo=m_dbSvc->query("offlinedb", stmt, res);
  if(rowNo==0){ std::cout<<"RowNo can not be 0 for MeasuredEcmsSvc, failed to read DB"<<std::endl; }
  else{
    DatabaseRecord* records=res[0];
    std::cout<<"MeasuredEcmsSvc first read the runNo, read the MeasuredEcms database"<<runNo<<std::endl;
    m_runFrom=records->GetDouble("RunFrom");
    m_runTo=records->GetDouble("RunTo");
    m_sampleName=records->GetDouble("sample");
    m_ecms=records->GetDouble("Ecms");
   // m_ecmsErr=records->GetDouble("Ecms_err");
   // m_aveEcms=records->GetDouble("Ave_Ecms");
   // m_aveEcmsErr=records->GetDouble("Ave_Ecms_err");
    m_runNoValid=true;
  }
}

void ReadME::readDB(int runNo){
  RunParamsMin=11414;
  RunParamsMax=23454;
  MeasuredEcmsMin=23463;
  MeasuredEcmsMax=38140;
  DsRunParamsMin=43716;
  DsRunParamsMax=47066;
  runxyzMin=47543;
  runxyzMax=51498;
  if(std::abs(runNo)>=RunParamsMin&&std::abs(runNo)<=RunParamsMax) readRunParams(runNo);
  if(std::abs(runNo)>=MeasuredEcmsMin&&std::abs(runNo)<=MeasuredEcmsMax) readMeasuredEcms(runNo);
  if(std::abs(runNo)>=DsRunParamsMin&&std::abs(runNo)<=DsRunParamsMax) readRunParams(runNo);
  if(std::abs(runNo)>=runxyzMin&&std::abs(runNo)<=runxyzMax) readMeasuredEcms(runNo);
}

void ReadME::readRunParams(int runNo){
  m_sampleName=0; m_ecms=0; m_ecmsErr=0;
  m_aveEcms=0; m_aveEcmsErr=0; m_runFrom=0;
  m_runTo=0; m_runNoValid=false;
  m_px=0; m_py=0; m_pz=0;
  
  std::cout<<"MeasuredEcmsSvc begin read database RunParams"<<std::endl;
  int runNum=std::abs(runNo);

  if(runNo<0) std::cout<<"MeasuredEcmsSvc for MC events"<<std::endl;
  else std::cout<<"MeasuredEcmsSvc for DT events"<<std::endl;

  char stmt1[400];
  snprintf(stmt1, 1024, "select beam_energy, px, py, pz from RunParams where run_number = %d", runNum);

  DatabaseRecordVector res;
  int row_no = m_dbSvc->query("offlinedb", stmt1, res);
  if(row_no==0){std::cout<<"RowNo can not be 0 for RunParams, failed to read DB"<<std::endl;}
  else{
    DatabaseRecord* records = res[0];
    std::cout<<"MeasuredEcmsSvc first read the runNo, read the  RunParams database"<<runNo<<std::endl;
    double bE=0;
    bE=records->GetDouble("beam_energy");
    m_px=records->GetDouble("px");
    m_py=records->GetDouble("py");
    m_pz=records->GetDouble("pz");
    m_ecms=bE*2.00;
    m_runNoValid=true;
  }
}

