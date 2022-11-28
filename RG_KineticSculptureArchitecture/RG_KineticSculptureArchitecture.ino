/** @file KineticSculptureArchitecture.ino */

#include "Globals.h" /**< All variables needed to control aspects of the Kinetic Sculpture */

/**
 * Setup the various components on the PCB
 *
 * Begins serial communication, set up LED on PCB and attaches ISR to tachometers
 */
void setup() {
  setupGlobals();
}

/**
 * Constantly check the state of all three switches. Run the corresponding mode.
 *
 * If switch is changed while running a mode the mode will run until completion, then change to new mode.
 */
void loop() {
    int state = 0;

    // 3 switches represent 3 bits in binary for a total of 8 possible combinations
    if(bit1Switch.getState()){
      state += 1; //Bit 1 is 001 in binary
    }
    if(bit2Switch.getState()){
      state += 2; //Bit 2 is 010 in binary
    }
    if(bit3Switch.getState()){
      state += 4; //Bit 3 is 100 in binary
    }
  
  switch (state) {
    case 0:
       noBitsActive(); // OOO in binary
       break;
    case 1:
       bit1Active(); // 001 in binary
       break;
    case 2:
       bit2Active(); // 010 in binary
       break;
    case 3:
       bit1AndBit2Active(); // 011 in binary
       break;
    case 4:
       bit3Active(); // 100 in binary
       break;
    case 5:
       bit1AndBit3Active(); // 101 in binary
       break;
    case 6:
       bit2AndBit3Active(); // 110 in binary
       break;
    case 7:
       allBitsActive(); // 111 in binary
       break;
  }
}

/**
 * No bits active (Goldilocks Mode)
 *
 * Goldilocks Mode takes as input the distance as measured by the ultrasonic sensor,
 * then sets the intensity of the LED Strip lights based on three gaussian distributions.
 * Red intensity peaks at minimum distance from the sensor, green peaks at 150 cm away,
 * and blue peaks at 300cm away.
 */
void noBitsActive(){
  backDisc.turnOffMotor();
  frontDisc.turnOffMotor();
  int dis = ultrasonicSensor.getDistanceInCM();
  int red = min(255, int(255*pow(2.7183, -0.0001*pow(dis,2))));
  int green = min(255, int(255*pow(2.7183, -0.0001*pow(dis-150,2))));
  int blue = min(255, int(255*pow(2.7183, -0.0001*pow(dis-300,2))));
  if (not (red == 0 and green == 0 and blue == 0)){
    ledBacklights.backlightStartTransition(red, green, blue, 300);
  }
}

/**
 * Bit 1 active (Distance-Based Oscillation Mode)
 *
 * Distance-Based Oscillation Mode cycles through set distribution of numbers.
 * The frequency of the cycle depends on the distance as measured by the sensor,
 * where the minimum transition time between colors is 300 milliseconds at 60 cm away
 * and the maximum transition time is 4 seconds past the farthest distance the ultrasonic
 * sensor can measure (~4 meters).
 */
void bit1Active(){
  int dis2 = ultrasonicSensor.getDistanceInCM() * 5;
  for (int x = 0; x < 25; x++){
    ledBacklights.backlightStartTransition(int(125.5*(sin(x*3.14/12)+1)), int(125.5*(cos(x*3.14/12)+1)), int(125.5*(sin(-x*3.14/12)+1)), min(max(int(dis2), 300), 4000));
  }
}

/**
 * Bit 2 active (Distance-Based Spin Mode)
 *
 * Distance-Based Spin Mode sets the rotational speed and direction of the wheels based
 * on the distance as measured by the sensor. After setting the wheels, it then runs through
 * a variety of colors.
 */
void bit2Active(){
  int dis3 = ultrasonicSensor.getDistanceInCM();
  double frontSpeed = 30 * sin(dis3/15);
  double backSpeed = 30 * cos(dis3/3);
  frontDisc.updateSpeed(frontSpeed);
  Serial.print("Front: ");
  Serial.print(frontSpeed);
  backDisc.updateSpeed(backSpeed);
  Serial.print("  Back: ");
  Serial.println(backSpeed);
  for (int x = 0; x < 90; x++){
    ledBacklights.backlightStartTransition(int(80 * sin(x*3.14/15) + 80), int(80*sin(x*3.14/20) + 80), int(80 * cos(x*3.14/10) + 80), 1000);
  }
}

/**
 * Bit 3 Active (Random Colors Mode)
 *
 * Random Colors Mode sets the backlight to random colors.
 */
void bit3Active(){

  backDisc.turnOnMotor();
  frontDisc.turnOnMotor();

  frontDisc.updateSpeed(-3);
  backDisc.updateSpeed(2);

  for (int x = 0; x < 5; x++)
  {

    delay(500);

      if(random(0, 250) < 220){ //Sunrise With No Clouds
        
        frontDisc.updateSpeed(-20);
      ledBacklights.backlightStartTransition(4, 4, 45, 7000); //Midnight/Sunrise Start/Maastricht Blue
      delay(100);
         backDisc.updateSpeed(15);
      ledBacklights.backlightStartTransition(62, 24, 20, 5000);//Phase 1
      ledBacklights.backlightStartTransition(255, 129, 20, 3000); //Very Orange color
      ledBacklights.backlightStartTransition(150, 129, 200, 3000);
      ledBacklights.backlightStartTransition(132, 210, 239, 3000);//Phase 2/Baby Blue
      ledBacklights.backlightStartTransition(132, 210, 239, 1000);//Baby Blue
      delay(2000);
      ledBacklights.backlightStartTransition(140, 210, 230, 1000);
      delay(2000);
      ledBacklights.backlightStartTransition(145, 210, 220, 3000);
    }

    else { //Sunrise With Clouds
      ledBacklights.backlightStartTransition(4, 4, 45, 3000); //Midnight/Sunrise Start/Maastricht Blue
      ledBacklights.backlightStartTransition(10, 24, 60, 2500); //Phase 1
      ledBacklights.backlightStartTransition(150, 60, 68, 2500);
      backDisc.updateSpeed(-7);
      frontDisc.updateSpeed(2);
      ledBacklights.backlightStartTransition(175, 140, 149, 2000);
      ledBacklights.backlightStartTransition(200, 200, 200, 10000);
      delay(5000);
    
    }

    backDisc.updateSpeed(15);
    frontDisc.updateSpeed(20);
    ledBacklights.backlightStartTransition(255, 200, 115, 3000);
    ledBacklights.backlightStartTransition(255, 156, 84, 3000);
    ledBacklights.backlightStartTransition(253, 94, 83, 3000);
    ledBacklights.backlightStartTransition(75, 61, 96, 3000); 
    ledBacklights.backlightStartTransition(21, 40, 82, 1500);
    ledBacklights.backlightStartTransition(8, 24, 58, 1500);
    ledBacklights.backlightStartTransition(4, 4, 45, 3000); //Midnight/Sunrise Start/Maastricht Blue
    delay(1000);

    //ALSO THIS IS NOT IN A GITHUB REPO, THAT ALSO NEEDS TO HAPPEN.

  }

  backDisc.turnOffMotor();
  frontDisc.turnOffMotor();

  delay(3000);
  
}


/**
 * Bits 1 and 2 Active (Primary Spinner Mode)
 *
 * Primary Spinner Mode sets the front wheel to 20rpm and the back wheel to 30 rpm,
 * while transitioning between red, green, and blue.
 */
void bit1AndBit2Active(){
  ledBacklights.backlightStartTransition(255, 10, 10, 1000);
  backDisc.updateSpeed(30);
  ledBacklights.backlightStartTransition(10, 255, 10, 1000);
  frontDisc.updateSpeed(20);
  ledBacklights.backlightStartTransition(10, 10, 255, 1000);
}

/**
 * Bits 1 amd 3 Active
 *
 * Random Roller Mode sets the wheel speeds and the colors to random values.
 * The absolute value of the wheel speed should always be greater than 5RPM.
 */
void bit1AndBit3Active(){
  frontDisc.updateSpeed(random(5, 30) * pow(-1, random(0,3)-1));
  delay(1750);
  ledBacklights.backlightStartTransition(random(0,255), random(0,255), random(0,255), 1500);
  backDisc.updateSpeed(random(5, 30) * pow(-1, random(0,3)-1));
  delay(1750);
  ledBacklights.backlightStartTransition(random(0,255), random(0,255), random(0,255), 1500);
}

/**
 * Bits 2 and 3 Active (Distance Roller Mode)
 *
 * Distance Roller Mode sets the backlights to full white,
 * then sets the wheel speeds based on distance. The front and back wheels
 * always move in opposite directions.
 */
void bit2AndBit3Active(){
  int dis = ultrasonicSensor.getDistanceInCM();
  if (dis > 500){
    dis = 220;
  }
  Serial.println(dis);
  ledBacklights.backlightStartTransition(255, 255, 255, 15);
  frontDisc.updateSpeed(10*sin(dis/10) + 20);
  backDisc.updateSpeed(10*cos(dis/5) - 20);
  delay(2000);
}

/**
 * All Bits Active (Sleepy Lights Mode)
 *
 * Sleepy Lights Mode flashes to full brightness, then slowly drops into a random color
 * before flashing back to full white.
 */
void allBitsActive(){
  frontDisc.turnOffMotor();
  backDisc.turnOffMotor();
  ledBacklights.backlightStartTransition(255, 255, 255, 300);
  int color1 = random(0, 255), color2 = random(0, 255), color3 = random(0, 255);
  ledBacklights.backlightStartTransition(color1, color2, color3, 3000);
}
