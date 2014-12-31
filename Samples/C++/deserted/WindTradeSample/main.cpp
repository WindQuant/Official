
#include "windows.h"

#include "WindTradeAPI.h"
#pragma comment(lib,"WindTradeAPI.lib") 

namespace
{
const char* p_ErrMsg = 0;
long        long_out;
const char* p_out;
double      double_out;

long        LogonID;
}

void WINAPI ResponseCallback( long ResponseID );

void Test_Login();
void Test_Order();
void Test_CapitalQry();
void Test_PositionQry();
void Test_OrderQry();
void Test_TradeQry();

void Test_login_report(long ResponseID);
void Test_Order_report(long ResponseID);
void Test_CapitalQry_report(long ResponseID);
void Test_PositionQry_report(long ResponseID);
void Test_OrderQry_report(long ResponseID);
void Test_Cancel_report(long ResponseID);
void Test_TradeQry_report(long ResponseID);
void Test_DepartmentQry_report(long ResponseID);
void Test_ReckoningQry_report(long ResponseID);
void Test_AccountQry_report(long ResponseID);

int main()
{
    WTradeInit(&ResponseCallback);

    WTradeAuthorize("填Wind认证ID","填Wind认证password");

    Test_Login();

    Test_Order();

    Test_CapitalQry();
    Test_PositionQry();
    Test_OrderQry();
    Test_TradeQry();


    return  0;
}

void WINAPI ResponseCallback( long ResponseID )
{
    const char* p_out = 0;
    long FunID;
    long ErrID;
    
    WTradeRespGetLong(ResponseID,0,W_FuncID,&FunID);
    WTradeRespGetLong(ResponseID,0,W_ErrID,&ErrID);
    WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);

    if(ErrID==WD_ERRID_SUCCESS)
    {
        switch(FunID)
        {
        case WD_FUNCID_LOGON: 
            Test_login_report(ResponseID);
            break;
        case WD_FUNCID_ORDER: 
            Test_Order_report(ResponseID);
            break;
        case WD_FUNCID_CAPITAL_QRY: 
            Test_CapitalQry_report(ResponseID);
            break;
        case WD_FUNCID_POSITION_QRY: 
            Test_PositionQry_report(ResponseID);
            break;
        case WD_FUNCID_ORDER_QRY: 
            Test_OrderQry_report(ResponseID);
            break;
        case WD_FUNCID_CANCEL: 
            Test_Cancel_report(ResponseID);
            break;
        case WD_FUNCID_TRADE_QRY: 
            Test_TradeQry_report(ResponseID);
            break;
        case WD_FUNCID_DEPARTMENT_QRY: 
            Test_DepartmentQry_report(ResponseID);
            break;
        case WD_FUNCID_ACCOUNT_QRY: 
            Test_AccountQry_report(ResponseID);
            break;
        }
    }
}

void Test_Login()
{
    long ResponseID = 0;
    int RequestID = WTradeCreateReq();
    
    if( RequestID )
    {
        WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_LOGON);        //功能号         
		WTradeReqSetStr(RequestID,W_BrokerID,"填券商BrokerID");      //券商代码        
        WTradeReqSetStr(RequestID,W_DepartmentID,0);                 //营业部代码        
        WTradeReqSetStr(RequestID,W_LogonAccount, "填资金账号");     //账号
        WTradeReqSetStr(RequestID,W_Password,"填密码");              //账号密码  
        WTradeReqSetLong(RequestID,W_AccountType,/*账号类型,比如:*/WD_ACCOUNT_SPSH);   //账号类型  

        WTradeSendReqSync(RequestID,&ResponseID);
    }

    Test_login_report(ResponseID);    
}

void Test_login_report(long ResponseID)
{
    if( ResponseID )
    {        
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);
    
        if( long_out == WD_ERRID_SUCCESS )
        {
            WTradeRespGetLong(ResponseID,0,W_RequestID,&long_out);
            WTradeRespGetLong(ResponseID,0,W_LogonID,&LogonID);
        }

		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }
}

void Test_Order()
{
    long ResponseID = 0;
    int RequestID = WTradeCreateReq();

    WTradeReqSetLong(RequestID,W_LogonID,LogonID);
    WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_ORDER);       //功能号     

    WTradeReqSetLong(RequestID,W_MarketType,WD_MARKET_CFE);      //市场代码    
    WTradeReqSetStr(RequestID,W_SecurityCode,"IF1308");         //证券代码    
               

    WTradeReqSetLong(RequestID,W_OrderVolume,1);                //委托数量    
    WTradeReqSetDouble(RequestID,W_OrderPrice,2300.0);          //委托价格    
    WTradeReqSetLong(RequestID,W_OrderType,WD_OrderType_LMT);	//价格委托方式    
    WTradeReqSetLong(RequestID,W_TradeSide,WD_TradeSide_Buy);
    WTradeReqSetLong(RequestID,W_HedgeType,WD_HedgeType_SPEC);

    WTradeSendReqAsyn(RequestID);
}        

void Test_Order_report(long ResponseID)
{
    if( ResponseID )
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);   
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);

        WTradeRespGetLong(ResponseID,0,W_MarketType,&long_out);
        WTradeRespGetStr(ResponseID,0,W_OrderNumber,&p_out);


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }
}

void Test_Cancel(long MarketType,const char* OrderNumber)
{
    int RequestID = WTradeCreateReq();

    WTradeReqSetLong(RequestID,W_LogonID,LogonID);
    WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_CANCEL);      //功能号      
    WTradeReqSetLong(RequestID,W_MarketType,MarketType);
    WTradeReqSetStr(RequestID,W_OrderNumber,OrderNumber);

    WTradeSendReqAsyn(RequestID);
}

void Test_Cancel_report(long ResponseID)
{
    if(ResponseID)
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);  
        WTradeRespGetLong(ResponseID,0,W_RequestID,&long_out);   
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out); 
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }
}    

void Test_CapitalQry()
{
    long ResponseID = 0;
    int RequestID = WTradeCreateReq();

    WTradeReqSetLong(RequestID,W_LogonID,LogonID);
    WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_CAPITAL_QRY);      //功能号

    WTradeSendReqSync(RequestID,&ResponseID);
    
    Test_CapitalQry_report(ResponseID);
}

void Test_CapitalQry_report(long ResponseID)
{
    if(ResponseID)
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);    
        WTradeRespGetLong(ResponseID,0,W_RequestID,&long_out);
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);
    
        WTradeRespGetLong(ResponseID,0,W_ResponseCount,&long_out);			//应答的记录条数                 

        WTradeRespGetStr(ResponseID,0,W_Customer,&p_out);	                //客户号                     
        WTradeRespGetStr(ResponseID,0,W_AssetAccount,&p_out);	            //资金账号                    
        WTradeRespGetStr(ResponseID,0,W_DepartmentID,&p_out);	            //营业部ID                   
        WTradeRespGetStr(ResponseID,0,W_MoneyType,&p_out);					//币种类型                    
        WTradeRespGetStr(ResponseID,0,W_Remark,&p_out);						//说明 

        //Stock
        WTradeRespGetDouble(ResponseID,0,W_AvailableFund,&double_out);		//资金可用                    
        WTradeRespGetDouble(ResponseID,0,W_BalanceFund,&double_out);	    //资金余额                    
        WTradeRespGetDouble(ResponseID,0,W_SecurityValue,&double_out);		//持仓市值资产                  
        WTradeRespGetDouble(ResponseID,0,W_FundAsset,&double_out);			//资金资产                    
        WTradeRespGetDouble(ResponseID,0,W_TotalAsset,&double_out);			//总资产                     
        WTradeRespGetDouble(ResponseID,0,W_Profit,&double_out);				//总盈亏                     
        WTradeRespGetDouble(ResponseID,0,W_FundFrozen,&double_out);			//冻结资金                    
        WTradeRespGetDouble(ResponseID,0,W_OtherFund,&double_out);			//其他资金                    
        WTradeRespGetDouble(ResponseID,0,W_BuyFund,&double_out);	        //今日买入金额                  
        WTradeRespGetDouble(ResponseID,0,W_SellFund,&double_out);	        //今日卖出金额                  
                   
    
        //Future
        WTradeRespGetDouble(ResponseID,0,W_FetchFund,&double_out);			//可取资金                    
        WTradeRespGetDouble(ResponseID,0,W_ExerciseMargin,&double_out);		//履约保证金                   
        WTradeRespGetDouble(ResponseID,0,W_RealFrozenMarginA,&double_out);	//当日开仓预冻结金额               
        WTradeRespGetDouble(ResponseID,0,W_RealFrozenMarginB,&double_out);	//当日开仓预冻结保证金和费用          
        WTradeRespGetDouble(ResponseID,0,W_HoldingProfit,&double_out);		//盯市盈亏                    
        WTradeRespGetDouble(ResponseID,0,W_TotalFloatProfit,&double_out);	//总浮动盈亏                   
        WTradeRespGetDouble(ResponseID,0,W_InitRightsBalance,&double_out);	//期初客户权益                  
        WTradeRespGetDouble(ResponseID,0,W_CurrRightsBalance,&double_out);	//客户权益                    
        WTradeRespGetDouble(ResponseID,0,W_FloatRightsBal,&double_out);		//浮动客户权益                  
        WTradeRespGetDouble(ResponseID,0,W_RealDrop,&double_out);	        //盯市平仓盈亏                  
        WTradeRespGetDouble(ResponseID,0,W_RealDrop_Float,&double_out);		//浮动平仓盈亏                  
        WTradeRespGetDouble(ResponseID,0,W_FrozenFare,&double_out);			//冻结费用                    
        WTradeRespGetDouble(ResponseID,0,W_CustomerMargin,&double_out);		//客户保证金                       
        WTradeRespGetDouble(ResponseID,0,W_RealOpenProfit,&double_out);		//盯市开仓盈亏                  
        WTradeRespGetDouble(ResponseID,0,W_FloatOpenProfit,&double_out);	//浮动开仓盈亏                  
        WTradeRespGetDouble(ResponseID,0,W_Interest,&double_out);	        //预计利息  


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }   
}

void Test_PositionQry()
{
    long ResponseID = 0;
    int RequestID = WTradeCreateReq();

    WTradeReqSetLong(RequestID,W_LogonID,LogonID);
    WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_POSITION_QRY);      //功能号   

    WTradeSendReqSync(RequestID,&ResponseID);

    Test_PositionQry_report(ResponseID);
}

void Test_PositionQry_report(long ResponseID)
{
    if(ResponseID)
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);      
        WTradeRespGetLong(ResponseID,0,W_RequestID,&long_out);
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);

        long record_count = 0;
        WTradeRespGetLong(ResponseID,0,W_ResponseCount,&record_count);		//应答的记录条数 

        for( int i=0; i<record_count; ++i )
        {
            WTradeRespGetStr(ResponseID,i,W_Customer,&p_out);	            //客户号                                    
            WTradeRespGetStr(ResponseID,i,W_AssetAccount,&p_out);           //资金账号                                   
            WTradeRespGetStr(ResponseID,i,W_MarketType,&p_out);	            //证券市场                                   
            WTradeRespGetStr(ResponseID,i,W_Shareholder,&p_out);	        //股东代码                                   
            WTradeRespGetStr(ResponseID,i,W_SecurityCode,&p_out);	        //证券代码                                   
            WTradeRespGetStr(ResponseID,i,W_SecurityName,&p_out);	        //证券名称                                   
            WTradeRespGetStr(ResponseID,i,W_DepartmentID,&p_out);	        //所属营业部                                  
            WTradeRespGetLong(ResponseID,i,W_MoneyType,&long_out);	        //币种类型 '0'-RMB '1'-HKD '2'-USD           
            WTradeRespGetStr(ResponseID,i,W_Remark,&p_out);	                //说明                                     
            
            //Stock
            WTradeRespGetDouble(ResponseID,i,W_SecurityBalance,&double_out);//股份余额                                   
            WTradeRespGetDouble(ResponseID,i,W_SecurityAvail,&double_out);	//股份可用                                   
            WTradeRespGetDouble(ResponseID,i,W_SecurityForzen,&double_out);	//股份冻结                                   
            WTradeRespGetDouble(ResponseID,i,W_TodayBuyVolume,&double_out);	//当日买入数                                  
            WTradeRespGetDouble(ResponseID,i,W_TodaySellVolume,&double_out);//当日卖出数                                  
            WTradeRespGetDouble(ResponseID,i,W_SecurityVolume,&double_out);	//当前拥股数                                  
            WTradeRespGetDouble(ResponseID,i,W_CallVolume,&double_out);	    //可申赎数量                                  
            WTradeRespGetDouble(ResponseID,i,W_CostPrice,&double_out);	        //成本价格                                   
            WTradeRespGetDouble(ResponseID,i,W_TradingCost,&double_out);		//当前成本                                   
            
            WTradeRespGetDouble(ResponseID,i,W_LastPrice,&double_out);	        //最新价格                                   
            WTradeRespGetDouble(ResponseID,i,W_HoldingValue,&double_out);	//市值                                     
            WTradeRespGetDouble(ResponseID,i,W_Profit,&double_out);	        //盈亏                                     
            
            //Future
            WTradeRespGetLong(ResponseID,i,W_TradeSide,&long_out);
            WTradeRespGetLong(ResponseID,i,W_BeginVolume,&long_out);		//期初数量                                   
            WTradeRespGetLong(ResponseID,i,W_EnableVolume,&long_out);		//可用数量                                   
            WTradeRespGetLong(ResponseID,i,W_TodayRealVolume,&long_out);	//当日可平仓数量                                
            WTradeRespGetLong(ResponseID,i,W_TodayOpenVolume,&long_out);	//当日开仓可用数量                               
            WTradeRespGetDouble(ResponseID,i,W_HoldingProfit,&double_out);	//盯市盈亏                               
            WTradeRespGetDouble(ResponseID,i,W_TotalFloatProfit,&double_out);//持仓浮动盈亏  
            WTradeRespGetDouble(ResponseID,i,W_PreMargin,&double_out);	    //上交易日保证金 
            
        }


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }
}    
 
void Test_OrderQry()
{
    long ResponseID = 0;
    {
        int RequestID = WTradeCreateReq();

        WTradeReqSetLong(RequestID,W_LogonID,LogonID);
        WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_ORDER_QRY);      //功能号        

        WTradeSendReqSync(RequestID,&ResponseID);
    }

    Test_OrderQry_report(ResponseID);
}

void Test_OrderQry_report(long ResponseID)
{
    long    MarketType=0;

    if(ResponseID)
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);       
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);

        long record_count = 0;
        WTradeRespGetLong(ResponseID,0,W_ResponseCount,&record_count);			//应答的记录条数       

        for( int i=0; i<record_count; ++i )
        {
            WTradeRespGetStr(ResponseID,i,W_Customer,&p_out);   	            //客户号                                                                    
            WTradeRespGetStr(ResponseID,i,W_AssetAccount,&p_out);	            //资金账号                                                                   
            WTradeRespGetStr(ResponseID,i,W_MoneyType,&p_out); 					//币种类型                                                                   
            WTradeRespGetLong(ResponseID,i,W_MarketType,&MarketType);	        //证券市场                                                               
                                                                                                                                                             
            WTradeRespGetLong(ResponseID,i,W_TradeSide,&long_out);
            WTradeRespGetStr(ResponseID,i,W_ExtFlag1,&p_out);	                //扩展标志
            WTradeRespGetStr(ResponseID,i,W_ExtFlag2,&p_out);   	            //扩展标志                                                                   
            WTradeRespGetStr(ResponseID,i,W_ExtFlag3,&p_out);   	            //扩展标志                                                                   
            WTradeRespGetLong(ResponseID,i,W_OrderStatus,&long_out);	        //委托状态 '0'-正常 '1'-撤单 '2'-无效                                              
            WTradeRespGetStr(ResponseID,i,W_Shareholder,&p_out);   				//股东代码                                                                   
            WTradeRespGetStr(ResponseID,i,W_SecurityCode,&p_out);  				//证券代码                                                                   
            WTradeRespGetStr(ResponseID,i,W_SecurityName,&p_out);  				//证券名称                                                                   
            WTradeRespGetStr(ResponseID,i,W_DepartmentID,&p_out);      			//所属营业部                                                                  
            WTradeRespGetLong(ResponseID,i,W_OrderDate,&long_out);   	        //委托日期                                                                   
            WTradeRespGetLong(ResponseID,i,W_OrderTime,&long_out);   	        //委托时间                                                                   
            WTradeRespGetLong(ResponseID,i,W_OrderVolume,&long_out); 	        //委托数量                                                               
            WTradeRespGetDouble(ResponseID,i,W_OrderPrice,&double_out);  	    //委托价格                                                                   
            WTradeRespGetLong(ResponseID,i,W_TradedVolume,&long_out);  			//成交数量                                                               
            WTradeRespGetDouble(ResponseID,i,W_TradedPrice,&double_out);   		//成交均价                                                                   
            WTradeRespGetLong(ResponseID,i,W_CancelVolume,&long_out);	        //撤单数量                                                               
            WTradeRespGetDouble(ResponseID,i,W_LastPrice,&double_out);     	    //最新价格                                                                   
            WTradeRespGetStr(ResponseID,i,W_OrderNumber,&p_out); 	            //委托序号                                                                                                                                                                                              
                                                                                                                                                             
            WTradeRespGetDouble(ResponseID,i,W_PreMargin,&double_out);			//开仓冻结保证金                                                                
            WTradeRespGetDouble(ResponseID,i,W_TotalFrozenCosts,&double_out);	//冻结总费用                                                                  
            WTradeRespGetLong(ResponseID,i,W_HedgeType,&long_out);   	        //套保标志  '0'-投机  '1'-保值                                                   
            WTradeRespGetStr(ResponseID,i,W_Seat,&p_out);      					//席位号                                                                    
            WTradeRespGetStr(ResponseID,i,W_Agent,&p_out);      	            //代理商号                                                                   
            WTradeRespGetStr(ResponseID,i,W_Remark1,&p_out);      				//说明1                                                                    
            WTradeRespGetStr(ResponseID,i,W_Remark2,&p_out);      				//说明2                                                                    
                       
        }


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }
}

void Test_TradeQry()
{
    long ResponseID = 0;
    {
        int RequestID = WTradeCreateReq();

        WTradeReqSetLong(RequestID,W_LogonID,LogonID);
        WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_TRADE_QRY);      //功能号     

        WTradeSendReqSync(RequestID,&ResponseID);
    }

    Test_TradeQry_report(ResponseID);
}

void Test_TradeQry_report(long ResponseID)
{
    if(ResponseID)
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);       
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);

        long record_count = 0;
        WTradeRespGetLong(ResponseID,0,W_ResponseCount,&record_count);			//应答的记录条数   

        for( int i=0; i<record_count; ++i )
        {
            WTradeRespGetStr(ResponseID,i,W_Customer,&p_out);   	            //客户号                                                              
            WTradeRespGetStr(ResponseID,i,W_AssetAccount,&p_out);	            //资金账号                                                             
            WTradeRespGetStr(ResponseID,i,W_MoneyType,&p_out);  	            //币种类型                                                             
            WTradeRespGetStr(ResponseID,i,W_MarketType,&p_out); 	            //证券市场                                                             
            WTradeRespGetLong(ResponseID,i,W_TradeSide,&long_out);
            WTradeRespGetStr(ResponseID,i,W_ExtFlag1,&p_out);   	            //扩展标志
            WTradeRespGetStr(ResponseID,i,W_ExtFlag2,&p_out);    	            //扩展标志                                                             
            WTradeRespGetStr(ResponseID,i,W_ExtFlag3,&p_out);    	            //扩展标志                                                             
            WTradeRespGetStr(ResponseID,i,W_TradedStatus,&p_out);  				//成交状态 '0'-正常 '1'-撤单                                               
            WTradeRespGetStr(ResponseID,i,W_Shareholder,&p_out);    	        //股东代码                                                             
            WTradeRespGetStr(ResponseID,i,W_SecurityCode,&p_out);   	        //证券代码                                                             
            WTradeRespGetStr(ResponseID,i,W_SecurityName,&p_out);   	        //证券名称                                                             
            WTradeRespGetLong(ResponseID,i,W_OrderDate,&long_out);   	        //委托日期                                                             
            WTradeRespGetLong(ResponseID,i,W_OrderTime,&long_out);   	        //委托时间                                                             
            WTradeRespGetLong(ResponseID,i,W_OrderVolume,&long_out); 	        //委托数量                                                             
            WTradeRespGetDouble(ResponseID,i,W_OrderPrice,&double_out);  	    //委托价格                                                             
            WTradeRespGetLong(ResponseID,i,W_TradedVolume,&long_out);  			//成交数量                                                             
            WTradeRespGetDouble(ResponseID,i,W_TradedPrice,&double_out);   		//成交价格                                                             
            WTradeRespGetLong(ResponseID,i,W_CancelVolume,&long_out);	        //撤单数量                                                             
            WTradeRespGetLong(ResponseID,i,W_TradedDate,&long_out);    			//成交日期                                                             
            WTradeRespGetLong(ResponseID,i,W_TradedTime,&long_out);    			//成交时间                                                             
            WTradeRespGetDouble(ResponseID,i,W_LastPrice,&double_out);    		//最新价格                                                             
            WTradeRespGetStr(ResponseID,i,W_OrderNumber,&p_out); 	            //委托序号                                                             
            WTradeRespGetStr(ResponseID,i,W_TradedNumber,&p_out);  				//成交编号                                                             
            WTradeRespGetStr(ResponseID,i,W_Remark,&p_out);      	            //说明                                                               
            WTradeRespGetStr(ResponseID,i,W_Remark1,&p_out);     	            //其它说明                                                             
            WTradeRespGetDouble(ResponseID,i,W_MadeAmt,&double_out);	        //成交金额                                                             

            
            WTradeRespGetLong(ResponseID,i,W_AmountPerHand,&long_out);    		//每手吨数                                       
            WTradeRespGetLong(ResponseID,i,W_HedgeType,&long_out);    			//套保标志  '0'-投机  '1'-保值                       
            WTradeRespGetDouble(ResponseID,i,W_TotalFrozenCosts,&double_out);	//冻结总费用                                      
            WTradeRespGetDouble(ResponseID,i,W_DropProfit,&double_out);			//平仓盈亏                                       
            WTradeRespGetDouble(ResponseID,i,W_DropFloatFrofit,&double_out);	//平仓浮动盈亏	                                 
            WTradeRespGetStr(ResponseID,i,W_Seat,&p_out);     					//席位号                                        
            WTradeRespGetStr(ResponseID,i,W_Agent,&p_out);     					//代理商号                                       
            
        }


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲

    }
}

void Test_DepartmentQry()
{
    long ResponseID = 0;
    {
        int RequestID = WTradeCreateReq();

        WTradeReqSetStr(RequestID,W_BrokerID,"填券商BrokerID");         //券商代码  
        WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_DEPARTMENT_QRY);	//功能号  

        WTradeSendReqSync(RequestID,&ResponseID);
    }

    Test_DepartmentQry_report(ResponseID);
}

void Test_DepartmentQry_report(long ResponseID)
{
    if(ResponseID)
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);       
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);

        long record_count = 0;
        WTradeRespGetLong(ResponseID,0,W_ResponseCount,&record_count);		//应答的记录条数 
    
        for( int i=0; i<record_count; ++i )
        {
            WTradeRespGetStr(ResponseID,i,W_DepartmentID,&p_out);          	//营业部ID                                                              
            WTradeRespGetStr(ResponseID,i,W_DepartmentName,&p_out);        	//营业部名称                                                              
            WTradeRespGetStr(ResponseID,i,W_AvailMarketFlag,&p_out);		//可操作市场标识, 按位运算                                                      
        }


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }
}

void Test_AccountQry()
{
    long ResponseID = 0;
    {
        int RequestID = WTradeCreateReq();

        WTradeReqSetLong(RequestID,W_LogonID,LogonID);
        WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_ACCOUNT_QRY);      //功能号    

        WTradeSendReqSync(RequestID,&ResponseID);
    }

    Test_AccountQry_report(ResponseID);
}

void Test_AccountQry_report(long ResponseID)
{
    if(ResponseID)
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);       
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);

        long record_count = 0;
        WTradeRespGetLong(ResponseID,0,W_ResponseCount,&record_count);		//应答的记录条数  
    
        for( int i=0; i<record_count; ++i )
        {
            WTradeRespGetLong(ResponseID,i,W_ShareholderStatus,&long_out);	//状态  '0'-正常 '1'-冻结        
            WTradeRespGetStr(ResponseID,i,W_MainShareholderFlag,&p_out);	//主股东标志 '1'-主  '0'-副           
            WTradeRespGetStr(ResponseID,i,W_AccountType,&p_out);	        //账号类型                         
            WTradeRespGetStr(ResponseID,i,W_MarketType,&p_out); 	        //市场代码                         
            WTradeRespGetStr(ResponseID,i,W_DepartmentID,&p_out);      		//所属营业部                        
            WTradeRespGetStr(ResponseID,i,W_Shareholder,&p_out);   			//股东代码                         
            WTradeRespGetStr(ResponseID,i,W_CustomerName,&p_out);   	    //客户姓名                         
            WTradeRespGetStr(ResponseID,i,W_AssetAccount,&p_out);	        //资金账号                         
            WTradeRespGetStr(ResponseID,i,W_Customer,&p_out);   	        //客户号                          
            WTradeRespGetStr(ResponseID,i,W_Seat,&p_out);	                //席位号                          
        }


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }
}
    
void Test_ReckoningQry()
{
    long ResponseID = 0;
    int RequestID = WTradeCreateReq();

    WTradeReqSetLong(RequestID,W_LogonID,LogonID);
    WTradeReqSetLong(RequestID,W_FuncID,WD_FUNCID_RECKONING_QRY);      //功能号    

    WTradeSendReqSync(RequestID,&ResponseID);

    Test_ReckoningQry_report(ResponseID);
}

void Test_ReckoningQry_report(long ResponseID)
{
    if(ResponseID)
    {
        WTradeRespGetLong(ResponseID,0,W_FuncID,&long_out);      
        WTradeRespGetLong(ResponseID,0,W_ErrID,&long_out);
        WTradeRespGetStr(ResponseID,0,W_ErrMsg,&p_out);

        long record_count = 0;
        WTradeRespGetLong(ResponseID,0,W_ResponseCount,&record_count);        //应答的记录条数  
    
        for( int i=0; i<record_count; ++i )
        {
            WTradeRespGetStr(ResponseID,i,W_Customer,&p_out);		//客户号
	        WTradeRespGetStr(ResponseID,i,W_AssetAccount,&p_out);	//资金账号
	        WTradeRespGetLong(ResponseID,i,W_InfoDate,&long_out);	//信息日期
	        WTradeRespGetLong(ResponseID,i,W_InfoTime,&long_out);	//信息时间
	        WTradeRespGetStr(ResponseID,i,W_InfoMsg,&p_out);	    //信息内容
        }


		WTradeBuffRelease(ResponseID);	//使用后释放回报缓冲
    }
}


