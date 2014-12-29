function DB = LoadKline(windcode,start_time,end_time,interval)
% 运行量化接口取分钟线
w = windmatlab;
[w_wsi_data,w_wsi_codes,w_wsi_fields,w_wsi_times,w_wsi_errorid,w_wsi_reqid]=w.wsi(windcode,'open,high,low,close',start_time,end_time);
% 开高低收
DB.Open = w_wsi_data(:,1);
DB.High = w_wsi_data(:,2);
DB.Low  = w_wsi_data(:,1);
DB.Close= w_wsi_data(:,2);
% 定位游标位置到第一条K线
DB.CurrentK = 1;
% K线总数
DB.NK = length(DB.Open);
end