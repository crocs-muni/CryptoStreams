/*
 * ***************************************************************************************************
 * WaMMErrorMessage.c ($RCSfile: $)
 *
 * Allocate the reference to the external variable _WaMM_ErrorMesssage
 *
 * Copyright 2008 John Washburn.  All rights reserved until I win the SHA3 contest.
 *
 *
 * ***************************************************************************************************

$Header: $

$Revision: $

$Log: $

 *
 */

#include <stdlib.h>
#include <memory.h>
#include "WaMMErrorMessage.h"

WaMMHashReturn _WaMM_ErrorCode = WaMM_NOT_IMPLEMENTED;
char *_WaMM_ErrorMesssage = NULL;

WaMMHashReturn InitErrorMessageBuffer(void)
{
	WaMMHashReturn retVal = WaMM_ERORR_MALLOC_ERROR_MSG_BUFFER;

	_WaMM_ErrorMesssage = (char*) malloc(WaMM_ErrorMessageBufferSize);
	if (NULL != _WaMM_ErrorMesssage)
	{
		memset(_WaMM_ErrorMesssage, 0, WaMM_ErrorMessageBufferSize);
		retVal = WaMM_SUCCESS;
	}

	return retVal;
}


