#include "MdSpi.h"
#include <iostream>
using namespace std;

//CThostFtdcMdApi *pUserApi;

char FRONT_ADDR[] = "tcp://218.202.237.33:10012";
TThostFtdcBrokerIDType  BROKER_ID = "9999";
TThostFtdcInvestorIDType INVESTOR_ID = "136588";
TThostFtdcPasswordType  PASSWORD = "doublewu1987";
char *ppInstrumentID[] = {"rb1905", "rb1906"};
int iInstrumentID = 2;
// 请求编号
int iRequestID = 0;

int main()
{
    CThostFtdcMdApi *pUserApi;
    pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
    CThostFtdcMdSpi* pUserSpi = new CMdSpi(pUserApi);
    cout << "before RegisterSpi" << endl;
    pUserApi->RegisterSpi(pUserSpi);
    pUserApi->RegisterFront(FRONT_ADDR);
    cout << "before Init" << endl;
    pUserApi->Init();
    cout << "after Init" << endl;
    pUserApi->Join();
    return 0;
    


}
