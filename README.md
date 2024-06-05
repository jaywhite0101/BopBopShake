# BopBopShake

"Bop Bop Shake" is an Arduino game that takes inspiration from the "Bop It" toy. When the device is turned on, different color lights will indicate what action the user is supposed to take, with the correct action taken within the time period adding a point to the user's score.

Every ten points, the user levels up, with the colored lights appearing for a shorter time each level, increasing the challenge.

Users can win by reaching level 5, or 50 points. Restarting the game is as easy as flipping the switch off, and then on again. 

The game begins when the switch is turned from "off" to "on", setting the points and levels to 0. There are three available inputs for the game: the left button (to be pressed when you see a green LED), the right button (to be pressed when you see a red LED), and the accelerometer (shake the device when you see a blue LED). 

If you fail to follow the direction for the correct input, or do not do it within the time frame, the board will flash with red LEDs and the points will reset back to 0.
