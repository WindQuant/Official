%{
功能：沪深300指数三因子模型择时策略。
Fama的三因子认为影响股价主要取决于下面3个因子。
A：市场超额收益率（RMT）
B：规模因子（SMB）
C：账面市值比（HML）
本策略目标是根据Fama三因子模型构建大盘小盘风格轮动策略。
第1版  张树德编写（sdzhang@wind.com.cn）   2013年9月5日
参考：
蒋瑛琨，国泰君安证券股份有限公司，多因子选股模型之因子分析与筛选Ⅰ:估值与财务成长类指标——数量化研究系列之十七。
%}


clc;clear;
w=windmatlab
%% 1.基本参数设置
% 时间区间为2013年1月1日至2013年6月1日
BeginDate  = '2013-06-01';
EndDate    = '2013-09-12';
%% 1.数据准备
%% 1.1  证指数收益
close_index_000001_SH=w.wsd('000300.SH','close',BeginDate,EndDate);
%% 1.2  银行间1天回购利率（R001.IB
rf_R001_IB=w.wsd('R001.IB','close',BeginDate,EndDate,'Fill=Previous');
%% 1.3  取得沪深300成分股代码
IndexConstituent_000300_SH=w.wset('IndexConstituent','date=20130912;windcode=000300.SH');
%% 1.4  提取收盘价（Close）、总流通股（free_float_shares）、总市值(mkt_cap)
for i=1:size(IndexConstituent_000300_SH,1)
Data              =  w.wsd(IndexConstituent_000300_SH(i,2),'close,free_float_shares,mkt_cap',BeginDate,EndDate,'Fill=Previous');
daily.close(:,i)  =  Data(:,1);
daily.free_float_shares(:,i)  =  Data(:,2);
daily.mkt_cap(:,i)            =  Data(:,3);
end
daily.free_float_shares=daily.free_float_shares';
%% 1.5   提取中报股东权益(净利润)
ShareholdersRight =  w.wss(IndexConstituent_000300_SH(:,2),'tot_equity','rptDate=20130630','rptType=1');
%% 2.  数据加工
%% 2.1 计算股票超额收益率
ret_close=price2ret(daily.close);
%% 2.2 计算市场超额收益率
ret_close_index_000001_SH=price2ret(close_index_000001_SH);
market_Premium=ret_close_index_000001_SH-log(1+rf_R001_IB(2:end)/100)/252;
%% 2.2 计算市账比
HML=daily.mkt_cap'./repmat(ShareholdersRight,1,71);
%% 2.3 计算规模
% 由于个股规模因子相差较大，我们用1表示小的流通市值，2表示大的流通市值。
% 自由流通股大于为1，小于为-1。
free_float_shares=daily.free_float_shares;
free_float_shares(free_float_shares<=8.2538e+08)=1;
free_float_shares(free_float_shares> 8.2538e+08)=2;
%% 2.2 对每天的收益情况回归(允许缺省值的回归用ecmmvnrmle)。
Par=[];
for i=1:70
x=[free_float_shares(:,i+1),HML(:,i+1)];
y=ret_close(i,:)'-ones(300,1)*market_Premium(i);
a= ecmmvnrmle(y,x);
Par=[Par;a'];
end
%% 3. 系数的自回归
AutoOPar1=parcorr(Par(:,1));
AutoOPar2=parcorr(Par(:,2));
%% 定义选股规则
% A：如果a(1)>0时,明天选择大盘股。如果a(1)<0时，明天选择小盘股。
%% 3.1 回测。
ret_close1=ret_close';
for i=3:71
N=free_float_shares(:,i-1)==2 ;  
if Par(i-1,1)>0   
   ret_backTest(i-2)=mean(ret_close1(N,i-1));
else
   ret_backTest(i-2)=mean(ret_close1(~N,i-1)) ;   
end   
end
%% 3.2 计算累计收益率
ret_backTest=cumsum(ret_backTest);
ret_HS300=log([close_index_000001_SH(3:end)/close_index_000001_SH(2)]);
plot([ret_backTest',ret_HS300],'LineWidth',4);
legend('大盘小盘选择组合','沪深300');
title('大盘小盘择时组合与沪深300指数走势对比');













