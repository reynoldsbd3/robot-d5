#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>

#include "logging.h"
#include "movement.h"
#include "robot.h"

#define BLNC_FCTR -7            // Offset factor for balamcing motor power
#define HEAD_ERR 2.0           // Maximum allowable heading error
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
#define BCK_FTRY_LINE 1.0
#define BCK_FTRY_NO_LINE 2.8
#define BCK_SHOP_LINE 2.8
#define BCK_SHOP_NO_LINE 1.0
#define LM_PWR_FW -90          // Left motor forward power
#define LM_PWR_FLW -60
#define LM_PWR_LADJ 70
#define LM_PWR_LR 70          // Left motor left rotation power
#define LM_PWR_LT 30           // Left motor left turn power
#define LM_PWR_RADJ -70
#define LM_PWR_RR -70          // Left motor right rotation power
#define LM_PWR_RT 70           // Left motor right turn power
#define LT_LPI 2.51            // Left tread links per inch
#define RM_PWR_FW -90          // Right motor forward power
#define RM_PWR_FLW -60
#define RM_PWR_LADJ -70
#define RM_PWR_LR -70         // Right motor left rotation power
#define RM_PWR_LT 70           // Right motor left turn power
#define RM_PWR_RADJ 70
#define RM_PWR_RR 70           // Right motor right rotation power
#define RM_PWR_RT 30           // Right motor right turn power
#define RT_LPI 2.00            // Right tread links per inch

// Prototypes
float head_diff(float, float);
int l_tgt_cts(float);
int r_tgt_cts(float);
void rot_deg(struct robot, int);
void rot_head(struct robot, int);
void ud_head(struct robot);

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
  (*bot->r_mot).SetPower(RM_PWR_FW + 8);
  
  // Wait for the specified amount of time
  Sleep(time);
  
  // Cease forward motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);

  // Log journal entry
  // bot->journal = log(bot->journal, &entry);
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
  float orig_head;
  float current_head;
  float start_time = TimeNow();

  // Get original heading
  // Ignore RPS failures
  while ((orig_head = bot->rps->Heading()) == 0.0);
  
  // Ensure a clean count to begin
  bot->l_enc->ResetCounts();
  bot->r_enc->ResetCounts();
  
  // Move forward until robot reaches desired distance
  while (bot->l_enc->Counts() < l_tgt &&
    bot->r_enc->Counts() < r_tgt) {
    
    // Move forward
    bot->l_mot->SetPower(l_pwr);
    bot->r_mot->SetPower(r_pwr);

    // Allow time for RPS values to change before re-mesuring
    if (TimeNow() - start_time >= 0.5) {

      // Get current heading
      // Ignore RPS failures
      while ((current_head = bot->rps->Heading()) == 0.0);
    
      // Check if one motor is outpacing another
      if (head_diff(orig_head, current_head) <
        -1 * HEAD_ERR) {
        
        // If left is faster than right, attempt to correct this
        // by applying a balancing factor
        l_pwr -= BLNC_FCTR;
        r_pwr += BLNC_FCTR;
        LCD.WriteLine("correcting to left");
        
      } else if (head_diff(orig_head, current_head) >
        HEAD_ERR) {
        
        // If right is faster than left, attempt to correct this
        // by applying a balancing factor
        l_pwr += BLNC_FCTR;
        r_pwr -= BLNC_FCTR;
        LCD.WriteLine("correction to right");
      }

      // Start counting time again
      start_time = TimeNow();
    }
  }
  
  // Cease forward motion
  bot->l_mot->SetPower(0);
  bot->r_mot->SetPower(0);
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
  float orig_head;

  // Get original heading
  ud_head(bot);
  orig_head = bot->head;
  
  // Ensure a clean count to begin
  (*bot->l_enc).ResetCounts();
  (*bot->r_enc).ResetCounts();
  
  // Move backward until robot reaches desired distance
  while ((*bot->l_enc).Counts() < l_tgt &&
    (*bot->r_enc).Counts() < r_tgt) {
    
    // Move forward
    (*bot->l_mot).SetPower(l_pwr);
    (*bot->r_mot).SetPower(r_pwr);

    // TODO algorithm is too greedy
    // Increase this time period to use with RPS
    Sleep(50);
    ud_head(bot);
    
    // // Check if one motor is outpacing another
    if ((*bot->l_enc).Counts() / l_tgt >
      (*bot->r_enc).Counts() / r_tgt) {
      
      // If left is faster than right, attempt to correct this
      // by applying a balancing factor
      l_pwr -= BLNC_FCTR;
      r_pwr += BLNC_FCTR;
      
    } else if ((*bot->l_enc).Counts() / l_tgt <
      (*bot->r_enc).Counts() / r_tgt) {
      
      // If right is faster than left, attempt to correct this
      // by applying a balancing factor
      l_pwr += BLNC_FCTR;
      r_pwr -= BLNC_FCTR;
    }
  }
  
  // Cease backward motion
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);
}

void bck_flw(struct robot *bot, float distance) {

  // Variable declarations
  enum line_state current_state;
  int i;
  bool in_factory = bot->rps->Y() > FTRY_THRESH;
  int l_tgt = l_tgt_cts(distance);
  enum line_location location = UNKNOWN;
  enum line_state previous_state;
  int r_tgt = r_tgt_cts(distance);

  // Keep following line until distance is reached
  while (bot->l_enc->Counts() < l_tgt &&
    bot->r_enc->Counts() < r_tgt) {

    // Determine current line state
    // Threshold comparisons vary depending on the location of
    // the robot
    if (in_factory) {

      // In factory area, lines are less reflective than
      // the floor
      if (bot->opt_1->Value() < BCK_FTRY_LINE) {

        // Robot is over line
        current_state = ON_LINE;

      } else if (bot->opt_1->Value() > BCK_FTRY_NO_LINE) {

        // Robot is not over line
        current_state = OFF_LINE;

      } else {

        // Robot is near line edge
        current_state = NEAR_EDGE;
      }
    } else {

      // In shop area, lines are more reflective than
      // the floor
      if (bot->opt_1->Value() > BCK_SHOP_LINE) {

        // Robot is over line
        current_state = ON_LINE;

      } else if (bot->opt_1->Value() < BCK_SHOP_NO_LINE) {

        // Robot is not over line
        current_state = OFF_LINE;

      } else {

        // Robot is near line edge
        current_state = NEAR_EDGE;
      }
    }

    // Adjust motors based on current state
    switch (current_state) {

      case ON_LINE:

        // Turn back towards the edge
        switch (location) {

          case LINE_ON_LEFT:

            // Robot has moved too far leftward
            bot->l_mot->SetPower(-1 * LM_PWR_RADJ);
            bot->r_mot->SetPower(-1 * RM_PWR_RADJ);
            break;

          case LINE_ON_RIGHT:

            // Robot has moved too far rightward
            bot->l_mot->SetPower(-1 * LM_PWR_LADJ);
            bot->r_mot->SetPower(-1 * RM_PWR_LADJ);
            break;

          case UNKNOWN:

            // Default to putting the line on the left
            bot->l_mot->SetPower(-1 * LM_PWR_RADJ);
            bot->r_mot->SetPower(-1 * RM_PWR_RADJ);
            location = LINE_ON_LEFT;
            break;
        }

        break;

      case NEAR_EDGE:

        // Robot is where it needs to be
        bot->l_mot->SetPower(-1 * LM_PWR_FLW);
        bot->r_mot->SetPower(-1 * RM_PWR_FLW);
        break;

      case OFF_LINE:

        switch (location) {

          case LINE_ON_LEFT:

            // Robot has moved too far rightward
            bot->l_mot->SetPower(-1 * LM_PWR_LADJ);
            bot->r_mot->SetPower(-1 * RM_PWR_LADJ);
            break;

          case LINE_ON_RIGHT:

            // Robot has moved too far leftward
            bot->l_mot->SetPower(-1 * LM_PWR_RADJ);
            bot->r_mot->SetPower(-1 * RM_PWR_RADJ);
            break;

          case UNKNOWN:

            // Find the line
            // Hopefully, it is close

            // Search to the left
            bot->l_mot->SetPower(-1 * LM_PWR_LADJ);
            bot->r_mot->SetPower(-1 * RM_PWR_LADJ);
            for (i = 0; i < 100; i++) {

              // Give time to search
              Sleep(10);

              // Stop search if line is found
              if (in_factory && bot->opt_1->Value() < BCK_FTRY_LINE)
                break;
              if (!in_factory && bot->opt_1->Value() > BCK_SHOP_LINE)
                break;
            }

            // If search was successful, mark line's location
            if (i != 100) {

              location = LINE_ON_LEFT;

            } else {

              // Otherwise, continue the search
              // Search to the right
              bot->l_mot->SetPower(-1 * LM_PWR_RADJ);
              bot->r_mot->SetPower(-1 * RM_PWR_RADJ);
              for (i = 0; i < 100; i++) {

                // Give time to search
                Sleep(10);

                // Stop search if line is found
                if (in_factory && bot->opt_1->Value() < BCK_FTRY_LINE)
                  break;
                if (!in_factory && bot->opt_1->Value() > BCK_SHOP_LINE)
                  break;
              }

              // If search was successful, mark line's location
              if (i != 100) {

                location = LINE_ON_RIGHT;

              } else {

                // Otherwise, line is totally lost
                return;
              }

            break;
        }

        break;
      }
    }

    // Give algorithm time to work
    Sleep(100);

    // Remeber state
    previous_state = current_state;
  }
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

  // Variable declarations
  float current_head;
  float prev_head;
  float target;
  bool wrapped = false;

  // Get current heading
  // Ignore RPS failures
  while ((current_head = bot->rps->Heading()) == 0.0);

  // Calculate target
  target = current_head + degree;

  // Correct target for wrap
  if (target < 0.0) {

    target = 180.0 - target;
    wrapped = true;

  } else if (target >= 179.9) {

    target -= 179.9;
    wrapped = true;
  }

  if (degree > 0) {

    (*bot->l_mot).SetPower(LM_PWR_LR);
    (*bot->r_mot).SetPower(RM_PWR_LR);

    // Wrap before checking heading
    if (wrapped) {
      
      do {

        if (bot->rps->Heading() != 0.0) {

          prev_head = current_head;
          Sleep(10);
          current_head = bot->rps->Heading();
        }

      } while (prev_head <= current_head);
    }

    // Continue rotating until heading is achieved
    while ((*bot->rps).Heading() < target);

  } else if (degree < 0) {

    (*bot->l_mot).SetPower(LM_PWR_RR);
    (*bot->r_mot).SetPower(RM_PWR_RR);

    // Wrap before checking heading
    if (wrapped) {
      
      do {

        if (bot->rps->Heading() != 0.0) {

          prev_head = current_head;
          Sleep(10);
          current_head = bot->rps->Heading();
         }

      } while (prev_head >=  current_head);
    }

    // Continue rotating until heading is achieved
    while ((*bot->rps).Heading() < target);
  }

  // Cease rotation
  (*bot->l_mot).SetPower(0);
  (*bot->r_mot).SetPower(0);
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

    // Less greedy; time to work
    Sleep(100);

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

void crct_head(struct robot *bot, float head) {

  // Variable declarations
  float diff = head_diff(head, bot->rps->Heading());

  while (diff > 1.0 || diff < -1.0) {

    if (diff < 0.0) {

      bot->l_mot->SetPower(LM_PWR_RR);
      bot->r_mot->SetPower(RM_PWR_RR);

    } else if (diff > 0.0) {

      bot->l_mot->SetPower(LM_PWR_LR);
      bot->r_mot->SetPower(RM_PWR_LR);
    }

    diff = head_diff(head, bot->rps->Heading());
  }

  bot->l_mot->SetPower(0);
  bot->r_mot->SetPower(0);
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
float head_diff(float current, float target) {

  // Variable 
  float difference = target - current;

  // Reverse direction if need be
  if (difference > 89.9) {

    difference = 179.9 - difference;
  }
  if (difference < -89.9) {

    difference += 179.9;
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
  struct log_data entry;

  // Set up journal entry
  // entry.fname = "ud_head";
  // entry.msg = "Updated heading to this value";

  // Take cases
  if (old_head > 45.0 && old_head <= 135.0) {

    // New heading is in 1st or 4th quadrant
    new_head = raw_head;

  } else if (old_head > 135.0 && old_head <= 225.0) {

    // New heading is in 1st or 2nd quadrant
    if (raw_head >= 90.0) {

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
    if (raw_head >= 90.0) {

      // 3rd quadrant
      new_head = 180.0 + raw_head;

    } else {

      // 4th quadrant
      new_head = raw_head;
    }
  }

  // Set new heading
  bot->head = new_head;
  // entry.value = new_head;

  // Override if RPS fails
  if ((*bot->rps).Heading() == 0) {

    // Just assume head hasn't changed
    bot->head = old_head;
  }

  // Log journal entry
  // bot->journal = log(bot->journal, &entry);
}
// TODO delete this ^^^

void flw_light(struct robot *bot) {

  if (bot->cds_0->Value() < 0.2) {

    rot_deg(bot, -45);
  }
}
