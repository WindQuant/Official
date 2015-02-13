\name{w.wset}
\alias{w.wset}
\title{
Retrieve some base info about market.
}
\description{
   wset is used to retrieve the market's base infomation, such as a sector constituents.\cr
   To show the guide dialog, please input w.menu("wset").\cr
   data <- w.wset(tablename,option)\cr
   \cr
   Description:\cr
        tablename      the table name.\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $Time          the time of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.wset(tablename,option='')
}
\examples{
library(WindR)
w.start()
w.wset('IndexConstituent',"date=20130518;windcode=000300.SH;field=date,sec_name,i_weight")   
}
\keyword{ wset }
