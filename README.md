# proveIR

<b> Description </b>

IR Timing gate used by Cal Poly PROVE Labs for their outreach program.

These Arduino sketches use:

TimerOne Library by Paul Stoffregen
https://github.com/PaulStoffregen/TimerOne

All versions take raw ADC Reads of IR proximity sensors, and if the returned distance is less than a certain threshold, (which means the voltage will be above a certain threshold) then the timer will start. It stops when a high voltage is detected at the end gate, returning the time between detections.

<b> Usage and Commands </b>

The number of reads, the ADC detection threshold, and a calibration factor can all be set in the sketch. The number of reads refers to the number of times the ADC will be read in a single burst to get an average. The default is 8, although becuase of the nature of the interrupts, this cannot be done in a loop. The concurrent reads are meant to prevent a false detect from any voltage spikes that may occur. The detection threshold is how high the ADC average has to get before it counts as a detection. And finally the calibration factor adcAdj is an array that can be used to make certain sensors more or less sensitive.

set threshold # - Sets the threshold for the adc read average needed to trigger a timer start stop.

burst - Performs a burst read of the number of values specified and returns the average for all sensors.

start - Maunally starts the timer.

end - Manually ends the timer and displays the time since it was started,

reset - Ends the timer without returning a time. This is done automatically whenever a setting is changed.

toggle - Toggle the timer on and off. The adcs will still read if the timer is off but they will not trigger start and end events.

<b> Physical Limitations </b>

1. IR sensors cannot be pointed directly at each other. The emitters will interfere with each other and not give accurate readings.

2. Although in sunlight these sensors work well, other light sources such as heat lamps that emit a large ammount of IR radiation will interfere with these sensors, even if they are not directly pointed at the sensor.

<b> Future Plans </b>

- Schematics and .brd files for Arduino Uno shields.
- A simple GUI created in MATLAB to issue commands and record times.
