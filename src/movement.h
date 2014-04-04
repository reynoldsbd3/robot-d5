#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "robot.h"

// Enumerations
enum line_state {

	ON_LINE,
	NEAR_EDGE,
	OFF_LINE
};

enum line_location {

	LINE_ON_LEFT,
	LINE_ON_RIGHT,
	UNKNOWN
};

// Forward motion
void fwd_time(struct robot *bot, float time);
void fwd_dist(struct robot *bot, float distance);
void fwd_flw(struct robot *bot, float distance);
void fwd_tst(struct robot *bot);

// Backward motion
void bck_time(struct robot *bot, float time);
void bck_dist(struct robot *bot, float distance);
void bck_tst(struct robot *bot);
void bck_flw(struct robot *bt, float distance);

// Turning
void rot_time(struct robot *bot, float time, bool cw);
void rot_deg(struct robot *bot, float degree);
void rot_corr(struct robot *bot, float degree);
void rot_head(struct robot *bot, float head);
void rot_tst(struct robot *bot, bool cw);
void crct_head(struct robot *bot, float head);

// Forklift
void raise_fork(struct robot *bot, float time);
void lower_fork(struct robot *bot, float time);

// Misc
float head_diff(float head, float target);
void ud_head(struct robot *bot);

#endif
