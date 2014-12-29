function Asset = InitAsset(DB)
NT = length(DB.Close);

% 当前持仓量
Asset.CurrentPosition = 0;
% 成交量
Asset.Volume = zeros(NT,1);
% 成交价
Asset.Price = zeros(NT,1);
% 持仓量
Asset.Position = zeros(NT,1);
% 资产
Asset.Cash = zeros(NT,1);

end

