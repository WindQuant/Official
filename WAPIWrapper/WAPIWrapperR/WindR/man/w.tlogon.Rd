\name{w.tlogon}
\alias{w.tlogon}
\title{
Logon into trading systems
}
\description{
   tlogon is used to logon into trading systems.\cr
   To show the guide dialog, please input w.menu("tlogon").\cr
   data <- w.tlogon(BrokerID, DepartmentID, LogonAccount, Password, AccountType,...)\cr
   \cr
   Description:\cr
        BrokerID       Broker's IDs.\cr
        DepartmentID   Department's IDs of the brokers.\cr
        LogonAccount   Logon Accounts.\cr
        Password       Passwords for the accounts.\cr
        AccountType    Account Type. (11/SH/SZ/SHSZ/SHSZA; 12/SZB; 13/SHB; 14/CZC; 15/SHF; 16/DCE; 17/CFE\cr
 	 \cr
        $Data	         the return result, a data.frame.\cr
        $ErrorCode	   the error ID (0 is OK).\cr   
}
\usage{
w.tlogon(BrokerID, DepartmentID, LogonAccount, Password, AccountType,...)
}
\examples{
library(WindR)
w.start()

#Please firstly create a filter named myKDJ.
#w.tlogon(BrokerID, DepartmentID, LogonAccount, Password, AccountType)   
}
\keyword{ tlogon }
