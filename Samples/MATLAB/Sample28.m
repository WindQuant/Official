%{ 
第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
功能：商品期货高频趋势交易策略。标的为大豆、豆油、豆粕、玉米， 跟踪强势领涨品种，适时买入同板块其他待涨的品种，
数据是2013年4月12日的1分钟收盘价，具体策略是计算4个商品中当前价距离盘中最低价涨幅大于3的家数，如果家数超过2家则说明其他品种存在补涨机会，
买入其余的品种，涨幅大于3时卖出，其余收盘前平仓，策略采取滚动操作。
%}
clc
clear
w=windmatlab
% 选取        豆一、     豆油、     豆粕      玉米作为标的。
strStockList='A1309.DCE,Y1309.DCE,M1309.DCE,C1309.DCE'
cellStockList=regexp(strStockList,'[,]','split');
%% 读取历史交易分钟价格
Close=[];
for i=1:length(cellStockList)
[Close_Daily,~,~,DateTime]=w.wsi(cellStockList{i},'close','2013-04-12 00:00:00','2013-04-12  15:30:00 ','barsize','1');
Close=[Close,Close_Daily];
end
minBar=Close(1,:);
HighBar=Close(1,:);
[m,n]=size(Close);
%% 设置交易状态初值
position=[0 0 0 0] ;                            % 记录交易位置， 1表示买入，-1表示卖出，0表示持仓。
position_Buy =logical([1 1 1 1]);               % 记录可买状态， 1表示可以买入，0表示不能买入
position_Sell=logical([0 0 0 0]);               % 记录可买状态， 1表示可以卖出，0表示不能卖出
position_Price=[0 0 0 0];                       % 记录买入价格
for i=2:m
%% 买入条件
minBar=min([minBar;Close(i,:)]);               
maxbar=max([minBar;Close(i,:)]);
sign_Buy=Close(i,:)-minBar>3;                  
conBuy_1=(sum(sign_Buy)>=2)*[1 1 1 1];         % 条件1：涨幅大于3的商品家数大于等于2.
conBuy_2=Close(i,:)-minBar<=5 ;                % 条件2：涨幅小于3品种入选
conBuy_3=position_Buy ;                        % 条件3：当前处于可买状态,1可以买入，0表示不可买入。
common=logical(conBuy_1.*conBuy_2.*conBuy_3);  % 买入条件交集，1表示条件都满足，0表示不能满足所有条件。
position_Buy (common)=logical(0) ;             % 修改可买状态，1表示可以买入，0表示不可买入。
position_Sell(common)=logical(1) ;             % 修改可卖状态，1表示可以卖出，0表示不可卖出。
position(i,:)=1*common  ;                      % 记录买入位置，1表示买入，-1表示卖出，0不变。
position_Price(common)=Close(i,common) ;       % 修改买入价格
%% 卖出条件
conSell_1=logical((Close(i,:)-position_Price>3).*(Close(i,:)-position_Price<1000)) ;% 条件1：收益大于3时卖出
conSell_2=position_Sell   ;                                                         % 条件2：当前处于可卖状态
common=logical(conSell_1.*conSell_2);                                               % 卖出条件交集 
position_Buy (common)=logical(1) ;            % 修改可买状态，1表示可以买入，0表示不可买入。
position_Sell(common)=logical(0) ;            % 修改可卖状态，1表示可以卖出，0表示不可卖出。
position(i,:)=position(i,:)+(-1)*common;      % 记录买入位置，1表示买入，-1表示卖出，0不变。
position_Price(common)=0  ;                   % 清除买入价格
minBar(common)=Close(i,common) ;              % 清除最低价
maxbar(common)=Close(i,common) ;              % 清除最高价
end
%% 买入与卖出配成对，如果最后一次交易只有买入，按收盘价平仓。
sign_pair=sum(position)==1;
sign_pair=sign_pair.*position(end,:)==1;
position(end,logical(sign_pair))=0;
sign_pair=sum(position)==1;
position(end,logical(sign_pair))=-1;
%% 计算收益
Return=nansum(Close.*position)
% % Return =% 
%      5    32   -25   -14
































