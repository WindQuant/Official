% 功能：读取实时价格。当用户在GUI中输入股票代码，点击“确定代码”按钮，该股票的实时价格就显示出来，点击“停止”，则股票停止更新价格。
w= windmatlab;
[w_data1,w_codes,w_fields,w_times,w_errorid,w_reqid]=w.wsq('000005.SZ,000006.SZ,000007.SZ,000008.SZ','rt_time,rt_last,rt_bid1,rt_ask1,rt_vwap');
% 其中，rt_time,rt_last,rt_bid,rt_ask,rt_vwap分别为时间、现价、买入价、卖出价、成交均价。
%% 下面做一个绘制股票价格实时控件图。
global h3 data data1 cellFields 
data1=[];
w_reqid=0;
fields='rt_ask5,rt_ask4,rt_ask3,rt_ask2,rt_ask1,rt_last,rt_bid1,rt_bid2,rt_bid3,rt_bid4,rt_bid5';
cellFields=regexp(fields,'[,]','split');
h=figure('menubar','none','numberTitle','off','name','股票实时价格','position',[400 400 270 300]);
strpath1='str21=get(h2,''string'');w.cancelRequest(w_reqid);'
strpath2=['[~,~,~,~,~,w_reqid]=w.wsq(str21,'];
strpath3='''rt_ask5,rt_ask4,rt_ask3,rt_ask2,rt_ask1,rt_last,rt_bid1,rt_bid2,rt_bid3,rt_bid4,rt_bid5'',@Sample5_sub1);'
strpath=[strpath1,strpath2,strpath3];
h1=uicontrol('position',[20 260 70 30],'string','确定代码','callback',strpath,'FontSize',10);
h2=uicontrol('style','edit','position',[95 260 78 30],'horizontal','left','string','600000.SH','FontSize',10);
h3=uicontrol('style','listbox','position',[20 20 220 230],'FontSize',12,'value',1,'string',[{'用法说明：'};{'输入股票代码后'};{'点击“确定代码”。'};{'点击“停止”中断实时行情'}]); 
h4=uicontrol('position',[180 260 60 30],'string','停止','FontSize',10,'callback','  w.cancelRequest(w_reqid),clc') ;







