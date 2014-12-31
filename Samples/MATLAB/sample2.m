% Sample2.m
% 提取中金所IF股指期货当月连续合约的3分钟数据，截止时间最新（now），起始时间前推100天（now-100）；
% 创建windmatlab对象；
w= windmatlab;
%设置起始时间和截止时间，通过wsi接口提取序列数据
codes='IF00.CFE';
fields='open,high,low,close';
begintime=now-100;
endtime=now
wdata= w.wsi(codes,fields,begintime,endtime,'BarSize','3');
% 其中now是matlab内置的日期函数，表示当前时刻。
