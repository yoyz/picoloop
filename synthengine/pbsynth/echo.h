#ifndef __ECHO_H__
#define __ECHO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mytypes.h"

class Echo {
	
public:
	Echo(long bufLength);
	~Echo();
	void setLength(long length);
	void process(mfloat *in, mfloat *out, int n);
	void setLevel(mfloat level);
	void setFeedback(mfloat feedback);
	void setLevel(int level);
	mfloat getLevel();
	mfloat getFeedback();
//	void setFeedback(int feedback);
	long getBufferLength();
	long getLength();
	void *getParameters(int *size);
	void setParameters(void *p);
	void reset();
	void clearBuffer();

protected:
	mfloat *buffer;
	long bufferLength;
	long position;

	struct {

		long echoLength;
		mfloat level;    /* -1..1 */
		mfloat feedback; /* -1..1 */
	} parameters;
};

#endif
