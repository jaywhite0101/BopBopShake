# BopBopShake

"Bop Bop Shake" is an Arduino game that takes inspiration from the "Bop It" toy. When the device is turned on, different color lights will indicate what action the user is supposed to take, with the correct action taken within the time period adding a point to the user's score.

Every ten points, the user levels up, with the user having to figure out the correct button to press from the first level-up and onward, as the colored LEDs mix in with the rainbow lights for the level-up.

Users can win by reaching level 5, or 50 points. Restarting the game is as easy as flipping the switch off, and then on again. 

The game begins when the switch is turned from "off" to "on", setting the points and levels to 0. There are two available inputs for the game: the left button (to be pressed when you see a green LED) and the right button (to be pressed when you see a red LED).

If you fail to follow the direction for the correct input, the board flashes red, makes a noise, and resets you to level 0 at zero points.
