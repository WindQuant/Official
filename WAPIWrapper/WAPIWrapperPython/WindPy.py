# -*- coding: cp936 -*-
"""
版本：1.0
作者：朱洪海 hhzhu@wind.com.cn 时间：20130712
更新时间：20130820 增加交易接口
文档介绍：Wind Python接口程序。需与WindPy.dll一起使用
修改历史：
版权：随Wind终端一起销售
"""

from ctypes import *
import threading
import sys
#import timeit
from datetime import datetime,date,time,timedelta
class c_variant(Structure):
    """
    定义与VC中Variant类型对应的类
    它包含vt(类型为c_uint16) 和 c_var_union 类型。
    该类型应该从DLL中返回,即POINT(c_variant)，并且应使用free_data释放。
    """
    _anonymous_ = ("val",)
    pass

class c_safearray_union(Union):
    _fields_=[("pbVal", POINTER(c_byte)),
              ("piVal", POINTER(c_int16)),
              ("plVal", POINTER(c_int32)),
              ("pllVal", POINTER(c_int64)),
              ("pfltVal", POINTER(c_float)),
              ("pdblVal", POINTER(c_double)),
              ("pdate", POINTER(c_double)),              
              ("pbstrVal", POINTER(c_wchar_p)),              
              ("pvarVal", POINTER(c_variant))]

class c_safearraybound( Structure):
    _fields_=[("cElements", c_uint32),
              ("lLbound", c_int32)]

class c_safearray(Structure):
    """
    定义与VC中SafeArray类型对应的类
    """    
    _anonymous_ = ("pvData",)
    _fields_=[("cDims", c_uint16),
              ("fFeatures", c_uint16),
              ("cbElements", c_uint32),
              ("cLocks", c_uint32),
              ("pvData", c_safearray_union),
              ("rgsabound", c_safearraybound*3)]    
class c_tagBRECORD(Structure):
   _fields_=[ ("pvRecord", c_void_p),
              ("pRecInfo", c_void_p)]
 
class c_var_union(Union):
    _fields_=[("llVal", c_int64),
              ("lVal",  c_int32),
              ("bVal",  c_int8),
              ("iVal",  c_int16),
              ("fltVal",c_float),
              ("dblVal",c_double),
              ("date", c_double),
              ("bstrVal", c_wchar_p),
              ("pbVal", POINTER(c_byte)),
              ("piVal", POINTER(c_int16)),
              ("plVal", POINTER(c_int32)),
              ("pllVal", POINTER(c_int64)),
              ("pfltVal", POINTER(c_float)),
              ("pdblVal", POINTER(c_double)),
              ("pdate", POINTER(c_double)),              
              ("pbstrVal", POINTER(c_wchar_p)),
              ("parray", POINTER(c_safearray)),              
              ("pvarVal", POINTER(c_variant)),
              ("__VARIANT_NAME_4",c_tagBRECORD)]          

c_variant._fields_ = [ ("vt",c_uint16), ("wr1",c_uint16),("wr2",c_uint16),("wr3",c_uint16),("val",c_var_union)]

"""
定义与VT常量
"""  
VT_EMPTY= 0
VT_NULL	= 1
VT_I2	= 2
VT_I4	= 3
VT_R4	= 4
VT_R8	= 5
VT_CY	= 6
VT_DATE	= 7
VT_BSTR	= 8
VT_DISPATCH	= 9
VT_ERROR	= 10
VT_BOOL	= 11
VT_VARIANT	= 12
VT_UNKNOWN	= 13
VT_DECIMAL	= 14
VT_I1	= 16
VT_UI1	= 17
VT_UI2	= 18
VT_UI4	= 19
VT_I8	= 20
VT_UI8	= 21
VT_INT	= 22
VT_UINT	= 23

VT_SAFEARRAY	= 27
VT_CF	= 71
VT_VECTOR	= 0x1000
VT_ARRAY	= 0x2000
VT_BYREF	= 0x4000
VT_RESERVED	= 0x8000
VT_ILLEGAL	= 0xffff
VT_ILLEGALMASKED= 0xfff
VT_TYPEMASK	= 0xfff

c_StateChangedCallbackType = CFUNCTYPE(c_int32, c_int32, c_int64,c_int32)

gNewDataCome=[3];

gLocker=[threading.Lock()]

gFunctionDict={}


class w:
    """
    作者：朱洪海，时间：20130707
    对Wind Python接口进行包装的类，从而提供与R相似调用接口的操作方式。
    使用方式是：
    首先调用w.start()
    然后分别使用w.wsd,w.wss,w.wst,w.wsi,w.wsq,w.wset,w.wpf,w.tdays,w.tdaysoffset,w.tdayscount命令获取数据。
    """

    class c_apiout(Structure):
        """
        用来描述API各种数据函数的返回值。高水平用户可以直接使用dll函数，从而提高速度。
        """
        #_anonymous_ = ("val",)
        _fields_=[("ErrorCode", c_int32),
                  ("StateCode", c_int32),
                  ("RequestID", c_int64),
                  ("Codes" , c_variant),
                  ("Fields", c_variant),
                  ("Times" , c_variant),
                  ("Data"  , c_variant)]    

        def __str__(self):
            a=".ErrorCode=%d\n"%self.ErrorCode + \
              ".RequestID=%d\n"%self.RequestID ##+ \
              #".Codes="+str(self.Codes) +\
              #"\n.Fields="+str(self.Fields) +\
              #"\n.Times="+str(self.Times) +\
              #"\n.Data="+str(self.Data)
            return a

        def __format__(self,fmt):
            return str(self);
        def __repr__(self):
            return str(self);
    
    """
    引出WindPy.dll 函数
    包含，c_start,c_stop,c_wsd,c_wss等等
    """
    
    sitepath=".";
    for x in sys.path:
        ix=x.find('site-packages')
        if( ix>=0 and x[ix:]=='site-packages'):
          sitepath=x;
          break;
    sitepath=sitepath+"\\WindPy.pth"
    pathfile=open(sitepath)
    dllpath=pathfile.readlines();
    pathfile.close();

    sitepath=dllpath[0]+"\\WindPy.dll"    
    c_windlib=cdll.LoadLibrary(sitepath)

    #start
    c_start=c_windlib.start;
    c_start.restype=c_int32;
    c_start.argtypes=[c_wchar_p,c_wchar_p,c_int32]

    #stop
    c_stop=c_windlib.stop;
    c_stop.restype=c_int32;
    c_stop.argtypes=[]

    #stop
    c_isConnectionOK=c_windlib.isConnectionOK;
    c_isConnectionOK.restype=c_int32;
    c_isConnectionOK.argtypes=[]
    #c_isConnectionOK=staticmethod(c_isConnectionOK)

    #menu
    c_menu=c_windlib.menu;
    #c_menu.restype=c_int32;
    c_menu.argtypes=[c_wchar_p]

    c_wsd=c_windlib.wsd
    c_wsd.restype=POINTER(c_apiout);
    c_wsd.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p] #code,field,begintime,endtime,option

    c_wsq=c_windlib.wsq
    c_wsq.restype=POINTER(c_apiout);
    c_wsq.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p]#codes,fields,options

    c_wsq_asyn=c_windlib.wsq_asyn
    c_wsq_asyn.restype=POINTER(c_apiout);
    c_wsq_asyn.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p]#codes,fields,options

    c_wsqtd=c_windlib.wsqtd
    c_wsqtd.restype=POINTER(c_apiout);
    c_wsqtd.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p]#codes,fields,options

    c_wsqtd_asyn=c_windlib.wsqtd_asyn
    c_wsqtd_asyn.restype=POINTER(c_apiout);
    c_wsqtd_asyn.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p]#codes,fields,options

    c_wss=c_windlib.wss
    c_wss.restype=POINTER(c_apiout);
    c_wss.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p] #codes,fields,options


    c_wst=c_windlib.wst
    c_wst.restype=POINTER(c_apiout);
    c_wst.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p]#code,field,begintime,endtime,option

    c_wsi=c_windlib.wsi
    c_wsi.restype=POINTER(c_apiout);
    c_wsi.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p]#code,field,begintime,endtime,option

    c_tdays=c_windlib.tdays
    c_tdays.restype=POINTER(c_apiout);
    c_tdays.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p] #begintime,endtime,options

    c_tdayscount=c_windlib.tdayscount
    c_tdayscount.restype=POINTER(c_apiout);
    c_tdayscount.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p] #begintime,endtime,options

    c_tdaysoffset=c_windlib.tdaysoffset
    c_tdaysoffset.restype=POINTER(c_apiout);
    c_tdaysoffset.argtypes=[c_wchar_p,c_int32,c_wchar_p] #begintime,offset,options

    c_wset=c_windlib.wset
    c_wset.restype=POINTER(c_apiout);
    c_wset.argtypes=[c_wchar_p,c_wchar_p] #tablename,options
    
    c_weqs=c_windlib.weqs
    c_weqs.restype=POINTER(c_apiout);
    c_weqs.argtypes=[c_wchar_p,c_wchar_p] #filtername,options
    
    c_wpf=c_windlib.wpf
    c_wpf.restype=POINTER(c_apiout);
    c_wpf.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p] #productname,tablename,options           

    c_wupf=c_windlib.wupf
    c_wupf.restype=POINTER(c_apiout);
    c_wupf.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p] #PortfolioName,TradeDate,WindCode,Quantity,CostPrice,options           

    c_tlogon=c_windlib.tLogon
    c_tlogon.restype=POINTER(c_apiout);
    c_tlogon.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p] #BrokerID,DepartmentID,LogonAccount,Password,AccountType,loptions           

    c_tlogout=c_windlib.tLogout
    c_tlogout.restype=POINTER(c_apiout);
    c_tlogout.argtypes=[c_wchar_p] #loptions           

    c_torder=c_windlib.tOrder
    c_torder.restype=POINTER(c_apiout);
    c_torder.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p] #SecurityCode,TradeSide,OrderPrice,OrderVolume,loptions           

    c_tspecial=c_windlib.tSpecial
    c_tspecial.restype=POINTER(c_apiout);
    c_tspecial.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p] #SecurityCode,TradeSide,OrderVolume,loptions

    c_tcancel=c_windlib.tCancel
    c_tcancel.restype=POINTER(c_apiout);
    c_tcancel.argtypes=[c_wchar_p,c_wchar_p] #OrderNumber,loptions           

    c_tquery=c_windlib.tQuery
    c_tquery.restype=POINTER(c_apiout);
    c_tquery.argtypes=[c_wchar_p,c_wchar_p] #qrycode,loptions           

    c_getversion=c_windlib.getVersion
    c_getversion.restype=POINTER(c_apiout);
    c_getversion.argtypes=[] #           

    c_tmonitor=c_windlib.tMonitor
    c_tmonitor.restype=POINTER(c_apiout);
    c_tmonitor.argtypes=[c_wchar_p] #loptions
    
    c_free_data=c_windlib.free_data
    c_free_data.restype=None;
    c_free_data.argtypes=[POINTER(c_apiout)]

    c_setStateCallback=c_windlib.setStateCallback
    c_setStateCallback.restype=None;
    c_setStateCallback.argtypes=[c_StateChangedCallbackType] #callback

    c_readanydata=c_windlib.readanydata
    c_readanydata.restype=POINTER(c_apiout);
    c_readanydata.argtypes=None  
    
    c_readdata=c_windlib.readdata
    c_readdata.restype=POINTER(c_apiout);
    c_readdata.argtypes=[c_int64] 

    c_cancelRequest=c_windlib.cancelRequest
    c_cancelRequest.restype=None;
    c_cancelRequest.argtypes=[c_int64]

    c_cleardata=c_windlib.cleardata
    c_cleardata.restype=None;
    c_cleardata.argtypes=[c_int64]

    c_bktstart=c_windlib.bktstart
    c_bktstart.restype=POINTER(c_apiout);
    c_bktstart.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p] #StrategyName,StartDate,EndDate,options

    c_bktquery=c_windlib.bktquery
    c_bktquery.restype=POINTER(c_apiout);
    c_bktquery.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p] #qrycode,qrytime,options

    c_bktorder=c_windlib.bktorder
    c_bktorder.restype=POINTER(c_apiout);
    c_bktorder.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p] #TradeTime,SecurityCode,TradeSide,TradeVol,options
    
    c_bktstatus=c_windlib.bktstatus
    c_bktstatus.restype=POINTER(c_apiout);
    c_bktstatus.argtypes=[c_wchar_p] #options
    
    c_bktend=c_windlib.bktend
    c_bktend.restype=POINTER(c_apiout);
    c_bktend.argtypes=[c_wchar_p] #options

    c_bktsummary=c_windlib.bktsummary
    c_bktsummary.restype=POINTER(c_apiout);
    c_bktsummary.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p] #BktID,View,options

    c_bktdelete=c_windlib.bktdelete
    c_bktdelete.restype=POINTER(c_apiout);
    c_bktdelete.argtypes=[c_wchar_p,c_wchar_p] #BktID,options

    c_bktstrategy=c_windlib.bktstrategy
    c_bktstrategy.restype=POINTER(c_apiout);
    c_bktstrategy.argtypes=[c_wchar_p] #options

    c_bktfocus=c_windlib.bktfocus
    c_bktfocus.restype=POINTER(c_apiout);
    c_bktfocus.argtypes=[c_wchar_p,c_wchar_p] #StrategyID,options
    
    c_bktshare=c_windlib.bktshare
    c_bktshare.restype=POINTER(c_apiout);
    c_bktshare.argtypes=[c_wchar_p,c_wchar_p] #StrategyID,options

    c_edb=c_windlib.edb
    c_edb.restype=POINTER(c_apiout);
    c_edb.argtypes=[c_wchar_p,c_wchar_p,c_wchar_p,c_wchar_p] #code,begintime,endtime,option
    
    
    def asDateTime(v, asDate=False):
        return datetime(1899,12,30,0,0,0,0)+timedelta(v+0.005/3600/24)
    asDateTime=staticmethod(asDateTime)

    cleardata=c_cleardata;
    cancelRequest=c_cancelRequest;

    
    class WindData:
        """
        作者：朱洪海 时间：20130707
        用途：为了方便客户使用，本类用来把api返回来的C语言数据转换成python能认的数据，从而为用户后面转换成numpy提供方便
             本类包含.ErrorCode 即命令错误代码，0表示错误；
                  对于数据接口还有：  .Codes 命令返回的代码； .Fields命令返回的指标；.Times命令返回的时间；.Data命令返回的数据
                  对于交易接口还有：  .Fields命令返回的指标；.Data命令返回的数据
                    
        """
        def __init__(self):
            self.ErrorCode = 0
            self.StateCode = 0
            self.RequestID = 0
            self.Codes = list()  #list( string)
            self.Fields = list() #list( string)
            self.Times = list() #list( time)
            self.Data = list()  #list( list1,list2,list3,list4)
            self.asDate=False
            self.datatype=0; #0-->DataAPI output, 1-->tradeAPI output
            pass

        def __del__(self):
            pass    

        def __str__(self):
            def str1D(v1d):
                #v1d = str(v1d_in);
                if( not(isinstance(v1d,list)) ):
                      return str(v1d);
                  
                outLen = len(v1d);
                if(outLen==0):
                    return '[]';
                outdot = 0;
                outx='';
                outr='[';
                if outLen>10 :
                    outLen = 10;
                    outdot = 1;

                
                for x in v1d[0:outLen]:
                  try:    
                    outr = outr + outx + str(x);
                    outx=',';
                  except UnicodeEncodeError:
                    outr = outr + outx + repr(x);
                    outx=',';

                if outdot>0 :
                    outr = outr + outx + '...';
                outr = outr + ']';
                return outr;

            def str2D(v2d):
                #v2d = str(v2d_in);
                outLen = len(v2d);
                if(outLen==0):
                    return '[]';
                outdot = 0;
                outx='';
                outr='[';
                if outLen>10 :
                    outLen = 10;
                    outdot = 1;
                    
                for x in v2d[0:outLen]:
                   outr = outr + outx + str1D(x);
                   outx=',';

                if outdot>0 :
                    outr = outr + outx + '...';
                outr = outr + ']';
                return outr;

            a=".ErrorCode=%d"%self.ErrorCode
            if(self.datatype==0):
                if(self.StateCode!=0): a=a+ "\n.StateCode=%d"%self.StateCode
                if(self.RequestID!=0): a=a+ "\n.RequestID=%d"%self.RequestID
                if(len(self.Codes)!=0):a=a+"\n.Codes="+str1D(self.Codes)
                if(len(self.Fields)!=0): a=a+"\n.Fields="+str1D(self.Fields)
                if(len(self.Times)!=0):                 
                    if(self.asDate):a=a+ "\n.Times="+str1D([ format(x,'%Y%m%d') for x in self.Times])
                    else:
                        a=a+ "\n.Times="+str1D([ format(x,'%Y%m%d %H:%M:%S') for x in self.Times])
            else:
                a=a+"\n.Fields="+str1D(self.Fields)
                
            a=a+"\n.Data="+str2D(self.Data)
            return a;    


        def __format__(self,fmt):
            return str(self);
        def __repr__(self):
            return str(self);
        
        def clear(self):
            self.ErrorCode = 0
            self.StateCode = 0
            self.RequestID = 0
            self.Codes = list()  #list( string)
            self.Fields = list() #list( string)
            self.Times = list() #list( time)
            self.Data = list()  #list( list1,list2,list3,list4)
            
        def setErrMsg(self,errid,msg):
            self.clear();
            self.ErrorCode = errid;
            self.Data=[msg];
        def __getTotalCount(self,f):
            if((f.vt & VT_ARRAY ==0) or (f.parray == 0) or (f.parray[0].cDims==0)):
                return 0;

            totalCount=1;
            for i in range(f.parray[0].cDims) :
                totalCount = totalCount * f.parray[0].rgsabound[i].cElements;
            return totalCount;

        def __getColsCount(self,f,index=0):
            if((f.vt & VT_ARRAY ==0) or (f.parray == 0) or (index<f.parray[0].cDims)):
                return 0;

            return f.parray[0].rgsabound[index].cElements;
            
        def __getVarientValue(self,data):
            ltype = data.vt ;
            #inttypearr=[VT_I2,VT_I4,VT_I1,VT_UI1,VT_UI2,VT_UI4,VT_INT,VT_UINT];
            if ltype in [VT_I2,VT_UI2]:
                return data.iVal;
            if( ltype in [VT_I4,VT_UI4,VT_INT,VT_UINT]):
                return data.lVal;
            if( ltype in [VT_I8,VT_UI8]):
                return data.llVal;
            if( ltype in [VT_UI1,VT_I1]):
                return data.bVal;

            if( ltype in [VT_R4]):
                return data.fltVal;
            
            if( ltype in [VT_R8]):
                return data.dblVal;
            
            if( ltype in [VT_DATE]):
                return w.asDateTime(data.date);
                    
            if( ltype in [VT_BSTR]):
                 return data.bstrVal;

            #if( ltype in [VT_VARIANT]):
            #   return list(f.parray[0].pvarVal[0:totalCount]);

            return None;

            
        def __tolist(self,data,basei=0,diff=1):
            """:
            用来把dll中的codes,fields,times 转成list类型
            data 为c_variant
            """
            totalCount = self.__getTotalCount(data);
            if(totalCount ==0): # or data.parray[0].cDims<1):
                return list();

            ltype = data.vt & VT_TYPEMASK;
            #inttypearr=[VT_I2,VT_I4,VT_I1,VT_UI1,VT_UI2,VT_UI4,VT_INT,VT_UINT];
            if ltype in [VT_I2,VT_UI2] :
                return data.parray[0].piVal[basei:totalCount:diff];
            if( ltype in [VT_I4,VT_UI4,VT_INT,VT_UINT]):
                return data.parray[0].plVal[basei:totalCount:diff];
            if( ltype in [VT_I8,VT_UI8]):
                return data.parray[0].pllVal[basei:totalCount:diff];        
            if( ltype in [VT_UI1,VT_I1]):
                return data.parray[0].pbVal[basei:totalCount:diff];        

            if( ltype in [VT_R4]):
                return data.parray[0].pfltVal[basei:totalCount:diff];        
            
            if( ltype in [VT_R8]):
                return data.parray[0].pdblVal[basei:totalCount:diff];        
            
            if( ltype in [VT_DATE]):
                return [w.asDateTime(x) for x in data.parray[0].pdate[basei:totalCount:diff]];        
                    
            if( ltype in [VT_BSTR]):
                return data.parray[0].pbstrVal[basei:totalCount:diff];

            if(ltype in [VT_VARIANT]):
                return [self.__getVarientValue(x) for x in data.parray[0].pvarVal[basei:totalCount:diff]];

            return list();
            
          

        #bywhich=0 default,1 code, 2 field, 3 time
        #indata: POINTER(c_apiout)
        def set(self,indata,bywhich=0,asdate=None,datatype=None):
            self.clear();
            if( indata == 0):
                self.ErrorCode = 1;
                return;
            
            if(asdate==True): self.asDate = True
            if(datatype==None): datatype=0;
            if(datatype<=2): self.datatype=datatype;

            self.ErrorCode = indata[0].ErrorCode
            self.Fields = self.__tolist(indata[0].Fields);
            self.StateCode = indata[0].StateCode
            self.RequestID = indata[0].RequestID
            self.Codes = self.__tolist(indata[0].Codes);
            self.Times = self.__tolist(indata[0].Times);
##            if(self.datatype==0):# for data api output
            if (1==1):
                codeL=len(self.Codes)
                fieldL=len(self.Fields)
                timeL=len(self.Times)
                datalen=self.__getTotalCount(indata[0].Data);
                minlen=min(codeL,fieldL,timeL);

                if( datatype == 2 ):
                    self.Data=self.__tolist(indata[0].Data);
                    return;

                if( datalen != codeL*fieldL*timeL or minlen==0 ):
                    return ;

                if(minlen!=1):
                    self.Data=self.__tolist(indata[0].Data);
                    return;

                if(bywhich>3):
                    bywhich=0;

                if(codeL==1 and not( bywhich==2 and fieldL==1)  and not( bywhich==3 and timeL==1) ):
                    #row=time; col=field;
                    self.Data=[self.__tolist(indata[0].Data,i,fieldL) for i in range(fieldL)];
                    return
                if(timeL ==1 and not ( bywhich==2 and fieldL==1) ):
                    self.Data=[self.__tolist(indata[0].Data,i,fieldL) for i in range(fieldL)];
                    return

                if(fieldL==1 ):
                    self.Data=[self.__tolist(indata[0].Data,i,codeL) for i in range(codeL)];
                    return
                
                self.Data=self.__tolist(indata[0].Data);
##            else:#for trade api output
##                fieldL=len(self.Fields)
##                datalen=self.__getTotalCount(indata[0].Data);
##                colsLen=self.__getColsCount(indata[0].Data);
##
##                if( datalen != colsLen or datalen==0):
##                    return ;
##
##                #if(fieldL!=1):
##                #    self.Data=self.__tolist(indata[0].Data);
##                #    return;
##
##                self.Data=[self.__tolist(indata[0].Data,i,fieldL) for i in range(fieldL)];
##                return
               
            return;

            
    def __targ2str(arg):
        if(arg==None): return [""];
        if(arg==""): return [""];
        if(isinstance(arg,str)): return [arg];
        if(isinstance(arg,list)): return [str(x) for x in arg];
        if(isinstance(arg,tuple)): return [str(x) for x in arg];
        if(isinstance(arg,float) or isinstance(arg,int)): return [str(arg)];
        if( str(type(arg)) == "<type 'unicode'>" ): return [arg];
        return None;
    __targ2str=staticmethod(__targ2str)
    
    def __targArr2str(arg):    
        v = w.__targ2str(arg);
        if(v==None):return None;
        return "$$".join(v);
    __targArr2str=staticmethod(__targArr2str)

    def __dargArr2str(arg):    
        v = w.__targ2str(arg);
        if(v==None):return None;
        return ";".join(v);
    __dargArr2str=staticmethod(__dargArr2str)

    def __d2options(options,arga,argb):
        options = w.__dargArr2str(options);
        if(options==None): return None;

        for i in range(len(arga)):
            v= w.__dargArr2str(arga[i]);
            if(v==None):
                continue;
            else:
                if(options==""):
                    options = v;
                else:
                    options = options+";"+v;
         
        keys=argb.keys();
        for key in keys:
            v =w.__dargArr2str(argb[key]);
            if(v==None or v==""):
                continue;
            else:
                if(options==""):
                    options = str(key)+"="+v;
                else:
                    options = options+";"+str(key)+"="+v;
        return options;
    __d2options=staticmethod(__d2options)

    def __t2options(options,arga,argb):
        options = w.__dargArr2str(options);
        if(options==None): return None;

        for i in range(len(arga)):
            v= w.__dargArr2str(arga[i]);
            if(v==None):
                continue;
            else:
                if(options==""):
                    options = v;
                else:
                    options = options+";"+v;
         
        keys=argb.keys();
        for key in keys:
            v =w.__targArr2str(argb[key]);
            if(v==None or v==""):
                continue;
            else:
                if(options==""):
                    options = str(key)+"="+v;
                else:
                    options = options+";"+str(key)+"="+v;
        return options;
    __t2options=staticmethod(__t2options)
    
    def isconnected():
        """判断是否成功启动w.start了"""
        r = w.c_isConnectionOK()
        if r !=0: return True;
        else: return False;
    isconnected=staticmethod(isconnected)
        
    def menu(menu=""):
        #c_menu(menu)
        return
    menu=staticmethod(menu)    
        
    def start(waitTime=120, showmenu=True):
            """启动WindPy，waitTime为命令等待时间。"""
            outdata=w.WindData();
            if(w.isconnected()):
                outdata.setErrMsg(0,"Already conntected!");
                return outdata;

            #w.global.Functions<<-list()
            
            err=w.c_start("","",waitTime*1000);
            lmsg="";
            if(err==0):
                lmsg="OK!"
            elif(err==-40520009):
                lmsg="WBox lost!";
            elif(err==-40520008):
                lmsg="Timeout Error!";
            elif(err==-40520005):
                lmsg="No Python API Authority!";
            elif(err==-40520004):
                lmsg="Login Failed!";
            elif(err==-40520014):
                lmsg="Please Logon iWind firstly!";
            else:
                lmsg="Start Error!";
            
            if(err==0):
                print("Welcome to use Wind Quant API 1.0 for Python (WindPy)!");
                print("You can use w.menu to help yourself to create commands(WSD,WSS,WST,WSI,WSQ,...)!");
                print("");
                print("COPYRIGHT (C) 2013 WIND HONGHUI INFORMATION & TECHKNOLEWDGE CO., LTD. ALL RIGHTS RESERVED.");
                print("IN NO CIRCUMSTANCE SHALL WIND BE RESPONSIBLE FOR ANY DAMAGES OR LOSSES CAUSED BY USING WIND QUANT API 1.0 FOR Python.")
                
                if(showmenu):
                    w.menu();

            outdata.setErrMsg(err,lmsg);
            return outdata;        
    start=staticmethod(start)

    def close():
        """停止WindPy。"""
        w.c_stop()
    close=staticmethod(close)

    def stop():
        """停止WindPy。"""
        w.c_stop()
    stop=staticmethod(stop)



    def wsd(codes, fields, beginTime=None, endTime=None, options=None,*arga,**argb):
            """wsd获取日期序列"""
            if(endTime==None):  endTime = datetime.today().strftime("%Y-%m-%d")
            if(beginTime==None):  beginTime = endTime            
            codes = w.__dargArr2str(codes);
            fields = w.__dargArr2str(fields);
            options = w.__t2options(options,arga,argb);
            if(codes==None or fields==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(isinstance(beginTime,date) or isinstance(beginTime,datetime)):
                beginTime=beginTime.strftime("%Y-%m-%d")

            if(isinstance(endTime,date) or isinstance(endTime,datetime)):
                endTime=endTime.strftime("%Y-%m-%d")

            out =w.WindData();
            apiout=w.c_wsd(codes,fields,beginTime,endTime,options);
            out.set(apiout,1,asdate=True);
            w.c_free_data(apiout);
            
            return out;
    wsd=staticmethod(wsd)
        
    def wst(codes, fields, beginTime=None, endTime=None, options=None,*arga,**argb):
            """wst获取日内跳价"""
            if(endTime==None): endTime = datetime.today().strftime("%Y%m%d %H:%M:%S")
            if(beginTime==None):  beginTime = endTime   
            codes = w.__dargArr2str(codes);
            fields = w.__dargArr2str(fields);
            options = w.__t2options(options,arga,argb);
            if(codes==None or fields==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(isinstance(beginTime,date) or isinstance(beginTime,datetime)):
                beginTime=beginTime.strftime("%Y%m%d %H:%M:%S")

            if(isinstance(endTime,date) or isinstance(endTime,datetime)):
                endTime=endTime.strftime("%Y%m%d %H:%M:%S")

            out =w.WindData();
            apiout=w.c_wst(codes,fields,beginTime,endTime,options);
            out.set(apiout,1);
            w.c_free_data(apiout);
            
            return out;
    wst=staticmethod(wst)
        
    def wsi(codes, fields, beginTime=None, endTime=None, options=None,*arga,**argb):
            """wsi获取分钟序列"""
            if(endTime==None): endTime = datetime.today().strftime("%Y%m%d %H:%M:%S")
            if(beginTime==None):  beginTime = endTime   
            codes = w.__dargArr2str(codes);
            fields = w.__dargArr2str(fields);
            options = w.__t2options(options,arga,argb);
            if(codes==None or fields==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(isinstance(beginTime,date) or isinstance(beginTime,datetime)):
                beginTime=beginTime.strftime("%Y%m%d %H:%M:%S")

            if(isinstance(endTime,date) or isinstance(endTime,datetime)):
                endTime=endTime.strftime("%Y%m%d %H:%M:%S")

            out =w.WindData();
            apiout=w.c_wsi(codes,fields,beginTime,endTime,options);
            out.set(apiout,1);
            w.c_free_data(apiout);
            
            return out;
    wsi=staticmethod(wsi)

    def tdays(beginTime=None, endTime=None, options=None,*arga,**argb):
            """tdays特定交易日函数"""
            if(endTime==None): endTime = datetime.today().strftime("%Y%m%d")
            if(beginTime==None):  beginTime = endTime   
            options = w.__t2options(options,arga,argb);
            if(endTime==None or beginTime==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(isinstance(beginTime,date) or isinstance(beginTime,datetime)):
                beginTime=beginTime.strftime("%Y%m%d")
            if(isinstance(endTime,date) or isinstance(endTime,datetime)):
                endTime=endTime.strftime("%Y%m%d")

            out =w.WindData();
            apiout=w.c_tdays(beginTime,endTime,options);
            out.set(apiout,1,asdate=True);
            w.c_free_data(apiout);
            
            return out;
    tdays=staticmethod(tdays)

    def tdayscount(beginTime=None, endTime=None, options=None,*arga,**argb):
            """tdayscount交易日统计"""
            if(endTime==None): endTime = datetime.today().strftime("%Y%m%d")
            if(beginTime==None):  beginTime = endTime 
            options = w.__t2options(options,arga,argb);
            if(endTime==None or beginTime==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(isinstance(beginTime,date) or isinstance(beginTime,datetime)):
                beginTime=beginTime.strftime("%Y%m%d")
            if(isinstance(endTime,date) or isinstance(endTime,datetime)):
                endTime=endTime.strftime("%Y%m%d")

            out =w.WindData();
            apiout=w.c_tdayscount(beginTime,endTime,options);
            out.set(apiout,1,asdate=True);
            w.c_free_data(apiout);
            
            return out;
    tdayscount=staticmethod(tdayscount)

    def tdaysoffset(offset, beginTime=None, options=None,*arga,**argb):
            """tdayscount日期偏移函数"""
            if(beginTime==None): beginTime = datetime.today().strftime("%Y%m%d")

            options = w.__t2options(options,arga,argb);
            if(beginTime==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(isinstance(beginTime,date) or isinstance(beginTime,datetime)):
                beginTime=beginTime.strftime("%Y%m%d")

            out =w.WindData();
            apiout=w.c_tdaysoffset(beginTime,offset,options);
            out.set(apiout,1,asdate=True);
            w.c_free_data(apiout);
            
            return out;
    tdaysoffset=staticmethod(tdaysoffset)

    def wset(tablename, options=None,*arga,**argb):
            """wset获取数据集"""
            tablename = w.__dargArr2str(tablename);
            options = w.__t2options(options,arga,argb);
            if(tablename==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_wset(tablename,options);
            out.set(apiout,3,asdate=True);
            w.c_free_data(apiout);
            
            return out;
    wset=staticmethod(wset)

    def weqs(filtername, options=None,*arga,**argb):
            """weqs获取条件选股的结果"""
            filtername = w.__dargArr2str(filtername);
            options = w.__t2options(options,arga,argb);
            if(filtername==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_weqs(filtername,options);
            out.set(apiout,3,asdate=True);
            w.c_free_data(apiout);
            
            return out;
    weqs=staticmethod(weqs)    
    
    def wpf(productname,tablename, options=None,*arga,**argb):
            """wpf资管函数"""
            productname = w.__dargArr2str(productname);
            tablename = w.__dargArr2str(tablename);
            options = w.__t2options(options,arga,argb);
            if(productname==None or tablename==None or options==None):
                print('Invalid arguments!');
                return;            
            
            out =w.WindData();
            apiout=w.c_wpf(productname,tablename,options);
            out.set(apiout,3);
            w.c_free_data(apiout);
            
            return out;
    wpf=staticmethod(wpf)   

    def wupf(PortfolioName,TradeDate,WindCode,Quantity,CostPrice, options=None,*arga,**argb):
            """wpf资管函数"""
            PortfolioName = w.__dargArr2str(PortfolioName);
            TradeDate = w.__dargArr2str(TradeDate);
            WindCode = w.__dargArr2str(WindCode);
            Quantity = w.__dargArr2str(Quantity);
            CostPrice = w.__dargArr2str(CostPrice);
            options = w.__t2options(options,arga,argb);
            if(PortfolioName==None or TradeDate==None or WindCode==None or Quantity==None or CostPrice==None or options==None):
                print('Invalid arguments!');
                return;            
            
            out =w.WindData();
            apiout=w.c_wupf(PortfolioName,TradeDate,WindCode,Quantity,CostPrice,options);
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);
            
            return out;
    wupf=staticmethod(wupf)   

    def wsq(codes, fields, options=None, func=None,*arga,**argb):
            """wsq获取实时数据"""
            codes = w.__dargArr2str(codes);
            fields = w.__dargArr2str(fields);
            options = w.__t2options(options,arga,argb);
            if(codes==None or fields==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(not callable(func)):
                out =w.WindData();
                apiout=w.c_wsq(codes,fields,options);
                out.set(apiout,3);
                w.c_free_data(apiout);
            else:
                out =w.WindData();
                apiout=w.c_wsq_asyn(codes,fields,options);
                out.set(apiout,3);
                w.c_free_data(apiout);                
                if(out.ErrorCode ==0):
                    global gFunctionDict
                    gFunctionDict[out.RequestID] = func;
            
            return out;
    wsq=staticmethod(wsq)

    def wsqtd(codes, fields, options=None, func=None,*arga,**argb):
            """wsqtd获取实时数据"""
            codes = w.__dargArr2str(codes);
            fields = w.__dargArr2str(fields);
            options = w.__t2options(options,arga,argb);
            if(codes==None or fields==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(not callable(func)):
                out =w.WindData();
                apiout=w.c_wsqtd(codes,fields,options);
                out.set(apiout,3);
                w.c_free_data(apiout);
            else:
                out =w.WindData();
                apiout=w.c_wsqtd_asyn(codes,fields,options);
                out.set(apiout,3);
                w.c_free_data(apiout);                
                if(out.ErrorCode ==0):
                    global gFunctionDict
                    gFunctionDict[out.RequestID] = func;
            
            return out;
    wsqtd=staticmethod(wsqtd)

    def wss(codes, fields, options=None,*arga,**argb):
            """wss获取快照数据"""
            codes = w.__dargArr2str(codes);
            fields = w.__dargArr2str(fields);
            options = w.__t2options(options,arga,argb);
            if(codes==None or fields==None or options==None):
                print('Invalid arguments!');
                return;

            out =w.WindData();
            apiout=w.c_wss(codes,fields,options);
            out.set(apiout,3);
            w.c_free_data(apiout);
            
            return out;
    wss=staticmethod(wss)
    
    def readdata(reqid):
        """readdata读取订阅id为reqid的数据"""
        out =w.WindData();
        apiout=w.c_readdata(reqid);
        out.set(apiout,3);
        w.c_free_data(apiout);
        return out
    readdata=staticmethod(readdata)

    def readanydata():
        """readdata读取任何订阅的数据"""
        out =w.WindData();
        apiout=w.c_readanydata();
        out.set(apiout,3);
        w.c_free_data(apiout);
        return out
    readanydata=staticmethod(readanydata)    


        

    def tlogon(BrokerID, DepartmentID, LogonAccount, Password, AccountType,options=None,*arga,**argb):
            BrokerID = w.__targArr2str(BrokerID);
            DepartmentID = w.__targArr2str(DepartmentID);
            LogonAccount = w.__targArr2str(LogonAccount);
            Password = w.__targArr2str(Password);
            AccountType = w.__targArr2str(AccountType);
            options = w.__t2options(options,arga,argb);
            if(BrokerID==None or DepartmentID==None or LogonAccount==None or Password==None or AccountType==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_tlogon(BrokerID,DepartmentID,LogonAccount,Password,AccountType,options);
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);
            
            return out;
    tlogon=staticmethod(tlogon)      


    def tlogout(LogonID=None,options=None,*arga,**argb):
            LogonID = w.__targArr2str(LogonID);
            if(LogonID!=None and LogonID!=''):
                argb['LogonID']=LogonID;
                
            options = w.__t2options(options,arga,argb);
            if(LogonID==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_tlogout(options);
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);
            
            return out;
    tlogout=staticmethod(tlogout)
    
    def torder(SecurityCode, TradeSide, OrderPrice, OrderVolume,options=None,*arga,**argb):
            SecurityCode = w.__targArr2str(SecurityCode);
            TradeSide = w.__targArr2str(TradeSide);
            OrderPrice = w.__targArr2str(OrderPrice);
            OrderVolume = w.__targArr2str(OrderVolume);
            options = w.__t2options(options,arga,argb);
            if(SecurityCode==None or TradeSide==None or OrderPrice==None or OrderVolume==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_torder(SecurityCode,TradeSide,OrderPrice,OrderVolume,options);
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);
            
            return out;
    torder=staticmethod(torder)  
    
    def tspecial(SecurityCode, TradeSide, OrderVolume,options=None,*arga,**argb):
            SecurityCode = w.__targArr2str(SecurityCode);
            TradeSide = w.__targArr2str(TradeSide);
            OrderVolume = w.__targArr2str(OrderVolume);
            options = w.__t2options(options,arga,argb);
            if(SecurityCode==None or TradeSide==None or OrderVolume==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_tspecial(SecurityCode,TradeSide,OrderVolume,options);
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);
            
            return out;
    tspecial=staticmethod(tspecial) 
    
    def tcancel(OrderNumber, options=None,*arga,**argb):
            OrderNumber = w.__targArr2str(OrderNumber);
            options = w.__t2options(options,arga,argb);
            if(OrderNumber==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_tcancel(OrderNumber,options);
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);
            
            return out;
    tcancel=staticmethod(tcancel)

    def tquery(qrycode, options=None,*arga,**argb):
            if(qrycode!=None):
               qrycode = str(qrycode);
            options = w.__t2options(options,arga,argb);
            if(qrycode==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_tquery(qrycode,options);
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);
            
            return out;
    tquery=staticmethod(tquery)

    def tmonitor(options=None,*arga,**argb):
            options = w.__t2options(options,arga,argb);
            if(options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_tmonitor(options);
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);
            
            return out;
    tmonitor=staticmethod(tmonitor)
    
    def getversion():
            apiout=w.c_getversion();
            out =w.WindData();
            out.set(apiout,3,datatype=1);
            w.c_free_data(apiout);

            try:
                return out.Data[0][0]+"WindPy.py version 1.0\n";
            except:
                return "Error in getversion.\nWindPy.py version 1.0\n";

    getversion=staticmethod(getversion)

    def bktstart(StrategyName, StartDate, EndDate, options=None,*arga,**argb):
            if(StrategyName!=None):
               StrategyName = str(StrategyName);
               
            if(isinstance(StartDate,date) or isinstance(StartDate,datetime)):
                StartDate=StartDate.strftime('%Y-%m-%d %H:%M:%S')

            if(isinstance(EndDate,date) or isinstance(EndDate,datetime)):
                EndDate=EndDate.strftime('%Y-%m-%d %H:%M:%S')

            StrategyName = w.__dargArr2str(StrategyName);
            StartDate = w.__dargArr2str(StartDate);
            EndDate = w.__dargArr2str(EndDate);            
            
            options = w.__d2options(options,arga,argb);
            if(StrategyName==None or StartDate==None or EndDate==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktstart(StrategyName,StartDate, EndDate,options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktstart=staticmethod(bktstart)

    def bktquery(qrycode, qrytime, options=None,*arga,**argb):
            if(isinstance(qrytime,date) or isinstance(qrytime,datetime)):
                qrytime=qrytime.strftime('%Y-%m-%d %H:%M:%S')

            qrycode = w.__dargArr2str(qrycode);
            qrytime = w.__dargArr2str(qrytime);
            
            options = w.__d2options(options,arga,argb);
            if(qrycode==None or qrytime==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktquery(qrycode, qrytime, options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktquery=staticmethod(bktquery)

    def bktorder(TradeTime, SecurityCode, TradeSide, TradeVol, options=None,*arga,**argb):
            if(isinstance(TradeTime,date) or isinstance(TradeTime,datetime)):
                TradeTime=TradeTime.strftime('%Y-%m-%d %H:%M:%S')

            SecurityCode = w.__dargArr2str(SecurityCode);
            TradeSide = w.__dargArr2str(TradeSide);
            TradeVol = w.__dargArr2str(TradeVol);
            TradeTime = w.__dargArr2str(TradeTime);
            
            options = w.__d2options(options,arga,argb);
            if(SecurityCode==None or TradeSide==None or TradeVol==None or TradeTime==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktorder(TradeTime,SecurityCode, TradeSide,TradeVol, options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktorder=staticmethod(bktorder)    
        

    def bktstatus(options=None,*arga,**argb):
            options = w.__d2options(options,arga,argb);
            if(options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktstatus(options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktstatus=staticmethod(bktstatus)

    def bktend(options=None,*arga,**argb):
            options = w.__d2options(options,arga,argb);
            if(options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktend(options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktend=staticmethod(bktend)

    def bktsummary(BktID,View, options=None,*arga,**argb):
            BktID = w.__dargArr2str(BktID);
            View = w.__dargArr2str(View);

            options = w.__d2options(options,arga,argb);
            if(BktID==None or View==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktsummary(BktID,View, options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktsummary=staticmethod(bktsummary)

    def bktdelete(BktID, options=None,*arga,**argb):
            BktID = w.__dargArr2str(BktID);

            options = w.__d2options(options,arga,argb);
            if(BktID==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktdelete(BktID, options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktdelete=staticmethod(bktdelete)

    def bktstrategy(options=None,*arga,**argb):
            options = w.__d2options(options,arga,argb);
            
            out =w.WindData();
            apiout=w.c_bktstrategy(options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktstrategy=staticmethod(bktstrategy)

    def bktfocus(StrategyID, options=None,*arga,**argb):
            StrategyID = w.__dargArr2str(StrategyID);
            options = w.__d2options(options,arga,argb);
            if(StrategyID==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktfocus(StrategyID, options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktfocus=staticmethod(bktfocus)    

    def bktshare(StrategyID, options=None,*arga,**argb):
            StrategyID = w.__dargArr2str(StrategyID);
            options = w.__d2options(options,arga,argb);
            if(StrategyID==None or options==None):
                print('Invalid arguments!');
                return;
            
            out =w.WindData();
            apiout=w.c_bktshare(StrategyID, options);
            out.set(apiout,3,datatype=2);
            w.c_free_data(apiout);
            
            return out;
    bktshare=staticmethod(bktshare)        
    
    
    def edb(codes, beginTime=None, endTime=None, options=None,*arga,**argb):
            """edb获取"""
            if(endTime==None):  endTime = datetime.today().strftime("%Y-%m-%d")
            if(beginTime==None):  beginTime = endTime            
            codes = w.__dargArr2str(codes);
            options = w.__t2options(options,arga,argb);
            if(codes==None or options==None):
                print('Invalid arguments!');
                return;
            
            if(isinstance(beginTime,date) or isinstance(beginTime,datetime)):
                beginTime=beginTime.strftime("%Y-%m-%d")

            if(isinstance(endTime,date) or isinstance(endTime,datetime)):
                endTime=endTime.strftime("%Y-%m-%d")

            out =w.WindData();
            apiout=w.c_edb(codes,beginTime,endTime,options);
            out.set(apiout,1,asdate=True);
            w.c_free_data(apiout);
            
            return out;
    edb=staticmethod(edb)
    
    
def DemoWSQCallback(indata):
    """
    作者：朱洪海  时间20130713
    DemoWSQCallback 是WSQ订阅时提供的回调函数模板。该函数只有一个为w.WindData类型的参数indata。
    该函数是被C中线程调用的，因此此线程应该仅仅限于简单的数据处理，并且还应该主要线程之间互斥考虑。

    用户自定义回调函数，请一定要使用try...except
    """
    try:
        lstr= '\nIn DemoWSQCallback:\n' + str(indata);
        print(lstr)
    except:
        return
    
def StateChangedCallback(state,reqid,errorid):
    """
    作者：朱洪海  时间20130713
    StateChangedCallback 是设置给dll api接口的回调函数。
    参数state表示订阅请求的状态，reqid为订阅请求的ID，errorid则是错误ID。
    state=1和state=2时表示reqid有效。state=1表示有一条返回信息，state=2表示最后一条返回信息已经来到。
    一般本函数将根据reqid读取数据w.readdata(reqid),然后再把数据分发给具体的回调函数wsq命令提供的函数。
    """
    try:
        global gNewDataCome
        global gFunctionDict
        #gNewDataCome[0] =gNewDataCome[0]+1;

        if (state !=1) and state!=2:
           return 0;

        out = w.readdata(reqid);
        if(out.StateCode==0):out.StateCode=state;

        f=gFunctionDict[reqid];
        if(state==2):
            del(gFunctionDict[reqid])
        if callable(f):
            f(out);
        else:
            #print "\nStateChangedCallback:",gNewDataCome[0],state,reqid,errorid,"\n"
            print(out);
        return 0;
        #gLocker[0].acquire()
        #print "\nStateChangedCallback:",gNewDataCome[0],state,reqid,errorid,"\n"
        #print out
        #gLocker[0].release();
        #return 0
    except:
        return 0;

gStateCallback=c_StateChangedCallbackType(StateChangedCallback)        
w.c_setStateCallback(gStateCallback);


