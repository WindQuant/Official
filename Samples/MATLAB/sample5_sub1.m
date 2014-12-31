function Sample5_sub1(reqid,isfinished,errorid,datas,codes,fields,times,selfdata)
%本函数是用于演示用户自定义回调函数具体撰写方式

global cellFields data1 h3
fields=lower(fields);
cellFIelds=lower(cellFields);
[ia,ib]=ismember(fields,cellFields);
data1(ib)=datas;
global str1 h3 
str1{1}=['卖5价：  ' ,num2str(data1(1))];     
str1{2}=['卖4价：  ' ,num2str(data1(2))];       
str1{3}=['卖3价：  ' ,num2str(data1(3))];       
str1{4}=['卖2价：  ' ,num2str(data1(4))];      
str1{5}=['卖1价：  ' ,num2str(data1(5))];      
str1{6}=['成交价：' ,num2str(data1(6))];     
str1{7}=['买1价：  ' ,num2str(data1(7))];  
str1{8}=['买2价：  ' ,num2str(data1(8))];     
str1{9}=['买3价：  ' ,num2str(data1(9))];    
str1{10}=['买4价： ' ,num2str(data1(10))];      
str1{11}=['买5价： ' ,num2str(data1(11))];   
str1=str1(:);
set(h3,'string',str1);
      
