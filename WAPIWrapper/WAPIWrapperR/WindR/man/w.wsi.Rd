\name{w.wsi}
\alias{w.wsi}
\title{
Retrieve Wind inter-day data
}
\description{
   WSI is used to obtain intraday candlestick chart data on specified varieties, including historical and current-day data. The minute cycle can be specified and the parameters of technical indicators can be customized.\cr
   To show the guide dialog, please input w.menu("wsi").\cr
   data <- w.wsi(windcodes,windfields,starttime,endtime,option)\cr
   \cr
   Description:\cr
        windcodes      the Wind-code, like "600000.SH", only one security allowed.\cr
        windfields     the fields, like "OPEN,CLOSE,HIGH".\cr
        starttime      the start time, like "20130502 09:30:00".\cr
        endtime        the end time, like "20120502 10:30:00"\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $Code          the code of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.wsi(codes, fields, beginTime, endTime, options = "")
}
\examples{
library(WindR)
w.start()
w.wsi("600000.SH","high,low","20130502 09:30:00","20130502 10:30:00")
}
\keyword{ wsi }
