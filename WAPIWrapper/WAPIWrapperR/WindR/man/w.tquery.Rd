\name{w.tquery}
\alias{w.tquery}
\title{
Query capital,position,order,trade,account,department or logonid.
}
\description{
   tquery is used to query capitial,position,order,trade,account,department or logonid.\cr
   To show the guide dialog, please input w.menu("tquery").\cr
   data <- w.tquery(qrycode, ...)\cr
   \cr
   Description:\cr
   			qrycode== 0/capital  query capital.\cr
   			  Options:\cr
   				    LogonID        Logon IDs.\cr
   			qrycode== 1/position query positions.\cr
   			  Options:\cr
   				    LogonID        Logon IDs.\cr   			
   			qrycode== 2/order    query orders.\cr   			
   			   Options:\cr
   			      LogonID        Logon IDs.\cr   	
   			      OrderNumber    Order Numbers
   			      RequestID      Request IDs of orders
   			qrycode== 3/trade    query trades.\cr   			
   			   Options:\cr
   			      LogonID        Logon IDs.\cr   	
   			qrycode== 4/Department    query departments of brokers.\cr   			
   			   Options:\cr
   			      BrokerID       Broker IDs.\cr   	
   			qrycode== 5/Account  query accounts.\cr   			
   			   Options:\cr
   			      LogonID        Logon IDs.\cr   	
   			qrycode== 6/Broker   query Brokers.\cr   			
   			   Options:\cr
   			      BrokerID       Broker IDs.\cr   	   			
   			qrycode== 7/LogonID  query Logon IDs.\cr   			
   			   Options:\cr
   			      LogonID        Logon IDs.\cr   	   			
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.tquery(qrycode, ...)
}
\examples{
library(WindR)
w.start()

#Please firstly create a filter named myKDJ.
w.tquery(7)   
w.tquery(6)   
}
\keyword{ tquery }