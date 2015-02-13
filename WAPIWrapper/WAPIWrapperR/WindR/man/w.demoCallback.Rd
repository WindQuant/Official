\name{w.demoCallback}
\alias{w.demoCallback}
\title{
Demo callback function
}
\description{
   w.demoCallback(data) is a demo of user callback function.\cr

 	 \cr
		data includes:\cr
        $RequestID     the request ID(can be used to cancel the request/subscription)   \cr
        $Field         the fields of the data.\cr
        $Code          the codes of the data.\cr
        $Time          the time of the data.\cr
        $ErrorCode	   the error ID (0 is OK).\cr
        $Data	         the data which is a 3D array[Field, Code,Time].\cr
	\cr
	Please refer to \link{w.wsq}        \cr
}
\usage{

}

\keyword{ demoCallback }

