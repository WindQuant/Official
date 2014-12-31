%{
功能：根据股价大于5分钟线买入，小于5分钟线卖出。
策略放在Sample16_sub1.m中。当价格变化时就会调用Sample16_sub1.m，给出买卖信号。
用户请在股指期货开盘时运行程序。
主函数分为下面2个部分
A：登录模拟账户
B：设立监控流水模板
C：设定传入策略参数
策略回调函数分为4个部分
A：接收参数
B：策略部分
C：委托查询
D：成交回报进入流水模板
%% 如果不能正常成交则撤单，股票交易被锁死。需要关掉监控界面，然后重新运行程序，目的是避免光大事件。 
%% 点击成交监控中的“退出”按钮，结束整个交易。
第1版  张树德编写     （sdzhang@wind.com.cn）   2013年9月5日
%}
clc;clear
clear global
%% 策略初始化参数
w        =  windmatlab;
secCode  =  'IF1403.CFE';
%% 1. 登录股指期货模拟账户        
% 如果万得用户号是W0813165，那么用户号后面加02（W081316502）就是期货模拟账号。
%                 经纪商  营业部  拟资金账号 资金密码  账号类型 
WindID = inputdlg({'输入Wind账号'},'',1,{''});
if length(WindID)==0;error('请重新输入账号');end
[Data1]= w.tlogon('0000','0',[WindID{1},'02'],'123456', 'CFE');

if Data1{1}<0;errordlg('资金账号错误,请确认是否开通连接外网权限。');assert('');end
%%
%% 3. 建立监控模板
RTMontor= figure('position',[300 500 810 250],...
  'Name','股指期货模拟账户成交监控',...
  'NumberTitle','off', ...
  'Menubar','none',...
  'Toolbar','none');
ColumnName={'代码','名称','买卖方向','成交价格','成交数量','时间'};
Data      ={'','','','','',''};
Data=repmat(Data,8,1);
foregroundColor = [1 1 1];
backgroundColor = [.4 .1 .1; .1 .1 .4];
LineDivision=uitable('Parent',RTMontor,...
  'Position', [25 40 800 200],...
  'ColumnName',ColumnName,...
  'ColumnWidth',{180 100 100 100 100 100},...
  'FontSize',12,...
  'ForegroundColor', foregroundColor,...
  'BackgroundColor', backgroundColor,...
  'Data',Data);
set(LineDivision,'ColumnWidth',{180 100 100 140 140});
uicontrol('Parent',RTMontor,'style','pushbutton','position',[330 10  80 20],'FontSize',12,'value',1,'string','退出','HorizontalAlignment','center','callback','w.cancelRequest(0);close(gcf)'); 
%% 2. 传入策略函数参数
global discQuery Sign RequestID;
discQuery.t1       = 0;
discQuery.t2       = 0;
Sign.Buy           = 1;
Sign.Sell          = 0;
parTrade.signLong  = 1;
parTrade.signShort = 0;
parTrade.pause     = 5;
parTrade.w         = w;
parTrade.Data      =  Data;
parTrade.LineDivision  = LineDivision;
parTrade.Data1         = Data1(:,1); 
parTrade.Timer         = now ;
w.wsq(secCode,'rt_last',@Sample16_sub1,parTrade);
