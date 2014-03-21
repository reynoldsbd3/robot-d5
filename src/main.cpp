
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
AnalogInputPin opt_2(FEHIO::P2_2);

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
  bot.opt_2 = &opt_2;
  bot.rps = &rps;
  bot.btns = &btns;
  bot.batt = &batt;

  test:

  // Wait for CdS cell
  LCD.WriteLine("Waiting for signal");
  while(cds_0.Value() > 0.5);
  LCD.WriteLine("Starting");

  // Move to the light
  LCD.WriteLine("Moving towards shop");
  fwd_dist(&bot, 23);
  Sleep(250);
  rot_deg(&bot, 81);
  Sleep(250);
  bck_dist(&bot, 20);
  Sleep(250);

  // Detect light color
  // Make sure a valid value is valid
  LCD.WriteLine("Getting light value");
  light_value = cds_0.Value();
  LCD.WriteLine("Got light value");
  Sleep(250);

  // Display light color
  if (light_value < 0.3) {

    LCD.Clear(FEHLCD::Red);
    LCD.SetBackgroundColor(FEHLCD::Red);

  } else if (light_value < 2.4) {

    LCD.Clear(FEHLCD::Blue);
    LCD.SetBackgroundColor(FEHLCD::Blue);

  } else {

    LCD.Clear(FEHLCD::Green);
    LCD.SetBackgroundColor(FEHLCD::Green);
  }
  Sleep(250);

  // Navigate to a counter
  rot_deg(&bot, -60);
  Sleep(250);
  bck_dist(&bot, 8.0);
  Sleep(250);
  rot_deg(&bot, 60.0);
  Sleep(250);
  bck_dist(&bot, 10.0);
  Sleep(250);

  // bck_dist(&bot, 10);
  // fwd_dist(&bot, 15);
  // rot_deg(&bot, 88);
  // fwd_dist(&bot, 20);


  for (;;) {
    if (btns.MiddlePressed()) {
      goto test;
    }
    if (btns.LeftPressed()) {
      goto end;
    }
  }

  end:

  // Program finished
  rps.Disable();
  LCD.Clear();
  LCD.WriteLine("Complete.");
  return 0;
}
