%   功能:测试均线MA策略收益。
% 1、遍历所选时间段所有的a股，当满足该股5日均线上穿30日均线时候，买入。
% 2、持有到该时间段内跌破股价10日均线时卖出；
% 3、若截止时间段末尾，仍没有跌破10日均线，则按最后收盘价计算收益率。
% 4、统计该时间段内每天的收益率
% 第一版  张树德 2013年7月29日
% 
clear
w=windmatlab;
[w_wset_data,w_wset_codes,w_wset_fields,w_wset_times,w_wset_errorid,w_wset_reqid]=w.wset('SectorConstituent','date=20130728;sector=CSI金融地产');
[m,n]=size(w_wset_data);
for i=1:60
data   = w.wsd(w_wset_data{i,2},'close',today-100,today);
[MA5]  = w.wsd(w_wset_data{i,2},'EXPMA',today-100,today,'EXPMA_N=5','Fill=Previous');
[MA10] = w.wsd(w_wset_data{i,2},'EXPMA',today-100,today,'EXPMA_N=10','Fill=Previous');
[MA30] = w.wsd(w_wset_data{i,2},'EXPMA',today-100,today,'EXPMA_N=30','Fill=Previous');
%%  策略
open=0;
for j=1:length(data)
   if MA5(j)>=MA30(j) && open==0
   open=data(j);
   ret(j)=0;
   elseif  data(j)<MA10(j) && open>0
   ret(j)= log(data(j)/open);
   open=0;
   else
   ret(j)=0; 
   end    
end
if open>0;ret(end)= log(data(end)/open);end
blotReturn(:,i)=ret(:);
end
sumReturn=cumsum(blotReturn);
Data{1,1}='策略名称';
Data{1,2}='平均收益';
Data{2,1}='均线策略';
Data{2,2}=num2str(nanmean(sumReturn(end,:)));
Data{3,1}='股票名称';
Data{3,2}='区间收益';
Data=[Data;[w_wset_data(1:60,3),mat2cell(sumReturn(end,:)',ones(60,1),1)]];
h1=figure('Position', [500 300 305 455],'Name','均线策略统计');
uitable('Parent',h1,'Data',Data,'fontsize',15,  'Position', [10 10 300 450],'ColumnWidth',{110 150 });


