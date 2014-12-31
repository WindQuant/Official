%% R-Breaker日内（非高频）交易策略
%{
 第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
R-Breaker是个经典的具有长生命周期的日内模型，曾14年排名Future Trust杂志年度前10最赚钱的策略。 
该策略包含突破与反转二种策略，
主要的思想：
根据前一个交易日的收盘价、最高价和最低价数据通过一定方式计算出六个价位，从大到小依次为：
观察卖出价(Ssetup):昨高+0.35*(昨收-昨低);           //ssetup
反转卖出价(Senter):(1.07/2)*(昨高+昨低)-0.07*昨低;  //senter
反转买入价(Benter):(1.07/2)*(昨高+昨低)-0.07*昨高;  //benter
观察买入价(Bsetup):昨低-0.35*(昨高-昨收);           //bsetup
突破买入价（Bbreak):(观察卖出价+0.25*(观察卖出价-观察买入价)); //bbreeak
突破卖出价(Sbreak):观察买入价-0.25*(观察卖出价-观察买入价);    //sbreak
反转策略:
持多单，当日内最高价超过观察卖出价(Ssetup)后，盘中价格出现回落，且进一步跌破反转卖出价（Senter）构成的支撑线时，采取反转策略，即在该点位反手做空；
持空单，当日内最低价低于观察买入价(Bsetup）后，盘中价格出现反弹，且进一步超过反转买入价(Benter)构成的阻力线时，采取反转策略，即在该点位反手做多；
突破策略:
在空仓的情况下，如果盘中价格超过突破买入价，则采取趋势策略，即在该点位开仓做多；
在空仓的情况下，如果盘中价格跌破突破卖出价，则采取趋势策略，即在该点位开仓做空；
资料来源
1、http://www.yafco.com/show.php?contentid=261740
%}
clc
clear
w=windmatlab
% 豆粕（M1309.DCE）作为标的。
strStockList='M1309.DCE';
[w_wsd_data]=w.wsd(strStockList,'open,high,low,close','2013-04-11','2013-04-11');
Open=w_wsd_data(1);
High=w_wsd_data(2);
Low=w_wsd_data(3);
Close=w_wsd_data(4);
%% 读取4月12日的分钟价格
[iPrice]=w.wsi(strStockList,'high,low,close','2013-04-12 09:00:00','2013-04-12  15:30:00 ','barsize','1');
%% 计算出6个价位。
Bsetup=Low-0.35*(High-Close);              % 观察买入价
Ssetup=High+0.35*(Close-Low);              % 观察卖出价
Benter=(1.07/2)*(High+Low)-0.07*High;      % 反转买入价
Senter=(1.07/2)*(High+Low)-0.07*Low;       % 反转卖出价
Bbreak=Ssetup+0.25*(Ssetup-Bsetup);        % 突破买入价
Sbreak=Bsetup-0.25*(Ssetup-Bsetup);        % 突破卖出价
%% 策略初值
holding=0;     % 1表示做多，-1表示做空，0表示没有操作。
hi=iPrice(1,1);
lo=iPrice(1,2);
con_3=0;
con_4=0;
for i=1:size(iPrice,1)
hi=max(iPrice(i,1),hi);
lo=min(iPrice(i,2),lo);
c=iPrice(i,3) ; 
con_1=c>Bbreak && sum(holding)==0;  % 空仓做多
con_2=c<Sbreak && sum(holding)==0;  % 空仓做空    
if hi>Ssetup ;con_3==1;end        
if lo<Bsetup ;con_4==1;end        
%% 交易
if con_3==1      &&  sum(holding)==1&& c<Senter      % 多单反转
    holding(i,1)=-1;    
elseif con_2==1                                      % 空仓开空 
    holding(i,1)=-1;        
elseif con_4==1  && sum(holding)==-1  && c>Benter    % 空单反转 
    holding(i,1)=1;         
elseif  con_1==1                                     % 空仓做多 
    holding(i,1)=1;     
else
    holding(i,1)=0;
end
end
%% 尾盘轧平
if sum(holding)==1&&holding(end)==1
holding(end)=0;
elseif sum(holding)==1
holding(end)=-1;
elseif sum(holding)==-1 && holding(end)==-1
holding(end)=0;
elseif sum(holding)==-1
holding(end)=1;
else
end
%% 统计并显示收益
disp('R-Break策略收益')
Return=nansum(iPrice(:,3).*holding)





