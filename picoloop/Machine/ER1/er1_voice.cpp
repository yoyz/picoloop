#include "er1_voice.h"

#include <cmath>
#include <cstdio>

static constexpr double PITCH_MIN = 20.0, PITCH_MAX = 12000.0;
static constexpr double SPEED_MIN = 0.1, SPEED_MAX = 5000.0;
static constexpr double DECAY_MIN = 0.005, DECAY_MAX = 10.0;
static constexpr double MOD_DEPTH_OCTAVES = 2.0;
static constexpr double DECAY_DB = 20.0;
static constexpr int PITCH_NOTE_MIN = -8;
static constexpr int PITCH_NOTE_MAX = 126;
static constexpr int WAVE_NOISE = 6;
static constexpr int MOD_NOISE = 4;
static constexpr int MOD_ENVELOPE = 5;
static constexpr int MOD_PULSE25 = 6;
static constexpr int MOD_PULSE125 = 7;
static const char *g_names[NUM_PARAMS] = {
    "Wave", "Pitch", "Detune", "Mod Type", "Mod Depth", "Mod Speed",
    "Sweeps", "Phase", "Span", "Wave Warp", "Bit Depth", "Decay",
    "Noise", "Noise Decay", "Level", "Pan", "Low Boost", "Filter",
    "Filter Pos", "Reso", "Key Track", "Filter Type",
};
static const bool g_discrete[NUM_PARAMS] = {
    true, false, false, true, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, true, false, false, true,
};
static const char *g_waves[7] = {
    "Sine", "Triangle", "Saw", "Square", "Pulse 25%", "Pulse 12.5%", "Noise",
};
static const char *g_mods[8] = {
    "Saw Down", "Square", "Triangle", "Sample & Hold", "Noise", "Envelope",
    "Pulse 25%", "Pulse 12.5%",
};
static const char *g_filters[4] = {"Off", "Lowpass", "Bandpass", "Highpass"};
static const char *g_filterPos[3] = {"Pre VCA", "Post VCA", "Post Crush"};

static double expMap(double t, double lo, double hi) {
    return lo * std::pow(hi / lo, t);
}

static double filterCutoffHz(double knob) {
    return 20.0 * std::pow(18000.0 / 20.0, knob / 127.0);
}

static double filterResoQ(double knob) {
    return 0.5 * std::pow(16.0, knob / 100.0);
}

// Bipolar log/audio taper for Wave Warp. Maps knob position t in [-1, 1] so
// that the audible change is spread across the usable range instead of being
// concentrated near +-100: 0 -> 0, +-0.5 -> +-0.74, +-1 -> +-1.
static double warpTaper(double t) {
    double a = std::fabs(t);
    if (a <= 0.0) return 0.0;
    double shaped = std::log1p(9.0 * a) / std::log(10.0);
    return t < 0.0 ? -shaped : shaped;
}

static char lowerNoSep(char c) {
    if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    if (c == ' ' || c == '_' || c == '-') return '\0';
    return c;
}

static bool nameEquals(const char *a, const char *b) {
    while (true) {
        while (*a && lowerNoSep(*a) == '\0') ++a;
        while (*b && lowerNoSep(*b) == '\0') ++b;
        if (lowerNoSep(*a) != lowerNoSep(*b)) return false;
        if (*a == '\0' || *b == '\0') return *a == '\0' && *b == '\0';
        ++a;
        ++b;
    }
}

int paramCount() { return NUM_PARAMS; }

const char *paramName(int i) {
    if (i < 0 || i >= NUM_PARAMS) return "";
    return g_names[i];
}

bool paramIsDiscrete(int i) {
    if (i < 0 || i >= NUM_PARAMS) return false;
    return g_discrete[i];
}

double paramMin(int i) {
    switch (i) {
        case P_PITCH: return PITCH_NOTE_MIN;
        case P_DETUNE: return -1.0;
        case P_MODDEPTH: return -64.0;
        case P_MODSPEED: return 0.0;
        case P_WAVEWARP: return -100.0;
        case P_BITDEPTH: return 1.0;
        case P_DECAY: return 0.0;
        case P_LEVEL: return 0.0;
        case P_PAN: return -100.0;
        case P_LOWBOOST: return 0.0;
        case P_FILTER: return 0.0;
        case P_FILTERPOS: return 0.0;
        case P_RESO: return 0.0;
        case P_KEYTRACK: return 0.0;
        case P_FILTERTYPE: return 0.0;
        default: return 0.0;
    }
}

double paramMax(int i) {
    switch (i) {
        case P_WAVE: return 6.0;
        case P_PITCH: return PITCH_NOTE_MAX;
        case P_DETUNE: return 1.0;
        case P_MODTYPE: return 7.0;
        case P_MODDEPTH: return 64.0;
        case P_MODSPEED: return 127.0;
        case P_SWEEPS: return 64.0;
        case P_PHASE: return 360.0;
        case P_SPAN: return 360.0;
        case P_WAVEWARP: return 100.0;
        case P_BITDEPTH: return 17.0;
        case P_DECAY: return 128.0;
        case P_NOISE: return 100.0;
        case P_NOISEDECAY: return 127.0;
        case P_LEVEL: return 100.0;
        case P_PAN: return 100.0;
        case P_LOWBOOST: return 100.0;
        case P_FILTER: return 127.0;
        case P_FILTERPOS: return 2.0;
        case P_RESO: return 100.0;
        case P_KEYTRACK: return 100.0;
        case P_FILTERTYPE: return 3.0;
        default: return 0.0;
    }
}

double paramStep(int i) {
    switch (i) {
        case P_DETUNE: return 0.1;
        case P_MODDEPTH: return 1.0;
        case P_LEVEL: return 2.0;
        case P_PAN: return 4.0;
        case P_LOWBOOST: return 4.0;
        case P_PHASE: return 15.0;
        case P_SPAN: return 15.0;
        default: return 1.0;
    }
}

int paramIndex(const char *name) {
    for (int i = 0; i < NUM_PARAMS; ++i) {
        if (nameEquals(g_names[i], name)) return i;
    }
    return -1;
}

int paramValueCount(int i) {
    switch (i) {
        case P_WAVE: return 7;
        case P_MODTYPE: return 8;
        case P_FILTERPOS: return 3;
        case P_FILTERTYPE: return 4;
        default: return 0;
    }
}

const char *paramValueName(int i, int idx) {
    switch (i) {
        case P_WAVE:
            if (idx >= 0 && idx < 7) return g_waves[idx];
            break;
        case P_MODTYPE:
            if (idx >= 0 && idx < 8) return g_mods[idx];
            break;
        case P_FILTERPOS:
            if (idx >= 0 && idx < 3) return g_filterPos[idx];
            break;
        case P_FILTERTYPE:
            if (idx >= 0 && idx < 4) return g_filters[idx];
            break;
        default:
            break;
    }
    return nullptr;
}

// --- WaveTables ---

WaveTables::WaveTables() {
    // Pulses are zero-mean: the high level is +1 and the low level is chosen
    // so the average over one cycle is zero. This avoids a DC step at note-on.
    constexpr float P25_LOW = -1.0f / 3.0f;
    constexpr float P125_LOW = -1.0f / 7.0f;
    for (int i = 0; i < LEN; ++i) {
        double t = static_cast<double>(i) / LEN;
        sine[i] = static_cast<float>(std::sin(2.0 * M_PI * t));
        tri[i] = static_cast<float>(1.0 - 4.0 * std::fabs(t - 0.5));
        saw[i] = static_cast<float>(2.0 * t - 1.0);
        square[i] = t < 0.5 ? 1.0f : -1.0f;
        pulse25[i] = t < 0.25 ? 1.0f : P25_LOW;
        pulse125[i] = t < 0.125 ? 1.0f : P125_LOW;
    }
    sine[LEN] = sine[0];
    tri[LEN] = tri[0];
    saw[LEN] = saw[0];
    square[LEN] = square[0];
    pulse25[LEN] = pulse25[0];
    pulse125[LEN] = pulse125[0];
}

float WaveTables::read(const float *tab, float phase) const {
    // Fast frac(phase): truncation toward zero, then correct negatives. The
    // carrier phase stays inside roughly [-0.4, 1.4] cycles, so int is safe.
    float p = phase - static_cast<int>(phase);
    if (p < 0.0f) p += 1.0f;
    if (p >= 1.0f) p = 0.0f;  // float rounding can hit exactly 1.0
    float pos = p * LEN;
    int i = static_cast<int>(pos);
    float f = pos - i;
    return tab[i] + f * (tab[i + 1] - tab[i]);
}

// --- ER1Voice ---

ER1Voice::ER1Voice(const WaveTables &tables, double sampleRate)
    : tables_(tables), sampleRate_(sampleRate) {
    initDefaults();
}

void ER1Voice::reset() {
    phase_ = 0.0f;
    modPhase_ = 0.0f;
    env_ = 0.0f;
    modEnv_ = 0.0f;
    noiseEnv_ = 0.0f;
    sh_ = 0.0f;
    lp_ = 0.0f;
    filtIc1_ = 0.0f;
    filtIc2_ = 0.0f;
    sweepCount_ = 0;
    modHeld_ = 0.0f;
    envModTime_ = 0.0f;
    modEnvEnd_ = 0.0f;
    releasing_ = false;
    trigger_.store(false);
    release_.store(false);
    active_.store(false);
}

void ER1Voice::initDefaults() {
    param_[P_WAVE]=0.0;
    param_[P_PITCH]=42.0;
    param_[P_DETUNE]=0.0;
    param_[P_MODTYPE]=5.0;
    param_[P_MODDEPTH]=64.0;
    param_[P_MODSPEED]=90.0;
    param_[P_SWEEPS]=0.0;
    param_[P_PHASE]=0.0;
    param_[P_SPAN]=360.0;
    param_[P_WAVEWARP]=0.0;
    param_[P_BITDEPTH]=17.0;
    param_[P_DECAY]=80.0;
    param_[P_NOISE]=0.0;
    param_[P_NOISEDECAY]=50.0;
    param_[P_LEVEL]=80.0;
    param_[P_PAN]=0.0;
    param_[P_LOWBOOST]=0.0;
    param_[P_FILTER]=127.0;
    param_[P_FILTERPOS]=1.0;
    param_[P_RESO]=0.0;
    param_[P_KEYTRACK]=0.0;
    param_[P_FILTERTYPE]=0.0;
}

void ER1Voice::render(float *out, unsigned long frames) {
    double pitchKnob = param_[P_PITCH];
    double detune = param_[P_DETUNE];
    double waveSel = param_[P_WAVE];
    double modType = param_[P_MODTYPE];
    int mt = static_cast<int>(modType + 0.5);
    if (mt < 0) mt = 0;
    if (mt > 7) mt = 7;
    float modDepth = static_cast<float>(param_[P_MODDEPTH] / 64.0 *
                                        modDepthScale(mt));
    double speedKnob = param_[P_MODSPEED];
    double sweeps = param_[P_SWEEPS];
    double phaseDeg = param_[P_PHASE];
    double spanDeg = param_[P_SPAN];
    double decayKnob = param_[P_DECAY];
    float level = static_cast<float>(param_[P_LEVEL] / 100.0);
    float pan = static_cast<float>(param_[P_PAN] / 100.0);
    float lowBoost =
        static_cast<float>(param_[P_LOWBOOST] / 100.0 * 2.0);
    float bits = static_cast<float>(param_[P_BITDEPTH]);
    float noiseLevel = static_cast<float>(param_[P_NOISE] / 100.0);
    double noiseDecayKnob = param_[P_NOISEDECAY];
    double filterKnob = param_[P_FILTER];
    if (filterKnob < 0.0) filterKnob = 0.0;
    if (filterKnob > 127.0) filterKnob = 127.0;
    double resoKnob = param_[P_RESO];
    if (resoKnob < 0.0) resoKnob = 0.0;
    if (resoKnob > 100.0) resoKnob = 100.0;
    double keyTrackKnob = param_[P_KEYTRACK];
    if (keyTrackKnob < 0.0) keyTrackKnob = 0.0;
    if (keyTrackKnob > 100.0) keyTrackKnob = 100.0;
    int filterType = static_cast<int>(param_[P_FILTERTYPE] + 0.5);
    if (filterType < 0) filterType = 0;
    if (filterType > 3) filterType = 3;
    int filterPos = static_cast<int>(param_[P_FILTERPOS] + 0.5);
    if (filterPos < 0) filterPos = 0;
    if (filterPos > 2) filterPos = 2;
    float waveWarp =
        static_cast<float>(warpTaper(param_[P_WAVEWARP] / 100.0));

    const float sr = static_cast<float>(sampleRate_);
    const float srInv = 1.0f / sr;
    float pitchHz = 440.0f * std::exp2f(
                        static_cast<float>((pitchKnob - 69.0) + detune) /
                        12.0f);
    float modSpeed =
        static_cast<float>(expMap(speedKnob / 127.0, SPEED_MIN, SPEED_MAX));
    float decayS =
        static_cast<float>(expMap(decayKnob / 127.0, DECAY_MIN, DECAY_MAX));
    int nSweeps = static_cast<int>(sweeps + 0.5);

    double tauAmp = static_cast<double>(decayS) /
                    (DECAY_DB / 20.0 * std::log(10.0));
    float ampCoef =
        static_cast<float>(std::exp(-1.0 / (sampleRate_ * tauAmp)));
    if (decayKnob >= 127.5) ampCoef = 1.0f;
    float releaseCoef = static_cast<float>(
        std::exp(-1.0 / (sampleRate_ * 0.005)));  // ~5 ms on note-off
    double tauMod = 1.0 / (2.0 * (modSpeed > 0.001f
                                      ? static_cast<double>(modSpeed)
                                      : 0.001));
    float modCoef =
        static_cast<float>(std::exp(-1.0 / (sampleRate_ * tauMod)));
    float envLen = static_cast<float>(tauMod * std::log(1000.0));
    float start = static_cast<float>(phaseDeg) / 360.0f;
    float envT0 = start * envLen;
    float envT1 = envT0 + (static_cast<float>(spanDeg) / 360.0f) * envLen;
    float span = static_cast<float>(spanDeg) / 360.0f;
    if (span < 0.0139f) span = 0.0139f;
    float lpCoef = 1.0f - std::exp(-2.0f * float(M_PI) * 120.0f / sr);

    float depthScale = static_cast<float>(MOD_DEPTH_OCTAVES) * modDepth;
    const bool doPitchMod = modDepth != 0.0f;
    const bool doBits = bits < 16.5f;
    const float bitQ = doBits ? std::exp2f(1.0f - bits) : 0.0f;

    // Noise transient (extension): independent white-noise layer and decay.
    float noiseDecayS = static_cast<float>(
        expMap(noiseDecayKnob / 127.0, DECAY_MIN, DECAY_MAX));
    double tauNoise = static_cast<double>(noiseDecayS) /
                      (DECAY_DB / 20.0 * std::log(10.0));
    float noiseCoef =
        static_cast<float>(std::exp(-1.0 / (sampleRate_ * tauNoise)));
    const bool doNoise = noiseLevel > 0.0f;

    // 2-pole TPT state-variable filter (extension). The cutoff is prewarped so
    // the response stays accurate up to Nyquist, and the feedback loop is
    // solved implicitly (no naive-SVF cutoff limit).
    const bool doFilter = filterType != 0;
    float filtK = 0.0f, filtA1 = 0.0f, filtA2 = 0.0f, filtA3 = 0.0f;
    if (doFilter) {
        double cutoff = filterCutoffHz(filterKnob);
        if (keyTrackKnob > 0.0) {
            // Key tracking is relative to A4 (MIDI 69, 440 Hz): 100% doubles
            // the cutoff per octave. It follows the played note plus detune,
            // not the audio-rate pitch modulation.
            double note = pitchKnob + detune;
            cutoff *= std::pow(2.0, (note - 69.0) / 12.0 *
                                        (keyTrackKnob / 100.0));
            if (cutoff < 20.0) cutoff = 20.0;
            if (cutoff > 18000.0) cutoff = 18000.0;
        }
        double q = filterResoQ(resoKnob);
        float g = static_cast<float>(
            std::tan(M_PI * cutoff / sampleRate_));
        filtK = static_cast<float>(1.0 / q);
        filtA1 = 1.0f / (1.0f + g * (g + filtK));
        filtA2 = g * filtA1;
        filtA3 = g * filtA2;
    }

    auto applyFilter = [&](float x) {
        float v3 = x - filtIc2_;
        float v1 = filtA1 * filtIc1_ + filtA2 * v3;
        float v2 = filtIc2_ + filtA2 * filtIc1_ + filtA3 * v3;
        filtIc1_ = 2.0f * v1 - filtIc1_;
        filtIc2_ = 2.0f * v2 - filtIc2_;
        if (filterType == 1) return v2;               // lowpass
        if (filterType == 2) return filtK * v1;       // bandpass
        return x - filtK * v1 - v2;                   // highpass
    };

    const float *tab = tables_.sine;
    int wi = static_cast<int>(waveSel + 0.5);
    if (wi < 0) wi = 0;
    if (wi > 6) wi = 6;
    if (wi == 1) tab = tables_.tri;
    else if (wi == 2) tab = tables_.saw;
    else if (wi == 3) tab = tables_.square;
    else if (wi == 4) tab = tables_.pulse25;
    else if (wi == 5) tab = tables_.pulse125;

    float theta = (pan + 1.0f) * (float(M_PI) / 4.0f);
    float panL = std::cos(theta);
    float panR = std::sin(theta);
    float gainL = level * panL;
    float gainR = level * panR;

    for (unsigned long i = 0; i < frames; ++i) {
        // Check with a cheap relaxed load first; only take the (locked)
        // atomic exchange when an event is actually pending. This keeps
        // per-sample trigger latency without a locked op on every sample.
        if (release_.load(std::memory_order_relaxed) &&
            release_.exchange(false, std::memory_order_relaxed))
            releasing_ = true;
        if (trigger_.load(std::memory_order_relaxed) &&
            trigger_.exchange(false, std::memory_order_relaxed)) {
            phase_ = 0.0f;
            modPhase_ = 0.0f;
            sweepCount_ = 0;
            modHeld_ = 0.0f;
            env_ = 1.0f;
            modEnv_ = std::exp(-envT0 / tauMod);
            noiseEnv_ = 1.0f;
            filtIc1_ = 0.0f;
            filtIc2_ = 0.0f;
            envModTime_ = envT0;
            modEnvEnd_ = envT1;
            lp_ = 0.0f;
            releasing_ = false;  // a fresh note-on wins over a pending note-off
            sh_ = nextRand();
        }

        float s = 0.0f;
        bool any = false;
        if (env_ > 1e-4f) {
            any = true;
            bool frozen = (mt <= 3 || mt >= MOD_PULSE25) && sweeps > 0.0 &&
                          sweepCount_ >= nSweeps;
            float m = 0.0f;

            if (frozen) {
                m = modHeld_;
            } else {
                float ang = start + modPhase_;
                if (ang >= 1.0f) ang -= 1.0f;
                float frac = ang;
                switch (mt) {
                    case 0: m = 1.0f - 2.0f * frac; break;
                    case 1: m = (frac < 0.5f) ? 1.0f : -1.0f; break;
                    case 2: m = 1.0f - 4.0f * std::fabs(frac - 0.5f); break;
                    case 3: m = sh_; break;
                    case MOD_NOISE: m = nextRand(); break;
                    case MOD_ENVELOPE: m = modEnv_; break;
                    case MOD_PULSE25:
                        m = (frac < 0.25f) ? 1.0f : -1.0f / 3.0f;
                        break;
                    case MOD_PULSE125:
                        m = (frac < 0.125f) ? 1.0f : -1.0f / 7.0f;
                        break;
                    default: m = 0.0f; break;
                }
            }

            if (mt == MOD_ENVELOPE) {
                if (envModTime_ < modEnvEnd_) {
                    modEnv_ *= modCoef;
                    envModTime_ += srInv;
                }
            } else if (mt <= 3 || mt >= MOD_PULSE25) {
                if (!frozen) {
                    modPhase_ += modSpeed * srInv;
                    while (modPhase_ >= span) {
                        if (sweeps > 0.0 && sweepCount_ + 1 >= nSweeps) {
                            modHeld_ = m;
                            sweepCount_++;
                            break;
                        }
                        modPhase_ -= span;
                        sweepCount_++;
                        if (mt == 3) sh_ = nextRand();
                    }
                }
            }

            float freq = doPitchMod ? pitchHz * std::exp2f(depthScale * m)
                                    : pitchHz;
            if (freq < 1e-3f) freq = 1e-3f;

            float carrierPhase = phase_;
            if (waveWarp != 0.0f && wi != WAVE_NOISE) {
                // Bipolar phase distortion, applied to the carrier before the
                // pitch/FM stage. The read position shifts by a fraction of a
                // cycle proportional to the carrier's own sine, so one half of
                // the cycle is compressed and the other is stretched. Exactly
                // one cycle still fits in each period: no pitch change, only
                // timbre. Two cascaded passes roughly double the effect while
                // each pass stays monotonic (|warp| <= 1, no wavefolding).
                for (int pass = 0; pass < 2; ++pass) {
                    carrierPhase +=
                        waveWarp * (tables_.read(tables_.sine, carrierPhase) /
                                    (2.0f * float(M_PI)));
                }
            }
            float carrier = (wi == WAVE_NOISE)
                                ? nextRand()
                                : tables_.read(tab, carrierPhase);
            if (doFilter && filterPos == 0) carrier = applyFilter(carrier);
            s = carrier * env_;
            phase_ += freq * srInv;
            if (phase_ >= 1.0f) phase_ -= static_cast<int>(phase_);
            env_ *= releasing_ ? releaseCoef : ampCoef;

            lp_ += lpCoef * (s - lp_);
            s += lowBoost * lp_;
        }

        // The noise layer has its own VCA, so it can outlast the main body.
        if (doNoise && noiseEnv_ > 1e-4f) {
            any = true;
            s += noiseLevel * noiseEnv_ * nextNoiseRand();
            noiseEnv_ *= releasing_ ? releaseCoef : noiseCoef;
            if (noiseEnv_ < 1e-4f) noiseEnv_ = 0.0f;
        }

        if (doFilter && filterPos == 1 && any) s = applyFilter(s);

        if (doBits && any) {
            float x = (s + 1.0f) / bitQ;
            int ix = static_cast<int>(x);
            if (static_cast<float>(ix) > x) --ix;
            s = (ix + 0.5f) * bitQ - 1.0f;
            if (s > 1.0f) s = 1.0f;
            if (s < -1.0f) s = -1.0f;
        }

        if (doFilter && filterPos == 2 && any) s = applyFilter(s);

        out[i * 2 + 0] = s * gainL;
        out[i * 2 + 1] = s * gainR;

        if (i == frames - 1) {
            active_.store(any);
        }
    }
}

void ER1Voice::adjustParam(int i, int dir) {
    double v = param_[i];

    // Sweeps has free (0) above the finite range. Increase from 64 reaches
    // free; decrease from free starts at 64. The TUI swaps left/right for this
    // parameter (and Bit Depth) so right reduces the displayed quantity.
    if (i == P_SWEEPS) {
        const double maxSweeps = paramMax(P_SWEEPS);
        if (dir > 0) {
            if (v >= maxSweeps - 0.5) v = 0.0;
            else if (v >= 0.5) v += 1.0;
        } else {
            if (v < 0.5) v = maxSweeps;
            else if (v > 1.5) v -= 1.0;
            if (v < 1.0) v = 1.0;
        }
        param_[i]=v;
        return;
    }

    if (g_discrete[i]) {
        int count = static_cast<int>(paramMax(i)) + 1;
        int n = (static_cast<int>(v + 0.5) + dir) % count;
        if (n < 0) n += count;
        param_[i]=static_cast<double>(n);
    } else {
        double step = paramStep(i);
        if (i == P_BITDEPTH) {
            if (v < 4.0 - 1e-9) step = 0.1;
            else if (v < 4.0 + 1e-9) step = (dir > 0) ? 1.0 : 0.1;
            else step = 1.0;
        }
        v += dir * step;
        if (v < paramMin(i)) v = paramMin(i);
        if (v > paramMax(i)) v = paramMax(i);
        param_[i]=v;
    }
}

std::string ER1Voice::valueStr(int i) const {
    char buf[64];
    double v = param_[i];
    switch (i) {
        case P_WAVE: {
            int idx = static_cast<int>(v + 0.5);
            if (idx < 0) idx = 0;
            if (idx > 6) idx = 6;
            return g_waves[idx];
        }
        case P_MODTYPE: {
            int idx = static_cast<int>(v + 0.5);
            if (idx < 0) idx = 0;
            if (idx > 7) idx = 7;
            return g_mods[idx];
        }
        case P_FILTERTYPE: {
            int idx = static_cast<int>(v + 0.5);
            if (idx < 0) idx = 0;
            if (idx > 3) idx = 3;
            return g_filters[idx];
        }
        case P_PITCH: {
            static const char *nm[12] = {"C",  "C#", "D",  "D#", "E",  "F",
                                         "F#", "G",  "G#", "A",  "A#", "B"};
            int note = static_cast<int>(std::lround(v));
            double det = param_[P_DETUNE];
            double hz = 440.0 * std::pow(2.0, (note - 69 + det) / 12.0);
            int oct = static_cast<int>(std::floor(note / 12.0)) - 1;
            std::snprintf(buf, sizeof(buf), "%s%d  %6.1fHz",
                          nm[((note % 12) + 12) % 12], oct, hz);
            return buf;
        }
        case P_DETUNE:
            std::snprintf(buf, sizeof(buf), "%+5.2f st", v);
            return buf;
        case P_MODSPEED:
            std::snprintf(buf, sizeof(buf), "%7.2f Hz",
                          expMap(v / 127.0, SPEED_MIN, SPEED_MAX));
            return buf;
        case P_DECAY:
            if (v >= 127.5) return "infinite";
            std::snprintf(buf, sizeof(buf), "%7.1f ms",
                          expMap(v / 127.0, DECAY_MIN, DECAY_MAX) * 1000.0);
            return buf;
        case P_MODDEPTH:
            std::snprintf(buf, sizeof(buf), "%+6.0f", v);
            return buf;
        case P_NOISE:
            std::snprintf(buf, sizeof(buf), "%6.0f", v);
            return buf;
        case P_NOISEDECAY:
            std::snprintf(buf, sizeof(buf), "%7.1f ms",
                          expMap(v / 127.0, DECAY_MIN, DECAY_MAX) * 1000.0);
            return buf;
        case P_SWEEPS:
            if (v < 0.5) return "  free";
            std::snprintf(buf, sizeof(buf), "%6d", static_cast<int>(v + 0.5));
            return buf;
        case P_PHASE:
            std::snprintf(buf, sizeof(buf), "%4.0f deg", v);
            return buf;
        case P_SPAN:
            std::snprintf(buf, sizeof(buf), "%4.0f deg", v);
            return buf;
        case P_WAVEWARP:
            if (std::fabs(v) < 0.5) return "   off";
            std::snprintf(buf, sizeof(buf), "%+6.0f", v);
            return buf;
        case P_BITDEPTH:
            if (v >= 16.5) {
                std::snprintf(buf, sizeof(buf), "%4s", "full");
            } else if (std::fabs(v - std::round(v)) < 0.05) {
                std::snprintf(buf, sizeof(buf), "%4d bit",
                              static_cast<int>(std::lround(v)));
            } else {
                std::snprintf(buf, sizeof(buf), "%4.1f bit", v);
            }
            return buf;
        case P_FILTER:
            std::snprintf(buf, sizeof(buf), "%7.0f Hz", filterCutoffHz(v));
            return buf;
        case P_FILTERPOS: {
            int idx = static_cast<int>(v + 0.5);
            if (idx < 0) idx = 0;
            if (idx > 2) idx = 2;
            return g_filterPos[idx];
        }
        case P_RESO:
            std::snprintf(buf, sizeof(buf), "Q %4.2f", filterResoQ(v));
            return buf;
        case P_KEYTRACK:
            std::snprintf(buf, sizeof(buf), "%5.0f%%", v);
            return buf;
        default:
            std::snprintf(buf, sizeof(buf), "%6.0f", v);
            return buf;
    }
}
