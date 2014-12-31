%{
功能：自动获取历史财务数据
%}
clc;
w= windmatlab;
%获取全市场股票代码
codes=w.wset('SectorConstituent','date=20131105;sector=全部A股'); 
rowcount=length(codes);
%全市场所有A股2010年至2013年，各季度的财务数据，财务指标如下：
%EBITDA
%年度分红
%主营业务收入
%长期借款
%短期借款
%获取报告期时间序列，注意选取日历日
w_tdays=w.tdays('2013-01-01','2013-09-30','Days=Alldays;Period=Q'); % 自动获取季度日期
colcount = length(w_tdays);
dataresult=zeros(rowcount,5,colcount);
for i=1:colcount
    rptdate=datestr(w_tdays(i),'yyyymmdd');
    year=datestr(w_tdays(i),'yyyy');
    dataresult(:,:,i)=w.wss(codes(:,2),'ebitda2,div_aualaccmdiv,oper_rev,lt_borrow,st_borrow','rptDate',rptdate,'rptType=1','year',year);
    fprintf('\n已经下载%s报告期的数据',rptdate);
end