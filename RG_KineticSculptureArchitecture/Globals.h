/** @file Globals.h Contains all global variables used to control the Kinetic Sculpture*/

#include "Motor.h"
#include "Ultrasonic.h"
#include "Switch.h"
#include "LEDStrip.h"
#include "Disc.h"

#define RED 255, 0, 0 /**< RGB values to display red */
#define GREEN 0, 255, 0 /**< RGB values to display green */
#define BLUE 0, 0, 255 /**< RGB values to display blue */
#define YELLOW 255, 255, 0 /**< RGB values to display yellow */
#define ORANGE 255, 140, 0 /**< RGB values to display orange */
#define MAGENTA 255, 0, 255 /**< RGB values to diplay magenta */
#define BLACK 0, 0, 0 /**< RGB values to diplay black */
#define WHITE_FULL 255, 255, 255 /**< RGB values to diplay full white */
#define WHITE 255, 255, 135 /**< RGB values to diplay white */
#define WHITE_MEDIUM 128, 128, 67 /**< RGB values to diplay white medium */
#define WHITE_LOW 64, 64, 34 /**< RGB values to diplay white low */
#define WHITE_VERY_LOW 48, 48, 25 /**< RGB values to diplay white very low */
#define WHITE_VERY_VERY_LOW 32, 32, 17 /**< RGB values to diplay white very very low */

void setupGlobals();
void frontDiscISR();
void backDiscISR();

/**
 * Switch declaration
 */
const int BIT_1_SWITCH_PIN = 63; /**< Pin that bit1 switch is attached to */
Switch bit1Switch(BIT_1_SWITCH_PIN); /**< Switch object that represents bit1 @return instance of Switch*/

const int BIT_2_SWITCH_PIN = 64; /**< Pin that bit2 switch is attached to */
Switch bit2Switch(BIT_2_SWITCH_PIN); /**< Switch object that represents bit 2 @return instance of Switch*/

const int BIT_3_SWITCH_PIN = 65; /**< Pin that bit3 switch is attached to */
Switch bit3Switch(BIT_3_SWITCH_PIN); /**< Switch that represents bit 3 @return instance of Switch*/


/**
 * Motor initialization
 */
const int RIGHT_MOTOR_DIRECTION_PIN = 14; /**< Pin that controls the direction of the right motor */
const int RIGHT_MOTOR_PWM_PIN = 12; /**< Pin that sends PWM power to the right motor */
const int RIGHT_MOTOR_TACHOMETER = 18; /**< Pin that the tachometer for the right motor is attached to */
Motor rightMotor(RIGHT_MOTOR_DIRECTION_PIN, RIGHT_MOTOR_PWM_PIN,
                 RIGHT_MOTOR_TACHOMETER); /**< Motor that represents the right motor on the PCB (controls back disc) @return instance of Motor*/

const int LEFT_MOTOR_DIRECTION_PIN = 15; /**< Pin that controls the direction of the left motor */
const int LEFT_MOTOR_PWM_PIN = 11; /**< Pin that sends PWM power to the left motor */
const int LEFT_MOTOR_TACHOMETER = 19; /**< Pin that the tachometer for the left motor is attached to */
Motor leftMotor(LEFT_MOTOR_DIRECTION_PIN, LEFT_MOTOR_PWM_PIN,
                LEFT_MOTOR_TACHOMETER); /**< Motor that represents the left motor on the PCB (controls front disc) @return instance of Motor*/


/**
 * Disc declaration
 */
Disc backDisc(&rightMotor); /**< Back disc object to control the back disc */
Disc frontDisc(&leftMotor); /**< Front disc object to control the front disc */

/**
 * LEDStrip declaration
 */
const int BACKLIGHT_RED_PIN = 6; /**< Attached pin on Arduino that controls red on LEDStrip */
const int BACKLIGHT_GREEN_PIN = 8; /**< Attached pin on Arduino that controls green on LEDStrip */
const int BACKLIGHT_BLUE_PIN = 7; /**< Attached pin on Arduino that controls blue on LEDStrip */
const int LED_PIN = 13; /**< Pin that the red LED on the left corner of the PCB that is attached to */
LEDStrip ledBacklights(BACKLIGHT_RED_PIN, BACKLIGHT_GREEN_PIN,
                       BACKLIGHT_BLUE_PIN); /**< LEDStrip that controls the backlighting @return instance of LEDStrip*/

/**
 * Ultrasonic declaration
 */
const int US_TRIGGER = 4; /**< Trigger pin that the ultrasonic sensor is attached to */
const int US_ECHO = 3; /**< Echo pin that the ultrasonic sensor is attached to */
Ultrasonic ultrasonicSensor(US_TRIGGER,
                            US_ECHO); /**< Ultrasonic object to interact with the ultrasonic sensor @return instance of Ultrasonic*/

/**
 * Setup all of the global variables.
 * Begin Serial Communication.
 * Set the LED on the PCB to be an output.
 * Set all of the interrupt service routines.
 */
void setupGlobals() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(frontDisc.motor->getTachAttachedPin()), frontDiscISR, RISING);
    attachInterrupt(digitalPinToInterrupt(backDisc.motor->getTachAttachedPin()), backDiscISR, RISING);
}

/**
 * Front disc tachometer interrupt service routine. Forwards the call to Motor::interruptServiceRoutine
 */
void frontDiscISR() {
    frontDisc.motor->interruptServiceRoutine();
}

/**
 * Back disc tachometer interrupt service rountine. Forwards the call to Motor::interruptServiceRoutine
 */
void backDiscISR() {
    backDisc.motor->interruptServiceRoutine();
}

/**
 * Easter egg mode for the Kinetic Sculpture. Does it mean anything?
 */
void easterEgg(){
    for(int i = 0; i <= 2; i++){
        ledBacklights.setColor(random(0, 255), random(0, 255), random(0, 255));
        delay(150);
        ledBacklights.setColor(0,0,0);
        delay(100);
    }

    delay(100);

    for(int i = 0; i <=2; i ++){
        ledBacklights.setColor(random(0, 255), random(0, 255), random(0, 255));
        delay(250);
        ledBacklights.setColor(0,0,0);
        delay(100);
    }

    delay(100);

    for(int i = 0; i <= 2; i++){
        ledBacklights.setColor(random(0, 255), random(0, 255), random(0, 255));
        delay(150);
        ledBacklights.setColor(0,0,0);
        delay(100);
    }
}