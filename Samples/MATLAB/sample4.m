% 提取浦发银行（600000.SH）、万科A（000002.SZ）、宝安A（000009.SZ）、南玻A（000012.SZ）、长城开发（000021.SZ）2012年11月30号的基本特征字段，
% 包括公司名称、公司英文名称、IPO日期、流通股、净流入资金、流入量，相应的字段为comp_name,comp_name_eng,ipo_date,float_a_shares,mf_amt,mf_vol。
% 创建windmatlab对象，如果已经创建，则不再重新创建；
w= windmatlab;
codes='600000.SH,000002.SZ,000009.SZ,000012.SZ,000021.SZ';
fields='comp_name,comp_name_eng,ipo_date,float_a_shares,mf_amt,mf_vol';
[wdata, codes, fields, times, errorid, reqid] = w.wss(codes,fields,'tradedate','20121130');
% 其中，’tradedate’表示交易日期。
