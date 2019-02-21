#include <iostream>
#include "ThostFtdcMdApi.h"

class CMduserHandler : public CThostFtdcMdApi
{
    private:
        CThostFtdcMdApi *m_mdApi;
    public:
        void connect();
};

