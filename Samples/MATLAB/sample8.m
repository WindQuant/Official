% 【例8】提取银行间国债09年07附息券（090007.IB）的全价、应计利息、估价修正久期，数据来源为中证指数公司，
%  对应的字段为dirty_csi、accruedinterest_csi、modidura_csi。日期为2013年4月6日至5月6日。
% 第1版    张树德（sdzhang@wind.com.cn）  2013年7月5日
w=windmatlab
[w_wsd_data,w_wsd_codes,w_wsd_fields,w_wsd_times,w_wsd_errorid]=w.wsd('090007.IB','dirty_csi,accruedinterest_csi,modidura_csi','2013-04-06','2013-05-06')
% 注意，目前支持中债公司、中证指数公司、清算所的债券估价，中债公司需要取得授权，清算所的数据较少。


