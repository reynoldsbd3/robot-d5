#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "robot.h"

// Forward motion
void fwd_time(struct robot *bot, float time);
void fwd_dist(struct robot *bot, float distance);
void fwd_flw(struct robot *bot, float distance);
void fwd_tst(struct robot *bot);

// Backward motion
void bck_time(struct robot *bot, float time);
void bck_dist(struct robot *bot, float distance);
void bck_tst(struct robot *bot);

// Turning
void rot_time(struct robot *bot, float time, bool cw);
void rot_deg(struct robot *bot, int degree);
void rot_head(struct robot *bot, int head);
void rot_tst(struct robot *bot, bool cw);

// Forklift
void raise_fork(struct robot *bot, float time);
void lower_fork(struct robot *bot, float time);

// Misc
int head_diff(int head, int target);
void ud_head(struct robot *bot);

#endif
