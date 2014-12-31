% 查询
% 第1版  张树德编写     （sdzhang@wind.com.cn）   2013年9月5日
%% 查询都是通过登录号进行的。
%% 1. 资金查询
[Data6,Fields,ErrorCode]=w.tquery('Capital', 'LogonId=1')
%% 2. 登录号（LogonID）查询
[Data7,Fields,ErrorCode]=w.tquery('LogonID', 'LogonId=3')
%% 3. 委托查询
% 用户查询LogonID=3，Request=17的委托及成交情况。
[Data8,Fields,ErrorCode]=w.tquery('Order', 'LogonId',3,'RequestID',Data4{2})
% 用户查询LogonID=3，Request=17,19的委托及成交情况。
[Data9,Fields,ErrorCode]=w.tquery('Order', 'LogonId=3','RequestID',[18 19])
%% 4.成交查询
%% 注意成交目前仅支持登录号（LogonID）查询
[Data10,Fields,ErrorCode]=w.tquery('Trade', 'LogonId=3');
%% 5. 持仓查询
[Data11,Fields,ErrorCode]=w.tquery('Position', 'LogonId=3');
%% 6. 账号
[Data12,Fields,ErrorCode]=w.tquery('Account', 'LogonId=3');