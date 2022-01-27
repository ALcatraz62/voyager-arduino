#ifndef VoyagerConfig
#define VoyagerConfig

/*
 * Arduino Pin Setup
 */
// Control Pins
#define NAV_TOGGLE_BTN 13
#define HEADLIGHT_BTN 13
#define INTERIOR_BTN 13
#define DEFLECTOR_BTN 13
#define PORT_PHASER_BTN 13
#define STARBOARD_PHASER_BTN 13
#define FORWARD_PHOTON_BTN 13
#define AFT_PHOTON_BTN 13
#define WARP_BTN 13

// Ship Pins 
#define NAV_LIGHTS_PIN 4
#define NAV_BEACON_PIN 7
//Might want to be dimmable via pwm
#define HEADLIGHT_PIN 3
#define INTERIOR_LIGHTS_PIN 5
#define DEFLECTOR_PIN 6
#define IMPLUSE_ENGINES_PIN 9
//RGB Data Pins
#define PHASER_PIN 8
#define FWD_PHOTON 10
#define AFT_PHOTON 11
#define WARP_ENGINES_PIN 12

/*
 * Navigation Settings
 */
#define BLINK_ON_TIME 1000
#define BLINK_OFF_TIME 2000

/*
 * Phaser Settings
 */
#define PHASER_LED_CNT 50
#define PHASER_PORT_START 0
#define PHASER_STAR_START 25
#define PHASER_MOVE_TIME 1000
#define PHASER_FIRE_SIZE 2
#define PHASER_MOVE_COLOR 0xAABB11
#define PHASER_FIRE_COLOR 0xFF0000


/*
 * Photon Torpedoes Settings
 */
#define PHOTON_IDLE_COLOR 0xBB110A
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
#define PHOTON_IDLE 7
#define FIRING 8
#endif
