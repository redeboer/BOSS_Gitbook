#ifndef __ranlxd_ph__
#define __ranlxd_ph__

#ifdef __cplusplus
extern "C" {
#endif

*   "User's guide for ranlxs and ranlxd v3.0"  (May 2001);
*   "Algorithms used in ranlux v3.0"  (May 2001);
*   void ranlxd (double r[],int n);
*   void rlxd_init (int level,int seed);
*   int rlxd_size (void);
*   void rlxd_get (int state[]);
*   void rlxd_reset (int state[]);
void rlxd_init (int level,int seed);
void ranlxd (double r[],int n);
int rlxd_size (void);
void rlxd_get (int state[]);
void rlxd_reset (int state[]);
void rlxd_init (int level,int seed);
void ranlxd (double r[],int n);
int rlxd_size (void);
void rlxd_get (int state[]);
void rlxd_reset (int state[]);

#ifdef __cplusplus
}
#endif

#endif

