%{
 功能：计算债券的久期与凸度。
 第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
 参考文献：
1. MATLAB2012a，  fixed-incomne Toolbox 
2.张树德，《金融数量方法教程》，经济科学出版社，2010年8月
3.张树德，《MATLAB金融计算与金融数据处理》，北京航天航空大学出版社，2008年3月
%}
clc;clear;
%% 读取国债品种特征(国债代码、国债名称、起息日、到期日、付息频率、票息率（当前）)与国债行情（2013年4月2日）。
strList='010004.IB,010011.IB';
strList='010004.IB,010011.IB,020005.IB,100014.IB,090025.IB,090020.IB,090031.IB,100017.IB,100027.IB,100033.IB'
strField=   'fullname,carrydate,maturitydate,interestfrequency,couponrate2';
BondList=regexp(strList,'[,]','split');
BondList=BondList(:);
w=windmatlab
BondInfo=w.wss(strList,strField);
BondInfo=[BondList,BondInfo];
for i=1:length(BondList)
    w_data=w.wsd(BondInfo{i,1},'close','2013-04-02','2013-04-02');   
    if iscell(w_data)~=1
    BondPrice(i,1)=w_data;
    else
    BondPrice(i,1)=nan;    
    end
end
w.close
%% 数据准备
[m,n]=size(BondInfo)
Settle=repmat(datenum('2013-04-02'),m,1)
for i=1:m
Maturity(i,1)=datenum(BondInfo{i,4}) ;   
Period(i,1)=BondInfo{i,5};   
CouponRate(i,1)=BondInfo{i,6}/100;
Face(i,1)=1000
end
Periods=double(Period);
Instruments = [Settle Maturity BondPrice CouponRate];
%% 根据债券价格计算久期与凸度
[ModDuration, YearDuration] = bnddurp(BondPrice, CouponRate,Settle, Maturity ,3, Periods); 
YearConvexity = bndconvp(BondPrice, CouponRate,Settle, Maturity ,3, Periods);
%% 输出
Result=num2cell([YearDuration,ModDuration,YearConvexity])
Result=[BondInfo(:,1:2),Result];
Result=[{'债券代码','债券名称','久期','修正久期','凸度'};Result]
%% 计算全价=净价+应计利息
Yields = bndyield(BondPrice, CouponRate, Settle, Maturity)
[CleanPrice, AccruedInterest] = bndprice(Yields,CouponRate,Settle, Maturity, Periods);
Prices  =  CleanPrice + AccruedInterest;
%% 债券数量(每个债券投资1百万元)
BondAmounts = 1000000./Prices
dy = -0.1:0.005:0.05;               % 设定未来收益率变化范围
% % D  = datevec(Settle);                % 得到日期分量
% % dt = datenum(D(1):2014, D(2), D(3));
% dt=datenum(Settle(1)):datenum(Settle(1))+1000
% 设定日期变动
dt=repmat(datenum('2013-04-02'),10,1);
[dT, dY]  =  meshgrid(dt, dy); % 加密
NumTimes  =  length(dt);       % 计算步数
NumYields =  length(dy);       % 收益率变化数Number of yield changes
NumBonds  =  length(Maturity); % 债券品种
Prices = zeros(NumTimes*NumYields, NumBonds);
%% 计算不同到期收益率不同到期日下的债券价格
for i = 1:NumBonds
   [CleanPrice, AccruedInterest] = bndprice(Yields(i)+... 
   dY(:), CouponRate(i), dT(:), Maturity(i), Periods(i),...
   [], [], [], [], [], [], Face(i));
   Prices(:,i) = CleanPrice + AccruedInterest;
end
Prices = Prices * BondAmounts;
Prices = reshape(Prices, NumYields, NumTimes);
figure                   % 增加一个新窗口
surf(dt, dy, Prices)     % 绘图
hold on                  % 增加当前估值Add the current value for reference
basemesh = mesh(dt, dy, 100000*ones(NumYields, NumTimes));
set(basemesh, 'facecolor', 'none');
set(basemesh, 'edgecolor', 'm');
set(gca, 'box', 'on');
dateaxis('x', 11);
xlabel('评估日');
ylabel('利率变化');
zlabel('组合市值');
hold off
view(-25,25);