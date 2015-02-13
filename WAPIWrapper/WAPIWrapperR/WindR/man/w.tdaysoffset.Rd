\name{w.tdaysoffset}
\alias{w.tdaysoffset}
\title{
Retrieve a day based on the input day
}
\description{
   tdaysoffset is used to retrieve a date by the offset of a base date.\cr
   To show the guide dialog, please input w.menu("tdaysoffset").\cr
   data <- w.tdaysoffset(offset,beginTime,option)\cr
   \cr
   Description:\cr
        offset         offset, a number such as 1,-1 ...\cr
        starttime      the base date, like "20120502", default is the current date.\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $Time          the time of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr  
}
\usage{
w.tdaysoffset(offset, beginTime, options = "")
}
\examples{
library(WindR)
w.start()
w.tdaysoffset(-5)
}
\keyword{ tdaysoffset }
