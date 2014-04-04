#include <FEHBattery.h>
#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>

#include "movement.h"
#include "robot.h"

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

// Declare button board
ButtonBoard btns(FEHIO::Bank3);

// Declare WONKA interface
FEHWONKA rps;

// Declare battery
FEHBattery batt(FEHIO::BATTERY_VOLTAGE);

int main() {

  // Variable declaration
  struct robot bot;
  float light_value;

  // Display battery info
  while (!btns.MiddlePressed()) {

    LCD.Clear();
    LCD.Write("Battery voltage: ");
    LCD.WriteLine(batt.Voltage());
    Sleep(500);
  }

  Sleep(250);

  // Initialize RPS and WONKA communication
  rps.InitializeMenu();
  rps.Enable();
  LCD.Clear();
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
  bot.rps = &rps;
  bot.btns = &btns;
  bot.batt = &batt; 

  // Wait for CdS cell
  LCD.WriteLine("Waiting for signal");
  while(cds_0.Value() > 0.5);
  LCD.WriteLine("Starting");

  // Drop fork a bit
  f_mot.SetPower(30);
  Sleep(200);
  f_mot.SetPower(0);

  // Move to pin
  fwd_dist(&bot, 28.0);
  Sleep(200);
  rot_corr(&bot, (90.0 - rps.Heading()) / 2);
  Sleep(200);
  rot_deg(&bot, 40.0);
  Sleep(200);
  fwd_dist(&bot, 4.5);
  Sleep(200);
  rot_deg(&bot, -40.0);
  Sleep(200);
  rot_corr(&bot, (90.0 - rps.Heading()) / 2);
  Sleep(200);
  rot_corr(&bot, 3.0);
  Sleep(200);
  fwd_dist(&bot, 14.0);
  Sleep(200);

  // Grab pin
  f_mot.SetPower(-30);
  Sleep(200);
  bck_dist(&bot, 10.0);
  Sleep(200);
  f_mot.SetPower(0);
  Sleep(200);

  // Line up with ramp
  rot_deg(&bot, 45.0);
  Sleep(200);
  rot_deg(&bot, 45.0);
  Sleep(200);

  // Do a heading correction to get heading close to 0
  // This correction is different dpending on whether the robot
  // has or has not already passed the 0 degree mark
  if (rps.Heading() < 90) {

    // In this case, move downward close to 0
    rot_corr(&bot, (0.5 - rps.Heading()) / 2);

  } else if (rps.Heading() > 90) {

    // In this case, move a litte further towards 0
    rot_corr(&bot, (179.4 - rps.Heading()) / 2);
  }
  Sleep(200);

  // Move to shop
  bck_dist(&bot, 22.0);
  Sleep(200);

  // Do a heading correction to get heading close to 0
  // This correction is different dpending on whether the robot
  // has or has not already passed the 0 degree mark
  if (rps.Heading() < 90) {

    // In this case, move downward close to 0
    rot_corr(&bot, (0.5 - rps.Heading()) / 2);

  } else if (rps.Heading() > 90) {

    // In this case, move a litte further towards 0
    rot_corr(&bot, (179.4 - rps.Heading()) / 2);
  }
  Sleep(200);

  // Line up with left counter
  rot_deg(&bot, 45.0);
  Sleep(200);
  bck_dist(&bot, 9.0);
  Sleep(200);
  rot_deg(&bot, -45.0);
  Sleep(200);

  // Do a heading correction to get heading close to 0
  // This correction is different dpending on whether the robot
  // has or has not already passed the 0 degree mark
  if (rps.Heading() < 90) {

    // In this case, move downward close to 0
    rot_corr(&bot, (0.5 - rps.Heading()) / 2);

  } else if (rps.Heading() > 90) {

    // In this case, move a litte further towards 0
    rot_corr(&bot, (179.4 - rps.Heading()) / 2);
  }
  Sleep(200);

  // Deposit scoop
  bck_dist(&bot, 9.5);
  Sleep(200);

  // Program finished
  rps.Disable();
  LCD.Clear();
  LCD.WriteLine("Complete.");
  return 0;
}
