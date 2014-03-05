#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>

#include "logging.h"
#include "movement.h"
#include "robot.h"

#define BLNC_FCTR 3            // Offset factor for balamcing motor power
#define HEAD_ERR 3.0           // Maximum allowable heading error
#define FL_PWR -90             // Forklift power
#define FTRY_THRESH 20         // Y-coordinate where shop ends
#define LINE_THRESH_FTRY_0 1.0 // Optosensor threshold for detecting line in factory
#define NONE_THRESH_FTRY_0 2.8 // Optosensor threshold for detecting emptiness in factory
#define LINE_THRESH_SHOP_0 2.8 // Optosensor threshold for detecting line in shop
#define NONE_THRESH_SHOP_0 1.0 // Optosensor threshold for detecting emptiness in shop
#define LINE_THRESH_FTRY_1 1.0 // Optosensor threshold for detecting line in factory
#define NONE_THRESH_FTRY_1 2.8 // Optosensor threshold for detecting emptiness in factory
#define LINE_THRESH_SHOP_1 2.8 // Optosensor threshold for detecting line in shop
#define NONE_THRESH_SHOP_1 1.0 // Optosensor threshold for detecting emptiness in shop
#define LM_PWR_FW -98          // Left motor forward power
#define LM_PWR_LR -90          // Left motor left rotation power
#define LM_PWR_LT 30           // Left motor left turn power
#define LM_PWR_RR 90           // Left motor right rotation power
#define LM_PWR_RT 70           // Left motor right turn power
#define LT_LPI 2.44            // Left tread links per inch
#define RM_PWR_FW -90          // Right motor forward power
#define RM_PWR_LR 95           // Right motor left rotation power
#define RM_PWR_LT 70           // Right motor left turn power
#define RM_PWR_RR -90          // Right motor right rotation power
#define RM_PWR_RT 30           // Right motor right turn power
#define RT_LPI 2.05            // Right tread links per inch

// Prototypes
int head_diff(int, int);
int l_tgt_cts(float);
int r_tgt_cts(float);
void rot_deg(struct robot, int);
void rot_head(struct robot, int);
void ud_head(struct robot);

// Enumerations
enum line_state {

  ON_LINE,
  NEAR_EDGE,
  OFF_LINE
};

// Forward motion --------------------------------------------------------------

/* Moves the robot forward for a specified amount of time in seconds.
 * 
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 *
 * PARAM time : float
 *    Amount of time for the robot to move forward
 *
 * RETURN void
 *    This method returns nothing
 */
void fwd_time(struct robot *bot, float time) {

  // Variable declaration
  struct log_data entry;

  // Initialize log entry
  entry.fname = "fwd_time";
  entry.msg = "Moved forward for this many seconds";
  entry.value = time;
  
  // Begin forward motion
  (*bot->l_mot).SetPower(LM_PWR_FW);
  (*bot->r_mot).SetPower(RM_PWR_FW);
  
  // Wait for the specified amount of time
  Sleep(time);
  
  // Cease forward motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);

  // Log journal entry
  bot->journal = log(bot->journal, &entry);
}

/* Moves the robot forward a specific distance in inches.
 * Uses digital encoders to more accurately measure distance
 * 
 * PARAM bot : struct robot*
 * 	  Points to struct with all the robot's peripherals
 * 
 * PARAM distance : float
 * 	  Distance in inches for which to move
 *
 * RETURN void
 *    This method returns nothing
 */
void fwd_dist(struct robot *bot, float distance) {
  
  // Declare variables
  int l_tgt = l_tgt_cts(distance);
  int r_tgt = r_tgt_cts(distance);
  int l_pwr = LM_PWR_FW;
  int r_pwr = RM_PWR_FW;
  int cnt_diff;
  float orig_head;
  bool adjustment_made = false;
  struct log_data entry_0;
  struct log_data entry_1;
  struct log_data entry_2;
  struct log_data entry_3;

  // Initialize journal entries
  entry_0.fname = "fwd_dist";
  entry_0.msg = "Moved forward for this many inches";
  entry_0.value = distance;
  entry_1.fname = "fwd_dist";
  entry_1.msg = "Left target was this";
  entry_1.value = (float)l_tgt;
  entry_2.fname = "fwd_dist";
  entry_2.msg = "Right target was this";
  entry_2.value = (float)r_tgt;
  entry_3.fname = "fwd_dist";
  entry_3.msg = "Adjustments were made this many times";
  entry_3.value = 0.0;

  // Get original heading
  ud_head(bot);
  orig_head = bot->head;
  
  // Ensure a clean count to begin
  (*bot->l_enc).ResetCounts();
  (*bot->r_enc).ResetCounts();
  
  // Move forward until robot reaches desired distance
  while ((*bot->l_enc).Counts() < l_tgt &&
    (*bot->r_enc).Counts() < r_tgt) {
    
    // Move forward
    (*bot->l_mot).SetPower(l_pwr);
    (*bot->r_mot).SetPower(r_pwr);

    Sleep(50);
    ud_head(bot);
    
    // // Check if one motor is outpacing another
    if (head_diff(bot->head, orig_head) > 0) {
      
      // If left is faster than right, attempt to correct this
      // by applying a balancing factor
      l_pwr -= BLNC_FCTR;
      r_pwr += BLNC_FCTR;
      adjustment_made = true;
      
    } else if (head_diff(bot->head, orig_head) < 0) {
      
      // If right is faster than left, attempt to correct this
      // by applying a balancing factor
      l_pwr += BLNC_FCTR;
      r_pwr -= BLNC_FCTR;
      adjustment_made = true;
    }

    if (adjustment_made) {

      entry_3.value += 1;
      adjustment_made = false;
    }
  }
  
  // Cease forward motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);

  // Log journal entries
  bot->journal = log(bot->journal, &entry_0);
  bot->journal = log(bot->journal, &entry_1);
  bot->journal = log(bot->journal, &entry_2);
  bot->journal = log(bot->journal, &entry_3);
}

/* Causes the robot to "follow" a black line for an approximate distance in inches.
 * Uses digital encoders to more accurately measure distance.
 * Uses analog optosensors to observe the line being followed.
 * 
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 * 
 * PARAM distance : float
 * 	  Approximate distacne (in inches) for which to follow
 *
 * RETURN void
 *    This method returns nothing
 */
void fwd_flw(struct robot *bot, float distance) {

  // Variable declarations
  enum line_state current_state = NEAR_EDGE;
  enum line_state last_state;

  // Ensure a clean count to begin
  (*bot->l_enc).ResetCounts();
  (*bot->r_enc).ResetCounts();

  while (((*bot->l_enc).Counts() < l_tgt_cts(distance)) &&
    ((*bot->r_enc).Counts() < r_tgt_cts(distance))) {

    // Respond to current state
    switch (current_state) {

      case ON_LINE: // Rotate right to find edge
        (*bot->l_mot).SetPower(LM_PWR_RR);
        (*bot->r_mot).SetPower(RM_PWR_RR);
        break;

      case NEAR_EDGE: // Continue straight
        (*bot->l_mot).SetPower(LM_PWR_FW);
        (*bot->r_mot).SetPower(RM_PWR_FW);
        break;

      case OFF_LINE: // Rotate left to find edge
        (*bot->l_mot).SetPower(LM_PWR_LR);
        (*bot->r_mot).SetPower(RM_PWR_LR);
        break;
    }

    // Store current state
    last_state = current_state;

    // Branch depending on location of the robot
    if ((*bot->rps).Y() > FTRY_THRESH) { // Robot is in factory

      if ((*bot->opt_0).Value() < LINE_THRESH_FTRY_0) { 

        // Robot is over line
        current_state = ON_LINE;

      } else if ((*bot->opt_0).Value() > NONE_THRESH_FTRY_0) {

        // Robot is over nothing
        current_state = OFF_LINE;

      } else {

        // Robot is near edge
        current_state = NEAR_EDGE;
      }

    } else { // Robot is in store

      if ((*bot->opt_0).Value() > LINE_THRESH_SHOP_0) {

        // Robot is over line
        current_state = ON_LINE;

      } else if ((*bot->opt_0).Value() < NONE_THRESH_SHOP_0) {

        // Robot is over nothing
        current_state = OFF_LINE;

      } else {

        // Robot is near edge
        current_state = NEAR_EDGE;
      }
    }
  }

  // Cease motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);
}

/* Moves the robot forward indefinitely.
 * Stops when middle button on button board is pressed.
 *
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 *
 * RETURN void
 *    This method returns nothing
 */
void fwd_tst(struct robot *bot) {

  // Variable declarations
  double start_time = TimeNow();
  double end_time;
  struct log_data entry;

  // Initialize log entry
  entry.fname = "fwd_test";
  entry.msg = "Moved forward for this many seconds";

  // Begin forward motion
  (*bot->l_mot).SetPower(LM_PWR_FW);
  (*bot->r_mot).SetPower(RM_PWR_FW);

  // Wait until the middle button is pressed
  while (!(*bot->btns).MiddlePressed());

  // Mark the time
  end_time = TimeNow();

  // Cease forward motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);

  // Display elapsed time
  LCD.Write("Time elapsed: ");
  LCD.WriteLine(end_time - start_time);

  // Update log entry
  entry.value = end_time - start_time;

  // Give time to remove finger from button
  Sleep(250);

  // Log journal entry
  bot->journal = log(bot->journal, &entry);
}

// Backward motion -------------------------------------------------------------

/* Moves the robot backward for a specified amount of time in seconds.
 * 
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 *
 * PARAM time : float
 *    Amount of time for the robot to move backward
 *
 * RETURN void
 *    This method returns nothing
 */
void bck_time(struct robot *bot, float time) {
  
  // Begin backward motion
  (*bot->l_mot).SetPower(-1 * LM_PWR_FW);
  (*bot->r_mot).SetPower(-1 * RM_PWR_FW);
  
  // Wait for the specified amount of time
  Sleep(time);
  
  // Cease forward motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);
}

/* Moves the robot backward a specific distance in inches.
 * Uses digital encoders to more accurately measure distance
 * 
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 * 
 * PARAM distance : float
 *    Distance in inches for which to move
 *
 * RETURN void
 *    This method returns nothing
 */
void bck_dist(struct robot *bot, float distance) {
  
  // Declare variables
  int l_tgt = l_tgt_cts(distance);
  int r_tgt = r_tgt_cts(distance);
  int l_pwr = -1 * LM_PWR_FW;
  int r_pwr = -1 * RM_PWR_FW;
  int cnt_diff;
  
  // Ensure a clean count to begin
  (*bot->l_enc).ResetCounts();
  (*bot->r_enc).ResetCounts();
  
  // Move backward until robot reaches desired distance
  while ((*bot->l_enc).Counts() < l_tgt && (*bot->r_enc).Counts() < r_tgt) {
    
    // Move backward
    (*bot->l_mot).SetPower(l_pwr);
    (*bot->r_mot).SetPower(r_pwr);
    
    // Check if one motor is outpacing another
    if ((*bot->l_enc).Counts() / l_tgt > 
      (*bot->r_enc).Counts() / r_tgt) {
      
      // If left is faster than right, attempt to correct this
      // by applying a balancing factor
      l_pwr += BLNC_FCTR;
      r_pwr -= BLNC_FCTR;
      
    } else if ((*bot->l_enc).Counts() / l_tgt < 
      (*bot->r_enc).Counts() / r_tgt) {
      
      // If right is faster than left, attempt to correct this
      // by applying a balancing factor
      l_pwr -= BLNC_FCTR;
      r_pwr += BLNC_FCTR;
    }

    Sleep(50);
  }
  
  // Cease back motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);
}

/* Moves the robot backward indefinitely.
 * Stops when middle button on button board is pressed.
 *
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 *
 * RETURN void
 *    This method returns nothing
 */
void bck_tst(struct robot *bot) {

  // Variable declarations
  double start_time = TimeNow();
  double end_time;

  // Begin backward motion
  (*bot->l_mot).SetPower(-1 * LM_PWR_FW);
  (*bot->r_mot).SetPower(-1 * RM_PWR_FW);

  // Wait until the middle button is pressed
  while (!(*bot->btns).MiddlePressed());

  // Mark the time
  end_time = TimeNow();

  // Cease backward motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);

  // Display elapsed time
  LCD.Write("Time elapsed: ");
  LCD.WriteLine(end_time - start_time);

  // Give time to remove finger from button
  Sleep(200);
}

// Turning ---------------------------------------------------------------------

/* Rotates the robot for a specified amount of time in seconds.
 *
 * Direction of rotation is specified throught the cw parameter.
 *
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 *
 * PARAM time : float
 *    Amount of time fot the robot to rotate
 *
 * PARAM cw : bool
 *    Whether the robot will turn clockwise. If false, the
 *    robot will turn counterclockwise
 *
 * RETURN void
 *    This method returns nothing
 */
void rot_time(struct robot *bot, float time, bool cw) {

  // Begin rotation
  if (cw) { // Rotate clockwise

    (*bot->l_mot).SetPower(LM_PWR_RR);
    (*bot->r_mot).SetPower(RM_PWR_RR);

  } else { // Rotate counterclockwise

    (*bot->l_mot).SetPower(LM_PWR_LR);
    (*bot->r_mot).SetPower(RM_PWR_LR);
  }

  // Wait for the specified amount of time
  Sleep(time);

  // Cease rotation
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);
}

/* Rotates the robot a specified number of degrees.
 *
 * Uses RPS to ensure accuracy.
 *
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 *
 * PARAM degree : int
 *    Amount of degrees to turn. If positive, rotation is
 *    counterclockwise; if negative rotation is cloclwise
 *
 * RETURN void
 *    This method returns nothing
 */
void rot_deg(struct robot *bot, float degree) {

  // Variable declaration
  float target;

  // Get an up to date reading
  ud_head(bot);

  // Determine a target heading
  target = bot->head + degree;

  // Ensure target is valid
  if (target >= 360.0) {

    target -= 360.0;

  } else if (target < 0) {

    target += 360.0;
  }

  // Rotate to the target heading
  rot_head(bot, target);
  ud_head(bot);
}

/* Rotates the robot such that it has a certain heading according
 * to the RPS sensor.
 *
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 *
 * PARAM heading : int
 *    Heading for robot to have after function call
 *
 * RETURN void
 *    This method returns nothing
 */
void rot_head(struct robot *bot, float head) {

  // Variable declarations
  float diff;

  // Get an up to date reading
  ud_head(bot);

  // Get difference to target
  diff  = head_diff(bot->head, head);

  // While the current heading is too far from target
  while (diff > HEAD_ERR || diff < -1 * HEAD_ERR) {

    // Move in the appropriate direction
    if (diff > HEAD_ERR) {

      // Counterclockwise
      (*bot->l_mot).SetPower(LM_PWR_LR);
      (*bot->r_mot).SetPower(RM_PWR_LR);

    } else if (diff < -1 * HEAD_ERR){

      // Clockwise
      (*bot->l_mot).SetPower(LM_PWR_RR);
      (*bot->r_mot).SetPower(RM_PWR_RR);
    }

    // Update heading and difference to target
    ud_head(bot);
    diff = head_diff(bot->head, head);
  }

  // Stop rotation
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);
}

/* Rotates robot indefinitely.
 *
 * Direction of rotation is specified through the cw parameter.
 *
 * PARAM bot : struct robot*
 *    Points to struct with all the robot's peripherals
 *
 * PARAM cw : bool
 *    Whether the robot will turn clockwise. If false, the
 *    robot will turn counterclockwise
 *
 * RETURN void
 *    This method returns nothing
 */
void rot_tst(struct robot *bot, bool cw) {

  // Variable declarations
  double start_time = TimeNow();
  double end_time;

  // Begin rotation
  if (cw) { // Rotate clockwise

    (*bot->l_mot).SetPower(LM_PWR_RR);
    (*bot->r_mot).SetPower(RM_PWR_RR);

  } else { // Rotate counterclockwise

    (*bot->l_mot).SetPower(LM_PWR_LR);
    (*bot->r_mot).SetPower(RM_PWR_LR);
  }

  // Wait until the middle button is pressed
  while (!(*bot->btns).MiddlePressed());

  // Mark the time
  end_time = TimeNow();

  // Cease rotation
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);

  // Display elapsed time
  LCD.Write("Time elapsed: ");
  LCD.WriteLine(end_time - start_time);

  // Give time to remove finger from button
  Sleep(200);
}

// Forklift --------------------------------------------------------------------

/* Raises the forklift for a specific amount of time.
 *
 * PARAM time : float
 *    Amount of time for which to raise the fork
 *
 * RETURN void
 *    This method returns nothing
 */
void raise_fork(struct robot *bot, float time) {

  // Begin raising fork
  (*bot->f_mot).SetPower(FL_PWR);

  // Wait the specified amount of time
  Sleep(time);

  // Cease raising fork
  (*bot->f_mot).SetPower(0);
}

/* Lowers the forklift for a specific amount of time.
 *
 * PARAM time : float
 *    Amount of time for which to lower fork
 *
 * RETURN void
 *    This method returns nothing
 */
void lower_fork(struct robot *bot, float time) {

  // Begin lowering fork
  (*bot->f_mot).SetPower(-1 * FL_PWR);

  // Wait the specified amount of time
  Sleep(time);

  // Cease lowering fork
  (*bot->f_mot).SetPower(0);
}

// Misc. and Utility -----------------------------------------------------------

/* Calculates the number of encoder transitions that should elapse
 * given a specific distance.
 * 
 * Specifically tuned for the left encoder.
 *
 * PARAM distance : float
 *    Distance for which encoder counts will be projected
 *
 * RETURN int
 *    Number of encoder counts expected for distance
 */
int l_tgt_cts(float distance) {

  return distance * LT_LPI;
}

/* Calculates the number of encoder transitions that should elapse
 * given a specific distance.
 * 
 * Specifically tuned for the right encoder.
 *
 * PARAM distance : float
 *    Distance for which encoder counts will be projected
 *
 * RETURN int
 *    Number of encoder counts expected for distance
 */
int r_tgt_cts(float distance) {
  
  return distance * RT_LPI;
}

/* Calculates the difference between current and target headings.
 *
 * PARAM head : int
 *    Robot's current heading
 *
 * PARAM target : int
 *    Target heading for the robot
 *
 * RETURN int
 *    Number of degrees seperating the robot from the
 *    desired heading
 */
float head_diff(float head, float target) {

  // Variable 
  float difference = target - head;

  // Reverse direction if need be
  if (difference > 179.9) {

    difference -= 360.0;
  }
  if (difference < -179.9) {

    difference += 360.0;
  }

  return difference;
}

/* Updates stored heading based on RPS feedback.
 *
 * Remains accurate and accounts for looping RPS value if
 * called before robot has moved more than HEAD_ERR degrees from
 * its previous heading.
 *
 * PARAM bot : struct robot*
 *    Points to struct with robot's peripherals
 *
 * RETURN void
 *    This method returns nothing
 */
void ud_head(struct robot *bot) {

  // Variable declarations
  float old_head = bot->head;
  float raw_head = (*bot->rps).Heading();
  float new_head;

  // Take cases
  if (old_head > 45.0 && old_head <= 135.0) {

    // New heading is in 1st or 4th quadrant
    new_head = raw_head;

  } else if (old_head > 135.0 && old_head <= 225.0) {

    // New heading is in 1st or 2nd quadrant
    if (new_head >= 90.0) {

      // 1st quadrant
      new_head = raw_head;

    } else {

      // 2nd quadrant
      new_head = 180.0 + raw_head;
    }
  } else if (old_head > 225.0 && old_head <= 315.0) {

    // New heading is in 2nd or 3rd quadrant
    new_head = 180.0 + raw_head;

  } else {

    // New heading is in 3rd or 4th quadrant
    if (new_head >= 90.0) {

      // 3rd quadrant
      new_head = 180.0 + raw_head;

    } else {

      // 4th quadrant
      new_head = raw_head;
    }
  }

  // Set new heading
  bot->head = new_head;
}