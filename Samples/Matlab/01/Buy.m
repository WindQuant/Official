function Asset = Buy(DB,Asset,volume,price,flag)
%当前K线位置
I = DB.CurrentK;
%成交量， 买入为正
Asset.Volume(I) = volume;
%成交价
if(strcmp(flag,'CLOSE')==0)
    Asset.Price(I)  = DB.Close(I); %成交价 恒为正
else
    Asset.Price(I) = price;
end
end