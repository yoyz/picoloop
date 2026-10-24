#pragma once

#include <atomic>
#include <cstdint>
#include <string>

// M_PI is not exposed by MinGW's math.h under -std=c++11 (__STRICT_ANSI__),
// same guard as Biquad.h.
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

// Sample rate the voice model runs at. Also used by the PortAudio frontend.
static constexpr double ER1_SAMPLE_RATE = 48000.0;

enum {
    P_WAVE,
    P_PITCH,
    P_DETUNE,
    P_MODTYPE,
    P_MODDEPTH,
    P_MODSPEED,
    P_SWEEPS,
    P_PHASE,
    P_SPAN,
    P_WAVEWARP,
    P_BITDEPTH,
    P_DECAY,
    P_NOISE,
    P_NOISEDECAY,
    P_LEVEL,
    P_PAN,
    P_LOWBOOST,
    P_FILTER,
    P_FILTERPOS,
    P_RESO,
    P_KEYTRACK,
    P_FILTERTYPE,
    NUM_PARAMS
};

// --- Parameter metadata (frontend / tooling helpers) ---

int paramCount();
const char *paramName(int i);
bool paramIsDiscrete(int i);
double paramMin(int i);
double paramMax(int i);
double paramStep(int i);

// Look up a parameter id by name (case/space/underscore-insensitive). -1 if
// unknown.
int paramIndex(const char *name);

// Named states for discrete parameters (Wave, Mod Type). count is 0 when the
// parameter has no named states.
int paramValueCount(int i);
const char *paramValueName(int i, int idx);

// Shared wavetable set. One instance is enough for any number of voices.
class WaveTables {
public:
    static constexpr int LEN = 4096;

    WaveTables();

    float read(const float *tab, float phase) const;

    float sine[LEN + 1];
    float tri[LEN + 1];
    float saw[LEN + 1];
    float square[LEN + 1];
    float pulse25[LEN + 1];
    float pulse125[LEN + 1];
};

// A single ER-1 analog-modeling percussion voice. Self-contained and
// instantiable: construct as many as you like and render each into the mix.
class ER1Voice {
public:
    explicit ER1Voice(const WaveTables &tables,
                      double sampleRate = ER1_SAMPLE_RATE);

    // The voice model is rate-agnostic once this is set. Must be called with
    // the engine rate before rendering (picoloop runs at DEFAULTFREQ).
    void setSampleRate(double sampleRate) { sampleRate_ = sampleRate; }

    // Clears the audio-thread state without touching the parameters.
    void reset();

    // Note-on. Consumed at the next rendered sample.
    void trigger() { trigger_.store(true); }

    // Note-off. Starts a fast amp release (avoids clicks); a later trigger
    // cancels it. Consumed at the next rendered sample.
    void release() { release_.store(true); }

    bool active() const { return active_.load(); }

    // Render `frames` stereo samples into an interleaved float buffer.
    void render(float *out, unsigned long frames);

    double param(int i) const { return param_[i]; }
    void setParam(int i, double v) { param_[i] = v; }
    void adjustParam(int i, int dir);

    std::string valueStr(int i) const;

private:
    static double modDepthScale(int mt) {
        switch (mt) {
            case 4: return 2.5;
            default: return 1.0;
        }
    }

    static float xorshiftBipolar(uint32_t &state) {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return static_cast<float>(state >> 8) * (1.0f / 8388608.0f) - 1.0f;
    }

    float nextRand() { return xorshiftBipolar(rng_); }
    float nextNoiseRand() { return xorshiftBipolar(noiseRng_); }

    void initDefaults();

    const WaveTables &tables_;
    double sampleRate_ = ER1_SAMPLE_RATE;

    // Plain doubles: in picoloop every writer (setI/setF) and the renderer
    // run from the audio callback, and this avoids libatomic on 32-bit
    // MIPS/ARM builds. The trigger/release/active handshake stays atomic.
    double param_[NUM_PARAMS];
    std::atomic<bool> trigger_{false};
    std::atomic<bool> release_{false};
    std::atomic<bool> active_{false};

    // Audio-thread-only state. All float: phase is wrapped to [0, 1) every
    // sample so it keeps full precision, and the envelopes decay over the
    // note's audible lifetime well within float accuracy.
    float phase_ = 0.0f;
    float modPhase_ = 0.0f;
    float env_ = 0.0f;
    float modEnv_ = 0.0f;
    float noiseEnv_ = 0.0f;  // independent transient/noise VCA (extension)
    float sh_ = 0.0f;
    float lp_ = 0.0f;
    float filtIc1_ = 0.0f;
    float filtIc2_ = 0.0f;
    int sweepCount_ = 0;
    float modHeld_ = 0.0f;
    float envModTime_ = 0.0f;
    float modEnvEnd_ = 0.0f;
    bool releasing_ = false;
    uint32_t rng_ = 0x9e3779b9u;
    uint32_t noiseRng_ = 0x243f6a88u;
};
