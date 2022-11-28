/** @file Switch.h */

#pragma once

/**
 * Class to interface with the switches on the PCB board.
 * These switches are user toggled to define which mode the Kinetic Sculpture will be in.
 *
 * Written by Wade Varesio 9/21/2018
 **/
class Switch {
  public:

    /**
     * Constructor for declaring a switch on the PCB.
     *
     * @param pin pin that the switch is attached to.
     */
    Switch(int pin) {
      switchPin = pin;
      setupSwitch();
    }

    /**
     * Sets up the switch
     *
     * Sets the pin mode on the switch to be INPUT_PULLUP which allows for reading of the switch without a dedicated resistor
     **/
    void setupSwitch() {
      pinMode(switchPin, INPUT_PULLUP);
    }

    /**
     * Call to get the state of the switch which is either HIGH (switch toggled up towards top of PCB) or vice versa
     *
     * @return digitalRead on the switch pin
     */
    byte getState() {
      return digitalRead(switchPin);
    }

  private:
    int switchPin; /**< Pin number the switch is attached to on the Arduino */
};
