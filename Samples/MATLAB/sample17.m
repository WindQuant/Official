%{
功能：批量下单GUI，多账户下单，账户支持多个证券公司与期货公司交易。可以多次查询。注意下单前修改价格与登录号是否合理。
操作流程：第1步：用户登录自己的账户（股票账户与期货账户），生成登录号（logonID），可参照例子中的登录函数。
         第2步：运行本程序，填写GUI的中的登录号（LogonID）、股票代码、买卖方向、委托价格和委托数量。
注意，登录号与交易品种匹配，如股票必须登录号的市场类型为上海及深圳股票交易所。买卖期货登录号中的市场类型为期交所。中金所品种为期货。
      下单后成功后，进入查询状态，委托内容不能修改。
      程序仅核对登录号是否正常，不会核对委托的内容是否合理,委托内容是否合理由用户自己仔细审核。
      程序返回“已成交数”是经纪商柜台的委托号查询结果。
第1版  张树德编写     （sdzhang@wind.com.cn）   2013年10月31日
%}
function Sample17
clc
%% 全局化变量
clear global  GUI1 GUI2 Data OrderQuery Data4 N w
global GUI1 GUI2 Data OrderQuery Data4 N w
OrderQuery.Num=1;
w=windmatlab;
%
% 定义总界面
        hTabFig = figure('Position', [100 200 760 400 ],...
            'Units', 'pixels',...
            'Toolbar', 'none',...
            'NumberTitle', 'off',...
            'Color', [0.3 0.7 0.3],...
            'Name', '多账户下单GUI',...
            'MenuBar', 'none',...
            'Resize', 'off',...
            'DockControls', 'off');
% 字段        1                  2               3            4           5                 6            7            8                     9            10 
FieldName={'Wind LogonID',     '股票代码',      '买卖方向',  '委托价格',     '委托数量',     '已成交数量' ,'盈亏' ,  '错误信息',          '请求号',    '委托号'   ;};
Data={      '2',         '600276.SH',     'Buy ',      '31',       '100',         ''         ,''    ,  ' '       ,          NaN  ,       NaN      ;...
            '2',         '600267.SH',     'Buy',      '16',       '200',         ''         ,''    ,  ' '       ,          NaN  ,       NaN      ;...
           '2',         '600267.SH',     'Buy',       '16',       '200',         ''         ,''    ,  ' '       ,          NaN  ,       NaN      ;...
};
Data(4:20,1:end)={''};
%% 设定下单列表内容
GUI1=uitable('Position', [5 150 750 250 ],...
            'Parent', hTabFig, ...
            'ColumnEditable', [true true true true true false],...
            'BackgroundColor', [0.9 0.7 0.7],...
            'ColumnName',FieldName(:,1:6),...
            'ColumnFormat', {'numeric', 'char','char', 'numeric', 'numeric','numeric'},...
            'Data',Data(:,1:6),...
            'RearrangeableColumns','on',...
            'ColumnWidth',{90 ,200 ,100 ,110, 100,100 },...
            'FontSize', 14);
%% 设定“确定下单”按钮内容
GUI2=uicontrol('Parent', hTabFig, ...
            'Position', [100 20 200 100], ...
            'String', '确定下单', ...
            'callback', @Sample17_sub1,...
            'Style', 'pushbutton',...
            'HorizontalAlignment', 'center',...
            'BackgroundColor', [0.5 0.8 0.8],...
            'FontWeight', 'bold',...
            'FontSize', 15);       
%% 设定“退出”按钮内容
GUI3=uicontrol('Parent', hTabFig, ...
            'Position', [370 20 200 100], ...
            'String', '退出', ...
            'callback', 'clear global  GUI1 GUI2 Data OrderQuery Data4 N;close(gcf)',...
            'Style', 'pushbutton',...
            'HorizontalAlignment', 'center',...
            'BackgroundColor', [0.5 0.8 0.8],...
            'FontWeight', 'bold',...
            'FontSize', 15);     

function Sample17_sub1(varargin)
%{
功能：多账户下单，账户支持多个证券公司与期货公司交易。
操作流程：第1步：用户登录自己的账户（股票账户与期货账户），生成登录号（logonID），可参照例子中的登录函数。
         第2步：运行本程序，
%}
global GUI1 GUI2 OrderQuery Data Data4 N w
set(GUI2,'BackgroundColor',[0.5 0.5 0.5]);
set(GUI2,'String','操作中');
if OrderQuery.Num==1    
       Data=get(GUI1,'Data');
       for i=1:size(Data,1);            
            if strcmp(Data{i,1},'')==1
            N(i)=logical(0); 
            else
            N(i)=logical(1);    
            end
        end        
        N=logical(N);
        % 检查登录号是否正确        
        [dataLogID]=w.tquery('LogonID','LogonID',Data(N,1));
        for i =1:size(dataLogID,1)
        if double(dataLogID{i})<=0;errordlg('检查登陆号是否正确','登陆错误');set(GUI2,'String', '确定下单','BackgroundColor', [0.5 0.8 0.8]);return;end    
        end      
        Data4=w.torder(Data(N,2),Data(N,3),Data(N,4),Data(N,5), 'OrderType=LMT;HedgeType=SPEC','LogonID',Data(N,1));        
        if Data4{1}==-40530101;errordlg(Data4{end},'下单失败');set(GUI2,'String', '确定下单','BackgroundColor', [0.5 0.8 0.8]);;return;end    
        OrderQuery.Num=OrderQuery.Num+1;        
end
set(GUI1,'ColumnEditable', [false false false false  false]); 
[Data5,s1]=w.tquery('order','logonID',Data(N,1),'requestID',Data4(N,1));% 查询委托号
[Data6,e]=w.tquery('Trade','logonID',Data(N,1),'OrderNumber',Data5(:,1));
t=1;
for i=1:size(Data,1);
if N(i)==1
    Data{i,6}=Data6{t,12};
    t=t+1;
end
end
set(GUI1,'Data',Data);
set(GUI2,'String','查询成交');
set(GUI2,'BackgroundColor',[0.5 0.8 0.8]);














