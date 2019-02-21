#include "CMduserHandler.h"

void CMduserHandler::connect()
{
    m_mdApi = CThostFtdcMdApi::CreateFtdcMdApi("", true, true);
    m_mdApi->RegisterSpi(this);
    m_mdApi->RegisterFront("tcp://218.202.237.33:10002");
    m_mdApi->Init(); 
}
