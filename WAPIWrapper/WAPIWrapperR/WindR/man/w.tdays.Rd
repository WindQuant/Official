\name{w.tdays}
\alias{w.tdays}
\title{
Retrieve valid days between two days.
}
\description{
   tdays is used to return the valid days between starttime and endtime.\cr
   To show the guide dialog, please input w.menu("tdays").\cr
   data <- w.tdays(starttime,endtime,option)\cr
   \cr
   Description:\cr
        starttime      the start date, like "20130502".\cr
        endtime        the end date, like "20120502", default is the current date.\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $Code          the time of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr
}
\usage{
w.tdays( beginTime, endTime, options = "")
}
\examples{
library(WindR)
w.start()
w.tdays("20130502","20130525")
}
\keyword{ tdays }
