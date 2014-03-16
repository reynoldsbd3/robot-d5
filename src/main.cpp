
#include <FEHBattery.h>
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
FEHMotor l_mot(FEHMotor::Motor2);
FEHMotor r_mot(FEHMotor::Motor0);
FEHMotor f_mot(FEHMotor::Motor3);

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

// Declare a battery
FEHBattery batt(FEHIO::BATTERY_VOLTAGE);

void turn_left();

int main() {

  // Variable declaration
  struct robot bot;
  struct log_data *journal;

  // Display battery info
  while (!btns.MiddlePressed()) {

    LCD.Clear();
    LCD.Write("Battery voltage: ");
    LCD.WriteLine(batt.Voltage());
    Sleep(250);

  }

  Sleep(250);

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
  bot.batt = &batt;
  bot.journal = journal;
  bot.head = rps.Heading();

  //Wait for CdS cell
  LCD.Clear();
  LCD.WriteLine("Waiting for signal light");
  while (cds_0.Value() > 0.5);

  // Move towards skid
  fwd_dist(&bot, 36.0);
  // while(!btns.MiddlePressed());

  // Rotate to face skid
  turn_left();
  // rot_head(&bot, 177);
  // while(!btns.MiddlePressed());

  // Lower fork
  f_mot.SetPower(30);
  Sleep(900);
  f_mot.SetPower(0);
  // while(!btns.MiddlePressed());

  // Move towards skid
  fwd_dist(&bot, 10);
  // while(!btns.MiddlePressed());

  // Raise skid, wait to raise if needed 
  f_mot.SetPower(-125);
  Sleep(400);
  bck_dist(&bot, 2.5);
  Sleep(5000);
  f_mot.SetPower(-30);
  // while(!btns.MiddlePressed());

  // Maneuver to ramp
  bck_dist(&bot, 5.0);
  rot_head(&bot, 135.0);
  bck_dist(&bot, 10.0);
  rot_head(&bot, 175);
  // while(!btns.MiddlePressed());

  // Go down ramp
  bck_dist(&bot, 26.0);
  // while(!btns.MiddlePressed());

  // Line up with chiller
  rot_head(&bot, 265);
  fwd_dist(&bot, 9.0);
  turn_left();
  // while (!btns.MiddlePressed());

  // Lower skid
  f_mot.SetPower(30);
  Sleep(200);
  f_mot.SetPower(0);

  // Insert skid
  fwd_dist(&bot, 6.0);
  bck_dist(&bot, 6.0);

  // Program finished
  rps.Disable();
  LCD.Clear();
  LCD.WriteLine("Complete.");
  LCD.WriteLine("Ready to dump...");
  LCD.WriteLine("Press middle button");
  dump(&bot);
  return 0;
}

void turn_left() {

  l_mot.SetPower(90);
  r_mot.SetPower(-90);
  Sleep(2000);
  l_mot.SetPower(0);
  r_mot.SetPower(0);
}
