# si4463-beacon
CW Beacon on Arduino+si4463

This CW beacon is built on Arduino + si4463 module.

To configure si4463 you need desktop application from Silabs named "Wireless Development Suite".
As a result you will have ./si4463/radio/radio_config.h 
| For details please exprore folder ./doc/WDS/*.jpg 

The radio_config.h in the repository has been configured for 432.400 MHz RF out for 26.0 Mhz reference crystal. 

Si4463 is able to cover 2m band as well (119MHz - 159MHz).
Please note! Without having amateur radio licence you are very limited in frequency range and transmittion power. Especially with external antenna, without proper LPF.
For ISM frequency details please refer to: https://en.wikipedia.org/wiki/ISM_band

Don't forget to setup your own beacon text in si4463-beacon.ino:32 - morse.setText() 

Supported symbols:
 * Digits  0-9;
 * Letters  A-Z;
 * Space;
 * Forward slash /;
 * Underscore: _ (continious carrier 4 seconds long).


After downloading, rename the folder to 'si4463-beacon' if it isn't so.

[Discussion Thread(in Russian)](http://forum.22dx.ru/viewtopic.php?f=15&t=130)

Alexander Sholohov <ra9yer@yahoo.com>
