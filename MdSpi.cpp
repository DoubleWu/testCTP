#include "MdSpi.h"
#include <iostream>
#include <string.h>
#pragma warning(disable : 4996)

using namespace std;

extern CThostFtdcMdApi* pUserApi;
extern char FRONT_ADDR[];       
extern TThostFtdcBrokerIDType   BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType   PASSWORD;
extern char* ppInstrumentID[];  
extern int iInstrumentID;

extern int iRequestID;

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
        cerr << "--->>> "<< __FUNCTION__ << endl;
            IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
        cerr << "--->>> " << __FUNCTION__ << endl;
        cerr << "--->>> Reason = " << nReason << endl;
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
        cerr << "--->>> " << __FUNCTION__ << endl;
        cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
   cerr << "--->>> " << __FUNCTION__ << endl;
   cout << "ReqUserLogin" << endl;
   ReqUserLogin();
   cout << "connected" << endl;
}

void CMdSpi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));    
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, INVESTOR_ID);
    strcpy(req.Password, PASSWORD);
    int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
    if (iResult != 0)
        cerr << "fail login, iResult=" << iResult << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << __FUNCTION__ << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        cout << "getTradingDay" << endl;
        cerr << "--->>> 获取当前交易日 = " << pUserApi->GetTradingDay() << endl;
        SubscribeMarketData();    
    }
}

void CMdSpi::SubscribeMarketData()
{
    int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);    
    cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << __FUNCTION__ << endl;
    cout << "InstrumentID" << pSpecificInstrument->InstrumentID << endl;
    cout << "nRequestID:" << nRequestID << endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << __FUNCTION__ << endl;
}

void CMdSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField*pRspInfo, int nRequestID, bool bIsLast) 
{
    cerr << __FUNCTION__ << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    cerr << __FUNCTION__ << endl;    
}

void CMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
    cerr << __FUNCTION__ << endl;    
}

void CMdSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
   cerr << __FUNCTION__ << endl; 
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
    bool bResult = (pRspInfo->ErrorID != 0);
    if (bResult)
        cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}
