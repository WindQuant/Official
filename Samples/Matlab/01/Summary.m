function Asset = Summary(DB,Asset)
asset = Asset.Position.*DB.Close + Asset.Cash;
plot(asset);
title('总资产曲线（初始为0）')