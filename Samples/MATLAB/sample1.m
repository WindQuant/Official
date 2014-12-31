%  文件名：Sample1.m
%  功能：提取银行间交易债券09付息国债（090007.IB）的净价序列数据，时间从2012-1-1到最新。
w= windmatlab;%  首先创建windmatlab对象；
begintime='20120101';%  然后设置起始时间和截止时间，通过wsd接口提取序列数据
endtime=today;
wdata= w.wsd('090007.IB','close',begintime,endtime,'Priceadj','CP','tradingcalendar','NIB');
w.close
% 下面我们提取000001.SZ的开盘、最高、最低收数据，起始时间前推100天（日期宏），截止时间最新，前复权数据。
% 创建windmatlab对象；
w= windmatlab;
begintime='20120101'; % 设置起始时间和截止时间，通过wsd接口提取序列数据
endtime=today;
wdata= w.wsd('000001.SZ','open,high,low,close','-100d',endtime,'Priceadj','F');
% 其中，-100d是日期宏函数，表示前推100天。
w.close








