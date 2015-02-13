\name{w.wss}
\alias{w.wss}
\title{
Retrieve Wind snapshot data
}
\description{
   WSS is used to obtain historical cross-section data on specified varieties, including the net profits of the CSI 300 stocks in Q3 2012.\cr
   To show the guide dialog, please input w.menu("wss").\cr
   data <- w.wss(windcodes,windfields,option)\cr
   \cr
   Description:\cr
       windcodes       the Wind-code, like "600000.SH,000001.SZ".\cr
       windfields      the fields, like "OPEN,CLOSE,HIGH".\cr
 	  \cr
       $Data	         the return result, a data.frame.\cr
       $Time           the time of the data.\cr
       $ErrorCode	     the error ID (0 is OK).\cr
 
}
\usage{
w.wss(codes, fields, options = "")
}
\examples{
library(WindR)
w.start()
w.wss("600000.SH","comp_name,comp_name_eng,ipo_date,float_a_shares,mf_amt,mf_vol")    
}
\keyword{ wss }

