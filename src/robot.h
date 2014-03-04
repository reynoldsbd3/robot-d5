#ifndef ROBOT_H_
#define ROBOT_H_

#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHWONKA.h>

#include "logging.h"

// Struct describing the capacities of the robot
struct robot {
  
  FEHMotor *l_mot;       // Pointer to left motor
  FEHMotor *r_mot;       // Pointer to right motor
  FEHMotor *f_mot;       // Pointer to forklift motor
  FEHEncoder *l_enc;     // Pointer to left encoder
  FEHEncoder *r_enc;     // Pointer to right encoder
  AnalogInputPin *cds_0; // Pointer to CdS cell
  AnalogInputPin *opt_0; // Pointer to first optosensor
  AnalogInputPin *opt_1; // Pointer to second optosensor
  AnalogInputPin *opt_2; // Pointer to third optosensor
  FEHWONKA *rps;         // Pointer to WONKA and RPS system
  ButtonBoard *btns;     // Pointer to button board
  
  float head;              // Robot's current heading

  struct log_data *journal; // Location of logging information
};

#endif