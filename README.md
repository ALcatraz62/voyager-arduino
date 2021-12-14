# voyager-arduino
Arduino controller code for voyager scale model special effects.
Still very much a work in progress. The goal is to have the controller handle button inputs and controlling various pins to the lighting outputs.

Current State includes<br>
<b>Navigation Lights Control:</b> A button cycles thru the effect state either off, blinking, or steady on.  There are 2 pins with the navigation lights one for always on lights and the other for a blinking beacon light.<br>
<b>Headlight Control:</b> A button to turn the registry headlight on or off<br>
<b>Interior Control:</b> A button to turn on the interior lights throughout the ship. Might add an option to dim them if pwm pin available for it.<br>
<b>Deflector Control:</b> A button to turn on or off the deflector dish. Might give it a slight breathing cycle if pwm pins are available.<br>
