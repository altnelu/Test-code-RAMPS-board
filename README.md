# Test-code-RAMPS-board

Check the endstop and temperature sensors, move steppers,ON OFF HEATER

Send serial command

x=nr (nr- The number of steps for X)

x=-nr (The number of steps in the opposite direction)

y=nr or y=-nr or z=nr or z=-nr

Ex:    x=1000

Ex:    x=-1000y1000

Ex:    x=-500y-500z-500

s=1 Time between two steps

h0=1 digitalWrite(HEATER_0_PIN,h0)

h1=1 digitalWrite(HEATER_1_PIN,h1)

h2=0 digitalWrite(HEATER_2_PIN,h2)
