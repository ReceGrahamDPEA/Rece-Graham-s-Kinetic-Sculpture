/** @file Disc.h File that contains class information for Disc Class */

#pragma once
#include "Motor.h"

#define MAX_RPM 30 /**< The maximum speed in RPM the motor is able to run at */

/**
 * Disc Class
 *
 * Class that represents a disc (front or back disc) on the Kinetic Sculpture.
 * Written by Wade Varesio & Courtney McIntosh 05/13/2019
 **/
class Disc{
  public:
    Motor *motor; /**<The Disc's attached motor */

   /**
    *  Constructor for a Disc Object
    *
    *  Constructs a Disc Object which represents the attached motor (and tachometer). Disc basically abstracts Motor to make controlling the Kinetic Sculpture more intuitive.
    *
    *  @param attachedMotor - the motor object that runs the disc object
    */
    Disc(Motor* attachedMotor) {
      motor = attachedMotor;
      motor->setupMotor();
    }

    /**
     *  Updates power sent to motors
     *
     *  Checks if the current RPM of each motor is greater or less than the desired RPM.
     *  If it is less, the motor is sped up, and if it is greater, the motor slows down is slowed down.
     *  It is also ensured that the motor will spin in the right direction by sending a positive or negative percentage when needed.
     *
     *  @param desiredRpm the desired RPM at which the motor would spin. Max desiredRpm is 30RPM
     */
    void updateSpeed(float desiredRpm) {
        if(desiredRpm > 30){ //If the desired RPM is greater than max run at max
          desiredRpm = MAX_RPM;
        }else if(desiredRpm < -30){
          desiredRpm = -1 * MAX_RPM;
        }
        
        delay(100);
        float tachRpm = motor->getRotationsPerMinute();
        double motorPercentage = motor->getPercentage();
        float desiredDiscRpm = desiredRpm;
        long desiredTachRpm = desiredDiscRpm * 1250; //converts DiscRpm to Tach rpm
        long desireMin = desiredTachRpm - 1000; //Creates a lower bound on rpm
        long desireMax = desiredTachRpm + 1000; //Creates an upper bound on rpm

        while (tachRpm > desireMax or tachRpm < desireMin){ //Runs until Rpm is within 10% of desired value
          delay(100);
          tachRpm = motor->getRotationsPerMinute();
          motorPercentage = motor->getPercentage();
          byte dir = motor->getDirection();
          if (dir == 0){
            tachRpm = -tachRpm;
          }
          if (desiredTachRpm > tachRpm){
            motor->increasePercentage(double((desiredTachRpm-tachRpm)/10000));
          }
          else if(desiredTachRpm < tachRpm){
            motor->decreasePercentage(double((tachRpm-desiredTachRpm)/10000));
          }
        }
    }
    /**
     * Get the rotations per minute
     *
     * @returns returns an unsigned long (a non-negative number that can include a fractional part)
     **/
    unsigned long getRotationsPerMinute(){
      return motor->getRotationsPerMinute();
    }
    
    /**
     * Set the interrupt service routine for the associated tachometer
     **/
    void setISR(){
      motor->interruptServiceRoutine();
    }

    /**
     * Forwards the call to turnOnMotor() to the appropriate Motor object using pointers
     *
     * @see Motor::turnOnMotor()
     **/
    void turnOnMotor(){
      motor->turnOnMotor();
    }

    /**
     * Forwards the call to turnOffMotor() to the appropriate Motor object using pointers
     *
     * @see Motor::turnOffMotor()
     **/
    void turnOffMotor(){
      motor->turnOffMotor();
    }

    /**
     * Forwards the call to runMotor() to the appropriate Motor object using pointers
     *
     * @param percent the percent at which you want the motor to run at
     * @see Motor::runMotor()
     **/
    void runDisc(double percent){
      motor->runMotor(percent);
    }

    /**
     * Run the disc in a random fashion.
     * 
     * Determines a random speed (RPM) and direction to run the disc at
     **/
    void runDiscRandomly(){
      int randRpm = rand() % MAX_RPM;
      int randDir = pow(-1, rand() % 2); //Randomly generate -1 or 1 to determine the direction of rotation
      
      updateSpeed(randRpm * randDir);
    }

    /**
     * Set the minimum percent nedded to run this discs motor at
     * 
     * @param percent the minimum percent needed to run the motor at. Must be non negative and less than 100
     * or the request will be ignored
     * @return true if percent is valid and the min usbale percentage was set, false otherwise
     * @see Motor::setMinMotorPercentage()
     **/
    bool setMinMotorPercentage(byte percent){
      return motor->setMinimumUsablePercent(percent);
    }

    /**
     * Set the maximum amount of power the motor is allowed to run at
     * 
     * @param percent the new maximum amount of power the motor is allowed to run at. Must be greater than setMinimumUsablePercent
     * but less than or equal to 100.
     * @return true if percent is valid and the motors max usable percent was set, false otherwise
     * @see Motor::setMaxMotorPercentage()
     */
    bool setMaxMotorPercentage(byte percent){
      return motor->setMaxUsablePercent(percent);
    }

  private:
};
