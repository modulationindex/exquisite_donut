## Exquisite Donut Instance

This is my implementation for the Exquisite Donut project. More info about this project can be found here:
https://github.com/workergnome/exquisite_donut

The **DonutCop** class holds the rules and the OSC signalling functionality. <br />

The **Sprinkle** class represents the particle

### Behavior

In this implementation, several sets of evenly spaced white lines attempt to expand or contract. Each sequential line set has the opposite direction of attempted motion from the prior set. When a particle crosses a line, the line abruptly animates against it's attempted direction of motion (expanding when it should be contracting, etc). Lines are continuously trying to reach their target thickness. 

When the number of sprinkles exceeds a configurable threshold (eatSprinklesOverSize), sprinkles are "eaten." They quickly shrink to zero radius and are removed from the queue. This is both an attempt to preserve the framerate and a certain amount of motion within the grid of lines. When there are too many sprinkles, the lines are constantly forced to the open or closed thickness, and that's not nearly as much fun to watch. 

A shader is used to mask particles so they only are revealed by the white lines. The goal of this process along with the increasing spatial resolution of the line sets over time is to play with perceptual continuity when observing the sprinkle trajectories.

Free particle variables are mapped to oscillation frequency and amplitude of the sprinkle radii. 


### Keyboard Shortcuts

g - show / hide gui <br />
f - show / hide framerate <br />
d - eat some sprinkles <br />
a - manually advance to the next line set <br />

### GUI

A GUI is included for easy configuration of a few variables on site.  <br />

Press the g key to open the gui.   <br />

Changes are saved to carlson_settings.xml when the gui is closed. Make sure you press g again to close the gui and save your changes (or edit the xml file directly). <br />







