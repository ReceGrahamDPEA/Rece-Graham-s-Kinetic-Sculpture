/** @file Motor.h File containing class information for Motor objects*/

#pragma once

/**
 * Class to interact with each motor on the PCB
 * The motor class also includes tachometer support which is used to get the RPM that the motor is running at
 *
 * Written by Wade Varesio and Courtney McIntosh 05/13/2019
 **/
class Motor{
  public:
    const byte FULL_OFF = 255; /**< The signal is inverted therefore FULL_OFF is 255*/
    const byte FULL_ON =  0; /**< The signal is inverted therefore FULL_ON is 0*/
    double percentage = 0; /**< The percentage of power that the motor is running at */

     /**
     * Constructor to create a Motor object
     *
     * @param direction - pin number that controls the direction of the motor
     * @param pwm - pin number that the motor PWM pin is on. (Hint: Pulse Width Modulation AKA PWM is how you dim LEDs and control the motor)
     * @param tachPin - pin number that the tachometer is attached to on the Arduino
     */
     Motor (byte direction, byte pwm, byte tachPin) {
        directionPin = direction;
        pwmPin = pwm;
        tachometerPin = tachPin;

        minUsablePercent = 40;
        maxUsablePercent = 100;
        /*
         * Change the prescaler for timer 1 (used by the motor PWM) to increase the PWM frequency
         * to 31250Hz so that it can not be heard  (1=31250Hz, 2=3906Hz, 3=488Hz, 4=122Hz, 5=30.5Hz)
         */
        TCCR1B = (TCCR1B & B11111000) | 1;
        setupMotor();
      }

    /**
     * Sets up Motor to spin.
     *
     * Sets pinNumber and directionPin to be an OUTPUT, tachometer pin to be an input.
     */
    void setupMotor() {
      pinMode(directionPin, OUTPUT);
      pinMode(pwmPin, OUTPUT);
      analogWrite(pwmPin, FULL_OFF); //Write off to ensure motor is initialized to be off

      pinMode(tachometerPin, INPUT_PULLUP);
    }

    /**
     * Runs a motor at a given speed in percent
     *
     * Makes the disc spin clockwise if percent is positive, and counterclockwise if it is negative.
     * Calls the turnOnMotor function at a given percent.
     *
     * @param percent -  the percent at which you want to run the motor.
     */
     void runMotor(double percent){
      if(percent < 0){
        digitalWrite(directionPin, LOW);
      }
      else{
        digitalWrite(directionPin, HIGH);
      }

      analogWrite(pwmPin, turnPercentIntoPwm(percent));
      percentage = percent;
    }

    /**
     * Takes a percentage in the form of a double and turns it into a PWM signal for motor control.
     *
     * A percentage value of 0 is off (returns a PWM value of 255), and 100 is fully on (returns a PWM value of 0).
     *
     * @param percent - the percentage value to be converted to a PWM value.
     * @return returns the conversion of the percent in a PWM value
     **/
   double turnPercentIntoPwm(double percent){
    // Maps the input percentage (0-100%) into a usable percentage for the motor based off minUsablePercent and maxUsablePercent
    if (percent > 0) {
      percent = map(percent, 0, 100, minUsablePercent, maxUsablePercent);
    } else if (percent < 0) {
      percent = map(percent, 0, -100, minUsablePercent, maxUsablePercent);
    }
    
    return 255 - (255 * abs(percent) / 100);
   }

    /**
     * Turn the motor completely off
     **/
    void turnOffMotor() {
      analogWrite(pwmPin, FULL_OFF);
    }

    /**
     * Turn the motor completely on
     **/
    void turnOnMotor() {
      analogWrite(pwmPin, FULL_ON);
    }

    /**
     * Increases percentage the motor is running at
     *
     * Calls the runMotor function at this new percentage.
     *
     * @param step -  the amount you want the percentage to increase by.
     */
    void increasePercentage(double step){
      if(percentage < 100){
        percentage += step;
        runMotor(percentage);
      }
    }

    /**
     * Decreases percentage
     *
     * Subtracts .25 from the value of percentage.
     * Calls the runMotor function at this new percentage.
     *
     * @param step -  the amount you want the percentage to increase by.
     */
    void decreasePercentage(double step){
      if(percentage > -100){
        percentage -= step;
        runMotor(percentage);
      }
    }
    /**
     * Interrupt service routine for the Tachometer
     *
     * ISR for when the pin falls low, this is necessary to get a measurement and convert into the RPM of the motor.
     **/
    void interruptServiceRoutine() {
      newTime = micros();
      microsecondsBetweenLines = newTime - timeOfLastMeasurement;
      timeOfLastMeasurement = newTime;
      rotationsPerMinute = 24000000 / microsecondsBetweenLines;
    }

    /**
     * @return the motors current rotations per minute
     */
    unsigned long getRotationsPerMinute(){
      return rotationsPerMinute;
    }

    /**
     * @return the attached Tachometer pin
     */
    byte getTachAttachedPin(){
      return tachometerPin;
    }

    /**
     * Get the motors current percentage
     *
     * @return percentage the motor is running at
     **/
    double getPercentage(){
      return percentage;
    }
    
    /**
     * Get the direction that the motor is spinning in
     * 
     * @return the direction that the motor is spinning in 1 clockwise, 0 counterclockwise
     **/
    double getDirection(){
      return digitalRead(directionPin); 
    }

    /**
     * Set the minimum percentage required to run a motor.
     * This should ONLY be called if your motor requires a higher minimum than the normal 40%.
     * 
     * @param percent new minimum motor percentage. Must be less than 100 and non negative or the request will be ignored.
     * @return true if percent is valid and the min usable percentage was set, false otherwise
     **/
    bool setMinimumUsablePercent(byte percent){
        if((percent < 0) || (percent > 100)){ //Ignore this request
          return false;
        }

        minUsablePercent = percent;
        return true;
    }

    /**
     * Get the minimum usable percent the motor is able to run at
     * 
     * @return the current minimum percentage of power required to run a motor
     */
    byte getMinUsablePercent(){
      return minUsablePercent;
    }

    /**
     * Get the maximum usable percent of power the motor is able to run at
     * 
     * @return Max percent of power the motor is able to run at
     */
    byte getMaxUsablePercent(){
      return maxUsablePercent;
    }

    /**
     * Set the maximum usable percentage the motor will run at.
     * This should only be called when a motor requires a special maximum power limit
     * 
     * @param percent the new maximum percentage a motor is allowed to run at. Percent must be greater than minUsablePercent (defaults to 40)
     * and less or equal to 100.
     * @return true if percent is valid and the motors max usable percent was set, false otherwise
     */
    bool setMaxUsablePercent(byte percent){
      if((percent < minUsablePercent)){ // If the percent is less than minUsablePercent return
        return false;
      }else if((percent < 0) || (percent > 100)){ // If the percent is negative or greater than 100
        return false;
      }

      maxUsablePercent = percent;
      return true;
    }

  private:
    byte minUsablePercent; /**< The minimum percent a motor can operate smoothly at, defaults to 40 */
    byte  maxUsablePercent; /**< The maximum percent a motor can operate smoothly at, defaults to 100 */
    byte pwmPin, directionPin, tachometerPin;
    unsigned long rotationsPerMinute, newTime, timeOfLastMeasurement, microsecondsBetweenLines;
};
