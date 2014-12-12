function Backtest(StrategyFunc,windcode,start_time,end_time,k_interval)
    % 加载K线数据
    DB = LoadKline(windcode,start_time,end_time,k_interval);    
    % 初始化资产池
    Asset = InitAsset(DB);
    
    % 按K线循环
    NK = DB.NK;
    for K = 1:NK
        DB.CurrentK = K; %当前K线
        Signal = StrategyFunc(DB); %运行策略函数，生成交易信号
        if( Asset.CurrentPosition ~= 100 && strcmp(Signal.Action,'BUY') == 0)
            Asset = Buy(DB,Asset,100,NaN,'Close'); % 按收盘价买
        elseif( Asset.CurrentPosition ~= -100 && strcmp(Signal.Action,'BUY') == 0 )
            Asset = Sell(DB,Asset,100,NaN,'Close');
        end
        
        % 每条K线在运行结束时都要清算
        Asset = Clearing(DB,Asset);
    end
    
    Summary(DB,Asset);
end