%提取平安银行（000001.SZ）当天的买卖盘数据；
%创建windmatlab对象；
w= windmatlab;
%设置起始时间和截止时间，通过wsi接口提取序列数据
begintime=datestr(now,'yyyymmdd 09:30:00');
endtime  =datestr(now,'yyyymmdd HH:MM:SS');
codes='000001.SZ'
%last最新价，amt成交额，volume成交量
%bid1 买1价，bsize1 买1量
%ask1 卖1价, asize1 卖1量
fields='last,bid1,ask1';
[wdata,codes,fields,times,errorid,reqid] = w.wst(codes,fields,begintime,endtime);

%% 读取伦敦铜分钟收盘数据
[data,~,~,times,~,~]=w.wst('CU1309.SHF','last','2013-03-08 09:00:00','2013-03-08 13:00:00')
% 用matlab自带的datestr函数显示日期
datestr(times)



