% 【例7】提取海正药业（600276.SH）、恒瑞医药（600276.SH）、双鹭药业（002038.SZ）、天士力（600535.SH）2012年年报中的营业收入、营业利润、净利润数据，数据来源为合并报表。
w=windmatlab
[w_wss_data,w_wss_codes,w_wss_fields,w_wss_times,w_wss_errorid]=w.wss('600267.SH,600276.SH,002038.SZ,600535.SH','oper_rev,opprofit,net_profit_is','rptDate=20121231','rptType=1')
% 其中，营业收入、营业利润、净利润对应的字段为oper_rev、opprofit、net_profit_is，报告期为2012年12月31日（rptDate=20121231），财务报表为合并报表（rptType=1）。



