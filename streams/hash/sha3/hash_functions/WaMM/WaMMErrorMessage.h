/*
 * ***************************************************************************************************
 * WaMMErrorMessage.h ($RCSfile: $)
 *
 * Create an external reference to a 4K buffer for error messages.  The reference is resolved in 
 * WaMMErrorMessage.c.
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

#ifndef WaMMErrorMessage_h_ABB7FEEB_F486_4fb6_9C88_9059AF6FE7FF
#define WaMMErrorMessage_h_ABB7FEEB_F486_4fb6_9C88_9059AF6FE7FF 1

/* TODO: Generate a new GUID for the guardian #ifndef/#define/#endif pre-processing structure */

#include "WaMMConstants.h"
//#include "SHA3API.h"

extern WaMMHashReturn _WaMM_ErrorCode;
extern char *_WaMM_ErrorMesssage;

WaMMHashReturn InitErrorMessageBuffer(void);

#endif /* WaMMErrorMessage_h_ABB7FEEB_F486_4fb6_9C88_9059AF6FE7FF */
