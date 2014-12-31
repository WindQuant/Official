/*************************************************************************
【文件名】          （WindTradeAPI.h）
【功能模块和目的】  （交易API接口头文件）
【开发者及日期】    （沈子敬 2013.5.20）
【版本】            （1.0.0.1）
【版权信息】        （wind资讯）
【更新记录】        （）
*************************************************************************/

#ifndef WINDTRADEAPI_H
#define WINDTRADEAPI_H

//--------------------------------------------------------------------------
//函数返回值定义 WD_ERR
typedef long WD_ERR;

#define WD_ERR_Success                  0       //正确

#define WD_ERR_Base                     -40530000

#define WD_ERR_DataErr                  WD_ERR_Base-1       //001数据错误
#define WD_ERR_Uninit                   WD_ERR_Base-2       //002未初始化
#define WD_ERR_FuncID                   WD_ERR_Base-3       //003功能号错
#define WD_ERR_LogonID                  WD_ERR_Base-4       //004LogonID错（或未输入此参数）
#define WD_ERR_BufferOver               WD_ERR_Base-5       //005回报队列已满
#define WD_ERR_SendRequestFailed        WD_ERR_Base-6       //006发送请求失败

#define WD_ERR_UnAuthorize              WD_ERR_Base-101     //101未通过认证
#define WD_ERR_LOGON                    WD_ERR_Base-102	    //102 登录错误
#define WD_ERR_NETWORK                  WD_ERR_Base-103	    //103 网络错误
#define WD_ERR_ORDER                    WD_ERR_Base-104	    //104 委托错误
#define WD_ERR_CANCEL                   WD_ERR_Base-105	    //105 撤单错误
#define WD_ERR_QUERY                    WD_ERR_Base-106	    //106 查询错误
#define WD_ERR_COVEREDCHG               WD_ERR_Base-107     //107 备兑证券划转错误

#define WD_ERR_Config                   WD_ERR_Base-201     //201获取配置错误
#define WD_ERR_BrokerID                 WD_ERR_Base-202     //202券商（期货商）代码错
#define WD_ERR_LogonAccount             WD_ERR_Base-203     //203登录代码错
#define WD_ERR_Password                 WD_ERR_Base-204     //204账号密码错
#define WD_ERR_AccountType              WD_ERR_Base-205     //205账号类型错
#define WD_ERR_LogonCountOver           WD_ERR_Base-206     //206登录失败次数过多

#define WD_ERR_ConnectFailed            WD_ERR_Base-301     //301建立连接失败
#define WD_ERR_TimeOut                  WD_ERR_Base-302     //302处理超时

#define WD_ERR_SecurityCode             WD_ERR_Base-401     //401交易代码错
#define WD_ERR_OrderType                WD_ERR_Base-402     //402价格委托方式错
#define WD_ERR_OrderVolume              WD_ERR_Base-403     //403委托数量错
#define WD_ERR_TradeSide                WD_ERR_Base-404     //404交易方向错
#define WD_ERR_MarketType               WD_ERR_Base-405     //405市场代码错
#define WD_ERR_HedgeType                WD_ERR_Base-406     //406投机套保字段错
#define WD_ERR_OrderListOver            WD_ERR_Base-407     //407委托队列满
#define WD_ERR_OptionType               WD_ERR_Base-408     //408期权类别字段错
#define WD_ERR_OptionUnderlyingCode     WD_ERR_Base-409     //409期权标的券字段错

#define WD_ERR_CancelOrderNumber        WD_ERR_Base-501     //501撤单委托序号错
#define WD_ERR_OrderStatusCannotCancel  WD_ERR_Base-502     //502不可撤单

#define WD_ERR_QueryTooQuick            WD_ERR_Base-601     //601查询过于频繁
#define WD_ERR_ResponseBufferUpdated    WD_ERR_Base-602     //602回报数据已被新请求覆盖
#define WD_ERR_ResponseCountOver        WD_ERR_Base-603     //603获取回报数据超出记录数
#define WD_ERR_MutliQueryCondition      WD_ERR_Base-604     //604多于一个的查询条件
#define WD_ERR_NoRequestID              WD_ERR_Base-605     //605无对应请求流水号记录
#define WD_ERR_NoOrderReport            WD_ERR_Base-606     //606无对应委托回报
#define WD_ERR_OrderFailed              WD_ERR_Base-607     //607委托失败
#define WD_ERR_OrderSent                WD_ERR_Base-608     //608委托已发送
#define WD_ERR_CoveredQryCode           WD_ERR_Base-609     //609备兑查询标的券字段错
#define WD_ERR_CoveredQryChg            WD_ERR_Base-610     //610备兑查询方向字段错

//--------------------------------------------------------------------------
//字段（TAG）定义
//W_开头表示TAG值
//WD_开头表示具体内容定义

//通用字段
#define W_LogonID               0       //登录返回的LogonID

    //--------------------------------------------------------------------------
#define W_FuncID                1       //功能代号
    //功能代号定义
    #define WD_FUNCID_LOGON             1001    //登录LOGON
    #define WD_FUNCID_LOGOUT            1002    //登出LOGOUT
    #define WD_FUNCID_SETT_CONFIRM      1003    //期货对账单确认
    #define WD_FUNCID_ORDER             2001    //委托
    #define WD_FUNCID_CANCEL            2002    //撤单
    #define WD_FUNCID_COVERED_LOCK      2003    //备兑证券锁定
    #define WD_FUNCID_COVERED_UNLOCK    2004    //备兑证券解锁
    #define WD_FUNCID_CAPITAL_QRY       3001    //资金查询
    #define WD_FUNCID_POSITION_QRY      3002    //持仓查询
    #define WD_FUNCID_ORDER_QRY         3003    //委托查询
    #define WD_FUNCID_TRADE_QRY         3004    //成交查询
    #define WD_FUNCID_DEPARTMENT_QRY    3005    //营业部查询
    #define WD_FUNCID_ACCOUNT_QRY       3006    //股东查询
    #define WD_FUNCID_RECKONING_QRY     3007    //对账单查询
    #define WD_FUNCID_DELIVERY_QRY	    3008	//交割单查询
    #define WD_FUNCID_COVERED_QRY       3009    //备兑股份查询
    #define WD_FUNCID_COVERED_MAXVOL    3010    //备兑可划转数量查询
    #define WD_FUNCID_GETBROKERLIST     9001    //获取券商（期货商）信息
    #define WD_FUNCID_GETLOGONEDLIST    9002    //获取已登录账户信息
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_AccountType           2       //账号类型
    //账号类型定义
    #define WD_ACCOUNT_SZSHA            11      //深圳上海A
    #define WD_ACCOUNT_SZB              12      //深圳B
    #define WD_ACCOUNT_SHB              13      //上海B

    #define WD_ACCOUNT_SPZZ             14      //郑州商品
    #define WD_ACCOUNT_SPSH             15      //上海商品
    #define WD_ACCOUNT_SPDL             16      //大连商品
    #define WD_ACCOUNT_CF               17      //股指商品

    #define WD_ACCOUNT_SHO              21      //上证期权
    //--------------------------------------------------------------------------

#define W_Customer              3       //客户号
#define W_AssetAccount          4       //资金账号

#define W_BankCode              6       //托管银行

#define W_Password              8       //客户密码
#define W_ResponseCount         9       //应答的记录条数

#define W_OrderNumber           11      //柜台委托编号(指定合同号)
#define W_OrderDate             12      //委托日期
#define W_OrderTime             13      //委托时间
#define W_OrderVolume           14      //委托数量
#define W_OrderPrice            15      //委托价格

    //--------------------------------------------------------------------------
#define W_TradeSide             16      //交易方向标志
    //交易方向定义
    #define WD_TradeSide_Buy            '1'      //买入开仓(等同=证券买入)
    #define WD_TradeSide_Short          '2'      //卖出开仓
    #define WD_TradeSide_Cover          '3'      //买入平仓
    #define WD_TradeSide_Sell           '4'      //卖出平仓(等同=证券卖出)
    #define WD_TradeSide_CoverToday     '5'      //买入平今仓
    #define WD_TradeSide_SellToday      '6'      //卖出平今仓
    #define WD_TradeSide_ShortCovered   '7'      //备兑开仓
    #define WD_TradeSide_CoverCovered   '8'      //备兑平仓

    //--------------------------------------------------------------------------

#define W_Shareholder           17      //客户代码(股东代码)

    //--------------------------------------------------------------------------
#define W_MarketType            18      //市场类型
    //市场代码定义
    #define WD_MARKET_SZ                0       //证券-深圳
    #define WD_MARKET_SH                1       //证券-上海
    #define WD_MARKET_OC                2       //证券-深圳特（三版）
    #define WD_MARKET_HK                6       //证券-港股
    #define WD_MARKET_CZC               7       //商品期货(郑州)
    #define WD_MARKET_SHF               8       //商品期货(上海)
    #define WD_MARKET_DCE               9       //商品期货(大连)
    #define WD_MARKET_CFE               10      //股指期货(中金)
    //--------------------------------------------------------------------------

#define W_SecurityCode          19      //交易代码（证券代码/期货合约代码/期权代码）

    //--------------------------------------------------------------------------
#define W_MoneyType             20      //币种类型
    //币种类型定义
    #define WD_MoneyType_ALL            'A'     //ALL-0x0-全部
    #define WD_MoneyType_RMB            '0'     //'0'-RMB
    #define WD_MoneyType_HSD            '1'     //'1'-HKD
    #define WD_MoneyType_USD            '2'     //'2'-USD
    //--------------------------------------------------------------------------

#define W_DepartmentID          21      //营业部ID

#define W_QryType               25      //查询模式（默认查所有）
    //--------------------------------------------------------------------------
    //查询模式定义
    #define WD_QryType_Ordering         '6'     //可撤单（挂单）查询（委托查询,委托数量>成交数量+撤单数量）

#define W_Remark                26      //备注说明
#define W_SecurityName          27      //交易品种名称（证券名称/期货合约名称/期权名称）
#define W_LastPrice             28      //最新价格
#define W_Profit                29      //盈亏
#define W_ExtFlag1              30      //扩展标志
#define W_ExtFlag2              31      //扩展标志
#define W_ExtFlag3              32      //扩展标志
#define W_QryCondition          33      //查询条件

#define W_RequestID             97      //对应的请求ID号

    //--------------------------------------------------------------------------
#define W_ErrID                 98      //错误代号（所以返回均带有此字段）
    //错误代号定义
    #define WD_ERRID_SUCCESS            WD_ERR_Success		    //0   正确
    #define WD_ERRID_AUTHORIZE          WD_ERR_UnAuthorize		//101 认证错误
    #define WD_ERRID_LOGON              WD_ERR_LOGON  		    //102 登录错误
    #define WD_ERRID_NETWORK            WD_ERR_NETWORK		    //103 网络错误
    #define WD_ERRID_ORDER              WD_ERR_ORDER  		    //104 委托错误
    #define WD_ERRID_CANCEL             WD_ERR_CANCEL 		    //105 撤单错误
    #define WD_ERRID_QUERY              WD_ERR_QUERY  		    //106 查询错误
    #define WD_ERRID_COVEREDCHG         WD_ERR_COVEREDCHG       //107 备兑证券划转错误
    //--------------------------------------------------------------------------

#define W_ErrMsg                99      //错误信息

    //--------------------------------------------------------------------------
#define W_LogonType             101     //登录类型   SABCK
    //登录类型定义
    #define WD_LOGON_S                  'S'     //S－股东代码
    #define WD_LOGON_A                  'A'     //A－资金账号
    #define WD_LOGON_B                  'B'     //B－经纪人号
    #define WD_LOGON_C                  'C'     //C－客户号
    #define WD_LOGON_K                  'K'     //K－卡号
    #define WD_LOGON_H                  'H'     //H－后台账号
    //--------------------------------------------------------------------------

#define W_LogonAccount          102     //登录代码
#define W_CheckPassword         103     //验证码
#define W_BrokerID              104     //券商代码

#define W_Seat                  131     //席位号
#define W_Agent                 132     //代理商号
#define W_TradePassword         133     //交易密码

    //--------------------------------------------------------------------------
#define W_OrderType             134     //价格委托方式
    //价格委托方式定义
    #define WD_OrderType_LMT            0       //限价委托
    #define WD_OrderType_BOC            1       //best of counterparty.对方最优价格委托
    #define WD_OrderType_BOP            2       //best of party.本方最优价格委托
    #define WD_OrderType_ITC            3       //immediately then cancel.即时成交剩余撤销
    #define WD_OrderType_B5TC           4       //best 5 then cancel.最优五档剩余撤销
    #define WD_OrderType_FOK            5       //fill or kill.全额成交或撤销委托(市价FOK)
    #define WD_OrderType_B5TL           6       //best 5 then limit.最优五档剩余转限价
    #define WD_OrderType_FOK_LMT        7       //fill or kill.全额成交或撤销委托(限价FOK)
    #define WD_OrderType_MTL            8       //market then limit.市价剩余转限价
    #define WD_OrderType_EXE            9       //option exercise 期权行权
                                                //目前，深圳证券支持的方式为　1－5　　　上海证券只支持4、6两种
                                                //上证期权 3、5、7、8、9
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_HedgeType             136     //套保标志
    //套保标志定义
    #define WD_HedgeType_SPEC           '0'      //'0'-投机
    #define WD_HedgeType_HEDG           '1'      //'1'-保值
    #define WD_HedgeType_ARBIT          '2'      //'2'-套利
    //--------------------------------------------------------------------------

#define W_UnderlyingCode        137     //标的券代码     备兑划转是要传入
#define W_UnderlyingName        138     //标的券名称
#define W_UnderlyingType        139     //标的证券类别  'A'-股票; 'D'-ETF基金; 'U'-个股期权

    //--------------------------------------------------------------------------
#define W_UnderlyingChg         140     //标的券划转
    //标的券状态定义
    #define WD_UnderlyingChg_Lock       'B'      //'B'-锁定
    #define WD_UnderlyingChg_UnLock     'S'      //'S'-解锁
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_UnderlyingChgStatus   141     //标的券划转状态
    //标的券划转状态列表
    #define WD_UnderlyingChgStatus_Success       '0'      //'0'-正常
    #define WD_UnderlyingChgStatus_Invalid       '2'      //'2'-无效
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_OptionType            142     //期权类别,行权时调用
    //期权类别定义
    #define WD_OptionType_Call                  'C'      //'C'-认购
    #define WD_OptionType_Put                   'P'      //'P'-认沽
    //--------------------------------------------------------------------------

#define W_OptionSign            143     //期权合约标识

    //--------------------------------------------------------------------------
#define W_OptionHoldingType     144     //期权持仓类别
    #define WD_OptionPositionType_Long          '0'      //'0'-权利仓
    #define WD_OptionPositionType_Short         '1'      //'1'-义务仓
    #define WD_OptionPositionType_Convered      '2'      //'2'-备兑仓
    //--------------------------------------------------------------------------

#define W_Multiplier            145     //合约乘数

#define W_AvailableFund         222     //资金可用
#define W_BalanceFund           223     //资金余额
#define W_SecurityValue         224     //持仓市值资产
#define W_FundAsset             225     //资金资产
#define W_TotalAsset            226     //总资产
#define W_FundFrozen            227     //冻结资金
#define W_OtherFund             228     //其他资金
#define W_BuyFund               229     //今日买入金额
#define W_SellFund              230     //今日卖出金额

#define W_FetchFund             232     //可取资金
#define W_ExerciseMargin        233     //履约保证金
#define W_RealFrozenMarginA     234     //当日开仓预冻结金额
#define W_RealFrozenMarginB     235     //当日开仓预冻结保证金和费用
#define W_HoldingProfit         236     //盯市盈亏
#define W_TotalFloatProfit      237     //总浮动盈亏
#define W_InitRightsBalance     238     //期初客户权益
#define W_CurrRightsBalance     239     //客户权益
#define W_FloatRightsBal        240     //浮动客户权益
#define W_RealDrop              241     //盯市平仓盈亏
#define W_RealDrop_Float        242     //浮动平仓盈亏
#define W_FrozenFare            243     //冻结费用
#define W_CustomerMargin        244     //客户保证金
#define W_RealOpenProfit        245     //盯市开仓盈亏
#define W_FloatOpenProfit       246     //浮动开仓盈亏
#define W_Interest              247     //预计利息
#define W_OrderPremium          248     //委托权利金
#define W_Premium               249     //权利金
#define W_Outflow               250     //出金
#define W_Inflow                251     //入金
#define W_AvailableMargin       252     //保证金可用
#define W_UsedMargin            253     //保证金已用
#define W_LongRealValue         254     //权利方平仓市值
#define W_ShortRealValue        255     //义务方平仓市值

#define W_SecurityBalance       321     //股份余额
#define W_SecurityAvail         322     //股份可用
#define W_SecurityForzen        323     //股份冻结
#define W_TodayBuyVolume        324     //当日买入数
#define W_TodaySellVolume       325     //当日卖出数
#define W_SecurityVolume        326     //当前拥股数
#define W_CallVolume            327     //可申赎数量
#define W_CostPrice             328     //成本价格
#define W_TradingCost           329     //当前成本
#define W_HoldingValue          330     //市值(证券市值)
#define W_FundValue             331     //基金市值

#define W_BeginVolume           331     //期初数量
#define W_EnableVolume          332     //可用数量
#define W_TodayRealVolume       333     //当日可平仓数量
#define W_TodayOpenVolume       334     //当日开仓可用数量

#define W_PreMargin             338     //上交易日保证金

    //--------------------------------------------------------------------------
#define W_OrderStatus           421     //委托状态
    //委托状态列表
    #define WD_OrderStatus_Success       '0'      //'0'-正常
    #define WD_OrderStatus_Cancel        '1'      //'1'-撤单
    #define WD_OrderStatus_Invalid       '2'      //'2'-无效
    #define WD_OrderStatus_OrderProc     '3'      //-处理中
    //--------------------------------------------------------------------------

#define W_TradedVolume          422     //成交数量
#define W_TradedPrice           423     //成交均价
#define W_CancelVolume          424     //撤单数量
#define W_OrderFrozenFund       425     //委托冻结金额
#define W_MadeAmt               426     //成交金额

#define W_TotalFrozenCosts      427     //冻结总费用
#define W_Remark1               428     //说明1
#define W_Remark2               429     //说明2

    //--------------------------------------------------------------------------
#define W_TradedStatus          521     //成交状态
    //成交状态列表
    #define WD_TradedStatus_Success      '0'      //'0'-正常
    #define WD_TradedStatus_Cancel       '1'      //'1'-撤单
    #define WD_TradedStatus_Invalid      '2'      //'2'-无效
    //--------------------------------------------------------------------------

#define W_TradedDate            522     //成交日期
#define W_TradedTime            523     //成交时间
#define W_TradedNumber          524     //成交编号

#define W_AmountPerHand         525     //每手吨数

#define W_DropProfit            527     //平仓盈亏
#define W_DropFloatFrofit       528     //平仓浮动盈亏

#define W_MarketName            621     //市场名称
#define W_DepartmentName        622     //营业部名称
#define W_AvailMarketFlag       623     //可操作市场标识, 按位运算

    //--------------------------------------------------------------------------
#define W_ShareholderStatus     721     //状态(股东状态)
    //状态(股东状态)列表
    #define WD_Status_Normal             '0'      //'0'-正常
    #define WD_Status_Frozen             '1'      //'1'-冻结
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
#define W_MainShareholderFlag   722     //主股东标志
    //主股东标志 列表
    #define WD_MainFlag_Main             '1'      //'1'-主
    #define WD_MainFlag_Sub              '0'      //'0'-副
    //--------------------------------------------------------------------------

#define W_CustomerName          723     //客户姓名

#define	W_BeginDate				801	    //开始日期(YYYYMMDD)
#define W_EndDate				802		//结束日期(YYYYMMDD)

#define W_InfoDate              821     //信息日期
#define W_InfoTime              822     //信息时间
#define W_InfoMsg               823     //信息内容

#define W_DigestName            831     //业务说明
#define W_DigestID			    832     //业务代码
#define W_ClearDate             833     //清算日期
#define W_ClearFund			    834     //清算金额
#define W_StampFee              835     //印花税
#define W_HandlingCharge		836     //手续费
#define W_TransferFee		    837    	//过户费
#define W_TransactionFee        838     //交易规费
#define W_HandlingFee	        839     //经手费
#define W_ManageFee			    840     //证管费
#define W_StdHandlingCharge     841     //标准手续费
#define W_BankAccount           842     //银行账号
#define W_DeliveryDate          843     //交收日期
#define W_TradedTimes			844     //成交笔数
#define W_FrontEndFee			845	    //前台费用
    //--------------------------------------------------------------------------
#define W_TradeType             846     //交易类型
    //交易类型 列表
    #define WD_TradeType_Buy                66       //'B'-买入
	#define WD_TradeType_Sell               83       //'S'-卖出
	#define WD_TradeType_MarginPurchase     111      //'o'-融资买入
    #define WD_TradeType_ShortSales         112      //'p'-融券卖出
	#define WD_TradeType_SwitchInStock      2        //转入股票
	#define WD_TradeType_SwitchOutStock     3        //转出股票
	#define WD_TradeType_RightsIssue        4        //配股
	#define WD_TradeType_Purchase           5        //申购
	#define WD_TradeType_CashDividend       6        //派现金
	#define WD_TradeType_IncomeTax          7        //所得税
	#define WD_TradeType_Deposit            8        //存款
	#define WD_TradeType_Withdraw           9        //取款
	#define WD_TradeType_Interest           10       //利息
	#define WD_TradeType_Loan               11       //贷款
	#define WD_TradeType_Repayment          12       //还贷
	#define WD_TradeType_InterestPursued    13       //还息
	#define WD_TradeType_CommissionRebate   14       //回佣
	#define WD_TradeType_IncidentalExpenses 15       //杂费
	#define WD_TradeType_Commission         16       //佣金
	#define WD_TradeType_ReverseTransaction 17       //冲正
    //--------------------------------------------------------------------------

#define W_BrokerName            921     //券商（期货商）名称
#define W_ConnectModel          922     //连接模式    #0:连接wts 1:直连ctp 真实环境 2:直连ctp 模拟环境
#define W_IsCheckDept           923     //登录时是否必填营业部  0：非必填  1：必填

#define W_ProxyType             1021    //代理类型:  0, 不用代理;  1, SOCKS4;  2, SOCKS4A;  3, SOCKS5;  4, HTTP 1.1
#define W_ProxyHost             1022    //代理服务地址
#define W_ProxyPort             1023    //代理服务端口
#define W_ProxyUserName         1024    //代理服务用户名
#define W_ProxyPassword         1025    //代理服务密码

#define	W_UpdateTime			1121	//数据更新时间



//--------------------------------------------------------------------------
//使用流程说明
/*
    1）WTradeInit
    2）WTradeAuthorize
    3）Logon
                WTradeCreateReq
                WTradeReqSet        //设置各字段（TAG）值
                WTradeSendReqSync   //请求Logon功能

    4）各种业务（委托、撤单、查询）

        发送请求：  WTradeCreateReq
                    WTradeReqSet    //设置各字段（TAG）值
                    WTradeSendReqAsyn（异步） WTradeSendReqSync（同步）

        返回应答：  P_ResCallback回调，或同步获得 ResponseID
                    WTradeRespGet   //获取各字段（TAG）值

    5）Logout
                WTradeCreateReq
                WTradeReqSet        //设置各字段（TAG）值
                WTradeSendReqSync   //请求Logout功能

    6）WTradeAuthQuit
*/

//--------------------------------------------------------------------------
//DLL 接口函数

#ifndef WINDTRADEAPI_EXPORT
    #define WINDTRADEAPI_EXPORT  __declspec(dllexport)
#endif
#ifdef __cplusplus
extern "C"
{
#endif

// 回调函数指针定义
//   使用异步接口时，数据应答返回时回调
//   使用同步接口时，仅在发生网络异常时回调
// 参数：   ResponseID      用于获取应答数据内容的应答流水号
typedef void (WINAPI *ResCallback)(long /*ResponseID*/);

// 函数说明：初始化（设回调函数指针）
// 参数：   OnResponse    回调函数指针  IN
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeInit(ResCallback OnResponse);

// 函数说明：退出
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeAuthQuit();

// 函数说明：Wind产品认证
// 参数：   WindID          Wind账号    IN
//          Password        账号密码    IN
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeAuthorize(const char* WindID, const char* Password);

// 函数说明：创建请求数据包（启动请求流程）
// 返回值：正数，可用请求流水号；其它，创建请求数据包失败
int  WINDTRADEAPI_EXPORT WINAPI WTradeCreateReq();

// 函数说明：向请求数据包中加入字段值
// 参数：   RequestID       创建请求数据包成功时返回的请求流水号    IN
//          Tag             字段代号                                IN
//          Value           字段内容                                IN
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqSetStr(int RequestID, int Tag, const char* Value);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqSetLong(int RequestID, int Tag, long Value);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqSetDouble(int RequestID, int Tag, double Value);

// 函数说明：从请求数据包中获取字段值
// 参数：   RequestID       请求流水号              IN
//          Tag             字段代号                IN
//          OutValue        字段内容                OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqGetStr(int RequestID, int Tag, const char** OutValue);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqGetLong(int RequestID, int Tag, long* OutValue);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeReqGetDouble(int RequestID, int Tag, double* OutValue);

// 函数说明：异步模式发起业务请求（委托、撤单、查询）
// 参数：   RequestID       请求流水号              IN
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeSendReqAsyn(int RequestID);

// 函数说明：同步模式发起业务请求（委托、撤单、查询）
// 参数：   RequestID       请求流水号              IN
//          ResponseID      同步返回的应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeSendReqSync(int RequestID,
                                                  long* ResponseID);

// 函数说明：取应答数据字段值
// 参数：   ResponseID      应答流水号                      IN
//          RecordCount     获取记录下标（回报记录从0开始） IN
//          Tag             字段代号                        IN
//          OutValue        字段内容                        OUT
// 返回值： WD_ERR所列函数返回值定义
// 说明：   无字段内容时，  字符串类型返回空字符串
//                          整数类型返回0x80000000
//                          浮点类型返回NaN
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeRespGetStr(long ResponseID, long RecordCount,
                                                  int Tag, const char** OutValue);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeRespGetLong(long ResponseID, long RecordCount,
                                                  int Tag, long* OutValue);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeRespGetDouble(long ResponseID, long RecordCount,
                                                    int Tag, double* OutValue);

// 函数说明：释放应答数据缓冲区（Response使用后释放内存缓冲区）
// 参数：   ResponseID      应答流水号                      IN
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeBuffRelease(long ResponseID);

// 函数说明：取LogonID FuncID对应的最新一次应答流水号
// 参数：   LogonID         Logon返回的ID代号           IN
//          FuncID          功能号                      IN
//          ResponseID      对应的最新一次应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeGetResponseID(long LogonID, long FuncID, long* ResponseID);

// 函数说明：以WD_ERR所列code号获取对应的错误信息
// 参数：   WD_ERR_code      WD_ERR所列函数返回值定义       IN
// 返回值： 对应的错误信息，输入WD_ERR_code无对应时返回0
WINDTRADEAPI_EXPORT const char* WINAPI WTradeGetErrorMessage(long WD_ERR_code);

// 函数说明：检查应答数据中是否存在某字段内容
// 参数：   ResponseID      应答流水号                      IN
//          RecordCount     获取记录下标（回报记录从0开始） IN
//          Tag             字段代号                        IN
// 返回值： true            有此字段内容
//          false           此字段内容为空
bool WINDTRADEAPI_EXPORT WINAPI WTradeHaveValueInResp(long ResponseID, long RecordCount, int Tag);

// 函数说明：用字符串方式取全部应答数据
// 参数：   ResponseID      应答流水号                  IN
//          OutValue        返回的字符串                OUT
// 返回值： WD_ERR所列函数返回值定义
// 说明：   无内容时，  字符串类型返回空字符串
//          有内容时，  返回全部应答数据组成的字符串
//                      每个TAG组为“TAG数值=TAG对应内容”
//                      以0x02分隔TAG组，以0x03分隔每条记录
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeRespGetOnce(long ResponseID, const char** OutValue);

//--------------------------------------------------------------------------
// 参数化函数

// 函数说明：Logon 登录
// 参数：   BrokerID        券商代码                    IN
//          DepartmentID    营业部代码(期货登录填写0)   IN
//          AccountID       资金账号                    IN
//          Password        账号密码                    IN
//          AccountType     账号类型                    IN
//          RequestID       请求流水号                  OUT
//          ResponseID      同步返回的应答流水号        OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeLogonAsyn(const char* BrokerID,
                                                const char* DepartmentID,
                                                const char* AccountID,
                                                const char* Password,
                                                const long AccountType,
                                                long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeLogonSync(const char* BrokerID,
                                                const char* DepartmentID,
                                                const char* AccountID,
                                                const char* Password,
                                                const long AccountType,
                                                long* ResponseID);

// 函数说明：Logout 登出
// 参数：   LogonID         Logon返回的ID代号           IN
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeLogout(long LogonID);

// 是否已登陆
// 参数：   LogonID         Logon成功后返回的ID代号 IN
// 返回值： true            已登录
//          false           未登录
bool WINDTRADEAPI_EXPORT WINAPI WTradeIsLogon(long LogonID);

// 函数说明：委托下单请求
// 参数：   LogonID         Logon返回的ID代号                   IN
//          MarketType      市场代码                            IN
//          TradeCode       交易代码（证券代码/期货合约代码）   IN
//          OrderVolume     委托数量                            IN
//          OrderPrice      委托价格                            IN
//          TradeSide       交易方向                            IN
//          OrderType       价格委托方式                        IN
//          HedgeType       套保标志 (期货用，证券交易填0)      IN
//          RequestID       请求流水号                          OUT
//          ResponseID      同步返回的应答流水号                OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeOrderAsyn(const long LogonID,
                                              const long MarketType,
                                              const char* TradeCode,
                                              const long OrderVolume,
                                              const double OrderPrice,
                                              const long TradeSide,
                                              const long OrderType,
                                              const long HedgeType,
                                              long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeOrderSync(const long LogonID,
                                              const long MarketType,
                                              const char* TradeCode,
                                              const long OrderVolume,
                                              const double OrderPrice,
                                              const long TradeSide,
                                              const long OrderType,
                                              const long HedgeType,
                                              long* ResponseID);

// 函数说明：撤销委托请求
// 参数：   LogonID         Logon返回的ID代号       IN
//          MarketType      市场代码                IN
//          OrderNumber     委托序号                IN
//          RequestID       请求流水号              OUT
//          ResponseID      同步返回的应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeCancelAsyn(const long LogonID,
                                                const long MarketType,
                                                const char* OrderNumber,
                                                long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeCancelSync(const long LogonID,
                                                const long MarketType,
                                                const char* OrderNumber,
                                                long* ResponseID);

// 函数说明：资金查询请求
// 参数：   LogonID         Logon返回的ID代号       IN
//          RequestID       请求流水号              OUT
//          ResponseID      同步返回的应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryCapitalAccountAsyn(const long LogonID,
                                                      long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryCapitalAccountSync(const long LogonID,
                                                      long* ResponseID);

// 函数说明：持仓查询请求
// 参数：   LogonID         Logon返回的ID代号       IN
//          RequestID       请求流水号              OUT
//          ResponseID      同步返回的应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryPositionAsyn(const long LogonID,
                                                long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryPositionSync(const long LogonID,
                                                long* ResponseID);

// 函数说明：当日委托查询请求
// 参数：   LogonID         Logon返回的ID代号       IN
//          RequestID       请求流水号              OUT
//          ResponseID      同步返回的应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryOrderAsyn(const long LogonID,
                                             long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryOrderSync(const long LogonID,
                                             long* ResponseID);

// 函数说明：当日成交查询请求
// 参数：   LogonID         Logon返回的ID代号       IN
//          RequestID       请求流水号              OUT
//          ResponseID      同步返回的应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryTradeAsyn(const long LogonID,
                                             long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryTradeSync(const long LogonID,
                                             long* ResponseID);

// 函数说明：营业部查询请求
// 参数：   BrokerID        券商（期货商）代码      IN
//          RequestID       请求流水号              OUT
//          ResponseID      同步返回的应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryDepartmentAsyn(const char* BrokerID,
                                                  long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryDepartmentSync(const char* BrokerID,
                                                  long* ResponseID);

// 函数说明：股东查询请求
// 参数：   LogonID         Logon返回的ID代号       IN
//          RequestID       请求流水号              OUT
//          ResponseID      同步返回的应答流水号    OUT
// 返回值： WD_ERR所列函数返回值定义
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryAccountAsyn(const long LogonID,
                                               long* RequestID);
WD_ERR WINDTRADEAPI_EXPORT WINAPI WTradeQryAccountSync(const long LogonID,
                                               long* ResponseID);


//--------------------------------------------------------------------------
// 函数说明：获取版本号
// 参数：
// 返回值： 非0                 版本号字符串
//          0                   获取失败
WINDTRADEAPI_EXPORT const char* WINAPI WTradeGetVersion();

//券商（期货商）BrokerInfo信息结构体
struct BrokerInfo
{
    char    BrokerID[16];   //Broker 代号
    char    BrokerName[64]; //Broker 名称
    short   ConnectModel;   //连接模式    #0:连接wts 1:直连ctp 真实环境 2:直连ctp 模拟环境
    short   IsCheckDept;    //登录时是否必填营业部  0：非必填  1：必填
};
// 函数说明：获取券商（期货商）BrokerInfo信息列表
// 参数：   BrokerInfo_Count    获取的信息记录数量（信息结构体数组包含个数）
// 返回值： 非0                 信息结构体数组首地址
//          0                   无BrokerInfo信息
WINDTRADEAPI_EXPORT  BrokerInfo* WINAPI WTradeGetBrokerInfo(int* BrokerInfo_Count);

//已登录账户信息结构体
struct LogonInfo
{
    long    LogonID;           //Logon返回的ID代号
    char    LogonAccount[32];  //登录代码(账号)
    int     AccountType;       //账号类型
};
// 函数说明：获取已登录账户LogonInfo信息列表
// 参数：   LogonInfo_Count     获取的信息记录数量（信息结构体数组包含个数）
// 返回值： 非0                 信息结构体数组首地址
//          0                   无LogonInfo信息
WINDTRADEAPI_EXPORT  LogonInfo* WINAPI WTradeGetLogonInfo(int* LogonInfo_Count);


//代理服务信息结构体
struct ProxyInfo
{
    int  ProxyType;         //代理类型
    char ProxyHost[32];     //代理服务地址
    int  ProxyPort;         //代理服务端口
    char ProxyUserName[32]; //代理服务用户名
    char ProxyPassword[32]; //代理服务密码
};
// 函数说明：获取代理服务信息
// 参数：   ProxyInfo_out   指向输出信息结构体的指针
void WINDTRADEAPI_EXPORT WINAPI WTradeGetProxyInfo(ProxyInfo* ProxyInfo_out);
// 函数说明：设置代理服务信息
// 参数：   ProxyInfo_in    输入信息结构体
void WINDTRADEAPI_EXPORT WINAPI WTradeSetProxyInfo(const ProxyInfo& ProxyInfo_in);

#ifdef __cplusplus
}
#endif

#endif  //WINDTRADEAPI_H


//业务请求内容&应答内容说明
/*
============================================================================
Logon登录请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号(=WD_FUNCID_LOGON)
W_BrokerID          string      Y           券商代码
W_DepartmentID      string      N           营业部代码(default)
W_LogonAccount      string      Y           登录账号
W_Password          string      Y           账号密码
W_CheckPassword     string      N           验证密码
W_AccountType       int         Y           账号类型

============================================================================
Logon登入返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_LogonID           int         N           登录返回LogonID
----------------------------------------------------------------------------
*/
/*
============================================================================
Logout登出请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号(=WD_FUNCID_LOGOUT)
W_LogonID           int         Y           登录LogonID
============================================================================
Logout登出返回消息
----------------------------------------------------------------------------
无
----------------------------------------------------------------------------
*/
/*
============================================================================
Order委托下单请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号(=WD_FUNCID_ORDER)
W_MarketType        int         Y           市场代码(W_MARKET_SZ    0   //  深圳
                                                     W_MARKET_SH    1   //  上海
                                                     W_MARKET_SZT   2   //  深圳特
                                                     W_MARKET_HK    6   //  港股
                                                     W_MARKET_SPZZ  7   //  商品期货(郑州)
                                                     W_MARKET_SPSH  8   //  商品期货(上海)
                                                     W_MARKET_SPDL  9   //  商品期货(大连)
                                                     W_MARKET_CF    10  //  股指期货)
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_OrderVolume       int         Y           委托数量
W_OrderPrice        double      Y           委托价格
W_TradeSide         int         Y           交易方向
                                                    WD_TradeSide_Buy            '1'     //买入开仓(等同=证券买入)
                                                    WD_TradeSide_Short          '2'     //卖出开仓
                                                    WD_TradeSide_Cover          '3'     //买入平仓
                                                    WD_TradeSide_Sell           '4'     //卖出平仓(等同=证券卖出)
                                                    WD_TradeSide_CoverToday     '5'     //买入平今仓
                                                    WD_TradeSide_SellToday      '6'     //卖出平今仓
                                                    WD_TradeSide_CoveredShort   '7'     //备兑开仓
                                                    WD_TradeSide_CoveredCover   '8'     //备兑平仓
W_OrderType         int         N           价格委托方式
                                                    WD_OrderType_LMT            0       //限价委托
                                                    WD_OrderType_BOC            1       //best of counterparty.对方最优价格委托
                                                    WD_OrderType_BOP            2       //best of party.本方最优价格委托
                                                    WD_OrderType_ITC            3       //immediately then cancel.即时成交剩余撤销
                                                    WD_OrderType_B5TC           4       //best 5 then cancel.最优五档剩余撤销
                                                    WD_OrderType_FOK            5       //fill or kill.全额成交或撤销委托
                                                    WD_OrderType_B5TL           6       //best 5 then limit.最优五档剩余转限价
                                                    WD_OrderType_FOK_LMT        7       //fill or kill.全额成交或撤销委托(限价FOK)
                                                    WD_OrderType_MTL            8       //market then limit.市价剩余转限价
                                                    WD_OrderType_EXE            9       //option exercise 期权行权
                                                    目前，深圳支持的方式为　1－5　　　上海只支持4、6两种"
                                                    上证期权 3、5、7、8、9
----------------------------------------------------------------------------
W_HedgeType         int         N           套保标志  48-'0'-投机  49-'1'-保值 '2'-套利
W_OptionTypde       int         N           期权类别（'C'-认购 'P'-认沽）:行权时调用

============================================================================
委托下单返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
股票返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息

W_OrderNumber       string      N           柜台委托编号
W_SecurityAvail     int         N           证券可用
W_AvailableFund     double      N           资金可用
W_MarketType        string      N           市场代码
W_TradeSide         int         N           交易方向
W_QryType           int         N           扩展标志(价格委托方式：参照同上)
W_SecurityCode      string      N           交易代码（证券代码/期货合约代码/期权代码）
W_OrderVolume       int         N           委托数量
W_OrderPrice        double      N           委托价格
期货返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息

W_OrderNumber       string      N           柜台委托编号
W_MarketType        string      N           市场代码
W_TradeSide         int         N           交易方向
W_SecurityCode      string      N           交易代码（证券代码/期货合约代码/期权代码）
W_OrderVolume       int         N           委托数量
W_OrderPrice        double      N           委托价格
W_HedgeType         int         N           套保标志
上证期权返回应答TAG-----------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息

W_OrderNumber       string      N           柜台委托编号
W_MarketType        string      N           市场代码
W_TradeSide         int         N           交易方向
W_SecurityCode      string      N           交易代码（证券代码/期货合约代码/期权代码）
W_OrderVolume       int         N           委托数量
W_OrderPrice        double      N           委托价格
W_HedgeType         int         N           套保标志
W_Underlying        string      N           标的券代码
----------------------------------------------------------------------------
*/
/*
============================================================================
Cancel撤销委托请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_MarketType        int         Y           市场代码
W_OrderNumber       string      Y           柜台委托编号
============================================================================
撤销委托返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息

W_OrderNumber       string      N           柜台委托编号
----------------------------------------------------------------------------
*/
/*
============================================================================
备兑证券锁定请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_UnderlyingCode    string      Y           标的券代码
W_OrderVolume       int         Y           数量
============================================================================
备兑证券锁定返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_OrderNumber       string      N           柜台委托编号
----------------------------------------------------------------------------
*/
/*
============================================================================
备兑证券解锁请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_UnderlyingCode    string      Y           标的券代码
W_OrderVolume       int         Y           数量
============================================================================
备兑证券解锁返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_OrderNumber       string      N           柜台委托编号
----------------------------------------------------------------------------
*/
/*
============================================================================
资金查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号
============================================================================
资金查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
股票返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_DepartmentID      string      N           营业部ID
W_MoneyType         int         Y           币种类型
W_Remark            string      N           说明
W_AvailableFund     double      Y           资金可用
W_BalanceFund       double      Y           资金余额
W_SecurityValue     double      N           持仓市值资产
W_FundAsset         double      N           资金资产
W_TotalAsset        double      N           总资产
W_Profit            double      N           总盈亏
W_FundFrozen        double      N           冻结资金
W_OtherFund         double      N           其他资金
W_BuyFund           double      N           今日买入金额
W_SellFund          double      N           今日卖出金额
期货返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_MoneyType         int         Y           币种类型
W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_BalanceFund       double      Y           资金余额
W_AvailableFund     double      Y           资金可用
W_Remark            string      N           说明
W_FetchFund         double      Y           可取资金
W_ExerciseMargin    double      Y           履约保证金
W_RealFrozenMarginA double      N           当日开仓预冻结金额
W_RealFrozenMarginB double      N           当日开仓预冻结保证金和费用
W_HoldingProfit     double      N           盯市盈亏
W_TotalFloatProfit  double      N           总浮动盈亏
W_InitRightsBalance double      N           期初客户权益
W_CurrRightsBalance double      N           客户权益
W_FloatRightsBal    double      N           浮动客户权益
W_RealDrop          double      N           盯市平仓盈亏
W_RealDrop_Float    double      N           浮动平仓盈亏
W_FrozenFare        double      N           冻结费用
W_CustomerMargin    double      Y           客户保证金
W_RealOpenProfit    double      N           盯市开仓盈亏
W_FloatOpenProfit   double      N           浮动开仓盈亏
W_Interest          double      N           预计利息
上证期权返回应答TAG---------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_MoneyType         int         Y           币种类型
W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_DepartmentID      string      N           营业部ID
W_Remark            string      N           说明
W_BalanceFund       double      Y           资金余额
W_AvailableFund     double      Y           资金可用
W_FetchFund         double      Y           可取资金
W_ExerciseMargin    double      Y           履约保证金
W_RealFrozenMarginA double      N           当日开仓预冻结金额
W_RealFrozenMarginB double      N           当日开仓预冻结保证金和费用
W_FundFrozen        double      N           冻结资金
W_TotalAsset        double      N           总资产
W_FundAsset         double      N           资金资产
W_HoldingValue      double      N           证券市值
W_FundValue         double      N           基金市值
W_CurrRightsBalance double      N           客户权益
W_CustomerMargin    double      Y           客户保证金
W_OrderPremium      double      Y           委托权利金
W_Premium           double      Y           权利金
W_Outflow           double      Y           当日出金
W_Inflow            double      Y           当日入金
W_FundAsset         double      N           资金资产
W_AvailableMargin   double      Y           保证金可用
W_UsedMargin        double      Y           保证金已用
W_LongRealValue     double      Y           权利方平仓市值
W_ShortRealValue    double      Y           义务方平仓市值
W_HoldingProfit     double      N           盯市盈亏
W_DropProfit        double      N           平仓盈亏
W_HandlingCharge	double      N			手续费
W_Profit            double      N           总盈亏
*/
/*
============================================================================
持仓查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号
============================================================================
持仓查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
股票返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_MarketType        int         Y           证券市场
W_Shareholder       string      Y           股东代码
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_DepartmentID      string      N           所属营业部
W_MoneyType         int         N           币种类型
W_Remark            string      N           说明
W_SecurityBalance   double      Y           股份余额
W_SecurityAvail     double      Y           股份可用
W_SecurityForzen    double      Y           股份冻结
W_TodayBuyVolume    double      N           当日买入数
W_TodaySellVolume   double      N           当日卖出数
W_SecurityVolume    double      N           当前拥股数
W_CallVolume        double      N           可申赎数量
W_CostPrice         double      Y           成本价格
W_TradingCost       double      N           当前成本
W_LastPrice         double      N           最新价格
W_HoldingValue      double      N           市值
W_Profit            double      N           盈亏
期货返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_Shareholder       string      Y           期货账号-同股东代码
W_DepartmentID      string      N           所属营业部
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_MarketType        int         Y           市场代码
W_MoneyType         int         N           币种类型
W_CostPrice         double      Y           成本价格
W_LastPrice         double      N           最新价格
W_TradeSide         int         Y           交易方向
W_BeginVolume       int         N           期初数量
W_EnableVolume      int         Y           可用数量
W_TodayRealVolume   int         N           当日可平仓数量
W_TodayOpenVolume   int         N           当日开仓可用数量
W_HoldingProfit     double      N           盯市盈亏
W_TotalFloatProfit  double      N           持仓浮动盈亏
W_PreMargin         double      N           上交易日保证金
上证期权返回应答TAG---------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_MarketType        int         Y           市场代码
W_MoneyType         int         N           币种类型
W_TradeSide         int         Y           交易方向
W_OptionHoldingType int         N           期权持仓类别
W_OptionType        int         N           期权类别
W_DepartmentID      string      N           所属营业部
W_Shareholder       string      Y           期权交易代码对应的衍生品合约账户-同股东代码
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_UnderlyingCode    string      Y           标的券代码
W_UnderlyingName    string      N           标的券名称
W_OptionSign        string      N           期权合约标识
W_Remark            string      N           说明
W_SecurityForzen    double      Y           股份冻结
W_EnableVolume      int         Y           可用数量
W_TodayOpenVolume   int         N           当日开仓可用数量
W_CostPrice         double      Y           成本价格
W_LastPrice         double      N           最新价格
W_TotalFloatProfit  double      N           持仓浮动盈亏
W_CustomerMargin    double      Y           客户保证金
W_Multiplier        double      N           合约乘数
*/
/*
============================================================================
当日委托查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号
W_OrderNumber       string      N           柜台委托编号(W_QryType=WD_QryType_OrderNumber时）
W_RequestID         int         N           对应的请求ID号（W_QryType=WD_QryType_RequestID时）
============================================================================
当日委托查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
股票返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_MoneyType         int         N           币种类型
W_MarketType        int         Y           证券市场
W_TradeSide         int         Y           交易方向
W_OrderType         int         N           价格委托方式
W_ExtFlag1          int         N           扩展标志
W_ExtFlag2          int         N           扩展标志
W_ExtFlag3          int         N           扩展标志
W_OrderStatus       int         Y           委托状态
W_Shareholder       string      N           股东代码
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_DepartmentID      string      N           所属营业部
W_OrderDate         int         Y           委托日期
W_OrderTime         int         Y           委托时间
W_OrderVolume       int         Y           委托数量
W_OrderPrice        double      Y           委托价格
W_TradedVolume      int         Y           成交数量
W_TradedPrice       double      N           成交均价
W_CancelVolume      int         Y           撤单数量
W_LastPrice         double      N           最新价格
W_OrderNumber       string      Y           柜台委托编号
W_Remark            string      N           说明
W_Seat              string      N           席位号
W_Agent             string      N           代理商号
W_OrderFrozenFund   double      N           委托冻结金额
W_MadeAmt           double      N           成交金额
期货返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_OrderDate         int         Y           委托日期
W_OrderTime         int         Y           委托时间
W_OrderVolume       int         Y           委托数量
W_OrderPrice        double      Y           委托价格
W_TradedVolume      int         Y           成交数量
W_TradedPrice       double      N           成交均价
W_CancelVolume      int         Y           撤单数量
W_LastPrice         double      N           最新价格
W_MarketType        int         Y           市场代码
W_OrderStatus       int         Y           委托状态
W_Shareholder       string      Y           期货账号-同股东代码
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_OrderNumber       string      Y           柜台委托编号
W_Remark            string      N           说明
W_ExtFlag2          int         N           扩展标志
W_ExtFlag3          int         N           扩展标志
W_PreMargin         double      Y           开仓冻结保证金
W_TotalFrozenCosts  double      Y           冻结总费用
W_TradeSide         int         Y           交易方向
W_HedgeType         int         Y           套保标志
W_Seat              string      N           席位号
W_Agent             string      N           代理商号
W_Remark1           string      N           说明1
W_Remark2           string      N           说明2
上证期权返回应答TAG---------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_DepartmentID      string      N           所属营业部
W_Shareholder       string      Y           期权交易代码对应的衍生品合约账户-同股东代码
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_OptionSign        string      N           期权合约标识
W_UnderlyingCode    string      Y           标的券代码
W_UnderlyingName    string      N           标的券名称
W_TradeSide         int         Y           交易方向
W_HedgeType         int         Y           套保标志
W_OptionType        int         N           期权类别（'C'-认购 'P'-认沽）
W_OrderType         int         N           价格委托方式
W_OrderStatus       int         Y           委托状态
W_MoneyType         int         N           币种类型
W_MarketType        int         Y           市场代码
W_OrderNumber       string      Y           柜台委托编号
W_Seat              string      N           席位号
W_Agent             string      N           代理商号
W_Remark            string      N           说明
W_Remark1           string      N           说明1
W_Remark2           string      N           说明2
W_LastPrice         double      N           最新价格
W_OrderPrice        double      Y           委托价格
W_TradedPrice       double      N           成交均价
W_OrderDate         int         Y           委托日期
W_OrderTime         int         Y           委托时间
W_OrderVolume       int         Y           委托数量
W_TradedVolume      int         Y           成交数量
W_CancelVolume      int         Y           撤单数量
W_OrderFrozenFund   double      N           委托冻结金额
W_MadeAmt           double      N           成交金额
*/
/*
============================================================================
当日成交查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号
============================================================================
当日成交查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
股票返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_MoneyType         int         N           币种类型
W_MarketType        int         Y           证券市场
W_TradeSide         int         Y           交易方向
W_ExtFlag1          int         N           扩展标志
W_ExtFlag2          int         N           扩展标志
W_ExtFlag3          int         N           扩展标志
W_TradedStatus      int         Y           成交状态
W_Shareholder       string      N           股东代码
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_OrderDate         int         N           委托日期
W_OrderTime         int         N           委托时间
W_OrderVolume       int         N           委托数量
W_OrderPrice        double      N           委托价格
W_TradedVolume      int         Y           成交数量
W_TradedPrice       double      Y           成交价格
W_CancelVolume      int         N           撤单数量
W_TradedDate        int         Y           成交日期
W_TradedTime        int         Y           成交时间
W_LastPrice         double      N           最新价格
W_OrderNumber       string      Y           柜台委托编号
W_TradedNumber      string      Y           成交编号
W_Remark            string      N           说明
W_Remark1           string      N           其它说明
W_MadeAmt           double      Y           成交金额
期货返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_TradedDate        int         Y           成交日期
W_TradedTime        int         Y           成交时间
W_OrderVolume       int         N           委托数量
W_OrderPrice        double      N           委托价格
W_TradedVolume      int         Y           成交数量
W_TradedPrice       double      Y           成交价格
W_CancelVolume      int         N           撤单数量
W_LastPrice         double      N           最新价格
W_MarketType        int         Y           市场代码
W_ExtFlag1          int         N           扩展标志
W_ExtFlag2          int         N           扩展标志
W_ExtFlag3          int         N           扩展标志
W_TradedStatus      int         Y           成交状态
W_Shareholder       string      Y           期货账号-同股东代码
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_OrderNumber       string      Y           柜台委托编号
W_TradedNumber      string      Y           成交编号
W_Remark            string      N           说明
W_Remark1           string      N           其它说明
W_AmountPerHand     int         N           每手吨数
W_TradeSide         int         Y           交易方向
W_HedgeType         int         Y           套保标志
W_TotalFrozenCosts  double      N           冻结总费用
W_DropProfit        double      N           平仓盈亏
W_DropFloatFrofit   double      N           平仓浮动盈亏
W_Seat              string      N           席位号
W_Agent             string      N           代理商号
上证期权返回应答TAG---------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_DepartmentID      string      N           所属营业部
W_Shareholder       string      Y           期权交易代码对应的衍生品合约账户-同股东代码
W_SecurityCode      string      Y           交易代码（证券代码/期货合约代码/期权代码）
W_SecurityName      string      Y           交易品种名称（证券名称/期货合约名称/期权名称）
W_OptionSign        string      N           期权合约标识
W_UnderlyingCode    string      Y           标的券代码
W_UnderlyingName    string      N           标的券名称
W_TradeSide         int         Y           交易方向
W_TradedNumber      string      Y           成交编号
W_TradedStatus      int         Y           成交状态 '0'-正常 '1'-撤单 '2'-无效
W_OrderNumber       string      Y           柜台委托编号
W_MarketType        int         Y           市场代码
W_MoneyType         int         N           币种类型
W_OptionType        int         N           期权类别（'C'-认购 'P'-认沽）
W_OrderType         int         N           价格委托方式
W_Seat              string      N           席位号
W_Agent             string      N           代理商号
W_Remark            string      N           说明
W_Remark1           string      N           其它说明
W_CancelVolume      int         N           撤单数量
W_TradedVolume      int         Y           成交数量
W_TradedPrice       double      Y           成交价格
W_TradedDate        int         Y           成交日期
W_TradedTime        int         Y           成交时间
W_OrderVolume       int         N           委托数量
W_OrderPrice        double      N           委托价格
W_OrderDate         int         N           委托日期
W_OrderTime         int         N           委托时间
W_LastPrice         double      N           最新价格
W_MadeAmt           double      Y           成交金额
*/
/*
===========================================================================
营业部查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_BrokerID          string      Y           券商代码
============================================================================
营业部查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数
W_DepartmentID      string      Y           营业部ID
W_DepartmentName    string      Y           营业部名称
W_AvailMarketFlag   string      N           可操作市场标识, 按位运算
W_LogonType         string      N           "可登录标识,按位运算
                                                    S－股东代码
                                                    A－资金账号
                                                    B－经纪人号
                                                    C－客户号
                                                    K－卡号
                                                    H－后台账号"
----------------------------------------------------------------------------
*/
/*
===========================================================================
股东查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号
============================================================================
股东查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID               int     Y           登录LogonID
W_FuncID                int     Y           功能号
W_ErrID                 int     Y           错误代号
W_ErrMsg                string  N           错误信息
W_ResponseCount         int     Y           应答的记录条数

W_ShareholderStatus     int     N           状态(股东状态)
W_MainShareholderFlag   int     N           主股东标志
W_AccountType           int     Y           账号类型
W_MarketType            int     Y           市场代码
W_DepartmentID          string  Y           所属营业部
W_Shareholder           string  N           股东代码
W_CustomerName          string  N           客户姓名
W_AssetAccount          string  Y           资金账号
W_Customer              string  Y           客户号
W_Seat                  string  N           席位号
----------------------------------------------------------------------------
*/
/*
===========================================================================
对账单查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号

W_BeginDate		    int	        N           开始日期(YYYYMMDD)
W_EndDate			int 		N           结束日期(YYYYMMDD)
============================================================================
对账单查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
股票返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_RequestID         int         Y           对应的请求ID号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_MoneyType         int         N           币种类型
W_MarketType        int         Y           证券市场
W_TradeType         int         Y           交易类型
W_ExtFlag1          int         N           扩展标志
                                                'A'  回购 //W_TradeType=111 'o'-融资买入(正回购)时:正回购回购
                                                            W_TradeType=112 'p'-融券卖出(逆回购)时:逆回购回购
                                                'B'  购回 //W_TradeType=111 'o'-融资买入(正回购)时:正回购购回
                                                            W_TradeType=112 'p'-融券卖出(逆回购)时:逆回购购回
W_ExtFlag2          int         N           扩展标志
W_ExtFlag3          int         N           扩展标志
W_Shareholder       string      N           股东代码
W_SecurityCode      string      Y           交易代码
W_SecurityName      string      Y           交易品种名称
W_DigestName		string      N           业务说明
W_DigestID			int         N		    业务代码
W_ClearDate			int         Y	        清算日期
W_OrderDate         int         N           委托日期
W_OrderTime         int         N           委托时间
W_OrderVolume       int         N           委托数量
W_OrderPrice        double      N           委托价格
W_TradedVolume      int         Y           成交数量
W_TradedPrice       double      Y           成交价格
W_SecurityBalance   double      Y           股份余额
W_OrderNumber       string      N           柜台委托编号
W_TradedNumber      string      Y           成交编号
W_Remark            string      N           说明
W_Remark1           string      N           其它说明
W_MadeAmt           double      Y           成交金额
W_ClearFund			double      Y           清算金额
W_BalanceFund       double      Y           资金余额
W_StampFee          double      N			印花税
W_HandlingCharge	double      N			手续费
W_TransferFee		double	    N	        过户费
W_TransactionFee    double      N			交易规费
W_HandlingFee	    double      N			经手费
W_ManageFee			double      N	        证管费
期货返回应答TAG-------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_InfoDate          int         Y           信息日期
W_InfoTime          int         Y           信息时间
W_InfoMsg           string      Y           信息内容
----------------------------------------------------------------------------
*/
/*
===========================================================================
交割单查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号

W_BeginDate		    int	        N           开始日期(YYYYMMDD)
W_EndDate			int 		N           结束日期(YYYYMMDD)
============================================================================
交割单查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_Customer          string      Y           客户号
W_AssetAccount      string      Y           资金账号
W_MoneyType         int         N           币种类型
W_MarketType        int         Y           证券市场
W_TradeType         int         Y           交易类型
W_ExtFlag1          int         N           扩展标志
                                                'A'  回购 //W_TradeType=111 'o'-融资买入(正回购)时:正回购回购
                                                            W_TradeType=112 'p'-融券卖出(逆回购)时:逆回购回购
                                                'B'  购回 //W_TradeType=111 'o'-融资买入(正回购)时:正回购购回
                                                            W_TradeType=112 'p'-融券卖出(逆回购)时:逆回购购回
W_ExtFlag2          int         N           扩展标志
W_ExtFlag3          int         N           扩展标志
W_Shareholder       string      N           股东代码
W_SecurityCode      string      Y           交易代码
W_SecurityName      string      Y           交易品种名称
W_DigestName		string      N           业务说明
W_DigestID			int         N		    业务代码
W_ClearDate			int         Y	        清算日期
W_DeliveryDate		int         N			交收日期
W_TradedTime        int         Y           成交时间
W_OrderDate         int         N           委托日期
W_OrderTime         int         N           委托时间
W_OrderVolume       int         N           委托数量
W_OrderPrice        double      N           委托价格
W_TradedVolume      int         Y           成交数量
W_TradedPrice       double      Y           成交价格
W_TradedTimes		int         N		    成交笔数
W_SecurityBalance   double      Y           股份余额
W_OrderNumber       string      N           柜台委托编号
W_TradedNumber      string      Y           成交编号
W_Remark            string      N           说明
W_Remark1           string      N           其它说明
W_MadeAmt           double      Y           成交金额
W_ClearFund			double      Y           清算金额
W_BalanceFund       double      Y           资金余额
W_StampFee          double      N			印花税
W_HandlingCharge	double      N			手续费
W_StdHandlingCharge double      N			标准手续费
W_FrontEndFee		double	    N	        前台费用
W_TransferFee		double	    N	        过户费
W_TransactionFee    double      N			交易规费
W_HandlingFee	    double      N			经手费
W_ManageFee			double      N	        证管费
W_BankCode		    string      N           银行代码
W_BankAccount       string      N           银行账号
----------------------------------------------------------------------------
*/
/*
===========================================================================
备兑股份查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号
============================================================================
备兑股份查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_AssetAccount      string      Y           资金账号
W_Shareholder       string      N           期权交易代码对应的衍生品合约账户-同股东代码
W_UnderlyingCode    string      Y           标的券代码
W_UnderlyingName    string      N           标的券名称
W_DepartmentID      string      N           营业部ID
W_UnderlyingType    int         N           标的证券类别  'A'-股票; 'D'-ETF基金; 'U'-个股期权
W_MoneyType         int         N           币种类型
W_ExtFlag1          int         N           扩展标志
W_Remark            string      N           说明
W_Remark1           string      N           说明1
W_BeginVolume       int         Y           备兑股份昨日余额
W_SecurityBalance   int         Y           备兑股份余额
W_SecurityAvail     int         Y           备兑股份可用
*/
/*
===========================================================================
备兑可划转数量查询查询请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_LogonID           int         Y           登录号

W_UnderlyingCode    string      Y           标的券代码
W_UnderlyingChg     int         Y           标的券划转  
                                                WD_UnderlyingChg_Lock       'B'      //'B'-锁定
                                                WD_UnderlyingChg_UnLock     'S'      //'S'-解锁
============================================================================
备兑可划转数量查询返回消息
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_LogonID           int         Y           登录LogonID
W_FuncID            int         Y           功能号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_MarketType        int         N           市场代码
W_UnderlyingCode    string      N           标的券代码
W_UnderlyingName    string      N           标的券名称
W_Remark            string      N           说明
W_SecurityAvail     int         Y           最大可划转数量
*/
/*
===========================================================================
获取获取券商（期货商）信息请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
============================================================================
券商（期货商）信息返回
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_BrokerID          string      Y           Broker 代号
W_BrokerName        string      Y           Broker 名称
W_ConnectModel      int         Y           连接模式 #0:连接wts 1:直连ctp 真实环境 2:直连ctp 模拟环境
W_IsCheckDept       int         Y           登录时是否必填营业部  0：非必填  1：必填
----------------------------------------------------------------------------
*/
/*
===========================================================================
获取已登录账户信息请求
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
============================================================================
已登录账户信息返回
----------------------------------------------------------------------------
TAG变量名---------类型------是否必填项------说明
----------------------------------------------------------------------------
W_FuncID            int         Y           功能号
W_ErrID             int         Y           错误代号
W_ErrMsg            string      N           错误信息
W_ResponseCount     int         Y           应答的记录条数

W_LogonID           int         Y           登录Logon返回的ID代号
W_LogonAccount      string      Y           登录账号
W_AccountType       int         Y           账号类型
----------------------------------------------------------------------------
*/