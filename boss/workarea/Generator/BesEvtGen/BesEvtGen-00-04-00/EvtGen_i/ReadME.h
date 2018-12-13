/****** Write By Lianjin WU <wulj@ihep.ac.cn> ******/
/****** Reading offline beam information ******/
#ifndef Analysis_ReadME_H
#define Analysis_ReadME_H
/******/
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/AlgFactory.h"
#include "DatabaseSvc/IDatabaseSvc.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <mysql.h>
#include <string.h>
class ReadME{
	public:
		ReadME(int runNo){
			if(runNo!=preRunNo){
				Gaudi::svcLocator()->service("DatabaseSvc", m_dbSvc, true);
				readDB(runNo);
				preRunNo=runNo;
			}
		}
		virtual ~ReadME(){}
		/******/
		void readMeasuredEcms(int runNo);
		int getSampleName(){return m_sampleName;}
		int getRunFrom(){return m_runFrom;}
		int getRunTo(){return m_runTo;}
		bool isRunNoValid(){return m_runNoValid;}
		double getEcms(){return m_ecms;}
		double getEcmsErr(){return m_ecmsErr;}
		double getAveEcms(){return m_aveEcms;}
		double getAveEcmsErr(){return m_aveEcmsErr;}
		void readDB(int runNo);
		void readRunParams(int runNo);
	private:
		//RunParamsMin and RunParamsMax are used to set range for psi3773 listed in RunParams
		int RunParamsMin, RunParamsMax;
		//MeasuredEcmsMin and MeasuredEcmsMax for the data listed in MeasuredEcms2
		int MeasuredEcmsMin, MeasuredEcmsMax;
		//DsRunParamsMin and DsRunParamsMax are used to set range for the new 4180 data listed in RunParams
		int DsRunParamsMin, DsRunParamsMax;

		IDatabaseSvc* m_dbSvc;
		static int preRunNo;
		int m_sampleName, m_runFrom, m_runTo;
		double m_ecms, m_ecmsErr, m_aveEcms, m_aveEcmsErr;
		double m_px, m_py, m_pz;
		bool m_runNoValid;
};
#endif 
