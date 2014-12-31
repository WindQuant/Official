%{
功能：无风险利率下的资产组合。重点是学会Matlab中组合的Portfolio类的使用方法。
第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
   
参考文献：
1. MATLAB2012a，  financial Toolbox 
2.张树德，《金融数量方法教程》，经济科学出版社，2010年8月
3.张树德，《MATLAB金融计算与金融数据处理》，北京航天航空大学出版社，2008年3月
%}
%% 数据准备
clc;clear;
w=windmatlab
RealEstimateList='000002.SZ,000048.SH,600185.SH'; % 标的股票代码
Field='sec_name'; 
StockList=regexp(RealEstimateList,'[,]','split');
StockList=StockList(:);
for i=1:length(StockList)
     StockList(i,2)=w.wsd(StockList{i},'sec_name','2012-12-31','2012-12-31'); % 读取代码简称
     Price(:,i)    =w.wsd(StockList{i},'close','2012-08-22','2012-12-31');    % 读取价格
end
%% 读取上证指数（000001.SH）交易数据
MarkerIndex=w.wsd('000001.SH','close','2012-08-22','2012-12-31');
%% 读取一年期SHIBOR利率（SHIBOR1Y.IR）数据
CashRet=w.wsd('SHIBOR1Y.IR','close','2012-08-22','2012-12-31');
w.close
%% 计算收益率均值与协方差 
AssetList=StockList(:,2);
AssetList=AssetList;
RetSeries=price2ret(Price);
[AssetMean,AssetCovar]=ewstats(RetSeries);
RetSeries=price2ret(MarkerIndex);
[MarketMean,MarketVar]=ewstats(RetSeries);
[CashMean,CashVar]=ewstats(CashRet/100/225);
mret = MarketMean;      % 市场平均收益
mrsk = sqrt(MarketVar); % 市场收益率的标准差
cret = CashMean;        % 无风险收益率均值
crsk = sqrt(CashVar);   % 无风险收益率的标准差
crsk=0;
%% 创建资产组合对象
p = Portfolio('AssetList', AssetList, 'RiskFreeRate', CashMean);
p = p.setAssetMoments(AssetMean, AssetCovar);
p = p.setInitPort(1/p.NumAssets);
[ersk, eret] = p.estimatePortMoments(p.InitPort);  % 计算组合的风险与收益率
%% 解决资产组合最优问题
p = p.setDefaultConstraints;
pwgt = p.estimateFrontier(20);   % 输出有效前沿上的点的组合。
[prsk, pret] = p.estimatePortMoments(pwgt);% 有效前沿上的风险与收益率
%% 添加资产组合前沿的切线
q = p.setBudget(0, 1);
qwgt = q.estimateFrontier(20);
[qrsk, qret] = q.estimatePortMoments(qwgt);
figure;
portfolioexamples_plot('考虑无风险利率的资产组合有效前沿', ...
 {'line', prsk, pret}, ...
 {'line', qrsk, qret, [], [], 1}, ...
 {'scatter', [mrsk, crsk, ersk], [mret, cret, eret], {'大盘', '无风险利率', '股票平均收益'}}, ...
 {'scatter', sqrt(diag(p.AssetCovar)), p.AssetMean, p.AssetList, '.r'});
xlabel('风险')
ylabel('收益')