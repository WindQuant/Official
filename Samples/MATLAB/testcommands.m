%本demo程序用来测试WSD,WSS,WSI,WST,WSQ 五个命令。
%主要使用函数wss，用户可以通过w.menu('wss')创建wss命令

%建立Wind对象；
answer=who('w');
if(isempty(answer) || ~isa(w,'windmatlab'))
    w= windmatlab;
end

codes='600000.SH';
% fields='low';
% 
 data=w.wsd(codes,'low','20120908','20120918')
 data=w.wss(codes,'comp_name,low')
% 
 data=w.wsi(codes,'low',now-1,now)
% 
 begintime=datestr(now,'yyyymmdd 09:30:00');
 endtime  =datestr(now,'yyyymmdd 9:40:00');
 data=w.wst(codes,'low',begintime,endtime)


data=w.wsq(codes,'rt_last,rt_last_vol,rt_ask1')

%w.close
%clear w;