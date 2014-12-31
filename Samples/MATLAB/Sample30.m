% 看看你是否是股指期货高手 
%{
使用说明。本程序是股指期货短线操作游戏，供用户娱乐之用，在股指期货交易期间使用。
         上面是股指期货行情曲线，可以先点击“买入指数”,也可以点击“卖出指数”。
         下面是买卖盈亏统计图。如果产生了灵感千万别忘了告诉我们。
第1版  张树德编写     （sdzhang@wind.com.cn）   2013年10月31日

%}

function Sample30
%% 下面做一个绘制股票价格实时控件图。
clear global h11 h12 h22 Rtprice  Profit Profit1 M Buy
global h11 h12 h22 Rtprice  Profit Profit1 M Buy

M.m1=0;M.m2=0;M.m3=0;M.m4=0;
Profit=0;Profit1=0;
Rtprice=nan(50,2);
w=windmatlab;
w.cancelRequest(0);
h=figure('menubar','none','numberTitle','off','name','股指期货游戏','position',[300 300 800 600],'Resize', 'off');
h1 = axes('Parent',h,'position',[0.1  0.55 0.8 0.35]);
h11=plot(h1,Rtprice(:,1),'LineWidth',3);
title('行情曲线','FontSize',20,  'FontWeight','bold');
set(gca,'XTickLabel','');
hold on
h12=plot(h1,Rtprice(:,2),'--o','MarkerSize',15);
set(gca,'XTickLabel','');
h2  = uicontrol('position', [115  255 150 50],'string','买入指数','FontSize',20,'callback','global M;M.m1= 1;M.m3=1;','BackgroundColor', [0.1 0.8 0.8]);
h3  = uicontrol('position', [315  255 150 50],'string','卖出指数','FontSize',20,'callback','global M;M.m2=-1;M.m3=2;','BackgroundColor', [0.1 0.8 0.8]) ;
h33 = uicontrol('position', [515  255 150 50],'string','退出游戏','FontSize',20,'callback','w=windmatlab;w.cancelRequest(0);clc;close(gcf)','BackgroundColor', [0.9 0.8 0.8]);
h4 = axes('Parent',h,'position',[0.1  0.05 0.8 0.3]);
h22=plot(h4,Profit,'LineWidth',1.5);
set(gca,'XTickLabel','');
title('盈亏曲线','FontSize',20,  'FontWeight','bold');
w.wsq('IF1312.CFE','rt_latest',@STplay_sub1);
function STplay_sub1(reqid,isfinished,errorid,datas,codes,fields,times)
%本函数是用于演示用户自定义回调函数具体撰写方式
datas
global h11 h12 h22 Rtprice  Profit Profit1 M Buy
if M.m1==1||M.m2==-1
Rtprice=[Rtprice;[datas,datas]]  ; 
if M.m1*M.m2==0&&M.m4==0
Buy=datas;
M.m4=1;
end
else
Rtprice=[Rtprice;[datas,nan]];
end
if  M.m1==1&&M.m2==-1
    if M.m3==1
       Profit=[Profit,(-1)*datas+Buy];
       M.m1=0;M.m2=0;M.m3=0;M.m4=0;Buy=nan;
    elseif M.m3==2
       Profit=[Profit,datas+(-1)*Buy]; 
       M.m1=0;M.m2=0;M.m3=0;M.m4=0;Buy=nan;
    else  
    end    
end
Profit1=[Profit1,nansum(Profit)];
set(h11, 'XData',[1:50]', 'YData',Rtprice(end-49:end,1)) ;
set(h12, 'XData',[1:50]', 'YData',Rtprice(end-49:end,2)) ;     
set(h22, 'XData',[1:length(Profit1)]','YData',Profit1)   ;     


    


















