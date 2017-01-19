#include <map>

#include "adlib.h"
#include "dbopl.h"

enum Waveform
{
	SIN = 0, HALF_SIN = 1, ABS_SIN = 2, QUART_SIN = 3
};

enum FreqMultiple
{
	xHALF=0, x1=1, x2=2, x3=3, x4=4, x5=5, x6=6, x7=7, x8=8, x9=9, x10=10, x12=12, x15=15
};


class Hiopl {
	public:
		static const int CHANNELS = 9;
		static const int OSCILLATORS = 2;
		Hiopl(int buflen);
		void Generate(int length, short* buffer);
		void Generate(int length, float* buffer);
		void SetSampleRate(int hz);
		void EnableWaveformControl();
		void EnableOpl3Mode();

		void EnableTremolo(int ch, int osc, bool enable);
		void EnableVibrato(int ch, int osc, bool enable);
		void VibratoDepth(bool high);
		void TremoloDepth(bool high);
		void EnableSustain(int ch, int osc, bool enable);
		void EnableKsr(int ch, int osc, bool enable);
		// true = additive; false = frequency modulation
		void EnableAdditiveSynthesis(int ch, bool enable);
		
		void SetWaveform(int ch, int osc, Waveform wave);
		void SetAttenuation(int ch, int osc, int level);
		void SetKsl(int ch, int osc, int level);
		void SetFrequencyMultiple(int ch, int osc, FreqMultiple mult);
		void SetEnvelopeAttack(int ch, int osc, int t);
		void SetEnvelopeDecay(int ch, int osc, int t);
		void SetEnvelopeSustain(int ch, int osc, int level);
		void SetEnvelopeRelease(int ch, int osc, int t);
		
		void SetModulatorFeedback(int ch, int level);
		void KeyOn(int ch, float frqHz);
		void KeyOff(int ch);
		void SetFrequency(int ch, float frqHz, bool keyOn=false);
		void _WriteReg(Bit32u reg, Bit8u value, Bit8u mask=0x0);
		void _ClearRegBits(Bit32u reg, Bit8u mask);
		~Hiopl();
		int _GetOffset(int ch, int osc);
	private:
		Adlib::Handler *adlib;
		Bit8u regCache[256];
		Bit32s *Buf32;
		bool _CheckParams(int ch, int osc);

		int _GetOffset(int ch);
		void _milliHertzToFnum(unsigned int milliHertz, unsigned int *fnum, unsigned int *block, unsigned int conversionFactor=49716);
		std::map<int, int> _op1offset;
		std::map<int, int> _op2offset;
};
