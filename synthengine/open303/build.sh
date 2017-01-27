#!/bin/bash
set -x


#g++ -c fft4g.c 					-o fft4g.o
g++ -O3 -c StringConversions.c 			-o StringConversions.o
g++ -O3 -c rosic_TeeBeeFilter.cpp 			-o rosic_TeeBeeFilter.o
g++ -O3 -c rosic_RealFunctions.cpp 			-o rosic_RealFunctions.o
g++ -O3 -c rosic_Open303.cpp 			-o rosic_Open303.o
g++ -O3 -c rosic_OnePoleFilter.cpp 			-o rosic_OnePoleFilter.o
g++ -O3 -c rosic_NumberManipulations.cpp 		-o rosic_NumberManipulations.o
g++ -O3 -c rosic_MipMappedWaveTable.cpp 		-o rosic_MipMappedWaveTable.o
g++ -O3 -c rosic_MidiNoteEvent.cpp 			-o rosic_MidiNoteEvent.o
g++ -O3 -c rosic_LeakyIntegrator.cpp 		-o rosic_LeakyIntegrator.o
g++ -O3 -c rosic_FunctionTemplates.cpp 		-o rosic_FunctionTemplates.o
g++ -O3 -c rosic_FourierTransformerRadix2.cpp 	-o rosic_FourierTransformerRadix2.o
g++ -O3 -c rosic_EllipticQuarterBandFilter.cpp 	-o rosic_EllipticQuarterBandFilter.o
g++ -O3 -c rosic_DecayEnvelope.cpp 			-o rosic_DecayEnvelope.o
g++ -O3 -c rosic_Complex.cpp 			-o rosic_Complex.o
g++ -O3 -c rosic_BlendOscillator.cpp 		-o rosic_BlendOscillator.o
g++ -O3 -c rosic_BiquadFilter.cpp 			-o rosic_BiquadFilter.o
g++ -O3 -c rosic_AnalogEnvelope.cpp 		-o rosic_AnalogEnvelope.o
g++ -O3 -c rosic_AcidSequencer.cpp 			-o rosic_AcidSequencer.o
g++ -O3 -c rosic_AcidPattern.cpp 			-o rosic_AcidPattern.o
g++ -O3 -c GlobalFunctions.cpp 			-o GlobalFunctions.o
g++ -O3 -c main.cpp                                 -o main.o
g++ -O3 -c bench.cpp                                -o bench.o

g++ StringConversions.o rosic_TeeBeeFilter.o rosic_RealFunctions.o rosic_Open303.o rosic_OnePoleFilter.o rosic_NumberManipulations.o rosic_MipMappedWaveTable.o rosic_MidiNoteEvent.o rosic_LeakyIntegrator.o rosic_FunctionTemplates.o rosic_FourierTransformerRadix2.o rosic_EllipticQuarterBandFilter.o rosic_DecayEnvelope.o rosic_Complex.o rosic_BlendOscillator.o rosic_BiquadFilter.o rosic_AnalogEnvelope.o rosic_AcidSequencer.o rosic_AcidPattern.o GlobalFunctions.o main.o -o main
g++ StringConversions.o rosic_TeeBeeFilter.o rosic_RealFunctions.o rosic_Open303.o rosic_OnePoleFilter.o rosic_NumberManipulations.o rosic_MipMappedWaveTable.o rosic_MidiNoteEvent.o rosic_LeakyIntegrator.o rosic_FunctionTemplates.o rosic_FourierTransformerRadix2.o rosic_EllipticQuarterBandFilter.o rosic_DecayEnvelope.o rosic_Complex.o rosic_BlendOscillator.o rosic_BiquadFilter.o rosic_AnalogEnvelope.o rosic_AcidSequencer.o rosic_AcidPattern.o GlobalFunctions.o bench.o -o bench


