Biquad *lpFilter = new Biquad();	// create a Biquad, lpFilter;

lpFilter->setBiquad(bq_type_lowpass, Fc / sampleRate, 0.707, 0);

// filter a buffer of input samples, in-place
for (int idx = 0; idx < bufSize; idx++) {
    in[idx] = lpFilter->process(in[idx]);
}
