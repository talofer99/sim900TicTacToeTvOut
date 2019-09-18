# sim900TicTacToeTvOut
Play tic tac toe on a TV using DTFM calling a sim 900 - confused - good - watch the video :)

It’s been a while that I wanted to build a basic mobile device/phone with Arduino and sim900.
And when doing somthing - why not over do it.

In this git you will find the following :
phone_ver1 - which is ment for mega, with sim900, 3X4 keypad, I2C 16x2 LCD. 
You an add a speaker and microphone and use the other functions the phone have.

tictactoeTVOUT - which is ment for uno using the TVOUT library, allowing you to connect the arduino to a composite 
video input. the game was created many years back, I just added the option to get the commands not from a keypad 
like the game was used to be, but from the serial, and that is connected to the serial2 of the mega.

I also added the handy At_CommandTest that allows you to send commands from the Serial to the SIM900.

You will need the following libraries : 
http://arduiniana.org/libraries/flash/  - You might have to adjust it
https://bitbucket.org/teckel12/arduino-timer-free-tone/downloads/
https://github.com/spicajames/Rtttl

and for the TVout 
https://github.com/nootropicdesign/arduino-tvout

