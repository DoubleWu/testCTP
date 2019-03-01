#include "TraderSpi.h"
#include <iostream>
using namespace std;

//CThostFtdcMdApi *pUserApi;

char FRONT_ADDR[] = "tcp://218.202.237.33:10002";
TThostFtdcBrokerIDType  BROKER_ID = "9999";
TThostFtdcInvestorIDType INVESTOR_ID = "136588";
TThostFtdcPasswordType  PASSWORD = "doublewu001";
//TThostFtdcPasswordType  PASSWORD = "doublewu1987";

TThostFtdcInstrumentIDType  INSTRUMENT_ID = "rb1906";
TThostFtdcDirectionType DIRECTION = THOST_FTDC_D_Sell;
TThostFtdcPriceType LIMIT_PRICE = 2380;


// 请求编号
int iRequestID = 0;

int main()
{
    CThostFtdcTraderApi *pUserApi;
    pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    CTraderSpi *pUserSpi = new CTraderSpi(pUserApi);
    pUserApi->RegisterSpi(pUserSpi);
    pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);
    pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);
    pUserApi->RegisterFront(FRONT_ADDR);
    cout << "init" << endl;
    pUserApi->Init();
    pUserApi->Join();
    return 0;
    


}
