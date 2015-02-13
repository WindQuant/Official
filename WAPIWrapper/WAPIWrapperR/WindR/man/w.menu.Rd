\name{w.menu}
\alias{w.menu}
\title{
Toggle the toolbar; open the guide dialog.
}
\description{
   To show the guide dialog.\cr
   w.menu(menu="")   \cr
    \cr
   Description:     \cr
      menu    the command string or NULL.\cr
     
}
\usage{
w.menu(menu = "")
}
\examples{
library(WindR)
w.start()
w.menu()
w.menu("wsd")
w.menu("wsq")
w.menu("wsi")
w.menu("wst")
w.menu("wss")
w.menu("wset")
w.menu("tdays")
}
\keyword{ menu }
