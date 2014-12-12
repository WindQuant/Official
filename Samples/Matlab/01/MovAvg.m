function ave = MovAvg(Prices,I,length)
if(length>I)
   ave = sum(Prices(1:I))/I; 
else
   ave = sum(Prices(I-length+1:I))/length; 
end
end