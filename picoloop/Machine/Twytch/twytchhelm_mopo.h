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
#ifndef TWITCHHELM_MOPO_H
#define TWITCHHELM_MOPO_H

#include "twytchhelm_alias.h"
#include "twytchhelm_arpeggiator.h"
#include "twytchhelm_bit_crush.h"
#include "twytchhelm_biquad_filter.h"
#include "twytchhelm_bypass_router.h"
#include "twytchhelm_circular_queue.h"
#include "twytchhelm_common.h"
#include "twytchhelm_delay.h"
#include "twytchhelm_distortion.h"
#include "twytchhelm_envelope.h"
#include "twytchhelm_feedback.h"
#include "twytchhelm_formant_manager.h"
#include "twytchhelm_linear_slope.h"
#include "twytchhelm_magnitude_lookup.h"
#include "twytchhelm_memory.h"
#include "twytchhelm_midi_lookup.h"
#include "twytchhelm_mono_panner.h"
#include "twytchhelm_note_handler.h"
#include "twytchhelm_operators.h"
#include "twytchhelm_oscillator.h"
#include "twytchhelm_portamento_slope.h"
#include "twytchhelm_processor.h"
#include "twytchhelm_processor_router.h"
#include "twytchhelm_resonance_lookup.h"
#include "twytchhelm_reverb.h"
#include "twytchhelm_reverb_all_pass.h"
#include "twytchhelm_reverb_comb.h"
#include "twytchhelm_simple_delay.h"
#include "twytchhelm_smooth_filter.h"
#include "twytchhelm_smooth_value.h"
#include "twytchhelm_state_variable_filter.h"
#include "twytchhelm_step_generator.h"
#include "twytchhelm_stutter.h"
#include "twytchhelm_tick_router.h"
#include "twytchhelm_trigger_operators.h"
#include "twytchhelm_utils.h"
#include "twytchhelm_value.h"
#include "twytchhelm_voice_handler.h"
#include "twytchhelm_wave.h"

#endif // MOPO_H
