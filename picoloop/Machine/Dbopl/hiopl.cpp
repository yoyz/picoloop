#include "hiopl.h"
#include <stdint.h>
#include <assert.h>
Bit32s CPU_Cycles;
Bit32s CPU_CycleLeft;
Bit32s CPU_CycleMax;
Bitu PIC_Ticks;


typedef  unsigned char		uint8;
//#include "../JuceLibraryCode/JuceHeader.h"

// A higher level wrapper around the DOSBox OPL emulator.

Hiopl::Hiopl(int buflen) {
	adlib = new DBOPL::Handler();
	Buf32 = new Bit32s[buflen*2];
	_op1offset[1] = 0x0;
	_op1offset[2] = 0x1;
	_op1offset[3] = 0x2;
	_op1offset[4] = 0x8;
	_op1offset[5] = 0x9;
	_op1offset[6] = 0xa;
	_op1offset[7] = 0x10;
	_op1offset[8] = 0x11;
	_op1offset[9] = 0x12;
	
	_op2offset[1] = 0x3;
	_op2offset[2] = 0x4;
	_op2offset[3] = 0x5;
	_op2offset[4] = 0xb;
	_op2offset[5] = 0xc;
	_op2offset[6] = 0xd;
	_op2offset[7] = 0x13;
	_op2offset[8] = 0x14;
	_op2offset[9] = 0x15;

	for (int i = 0; i < 256; i++) {
		_WriteReg(i, 0);
	}
}

void Hiopl::Generate(int length, short* buffer) {
	adlib->Generate(length, Buf32);
	for (int i = 0; i < length; i++) {
		buffer[i] = (short)(Buf32[i]);
	}
}

void Hiopl::Generate(int length, float* buffer) {
	// This would be better done using Juce's built in audio format conversion.
	adlib->Generate(length, Buf32);
	for (int i = 0; i < length; i++) {
		buffer[i] = (float)(Buf32[i])/32768.0f;
	}
	/*
	AudioData::ConverterInstance<
		AudioData::Pointer <AudioData::Int16,
			AudioData::NativeEndian,
			AudioData::Interleaved,
			AudioData::Const>,
		AudioData::Pointer <AudioData::Float32,
			AudioData::NativeEndian,
			AudioData::NonInterleaved,
			AudioData::NonConst>
	>converter;
	converter.convertSamples(buffer, 0, Buf32, 0, length);
	*/
}

void Hiopl::SetSampleRate(int hz) {
	adlib->Init(hz);
}

void Hiopl::_WriteReg(Bit32u reg, Bit8u value, Bit8u mask) {
	if (mask > 0) {
		value = (regCache[reg] & (~mask)) | (value & mask);
	}
	adlib->WriteReg(reg, value);
	regCache[reg] = value;
}

void Hiopl::_ClearRegBits(Bit32u reg, Bit8u mask) {
	_WriteReg(reg, regCache[reg] & ~mask);
}

void Hiopl::EnableWaveformControl() {
	_WriteReg(0x01, 0x20);
}

void Hiopl::SetWaveform(int ch, int osc, Waveform wave) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0xe0+offset, (Bit8u)wave, 0x7);
}

void Hiopl::SetAttenuation(int ch, int osc, int level) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x40+offset, (Bit8u)level, 0x3f);
}

void Hiopl::SetKsl(int ch, int osc, int level) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x40+offset, (Bit8u)(level<<6), 0xc0);
}

void Hiopl::SetFrequencyMultiple(int ch, int osc, FreqMultiple mult) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x20+offset, (Bit8u)mult, 0xf);
}

void Hiopl::SetEnvelopeAttack(int ch, int osc, int t) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x60+offset, (Bit8u)t<<4, 0xf0);
}

void Hiopl::SetEnvelopeDecay(int ch, int osc, int t) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x60+offset, (Bit8u)t, 0x0f);
}

void Hiopl::SetEnvelopeSustain(int ch, int osc, int level) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x80+offset, (Bit8u)level<<4, 0xf0);
}

void Hiopl::SetEnvelopeRelease(int ch, int osc, int t) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x80+offset, (Bit8u)t, 0x0f);
}

void Hiopl::EnableTremolo(int ch, int osc, bool enable) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x20+offset, enable ? 0x80 : 0x0, 0x80);
}

void Hiopl::EnableVibrato(int ch, int osc, bool enable) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x20+offset, enable ? 0x40 : 0x0, 0x40);
}

void Hiopl::TremoloDepth(bool high) {
	_WriteReg(0xbd, high ? 0x80 : 0x0, 0x80);
}

void Hiopl::VibratoDepth(bool high) {
	_WriteReg(0xbd, high ? 0x40 : 0x0, 0x40);
}

void Hiopl::EnableSustain(int ch, int osc, bool enable) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x20+offset, enable ? 0x20 : 0x0, 0x20);
}

void Hiopl::EnableKsr(int ch, int osc, bool enable) {
	int offset = this->_GetOffset(ch, osc);
	_WriteReg(0x20+offset, enable ? 0x10 : 0x0, 0x10);
}

void Hiopl::EnableAdditiveSynthesis(int ch, bool enable) {
	int offset = this->_GetOffset(ch);
	_WriteReg(0xc0+offset, enable ? 0x1 : 0x0, 0x1);
}

void Hiopl::SetModulatorFeedback(int ch, int level) {
	int offset = this->_GetOffset(ch);
	_WriteReg(0xc0+offset, (Bit8u)level, 0x0e);
}

void Hiopl::KeyOn(int ch, float frqHz) {
	Hiopl::SetFrequency(ch, frqHz, true);
}

void Hiopl::KeyOff(int ch) {
	int offset = this->_GetOffset(ch);
	_ClearRegBits(0xb0+offset, 0x20);
}

void Hiopl::SetFrequency(int ch, float frqHz, bool keyOn) {
	unsigned int fnum, block;
	int offset = this->_GetOffset(ch);
	_milliHertzToFnum((unsigned int)(frqHz * 1000.0), &fnum, &block);
	_WriteReg(0xa0+offset, fnum % 0x100);
	uint8 trig = (regCache[0xb0+offset] & 0x20) | (keyOn ? 0x20 : 0x00);
	_WriteReg(0xb0+offset, trig|((block&0x7)<<2)|(0x3&(fnum/0x100)));
}

// from libgamemusic, opl-util.cpp
void Hiopl::_milliHertzToFnum(unsigned int milliHertz,
	unsigned int *fnum, unsigned int *block, unsigned int conversionFactor)
{
	// Special case to avoid divide by zero
	if (milliHertz == 0) {
		*block = 0; // actually any block will work
		*fnum = 0;
		return;
	}
	// Special case for frequencies too high to produce
	if (milliHertz > 6208431) {
		*block = 7;
		*fnum = 1023;
		return;
	}

	// This is a bit more efficient and doesn't need log2() from math.h
	if (milliHertz > 3104215) *block = 7;
	else if (milliHertz > 1552107) *block = 6;
	else if (milliHertz > 776053) *block = 5;
	else if (milliHertz > 388026) *block = 4;
	else if (milliHertz > 194013) *block = 3;
	else if (milliHertz > 97006) *block = 2;
	else if (milliHertz > 48503) *block = 1;
	else *block = 0;

	// Slightly more efficient version
	*fnum = (unsigned int)(((unsigned long long)milliHertz << (20 - *block)) / (conversionFactor * 1000.0) + 0.5);
	if ((*block == 7) && (*fnum > 1023)) {
		// frequency out of range, clipping to maximum value.
		*fnum = 1023;
	}
	assert(*block <= 7);
	assert(*fnum < 1024);
	return;
}

Hiopl::~Hiopl() {
	delete[] Buf32;
	delete adlib;
};

bool Hiopl::_CheckParams(int ch, int osc=OSCILLATORS) {
	return ch > 0 && ch <= CHANNELS && osc > 0 && osc <= OSCILLATORS;
}

int Hiopl::_GetOffset(int ch, int osc) {
	assert(_CheckParams(ch, osc));
	return (1 == osc) ? _op1offset[ch] : _op2offset[ch];
}

int Hiopl::_GetOffset(int ch) {
	assert(_CheckParams(ch));
	return ch - 1;
}
