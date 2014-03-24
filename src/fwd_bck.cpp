#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>

#include "fwd_bck.h"
#include "robot.h"
#include "rotation.h"

/* Moves robot forward a specified distance.
 *
 * See documentation in header file.
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

/* Causes robot to follow a line for a specified distance.
 *
 * See documentation in header file.
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

/* Moves the robot forward a specified amount of time.
 *
 * See documentation in header file.
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
