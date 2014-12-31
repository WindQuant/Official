function w_errorid=getRealtimeWSQ(w,codes)
    %本demo程序实现实时订阅一些股票的交易信息，订阅的信息通过回调函数实时显示（见WSQCallback回调函数）。
    %主要使用函数wsq，用户可以通过w.menu('wsq')创建wsq命令

    if nargin<2
        %准备需要获取信息的股票代码
        codes='AUDUSD.FX';%600000.SH,000001.SZ,000002.SZ';
    end

    if nargin<1
        %建立Wind对象；
        global gWindData;
        if ~isa(gWindData,'windmatlab')
            w=windmatlab;
        else
            w=gWindData;
        end
    end
    

    %确定指标
    %用户可以通过w.menu('wsq')
    %rt_last 现价，rt_last_vol 现量
    %rt_ask1 卖1价，rt_asize1 卖1量
    %rt_bid1 买1价，rt_bsize1 买1价
    fields='rt_last,rt_last_vol,rt_ask1,rt_asize1,rt_bid1,rt_bsize1';

    [w_data,w_codes,w_fields,w_times,w_errorid,w_reqid] =w.wsq(codes,fields,@WSQCallback);

    %等待自动调用回调函数
    fprintf('Waiting for the data. Subscription will be terminated in 60 seconds.\n');
    %fprintf('正在等待回调函数被调用，60秒后自动退出等待，并终止订阅\n');

    endtime = now + 60.0/(3600*24);
    while now < endtime
        pause(1);
        fprintf('.');
    end

    %您可以使用w.cancelRequest命令可以停止订阅
    w.cancelRequest(w_reqid)

    %fprintf('\n已经停止订阅\n');
    fprintf('\nSubscription is expired.\n');

    %如果不再使用wind对象，您可以断开它
    %w.close
end
