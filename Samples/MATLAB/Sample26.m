%{
 功能：计算利率期限结构。
 第1版    张树德（sdzhang@wind.com.cn）   2013年7月5日
 参考文献：
1. MATLAB2012a，  fixed-incomne Toolbox 
2.张树德，《金融数量方法教程》，经济科学出版社，2010年8月
3.张树德，《MATLAB金融计算与金融数据处理》，北京航天航空大学出版社，2008年3月
%}
%% 读取银行间国债品种(国债代码、国债名称、起息日、到期日、付息频率、票息率（当前）)以及日收盘价（2013年4月2日）
clc;clear;
strList='010004.IB,010011.IB,020005.IB,100014.IB,090025.IB,090020.IB,090031.IB,100017.IB,100027.IB,100033.IB';
BondList=regexp(strList,'[,]','split');
BondList=BondList(:);
w=windmatlab
strField=   'fullname,carrydate,maturitydate,interestfrequency,couponrate2';
BondInfo=w.wss(strList,strField);
BondInfo=[BondList,BondInfo];
%% 2 读取债券收盘价（2013年4月2日）
for i=1:length(BondInfo)
    [w_data,w_codes,w_fields,w_times,w_errorid,w_reqid]=w.wsd(BondInfo{i,1},'close','2013-04-02','2013-04-02');   
    if iscell(w_data)~=1
    BondPrice(i,1)=w_data;
    else
    BondPrice(i,1)=nan;    
    end
end
w.close
% 剔除没有成交债券品种
nanPosition=isnan(BondPrice)
BondInfo=BondInfo(~nanPosition,:)
BondPrice=BondPrice(~nanPosition,:)

%% 数据准备
[m,n]=size(BondInfo)
Settle=repmat(datenum('2013-04-02'),m,1)
for i=1:m
Maturity(i,1)=datenum(BondInfo{i,4}) ;   
Period(i,1)=BondInfo{i,5};   
CouponRate(i,1)=BondInfo{i,6}/100;
end
Period=double(Period);
Instruments = [Settle Maturity BondPrice CouponRate];
% 剔除异常数据
abnormDays=find(Maturity<Settle(1))

%% 建立NelsonSiegel模型
%% 建立fitSvensson模型
OptOptions = optimset('lsqnonlin');
OptOptions = optimset(OptOptions,'MaxFunEvals',1000);
% 设定Svensson模型中参数的初值及参数上界与下界
fIRFitOptions = IRFitOptions([7.82 -2.55 -.87 0.45 ],'FitType','durationweightedprice','OptOptions',OptOptions,...
    'LowerBound',[3 -Inf -Inf -Inf ],'UpperBound',[Inf Inf Inf Inf ]);

NSModel = IRFunctionCurve.fitNelsonSiegel('Zero',Settle(1),Instruments,'IRFitOptions',fIRFitOptions,'InstrumentPeriod',Period,'instrumentbasis',3);
NSModel = IRFunctionCurve.fitNelsonSiegel('Zero',Settle(1),Instruments,'InstrumentPeriod',Period,'instrumentbasis',3);


%% 建立fitSvensson模型
OptOptions = optimset('lsqnonlin');
OptOptions = optimset(OptOptions,'MaxFunEvals',1000);
% 设定Svensson模型中参数的初值及参数上界与下界
fIRFitOptions = IRFitOptions([5.82 -2.55 -.87 0.45 3.9 0.44],'FitType','durationweightedprice','OptOptions',OptOptions,...
    'LowerBound',[0 -Inf -Inf -Inf 0 0],'UpperBound',[Inf Inf Inf Inf Inf Inf]);
SvenssonModel = IRFunctionCurve.fitSvensson('Zero',Settle(1),Instruments,'IRFitOptions',fIRFitOptions,...
    'InstrumentPeriod',Period,'instrumentbasis',3);

%% 绘制利率期限结构
PlottingPoints= Settle(1):2000:max(Maturity);
TimeToMaturity = yearfrac(Settle(1),PlottingPoints);
figure 
plot(TimeToMaturity,NSModel.getParYields(PlottingPoints),'-.r')
hold on
plot(TimeToMaturity,SvenssonModel.getParYields(PlottingPoints),'g')
legend({'Nelson Siegel模型','Svensson模型'})
title('利率期限结构')
xlabel('存续期（年）')

