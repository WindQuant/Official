% 撤单
%第1版  张树德编写     （sdzhang@wind.com.cn）   2013年9月5日
% 登录股票账号
[Data1]=w.tlogon('0000', '0',   {'W081316502';'W081316501'}, {'123456';'123456'}, {'CFE';'SHSZ'});
pause(10)
%% 买入600276.SH股票100股，价格31元。
[Data2]=w.torder({'600276.SH';'600267.SH'}, {'Buy';'Buy'},[32,15], 100, 'OrderType=LMT;HedgeType=SPEC','LogonID',Data1{2,1});
pause(10)
%% 查询委托
[Data3,Fields,ErrorCode]=w.tquery('Order', 'LogonId',Data1{2,1},'RequestID',Data2(:,1));
pause(10)
%% 撤销 600267.SH委托
[Data4,Fields,ErrorCode]=w.tcancel(Data3(:,1), 'LogonID',Data1{2,1});
pause(10)
%% 查询委托是否被撤销。
[Data3,Fields,ErrorCode]=w.tquery('Order', 'LogonId',Data1{2,1},'RequestID',Data2(:,1));


