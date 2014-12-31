function Sample60_sub1(reqid,isfinished,errorid,datas,codes,fields,times,selfdata)
% 本函数是用于演示用户自定义回调函数具体撰写方式
global cellList cellFields data1 H2 cellData
lowList=lower(cellList);
lowCode=lower(codes);
[ia1,ib1]=ismember(lowCode,lowList);
fields=lower(fields);
cellFIelds=lower(cellFields);
[ia,ib]=ismember(fields,cellFields);
data1(ib,ib1)=datas;
cellData(2:12,2)=num2cell(data1(1 :11,1));
cellData(2:12,3)=num2cell(data1(12:22,1));
cellData(2:12,4)=num2cell(data1(1 :11,2));
cellData(2:12,5)=num2cell(data1(12:22,2));
set(H2,'Data',cellData);



