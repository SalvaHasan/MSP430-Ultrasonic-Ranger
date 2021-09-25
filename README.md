# MSP430 interfaced with an Ultrasonic ranger
In this project MSP430F2418 is interfaced with SRF04 sensor (Ultrasonic ranger) which is used to determine the distance of an obstacle from the sensor. Major application for this sensor include obstacle avoidance in toys and small robots. 

# Working Principle
The sensor sends a sonic burst of eight cycles towards an obstacle, frequency of these eight cycles is 40kHz. At the end of the eighth cycle the sensor raise its echo line high. The sound waves collided with an obstacle and echo back towards the sensor. The echo line is therefore a pulse whose width is proportional to the distance to the obstacle. By determining width of this pulse, it is possible to calculate the distance in inches or centimeters. If nothing is detected, then the SRF04 sensor will lower its echo line anyway after about 36ms. 

# Circuit Diagram
![CircuitDiagram](https://user-images.githubusercontent.com/22564930/134786061-e00261de-1190-43a9-8eb5-f5b88ae3df62.png)