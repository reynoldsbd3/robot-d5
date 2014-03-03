#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>

#include "logging.h"
#include "movement.h"
#include "robot.h"

#define RE_H_THRESH 3.0
#define RE_L_THRESH 0.5
#define LE_H_THRESH 3.0
#define LE_L_THRESH 0.5

// Declare motors
FEHMotor l_mot(FEHMotor::Motor1);
FEHMotor r_mot(FEHMotor::Motor0);
FEHMotor f_mot(FEHMotor::Motor2);

// Declare sensors
FEHEncoder l_enc(FEHIO::P0_0);
FEHEncoder r_enc(FEHIO::P0_1);
AnalogInputPin cds_0(FEHIO::P1_0);
AnalogInputPin opt_0(FEHIO::P2_0);
AnalogInputPin opt_1(FEHIO::P2_1);
AnalogInputPin opt_2(FEHIO::P2_2);

// Button board
ButtonBoard btns(FEHIO::Bank3);

// Declare WONKA interface
FEHWONKA rps;

int main() {

  // Variable declaration
  struct robot bot;
  int i;
  struct log_data *journal;

  // Initialize logging data
  journal = init_log();

  // Initialize RPS and WONKA communication
  rps.InitializeMenu();
  rps.Enable();

  // Allow time to initialize
  Sleep(250);
  
  // Pack the robot struct
  bot.l_mot = &l_mot;
  bot.r_mot = &r_mot;
  bot.f_mot = &f_mot;
  bot.l_enc = &l_enc;
  bot.r_enc = &r_enc;
  bot.cds_0 = &cds_0;
  bot.opt_0 = &opt_0;
  bot.opt_1 = &opt_1;
  bot.opt_2 = &opt_2;
  bot.rps = &rps;
  bot.btns = &btns;
  bot.journal = journal;
  bot.head = 90;

  //Wait for CdS cell
  LCD.Clear();
  LCD.WriteLine("Waiting for signal light");
  while (cds_0.Value() > 0.5);
  
  // Program finished
  rps.Disable();
  LCD.WriteLine("Complete.");
  return 0;
}
