#!/bin/bash
set -x

g++  -std=c++11 -g  -c alias.cpp   -o      alias.o
g++  -std=c++11 -g  -c arpeggiator.cpp     -o      arpeggiator.o
g++  -std=c++11 -g  -c bit_crush.cpp       -o      bit_crush.o
g++  -std=c++11 -g  -c bypass_router.cpp   -o      bypass_router.o
g++  -std=c++11 -g  -c delay.cpp   -o      delay.o
g++  -std=c++11 -g  -c distortion.cpp      -o      distortion.o
g++  -std=c++11 -g  -c envelope.cpp        -o      envelope.o
g++  -std=c++11 -g  -c feedback.cpp        -o      feedback.o
g++  -std=c++11 -g  -c filter.cpp  -o      filter.o
g++  -std=c++11 -g  -c fixed_point_oscillator.cpp  -o      fixed_point_oscillator.o
g++  -std=c++11 -g  -c fixed_point_wave.cpp        -o      fixed_point_wave.o
g++  -std=c++11 -g  -c formant_manager.cpp -o      formant_manager.o
g++  -std=c++11 -g  -c helm_engine.cpp     -o      helm_engine.o
g++  -std=c++11 -g  -c helm_lfo.cpp        -o      helm_lfo.o
g++  -std=c++11 -g  -c helm_module.cpp     -o      helm_module.o
g++  -std=c++11 -g  -c helm_common.cpp     -o      helm_common.o
g++  -std=c++11 -g  -c helm_oscillators.cpp        -o      helm_oscillators.o
g++  -std=c++11 -g  -c helm_voice_handler.cpp      -o      helm_voice_handler.o
g++  -std=c++11 -g  -c linear_slope.cpp    -o      linear_slope.o
g++  -std=c++11 -g  -c magnitude_lookup.cpp        -o      magnitude_lookup.o
g++  -std=c++11 -g  -c memory.cpp  -o      memory.o
g++  -std=c++11 -g  -c midi_lookup.cpp     -o      midi_lookup.o
g++  -std=c++11 -g  -c mono_panner.cpp     -o      mono_panner.o
g++  -std=c++11 -g  -c noise_oscillator.cpp        -o      noise_oscillator.o
g++  -std=c++11 -g  -c operators.cpp       -o      operators.o
g++  -std=c++11 -g  -c oscillator.cpp      -o      oscillator.o
g++  -std=c++11 -g  -c phaser.cpp  -o      phaser.o
g++  -std=c++11 -g  -c processor.cpp       -o      processor.o
g++  -std=c++11 -g  -c processor_router.cpp        -o      processor_router.o
g++  -std=c++11 -g  -c resonance_cancel.cpp        -o      resonance_cancel.o
g++  -std=c++11 -g  -c resonance_lookup.cpp        -o      resonance_lookup.o
g++  -std=c++11 -g  -c reverb_all_pass.cpp -o      reverb_all_pass.o
g++  -std=c++11 -g  -c reverb_comb.cpp     -o      reverb_comb.o
g++  -std=c++11 -g  -c reverb.cpp  -o      reverb.o
g++  -std=c++11 -g  -c simple_delay.cpp    -o      simple_delay.o
g++  -std=c++11 -g  -c smooth_filter.cpp   -o      smooth_filter.o
g++  -std=c++11 -g  -c smooth_value.cpp    -o      smooth_value.o
g++  -std=c++11 -g  -c step_generator.cpp  -o      step_generator.o
g++  -std=c++11 -g  -c stutter.cpp -o      stutter.o
g++  -std=c++11 -g  -c switch.cpp  -o      switch.o
g++  -std=c++11 -g  -c trigger_operators.cpp       -o      trigger_operators.o
g++  -std=c++11 -g  -c value.cpp   -o      value.o
g++  -std=c++11 -g  -c voice_handler.cpp   -o      voice_handler.o
g++  -std=c++11 -g  -c wave.cpp    -o      wave.o

g++  -std=c++11 -g  -c main.cpp    -o      main.o

g++ alias.o arpeggiator.o bit_crush.o bypass_router.o delay.o distortion.o envelope.o feedback.o filter.o fixed_point_oscillator.o fixed_point_wave.o formant_manager.o helm_engine.o helm_lfo.o helm_module.o helm_oscillators.o helm_voice_handler.o linear_slope.o magnitude_lookup.o memory.o midi_lookup.o mono_panner.o noise_oscillator.o operators.o oscillator.o phaser.o processor.o processor_router.o resonance_cancel.o resonance_lookup.o reverb_all_pass.o reverb_comb.o reverb.o simple_delay.o smooth_filter.o smooth_value.o step_generator.o stutter.o switch.o trigger_operators.o value.o voice_handler.o wave.o main.o helm_common.o -o main

#g++ cursynth_engine.o wave.o voice_handler.o value.o trigger_operators.o step_generator.o smooth_value.o smooth_filter.o send_receive.o processor_router.o processor.o oscillator.o operators.o mono_panner.o midi_lookup.o linear_slope.o filter.o feedback.o envelope.o delay.o cursynth_strings.o main.o -o main

#g++ -c main.cpp -o main.o && g++ cursynth_engine.o wave.o voice_handler.o value.o trigger_operators.o step_generator.o smooth_value.o smooth_filter.o send_receive.o processor_router.o processor.o oscillator.o operators.o mono_panner.o midi_lookup.o linear_slope.o filter.o feedback.o envelope.o delay.o cursynth_strings.o main.o -o main
#g++ -c main.cpp -o main.o && g++ alias.o arpeggiator.o bit_crush.o bypass_router.o delay.o distortion.o envelope.o feedback.o filter.o fixed_point_oscillator.o fixed_point_wave.o formant_manager.o helm_engine.o helm_lfo.o helm_module.o helm_oscillators.o helm_voice_handler.o linear_slope.o magnitude_lookup.o memory.o midi_lookup.o mono_panner.o noise_oscillator.o operators.o oscillator.o phaser.o processor.o processor_router.o resonance_cancel.o resonance_lookup.o reverb_all_pass.o reverb_comb.o reverb.o simple_delay.o smooth_filter.o smooth_value.o step_generator.o stutter.o switch.o trigger_operators.o value.o voice_handler.o wave.o main.o -o main

#g++ -std=c++11 -g   alias.cpp arpeggiator.cpp bit_crush.cpp bypass_router.cpp delay.cpp distortion.cpp envelope.cpp feedback.cpp filter.cpp fixed_point_oscillator.cpp fixed_point_wave.cpp formant_manager.cpp helm_common.cpp helm_engine.cpp helm_lfo.cpp helm_module.cpp helm_oscillators.cpp helm_voice_handler.cpp linear_slope.cpp magnitude_lookup.cpp main.cpp memory.cpp midi_lookup.cpp mono_panner.cpp noise_oscillator.cpp operators.cpp oscillator.cpp phaser.cpp processor.cpp processor_router.cpp resonance_cancel.cpp resonance_lookup.cpp reverb_all_pass.cpp reverb_comb.cpp reverb.cpp simple_delay.cpp smooth_filter.cpp smooth_value.cpp step_generator.cpp stutter.cpp switch.cpp trigger_operators.cpp value.cpp voice_handler.cpp wave.cpp -o main.o
