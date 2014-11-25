#include "ConnectionError.hpp"

ConnectionError::ConnectionError(ConnectionErrorEnum ce)
	: mError(ce)
{
}

QString ConnectionError::errorString()
{
	switch(mError)
	{
		case NET_CONNECT_FAILED: return NET_CONNECT_FAILED_STRING;
		case IDENT_SENT_EMPTY_NAME: return IDENT_SENT_EMPTY_NAME_STRING;
		case IDENT_UNSPECIFIED_ERROR: return IDENT_UNSPECIFIED_ERROR_STRING;
		default: return UNSPECIFIED_ERROR_STRING;
	}
}

