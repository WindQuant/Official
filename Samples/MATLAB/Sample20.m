function varargout = Sample20(varargin)
%{
功能：配对交易GUI。
版本历史：
张树德，2013年07月05日，配对交易基本模板
张树德，2013年08月05日，修改了模板布置，报表部分添加了持仓及持仓盈亏。 
参考文献：
张树德，《金融工程与资产管理》，北京航空航天大学出版社，2008年
Ernest P. Chan (2009). Quantitative Trading: How to Build Your Own Algorithmic Trading Business. John Wiley & Sons

%% 配对交易概念。
配对交易是统计套利的一种形式，就是通过对股票历史价格走势的统计分析，选出有相似价格走势的股票对，当两者价格走势出现明显分离时，
卖空相对价格走势偏高的股票，买入相对价格走势偏低的股票，构成一对组合，当价格走势回归到正常水平时进行平仓操作，锁定价差收益。
该策略属于目标为绝对收益的市场中性策略。

%% 备选股票对选择标准：
本例我们主要考虑同一行内的股票，对应上市公司业务相同，可比性强，需要注意的是，行业划分的越细，同行业内公司的“相似度”可能会越高，
但相应的行业内个股数目会减少，能筛选出的股票对数目也就越少，因此行业划分需要把握一定的尺度。
本例中备选股票8个，具体如下：
000001.SZ   平安银行
002142.SZ   宁波银行
600000.SH   浦发银行
600015.SH   华夏银行
600036.SH   招商银行
601009.SH   南京银行
601166.SH   兴业银行
601169.SH   北京银行

%% 配对方法有五种：
方法1：相关系数法
    相关系数是日收益率的相关系数，如果两个品种日收益率相关系数大于0.8该股票对入选。
方法2：价格比率法
    首先计算2个品种时间序列的价格之比，根据历史价格比波动情况确定上下区间，
方法3：单位根法
    单位根方法如下，就是股票价差是否存在单位根。
方法4：随机选对
    随机配成股票对。
方法5：走势相似性
    利用对股价进行拟合，根据拟合差判定相似性。
%%  配对风险控制
1.  样本大小
    按要优化的自由参数个数，使用足够多的回测数据，是降低数据迁就偏差的最基本方法。据经验规则，每个自由参数要用252个数据点（市场一年的交易天数）优化。
    对一个三参数的当日交易模型回测，至少要用三年的日价格数据。如果是分钟交易模型，则至少需要七个月（252/390年）的分钟数据（每天交易分钟数6.5*60=390）。
    要注意的是，七个月的分钟数据，对一个日交易模型来说是远远不够的，因为这时实际上只有7*21=147个有效数据（每月交易天数21）。
2.  样本外测评
    把历史数据分为前后两段，用后一段对策略进行样本外测评（Out-of-Sample Testing）。
    模型参数的优化用前一段数据（称为训练集[training set]），模型效果的检验用后一段数据（称为测评集[test set]）。
    两段数据量长短要大致相仿；若数据量不够，用不少于三分之一的数据作测评，而训练集的大小由前面提到的经验规则决定。在最理想的情况下，基于训练集的最优参数及决策对于测评集也是最优的。
    尽管实际上很难做到这一点，但测评集上的绩效起码要说得过去。否则，模型就存在数据迁就偏差，需要进一步简化并减少参数。
    动态参数优化（moving optimization）是更严密的、却更费时的样本外测评方法。通过使参数不断适应变化的历史数据，来消除数据迁就偏差。
    许多交易员都知道，基于尚未发生的真实数据的仿真交易（paper trading）是最靠谱的样本外测评方法。通过仿真交易常常可找出先窥偏差，且发现各种与操作相关的问题（见第五章）。
    对于一个需要用回测来核实其结果的公开策略，从策略出版日到测试日的这段时间是不折不扣的样本外测评期。只要不使用这段时间的数据来优化参数，这段时间的数据同样可以用来做仿真交易。
3.  敏感性分析 
    在模型参数优化、通过测评集的检验之后，改变这些参数或改变模型的定性决策，来观察模型绩效在训练集和测评集上的变化。如果绩效变化很大，在参数取任何其它值时绩效都很糟，模型很可能具有数据迁就偏差。
    各种简化模型的方法都值得一试。决定是否交易真的需要五个不同的条件吗？一个一个的移除这些条件，模型在训练集上的绩效何时会降得太低呢？模型在测评集上的绩效会相应的降低吗？只要没有显著降低测评集上的绩效，就应该尽可能移除更多的条件、约束和参数。
在简化了参数集和条件，并确保样本外测评的绩效在参数和条件的微小变化下不受显著的影响之后，考虑将资金分配到不同的参数值和条件集。这种资金在参数上的平均化将进一步确保模型真实的交易绩效与回测绩效相差不大。
 
%% 其他因素  
用历史数据对策略的绩效进行切实的模拟检验的过程叫回测，这牵涉许多具体细节问题： 
·        数据：  股票拆分和红利调整，日最高价、最低价的噪音，生存选样偏差
·        绩效度量： 年化夏普比和最大挫跌
·        先窥偏差： 在过去的交易决策中使用无法得到的事后信息
·        数据迁就偏差： 拟合历史数据时使用过多参数。用大样本数据、样本外测评、敏感性分析来避免此类偏差
·        交易成本： 交易成本会大大影响策略绩效
·        策略改进： 通过微小调整来优化绩效的常见方法
%% 配对交易优势
1.  配对交易在很大概率上规避掉了市场风险，有较高的安全边际
    配对交易是一个市场中性策略，通过同时卖空一支股票买入一支股票来创造系统风险暴露为零的组合，扑捉的收益是由于价格发现不充分产生的差异。
同时，配对交易的空头部分本身就比单向多头交易在熊市中更有可能获得更高的安全边际，因为下跌的股票多过上涨的，跌幅大于涨幅。
2.  在市场氛围不明朗的情况下，进行相对保守的操作更为明智
    通常在熊市或者震荡市中，投资者通过寻找逆市上涨带来高回报收益难度很大，稍不注意就容易被套牢。在这样市场环境下，投资者想要追求正回报，
    就不能完全依靠相对收益产品，需要转投绝对收益产品。同时在熊市或者震荡市中，个股的非系统性风险会演化为系统性风险，单向多头持有者非常敏感，
    市场上的消极信息很有可能被放大而造成个股下跌，即使投资者已经在个股上获取正向收益，也很有可能因为各种原因导致收益被抹平。
    目前的市场，受各种因素影响，资金多数持观望态度，即使是有炒作的题材，也是稍微获利就了解离场。
    所以说目前的A股市场氛围并不明朗，在这样的环境下，不追求高额收益，选取风险相对可控，收益相对稳健的操作策略，才是保住资本，
    让资产逐步升值的明智之举，只有这样，在行情到来的时候，才能有足够的资本进行投入，而不会因为满仓或者套牢而望市感叹。
3.  在目前机构投资者不能进行A股市场直接卖空的前提下，配对交易的机会和胜算较高
    任何套利机会，都是参与的人越多，机会越少，ETF申赎如此，股指期货套利如此，统计套利也是如此。越是专业的投资者，
    越能够准确精准地把握机会，而参与的人越多，机会就越难被发现。目前，我国A股融资融券业务还处于发展期，机构投资者还不能直接进行A股卖空操作，
    对于普通的投资者来讲，现在进行配对交易，无疑是最好的时机。
%% 配对交易风险
    对交易的风险来源于三个部分：投资者操作误差、市场突发事件和模型本身误差。
由于是统计套利，突发的市场事件有可能对个股态势造成影响，造成模型失效。同时，配对交易本身也不是无风险套利，而是有一定胜率的套利策略。
从第一天开始推配对交易开始我就知道我的组合中肯定会有出现亏损的组合，我们要做的，就是利用自己的专业知识，不断地去完善和调整模型，
使其更贴合市场，减少亏损的发生。但是从长期和从整体来看，配对交易是风险可控收益稳健的策略。
%%   策略统计
总利润：即本次回测产生的每单笔产生的收益累计
总损失：即本次回测产生的每单笔产生的损失累计
净利润：该策略本次回测时间段里交易产生的所有的损益（包括手续费），即总利润和总损失之和
总手续费：即交易差生的手续费。这里我们是按如果是当天开平的话算只算一次手续费
 
总交易次数：记录交易的次数，即一对开仓和平仓算一次交易
盈利次数：显示收益次数
盈利次数比例：即盈利比例
总损失次数：即显示损失的次数
  
最大盈利:即所有盈利中盈利最大的那笔所获的金额
最大损失:
即所有损失中损失最多的那笔交易的具体损失金额
平均盈利:总利润的平均值
平均损失:总损失的平均值
平均盈利/平均损失:即平均盈利和平均损失的比例
平均净利润:净利润的平均值 
 
最大连续盈利次数:记录连续盈利的最大次数
最大连续损失次数:记录连续亏损的最大次数
盈利因子（总盈利/总损失）:即总利润与总损失的比值
最大合约持仓数:显示最大的持仓数
需要资金:即现有持仓需要的资金
资金回报率（净利润/需要资金）
即净利润和需要资金的比值
最大回撤:即交易会产生的最大亏损 
%}


% SAMPLE20 MATLAB code for Sample20.fig
%      SAMPLE20, by itself, creates a new SAMPLE20 or raises the existing
%      singleton*.
%
%      H = SAMPLE20 returns the handle to a new SAMPLE20 or the handle to
%      the existing singleton*.
%
%      SAMPLE20('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SAMPLE20.M with the given input arguments.
%
%      SAMPLE20('Property','Value',...) creates a new SAMPLE20 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Sample20_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Sample20_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Sample20

% Last Modified by GUIDE v2.5 04-Sep-2013 16:42:39

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Sample20_OpeningFcn, ...
                   'gui_OutputFcn',  @Sample20_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before Sample20 is made visible.
function Sample20_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Sample20 (see VARARGIN)

% Choose default command line output for Sample20
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Sample20 wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Sample20_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%%  模型类型选择
v(1)=get(handles.radiobutton1,'value');
v(2)=get(handles.radiobutton2,'value');
v(3)=get(handles.radiobutton3,'value');
v(4)=get(handles.radiobutton4,'value');
v(5)=get(handles.radiobutton5,'value');
v1=find(v==1);
%%  模型类型选择
va(1)=get(handles.radiobutton6,'value');
va(2)=get(handles.radiobutton7,'value');
v2=find(va==1);

%% 读取股票历史价格
set(handles.listbox1,'string',{1});
w=windmatlab;
%% 1.1 银行股名单
strStockList='000001.SZ,002142.SZ,600000.SH,600015.SH,600036.SH,601009.SH,601166.SH,601169.SH';
data=w.wss(strStockList,'sec_name,trade_code');
StockList=regexp(strStockList,'[,]','split');
StockList=StockList(:);
%% 1.2 取得历史价格
price=[];
[~,~,~,numDate]=w.wsd(StockList{1},'close',today-200,today);
for i=1:length(StockList)
data1=w.wsd(StockList{i},'close',today-200,today,'Fill=Previous') ;
% 开盘价
open1=w.wsd(StockList{i},'open',today,today) ;
if iscell(open1)==1
openPrice(i)=NaN;
else
openPrice(i)=open1  ;  
end
%% 数据连接错误提示
if iscell(data1)==1;errordlg(['数据连接错误：',data1{1}],'请与销售联系。');error;end
price=[price,data1];
end
w.close;
%% 1.3 选择相关系数高的股票对
%% 1.3.1 相关系数法
[numDays,numAssets]=size(price);
ret=price2ret(price);
matCorrcoef=corrcoef(ret);
t=1;
switch v1
    case 1
for i=2:8
    for j=1:i-1
        if matCorrcoef(i,j)>0.8;
        pairs(t,1:3)=[t,i,j]  ;
        cellPairs(t,1)={[data{i,1},' VS ',data{j,1}]};
        cellPairs(t,2)=data(i,1);
        cellPairs(t,3)=data(j,1);   
        cellPairs{t,4}=i;    
        cellPairs{t,5}=j;    
        t=t+1;
        end
    end
end
%% 1.3.2 价格比率法
    case 2
t=1;
for i=2:8
  for  j=1:i-1
    ret1=price(:,i)/mean(price(:,i));
    ret2=price(:,j)/mean(price(:,j));
    ret=ret1./ret2;
    if std(ret)<0.1
        pairs(t,1:3)=[t,i,j]  ;
        cellPairs(t,1)={[data{i,1},' VS ',data{j,1}]};
        cellPairs(t,2)=data(i,1);
        cellPairs(t,3)=data(j,1);   
        cellPairs{t,4}=i;    
        cellPairs{t,5}=j;    
        t=t+1;             
    end   
  end
end
%% 1.3.3 单位根法
    case 3
t=1;
for i=2:8
  for  j=1:i-1
    parReg=regress(price(:,i),[ones(numDays,1),price(:,j)]);
    dis=price(:,i)-parReg(1)-parReg(2)*price(:,j);
    h = adftest(dis,'alpha',0.02)   ; 
    if h==1
        pairs(t,1:3)=[t,i,j]  ;
        cellPairs(t,1)={[data{i,1},' VS ',data{j,1}]};
        cellPairs(t,2)=data(i,1);
        cellPairs(t,3)=data(j,1);   
        cellPairs{t,4}=i;    
        cellPairs{t,5}=j;    
        t=t+1;             
    end   
  end
end
%% 1.3.4 随机配对法
    case 4
rand('seed',1);
clear cellPairs;
t=1;
for i=2:8
  for  j=1:i-1
    if rand<0.5
        pairs(t,1:3)=[t,i,j]  ;
        cellPairs(t,1)={[data{i,1},' VS ',data{j,1}]};
        cellPairs(t,2)=data(i,1);
        cellPairs(t,3)=data(j,1);   
        cellPairs{t,4}=i;    
        cellPairs{t,5}=j;    
        t=t+1;             
    end   
  end
end
%% 1.3.5 形态拟合
    case 5
clear cellPairs
t=1;
for i=2:8
  for  j=1:i-1
    parReg=regress(price(:,i),[ones(numDays,1),price(:,j)]);
    dis=price(:,i)-parReg(1)-parReg(2)*price(:,j);
    dis=abs(dis);
    if max(dis)<3
        pairs(t,1:3)=[t,i,j]  ;
        cellPairs(t,1)={[data{i,1},' VS ',data{j,1}]};
        cellPairs(t,2)=data(i,1);
        cellPairs(t,3)=data(j,1);   
        cellPairs{t,4}=i;    
        cellPairs{t,5}=j;    
        t=t+1;             
    end   
  end
end
end 

%% 计算价差方法
switch v2
    case 1
      matPrice=price./price(ones(size(price,1),1),:);
    case 2
      matPrice=price./repmat(nanmean(price),numDays,1);      
end
%% 1.4计算价差
for i=1:length(cellPairs)
priceSpread{i}=[numDate,matPrice(:,pairs(i,2))-matPrice(:,pairs(i,3))];      
end
%% 1.5 将变量保存为结构变量
BankData.cellPairs=cellPairs;      % 满足价差条件的股票对
BankData.StockList=StockList;      % 股票列表
BankData.Price=price;              % 股票价格
BankData.matPrice=matPrice;        % 归一化的价格
BankData.numDate=numDate;          % 日期序列
BankData.priceSpread=priceSpread;  % 价差
BankData.openPrice=openPrice;      % 开盘价
BankData.strStockList=strStockList;% 银行股股票列表
set(handles.listbox1,'string',cellPairs(:,1)) ;


% --- Executes on selection change in listbox1.
function listbox1_Callback(hObject, eventdata, handles)
% hObject    handle to listbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns listbox1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from listbox1


% --- Executes during object creation, after setting all properties.
function listbox1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to listbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in radiobutton6.
function radiobutton6_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton6 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton6
set(handles.radiobutton6,'value',1);
set(handles.radiobutton7,'value',0);

% --- Executes on button press in radiobutton7.
function radiobutton7_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton7 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton7
set(handles.radiobutton6,'value',0)
set(handles.radiobutton7,'value',1)
% --- Executes on button press in radiobutton1.
function radiobutton1_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton1
%% 2、价格比率
set(handles.radiobutton1,'value',1);
set(handles.radiobutton2,'value',0);
set(handles.radiobutton3,'value',0);
set(handles.radiobutton4,'value',0);
set(handles.radiobutton5,'value',0);
% --- Executes on button press in radiobutton2.
function radiobutton2_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton2
%% 3、单位根
set(handles.radiobutton1,'value',0);
set(handles.radiobutton2,'value',1);
set(handles.radiobutton3,'value',0);
set(handles.radiobutton4,'value',0);
set(handles.radiobutton5,'value',0);




% --- Executes on button press in radiobutton3.
function radiobutton3_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton3
%% 1、相关系数
set(handles.radiobutton1,'value',0);
set(handles.radiobutton2,'value',0);
set(handles.radiobutton3,'value',1);
set(handles.radiobutton4,'value',0);
set(handles.radiobutton5,'value',0);
% --- Executes on button press in radiobutton4.
function radiobutton4_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton4
%% 5.形态拟合
set(handles.radiobutton1,'value',0);
set(handles.radiobutton2,'value',0);
set(handles.radiobutton3,'value',0);
set(handles.radiobutton4,'value',1);
set(handles.radiobutton5,'value',0);
% --- Executes on button press in radiobutton5.
function radiobutton5_Callback(hObject, eventdata, handles)
% hObject    handle to radiobutton5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of radiobutton5
%% 4.随机配对
set(handles.radiobutton1,'value',0);
set(handles.radiobutton2,'value',0);
set(handles.radiobutton3,'value',0);
set(handles.radiobutton4,'value',0);
set(handles.radiobutton5,'value',1);


% --- Executes during object creation, after setting all properties.
function radiobutton3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to radiobutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
% set(handles.radiobutton1,'value',1)
% 

