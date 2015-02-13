\name{w.weqs}
\alias{w.weqs}
\title{
Retrieve stocks by custom filter.
}
\description{
   wset is used to retrieve stocks by custom filter.\cr
   To show the guide dialog, please input w.menu("weqs").\cr
   data <- w.wset(filtername,...)\cr
   \cr
   Description:\cr
        filtername     the filter's name.\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $Time          the time of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.weqs(filtername,...)
}
\examples{
library(WindR)
w.start()

#Please firstly create a filter named myKDJ.
w.wset('myKDJ')   
}
\keyword{ weqs }
