%{
%  功能：构建资产组合有效前沿。
%  内容：
        A：读取银行板块股票价格。
        B：绘制资产组合的有效前沿。
        C：计算目标收益下的最优权重组合。
        D：计算约束条件下的最优组合。

 第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
参考文献：
1. MATLAB Financial Toolbox 2012a
2.张树德，《金融数量方法教程》，经济科学出版社，2010年8月
3.张树德，《MATLAB金融计算与金融数据处理》，北京航天航空大学出版社，2008年3月

%}

%% 股票列表（银行板块）及价格起止日期 
strList='002142.SZ,601166.SH,601169.SH,601288.SH,601328.SH,601398.SH,601818.SH,601939.SH,601988.SH,601998.SH';
StockList=regexp(strList,'[,]','split');
StockList=StockList(:);
BeginTime='2013-01-01';
EndTime='2013-04-01';

%% 读取股票价格列表
w=windmatlab;
t=1;
for i=1:length(StockList)    
    [wdata,wcodes,wfields,wtimes,werrorid,wreqid]=w.wsd(StockList{i},'close',BeginTime,EndTime);    
    if werrorid==107;error('数据请求错误');end
     matPrice(:,i)=wdata;
end
    matTime=datenum(wtimes);
    matPrice=[matTime,matPrice];
    w.close;
%% 计算价格序列的日收益率
RetSeries=price2ret(matPrice(:,2:end));
%% 绘制资产组合有效前沿图
[ExpReturn,ExpCovariance]=ewstats(RetSeries);
frontcon(ExpReturn*225,ExpCovariance, 20);
title('资产组合有效前沿');
xlabel('风险（标准差）');
ylabel('收益');
%% 读取给定目标收益率（5%/年）的组合
% 如果要求目标
retTarget=0.05;
[PortRisk, PortReturn, PortWts] =frontcon(ExpReturn*225,ExpCovariance,[],retTarget);
% PortWts =[0 0 0 0 0 0 0.1430 0 0.5813 0.2757];
cellWeight=num2cell(PortWts');
cellWeight_1=[StockList,cellWeight];
cellWeight_2=[{'股票代码','权重'};cellWeight_1];
disp('目标收益率为0.05/年的组合权重');
disp(cellWeight_2)











