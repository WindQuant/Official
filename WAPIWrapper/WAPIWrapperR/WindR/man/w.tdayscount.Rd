\name{w.tdayscount}
\alias{w.tdayscount}
\title{
Retrieve duration between two days. 
}
\description{
   tdayscount is used to count the valid days between starttime and endtime.\cr
   To show the guide dialog, please input w.menu("tdayscount").\cr
   data <- w.tdayscount(starttime,endtime,option)\cr
   \cr
   Description:\cr
        starttime      the start date, like "20130502".\cr
        endtime        the end date, like "20120502", default is the current date.\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $Time          the time of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr     
}
\usage{
w.tdayscount(beginTime, endTime, options = "")
}
\examples{
library(WindR)
w.start()
w.tdayscount("20130502","20130525")
}
\keyword{ tdayscount }
