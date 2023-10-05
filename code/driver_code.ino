// Playing a digital WAV recording repeatadly using the XTronical DAC Audio library
// prints out to the serial monitor numbers counting up showing that the sound plays 
// independently of the main loop
// See www.xtronical.com for write ups on sound, the hardware required and how to make
// the wav files and include them in your code

#include "SoundData.h"
#include "XT_DAC_Audio.h"

#define SNS1 27
#define SNS2 26
#define SNS3 33
#define SNS4 32
#define LD1 35
#define LD2 34
#define LD3 39
#define LD4 36
#define GD1 23
#define GD2 22
#define GD3 18
#define THRESH 0

XT_Wav_Class* balloons[] = {new XT_Wav_Class(b1), new XT_Wav_Class(b2), new XT_Wav_Class(b3), new XT_Wav_Class(b4),
                           new XT_Wav_Class(b5), new XT_Wav_Class(b6), new XT_Wav_Class(b7), new XT_Wav_Class(b8)};

//XT_Wav_Class balloon(b1);
                                      
XT_DAC_Audio_Class DacAudio(25,3);    // Create the main player class object. 
                                      // Use GPIO 25, one of the 2 DAC pins and timer 0

int led_state[4][3];

int sensors[] = {SNS1, SNS2, SNS3, SNS4};
int leds[] = {LD1, LD2, LD3, LD4};
int grounds[] = {GD1, GD2, GD3};

//int pwm_channel = 5;
//int pwm_freq = 40000000;

void setup() {
  Serial.begin(115200);               // Not needed for sound, just to demo printing to the serial
                                      // Monitor whilst the sound plays, ensure your serial monitor
                                      // speed is set to this speed also.
//  ledcSetup(pwm_channel, pwm_freq, 8);
//  ledcAttachPin(32, pwm_channel);

  pinMode(SNS1, INPUT);
  pinMode(SNS2, INPUT);
  pinMode(SNS3, INPUT);
  pinMode(SNS4, INPUT);
  pinMode(LD1, OUTPUT);
  pinMode(LD2, OUTPUT);
  pinMode(LD3, OUTPUT);
  pinMode(LD4, OUTPUT);
  pinMode(GD1, OUTPUT);
  pinMode(GD2, OUTPUT);
  pinMode(GD3, OUTPUT);

  for(int i=0; i<4; i++){
    for(int j=0; j<3; j++){
      led_state[i][j] = HIGH;
    }
  }
}

int sound_counter = 0;
bool play = false;

void loop() {
  DacAudio.FillBuffer();                // Fill the sound buffer with data
  if(play){
    DacAudio.Play(balloons[sound_counter]);
    play = false;
  }
  
  for(int j=0; j<3; j++){
    digitalWrite(grounds[j], LOW);
    digitalWrite(grounds[(j+1)%3], HIGH);
    digitalWrite(grounds[(j+2)%3], HIGH);
    for(int i=0; i<4; i++){
      digitalWrite(leds[i], led_state[i][j]);
      delay(1);
      digitalWrite(leds[i], LOW);

      if(analogRead(sensors[i]) < THRESH && led_state[i][j] == HIGH){

        sound_counter = (sound_counter + 1)%8;
        
        led_state[i][j] = LOW;

        play = true;
      }

      if(i == 0 && j == 2){
        Serial.print(j);
        Serial.print(" ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(analogRead(sensors[i]));
      }
    }
  }
}
