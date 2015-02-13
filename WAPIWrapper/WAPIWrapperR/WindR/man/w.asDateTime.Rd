\name{w.asDateTime}
\alias{w.asDateTime}
\title{
Time function
}
\description{
   w.asDateTime is used to interpret double-datetimes to R-datetime.\cr
   w.asDateTime(x,asdate=FALSE)\cr
   if asdate==TRUE, the return is date type, else the result is datetime type.

}
\usage{
w.asDateTime(x,asdate=FALSE)
}
\examples{
library(WindR)
w.start(showmenu=F);
data<-w.wss("600000.SH,000002.SZ","last_trade_day,pre_close")
w.asDateTime(data$Data[,2])
w.asDateTime(data$Data[,2],asdate=T)
}
\keyword{ asDateTime }

