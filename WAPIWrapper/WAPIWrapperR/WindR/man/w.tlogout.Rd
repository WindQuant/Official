\name{w.tlogout}
\alias{w.tlogout}
\title{
Logout from trading systems
}
\description{
   tlogout is used to logout from trading systems.\cr
   To show the guide dialog, please input w.menu("tlogout").\cr
   data <- w.tlogout(LogonID='',...)\cr
   \cr
   Description:\cr
        LogonID        the logon IDs.\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.tlogout(LogonID,...)
}
\examples{
library(WindR)
w.start()

#Please firstly logon.
w.tlogout()   
}
\keyword{ tlogout }
