# voyager-arduino
Arduino controller code for voyager scale model special effects. This setup is squeezing all the code onto an ardiuno uno board.  It is also using the Adafruit Sound FX board as well as Adafruit Capacitive touch board.

Current State includes<br>
<b>* Navigation Lights Control:</b> A button cycles thru the effect state either off, blinking, or steady on.  There are 2 pins with the navigation lights one for always on lights and the other for a blinking beacon light.<br>
<b>* Headlight Control:</b> A button to turn the registry headlight on or off<br>
<b>* Interior Control:</b> A button to turn on the interior lights throughout the ship. Might add an option to dim them if pwm pin available for it.<br>
<b>* Deflector Control:</b> A button to turn on or off the deflector dish. Might give it a slight breathing cycle if pwm pins are available.<br>
<b>* Impluse control:</b> A button to turn on or off the impulse engine. Might give it a slight breathing cycle if pwm pins are available.<br>
<b>* Proton torpedo contols:</b> One button to turn on the idle lights. Two other buttons to fire the front and rear torpedoes. Each time a fire button is pressed it will alternate between port and starboard launchers<br>
<b>* Warp Control:</b> A button that will first power up the engines to an idle color. Next press will do goto warp animation, which leaves the engines lights at full brightness. The third time pressed will turn them off.<br>
<b>* Phaser Control:</b> Buttons for port and starboard main phaser strips.  This will animate the blasting of them<br>

