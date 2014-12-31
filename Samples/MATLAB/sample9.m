%{
功能：读取资管资管账户及客户自选股组合数据
目前可以读取万得资讯终端中的关注组合、我的自选股等的账户报表数据。
第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
%}
% 例如某用户终端中资管中选择了名为“130325”的组合，现在将该组合的统计数据读出来。
% 选择的报表为“组合结算数据”，报表字段为：Portfolio_Name（组合名称）、Portfolio_ID（组合ID）、Total_Asset（总资产）。
[w_wpf_data,w_wpf_codes,w_wpf_fields,w_wpf_times,w_wpf_errorid,w_wpf_reqid]=w.wpf('130325','PMS.PortfolioDaily','startdate=20130503;enddate=20130603;reportcurrency=CNY;owner=;field=Portfolio_Name;Portfolio_ID;Total_Asset')















