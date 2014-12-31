%{
功能：读取股票数据并绘制股票日K线图
第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
修订：2013年8月5日。
参考：
1.MATLAB Financial Toolbox 2012a
2.张树德，《金融数量方法教程》，经济科学出版社，2010年8月
3.张树德，《MATLAB金融计算与金融数据处理》，北京航天航空大学出版社，2008年3月
%}
%% 读取股票品种
clc;clear
StockList='600276.SH'   % 选择恒瑞医药
%% 读取债券收盘价（2013年4月2日）
w=windmatlab
[w_data,w_codes,w_fields,w_times,w_errorid,w_reqid]=w.wsd(StockList,'open,low,high,close','2013-01-02','2013-04-02');   
fintsPrice=fints(w_times,w_data,{'Open','High','Low','Close'},1)
fintsPrice.desc='恒瑞医药的股票日K线价格'
%% 时间序列绘图（棒图、）
% 绘制日K线图
candle(fintsPrice)
title('恒瑞医药股价日K线图')
% 绘制日K线高低图
figure
highlow(fintsPrice)
title('恒瑞医药股价日K线高低图')
% 棒图
figure
bar(fintsPrice)
title('恒瑞医药股价K线棒图')
% 绘制股票价格的梯状图
figure
kagi([w_times,w_data(:,end)])
title('恒瑞医药股价梯状图')
% 黑白图
figure
renko([w_times,w_data(:,end)])  
title('恒瑞医药股价黑白图')
% 绘制折线图
figure
plot(fintsPrice)
title('恒瑞医药股价折线图')

