#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "echo.h"

Echo::Echo(long bufLength) {
	
	long i;
		
	buffer = (mfloat*)malloc(bufLength * sizeof(mfloat));
	
	for (i = 0; i < bufLength; i++)
		buffer[i] = 0;
		
	bufferLength = bufLength;
	reset();
}

Echo::~Echo() {
	
	free(buffer);
}

void Echo::clearBuffer() {
	
	long i;

	for (i = 0; i < getBufferLength(); i++)
		buffer[i] = 0;
}

long Echo::getBufferLength() {
	
	return bufferLength;
}

void Echo::setLength(long echoLength) {
	
	if (bufferLength < echoLength)
		return;
	
	parameters.echoLength = echoLength;
	position = 0;
	clearBuffer();
}

void Echo::setLevel(mfloat level) {
	
	if (level < -1)
		level = -1;
	else
	if (level > 1)
		level = 1;
	
	parameters.level = level;
}

void Echo::setFeedback(mfloat feedback) {
	
	parameters.feedback = feedback;
}

void Echo::setLevel(int level) {
	
	if (level < -1)
		level = -1;
	else
	if (level > 1)
		level = 1;
	
	parameters.level = level;
}
/*
void Echo::setFeedback(int feedback) {
	
	parameters.feedback = feedback;
}
*/
void Echo::process(mfloat *in, mfloat *out, int n) {
	
	int i;
	
	if ((parameters.echoLength - position) <= n)
	for (i = 0; i < n; i++) {
		
		mfloat now, delayed;
		mfloat s;
		
		now = in[i];
		delayed = buffer[position];

		s = now + delayed;
		if (s < -1)
			s = -1;
		else
		if (s > 1)
			s = 1;

		buffer[position] = s * parameters.feedback;

		out[i] = (delayed * parameters.level) + now;
		
		position++;
		if (position >= parameters.echoLength) {

			position = 0;
		}
	}
	else
	for (i = 0; i < n; i++) {
		
		mfloat now, delayed;
		mfloat s;
		
		now = in[i];
		delayed = buffer[position];

		s = now + delayed;
		if (s < -1)
			s = -1;
		else
		if (s > 1)
			s = 1;

		buffer[position] = s * parameters.feedback;

		out[i] = (delayed * parameters.level) + now;
		
		position++;
	}
}

void *Echo::getParameters(int *size) {

	void *p;
	
	*size = sizeof(parameters);
	p = malloc(sizeof(parameters));
	memcpy(p, &parameters, sizeof(parameters));

	return p;
}

void Echo::setParameters(void *p) {
	
	memcpy(&parameters, p, sizeof(parameters));
}

mfloat Echo::getLevel() {
	
	return parameters.level;
}

mfloat Echo::getFeedback() {
	
	return parameters.feedback;
}

long Echo::getLength() {
	
	return parameters.echoLength;
}

void Echo::reset() {
	
	setLength(getBufferLength() / 2);
	setLevel(0);
	setFeedback(0.5f);
	clearBuffer();
}
