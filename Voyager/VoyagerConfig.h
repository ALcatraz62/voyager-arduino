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
#define WARP_BTN 7

// Ship Pins 
#define NAV_LIGHTS_PIN 3
#define NAV_BEACON_PIN 13
#define HEADLIGHT_PIN 5
#define PORT_PHASER_PIN 6
#define STARBOARD_PHASER_PIN 7
#define INTERIOR_LIGHTS_PIN 10
#define DEFLECTOR_PIN 9
#define FWD_PHOTON_PORT 10
#define FWD_PHOTON_STARBOARD 11
#define AFT_PHOTON_PORT 10
#define AFT_PHOTON_STARBOARD 12
#define WARP_ENGINES_PIN 8

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
#define WARP_LED_CNT 7
#define COLLECTOR_INDEX 6
#define ENGAGE_TIME 3000
#define ENGAGE_FLASH_TIME 200
#define IDLE_CHILLER_COLOR 0x000030
#define IDLE_COLLECTOR_COLOR 0x300000
#define ON_CHILLER_COLOR 0x0011FF
#define ON_COLLECTOR_COLOR 0xFF0000

/* STATE FLAGS*/
#define OFF 0
#define ON 1
#define BLINK_ON 2
#define BLINK_OFF 3
#define WARP_IDLE 4
#define WARP_ENGAGE 5
#define WARP_FLASH 6
#endif
