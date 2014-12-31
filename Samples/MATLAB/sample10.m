%{
第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
功能：读取数据集。
目前可以读取板块成分、指数成分股及权重、ETF申赎成分信息、分级基金明细、融资标的、融券标的、融资融券担保品、回购担保品、停牌股票、分红送转等股票数据。
%}
% 读取HS300成分股指数，日期为2013年6月3日，为自然日。
choice = questdlg({'本程序运行时间大于20分钟;','要继续运行吗?'},'运行提示','运行','不运行','运行')
if strcmpi(choice,'不运行')==1,return;end

w=windmatlab
[w_wset_data,w_wset_codes,w_wset_fields,w_wset_times,w_wset_errorid,w_wset_reqid]=w.wset('IndexConstituent','date=20130603;windcode=000300.SH')
%%  读取沪深2市融资融券标的余额与资金流入流出统计。
% 变量说明：
% %%　融资标的代码
% w_wset_data1    融资标的代码
% w_wset_data2    融券标的代码
% %%  融资余额统计
% MarginBuy1;% 融资买入额
% MarginBuy2;% 融资偿还额
% MarginBuy3;% 融资余额
% %%  融券余额统计
% MarginSell1;% 融券卖出量
% MarginSell2;% 融券偿还量
% MarginSell3;% 融券余量
% MarginSell4;% 融券余额
% %% 融资品种资金流入统计
% buyCash1;% 净流入资金
% buyCash2;% 净流入量
% buyCash3;% 金额流入率
% buyCash4;% 资金流向占比
% buyCash5;% 尾盘净流入资金
% buyCash6;% 开盘净流入资金
% %% 融券品种资金流入统计
% SellCash1;% 净流入资金
% SellCash2;% 净流入量
% SellCash3;% 金额流入率
% SellCash4;% 资金流向占比
% SellCash5;% 尾盘净流入资金
% SellCash6;% 开盘净流入资金
%% 0. 目录
clear
w=windmatlab;
% 开始时间与结束时间
BeginDay = '2013-10-01'
EndDay   = '2013-11-19'
%% 1. 融资标的统计
%% 1.1 读取融资标的
% 读取融资标的
[w_wset_data1]=w.wset('MarginTradingUnderlying','date=20131001');
%% 1.2 融资标的余额统计
MarginBuy1=[];% 融资买入额
MarginBuy2=[];% 融资偿还额
MarginBuy3=[];% 融资余额
for i=1:length(w_wset_data1)
data=w.wsd(w_wset_data1{i,2},'mrg_long_amt,mrg_long_repay,mrg_long_bal',BeginDay,EndDay);
if iscell(data)==1&&isnan(data{1})==1;data=cell2mat(data);end
MarginBuy1=[MarginBuy1,data(:,1)];  
MarginBuy2=[MarginBuy2,data(:,2)];  
MarginBuy3=[MarginBuy3,data(:,3)];  
end
%% 1.3 融资标的资金流向统计
buyCash1=[];% 净流入资金
buyCash2=[];% 净流入量
buyCash3=[];% 金额流入率
buyCash4=[];% 资金流向占比
buyCash5=[];% 尾盘净流入资金
buyCash6=[];% 开盘净流入资金
for i=1:length(w_wset_data1)
data=w.wsd(w_wset_data1{i,2},'mf_amt,mf_vol,mf_amt_ratio,mf_vol_ratio,mf_amt_close,mf_amt_open',BeginDay,EndDay);
if iscell(data)==1&&isnan(data{1})==1;data=cell2mat(data);end
buyCash1=[buyCash1,data(:,1)];  
buyCash2=[buyCash1,data(:,2)]; 
buyCash3=[buyCash1,data(:,3)]; 
buyCash4=[buyCash1,data(:,4)]; 
buyCash5=[buyCash1,data(:,5)]; 
buyCash6=[buyCash1,data(:,6)]; 
end
%% 2. 融券标的统计

%% 2.1 读取融券标的
[w_wset_data2]=w.wset('ShortSellingUnderlying','date=20130530');
%% 2.2 融券标的余额统计
MarginSell1=[];% 融券卖出量
MarginSell2=[];% 融券偿还量
MarginSell3=[];% 融券余量
MarginSell4=[];% 融券余额
for i=1:length(w_wset_data2)
data=w.wsd(w_wset_data2{i,2},'mrg_short_vol,mrg_short_vol_repay,mrg_short_vol_bal,mrg_short_bal,',BeginDay,EndDay);
if iscell(data)==1&&isnan(data{1})==1;data=cell2mat(data);end 
MarginSell1=[MarginSell1,data(:,1)];
MarginSell2=[MarginSell2,data(:,2)];
MarginSell3=[MarginSell3,data(:,3)];
MarginSell4=[MarginSell4,data(:,4)];  
end
%% 2.3 融券余额资金流向统计
SellCash1=[];% 净流入资金
SellCash2=[];% 净流入量
SellCash3=[];% 金额流入率
SellCash4=[];% 资金流向占比
SellCash5=[];% 尾盘净流入资金
SellCash6=[];% 开盘净流入资金
for i=1:length(w_wset_data2)
data=w.wsd(w_wset_data2{i,2},'mf_amt,mf_vol,mf_amt_ratio,mf_vol_ratio,mf_amt_close,mf_amt_open',BeginDay,EndDay);
if iscell(data)==1&&isnan(data{1})==1;data=cell2mat(data);end    ;
SellCash1=[SellCash1,data(:,1)];  
SellCash2=[SellCash1,data(:,2)]; 
SellCash3=[SellCash1,data(:,3)]; 
SellCash4=[SellCash1,data(:,4)]; 
SellCash5=[SellCash1,data(:,5)]; 
SellCash6=[SellCash1,data(:,6)]; 
end

















