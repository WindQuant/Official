function Sample16_sub1(reqid,isfinished,errorid,datas,secCode,fields,times,parTrade)
%{
策略回调函数分为4个部分
A：接收参数
B：策略部分
C：委托查询
D：成交回报
第1版  张树德编写     （sdzhang@wind.com.cn）   2013年9月5日
%}
       %% 1. 接收参数
        global discQuery  Sign RequestID 
        signLong  =  parTrade. signLong;
        signShort =  parTrade.signShort;
        pause     =  parTrade.    pause;
        w         =  parTrade.        w;
        Data      =  parTrade.     Data;
        Data1     =  parTrade.Data1(:,1); % 传回登录号 
        Timer     =  parTrade.     Timer;
        t2.w3     =  datas;
        LineDivision  = parTrade.LineDivision;
       %% 2. 策略部分          
        MA_minte5     =  w.wsi(secCode,'EXPMA',now-3/24/60,now,'EXPMA_N=1');
        MA_minte5     =  MA_minte5(end);  
        if datas>MA_minte5 && Sign.Buy==1
           
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
        discQuery.t2=now;
        end








