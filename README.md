# si4463-beacon
CW Beacon on Arduino+si4463

This CW beacon is built on Arduino + si4463 module.

To configure si4463 you need desktop application from Silabs named "Wireless Development Suite".
As a result you will have ./si4463/radio/radio_config.h 
| For details please exprore folder ./doc/WDS/*.jpg 


Si4463 is able to cover 2m band as well (119MHz - 159MHz).
Please note! Without having amateur radio licence you are very limited in frequency range and transmittion power. Especially using external antenna and/or without proper LPF.
For ISM frequency details please refer to: https://en.wikipedia.org/wiki/ISM_band

Don't forget to setup your own beacon text in si4463-beacon.ino:32 - morse.setText() 

Supported symbols:
 * Digits  0-9;
 * Letters  A-Z;
 * Forward slash /;
 * Underscore: _ (4 seconds long continious carrier).


After downloading, rename folder to 'si4463-beacon'.
The project was tested in Arduino 1.6.7 development environment. If you have any problems in sketch compiling please use exactly this version.


Alexander Sholohov <ra9yer@yahoo.com>
