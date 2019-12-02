/* Copyright 2013-2017 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef TWITCHHELM_REVERB_ALL_PASS_H
#define TWITCHHELM_REVERB_ALL_PASS_H

#include "twytchhelm_memory.h"
#include "twytchhelm_processor.h"

namespace twytchhelmmopo {

  // An all-pass filter useful in a reverb processor.
  class ReverbAllPass : public Processor {
    public:
      enum Inputs {
        kAudio,
        kSampleDelay,
        kFeedback,
        kNumInputs
      };

      ReverbAllPass(int size);
      ReverbAllPass(const ReverbAllPass& other);
      virtual ~ReverbAllPass();

      virtual Processor* clone() const override {
        return new ReverbAllPass(*this);
      }

      virtual void process() override;

      void tick(int i, mopo_float* dest, int period,
                const mopo_float* audio_buffer, const mopo_float* feedback_buffer) {
        mopo_float audio = audio_buffer[i];
        mopo_float feedback = feedback_buffer[i];

        mopo_float read = memory_->getIndex(period);
        memory_->push(audio + read * feedback);
        dest[i] = read - audio;
      }

    protected:
      Memory* memory_;
  };
} // namespace twytchhelmmopo

#endif // REVERB_ALL_PASS_H
