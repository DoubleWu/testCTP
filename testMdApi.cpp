#include "ThostFtdcMdApi.h"
#include "MdSpi.h"

CThostFtdcMdApi *pUserApi;

char FRONT_ADDR[] = "tcp://218.202.237.337:10012";
TThostFtdcBrokerIDType  BROKER_ID = "9999";
TThostFtdcInvestorIDType INVESTOR_ID = "136588";
TThostFtdcPasswordType  PASSWORD = "doublewu1987";
char *ppInstrumentID[] = {"cu0907", "cu0909"};
int iInstrumentID = 2;
// 请求编号
int iRequestID = 0;

int main()
{
    pUserApi= CThostFtdcMdApi::CThostFtdcMdApi();
    CThostFtdcMdSpi* pUserSpi = new CMdSpi();
    


}
