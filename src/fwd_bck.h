#ifndef FWD_BCK_H_
#define FWD_BCK_H_

/* Moves robot backward for an abslute distance according to
 * shaft encoding hardware.
 *
 * PARAM bot : struct robot*
 *		Points to a robot struct describing the robot
 *
 * PARAM distance : float
 *		Distance that the robot will travel
 *
 * Notes:
 *		- Requires that bot's fields l_enc, l_mot, r_enc,
 *			r_mot, and rps point to initialized objects from 
 *			the FEH libraries.
 *		- Uses RPS heading data every 300 ms to maintain a
 *			straight path.
 *		- Uses the constants LT_LPI and RT_RPI to accurately
 *			gage distance travelled. These values must be
 *			calibrated to ensure reliable robot performance.
 */
void bck_dist(struct robot *bot, float distance);

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

/* Moves robot forward for an abslute distance according to
 * shaft encoding hardware.
 *
 * PARAM bot : struct robot*
 *		Points to a robot struct describing the robot
 *
 * PARAM distance : float
 *		Distance that the robot will travel
 *
 * Notes:
 *		- Requires that bot's fields l_enc, l_mot, r_enc,
 *			r_mot, and rps point to initialized objects from 
 *			the FEH libraries.
 *		- Uses RPS heading data every 300 ms to maintain a
 *			straight path.
 *		- Uses the constants LT_LPI and RT_RPI to accurately
 *			gage distance travelled. These values must be
 *			calibrated to ensure reliable robot performance.
 */
void fwd_dist(struct robot *bot, float distance);

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