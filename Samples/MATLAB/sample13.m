% 下单
% 第1版  张树德编写     （sdzhang@wind.com.cn）   2013年9月5日
%% 1. 单账号下单
% 登录时生成的登录号为1 现需要退出登录号1.
[Data3]=w.torder('600276.SH', 'Buy',32, 100, 'OrderType=LMT;HedgeType=SPEC','LogonID',1)
%% 2. 多资产多账户下单
% 登录号有2个，分别为2与3。
[Data4]=w.torder({'600276.SH';'600267.SH'}, {'sell';'Buy'},[32,19], 100, 'OrderType=LMT;HedgeType=SPEC','LogonID',[2 3])
% 登录号有2个，分别为2与3。
[Data4]=w.torder({'600276.SH';'600267.SH'}, {'sell';'Buy'},[32,19], 100, 'OrderType=LMT;HedgeType=SPEC','LogonID',{3})
