classdef windmatlab
%   WindMATLAB is an addin tool to retrieve financial data from Wind Information.
%       w is the default data object created using the shortcut menu.
%       If you got this object, you can call its functions to retrieve finacial data;
%       otherwise please connect Wind for technical support.
%
%   WindMATLAB Methods:
%
%   Constructor:
%       windmatlab      - construct a wind data object. 
%
%   Data Retrieve Functions:
%       wsd             - retrieve Wind daily data
%       wsi             - retrieve Wind inter-day data
%       wst             - retrieve Wind tick data
%       wss             - retrieve Wind snapshot data
%       wsq             - retrieve Wind quote data
%       wsqtd           - retrieve Wind quote data by td
%       wset            - retrieve some base info about market
%       weqs            - retrieve stocks by custom filter
%       wpf             - retrive AMS & PMS info
%       wupf            - upload PMS info
%       tdays           - retrieve valid days between two days
%       tdaysoffset     - retrieve a day based on the input day
%       tdayscount      - retrieve duration between two days
%       edb							- retrieve Wind economy data

%       tlogon          - logon into trading systems
%       tlogout         - logout from trading systems
%       torder          - send orders to trading systems
%       tcovered        - send covered lock/unlock to trading systems
%       tcancel         - cancel tradeing orders
%       tquery          - query capitial,position,order,trade,account,department or logonid.
%
%       bktstart        - start a backtest
%       bktquery        - query capital/position/order of the running backtest
%       bktorder        - order in the running backtest
%       bktend          - end the running backtest
%       bktstatus       - return the status of backtests
%       bktsummary      - return the summary of backtests
%       bktdelete       - delete a backtest
%       bktstrategy     - return strategies
%       bktfocus        - focus a strategy
%       bktshare        - share a strategy
%

%
%   Other Functions:
%       close           - destroy the wind data interface object
%       menu            - toggle the toolbar; open the guide dialog
%       isconnected     - check if the wind object avaliable
%       cancelRequest   - cancel an unfinished request by its id
%
    properties (Access = 'private',Hidden=true)
        mWind
        mFigure
        mTotalTimeout
        mTimer
        mWaitFunctions %已经发出请求，等待回调的函数列表 id - { callback，updated,funData}
        mSessionStateChangedCount
        mLoginFailure
        mToRunCmd;
        mbShowWelcome
    end
  
  methods (Static,Access = 'public')
    
    %% clear() 清空wind对象；
    function clear()
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            return;
        end
        
        aa = timerfind('Name','gWindDataTimer');
        if(~isempty(aa))
            stop(aa);
            delete(aa);
        end;      
        
        if(~isempty(gWindData.mWaitFunctions))
            clear gWindData.mWaitFunctions;
        end
        gWindData.mWaitFunctions=[];
        gWindData.mTimer = [];
        try
            if(~isempty(gWindData.mWind))
                gWindData.mWind.stop();
                gWindData.mWind.delete;
                clear gWindData.mWind;
            end
            gWindData.mWind=[];
            if( ~isempty(gWindData.mFigure))
                close(gWindData.mFigure);
            end
            gWindData.mFigure=[];
        catch funcerr
            funcerr.message
        end
        clear GLOBAL gWindData ;
    end %end close method
  end
    
  methods (Access = 'public')
    function display(w)
        global gWindData;
        if gWindData.mbShowWelcome
            disp('   Welcome to Wind Quant API!');
            disp('   Copyright(c) 2012-2013 Shanghai Wind Information Co., Ltd. All rights reserved.');
            disp('   In no circumstance shall Wind be responsible for any damages or losses caused by using Wind Quant API.');
            disp('   ');
            disp('   You can use w.menu to toggle the wind toolbar on and off.');
            disp('   Type ''help windmatlab'' for more information.');
            gWindData.mbShowWelcome = 0;
        end
    end
    
    function w = windmatlab(showmenu,totaltimeout,others)
    %   Constructs a wind data interface object.
    %       w = windmatlab(showmenu,totaltimeout)
    %       totaltimeout is timeout period(s), default to 300 seconds.
    %       showmenu is to show or not to show the toolbar. Default is not showing.
    %     
    %   Example:
    %       w = windmatlab() 
    %       Create the wind interface by the default settings.
    %     
    %       w = windmatlab(0, 300);
    %       Create the wind interface with the 300 seconds timeout and not show the
    %       wind data feed toolbar.
    % 创建Wind对象
    % w = wind(showmenu,totaltimeout,others) 
    % totaltimeout 超时时间（秒)，缺省为300秒；
    % showmenu (缺省=1显示）是否显示工具栏
        global gWindData;
        
        if(nargin<1)
            showmenu = 0;
        end;

        if isa(gWindData,'windmatlab')
            %gWindData.close();
            %global gWindData;            
            w = gWindData
            if(showmenu)
               gWindData.menu();
            end              
            return;
        end
        
        gWindData = w;
        
        if(nargin>1)
            gWindData.mTotalTimeout = totaltimeout;
        else
            gWindData.mTotalTimeout = 120;
        end;
        gWindData.mSessionStateChangedCount = 0;
        
        gWindData.mbShowWelcome = 1;
        gWindData.mToRunCmd = cellstr('');
        
        try
            %gWindData.mWaitFunctions = containers.Map('KeyType','uint64','ValueType','any');
            gWindData.mWaitFunctions = containers.Map(uint64(0), 'a', 'uniformValues', false);
            gWindData.mWaitFunctions.remove(0);
    
            gWindData.mFigure = figure('Name','WindDataCOM_hid','Position',[1,1,100,100],'Visible','off');
            gWindData.mWind = actxcontrol('WINDDATACOM.WindDataCOMCtrl.1',[1,1,20,20],gWindData.mFigure, ...
                                {'stateChanged' @gWindData.stateChangedCallback;'helpReport' @gWindData.helpReport});
            set(gWindData.mFigure,'HandleVisibility','callback')
            %set(0,'CurrentFigure',curFigure)   
            
            
            gWindData.mTimer= timer('Name','gWindDataTimer','TimerFcn',@w.timerFunc, 'Period', 1,'ExecutionMode','fixedSpacing', 'StartDelay', 0.01);
            %start(gWindData.mTimer);

            gWindData.mLoginFailure = 0;
            gWindData.mWind.setTimeout(0,gWindData.mTotalTimeout*1000);
            err=gWindData.mWind.start_syn('','',gWindData.mTotalTimeout*2*1000);
            %set(gcf, 'Visible', 'off')
            %globalWindObj = w;
            
            if(err == 0) %登录成功
               if(showmenu)
                   gWindData.menu();
               end              
               return;
            end
                
            if(err == -40520009) %WERR_LOST_WBOX)
                display('windmatlab:wbox lost');
                throw;
            elseif(err==-40520008)%WERR_TIMER_OUT
                display('windmatlab:start error:timeout');
                throw;                
            elseif(err==-40520005)%WERR_LOGON_NOAUTH
                display('windmatlab:no matlab api authority');
                throw;
            elseif(err==-40520004)%WERR_LOGON_FAILED
                display('windmatlab:login failed');
                throw;
            elseif(err==-40520014)%WQERR_LOGON_NO_WIM
                display('windmatlab:Please Logon iWind First!');
                throw;
            elseif(err==-40520013)%WQERR_LOGON_CONFLICT
                display('windmatlab:you have logined other wind product by another username');
                throw;
            elseif(err==-40520015)%WQERR_TOO_MANY_LOGON_FAILED
                display('windmatlab:too many consecutive login failure.');
                throw;
            else
                fprintf('windmatlab:start error:%d\n',err);
                throw;
            end
        catch
            windmatlab.clear();
            error('windmatlab: failure to create wind object');
        end
    end   %end wind constructor
    
    %% close(w) 结束访问Wind对象
    function close(w)
    %w.close;或者close(w);
    %   CLOSE will stop using the wind object.
        aa=timerfind('Name','gWindDataTimer');
        if(~isempty(aa))
            stop(aa);
            delete(aa);
        end;
        windmatlab.clear();
        clear w;
    end %end close method
    

    %% menu(Options) 显示导航界面
    function menu(w, Options)
%   w.menu('wsq'),w.menu('wsd'),....显示导航界面，帮助组件访问命令；
%
%   MENU is to operate the wind API toolbar.
%   MENU, by itself, will toggle the toolbar on and off.
%
%   MENU ('FUN') can directly show the guide dialog of appointted
%   function, desprite of the toolbar enabled or not.
%   Supported FUN: all the Data Retrieve Functions, i.e. wsd, wst, wsi,
%   wss, and wsq.
%
%   Example:
%       w.menu  -   it will toggle the toolbar on or off.
%
%       w.menu ('wsd')  -  it will open the wsd guide dialog. (as if you click the
%                          'WSD' button on wind toolbar.)
    
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end        
        
        if(nargin<2)
            Options = '';
        end
        gWindData.mWind.help(Options);
    end
    
    function x = isconnected(w)
%   ISCONNECTED will check the wind object is correctly logined.
%
%   Example:
%       x = isconnected(w) - 用来判断wind对象是否已经登录成功
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            x = 0;
            return ;            
        end          
        %    enum enumSessiontState {
        % 		 SS_NOT_INITED=0
        % 		,SS_Started 
        % 		,SS_Running
        % 		,SS_Error    };        
        [state,err]=gWindData.mWind.getConnectionState();
        if(state == 2 && err ==0)
            x = 1;
            return;
        else
            x=0;
            return;
        end
    end  %end isconnected method
    
    %% cancelRequest(w,reqid) 通过reqid取消某个没有完成的请求
    function cancelRequest(w,reqid)
%   reqid 来自于WSQ等使用回调函数情况下返回的id值。
%   demo :getRealtimeWSQ.m
%   [~,~,~,~,errorid,id]=w.wsq('600000.SH,000002.SH','rt_last,rt_last_vol,rt_ask1,rt_asize1',@WSQCallback);   
%   w.cancelRequest(id);
%   cancelRequest will cancel a certain request by its id.
%   if id==0, then cancel All Request.
%   The reqid is returned whenyou send the request.
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end          
        if(nargin<1 )
            return;
        end
        
        gWindData.mWind.cancelRequest(reqid);
    end

       
    %% [data,codes,fields,times,errorid,reqid] = wsq(w,WindCodes,WindFields,varargin)
    function [data,codes,fields,times,errorid,reqid] = wsq(w,WindCodes,WindFields,varargin)
%        
%   WSQ用来获取当天实时指标数据，数据可以一次性请求，也可以通过订阅的方式获取当日实时行情数据.
%   如需向导，请使用w.menu('wsq')创建命令.
% 
%   一次性请求实时行情数据：
%   [data,codes,fields,times,errorid] = w.wsq(windcodes,windfields)
% 
%   订阅实时行情数据：
%   [~,~,~,~,errorid,reqid] = w.wsq(windcodes,windfields,callback,userdata)
%   其中callback为回调函数，用来指定实时指标触发时执行相应的回调函数.
%       userdata为传递给回调函数的用户自己的数据
%  
%   Description：
%        w	            创建的windmatlab对象；
%        windcodes      Wind代码，格式为'600000.SH',单个请求支持多品种.
%        windfields     提取指标，格式为'rt_last_vol,rt_ask1,rt_asize1'.
%        callback       回调函数,通过回调函数接收不断传递回来的实时数据
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid        函数运行的错误ID.
%        reqid          在订阅时为请求id，用来取消订阅的
% 
%   Example：	    
% 	     [~,~,~,~,errorid,reqid]=w.wsq('600000.SH,000002.SH','rt_last,rt_last_vol,rt_ask1,rt_asize1',@WSQCallback) 
% 	     w.cancelRequest(reqid);
% 	     或者可以运行Sample中的getRealtimeWSQ.m文件.
%
 
        
        data=[];
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        reqid=0;
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return;
        end        
        
        [WindCodes,WindFields] = w.prepareCodeAndField(WindCodes,WindFields);        
            
        %set other parameters, 
        callbackfunarg='';
        Options = '';
        if(length(varargin)==1)
            callbackfun = varargin{1};%callback;
            useasyn = 1;
            Options = 'realtime=y';
        elseif length(varargin)>1
            [Options,callbackfun,useasyn,callbackfunarg] = w.prepareWsqOptions(varargin);
        else
            useasyn = 0;
            callbackfun =@gWindData.dealDataFunc;     
            Options='';
        end
        if( ~isa(callbackfun,'function_handle'))
            useasyn = 0;
            callbackfun =@gWindData.dealDataFunc;     
            Options='';
        end

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        if(useasyn==0)
            [data,codes,fields,times,errorid]=gWindData.mWind.wsq_syn(WindCodes,WindFields,Options);
            times = times';
            data =w.reshapedata(data,codes,fields,times);            
            return;
        end
    
        %以下是异步的过程
        %stop(gWindData.mTimer);
        [reqid,errorid] = gWindData.mWind.wsq(WindCodes,WindFields,Options);
        waitdata_asyn(w,reqid,errorid,callbackfun,callbackfunarg);
        %times = datestr(timesv,31);
        return;
    end
    
    %% [data,codes,fields,times,errorid,reqid] = wsqtd(w,WindCodes,WindFields,varargin)
    function [data,codes,fields,times,errorid,reqid] = wsqtd(w,WindCodes,WindFields,varargin)
%        
%   WSQTD用来获取当天实时指标数据，数据可以一次性请求，也可以通过订阅的方式获取当日实时行情数据.
%   如需向导，请使用w.menu('wsq')创建命令.
% 
%   一次性请求实时行情数据：
%   [data,codes,fields,times,errorid] = w.wsqtd(windcodes,windfields)
% 
%   订阅实时行情数据：
%   [~,~,~,~,errorid,reqid] = w.wsqtd(windcodes,windfields,callback,userdata)
%   其中callback为回调函数，用来指定实时指标触发时执行相应的回调函数.
%       userdata为传递给回调函数的用户自己的数据
%  
%   Description：
%        w	            创建的windmatlab对象；
%        windcodes      Wind代码，格式为'600000.SH',单个请求支持多品种.
%        windfields     提取指标，格式为'rt_last_vol,rt_ask1,rt_asize1'.
%        callback       回调函数,通过回调函数接收不断传递回来的实时数据
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid        函数运行的错误ID.
%        reqid          在订阅时为请求id，用来取消订阅的
% 
%   Example：	    
% 	     [~,~,~,~,errorid,reqid]=w.wsqtd('600000.SH,000002.SH','rt_last,rt_last_vol,rt_ask1,rt_asize1',@WSQCallback) 
% 	     w.cancelRequest(reqid);
% 	     或者可以运行Sample中的getRealtimeWSQ.m文件.
%
 
        
        data=[];
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        reqid=0;
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return;
        end        
        
        [WindCodes,WindFields] = w.prepareCodeAndField(WindCodes,WindFields);        
            
        %set other parameters, 
        callbackfunarg='';
        Options = '';
        if(length(varargin)==1)
            callbackfun = varargin{1};%callback;
            useasyn = 1;
            Options = 'realtime=y';
        elseif length(varargin)>1
            [Options,callbackfun,useasyn,callbackfunarg] = w.prepareWsqOptions(varargin);
        else
            useasyn = 0;
            callbackfun =@gWindData.dealDataFunc;     
            Options='';
        end
        if( ~isa(callbackfun,'function_handle'))
            useasyn = 0;
            callbackfun =@gWindData.dealDataFunc;     
            Options='';
        end

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        if(useasyn==0)
            [data,codes,fields,times,errorid]=gWindData.mWind.wsqtd_syn(WindCodes,WindFields,Options);
            times = times';
            data =w.reshapedata(data,codes,fields,times);            
            return;
        end
    
        %以下是异步的过程
        %stop(gWindData.mTimer);
        [reqid,errorid] = gWindData.mWind.wsqtd(WindCodes,WindFields,Options);
        waitdata_asyn(w,reqid,errorid,callbackfun,callbackfunarg);
        %times = datestr(timesv,31);
        return;
    end
    
    
    %% [data,codes,fields,times,errorid] = wsd(w,WindCodes,WindFields,StartTime,EndTime,varargin)
    function [data,codes,fields,times,errorid,useless] = wsd(w,WindCodes,WindFields,StartTime,EndTime,varargin)
%
%   WSD 用来获取选定证券品种的历史序列数据,包括日间的行情数据，基本面数据以及技术数据指标.
%   如需向导，请使用w.menu('wsd')创建命令.
%   [data,codes,fields,times,errorid] = w.wsd(windcodes,windfields,starttime,endtime,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        windcodes      Wind代码，格式为'600000.SH',单个请求只支持单品种.
%        windfields     提取指标，格式为'OPEN,CLOSE,HIGH'.
%        starttime	    开始日期'20120701'.
%        endTime        结束日期'20120919'.
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：	    
% 	     w.wsd('600000.SH','high,low,close,open',now-5,now)    
% 	     或者可以运行Sample中的get300WSDInfo.m文件.
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end         
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;
        [WindCodes,WindFields] = w.prepareCodeAndField(WindCodes,WindFields);        

        if(nargin<4 || isempty(StartTime))
            StartTime = now;
        end;
        
        if(nargin<5 || isempty(EndTime))
            EndTime='';
        end;
        [StartTime,EndTime] = w.prepareTime(StartTime,EndTime,1);

        %set other parameters, 
        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.wsd_syn(WindCodes,WindFields,StartTime ...
                                ,EndTime,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        
%         stop(gWindData.mTimer);
%         [reqid,errorid] = gWindData.mWind.history(WindCodes,WindFields,StartTime ...
%                                 ,EndTime,Options);
%         [data,id,codes,fields,times,errorid] = waitdata(w,reqid,errorid,callbackfun,useasyn,callbackfunarg);
%         %times=datestr(timesv,31);
        return;        
    end
    
   
   %%%%%%%%%%%%%%%EDB
   %% [data,codes,fields,times,errorid] = edb(w,WindCodes,StartTime,EndTime,varargin)
    function [data,codes,fields,times,errorid,useless] = edb(w,WindCodes,StartTime,EndTime,varargin)
%
%   edb 用来获取经济数据.
%   如需向导，请使用w.menu('edb')创建命令.
%   [data,codes,fields,times,errorid] = w.edb(windcodes,starttime,endtime,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        windcodes      Wind代码，格式为'M0000001',单个请求只支持单品种.
%        starttime	    开始日期'20120701'.
%        endTime        结束日期'20120919'.
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：	    
% 	     w.edb('M0000001','2012-05-12','2014-06-11')   
% 	     
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end         
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;
        %[WindCodes,WindFields] = w.prepareCodeAndField(WindCodes,WindFields);        

        if(nargin<4 || isempty(StartTime))
            StartTime = now;
        end;
        
        if(nargin<5 || isempty(EndTime))
            EndTime='';
        end;
        [StartTime,EndTime] = w.prepareTime(StartTime,EndTime,1);

        %set other parameters, 
        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.edb_syn(WindCodes,StartTime ...
                                ,EndTime,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        
        return;        
    end
   
   
   
    %% [data,codes,fields,times,errorid] = wsi(w,WindCodes,WindFields,StartTime,EndTime,varargin)
    function [data,codes,fields,times,errorid,useless] = wsi(w,WindCodes,WindFields,StartTime,EndTime,varargin)
%
%   WSI用来获取指定品种的日内分钟K线数据，包含历史和当天，分钟周期可以指定，技术指标参数可以自定义设置.
%   如需向导，请使用w.menu('wsi')创建命令.
%   [data,codes,fields,times,errorid] = w.wsi(windcodes,windfields,starttime,endtime,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        windcodes      Wind代码，格式为'600000.SH',单个请求只支持单品种.
%        windfields     提取指标，格式为'OPEN,CLOSE,HIGH'.
%        starttime      开始日期，格式为 '20120701 09:30:00'.
%        endtime        结束日期，格式为'20120919 09:30:00'
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.wsi('600000.SH','high,low,close,open',now-5,now)   
% 	      或者可以运行Sample中的get5mDataWSI.m文件.
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        [WindCodes,WindFields] = w.prepareCodeAndField(WindCodes,WindFields);        

        if(nargin<4 || isempty(StartTime))
            StartTime = now;
        end;
        
        if(nargin<5 || isempty(EndTime))
            EndTime=now;
        end;
        [StartTime,EndTime] = w.prepareTime(StartTime,EndTime,0);

        %set other parameters, 
        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.wsi_syn(WindCodes,WindFields,StartTime ...
                                ,EndTime,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
%         stop(gWindData.mTimer);
%         [reqid,errorid] = gWindData.mWind.wsi(WindCodes,WindFields,StartTime ...
%                                 ,EndTime,Options);
%         [data,id,codes,fields,times,errorid] = waitdata(w,reqid,errorid,callbackfun,useasyn,callbackfunarg);
%         %times = datestr(timesv,31);
        return; 
    end
   
    %% [data,codes,fields,times,errorid] = wst(w,WindCodes,WindFields,StartTime,EndTime,varargin)
    function [data,codes,fields,times,errorid,useless] = wst(w,WindCodes,WindFields,StartTime,EndTime,varargin)
%
%   WST用来获取返回日内盘口买卖十档快照数据和分时成交数据.
%   如需向导，请使用w.menu('wst')创建命令.
%   [data,codes,fields,times,errorid] = w.wst(windcodes,windfields,starttime,endtime,varargin)
% 
%   Description：
%       w	            为创建的windmatlab对象；
%       windcodes       Wind代码，格式为'600000.SH',单个请求只支持单品种.
%       windfields      提取指标，格式为'OPEN,CLOSE,HIGH'.
%       starttime       开始日期，格式为 '20120701 09:30:00'.
%       endtime         结束日期，格式为'20120919 09:30:00'
% 	 
%       data	        返回的数据结果.
%       codes           返回数据对应的代码.
%       fields          返回数据对应的指标.
%       times           返回数据对应的时间.
%       errorid	        函数运行的错误ID.
% 
%   Example：	    
% 	    w.wst('600000.SH','last,volume,amt,bid1,bsize1,ask1,asize1','20120919 09:30:00','20120919 10:30:00')  
% 	    或者可以运行Sample中的getTickDataWST.m文件.	
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end           
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        [WindCodes,WindFields] = w.prepareCodeAndField(WindCodes,WindFields);        

        if(nargin<4 || isempty(StartTime))
            StartTime = now;
        end;
        
        if(nargin<5 || isempty(EndTime))
            EndTime='';
        end;
        [StartTime,EndTime] = w.prepareTime(StartTime,EndTime,0);

        %set other parameters, 
        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.wst_syn(WindCodes,WindFields,StartTime ...
                                ,EndTime,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        
%         stop(gWindData.mTimer);
%         [reqid,errorid] = gWindData.mWind.wst(WindCodes,WindFields,StartTime ...
%                                 ,EndTime,Options);
%         [data,id,codes,fields,times,errorid] = waitdata(w,reqid,errorid,callbackfun,useasyn,callbackfunarg);
%         %times = datestr(timesv,31);
        return; 
    end     
    
    %% [data,codes,fields,times,errorid] = wss(w,WindCodes,WindFields,varargin)
    function [data,codes,fields,times,errorid,useless] = wss(w,WindCodes,WindFields,varargin)
%
%   WSS用来获取指定品种的历史截面数据，比如取沪深300只股票的2012年3季度的净利润财务指标数据.
%   如需向导，请使用w.menu('wss')创建命令.
%   [data,codes,fields,times,errorid] = w.wss(windcodes,windfields,varargin)
% 
%   Description：
%       w	            为创建的windmatlab对象；
%       windcodes       Wind代码，格式为'600000.SH',单个请求支持多品种.
%       windfields      提取指标，格式为'OPEN,CLOSE,HIGH'.
% 	 
%       data	        返回的数据结果.
%       codes           返回数据对应的代码.
%       fields          返回数据对应的指标.
%       times           返回数据对应的时间.
%       errorid	        函数运行的错误ID.
% 
%   Example：	    
% 	    w.wss('600000.SH','comp_name,comp_name_eng,ipo_date,float_a_shares,mf_amt,mf_vol')    
% 	    或者可以运行Sample中的getSnapWSS.m文件.
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end           
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        [WindCodes,WindFields] = w.prepareCodeAndField(WindCodes,WindFields);    

        %set other parameters, 
        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.wss_syn(WindCodes,WindFields,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        
%         %[totalOptions] = w.toOptions('','',Options);
%         %totalOptions = Options;
%         stop(gWindData.mTimer);
%         [reqid,inerrorid] = gWindData.mWind.wss(WindCodes,WindFields,Options);
%         [data,id,codes,fields,times,errorid] = waitdata(w,reqid,inerrorid,callbackfun,useasyn,callbackfunarg);
%         %times = datestr(timesv,31);
        return;
    end
    
    
    %% [Data,Fields,ErrorCode] = tlogon(w,BrokerID, DepartmentID, LogonAccount, Password, AccountType,varargin)
    function [Data,Fields,ErrorCode] = tlogon(w,BrokerID, DepartmentID, LogonAccount, Password, AccountType, varargin)
%   tlogon用来登录交易账号。
%   如需向导，请使用w.menu('tlogon')创建命令.
%   [Data,Fields,ErrorCode] = tlogon(w,BrokerID, DepartmentID, LogonAccount, Password, AccountType, varargin)
% 
%   Description：
%       w	            为创建的windmatlab对象；
%       BrokerID        经纪商代码.
%       DepartmentID    营业部代码(期货登录填写0)
%       LogonAccount    资金账号
%       Password        账号密码
%       AccountType     账号类型: 深圳上海A ：11或SH、SZ、SHSZ;	深圳B：12 或 SZB;上海B：13或SHB;郑商所：14或CZC
%                       上期所：15或SHF;大商所：16或DCE; 中金所：17或CFE
% 
% 	返回结果： 
%       Data            返回的数据内容,为一个cell，最后两列为错误号和错误消息。
%       Fields          返回的数据内容中对应的解释.
%       ErrorCode	    返回的错误号，0表示所有操作都对，其他表示有错，可以根据Data定位具体错误 . 
%    注：此命令支持向量操作。
%   Example：	    
% 	        
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end           
        
        Data=[];       Fields=[];      ErrorCode=-1;
        
        [BrokerID,valid] = w.prepareArray(BrokerID);
        if(valid)   [DepartmentID,valid] = w.prepareArray(DepartmentID);   end
        if(valid)   [LogonAccount,valid] = w.prepareArray(LogonAccount);         end
        if(valid)   [Password,valid] = w.prepareArray(Password);           end
        if(valid)   [AccountType,valid] = w.prepareArray(AccountType);     end
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArray(varargin);   end
        
        if(~valid)
            display('The input arguments are not right!');
            return
        end
       
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.tLogon(BrokerID,DepartmentID,LogonAccount,Password,AccountType,Options);
        Data = Data';    Fields = Fields';

        return;
    end

    %% [Data,Fields,ErrorCode] = tlogout(w,LogonID)
    function [Data,Fields,ErrorCode] = tlogout(w,LogonID)
%   tlogout用来退出交易账号登录。
%   如需向导，请使用w.menu('tlogout')创建命令.
%   [Data,Fields,ErrorCode] = tlogon(w,LogonID)
% 
%   Description：
%       w	            为创建的windmatlab对象；
%       LogonID         交易登录ID. 单个账户登录时可以不填
% 	 
% 	返回结果： 
%       Data            返回的数据内容,为一个cell，最后两列为错误号和错误消息。
%       Fields          返回的数据内容中对应的解释.
%       ErrorCode	    返回的错误号，0表示所有操作都对，其他表示有错，可以根据Data定位具体错误 . 
%    注：此命令支持向量操作。
%   Example：	    
%      %假设用于登录了两个交易账号LogonID 分别为 111，222
%      w.tlogout({111,222})用来把两个账号退出。
% 	   
%      %当用户只有一个交易账号登录时，此时不需要LogonID参数，即
%      w.tlogout();
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end           
        
        Data=[];       Fields=[];      ErrorCode=-1;
        
        valid =1;
        if(nargin<2)
            Options='';
        else
            [LogonID,valid] = w.prepareArray(LogonID);
            Options = strcat('LogonID=',LogonID);
        end
        
        if(~valid)
            display('The input arguments are not right!');
            return
        end
       
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.tLogout(Options);
        Data = Data';    Fields = Fields';

        return;
    end    

    %% [Data,Fields,ErrorCode] = torder(w,WindCode, TradeSide, OrderPrice, OrderVolume, varargin)
    function [Data,Fields,ErrorCode] = torder(w,WindCode, TradeSide, OrderPrice, OrderVolume, varargin)
%   tlogon用来登录交易账号。
%   如需向导，请使用w.menu('torder')创建命令.
%   [Data,Fields,ErrorCode] = torder(w,WindCode, TradeSide, OrderPrice, OrderVolume, varargin)
% 
%   Description：
%       w	            为创建的windmatlab对象；
%       WindCode        需要交易的证券代码.
%       TradeSide       交易方向
%                       开仓买入(证券买入) ：1  or Buy
%                       开仓卖出          : 2 or Short
%                       平仓买入          : 3 or Cover
%                       平仓卖出(证券卖出) :4 or Sell
%                       平今仓买入        : 5 or CoverToday
%                       平今仓卖出        :6 or SellToday
%       OrderPrice      交易价格
%       OrderVolume     交易数量
%   其中可选输入： 
%       'LogonID'       交易登录ID,当有多个交易账号时需要指明
%       'MarketType'    当证券代码不是Wind码时需要提供证券所处的市场代码。
%                        深圳：0 or SZ；上海：1 or SH；深圳特 三板: 2 or OC
%                        港股	:	6 or HK；郑商所:7 or CZC；上期所:8 or SHF；
%                        大商所:9 or DCE；中金所： 10 or CFE
%       'OrderType'     价格委托方式。缺省为限价委托0
%                       限价委托 : 0 缺省 LMT
%                       对方最优价格委托  : 1 BOC
%                       本方最优价格委托  :2 BOP
%                       即时成交剩余撤销  :3 ITC
%                       最优五档剩余撤销  :4 B5TC
%                       全额成交或撤销委托：5 FOK
%                       最优五档剩余转限价：6 B5TL
%       'HedgeType'     对于期货需要填
%                       -投机    ： 0 or SPEC    缺省
%                       -保值    :  1 or HEDG
% 	返回结果： 
%       Data            返回的数据内容,为一个cell，最后两列为错误号和错误消息。
%       Fields          返回的数据内容中对应的解释.
%       ErrorCode	    返回的错误号，0表示所有操作都对，其他表示有错，可以根据Data定位具体错误 . 
%    注：此命令支持向量操作。
%   Example：	    
% 	        
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end           
        
        Data=[];       Fields=[];      ErrorCode=-1;
        
        [WindCode,valid] = w.prepareArray(WindCode);
        if(valid)   [TradeSide,valid] = w.prepareArray(TradeSide);   end
        if(valid)   [OrderPrice,valid] = w.prepareArray(OrderPrice);         end
        if(valid)   [OrderVolume,valid] = w.prepareArray(OrderVolume);           end
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArray(varargin);   end
        
        if(~valid)
            display('The input arguments are not right!');
            return
        end
       
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.tSendOrder(WindCode,TradeSide,OrderPrice,OrderVolume,Options);
        Data = Data';    Fields = Fields';

        return;
    end
    
    %% [Data,Fields,ErrorCode] = tcovered(w,WindCode, TradeSide, OrderVolume, varargin)
    function [Data,Fields,ErrorCode] = tcovered(w,WindCode, TradeSide,  OrderVolume, varargin)
%   tlogon用来登录交易账号。
%   如需向导，请使用w.menu('torder')创建命令.
%   [Data,Fields,ErrorCode] = tcovered(w,WindCode, TradeSide, OrderVolume, varargin)
% 
%   Description：
%       w	            为创建的windmatlab对象；
%       WindCode        需要交易的证券代码.
%       TradeSide       交易方向
%                       锁定 ：0 or Lock
%                       解锁 : 1 or UnLock
%       OrderVolume     交易数量
%   其中可选输入： 
%       'LogonID'       交易登录ID,当有多个交易账号时需要指明
% 	返回结果： 
%       Data            返回的数据内容,为一个cell，最后两列为错误号和错误消息。
%       Fields          返回的数据内容中对应的解释.
%       ErrorCode	    返回的错误号，0表示所有操作都对，其他表示有错，可以根据Data定位具体错误 . 
%    注：此命令支持向量操作。
%   Example：	    
% 	        
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end           
        
        Data=[];       Fields=[];      ErrorCode=-1;
        
        [WindCode,valid] = w.prepareArray(WindCode);
        if(valid)   [TradeSide,valid] = w.prepareArray(TradeSide);   end
        if(valid)   [OrderVolume,valid] = w.prepareArray(OrderVolume);           end
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArray(varargin);   end
        
        if(~valid)
            display('The input arguments are not right!');
            return
        end
       
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.tSendCovered(WindCode,TradeSide,OrderVolume,Options);
        Data = Data';    Fields = Fields';

        return;
    end
    
    %% [Data,Fields,ErrorCode] = tcancel(w,OrderNumber, varargin)
    function [Data,Fields,ErrorCode] = tcancel(w,OrderNumber, varargin)
%   tlogon用来登录交易账号。
%   如需向导，请使用w.menu('tcancel')创建命令.
%   [Data,Fields,ErrorCode] = tcancel(w,OrderNumber, varargin)
% 
%   Description：
%       w	            为创建的windmatlab对象；
%       OrderNumber     委托下单后查询到的委托号 .
%   其中可选输入： 
%       'LogonID'       交易登录ID,当有多个交易账号时需要指明
%       'MarketType'    有时需要提供证券所处的市场代码。
%                        深圳：0 or SZ；上海：1 or SH；深圳特 三板: 2 or OC
%                        港股	:	6 or HK；郑商所:7 or CZC；上期所:8 or SHF；
%                        大商所:9 or DCE；中金所： 10 or CFE
% 	返回结果： 
%       Data            返回的数据内容,为一个cell，最后两列为错误号和错误消息。
%       Fields          返回的数据内容中对应的解释.
%       ErrorCode	    返回的错误号，0表示所有操作都对，其他表示有错，可以根据Data定位具体错误 . 
%    注：此命令支持向量操作。
%   Example：	    
% 	        
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end           
        
        Data=[];       Fields=[];      ErrorCode=-1;
        
        [OrderNumber,valid] = w.prepareArray(OrderNumber);
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArray(varargin);   end
        
        if(~valid)
            display('The input arguments are not right!');
            return
        end
       
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.tCancelOrder(OrderNumber,Options);
        Data = Data';    Fields = Fields';

        return;
    end

    %% [Data,Fields,ErrorCode] = tquery(w,qrycode, varargin)
    function [Data,Fields,ErrorCode] = tquery(w,qrycode, varargin)
%   tquery用来查询交易相关所有信息。
%   如需向导，请使用w.menu('tquery')创建命令.
%   [Data,Fields,ErrorCode] = tquery(w,qrycode, varargin)
% 
%   Description：
%       w	            为创建的windmatlab对象；
%       qrycode         委托下单后查询到的委托号 .
%                       0 Capital 资金； 1 Position   持仓； 2 Order 当日委托； 
%                       3 Trade 当日成交;4 Department 营业部;5 Account 股民账号
%                       6 Broker 经纪商; 7 LogonID   登录ID
%   根据qrycode可选输入可能有： 
%       'LogonID'       交易登录ID,当有多个交易账号时需要指明
%       'RequestID'     委托下单返回的请求ID
%       'OrderNumber'   委托下单柜台返回的ID
%       'WindCode'      依据WindCode查询
%
% 	返回结果： 
%       Data            返回的数据内容,为一个cell，最后两列为错误号和错误消息。
%       Fields          返回的数据内容中对应的解释.
%       ErrorCode	    返回的错误号，0表示所有操作都对，其他表示有错，可以根据Data定位具体错误 . 
%    注：此命令qrycode只能选一个，可选参数支持向量操作。
%   Example：	    
% 	        
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end           
        
        Data=[];       Fields=[];      ErrorCode=-1;
        
        [qrycode,valid] = w.prepareArray(qrycode);
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArray(varargin);   end
        
        if(~valid)
            display('The input arguments are not right!');
            return
        end
       
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.tQuery(qrycode,Options);
        Data = Data';    Fields = Fields';

        return;
    end    
%     %% [Data,Fields,ErrorCode] = tmonitor(w,varargin)
%     function [Data,Fields,ErrorCode] = tmonitor(w,varargin)
% %   tmonitor用来调出交易监控界面。
% %   如需向导，请使用w.menu('tmonitor')创建命令.
% %   [Data,Fields,ErrorCode] = tmonitor(w,varargin)
% % 
% %   Description：
% %       w	            为创建的windmatlab对象；
% % 	返回结果： 
% %       Data            返回的数据内容,为一个cell，最后两列为错误号和错误消息。
% %       Fields          返回的数据内容中对应的解释.
% %       ErrorCode	    返回的错误号，0表示所有操作都对，其他表示有错，可以根据Data定位具体错误 . 
% %   Example：	    
% % 	        
% %
%         global gWindData;
%         if ~isa(gWindData,'windmatlab')
%             display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
%             return ;            
%         end           
%         
%         Data=[];       Fields=[];      ErrorCode=-1;
%         
%         [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArray(varargin); 
%         
%         if(~valid)
%             display('The input arguments are not right!');
%             return
%         end
%        
%         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%         [Data,Fields,ErrorCode]=gWindData.mWind.tMonitor(Options);
%         Data = Data';    Fields = Fields';
% 
%         return;
%     end      
    
    %% [data,codes,fields,times,errorid] = weqs(w,filtername,varargin)
    function [data,codes,fields,times,errorid,useless] = weqs(w,filtername,varargin)
%
%   weqs用来获取证券筛选的结果.
%   如需向导，请使用w.menu('weqs')创建命令.
%   [data,codes,fields,times,errorid] = w.weqs(filtername,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        filtername     筛选方案的名字.
%        varargin       其他可选参数.
% 	 
%        data	        返回的筛选数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：
% 	      w.weqs('KDJ 低位金叉')   
% 	      或者可以运行Sample中的weqsdemo.m文件.
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        if( iscell(filtername))
            filtername=filtername{1}
        end

        %set other parameters, 
        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.weqs_syn(filtername,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        return; 
    end
     
    %% [data,codes,fields,times,errorid] = wset(w,tablename,varargin)
    function [data,codes,fields,times,errorid,useless] = wset(w,tablename,varargin)
%
%   wset用来获取指定数据集内容，包含指数成分、权重等.
%   如需向导，请使用w.menu('wset')创建命令.
%   [data,codes,fields,times,errorid] = w.wset(tablename,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        tablename      数据集的名字.
%        varargin       其他可选参数，如field,windcode,date等；"date=20130518;windcode=000300.SH;field=date,sec_name,i_weight".
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：
% 	      w.wset('IndexConstituent','date=20130518;windcode=000300.SH;field=date,sec_name,i_weight')   
% 	      或者可以运行Sample中的wsetdemo.m文件.
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        if( iscell(tablename))
            tablename=tablename{1}
        end

        %set other parameters, 
        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.wset_syn(tablename,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        return; 
    end
    %% [data,codes,fields,times,errorid] = wpf(w,productname,tablename,varargin)
    function [data,codes,fields,times,errorid,useless] = wpf(w,productname,tablename,varargin)
%
%   wset用来获取指定数据集内容，包含指数成分、权重等.
%   如需向导，请使用w.menu('wset')创建命令.
%   [data,codes,fields,times,errorid] = w.wpf(productname,tablename,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        productname    产品名
%        tablename      数据集的名字.
%        varargin       其他可选参数，如field,windcode,date等；"date=20130518;windcode=000300.SH;field=date,sec_name,i_weight".
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        if( iscell(productname))
            productname=productname{1}
        end        
        if( iscell(tablename))
            tablename=tablename{1}
        end

        %set other parameters, 
        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.wpf_syn(productname,tablename,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        return; 
    end    
    %% [data,codes,fields,times,errorid,useless] = wupf(w,PortfolioName,TradeDate,WindCode,Quantity,CostPrice,varargin)
    function [data,codes,fields,times,errorid,useless] = wupf(w,PortfolioName,TradeDate,WindCode,Quantity,CostPrice,varargin)
%
%   wupf用来上传组合.
%   如需向导，请使用w.menu('wupf')创建命令.
%   [data,fields,errorid] = wupf(w,PortfolioName,TradeDate,WindCode,Quantity,CostPrice,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        PortfolioName  组合名称
%        TradeDate      调整日期
%        WindCode       证券代码
%        Quantity       证券数量
%        CostPrice      成本价格
%        varargin       其他可选参数    WindID      Wind用户帐号 
%                                       TradeSide   多空方向，默认为做多 
%                                       AssetType   资产类别
%                                       HedgeType   投机套保
% 	 
%        data	        返回的数据结果.
%        fields         返回数据对应的指标.
%        errorid	    函数运行的错误ID.
% 
%   Example：
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        data=[];
        fields=[];
        errorid=-1;
        useless=0;
        
        [PortfolioName,valid] = w.prepareArrayCommon(PortfolioName);
        if(valid)   [TradeDate,valid] = w.prepareArrayCommon(TradeDate);   end
        if(valid)   [WindCode,valid] = w.prepareArrayCommon(WindCode);         end
        if(valid)   [Quantity,valid] = w.prepareArrayCommon(Quantity);           end
        if(valid)   [CostPrice,valid] = w.prepareArrayCommon(CostPrice);           end
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);   end
        
%         if( iscell(PortfolioName))
%             PortfolioName=PortfolioName{1}
%         end    
%         if( iscell(TradeDate))
%             TradeDate=TradeDate{1}
%         end     
%         if( iscell(WindCode))
%             WindCode=WindCode{1}
%         end 
%         if( iscell(Quantity))
%             Quantity=Quantity{1}
%         end 
%         if( iscell(CostPrice))
%             CostPrice=CostPrice{1}
%         end 
%        %set other parameters, 
%        [Options,callbackfun,useasyn,callbackfunarg] = w.prepareOptions(varargin);

        if(~valid)
            display('The input arguments are not right!');
            return
        end
        
        %fprintf('[data,codes,fields,times,errorid]=gWindData.mWind.wupf_syn(%s,%s,%s,%s,%s,%s)\n',PortfolioName,TradeDate,WindCode,Quantity,CostPrice,Options);
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.wupf_syn(PortfolioName,TradeDate,WindCode,Quantity,CostPrice,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        %data = data;    fields = fields;
        return; 
    end    
      %% [data,codes,fields,times,errorid] = tdays(w,StartTime,EndTime,varargin)
    function [data,codes,fields,times,errorid,useless] = tdays(w,StartTime,EndTime,varargin)
%
%   tdays用来获取指定时间段内的日期序列.
%   如需向导，请使用w.menu('tdays')创建命令.
%   [data,codes,fields,times,errorid] = w.tdays(starttime,endtime,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        starttime      开始日期，格式为 '20120701'.
%        endtime        结束日期，格式为'20120919'，缺省为当前时间
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.tdays(now-5,now)   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        if(nargin<2 || isempty(StartTime))
            StartTime = now;
        end;
        
        if(nargin<3 || isempty(EndTime))
            EndTime=now;
        end;
        [StartTime,EndTime] = w.prepareTime(StartTime,EndTime,1);

        %set other parameters, 
        Options= w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.tdays_syn(StartTime,EndTime,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        return; 
    end  
      %% [data,codes,fields,times,errorid] = tdaysoffset(w,offset,StartTime,varargin)
    function [data,codes,fields,times,errorid,useless] = tdaysoffset(w,offset,StartTime,varargin)
%
%   tdaysoffset用来获取指定时间段内的有效天数.
%   如需向导，请使用w.menu('tdaysoffset')创建命令.
%   [data,codes,fields,times,errorid] = w.tdaysoffset(offset,StartTime,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        offset         偏移值（整数） ，格式为1，-3，+9等
%        starttime      开始日期，格式为 '20120701'.缺省为当前时间
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.tdaysoffset(-5)   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        if(nargin<3 || isempty(StartTime))
            StartTime = now;
        end;
        
        %if(nargin<2 || isempty(offset))
        %    offset=0;
        %end;
        offset=int32(offset);
        
        [StartTime,EndTime] = w.prepareTime(StartTime,now,1);

        %set other parameters, 
        Options= w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.tdaysoffset_syn(StartTime,offset,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        return; 
    end 
  
      %% [data,codes,fields,times,errorid] = tdayscount(w,StartTime,EndTime,varargin)
    function [data,codes,fields,times,errorid,useless] = tdayscount(w,StartTime,EndTime,varargin)
%
%   tdayscount用来获取指定时间段内的有效天数.
%   如需向导，请使用w.menu('tdayscount')创建命令.
%   [data,codes,fields,times,errorid] = w.tdayscount(starttime,endtime,varargin)
% 
%   Description：
%        w	            为创建的windmatlab对象；
%        starttime      开始日期，格式为 '20120701'.
%        endtime        结束日期，格式为'20120919'.缺省为当前时间
% 	 
%        data	        返回的数据结果.
%        codes          返回数据对应的代码.
%        fields         返回数据对应的指标.
%        times          返回数据对应的时间.
%        errorid	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.tdayscount(now-5)   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        data=[];
        id=0;
        codes=[];
        fields=[];
        times=[];
        errorid=-1;
        useless=0;

        if(nargin<2 || isempty(StartTime))
            StartTime = now;
        end;
        
        if(nargin<3 || isempty(EndTime))
            EndTime=now;
        end;
        [StartTime,EndTime] = w.prepareTime(StartTime,EndTime,1);

        %set other parameters, 
        Options= w.prepareOptions(varargin);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [data,codes,fields,times,errorid]=gWindData.mWind.tdayscount_syn(StartTime,EndTime,Options);
        times = times';
        data =w.reshapedata(data,codes,fields,times);
        return; 
    end      
    
    
      %% [Data,Fields,ErrorCode] = bktstart(w,StrategyName, StartDate, EndDate,varargin)
    function [Data,Fields,ErrorCode] = bktstart(w,StrategyName, StartDate, EndDate,varargin)
%   bktstart用来开始一个回测.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktstart(w,StrategyName, StartDate,EndDate,varargin)
%   Description：
%        w	            为创建的windmatlab对象；
%        StrategyName   策略名称，如果与已有策略名称一致则直接使用原策略.
%        StartDate      开始日期，格式为 '20120701' 或者'20120701 00:00:00'.
%        EndDate        结束日期，格式为 '20120919 23:59:59'.缺省为前一天
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktstart('测试1', '20130301','20130808')   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;
        
        if(nargin<3 || isempty(StartDate))
            StartDate = now-2;
        end;
        
        if(nargin<4 || isempty(EndDate))
            EndDate=now-1;
        end;
        
        [StartDate,EndDate] = w.prepareTime(StartDate,EndDate);
        [StrategyName,valid] = w.prepareArrayCommon(StrategyName);
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);   end
       
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        %-------------------------------------
        [Data,Fields,ErrorCode]=gWindData.mWind.bktstart(StrategyName,StartDate,EndDate,Options);
        Data = Data';    Fields = Fields';
        return;
    end      
    

    %% [Data,Fields,ErrorCode] = bktquery(w,qrycode, qrytime,varargin)
    function [Data,Fields,ErrorCode] = bktquery(w,qrycode, qrytime,varargin)
%   bktquery用来查询回测进行中的状态.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktquery(w,qrycode, qrytime,varargin)
%   Description：
%        w	            为创建的windmatlab对象；
%        qrycode        查询代码.'capital','position','order'
%        qrytime        查询时间，格式为 '20120701' 或者'20120701 00:00:00'.
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktquery('capital', '20130808')   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;
        
        if(nargin<2 || isempty(qrycode))
            qrycode='capital';
        end;        
        if(nargin<3 || isempty(qrytime))
            qrytime = '';
        end;
       
        [qrytime,EndTime] = w.prepareTime(qrytime,now);
        [qrycode,valid] = w.prepareArrayCommon(qrycode);
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);   end
       
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.bktquery(qrycode,qrytime,Options);
        Data = Data';    Fields = Fields';
        return;
    end          
      %% [Data,Fields,ErrorCode] = bktorder(w,TradeTime, SecurityCode, TradeSide, TradeVol,varargin)
    function [Data,Fields,ErrorCode] = bktorder(w,TradeTime, SecurityCode, TradeSide, TradeVol,varargin)
%   bktorder 用来对回测下单.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktorder(w,TradeTime, SecurityCode, TradeSide, TradeVol,varargin)
%   Description：
%        w	            为创建的windmatlab对象；
%        TradeTime      下单时间，格式为 '20120701' 或者'20120701 00:00:00'.
%        SecurityCode   交易品种Wind码，如600177.SH
%        TradeSide      交易方向,如 buy,sell,short,cover
%        TradeVol       交易量，
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktorder('20130301','600177.sh','buy',100)   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;
        
        [TradeTime,EndTime] = w.prepareTime(TradeTime,now);
        [SecurityCode,valid] = w.prepareArrayCommon(SecurityCode);
        if(valid)   [TradeSide,valid] = w.prepareArrayCommon(TradeSide);         end
        if(valid)   [TradeVol,valid] = w.prepareArrayCommon(TradeVol);         end
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);   end
       
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        %-------------------------------------
        [Data,Fields,ErrorCode]=gWindData.mWind.bktorder(TradeTime,SecurityCode,TradeSide,TradeVol,Options);
        Data = Data';    Fields = Fields';
        return;
    end    
    
 %% [Data,Fields,ErrorCode] = bktstatus(w,varargin)
    function [Data,Fields,ErrorCode] = bktstatus(w,varargin)
%   bktstatus用来查询回测的情况状态.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktstatus(w,varargin)
%   Description：
%        w	            为创建的windmatlab对象；
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktstatus()   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;

        [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        [Data,Fields,ErrorCode]=gWindData.mWind.bktstatus(Options);
        Data = Data';    Fields = Fields';
        return;
    end    
%% [Data,Fields,ErrorCode] = bktend(w,varargin)
    function [Data,Fields,ErrorCode] = bktend(w,varargin)
%   bktend用来结束回测.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktend(w,varargin)
%   Description：
%        w	            为创建的windmatlab对象；
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktend()   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;

        [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        [Data,Fields,ErrorCode]=gWindData.mWind.bktend(Options);
        Data = Data';    Fields = Fields';
        return;
    end

    %% [Data,Fields,ErrorCode] = bktsummary(w,BktID, View,varargin)
    function [Data,Fields,ErrorCode] = bktsummary(w,BktID, View,varargin)
%   bktsummary用来查询回测结果.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktsummary(w,BktID, View,varargin)
%   Description：
%        w	            为创建的windmatlab对象；
%        BktID          回测ID.
%        View           要查询的内容：KPI，NAV,Trade,Position,PositionRate,PL_Monthly.
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktsummary('111', 'kpi')   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;
  
        if(nargin<3 || isempty(View))
            View = 'KPI';
        end;
       
        [BktID,valid] = w.prepareArrayCommon(BktID);
        if(valid)   [View,valid] = w.prepareArrayCommon(View);         end
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);   end
       
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.bktsummary(BktID,View,Options);
        Data = Data';    Fields = Fields';
        return;
    end  
    
    %% [Data,Fields,ErrorCode] = bktdelete(w,BktID, varargin)
    function [Data,Fields,ErrorCode] = bktdelete(w,BktID, varargin)
%   bktdelete用来删除一个回测.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktdelete(w,BktID, varargin)
%   Description：
%        w	            为创建的windmatlab对象；
%        BktID          回测ID.
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktdelete(123)   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;
  
       
        [BktID,valid] = w.prepareArrayCommon(BktID);
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);   end
       
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.bktdelete(BktID,Options);
        Data = Data';    Fields = Fields';
        return;
    end    
    
%% [Data,Fields,ErrorCode] = bktstrategy(w,varargin)
    function [Data,Fields,ErrorCode] = bktstrategy(w,varargin)
%   bktstrategy用来返回回测列表.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktstrategy(w,varargin)
%   Description：
%        w	            为创建的windmatlab对象；
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktstrategy()   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;

        [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        [Data,Fields,ErrorCode]=gWindData.mWind.bktstrategy(Options);
        Data = Data';    Fields = Fields';
        return;
    end
    
   %% [Data,Fields,ErrorCode] = bktfocus(w,StrategyID, varargin)
    function [Data,Fields,ErrorCode] = bktfocus(w,StrategyID, varargin)
%   bktfocus用来关注一个策略.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktfocus(w,StrategyID, varargin)
%   Description：
%        w	            为创建的windmatlab对象；
%        StrategyID     策略ID.
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktfocus(123)   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;
  
       
        [StrategyID,valid] = w.prepareArrayCommon(StrategyID);
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);   end
       
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.bktfocus(StrategyID,Options);
        Data = Data';    Fields = Fields';
        return;
    end     
    
  %% [Data,Fields,ErrorCode] = bktshare(w,StrategyID, varargin)
    function [Data,Fields,ErrorCode] = bktshare(w,StrategyID, varargin)
%   bktshare用来共享一个策略.
%   如需向导，请使用w.menu('bkt')创建命令.
%   [Data,Fields,ErrorCode] = bktshare(w,StrategyID, varargin)
%   Description：
%        w	            为创建的windmatlab对象；
%        StrategyID     策略ID.
% 	 
%        Data	        返回的数据结果.
%        Fields         返回数据对应的指标.
%        ErrorCode	    函数运行的错误ID.
% 
%   Example：	    
% 	      w.bktfocus(123)   
%
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            display('Wind object is not initialized! Please use w=windmatlab to create wind object firstly!')
            return ;            
        end            
        Data=[];       Fields=[];      ErrorCode=-1;
  
       
        [StrategyID,valid] = w.prepareArrayCommon(StrategyID);
        if(valid) [Options,callbackfun,useasyn,callbackfunarg,valid] = w.prepareOptionsArrayCommon(varargin);   end
       
        if(~valid)
            display('The input arguments are not right!');
            return
        end        

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        [Data,Fields,ErrorCode]=gWindData.mWind.bktshare(StrategyID,Options);
        Data = Data';    Fields = Fields';
        return;
    end    
  end
    
  methods (Access = 'private')
    %%
     function [dataout,valid,count]=prepareArray(w,datain)
        dataout='';
        valid =1;
        sep='';
        [row,col]=size(datain);

        if( ischar(datain))
            for r=1:row
                dataout=strcat(dataout,sep,datain(r,:));
                sep='$$';
            end
            count = row;
            return;
        end
        
        count = row*col;
        if( isnumeric(datain))
            for r=1:row
                for c=1:col
                    dataout=strcat(dataout,sep,num2str(datain(r,c)));
                    sep='$$';
                end
            end

            return;
        end 

        if(iscell(datain))
            for( r=1:row)
                for c=1:col
                   v=datain{r,c};
                   if(isnumeric(v))
                       dataout=strcat(dataout,sep,num2str(v(1)));
                   elseif ischar(v)
                       dataout=strcat(dataout,sep,v(1,:));
                   else
                       valid = 0;
                       return 
                   end
                   sep='$$';
                end
            end
            return;
        end
        valid = 0;
        return 
     end
    
    function [dataout,valid,count]=prepareArrayCommon(w,datain)
        dataout='';
        valid =1;
        sep='';
        [row,col]=size(datain);

        if( ischar(datain))
            for r=1:row
                dataout=strcat(dataout,sep,datain(r,:));
                sep=',';
            end
            count = row;
            return;
        end
        
        count = row*col;
        if( isnumeric(datain))
            for r=1:row
                for c=1:col
                    dataout=strcat(dataout,sep,num2str(datain(r,c)));
                    sep=',';
                end
            end

            return;
        end 

        if(iscell(datain))
            for( r=1:row)
                for c=1:col
                   v=datain{r,c};
                   if(isnumeric(v))
                       dataout=strcat(dataout,sep,num2str(v(1)));
                   elseif ischar(v)
                       dataout=strcat(dataout,sep,v(1,:));
                   else
                       valid = 0;
                       return 
                   end
                   sep=',';
                end
            end
            return;
        end
        valid = 0;
        return 
    end
    %%
    function [Options,callbackfun,useasyn,callbackfunarg,valid] = prepareOptionsArray(w,inputs)
        Options='';
        prefstr='';
        callbackfun = [];
        callbackfunarg ='';
        valid=1;
        numin = length(inputs);
        i = 1;
        while i <= numin
            if strcmpi(inputs{i},'callbackfunc')
                callbackfun = inputs{i+1};
            elseif strcmpi(inputs{i},'callbackfunarg')
                callbackfunarg = inputs{i+1};
            elseif strfind(inputs{i},'=')
                Options = strcat(Options,prefstr,inputs{i});
                prefstr=';';
                i = i + 1;
                continue;
            else
                if(i==numin)
                    break;
                end
                [dataout,valid] = w.prepareArray(inputs{i+1});
                if(~valid)
                    display('invalid options......');
                    return;
                end
                Options = strcat(Options,prefstr,inputs{i},'=',dataout);
                prefstr=';';                
            end;
            i = i + 2;
        end

        useasyn=1;
        if(isempty(callbackfun))
            useasyn = 0;
            global gWindData;
            callbackfun =@gWindData.dealDataFunc;
        end        
    end    

    function [Options,callbackfun,useasyn,callbackfunarg,valid] = prepareOptionsArrayCommon(w,inputs)
        Options='';
        prefstr='';
        callbackfun = [];
        callbackfunarg ='';
        valid=1;
        numin = length(inputs);
        i = 1;
        while i <= numin
            if strcmpi(inputs{i},'callbackfunc')
                callbackfun = inputs{i+1};
            elseif strcmpi(inputs{i},'callbackfunarg')
                callbackfunarg = inputs{i+1};
            elseif strfind(inputs{i},'=')
                Options = strcat(Options,prefstr,inputs{i});
                prefstr=';';
                i = i + 1;
                continue;
            else
                if(i==numin)
                    break;
                end
                [dataout,valid] = w.prepareArrayCommon(inputs{i+1});
                if(~valid)
                    display('invalid options......');
                    return;
                end
                Options = strcat(Options,prefstr,inputs{i},'=',dataout);
                prefstr=';';                
            end;
            i = i + 2;
        end

        useasyn=1;
        if(isempty(callbackfun))
            useasyn = 0;
            global gWindData;
            callbackfun =@gWindData.dealDataFunc;
        end        
    end        

    function printFunc(w,reqid,isfinished,errorid,datas,codes,fields,times,selfdata)
        %fprintf('dealDataFunc begin-\n');
        %fprintf('dealDataFunc begin--%d,%d,%d,%s\n',reqid,isfinished,errorid,codes{1});
        
        global gWindData;     
        
        %fprintf('dealDataFunc begin2-\n');        
%         mapdata= gWindData.mWaitFunctions(reqid);
%         
%         if(isfinished)
%             gWindData.mWaitFunctions.remove(reqid);
%         end
        
          if(length(codes)==0|| length(fields)==0||length(times)==0)
          else
              if(length(times)==1)
                  datas = reshape(datas,length(fields),length(codes))';
              elseif (length(codes)==1)
                  datas = reshape(datas,length(fields),length(times))';
              elseif(length(fields)==1)
                  datas = reshape(datas,length(codes),length(times))';
%               else
%                   data = mapdata{5};
              end
          end
       
        %disp(datas);
        %disp(codes);
        %disp(fields);
        %disp(times);
%         datestr(times,'yyyymmdd HH:MM:SS');
%         disp(errorid);
%         disp(reqid);          
        if(isempty(selfdata))

            return;
        end
        
        vars = regexp(selfdata,'\w*','match');
        
        outvars='';
        prefstr='';
        if(length(vars)>=1)
            assignin('base', vars{1}, datas) 
            fprintf('%s = \n', vars{1});
            disp(datas);
%             outvars=strcat(outvars,prefstr,vars{1});
%             prefstr=',';
        end
        
        if(length(vars)>=2)
            assignin('base', vars{2}, codes) 
            fprintf('%s = \n', vars{2});
            disp(codes);
%             outvars=strcat(outvars,prefstr,vars{2});
%             prefstr=',';
        end
        if(length(vars)>=3)
            assignin('base', vars{3}, fields) 
            fprintf('%s = \n', vars{3});
            disp(fields);
%             outvars=strcat(outvars,prefstr,vars{3});
%             prefstr=',';            
        end
        if(length(vars)>=4)
            assignin('base', vars{4}, times) 
            fprintf('%s = \n', vars{4});
            disp(datestr(times,31));
            fprintf('\n');
%             outvars=strcat(outvars,prefstr,vars{4});
%             prefstr=',';               
        end
        if(length(vars)>=5)
            assignin('base', vars{5}, errorid) 
            fprintf('%s = %d\n\n', vars{5}, errorid);
%             outvars=strcat(outvars,prefstr,vars{5});
%             prefstr=',';                 
        end
        if(length(vars)>=6)
            assignin('base', vars{6}, reqid) 
            fprintf('%s = %d\n\n', vars{6}, reqid);
%             outvars=strcat(outvars,prefstr,vars{6});
%             prefstr=',';                 
        end        
        
%         if(~isempty(prefstr))
%             evalin('base',prefstr);
%         end
        
        return;        
    end       
    function stateChangedCallback(w,varargin)
        global gWindData;        
        % 	enSCS_None = 0
        % 	,enSCS_Request_newData = 1	
        % 	,enSCS_Request_finished= 2	
        % 	,enSCS_Connection_state=3        
        %stateChanged = void stateChanged(int32 state, uint64_T requestid, int32 errorCode)
        %'stateChangedCallback'
        state = varargin{3};
        requestid = varargin{4};
        errorCode = varargin{5};

        %state,requestid,errorCode
        %fprintf('stateChangedCallback--begin:%d,%d,%d\n',state,requestid,errorCode);

        if(state == 3)
            gWindData.mSessionStateChangedCount = gWindData.mSessionStateChangedCount + 1;
            if(errorCode == 7)%MTSessionLoginFailure)
                gWindData.mLoginFailure = 1;
            elseif(errorCode == 8)%MTSessionNoAuthority
                gWindData.mLoginFailure = 2;
            elseif(errorCode == 9) 
                gWindData.mLoginFailure = 10;
            end
        elseif(state==1||state==2)%enSCS_Request_newData || state==enSCS_Request_finished)
            if ~gWindData.mWaitFunctions.isKey(requestid)
                %fprintf('stateChangedCallback:No dealing function for%d,%d,%d\n',state,requestid,errorCode);
                
                gWindData.mWind.cleardata(requestid);
                return;                
            end
            mapdata= gWindData.mWaitFunctions(requestid);
            %gWindData.mWaitFunctions(reqid) = {callbackfun1,isupdate2,isfinished3,terrorid4,tdatas5,tcodes6,tfields7,ttimes8,useasyn};
            
            %VARIANT readdata(ULONGLONG requestid, [out]VARIANT* codes, [out]VARIANT* fields
			%	                   , [out]VARIANT* times, [out]LONG* requestState, [out]LONG* errorCode);
            [outdata,outcodes,outfields,outtimes,outstate,outerror]=gWindData.mWind.readdata(requestid);
            
            if(outerror==0)
                outdata = reshape(outdata,length(outfields),length(outcodes),length(outtimes));
                outtimes = outtimes';
            else
                %fprintf('stateChangedCallback--outerror:%d,%d,%d,%s\n',state,requestid,errorCode,outdata{1});
                %outdata,outcodes,outfields,outtimes,outstate,outerror
            end
            
            %mapdata{1}(requestid,outstate,outerror,outdata,outcodes,outfields,outtimes);
            isfinish=0;
            if(state==2)
                isfinish = 1;
            end
            
            %fprintf('stateChangedCallback--mapdata{1}:%d,%d,%d,%s\n',state,requestid,errorCode,outcodes{1});
            %if( isempty(mapdata{10}))
            %    mapdata{1}(requestid,isfinish,outerror,outdata,outcodes,outfields,outtimes);
            %else
            %    mapdata{1}(requestid,isfinish,outerror,outdata,outcodes,outfields,outtimes,mapdata{10});
            %end
            %fprintf('stateChangedCallback--mapdata{1}:%d,%d,%d\n',state,requestid,errorCode);
            
            if( isa(mapdata{1},'function_handle'))
                try
                    if( isempty(mapdata{10}))
                        mapdata{1}(requestid,isfinish,outerror,outdata,outcodes,outfields,outtimes);
                    else
                        mapdata{1}(requestid,isfinish,outerror,outdata,outcodes,outfields,outtimes,mapdata{10});
                    end
                catch funcerr
                    'call back error:'
                    funcerr.message
                end          
            end
            
            
            if(mapdata{9})
                if(outstate==2) %enSCS_Request_finished
                    gWindData.mWaitFunctions.remove(requestid);
                    return;
                else
                    %%%%do nothing.....
                end
            else
                %%%do nothing......
            end
        end
        %fprintf('stateChangedCallback--end:%d,%d,%d\n',state,requestid,errorCode);
        %fprintf('.');
    end

    function helpReport(varargin)
        global gWindData;          
        %void helpReport(string helpstring,LONG runit)
        %'helpReport'
        fprintf('%s\n',varargin{4});

        try
            if(varargin{5} == -1)
                evalin('base',varargin{4});
            elseif(varargin{5}~=0)
                %gWindData.mToRunCmd = [gWindData.mToRunCmd; varargin{4}];
                astr=char(strtrim(varargin{4}));

                evalin('base',astr);
            end
        catch funcerr
                'helpReport error:'
                funcerr.message
        end     
    end
    
    function timerFunc(w, event, string_arg)
        global gWindData;    
        %fprintf('timerFunc--begin\n');

        if(isempty(gWindData))
            aa=timerfind('Name','gWindDataTimer');
            stop(aa);
            delete(aa);
            'isempty(gWindData)';
            return;
        end
        if gWindData.mLoginFailure == 10
            aa=timerfind('Name','gWindDataTimer');
            stop(aa);
            t = timer('TimerFcn', @ClearTimerFcn, 'StartDelay', 1, 'ExecutionMode', 'singleShot');
            start(t);
            return;
        end
%         while(length(gWindData.mToRunCmd) > 1)
%             astr=char(strtrim(gWindData.mToRunCmd(2)));
%             gWindData.mToRunCmd(2,:) = [];
%              eval(astr);
%         end

        if ishandle(gWindData.mWind)
            %'timerFunc'
%%%            gWindData.mWind.FireEvents(100);
        else
            stop(gWindData.mTimer);
        end
        %fprintf('timerFunc--end\n');
    end    
    function dealDataFunc(w,reqid,isfinished,errorid,datas,codes,fields,times,selfdata)
        %fprintf('dealDataFunc begin-\n');
        %fprintf('dealDataFunc begin--%d,%d,%d,%s\n',reqid,isfinished,errorid,codes{1});
        
        global gWindData;     
        if ~gWindData.mWaitFunctions.isKey(reqid)
            fprintf('No dealing function for %d\n',reqid);
            return;                
        end        
        
        %fprintf('dealDataFunc begin2-\n');        
        mapdata= gWindData.mWaitFunctions(reqid);
        %gWindData.mWaitFunctions(reqid) = {callbackfun1,isupdate2,isfinished3,terrorid4,tdatas5,tcodes6,tfields7,ttimes8,useasyn};
        if(isempty(mapdata))
            fprintf('dealDataFunc end if(isempty(mapdata))--%d,%d,%d,%s\n',reqid,isfinished,errorid,codes{1});
            return;
        end
        
        mapdata{2} =1;
        mapdata{3} =isfinished;
        mapdata{4} =errorid;

        if( isempty(mapdata{5}) && isempty(mapdata{6}) &&isempty(mapdata{7}) &&isempty(mapdata{8}))
            mapdata{5} = datas;
            mapdata{6} = codes;
            mapdata{7} = fields;
            mapdata{8} = times;
            gWindData.mWaitFunctions(reqid) = mapdata;
            
            %fprintf('dealDataFunc end isempty(mapdata{5}) && isempty(mapdata{6}) &&isempty(mapdata{7}) &&isempty(mapdata{8}--%d,%d,%d,%s %d\n',reqid,isfinished,errorid,codes{1});
            return;
        end
        
        if( isequal(fields,mapdata{7}))
            if(isequal(codes,mapdata{6}) )
                mapdata{5} = cat(3,mapdata{5} ,datas);
                mapdata{8} = cat(1,mapdata{8} ,times);
            elseif(isequal(times,mapdata{8}) )
                mapdata{5} = cat(2,mapdata{5} ,datas);
                mapdata{6} = cat(1,mapdata{6} ,codes);     
            else
                fprintf('Error:wind:dealDataFunc error: same fields,but not same times or codes\n');   
                gWindData.mWaitFunctions(reqid) = mapdata;
                %error('wind:dealDataFunc error: same fields,but not same times or codes');
                return;
            end
        elseif(isequal(codes,mapdata{6}) )
            if(isequal(times,mapdata{8}) )
                mapdata{5} = cat(1,mapdata{5} ,datas);
                mapdata{7} = cat(1,mapdata{7} ,fields);     
            else
                fprintf('Error:wind:dealDataFunc error: same code,but not same times or fields\n');   
                gWindData.mWaitFunctions(reqid) = mapdata;
                %error('wind:dealDataFunc error: same code,but not same times or fields');
                return;
            end
        else
            fprintf('Error:wind:dealDataFunc error: has no same fields or codes\n');   
            gWindData.mWaitFunctions(reqid) = mapdata;
            %error('wind:dealDataFunc error: has no same fields or codes');
            return;            
        end
        
        gWindData.mWaitFunctions(reqid) = mapdata;
        
        %fprintf('dealDataFunc end --%d,%d,%d,%s\n',reqid,isfinished,errorid,codes{1});
        return;        
    end    
    function [Options,callbackfun,useasyn,callbackfunarg] = prepareWsqOptions(w,inputs)
        Options='';
        useasyn = 1;
        callbackfun = [];
        callbackfunarg ='';
        numin = length(inputs);
        if (numin == 1)
            callbackfun = inputs{1};%callback;
            Options = 'realtime=y';
        elseif numin >= 2
            callbackfun = inputs{1};
            callbackfunarg = inputs{2};
            Options = 'realtime=y';
            %for i=1:2:numin
            %    if strcmpi(inputs{i},'callbackfunc')
            %        callbackfun = inputs{i+1};
            %    elseif strcmpi(inputs{i},'callbackfunarg')
            %        callbackfunarg = inputs{i+1};
            %    end
            %end
        end
            
        if(isempty(callbackfun))
            useasyn = 0;
            global gWindData;
            callbackfun =@gWindData.dealDataFunc;
        end        
    end
    function [Options,callbackfun,useasyn,callbackfunarg] = prepareOptions(w,inputs)
        Options='';
        prefstr='';
        callbackfun = [];
        callbackfunarg ='';
        numin = length(inputs);
        i = 1;
        while i <= numin
            if strcmpi(inputs{i},'callbackfunc')
                callbackfun = inputs{i+1};
            elseif strcmpi(inputs{i},'callbackfunarg')
                callbackfunarg = inputs{i+1};
            elseif strfind(inputs{i},'=')
                Options = strcat(Options,prefstr,inputs{i});
                prefstr=';';
                i = i + 1;
                continue;
            else
                if(i==numin)
                    break;
                end
                Options = strcat(Options,prefstr,inputs{i},'=',inputs{i+1});
                prefstr=';';                
            end;
            i = i + 2;
        end

        useasyn=1;
        if(isempty(callbackfun))
            useasyn = 0;
            global gWindData;
            callbackfun =@gWindData.dealDataFunc;
        end        
    end    
    function [WindCodes,WindFields] = prepareCodeAndField(w,codes,fields)
        WindCodes = codes;
        WindFields = fields;
        if(isempty(WindCodes) || isempty(WindFields) )
            error('windmatlab:prepareCodeAndField error:isempty(WindCodes) || isempty(WindFields)');
            return;
        end

        if ~ischar(WindCodes) 
            if ~iscell(WindCodes)
                error('windmatlab:prepareCodeAndField error:~ischar(WindCodes) /~iscell(WindCodes) ');
                return;
            end

            l='';
            prefstr='';
            for(i=1:length(WindCodes))
                l=strcat(l,prefstr,WindCodes{i});
                prefstr=',';
            end
            WindCodes = l;
        end

        if ~ischar(WindFields) 
            if ~iscell(WindFields)
                error('windmatlab:prepareCodeAndField error:~ischar(WindFields) /~iscell(WindFields) ');
                return;
            end

            l='';
            prefstr='';
            for(i=1:length(WindFields))
                l=strcat(l,prefstr,WindFields{i});
                prefstr=',';
            end
            WindFields = l;
        end    
        return;              
    end
    function [StartTime,EndTime] = prepareTime(w,starttime,endtime,isdate)
        StartTime = starttime;
        EndTime = endtime;
        if(nargin<4)
            isdate=0;
        end
        
        if ~ischar(StartTime) 
            if(isdate)
                StartTime = datestr(StartTime,'yyyymmdd');    
            else
                StartTime = datestr(StartTime,'yyyymmdd HH:MM:SS');
            end
        end
        
        
        if ~ischar(EndTime) 
            if(isdate)
                EndTime = datestr(EndTime,'yyyymmdd');    
            else
                EndTime = datestr(EndTime,'yyyymmdd HH:MM:SS');
            end
        end

        return;              
    end
    function [totalOptions] = toOptions(w,StartTime,EndTime,Options)
        totalOptions='';
        prefstr='';
        if(~isempty(StartTime))
            totalOptions = strcat(totalOptions,prefstr,'StartTime=',StartTime);
            prefstr=';';
        end
        if(~isempty(EndTime))
            totalOptions = strcat(totalOptions,prefstr,'EndTime=',EndTime);
            prefstr=';';
        end    
        if(~isempty(Options))
            totalOptions = strcat(totalOptions,prefstr,Options);%
            prefstr=';';
        end                 
    end    
    function data =reshapedata(w,indata,codes,fields,times)
              if(isempty(codes)|| isempty(fields)||isempty(times))
                    data = indata;
                    return;
              end
              
              if(length(times)==1)
                  data = reshape(indata,length(fields),length(codes))';
              elseif (length(codes)==1)
                  data = reshape(indata,length(fields),length(times))';
              elseif(length(fields)==1)
                  data = reshape(indata,length(codes),length(times))';
              else
                  data = indata;
              end
    end
    function waitdata_asyn(w,reqid,inerrorid,callbackfun,callbackfunarg)
       global gWindData;        
       if(reqid~=0 && inerrorid ==0)
            %//request ok
             %gWindData.mWaitFunctions(reqid) = {callbackfun,isupdate,isfinished,terrorid,tdatas,tcodes,tfields,ttimes,useasyn};
            gWindData.mWaitFunctions(reqid) = {callbackfun,0,0,0,[],[],[],[],1,callbackfunarg}; 
            %start(gWindData.mTimer);
        else
            %start(gWindData.mTimer);
            fprintf('windmatlab:waitdata_asyn error: if(reqid~=0 && errorid ==0) ');
            return;
       end
    end
   
  end  %end private methods
end

function ClearTimerFcn(w, event)
    global gWindData;    

    if(isempty(gWindData))
        aa=timerfind('Name','gWindDataTimer');
        stop(aa);
        delete(aa);
        'isempty(gWindData)';
    else
        fprintf('windmatlab: Network failure, please rebuild the windmatlab object!\n');
        aa=timerfind('Name','gWindDataTimer');
        stop(aa);
        %wait(aa);
        %delete(aa);
        gWindData.clear;
    end            
end

