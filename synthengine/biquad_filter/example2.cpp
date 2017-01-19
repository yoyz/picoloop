Biquad *filter1 = new Biquad(bq_type_peak, 200.0 / sampleRate, 1.0, -3.0);
Biquad *filter2 = new Biquad(bq_type_peak, 6000.0 / sampleRate, 5.0, 6.0);

// filter a buffer of input samples, in-place
for (int idx = 0; idx < bufSize; idx++) {
    in[idx] = filter2->process(filter1->process(in[idx]));
}
