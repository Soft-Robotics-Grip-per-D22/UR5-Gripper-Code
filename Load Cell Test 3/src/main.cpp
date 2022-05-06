 /*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.
 
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch
 
 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).
 
 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state
 This example code uses bogde's excellent library:"https://github.com/bogde/HX711"
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"
#include <SimpleKalmanFilter.h>

SimpleKalmanFilter pressureKalmanFilter1(1, 1, 1.5);
SimpleKalmanFilter pressureKalmanFilter2(1, 1, 1.5);

#define LOADCELL_DOUT_PIN_1  4
#define LOADCELL_SCK_PIN_1   5
#define LOADCELL_DOUT_PIN_2  2
#define LOADCELL_SCK_PIN_2   3

HX711 scale1;
HX711 scale2;

float calibration_factor1 = 694000; //-7050 worked for my 440lb max scale setup
float calibration_factor2 = 694000; //-7050 worked for my 440lb max scale setup

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  
  scale1.begin(LOADCELL_DOUT_PIN_1, LOADCELL_SCK_PIN_1);
  scale1.set_scale();
  scale1.tare(); //Reset the scale to 0

  scale2.begin(LOADCELL_DOUT_PIN_2, LOADCELL_SCK_PIN_2);
  scale2.set_scale();
  scale2.tare(); //Reset the scale to 0
  
  long zero_factor_1 = scale1.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor_1);

  long zero_factor_2 = scale2.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor_2);
}

void loop() {  
  scale1.set_scale(calibration_factor1); //Adjust to this calibration factor
  // Serial.print("1 Reading: ");
  // Serial.print(scale.get_units(), 1);
  float lbs1 = scale1.get_units();
  float estimated_lbs1 = pressureKalmanFilter1.updateEstimate(lbs1);
  float grams1 = lbs1*453.592;
  float estimated_grams1 = estimated_lbs1*453.592;
  // Serial.print(grams1);
  // Serial.print(" g"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  // Serial.print(estimated_grams1);
  // Serial.print(" g"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  // Serial.print(" calibration_factor: ");
  // Serial.print(calibration_factor1);
  // Serial.println();

  scale2.set_scale(calibration_factor2); //Adjust to this calibration factor
  // Serial.print("2 Reading: ");
  // Serial.print(scale.get_units(), 1);
  float lbs2 = scale2.get_units();
  float estimated_lbs2 = pressureKalmanFilter2.updateEstimate(lbs2);
  float grams2 = lbs2*453.592;
  float estimated_grams2 = estimated_lbs2*453.592;
  // Serial.print(grams2);
  // Serial.print(" g"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  // Serial.print(estimated_grams2);
  // Serial.print(" g"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
  // Serial.print(" calibration_factor: ");
  // Serial.print(calibration_factor2);

  Serial.print("g: ");
  float total_estimated_grams = estimated_grams1 + estimated_grams2;
  Serial.print(total_estimated_grams);
  Serial.println();

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'a'){
      scale1.set_scale();
      scale1.tare();
      scale2.set_scale();
      scale2.tare();
    }

  }

}
