\name{w.wupf}
\alias{w.wupf}
\title{
Upload portfolio Information.
}
\description{
   wupf is used to upload the user's assemet portfolio.\cr
   To show the guide dialog, please input w.menu("wupf").\cr
   data <- w.wupf(PortfolioName,TradeDate,WindCode,Quantity,CostPrice,option)\cr
   \cr
   Description:\cr
   	    PortfolioName    the product name.\cr
        TradeDate      the trade date.\cr
        WindCode       the codes. \cr
        Quantity			 \cr
        CostPrice			 \cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $ErrorCode	   the error ID (0 is OK).\cr  
}
\usage{
w.wupf(PortfolioName,TradeDate,WindCode,Quantity,CostPrice,option='')
}
\keyword{ wpf }
