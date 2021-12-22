#ifndef VoyagerConfig
#define VoyagerConfig

/*
 * Arduino Pin Setup
 */
// Control Pins
#define NAV_TOGGLE_BTN 2
#define HEADLIGHT_BTN 2
#define INTERIOR_BTN 2
#define DEFLECTOR_BTN 2
#define PORT_PHASER_BTN 4
#define STARBOARD_PHASER_BTN 5
#define FORWARD_PHOTON_BTN 6
#define AFT_PHOTON_BTN 7
#define ENGINES_TOGGLE_PIN 8

// Ship Pins 
#define NAV_LIGHTS_PIN 3
#define NAV_BEACON_PIN 13
#define HEADLIGHT_PIN 5
#define PORT_PHASER_PIN 6
#define STARBOARD_PHASER_PIN 7
#define INTERIOR_LIGHTS_PIN 8
#define DEFLECTOR_PIN 9
#define FWD_PHOTON_PORT 10
#define FWD_PHOTON_STARBOARD 11
#define AFT_PHOTON_PORT 10
#define AFT_PHOTON_STARBOARD 12

/*
 * Navigation Settings
 */
#define BLINK_ON_TIME 1000
#define BLINK_OFF_TIME 2000

/*
 * Phaser Settings
 */


/*
 * Photon Torpedoes Settings
 */

/*
 * Engine Settings
 */


/* STATE FLAGS*/
#define OFF 0
#define ON 1
#define BLINK_ON 2
#define BLINK_OFF 3
#endif
