#include "TraderSpi.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

extern char FRONT_ADDR[];
extern TThostFtdcBrokerIDType  BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType  PASSWORD;

extern char INSTRUMENT_ID[];    // 合约代码
extern TThostFtdcPriceType  LIMIT_PRICE;    // 价格
extern TThostFtdcDirectionType  DIRECTION;  // 买卖方向
extern int iRequestID;

TThostFtdcFrontIDType   FRONT_ID;   //前置编号
TThostFtdcSessionIDType SESSION_ID; //会话编号
TThostFtdcOrderRefType  ORDER_REF;  //报单引用
time_t lOrderTime;
time_t lOrderOkTime;


void CTraderSpi::OnFrontConnected()
{
    cerr << "connected" << endl;
    ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
    CThostFtdcReqUserLoginField reqUserLogin;
    memset(&reqUserLogin, 0, sizeof(reqUserLogin));
    strcpy(reqUserLogin.BrokerID, BROKER_ID);
    strcpy(reqUserLogin.UserID, INVESTOR_ID);
    strcpy(reqUserLogin.Password, PASSWORD);
    int iResult = pUserApi->ReqUserLogin(&reqUserLogin, ++iRequestID);
    cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}
void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspUserLogin" << endl;
    /*if (pRspInfo->ErrorID == 140)
    {
        cerr << "update password" << endl;
        CThostFtdcUserPasswordUpdateField req;
        strcpy(req.BrokerID, BROKER_ID);
        strcpy(req.UserID, INVESTOR_ID);
        strcpy(req.OldPassword, PASSWORD);
        strcpy(req.NewPassword, "doublewu001");
        int iResult = pUserApi->ReqUserPasswordUpdate(&req, ++iRequestID);
        if (iResult != 0)
            cerr << "--->>> ReqUserPasswordUpdate Error" <<endl;
        //else
        //{
        //    strcpy(PASSWORD, "doublewu001");
        //    cerr << "relogin" << endl;
        //    ReqUserLogin();
        //}
            
    }*/
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        FRONT_ID = pRspUserLogin->FrontID;
        SESSION_ID = pRspUserLogin->SessionID;
        int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
        iNextOrderRef++;
        sprintf(ORDER_REF, "%d", iNextOrderRef);
        cerr << "--->>> 获取当前交易日 = " << pUserApi->GetTradingDay() << endl;
        //结算结果确认
        ReqSettlementInfoConfirm();            
    }
}

void CTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "ErrorID:" << pRspInfo->ErrorID << endl;
    cerr << "oldpassword:" << pUserPasswordUpdate->OldPassword << "newpassword:" << pUserPasswordUpdate->NewPassword << endl;
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
    cerr << "--->>> ReqSettlementInfoConfirm" << endl;    
    CThostFtdcSettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
    cerr << "--->>> 投资者结算结果确认: " << ((iResult == 0) ? "成功" : "失败") << endl;

}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        //请求查询合约
        ReqQryInstrument();
    }
}

void CTraderSpi::ReqQryInstrument()
{
    CThostFtdcQryInstrumentField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.InstrumentID, INSTRUMENT_ID);    
    int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
    cerr << "--->>> 请求查询合约: " << ((iResult == 0) ? "成功" : "失败") << endl; 
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
        cerr << "--->>> " << "OnRspQryInstrument" << endl;
        cerr << "ID:" << pInstrument->ExchangeID << " InstrumentName:" << pInstrument->InstrumentName << " ExchangeInstID:" << pInstrument->ExchangeInstID << endl;
        if (bIsLast && !IsErrorRspInfo(pRspInfo))
        {
            //查询资金账户
            sleep(1);
            ReqQryTradingAccount();
        }    
}

void CTraderSpi::ReqQryTradingAccount()
{
        CThostFtdcQryTradingAccountField req;
        memset(&req, 0, sizeof(req));
        strcpy(req.BrokerID, BROKER_ID);
        strcpy(req.InvestorID, INVESTOR_ID);
        strcpy(req.CurrencyID, "CNY");
        int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
        cerr << "--->>> 请求查询资金账户: " << ((iResult == 0) ? "成功" : "失败") << endl;    
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspQryTradingAccount" << endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        //cerr << "AccountID:" <<pTradingAccount->AccountID << " Available:" << pTradingAccount->Available << endl;
        printf("AccountID:%s, Available:%lf\n", pTradingAccount->AccountID, pTradingAccount->Available);
        //请求查询投资者持仓
        sleep(1);
        ReqQryInvestorPosition();
    }    
    
}

void CTraderSpi::ReqQryInvestorPosition()
{
    CThostFtdcQryInvestorPositionField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
    cerr << "--->>> 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << iResult << endl;        
    
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspQryInvestorPosition" << endl;
    //cout << "Position" << pInvestorPosition->Position << endl;
    if (pInvestorPosition == NULL)
        cout << "持仓数为0" <<endl;
    if (bIsLast && !IsErrorRspInfo(pRspInfo))
    {
        //报单录入请求
        ReqOrderInsert();
    }

}

void CTraderSpi::ReqOrderInsert()
{
    CThostFtdcInputOrderField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    strcpy(req.InstrumentID, INSTRUMENT_ID);
    strcpy(req.OrderRef, ORDER_REF);
    req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
    req.Direction = DIRECTION;
    req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
    req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
    req.VolumeTotalOriginal = 1;
    req.TimeCondition = THOST_FTDC_TC_GFD;
    req.VolumeCondition = THOST_FTDC_VC_AV;
    req.MinVolume = 1;
    req.ContingentCondition = THOST_FTDC_CC_Immediately;
    req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    req.IsAutoSuspend = 0;
    req.UserForceClose = 0;
    lOrderTime=time(NULL);
    int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
    cerr << "--->>> 报单录入请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspOrderInsert" << endl;
    IsErrorRspInfo(pRspInfo);

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
    
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    
}

void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
    cerr << "--->>> " << "OnRtnTrade"  << endl;
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
    cerr << "--->>> " << "OnFrontDisconnected" << endl;
    cerr << "--->>> Reason = " << nReason << endl;
}
        
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
    cerr << "--->>> " << "OnHeartBeatWarning" << endl;
    cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    cerr << "--->>> " << "OnRspError" << endl;
    IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
        cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
    return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
    return ((pOrder->FrontID == FRONT_ID) &&
            (pOrder->SessionID == SESSION_ID) &&
            (strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
    return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
            (pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
            (pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}
