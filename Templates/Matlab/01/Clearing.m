function Asset = Clearing(DB,Asset)
%当前K线位置
I = DB.CurrentK;
if(I == 1)
    % 当前持仓
    Asset.CurrentPosition = Asset.Volume(I);
    Asset.Position(I) = Asset.CurrentPosition;
    % 当前现金
    Asset.Cash (I) = 0 - Asset.Volume(I)*Asset.Price(I);
else
    % 当前持仓
    Asset.CurrentPosition = Asset.Volume(I) + Asset.Position(I-1);
    Asset.Position(I) = Asset.CurrentPosition;
    % 当前现金
    Asset.Cash (I) = Asset.Cash(I-1) - Asset.Volume(I)*Asset.Price(I);
end

end