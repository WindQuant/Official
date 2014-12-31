%{
 功能：读取交易日期函数
读取日期交易接口的函数有3个，分别为tdays、tdaysoffset、tdayscount。
tdays函数负责读取交易日，tdaysoffset负责计算日期偏移，tdayscount负责统计区间交易日个数。

 Copyright： 2013-2014， 上海万得资讯.   
 修订2013年4月3日。
 参考文献：
1. MATLAB2012a，  fixed-incomne Toolbox 
2.张树德，《金融数量方法教程》，经济科学出版社，2010年8月
3.张树德，《MATLAB金融计算与金融数据处理》，北京航天航空大学出版社，2008年3月
%}


%% 提取上海期货交易所2013年5月3日至6月3日的交易日期
[w_tdays_data,w_tdays_codes,w_tdays_fields,w_tdays_times,w_tdays_errorid,w_tdays_reqid]=w.tdays('2013-05-03','2013-06-03','TradingCalendar=SHFE;')
% 其中，'TradingCalendar=SHFE;'是上海期货交易所代码，默认是上海证券交易所。
%% 提取上海股票交易所2013年6月3日前推4个交易日的日期。
[w_tdays_data,w_tdays_codes,w_tdays_fields,w_tdays_times,w_tdays_errorid,w_tdays_reqid]=w.tdaysoffset(-4,'2013-06-03')
%% 统计上海证券交易所交易日期统计。
[w_tdays_data,w_tdays_codes,w_tdays_fields,w_tdays_times,w_tdays_errorid,w_tdays_reqid]=w.tdayscount('2013-05-03','2013-06-03')








