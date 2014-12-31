%{
尽管MATLAB自带K线函数，但是还不能满足用户需求。我们给出了K线的模板，用户可以在此基础上改成自己需要的K线图。
如，1分钟的K线。
第1版  张树德(sdzhang@wind.com.cn)  2013-10-30
%}
clear
w=windmatlab
[w_data,w_codes,w_fields,w_times,w_errorid,w_reqid]=w.wsd('600276.SH','open,low,high,close','2013-01-02','2013-04-02');   
Open           =  w_data(:,1)
High           =  w_data(:,2)
Low            =  w_data(:,3)
Close          =  w_data(:,4)
color          =  [0.9 0.5 0.5]
cls = get(gca, 'colororder');
color = cls(1, :);
back = get(gca, 'color');
% Determine if current plot is held or not
if ishold
    hldflag = 1;
else
    hldflag = 0;
end
m = length(High(:));
% Need to pad all inputs with NaN's to leave spaces between day data
tmp = nan;
nanpad = tmp(1, ones(1, m));
hilo = [High'; Low'; nanpad];
index = 1:m;
indhilo = index(ones(3, 1), :);
x = indhilo(:);
y = hilo(:);
for i = 1:length(Close)
    ind = (i*3-2):(i*3);
    if Close(i)>=Open(i)
        plot( x(ind),y(ind),'k' );
        hold on;
    else
        plot( x(ind),y(ind),'k' );
        hold on;
    end
end
% plot(indhilo(:), hilo(:), 'color', color);
%% =========================================
clpad = [Close(:)';nanpad];
clpad = clpad(:)';
oppad = [Open(:)'; nanpad];
oppad = oppad(:)';
% Create boundaries for filled regions
xbottom = index - 0.25;
xbotpad = [xbottom(:)'; nanpad];
xbotpad = xbotpad(:)';
xtop = index + 0.25;
xtoppad = [xtop(:)'; nanpad];
xtoppad = xtoppad(:)';
ybottom = min(clpad, oppad);
ytop = max(clpad, oppad);

% Plot lines between high and low price for day
hold on

% z-data used to stagger layering. This prevents renderer layering issues.
zdata = xtoppad;
zdata(~isnan(zdata)) = .01;
zdata2 = zdata + .01;
 
% Plot box representing closing and opening price span
% If the opening price is less than the close, box is empty
i = find(oppad(:) <= clpad(:));
boxes(i) = patch([xbotpad(i); xbotpad(i); xtoppad(i); xtoppad(i)],...
    [ytop(i); ybottom(i); ybottom(i); ytop(i)], ...
    [zdata(i); zdata(i); zdata(i); zdata(i)], ...
    back, 'edgecolor', [0 0 0]);
i = find(oppad(:) > clpad(:));
boxes(i) = patch([xbotpad(i); xbotpad(i); xtoppad(i); xtoppad(i)],...
    [ytop(i); ybottom(i); ybottom(i); ytop(i)],...
    [zdata2(i); zdata2(i); zdata2(i); zdata2(i)], ...
    [0 0 0], 'edgecolor', [0 0 0]); 

