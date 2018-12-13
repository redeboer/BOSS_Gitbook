#ifndef __ranlux_fort_ph__
#define __ranlux_fort_ph__

#ifdef __cplusplus
extern "C" {
#endif

int rlxdinit_ (int *lux,int *seed);
int ranlxdf_ (double vec[],int *lvec);
int rlxdgetf_ (int *state);

#ifdef __cplusplus
}
#endif

#endif

