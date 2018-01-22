/*
 * Copyright (c) Innovation Labs, Tata Consultancy Services (TCS), Hyderabad. All Rights
 * reserved.
 *
 * This software is the confidential and proprietary information of TCS.
 * ("Confidential Information").  You shall not disclose such
 * Confidential Information and shall use it only in accordance with
 * the terms of the license agreement you entered into with TCS.
 *
 * TCS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF
 * THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  TCS SHALL NOT BE LIABLE FOR
 * ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
 * DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
 */


#include "common.h"

#ifndef _KHICHIDI_CORE_H_
#define _KHICHIDI_CORE_H_

//
// The C++ part
//
#ifdef  __cplusplus
extern "C" {
#endif

#ifdef EIGHT_BIT

	/* Shuffling operation */
	void shuffling(const KhichidiBitSequence *in,KhichidiBitSequence *out);

	/* T- function */
	void t_function(const KhichidiBitSequence *in,KhichidiBitSequence *out);

	/*LFSR*/
	void lfsr(const KhichidiBitSequence *in,KhichidiBitSequence *out);

	/* message Pre-processing */
	void message_pre_process(const KhichidiBitSequence* in,KhichidiBitSequence* out, short from);

	/* square of unsigned integer - 4 bytes*/
	void square(unsigned char *Cval,const unsigned char *Aval);


#else

	/* Shuffling operation */
	KHICHIDI_WORD shuffling(KHICHIDI_WORD x);

	/* T- function */
	KHICHIDI_WORD t_function(KHICHIDI_WORD n);

	/*LFSR*/
	KHICHIDI_WORD lfsr(KHICHIDI_WORD t_result);

	/* message Pre-processing */
	KHICHIDI_WORD message_pre_process(KHICHIDI_WORD z);

#endif
	/* Pad the data if it is less than the required length */
	void dopad(const KhichidiBitSequence *data, int datalen, KhichidiBitSequence *paddeddata, int paddeddatalen);

	/* addition of unsigned integer */
	void uadd(unsigned char *Cval,const unsigned char *Aval, const unsigned char *Bval,unsigned char limit);

//
// The C++ part
//
#ifdef  __cplusplus
}
#endif

#endif
