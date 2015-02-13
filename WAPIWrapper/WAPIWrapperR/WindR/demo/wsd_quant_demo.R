# w.wsd demo
# Honghai Zhu 2013
#

require(WindR)
require(quantmod)

#user should start WindR firstly.
w.start(showmenu=FALSE);

code<-"600004.SH"
wsd_data<- w.wsd(code,"open,high,low,close,volume",Sys.time()-24*3600*100,Sys.time()-24*3600)
if(wsd_data$ErrorCode[[1]]!=0)
{
  error("w.wsd error")
}

data <-wsd_data$Data
ts <- xts(data[,-1],data[,1])
chartSeries(ts,TA=c(addVo(),addBBands(),addMACD()),up.col="red",dn.col="#00ffff",name=code)
