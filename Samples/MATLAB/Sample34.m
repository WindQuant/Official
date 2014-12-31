function Sample34
%{
名称：定时器控制模拟交易例子
功能：本策略是大于1分钟K线买入，小于1分钟K线卖出。
策略放在Sample35_sub1.m中，当价格变化时就会调用Sample35_sub.m，给出买卖信号。
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
uicontrol('Parent',RTMontor,'style','pushbutton','position',[330 10  80 20],'FontSize',12,'value',1,'string','退出','HorizontalAlignment','center','callback','close(gcf)'); 
%% 2. 传入策略函数参数
global discQuery Sign RequestID parTrade Profit;
discQuery.t1       = 0;
discQuery.t2       = 0;
Sign.Buy           = 1;
Sign.Sell          = 0;
parTrade.signLong  = 1;
parTrade.signShort = 0;
parTrade.pause     = 5;
parTrade.Data      =  Data;
parTrade.LineDivision  = LineDivision;
parTrade.Data1         = Data1(:,1); 
parTrade.Timer         = now ;
Profit=0;
%% 设置定时器，每隔1秒钟运行一次策略
Timer1=timer('TimerFcn',{@Sample35_sub,secCode,parTrade,w},'period',1,'ExecutionMode','fixedspacing','ErrorFcn',{@Sample35_sub,secCode,parTrade,w});
start(Timer1);% 启动定时器
function Sample35_sub(object,event,secCode,parTrade,w)
%{
策略回调函数分为4个部分
A：接收参数
B：策略部分
C：委托查询
D：成交回报
第1版  张树德编写     （sdzhang@wind.com.cn）   2013年9月5日
%}
       %% 1. 接收参数
        datas=w.wsq(secCode,'rt_last');
        global discQuery  Sign RequestID Profit;
        signLong  =  parTrade. signLong;
        signShort =  parTrade.signShort;
        pause     =  parTrade.    pause;
        Data      =  parTrade.     Data;
        Data1     =  parTrade.Data1(:,1); % 传回登录号 
        Timer     =  parTrade.     Timer;
        t2.w3     =  datas;
        LineDivision  = parTrade.LineDivision;
       %% 2. 策略部分 
        MA_minte5     =  w.wsi(secCode,'EXPMA',now-3/24/60,now,'EXPMA_N=1');
        MA_minte5     =  MA_minte5(end); 
        if  datas>MA_minte5 && Sign.Buy==1
       %% 2.1 买入1手       
        [RequestID]   = w.torder(secCode, 'Buy', datas+10, 1, 'OrderType=LMT;HedgeType=SPEC','LogonID',Data1{1}) ;
        Sign.Buy      = 0;
        Sign.Sell     = 0;
        discQuery.t1  = now;
        elseif datas<MA_minte5 && Sign.Sell==1
       %% 2.2 卖出1手
        [RequestID]   = w.torder(secCode, 'Sell', datas-10, 1, 'OrderType=LMT;HedgeType=SPEC','LogonID',Data1{1})  ;
        Sign.Buy      = 0;
        Sign.Sell     = 0;                    
        discQuery.t1  = now ;                  
        else
        end     
       %% 3. 查询委托与成交
        if   Sign.Buy==0&&Sign.Sell==0 && now-discQuery.t1>5*1/24/60/60     
            [Data4, we]=w.tquery('Order','LogonID',Data1{1},'RequestID',RequestID{1}) ;
            [Data14,we]=w.tquery('Trade','LogonID',Data1{1},'RequestID',RequestID{1},'OrderNumber',Data4(1)) ;
            if       strcmpi(Data14{3},'Normal')==1      &&  strcmpi(Data14{6},'Buy')==1 && Data14{11}==1  
            Sign.Sell=1;
            elseif   strcmpi(Data14{3},'Normal')==1      &&  strcmpi(Data14{6},'Sell')==1  && Data14{11}==1
            Sign.Buy=1;
            else 
           %% 如果不能正常撤单，股票交易被锁死。需要关掉监控界面，然后重新运行程序，目的是避免光大事件。     
            [Data5,Fields,ErrorCode]=w.tcancel(Data4{1}, 'LogonID',RequestID{1});  
            end           
        end  
       %% 5. 统计成交(5秒钟查询一次)
        if  now-discQuery.t1>5*1/24/60/60            
        % 成交序号 Wind代码 交易方向  成交价格 成交数量        
        Data6=w.tquery('order', 'LogonId',Data1{1})  ;    
        if size(Data6,2)>=8
            datenum(Data6(:,8));
            N=find(datenum(Data6(:,8))>Timer);
            Data9=Data6(:,8);
            for i=1:size(Data6,1)   
            Data6{i,9}= sprintf('%6.2f\n',Data6{i,9}) ;
            Data9{i,1}= Data6{i,8}(12:end);
            end        
            Data7 = Data6(N,[3 4 5 9 7 8]);
            Data7(:,end)=Data9(N,:) ;          
                if length(N)>=8
                Data7 = Data7(end:-1:1,:) ;
                else
                numLength=length(N);
                Data7=[Data7(end:-1:1,:);Data(3:8-numLength,:)] ;
                end
            set(LineDivision,'data',Data7);    
        end
        discQuery.t2=now;
        end