/*
 * ***************************************************************************************************
 * ReverseBits.h ($RCSfile: ReverseBits.h,v $)
 *
 * Functions to reverse the bits within an integer type
 *
 * Copyright 2008 John Washburn.  All rights reserved until I win the SHA3 contest.
 *
 *
 * ***************************************************************************************************

$Header: D:\\RCSArchive\\D\\Programming\\WaMM\\ReferenceImplementation\\WaMM\\WaMM\\ReverseBits.h,v 1.0 2008-10-13 18:11:12-05 johnwashburn Exp johnwashburn $

$Revision: 1.0 $

$Log: ReverseBits.h,v $
Revision 1.0  2008-10-13 18:11:12-05  johnwashburn
Initial revision


 *
 */
#ifndef ReverseBits_h_FE52A5B6_F93D_4a35_8591_141720B1FF33
#define ReverseBits_h_FE52A5B6_F93D_4a35_8591_141720B1FF33 1

unsigned long long ReverseBitsULongLong(unsigned long long ullOrgInteger);
unsigned long ReverseBitsULong(unsigned long ulOrgInteger);
unsigned int ReverseBitsUInt(unsigned int uiOrgInteger);
unsigned short ReverseBitsUShort(unsigned short usOrgInteger);
unsigned char ReverseBitsUChar(unsigned char uchOrgInteger);

unsigned long long ReverseBytesULongLong(unsigned long long ullOrgInteger);
unsigned long ReverseBytesULong(unsigned long ulOrgInteger);
unsigned int ReverseBytesUInt(unsigned int uiOrgInteger);
unsigned short ReverseBytesUShort(unsigned short usOrgInteger);
unsigned char ReverseBytesUChar(unsigned char uchOrgInteger);

#endif /* #ifndef ReverseBits_h_FE52A5B6_F93D_4a35_8591_141720B1FF33 */