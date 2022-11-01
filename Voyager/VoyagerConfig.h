#ifndef VoyagerConfig
#define VoyagerConfig

#define CAP_DEBOUNCE 75
/*
 * Arduino Pin Setup
 */
// Control Buttons
#define NAV_TOGGLE_BTN A0
#define HEADLIGHT_BTN A0
#define INTERIOR_BTN A0
#define DEFLECTOR_BTN A0
#define PORT_PHASER_BTN A0
#define STARBOARD_PHASER_BTN A0
#define FORWARD_PHOTON_BTN A0
#define AFT_PHOTON_BTN A0
#define PHOTON_TOGGLE_BTN A0
#define WARP_BTN A0
#define IMPULSE_BTN A0
#define PHASER_PORT_BTN A0
#define PHASER_STAR_BTN A0

// Ship Pins 
#define NAV_LIGHTS_PIN 3
#define NAV_BEACON_PIN 4
//Might want to be dimmable via pwm
#define HEADLIGHT_PIN 5
#define IMPLUSE_ENGINES_PIN 6
#define REAR_LOUNGE_PIN 13
#define INTERIOR_LIGHTS_PIN 9
#define DEFLECTOR_PIN 10

//RGB Data Pins
#define FWD_PHOTON 7
#define AFT_PHOTON 8
#define PHASER_PIN 11
#define WARP_ENGINES_PIN 12


//Adafruit Sound Board
#define SND_RX A2
#define SND_TX A3
#define SND_RST A1
#define SND_VOL 200

//Sound FX Indices
#define DEFLECTOR_ON_FX 5
#define DEFLECTOR_OFF_FX 6
#define INTERIOR_ON_FX 12
#define INTERIOR_OFF_FX 6
#define IMPULSE_ON_FX 11
#define IMPULSE_OFF_FX 6
#define HEADLIGHT_ON_FX 10
#define HEADLIGHT_OFF_FX 6
#define NAV_ON_FX 12
#define NAV_BLINK_FX 11
#define NAV_OFF_FX 6
#define WARP_ON_FX 0
#define WARP_ENGAGE_FX 4
#define WARP_FLASH_FX 3
#define WARP_OFF_FX 6
#define PHOTON_ON_FX 13
#define PHOTON_FIRE_FX 1
#define PHOTON_FIRE2_FX 2
#define PHOTON_OFF_FX 6
#define PHASER_ON_FX 16
#define PHASER_OFF_FX 2
#define ERROR_FX 9



/*
 * Navigation Settings
 */
#define BLINK_ON_TIME 1000
#define BLINK_OFF_TIME 2000

/*
 * Interior Settings
 */
 #define INTERIOR_BRIGHTNESS 150

/*
 * Phaser Settings
 */
#define PHASER_LED_CNT 46
#define PHASER_PORT_START 0
#define PHASER_STAR_START 23
#define PHASER_LEN 23
#define PHASER_MOVE_TIME 400
#define PHASER_BLAST_TIME 900
#define PHASER_FIRE_SIZE 1
#define PHASER_MOVE_COLOR 0xFA3C00
#define PHASER_FIRE_COLOR 0xFFFF11
#define PHASER_PORT 1
#define PHASER_STAR 2


/*
 * Photon Torpedoes Settings
 */
#define PHOTON_IDLE_COLOR 0x0F0000
#define PHOTON_FIRE_COLOR 0xFF0000
/*
 * Engine Settings
 */
#define WARP_LED_CNT 7
#define COLLECTOR_INDEX 6
#define ENGAGE_TIME  2200
#define ENGAGE_FLASH_TIME 200
#define IDLE_CHILLER_COLOR 0x000020
#define IDLE_COLLECTOR_COLOR 0x200000
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
#define PHOTON_IDLE_1 7
#define FIRING_1 8
#define PHOTON_IDLE_2 9
#define FIRING_2 10
#define PHASER_MOVING 11
#define PHASER_FIRE 12
#endif
