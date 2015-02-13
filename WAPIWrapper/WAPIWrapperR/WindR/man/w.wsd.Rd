\name{w.wsd}
\alias{w.wsd}
\title{
Retrieve Wind daily data
}
\description{
   WSD is used to retrieve the history daily data of a security, such as intraday k-line.\cr
   To show the guide dialog, please input w.menu("wsd").\cr
   data<- w.wsd(windcodes,windfields,starttime,endtime,option)\cr
   \cr
   Description:\cr
        windcodes      the Wind-code, like "600000.SH", only one security allowed.\cr
        windfields     the fields, like "OPEN,CLOSE,HIGH".\cr
        starttime	     the start date, like "20120701".\cr
        endTime        the end date, like "20120919".\cr
 	      \cr
        $Data	         the return result, a data.frame.\cr
        $Code          the code of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr 
 
}
\usage{
w.wsd(codes, fields, beginTime, endTime, options = "")
}
\examples{
library(WindR)
w.start()
w.wsd("600000.SH","high,low,close,open","20120701","20120919")    
}
\keyword{ wsd }
