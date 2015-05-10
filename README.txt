Clara is a brain-sensing, environment-augmenting lamp. Its Neopixel strip slowly changes colors from warm 2700k light to cool, "full-spectrum" 6500k light as it detects that you're paying attention to whatever you're working on, in an effort to enhance your focus and minimize external distractions. At the same time, the volume of the ambient music track provided increases. (We used "The Spire" by Stafford Bawler, from the Monument Valley soundtrack.)

In the demo code provided, the light changes more rapidly than it would in real-world usage. Also, the start color is set to a bright red and the end color to a bright blue in order to more clearly demo the lamp's functionality.

A full writeup can be found here: http://www.thingswemake.com/clara/
Some process shots can be found here: http://macdevstudio.tumblr.com/post/117577725267/clara-by-belen-josh-and-marcelo

Developed by:

- Belen Tenorio / belentenorio.com / SVA MFA Products of Design '16
- Josh Sucher / joshsucher.com / SVA MFA Interaction Design '16
- Marcelo Mejia Cobo / macdevstudio.com / SVA MFA Interaction Design '16

Hardware used:

- Arduino Uno (should probably be a Mega, ultimately)
- Adafruit Music Maker shield
- Sparkfun BlueSMiRF Silver
- Adafruit 1m 60-LED Neopixel Strip (this code only lights up every other pixel. Use a Mega and remedy this.)
- Neurosky MindWave Mobile (also tested and works with a MindFlex headset.)
- 4ohm, 3w Adafruit speaker

Libraries needed:

- Adafruit Neopixel
- Adafruit VS1053
- kitschpatrol's Brain library: https://github.com/kitschpatrol/Brain

This project would not have been possible without the efforts of Arturo Vidich, Sofy Yuditskaya, and Eric Mika at ITP: 
http://frontiernerds.com/brain-hack