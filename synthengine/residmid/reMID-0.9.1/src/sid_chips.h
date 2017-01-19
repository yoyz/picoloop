
struct SID;

struct SID **sid_init();

void sid_close(struct SID **chips);

void sid_set_srate(struct SID **chips, int pal, double sample_freq);

short *sid_process(struct SID **chips, int num_samples);

