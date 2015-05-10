// Clara, the brain-sensing, environment-augmenting lamp
// by Marcelo Mejia Cobo, Josh Sucher, Belen Tenorio
// thingswemake.com/clara
// 2015

#define BAUDRATE 57600
#define DEBUGOUTPUT 0

#include <Brain.h>
#include <Adafruit_NeoPixel.h>

#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

#define PIN 8

#define NUMPIXELS      60

#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t myColors[][3] = {{252,103,44},   // halogen
                         {43,170,218}};   // direct sun 

// halogen 224, 241, 224

int beginRed = myColors[0][0];
int beginGreen = myColors[0][1];
int beginBlue = myColors[0][2];
int beginVol = 30;

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
int payloadIndex;
byte payloadData[169] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;

int state = 0;
const int STATE_WAIT_FOR_FIRST_A        = 0;
const int STATE_WAIT_FOR_SECOND_A       = 1;
const int STATE_WAIT_FOR_PAYLOAD_LENGTH = 2;
const int STATE_WAIT_FOR_PAYLOAD        = 3;
const int STATE_WAIT_FOR_CHECKSUM       = 4;

void setup() {
   Serial.begin(BAUDRATE);           // USB  
  
  strip.begin();
  strip.setBrightness(150);
  
    for(int j = -1; j < NUMPIXELS; j++) {
      strip.setPixelColor(j, strip.Color(beginRed, beginGreen, beginBlue));
      j++;
}

  strip.show(); // Initialize all pixels to 'off'
  
    // initialise the music player
  if (! musicPlayer.begin()) { // initialise the music player
   //  Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
//  Serial.println(F("VS1053 found"));

  if (!SD.begin(CARDCS)) {
  //  Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
//  Serial.println("SD OK!");

  musicPlayer.setVolume(beginVol,beginVol);

  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    //Serial.println(F("DREQ pin is not an interrupt pin"));
  }

}
void loop() {

     if (! musicPlayer.startPlayingFile("03.mp3")) {
    //Serial.println("Could not open file 03.mp3");
    while (1);
  }
  //Serial.println(F("Started playing"));

  while (musicPlayer.playingMusic) {

serialEvent();

  }

}
void parsePayload(){
  
  
  
  poorQuality = 200;
  attention = 0;
  meditation = 0;

  for(int i = 0; i < payloadLength; i++) {    // Parse the payload
    switch (payloadData[i]) {
    case 2:
      i++;            
      poorQuality = payloadData[i];
      bigPacket = true;            
      break;
    case 4:
      i++;
      attention = payloadData[i];                        
      break;
    case 5:
      i++;
      meditation = payloadData[i];
      break;
    case 0x80:
      i = i + 3;
      break;
    case 0x83:
      i = i + 25;      
      break;
    default:
      break;
    } // switch
  } // for loop
  if(bigPacket) {
    
  strip.setBrightness(255);

//    for(int j = -1; j < 16; j++) {
//      strip.setPixelColor(j, strip.Color(255, 241, 224));
//}
      strip.show();

int red = map(attention,0,100,252,43);
int green = map(attention,0,100,103,170);
int blue = map(attention,0,100,44,218);

if(attention == 0) {
  
red = 0;
green = 0;
blue = 0;
  
}

//int red = map(attention,0,100,100,255);
//int green = map(attention,0,100,100,255);
//int blue = map(attention,0,100,100,255);

int vol = map(attention,1,100,30,0);

fade(100,beginRed,beginGreen,beginBlue,red,green,blue,beginVol,vol);

//    Serial.print("PoorQuality: ");
//    Serial.print(poorQuality, DEC);
//    Serial.print(" Attention: ");
//    Serial.print(attention, DEC);
//    Serial.print(" Time since last packet: ");
//    Serial.print(millis() - lastReceivedPacket, DEC);
    lastReceivedPacket = millis();
//    Serial.print("\n");                     
  }
  bigPacket = false;   
}


void fade(int duration, int startRed, int startGreen, int startBlue, int endRed, int endGreen, int endBlue, int startVol, int endVol) {
  
  int redDiff = endRed - startRed;
  int greenDiff = endGreen - startGreen;
  int blueDiff = endBlue - startBlue;
  
  int volDiff = endVol - startVol;
    
  int neoDelay = 2;
  int steps = duration / neoDelay;
  
  int redValue, greenValue, blueValue, volValue;

  for(int i = 0; i < steps - 1; ++i) // larger values of 'n' will give a smoother/slower transition.
{
   redValue = startRed + (redDiff * i / steps);
   greenValue = startGreen + (greenDiff * i / steps);
   blueValue = startBlue + (blueDiff * i / steps);
   volValue = startVol + (volDiff * i / steps);
   
   musicPlayer.setVolume(volValue,volValue);
   
    for(int j = -1; j < NUMPIXELS; j++) {
      strip.setPixelColor(j, strip.Color(redValue, greenValue, blueValue));
      delay(neoDelay);
      j++;
}
    strip.show();
   
}

    for(int j = -1; j < NUMPIXELS; j++) {
      strip.setPixelColor(j, strip.Color(endRed, endGreen, endBlue));
      delay(neoDelay);
      j++;
}
    strip.show();

beginRed = endRed;
beginGreen = endGreen;
beginBlue = endBlue;
beginVol = endVol;
  
}

void serialEvent(){
  if(Serial.peek() >= 0){
    switch(state){
      case STATE_WAIT_FOR_FIRST_A:
        if(Serial.read() == 170) state = STATE_WAIT_FOR_SECOND_A;
      break;
      case STATE_WAIT_FOR_SECOND_A:
        if(Serial.read() == 170) state = STATE_WAIT_FOR_PAYLOAD_LENGTH;
      break;
      case STATE_WAIT_FOR_PAYLOAD_LENGTH:
        payloadLength = Serial.read();
        if(payloadLength > 169){
          state = STATE_WAIT_FOR_FIRST_A;
          return;
        }
        generatedChecksum = payloadIndex = 0;
        state = STATE_WAIT_FOR_PAYLOAD;
      break;
      case STATE_WAIT_FOR_PAYLOAD:
        if(payloadIndex < payloadLength){
          payloadData[payloadIndex] = Serial.read();
          generatedChecksum += payloadData[payloadIndex];
          payloadIndex++;
        }else{
          state = STATE_WAIT_FOR_CHECKSUM;
        }
      break;
      case STATE_WAIT_FOR_CHECKSUM:
        checksum = Serial.read();
        generatedChecksum = 255 - generatedChecksum;
        if(checksum == generatedChecksum) {
          parsePayload();
          state = STATE_WAIT_FOR_FIRST_A;
        }else{
          state = STATE_WAIT_FOR_FIRST_A;
        }
      break;
    }
  }
}
