% 功能:查找2013年9月16日股价在200日均线上方的股票代码及名称。
% 第一版  张树德 2013年7月29日
clear
w=windmatlab;
w_wset_data=w.wset('SectorConstituent','date=20130728;sector=CSI金融地产');
%% 读取股票收盘价与200日均线（MA_N=200）价格。
Data1=w.wss(w_wset_data(:,2),'close;MA','tradeDate=20130916','MA_N=200','priceAdj=F','cycle=D');
N=Data1(:,1)>Data1(:,2);
%% 股价运行在200日均线上方的股票
List=w_wset_data(N,2:3);
%% 股价运行在200日下方的股票
h1=figure('Position', [500 400 422 550],'Name','股价在200日均线上方股票');
uitable('Parent',h1,'Data',List,'fontsize',15,  'Position', [25 40 420 500],'ColumnWidth',{180 190 });





