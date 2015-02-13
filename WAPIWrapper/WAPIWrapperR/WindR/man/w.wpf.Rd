\name{w.wpf}
\alias{w.wpf}
\title{
Retrieve portfolio Information.
}
\description{
   wpf is used to retrieve a product info of the user's assemet portfolio.\cr
   To show the guide dialog, please input w.menu("wpf").\cr
   data <- w.wpf(productname,tablename,option)\cr
   \cr
   Description:\cr
   	    productname    the product name.\cr
        tablename      the table name of the product.\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $Time          the time of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr  
}
\usage{
w.wpf(productname,tablename,option='')
}
\keyword{ wpf }
