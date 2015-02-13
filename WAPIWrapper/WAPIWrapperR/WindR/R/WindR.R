#Wind R API
#honghai zhu <hhzhu@wind.com.cn> 2013

w.start<-function(waitTime=300,showmenu=TRUE)
{#waitTime in seconds
	if(w.isconnected())
	{
		return( list(ErrorCode=0,ErrorMsg="Already conntected!"));
	}


	w.global.Functions<<-list()
	
	err<-.Call("start","","",waitTime*1000);
	if(err==0){
		msg<-"OK!"
	}else if(err==-40520009){
		msg<-"WBox lost!";
	}else if(err==-40520008){
		msg<-"Timeout Error!";
	}else if(err==-40520005){
		msg<-"No R API Authority!";
	}else if(err==-40520004){
		msg<-"Login Failed!";
	}else if(err==-40520014){
		msg<-"Please Logon iWind firstly!";
	}else if(err==-40520016){
		msg<-"Please download new version!!!";
	}
	else{ 
            msg<-"Start Error!";
      }
	
	if(err==0)
	{
		print("Welcome to use WIND Quant API 1.0 for R (WindR)!");
		print("You can use w.menu to help yourself to create commands(WSD,WSS,WST,WSI,WSQ,...)!");
		print("");
	
		print("COPYRIGHT (C) 2013 WIND HONGHUI INFORMATION & TECHKNOLEWDGE CO., LTD. ALL RIGHTS RESERVED.");
		print("IN NO CIRCUMSTANCE SHALL WIND BE RESPONSIBLE FOR ANY DAMAGES OR LOSSES CAUSED BY USING WIND QUANT API 1.0 FOR R.")

		
		if(showmenu)
		{
				w.menu();
		}
	}

	return( list(ErrorCode=err,ErrorMsg=msg));
}
w.isconnected<-function()
{
	r<-.Call("isConnectionOK");
	if(r==0){
		return (FALSE);
	}else  return (TRUE);
}
w.close<-function()
{
	.Call("stop")
}
w.stop<-function()
{
	.Call("stop")
}
w.menu<-function(menu="")
{
	.Call("menu",menu)
}

w.asDateTime<-function(intime,asdate=FALSE)
{
	if(asdate)
	{	return(as.Date(intime,tz=Sys.getenv("TZ"),origin='1899-12-30'))}
	else
	{	return (ISOdatetime(1899,12,30,0,0,0,tz=Sys.getenv("TZ"))+(intime*24*3600+0.005))
	}
}
inw.unlistAll<-function(indata)
{
	classname<-lapply(indata$Data,class)
	
	classL=length(classname);
	
	if(classL<2)
		return(indata);
		
	for(i in 1:classL)
	{
		if(length(classname[[i]])==1 && classname[[i]]=="list")
		{
			indata$Data[i] <- unlist(indata$Data[i]);
		}
	}
	return (indata);
}

#bywhich=0 default,1 code, 2 field, 3 time
inw.toDataFrame<-function(indata,bywhich=0,asdate=FALSE)
{
  if(length(names(indata))<3)
  {
      return(indata)
  }
  
  out<-list();
	out$ErrorCode<-indata$ErrorCode;

	dataL <- length(indata$Data);
	codeL <- length(indata$Code);
	fieldL<- length(indata$Field);
	timeL <- length(indata$Time);
	if(codeL<=0 || fieldL<=0 ||timeL <=0||dataL <=0 || (dataL!=codeL*fieldL*timeL))
	{
		out$Data<-data.frame();
		out$Code<-indata$Code;
		out$Field<-indata$Field;
		if(timeL>1){
			out$Time <-w.asDateTime(indata$Time);
		}
		return (inw.unlistAll(out));
	}
	
	if(bywhich>3)		bywhich=0;

	if(codeL==1 && !( bywhich==2 && fieldL==1)  
	            && !( bywhich==3 && timeL==1)  
	   )
	{#row=time; col=field;
		if(fieldL!=1 || (fieldL==1 && timeL==1)){
			out$Data<-data.frame(w.asDateTime(indata$Time,asdate),t(indata$Data[,1,]),stringsAsFactors =FALSE)
		}else{#fieldL==1 and timeL!=1
			llt<-data.frame(t(indata$Data[,1,]),stringsAsFactors =FALSE);
			if(length(llt)>1)	{		llt<-t(llt);}
			
			out$Data<-data.frame(w.asDateTime(indata$Time,asdate),llt,stringsAsFactors =FALSE)
				
		}
		colnames(out$Data) <- c("DATETIME",indata$Field);
		out$Code <- indata$Code;
		return (inw.unlistAll(out));
	}

	if(timeL ==1 && !( bywhich==2 && fieldL==1)  
	   )#codeL!=1 || (bywhich==3)
	{
		if(fieldL!=1 || (fieldL==1 && codeL==1)){
			out$Data<-data.frame(indata$Code,t(indata$Data[,,1]),stringsAsFactors =FALSE)
		}else{#fieldL==1,codeL!=1
			llt<-data.frame(t(indata$Data[,,1]),stringsAsFactors =FALSE);
			if(length(llt)>1)	{		llt<-t(llt);}
			out$Data<-data.frame(indata$Code,llt,stringsAsFactors =FALSE)		
		}
		
		colnames(out$Data) <- c("CODE",indata$Field);
		out$Time <- w.asDateTime(indata$Time);
		return (inw.unlistAll(out));		
	}

	if(fieldL==1)#codeL!=1 and TimeL!=1;
	{
		out$Data<-data.frame(w.asDateTime(indata$Time,asdate),t(indata$Data[1,,]),stringsAsFactors =FALSE)
		colnames(out$Data) <- c("DATETIME",indata$Code);
		out$df$Field <- indata$Field;
		return (inw.unlistAll(out));		
	}

	out$Data<-indata$Data;
	out$Code<-indata$Code;
	out$Field<-indata$Field;
	out$Time<-w.asDateTime(indata$Time);
		
	return (out);
}

inw.toOptions<-function(args,options)
{
	out<-paste(options,collapse=";");
  lenarg=length(args);
  if(lenarg<=0) return(out);
  
  for (i in 1:lenarg)
  {
    if(is.null(names(args[i])) || names(args[i])=="")
    {
    	tempv<-paste(args[[i]],collapse=";");
    }else{
	    tempv<-paste(args[[i]],collapse="$$");
  	  tempv<-paste(toupper(names(args[i])),'=',tempv,sep="")
  	}
    if(out==""){  out=tempv;}
    else
      out=paste(out,";",tempv,sep="")
  }
  #print(out);
  return(out);
}

inw.toOptionsCloud<-function(args,options)
{
	out<-paste(options,collapse=";");
  lenarg=length(args);
  if(lenarg<=0) return(out);
  
  for (i in 1:lenarg)
  {
    if(is.null(names(args[i])) || names(args[i])=="")
    {
    	tempv<-paste(args[[i]],collapse=";");
    }else{
	    tempv<-paste(args[[i]],collapse=",");
  	  tempv<-paste(toupper(names(args[i])),'=',tempv,sep="")
  	}
    if(out==""){  out=tempv;}
    else
      out=paste(out,";",tempv,sep="")
  }
  #print(out);
  return(out);
}

inw.tToOutput<-function(out)
{
  if(length(out$Field)>0)
  {
    if( length(dim(out$Data))==3){
    	if( dim(out$Data)[3]==1)
      {	
      	out$Data<-as.data.frame(t(out$Data[,,1]),stringsAsFactors =FALSE);
      }
      else{
      	return(out);
      }
    }else{
    	out$Data<-as.data.frame(t(out$Data),stringsAsFactors =FALSE);
    }
    colnames(out$Data)<-out$Field;
    out$Field<-NULL;
    out$Code<-NULL;
    out$Time<-NULL;
    out$RequestID<-NULL;
    out$RequestState<-NULL;
    
    return (inw.unlistAll(out));
  }
  return (out);
}

w.wsd<-function(codes, fields, beginTime='ED', endTime=Sys.time(), ...,options="")
{
	codes<-paste(codes,collapse=",");
	fields<-paste(fields,collapse=",");
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
  	
	if(!is.character(beginTime))
	{
		beginTime<-format(beginTime,"%Y%m%d");
	}
	if(!is.character(endTime))
	{
		endTime<-format(endTime,"%Y%m%d");
	}	
	
	out<-.Call("wsd",codes,fields,beginTime,endTime,loptions);
	
	return (inw.toDataFrame(out,1,asdate=TRUE));
}

w.wst<-function(codes, fields, beginTime, endTime=Sys.time(), ...,options="")
{
	codes<-paste(codes,collapse=",");
	fields<-paste(fields,collapse=",");
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
	
	if(!is.character(beginTime))
	{
		beginTime<-format(beginTime,"%Y%m%d %H:%M:%S");
	}
	if(!is.character(endTime))
	{
		endTime<-format(endTime,"%Y%m%d %H:%M:%S");
	}	
	
	out<-.Call("wst",codes,fields,beginTime,endTime,loptions);
	return (inw.toDataFrame(out,1));
}
w.wsi<-function(codes, fields, beginTime, endTime=Sys.time(), ...,options="")
{
	codes<-paste(codes,collapse=",");
	fields<-paste(fields,collapse=",");
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
	
	if(!is.character(beginTime))
	{
		beginTime<-format(beginTime,"%Y%m%d %H:%M:%S");
	}
	if(!is.character(endTime))
	{
		endTime<-format(endTime,"%Y%m%d %H:%M:%S");
	}	
	
	out<-.Call("wsi",codes,fields,beginTime,endTime,loptions);
	return (inw.toDataFrame(out,1));
}

w.tdays<-function(beginTime, endTime=Sys.time(), ...,options="")
{
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
  	
	if(!is.character(beginTime))
	{
		beginTime<-format(beginTime,"%Y%m%d");
	}
	if(!is.character(endTime))
	{
		endTime<-format(endTime,"%Y%m%d");
	}	
	
	out<-.Call("tdays",beginTime,endTime,loptions);
	return (inw.toDataFrame(out,1,asdate=TRUE));
}
w.tdayscount<-function(beginTime, endTime=Sys.time(), ...,options="")
{
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);		
	
	if(!is.character(beginTime))
	{
		beginTime<-format(beginTime,"%Y%m%d");
	}
	if(!is.character(endTime))
	{
		endTime<-format(endTime,"%Y%m%d");
	}	
	
	out<-.Call("tdayscount",beginTime,endTime,loptions);
	return (inw.toDataFrame(out,1,asdate=TRUE));
}
w.tdaysoffset<-function(offset, beginTime=Sys.time(),...,options="")
{
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
	
	if(!is.character(beginTime))
	{
		beginTime<-format(beginTime,"%Y%m%d");
	}
	offset<-as.integer(offset);
	
	out<-.Call("tdaysoffset",beginTime,offset,loptions);
	return (inw.toDataFrame(out,1,asdate=TRUE));
}

w.weqs<-function(filtername, ...,options="")
{
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	

	filtername<-paste(filtername,collapse=",");
	
	out<-.Call("weqs",filtername,loptions);
	return (inw.toDataFrame(out,3));
}

w.wset<-function(tablename, ...,options="")
{
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	

	tablename<-paste(tablename,collapse=",");
	
	out<-.Call("wset",tablename,loptions);
	return (inw.toDataFrame(out,3));
}
w.wpf<-function(productname,tablename, ...,options="")
{
	productname<-paste(productname,collapse=",");
	tablename<-paste(tablename,collapse=",");
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
	
	out<-.Call("wpf",productname,tablename,loptions);
	return (inw.toDataFrame(out,3));
}
w.wupf<-function(PortfolioName,TradeDate,WindCode,Quantity,CostPrice, ...,WindID='',TradeSide='',AssetType='',HedgeType='',options="")
{
	PortfolioName<-paste(PortfolioName,collapse=",");
	TradeDate<-paste(TradeDate,collapse=",");
	WindCode<-paste(WindCode,collapse=",");
	Quantity<-paste(Quantity,collapse=",");
	CostPrice<-paste(CostPrice,collapse=",");
	
  args=list(...);

  WindID<-paste(WindID,collapse=",");
  TradeSide<-paste(TradeSide,collapse=",");
  AssetType<-paste(AssetType,collapse=",");
  HedgeType<-paste(HedgeType,collapse=",");
  if(WindID!='') args$WindID<-WindID;  
  if(TradeSide!='') args$TradeSide<-TradeSide;  
  if(AssetType!='') args$AssetType<-AssetType;  
  if(HedgeType!='') args$HedgeType<-HedgeType;  
  	
  loptions<-inw.toOptionsCloud(args,options);	
	
	out<-.Call("wupf",PortfolioName,TradeDate,WindCode,Quantity,CostPrice,loptions);
	return (inw.tToOutput(out));
}

w.wsq<-function(codes, fields, ...,options="",func=NULL)
{
	if(!is.null(func) && !is.function(func))
	{
		print("func should be a function!");
		return(NULL);
	}
	
	codes<-paste(codes,collapse=",");
	fields<-paste(fields,collapse=",");
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
	
	if(is.null(func))
	{
		out<-.Call("wsq",codes,fields,loptions);
		return (inw.toDataFrame(out,3));
	}else{
		out<-.Call("wsq_asyn",codes,fields,loptions);
		if(out$ErrorCode !=0 || out$RequestID==0)
		{
			return (out);
		}
		idstr<-sprintf("%d_%d",out$RequestID[[1]],out$RequestID[[2]]);
		w.global.Functions[[idstr]]<<-func;
		return (out);
	}
}
w.wsqtd<-function(codes, fields, ...,options="",func=NULL)
{
	if(!is.null(func) && !is.function(func))
	{
		print("func should be a function!");
		return(NULL);
	}
	
	codes<-paste(codes,collapse=",");
	fields<-paste(fields,collapse=",");
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
	
	if(is.null(func))
	{
		out<-.Call("wsqtd",codes,fields,loptions);
		return (inw.toDataFrame(out,3));
	}else{
		out<-.Call("wsqtd_asyn",codes,fields,loptions);
		if(out$ErrorCode !=0 || out$RequestID==0)
		{
			return (out);
		}
		idstr<-sprintf("%d_%d",out$RequestID[[1]],out$RequestID[[2]]);
		w.global.Functions[[idstr]]<<-func;
		return (out);
	}
}
w.demoCallback<-function(out)
{
	print(out);
}
w.cancelRequest<-function(requestid)
{
	if(!is.numeric(requestid) || !is.vector(requestid))
	{
		print("Incorrect arguments!");
		return (NULL);
	}
	if(length(requestid)<2)
	{
		requestid<-c(requestid[[1]],0);
	}
	
	.Call("cancelRequest",requestid);
	
	idstr<-sprintf("%d_%d",requestid[[1]],requestid[[2]]);
	w.global.Functions[[idstr]]<<-NULL;
	
	return();
}
w.clearAllRequest<-function()
{
	.Call("cleardata",c(0,0));
	w.global.Functions<<-list();
}

w.wss<-function(codes, fields, ...,options="")
{
	codes<-paste(codes,collapse=",");
	fields<-paste(fields,collapse=",");
  args=list(...);
  loptions<-inw.toOptions(args,options);	

	out<-.Call("wss",codes,fields,loptions);
	return (inw.toDataFrame(out,3));
}
w.callback<-function(state,requestid,errorCode)
{
	#s<-sprintf("state=%d,requestid=c(%d,%d),errorCode=%d"
	#           ,state,requestid[1],requestid[2],errorCode)	
	#print(s)
		           
	if( (state !=1) && (state!=2))
	{
		return();
	}

	idstr<-sprintf("%d_%d",requestid[[1]],requestid[[2]]);
	w_data<-.Call("readdata",	requestid)
	#print(w_data)
	
	#funlen<-length(w.global.Functions);
	#if(funlen<=0)
	#	return();

	dealfun<-w.global.Functions[[idstr]];
	if(is.null(dealfun))
		return();

	#case 	1://,enSCS_Request_newData = 1	
	#case	2://,enSCS_Request_finished= 2	
		
	if(state==2)
		w.global.Functions[[idstr]] <<- NULL;
		
	if( is.function(dealfun))
	{
		do.call(dealfun,list(w_data));
	}else{
		print(w_data)
	}
}



w.tlogon<-function(BrokerID, DepartmentID, LogonAccount, Password, AccountType, ...,options="")
{
  args=list(...);
  BrokerID<-paste(BrokerID,collapse="$$");
  DepartmentID<-paste(DepartmentID,collapse="$$");
  LogonAccount<-paste(LogonAccount,collapse="$$");
  Password<-paste(Password,collapse="$$");
  AccountType<-paste(AccountType,collapse="$$");
  loptions<-inw.toOptions(args,options);	
  
  out<-.Call("tLogon",BrokerID,DepartmentID,LogonAccount,Password,AccountType,loptions);
  return (inw.tToOutput(out));
}
w.tlogout<-function(LogonID='',...,options="")
{
  args<-list(...);
  LogonID<-paste(LogonID,collapse="$$");
  if(LogonID!='') args$LogonID<-LogonID;
  loptions<-inw.toOptions(args,options);	
  
  
  out<-.Call("tLogout",loptions);
  return (inw.tToOutput(out));
}
w.torder<-function(SecurityCode, TradeSide, OrderPrice, OrderVolume, ... , MarketType='',OrderType='',HedgeType='',LogonID='',options="")
{
  args=list(...);
  
  MarketType<-paste(MarketType,collapse="$$");
  OrderType<-paste(OrderType,collapse="$$");
  HedgeType<-paste(HedgeType,collapse="$$");
  LogonID<-paste(LogonID,collapse="$$");
  if(MarketType!='') args$MarketType<-MarketType;  
  if(OrderType!='') args$OrderType<-OrderType;  
  if(HedgeType!='') args$HedgeType<-HedgeType;  
  if(LogonID!='') args$LogonID<-LogonID;  
  
  SecurityCode<-paste(SecurityCode,collapse="$$");
  TradeSide<-paste(TradeSide,collapse="$$");
  OrderPrice<-paste(OrderPrice,collapse="$$");
  OrderVolume<-paste(OrderVolume,collapse="$$");
  loptions<-inw.toOptions(args,options);	
  
  out<-.Call("tSendOrder",SecurityCode,TradeSide,OrderPrice,OrderVolume,loptions);
  return (inw.tToOutput(out));
}
w.tspecial<-function(SecurityCode, TradeSide, OrderVolume, ... , LogonID='',options="")
{
  args=list(...);
  
  LogonID<-paste(LogonID,collapse="$$");
  if(LogonID!='') args$LogonID<-LogonID;  
  
  SecurityCode<-paste(SecurityCode,collapse="$$");
  TradeSide<-paste(TradeSide,collapse="$$");
  OrderVolume<-paste(OrderVolume,collapse="$$");
  loptions<-inw.toOptions(args,options);	
  
  out<-.Call("tSendSpecial",SecurityCode,TradeSide,OrderVolume,loptions);
  return (inw.tToOutput(out));
}
w.tcancel<-function(OrderNumber, ..., MarketType='',LogonID='',options="")
{
  args=list(...);
  MarketType<-paste(MarketType,collapse="$$");
  LogonID<-paste(LogonID,collapse="$$");
  if(MarketType!='') args$MarketType<-MarketType;  
  if(LogonID!='') args$LogonID<-LogonID;    
  OrderNumber<-paste(OrderNumber,collapse="$$");
  loptions<-inw.toOptions(args,options);	
  
  out<-.Call("tCancelOrder",OrderNumber,loptions);
  return (inw.tToOutput(out));
}
w.tquery<-function(qrycode, ...,LogonID='',RequestID='',OrderNumber='',SecurityCode='',options="")
{
  args=list(...);
  RequestID<-paste(RequestID,collapse="$$");
  OrderNumber<-paste(OrderNumber,collapse="$$");
  SecurityCode<-paste(SecurityCode,collapse="$$");
  LogonID<-paste(LogonID,collapse="$$");  
  if(RequestID!='') args$RequestID<-RequestID;  
  if(OrderNumber!='') args$OrderNumber<-OrderNumber;  
  if(SecurityCode!='') args$SecurityCode<-SecurityCode;  
  if(LogonID!='') args$LogonID<-LogonID;    
  qrycode<-paste(qrycode,collapse="$$");
  loptions<-inw.toOptions(args,options);	
  
  out<-.Call("tQuery",qrycode,loptions);
  return (inw.tToOutput(out));
}

w.tmonitor<-function( ...,options="")
{
  args=list(...);
  loptions<-inw.toOptions(args,options);	
  
  out<-.Call("tMonitor",loptions);
  return (inw.tToOutput(out));
}

w.getversion<-function()
{
  out<-.Call("getVersion");
  return( paste(out,"WindR version 1.2"));
}

###
##
w.bktstart<-function(StrategyName, StartDate, EndDate, ...,options="")
{
  args=list(...);
  StrategyName<-paste(StrategyName,collapse=",");
  #AssetClass<-paste(AssetClass,collapse=",");
  AssetClass<- '';
  
	if(!is.character(StartDate))
	{
		StartDate<-format(StartDate,"%Y%m%d %H:%M:%S");
	}  
	if(!is.character(EndDate))
	{
		EndDate<-format(EndDate,"%Y%m%d %H:%M:%S");
	} 	
  
  StartDate<-paste(StartDate,collapse=",");
  EndDate<-paste(EndDate,collapse=",");
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktstart",StrategyName,StartDate,EndDate,loptions);
  return (inw.tToOutput(out));
}
w.bktquery<-function(qrycode, qrytime, ...,options="")
{
  args=list(...);
  qrycode<-paste(qrycode,collapse=",");
  
	if(!is.character(qrytime))
	{
		qrytime<-format(qrytime,"%Y%m%d %H:%M:%S");
	} 	  
  qrytime<-paste(qrytime,collapse=",");
  
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktquery",qrycode,qrytime,loptions);
  return (inw.tToOutput(out));
}
w.bktorder<-function(TradeTime, SecurityCode, TradeSide, TradeVol, ...,options="")
{
  args=list(...);
  
	if(!is.character(TradeTime))
	{
		TradeTime<-format(TradeTime,"%Y%m%d %H:%M:%S");
	} 	  
  TradeTime<-paste(TradeTime,collapse=",");
  SecurityCode<-paste(SecurityCode,collapse=",");
  TradeSide<-paste(TradeSide,collapse=",");
  TradeVol<-paste(TradeVol,collapse=",");
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktorder",TradeTime,SecurityCode,TradeSide,TradeVol,loptions);
  return (inw.tToOutput(out));
}
w.bktstatus<-function(...,options="")
{
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktstatus",loptions);
  return (inw.tToOutput(out));
}
w.bktend<-function(...,options="")
{
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktend",loptions);
  return (inw.tToOutput(out));
}
w.bktsummary<-function(BktID,View, ...,options="")
{
  args=list(...);
  BktID<-paste(BktID,collapse=",");
  View<-paste(View,collapse=",");
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktsummary",BktID,View,loptions);
  return (inw.tToOutput(out));
}
w.bktdelete<-function(BktID, ...,options="")
{
  args=list(...);
  BktID<-paste(BktID,collapse=",");
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktdelete",BktID,loptions);
  return (inw.tToOutput(out));
}
w.bktstrategy<-function(...,options="")
{
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktstrategy",loptions);
  return (inw.tToOutput(out));
}
w.bktfocus<-function(StrategyID, ...,options="")
{
  args=list(...);
  StrategyID<-paste(StrategyID,collapse=",");
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktfocus",StrategyID,loptions);
  return (inw.tToOutput(out));
}
w.bktshare<-function(StrategyID, ...,options="")
{
  args=list(...);
  StrategyID<-paste(StrategyID,collapse=",");
  loptions<-inw.toOptionsCloud(args,options);	
  
  out<-.Call("bktshare",StrategyID,loptions);
  return (inw.tToOutput(out));
}
w.edb<-function(codes, beginTime='ED', endTime=Sys.time(), ...,options="")
{
	codes<-paste(codes,collapse=",");
  args=list(...);
  loptions<-inw.toOptionsCloud(args,options);	
  	
	if(!is.character(beginTime))
	{
		beginTime<-format(beginTime,"%Y%m%d");
	}
	if(!is.character(endTime))
	{
		endTime<-format(endTime,"%Y%m%d");
	}	
	
	out<-.Call("edb",codes,beginTime,endTime,loptions);
	
	return (inw.toDataFrame(out,1,asdate=TRUE));
}