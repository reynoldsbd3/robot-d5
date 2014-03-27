#include <FEHBattery.h>
#include <FEHIO.h>
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHUtility.h>
#include <FEHWONKA.h>

#include "logging.h"
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
    Sleep(250);
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

  test:

  // Wait for CdS cell
  LCD.WriteLine("Waiting for signal");
  while(cds_0.Value() > 0.5);
  LCD.WriteLine("Starting");

  // Short circuit to a specific test
  goto pin;

  pin:

  // Drop fork a bit
  f_mot.SetPower(30);
  Sleep(200);
  f_mot.SetPower(0);

  // Move to pin
  fwd_dist(&bot, 28.0);
  Sleep(200);
  rot_deg(&bot, 40.0);
  Sleep(200);
  fwd_dist(&bot, 4.0);
  Sleep(200);
  rot_deg(&bot, -43.0);
  Sleep(200);
  fwd_dist(&bot, 14.0);
  Sleep(200);

  // Grab pin
  f_mot.SetPower(-30);
  Sleep(200);
  bck_dist(&bot, 7.0);
  Sleep(200);
  f_mot.SetPower(0);

  // Drop off pin
  rot_deg(&bot, -45.0);
  Sleep(200);
  f_mot.SetPower(60);
  Sleep(350);
  f_mot.SetPower(0);
  Sleep(200);
  bck_dist(&bot, 4.0);
  Sleep(200);

  // Line up with skid
  f_mot.SetPower(-60);
  Sleep(800);
  f_mot.SetPower(0);
  Sleep(200);
  rot_deg(&bot, 45.0);
  Sleep(200);
  fwd_dist(&bot, 4.0);
  Sleep(200);
  rot_deg(&bot, 90.0);
  Sleep(200);

  while (!btns.MiddlePressed());

  skid:

  // Lower fork, move to skid
  f_mot.SetPower(60);
  Sleep(600);          // TODO adjust to lower prongs
  f_mot.SetPower(0);
  Sleep(200);
  fwd_dist(&bot, 12.0);
  Sleep(200);
  f_mot.SetPower(-60);
  Sleep(600);
  bck_dist(&bot, 5.0);
  Sleep(200);
  f_mot.SetPower(-130);
  Sleep(1500);
  f_mot.SetPower(-30);
  Sleep(200);

  // Back up, line up with switch
  rot_deg(&bot, 90.0);
  Sleep(200);

  while (!btns.MiddlePressed());

  switch_:

  // Flip switch
  fwd_dist(&bot, 8.0);
  Sleep(200);
  bck_dist(&bot, 8.0);
  Sleep(200);

  // Line up with button
  rot_deg(&bot, 45.0);
  Sleep(200);
  fwd_dist(&bot, 12.0);
  Sleep(200);
  rot_deg(&bot, -45.0);
  Sleep(200);
  fwd_dist(&bot, 10.0);
  Sleep(200);

  while (!btns.MiddlePressed());

  button:

  // Move to button
  fwd_dist(&bot, 10.0);
  Sleep(200);

  // Press button a certain number of times
  for (int i = 0; i < rps.Oven(); i++) {

    fwd_dist(&bot, 4.0);
    Sleep(200);
    bck_dist(&bot, 4.0);
    Sleep(200);
  }

  // Move to and line up with ramp
  bck_dist(&bot, 5.0);
  Sleep(200);
  rot_deg(&bot, -45.0);
  Sleep(200);
  fwd_dist(&bot, 10.0);
  Sleep(200);
  rot_deg(&bot, 45.0);
  Sleep(200);

  while (!btns.MiddlePressed());

  down_ramp:

  // Move down ramp, stop at light
  bck_dist(&bot, 23.0);
  Sleep(200);

  while (!btns.MiddlePressed());

  scoop:

  // Get value of shop light
  light_value = cds_0.Value();

  if (light_value < 0.3) {

    // Light is red; move to right
    rot_deg(&bot, -45.0);
    Sleep(200);
    bck_flw(&bot, 15.0);
    Sleep(200);
    fwd_dist(&bot, 4.0);
    Sleep(200);
    rot_deg(&bot, 90.0);
    Sleep(200);
    fwd_dist(&bot, 4.0);
    Sleep(200);
    rot_deg(&bot, -90.0);
    Sleep(200);

  } else {

    // Light is blue; move to left
    rot_deg(&bot, 45.0);
    Sleep(200);
    bck_flw(&bot, 15.0);
    fwd_dist(&bot, 4.0);
    Sleep(200);
    rot_deg(&bot, 90.0);
    Sleep(200);
    fwd_dist(&bot, 14.0);
    Sleep(200);
    rot_deg(&bot, -90.0);
    Sleep(200);
  }

  while (!btns.MiddlePressed());

  chiller:

  // Lower skid, insert into chiller
  f_mot.SetPower(60);
  Sleep(400);
  f_mot.SetPower(0);
  Sleep(200);
  fwd_dist(&bot, 8.0);
  Sleep(200);
  bck_dist(&bot, 8.0);
  Sleep(200);

  // Raise skid, line up with ramp
  f_mot.SetPower(80);
  Sleep(500);
  f_mot.SetPower(0);
  Sleep(200);
  rot_deg(&bot, -45.0);
  Sleep(200);
  fwd_dist(&bot, 3.0);
  Sleep(200);
  rot_deg(&bot, -45.0);
  Sleep(200);
  fwd_dist(&bot, 3.0);
  Sleep(200);
  rot_deg(&bot, 90.0);
  Sleep(200);

  while (!btns.MiddlePressed());

  up_ramp:

  // Move up ramp
  fwd_dist(&bot, 15.0);

  // Line up with charger
  rot_deg(&bot, 90.0);

  while (!btns.MiddlePressed());

  charger:

  // Move to and activate charger
  fwd_dist(&bot, 23.0);

  while (true) {

    if (btns.MiddlePressed()) {

      goto test;
    }

    if (btns.LeftPressed()) {

      break;
    }
  }

  while (!btns.MiddlePressed());

  end:

  // Program finished
  rps.Disable();
  LCD.Clear();
  LCD.WriteLine("Complete.");
  return 0;
}
