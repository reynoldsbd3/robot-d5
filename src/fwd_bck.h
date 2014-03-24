#ifndef FWD_BCK_H_
#define FWD_BCK_H_

/*
 *
 */
void bck_dist(struct robot*, float);

/*
 *
 */
void bck_flw(struct robot*, float);

/*
 *
 */
void bck_time(struct robot*, float);

/*
 *
 */
void bck_tst(struct robot*);

/*
 *
 */
void fwd_dist(struct robot*, float);

/*
 *
 */
void fwd_flw(struct robot*, float);

/*
 *
 */
void fwd_time(struct robot*, float);

/*
 *
 */
void fwd_tst(struct robot*);

#endif