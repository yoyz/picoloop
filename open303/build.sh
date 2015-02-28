#!/bin/bash
set -x


#g++ -c fft4g.c 					-o fft4g.o
g++ -c StringConversions.c 			-o StringConversions.o
g++ -c rosic_TeeBeeFilter.cpp 			-o rosic_TeeBeeFilter.o
g++ -c rosic_RealFunctions.cpp 			-o rosic_RealFunctions.o
g++ -c rosic_Open303.cpp 			-o rosic_Open303.o
g++ -c rosic_OnePoleFilter.cpp 			-o rosic_OnePoleFilter.o
g++ -c rosic_NumberManipulations.cpp 		-o rosic_NumberManipulations.o
g++ -c rosic_MipMappedWaveTable.cpp 		-o rosic_MipMappedWaveTable.o
g++ -c rosic_MidiNoteEvent.cpp 			-o rosic_MidiNoteEvent.o
g++ -c rosic_LeakyIntegrator.cpp 		-o rosic_LeakyIntegrator.o
g++ -c rosic_FunctionTemplates.cpp 		-o rosic_FunctionTemplates.o
g++ -c rosic_FourierTransformerRadix2.cpp 	-o rosic_FourierTransformerRadix2.o
g++ -c rosic_EllipticQuarterBandFilter.cpp 	-o rosic_EllipticQuarterBandFilter.o
g++ -c rosic_DecayEnvelope.cpp 			-o rosic_DecayEnvelope.o
g++ -c rosic_Complex.cpp 			-o rosic_Complex.o
g++ -c rosic_BlendOscillator.cpp 		-o rosic_BlendOscillator.o
g++ -c rosic_BiquadFilter.cpp 			-o rosic_BiquadFilter.o
g++ -c rosic_AnalogEnvelope.cpp 		-o rosic_AnalogEnvelope.o
g++ -c rosic_AcidSequencer.cpp 			-o rosic_AcidSequencer.o
g++ -c rosic_AcidPattern.cpp 			-o rosic_AcidPattern.o
g++ -c GlobalFunctions.cpp 			-o GlobalFunctions.o
g++ -c main.cpp                                 -o main.o

g++ StringConversions.o rosic_TeeBeeFilter.o rosic_RealFunctions.o rosic_Open303.o rosic_OnePoleFilter.o rosic_NumberManipulations.o rosic_MipMappedWaveTable.o rosic_MidiNoteEvent.o rosic_LeakyIntegrator.o rosic_FunctionTemplates.o rosic_FourierTransformerRadix2.o rosic_EllipticQuarterBandFilter.o rosic_DecayEnvelope.o rosic_Complex.o rosic_BlendOscillator.o rosic_BiquadFilter.o rosic_AnalogEnvelope.o rosic_AcidSequencer.o rosic_AcidPattern.o GlobalFunctions.o main.o -o main

#g++ cursynth_engine.o wave.o voice_handler.o value.o trigger_operators.o step_generator.o smooth_value.o smooth_filter.o send_receive.o processor_router.o processor.o oscillator.o operators.o mono_panner.o midi_lookup.o linear_slope.o filter.o feedback.o envelope.o delay.o cursynth_strings.o main.o -o main

#g++ -c main.cpp -o main.o && g++ cursynth_engine.o wave.o voice_handler.o value.o trigger_operators.o step_generator.o smooth_value.o smooth_filter.o send_receive.o processor_router.o processor.o oscillator.o operators.o mono_panner.o midi_lookup.o linear_slope.o filter.o feedback.o envelope.o delay.o cursynth_strings.o main.o -o main

