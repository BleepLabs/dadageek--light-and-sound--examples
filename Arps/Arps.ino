/*
 Examples created for my fall 2020 intro to Arduino course
 
 todo: comments!

 This example had two arpeggiators that randomly climb major scales
 There are control of the delay, filter, decay, and arp rate
 
 
 */

#include "ramp.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


AudioSynthWaveform       waveform1, waveform2, waveform3, waveform4, waveform5, waveform6; //xy=267,333
AudioAnalyzePeak         peak1, peak2, peak3, peak4;      //xy=277,514
AudioRamp     env1, env2;
AudioEffectTapeDelay      delay1;
AudioEffectMultiply      am1, am2;
AudioFilterStateVariable filterH1, filterH2, filterL1, filterL2;
AudioMixer4              mixer1;         //xy=416,340
AudioOutputAnalog        dac1;           //xy=571,350
AudioConnection          patchCord1(waveform1, peak1);
AudioConnection          patchCord2(waveform2, peak2);
AudioConnection          patchCord4(waveform3, peak3);
AudioConnection          patchCord4b(waveform4, peak4);
AudioConnection          patchCord5(env1, 0, am1, 1);
AudioConnection          patchCord51(env2, 0, am2, 1);

AudioConnection          patchCord6(waveform5, 0, filterL1, 0);
AudioConnection          patchCord6b(filterL1, 0, filterH1, 0);
AudioConnection          patchCord6c(filterH1, 2, am1, 0);

AudioConnection          patchCord6d(waveform6, 0, filterL2, 0);
AudioConnection          patchCord6e(filterL2, 0, filterH2, 0);
AudioConnection          patchCord6f(filterH2, 2, am2, 0);

AudioConnection          patchCord7(am2, 0, mixer1, 1);
AudioConnection          patchCord7b(am1, 0, mixer1, 0);



AudioConnection          patchCord8(mixer1, 0, delay1, 0);
AudioConnection          patchCord9(delay1, 0, mixer1, 3);

AudioConnection          patchCord12(mixer1, dac1);
// GUItool: end automatically generated code

#include <WS2812Serial.h>

const int numled = 64;
const int pin = 5;
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
byte drawingMemory[numled * 3];       //  3 bytes per LED
DMAMEM byte displayMemory[numled * 12]; // 12 bytes per LED

WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_GRB);

const float chromatic[88] = {55.00000728, 58.27047791, 61.73542083, 65.40639999, 69.29566692, 73.4162017, 77.78175623, 82.40690014, 87.30706942, 92.49861792, 97.99887197, 103.8261881, 110.0000146, 116.5409558, 123.4708417, 130.8128, 138.5913338, 146.8324034, 155.5635124, 164.8138003, 174.6141388, 184.9972358, 195.9977439, 207.6523763, 220.0000291, 233.0819116, 246.9416833, 261.6255999, 277.1826676, 293.6648067, 311.1270248, 329.6276005, 349.2282776, 369.9944716, 391.9954878, 415.3047525, 440.0000581, 466.1638231, 493.8833665, 523.2511997, 554.3653352, 587.3296134, 622.2540496, 659.2552009, 698.4565551, 739.9889431, 783.9909755, 830.6095048, 880.0001162, 932.3276461, 987.7667329, 1046.502399, 1108.73067, 1174.659227, 1244.508099, 1318.510402, 1396.91311, 1479.977886, 1567.981951, 1661.219009, 1760.000232, 1864.655292, 1975.533466, 2093.004798, 2217.46134, 2349.318453, 2489.016198, 2637.020803, 2793.82622, 2959.955772, 3135.963901, 3322.438019, 3520.000464, 3729.310584, 3951.066931, 4186.009596, 4434.92268, 4698.636906, 4978.032395, 5274.041605, 5587.652439, 5919.911543, 6271.927802, 6644.876037, 7040.000927, 7458.621167, 7902.133861, 8372.019192};
byte major[65] = {1, 3, 5, 6, 8, 10, 12, 13, 15, 17, 18, 20, 22, 24, 25, 27, 29, 30, 32, 34, 36, 37, 39, 41, 42, 44, 46, 48, 49, 51, 53, 54, 56, 58, 60, 61, 63, 65, 66, 68, 70, 72, 73, 75, 77, 78, 80, 82, 84, 85, 87, 89, 90, 92, 94, 96, 97, 99, 101, 102};

//Then we have our varible declarations like before
unsigned long cm;
unsigned long prev[8];
float flfo[8], freq[8], amp[8];
int lfo[8], prev_lfo[8], osc[8], out[8];
float test1;
unsigned long cu, du, looptime;
int jj;
int xy1, xy2;
byte trails[20];
byte tc;
float fade[8];
byte newnote;
#define delay_len 10000
short delaybank[delay_len];
byte rh[8] = {24, 24, 24, 24};
int dx;
byte seq_step[4];
int seq_step_lerp[4];
byte arp_dir = 1;
byte arp_top[4];
float pot[8];
float ff[8];
int seq_rate;
int bright = 24;
byte mask[4][64] = {
  {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    1, 2, 0, 0, 0, 0, 0, 0,
    1, 2, 3, 0, 0, 0, 0, 0,
    1, 2, 3, 0, 0, 0, 0, 0,
    1, 2, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
  },
  {
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 2, 2, 2, 2, 0, 0,
    0, 0, 0, 3, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
  },
  {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 2, 1,
    0, 0, 0, 0, 0, 3, 2, 1,
    0, 0, 0, 0, 0, 3, 2, 1,
    0, 0, 0, 0, 0, 0, 2, 1,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
  },
  {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 3, 0, 0, 0,
    0, 0, 2, 2, 2, 2, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
  },
};

void setup() {
  AudioNoInterrupts();
  leds.begin();
  pinMode(0, INPUT_PULLUP);
  analogReference(EXTERNAL);
  AudioMemory(15);
  waveform5.begin(1, 220.0, WAVEFORM_SAWTOOTH);
  waveform6.begin(1, 220.0, WAVEFORM_SQUARE);

  waveform1.begin(1, .15, WAVEFORM_TRIANGLE);
  waveform2.begin(1, .18, WAVEFORM_TRIANGLE);
  waveform3.begin(1,  .2,  WAVEFORM_TRIANGLE);
  waveform4.begin(1, .15, WAVEFORM_TRIANGLE);

  mixer1.gain(0, .5);
  mixer1.gain(1, .4);
  mixer1.gain(2, 0);
  mixer1.gain(3, .2);

  //  void begin(short *delayline, uint32_t max_len, uint32_t dly_len, short redux, short lerp);

  delay1.begin(delaybank, delay_len, delay_len / 2, 1, 1);
  analogWriteResolution(12);
  analogReadResolution(12);
  analogReadAveraging(16);
  AudioInterrupts();
  //env1.trig();
  //env2.trig();
  prev[2] = 50;
}

void loop() {
  cm = millis();

  if (cm - prev[1] > 33) {
    prev[1] = cm;


    if (peak1.available()) {
      lfo[0] = (peak1.read() * 800.0) + 100.0;
    }


    if (peak2.available()) {
      lfo[1] = (peak2.read() * 800.0) + 100.0;
    }

    pot[0] = analogRead(A0);
    pot[1] = analogRead(A1) / 4;
    pot[2] = analogRead(A2) >> 4;
    env1.decay(.9999 - ((pot[2] / 255)*.005));

    pot[3] = (analogRead(A3) / 4095.0) * delay_len;
    pot[4] = (analogRead(A4) / 4095.0);
    delay1.length(pot[3]);
    mixer1.gain(3, pot[4]);

    seq_rate = pot[1] + 33;
    ff[1] = (freq[0] * .5) + pot[0];
    ff[2] = (freq[1] * .5) + pot[0];

    if (ff[1] > 16000) {
      ff[1] = 16000;
    }
    filterL1.frequency(ff[1] + lfo[0]);
    filterL1.resonance(3.1);

    filterH1.frequency(ff[1]);
    filterH1.resonance(3.1);


    filterL2.frequency(ff[2]);
    filterL2.resonance(1.2);

    filterH2.frequency(20);
    filterH2.resonance(.7);

    if (seq_step_lerp[0] < seq_step[0] - 2 ) {
      seq_step_lerp[0] += 2;
    }

    if (seq_step_lerp[0] > seq_step[0] + 2 ) {
      seq_step_lerp[0] -= 2;
    }

    if (seq_step_lerp[1] < seq_step[0] - 2 ) {
      seq_step_lerp[1] += 1;
    }

    if (seq_step_lerp[1] > seq_step[1] + 2 ) {
      seq_step_lerp[1] -= 1;
    }

    for ( int xr = 0; xr < 8; xr++) {
      for ( int yc = 0; yc < 8; yc++) {
        int xyc = (xr) + ((yc) * 8);
        hsv(xyc, 0, 0, 0);


        if (mask[0][xyc] > 0) {
          hsv(xyc, pot[2] + 120 + ((seq_step_lerp[0] * 3) + (xr * 16)), 255, bright);
        }
        if (mask[2][xyc] > 0) {
          hsv(xyc, pot[2] + 120 + ((seq_step_lerp[0] * 3) + (((xr - 7) * -1) * 16)), 255, bright);
        }
        if (mask[1][xyc] > 0) {
          hsv(xyc, pot[2] + 100 - ((seq_step_lerp[1] * 3) + (yc * 32)), 240 + random(10), bright);
        }
        if (mask[3][xyc] > 0) {
          hsv(xyc, pot[2] + 100 - ((seq_step_lerp[1] * 3) + (((yc - 7) * -1) * 32)), 240 + random(10), bright);
        }

      }
    }
    leds.show();
  }

  if (cm - prev[3] > seq_rate) {
    prev[3] = cm;
    seq_step[0] += random(2, 4);

    if (seq_step[0] > arp_top[0]) {
      seq_step[0] = random(4, 10);
      arp_top[0] = seq_step[0] + random(10, 32);
    }

    env1.trig();
    freq[0] = chromatic[major[seq_step[0]]];
    waveform5.frequency(freq[0]);
  }

  if (cm - prev[2] > seq_rate) {
    prev[2] = cm;
    seq_step[1] += random(2, 4);

    if (seq_step[1] > arp_top[1]) {
      //seq_step[1] = random(4, 10);
      seq_step[1] = seq_step[0] / 2;
      arp_top[1] = arp_top[0] / 2;
    }
    env2.decay(.9999);
    env2.trig();
    freq[1] = chromatic[major[seq_step[1]]];
    waveform6.frequency(freq[1]);
  }

  if (cm - prev[0] > 500 && 1 == 1) {
    prev[0] = cm;
    dx += 100;
    if (dx > delay_len - 2) {
      dx = 0;
    }
    //delay1.length(dx);
    Serial.print(pot[0]);
    Serial.print(" ");
    Serial.println(pot[1]);
    /*
      Serial.print("processor: ");
      Serial.print(AudioProcessorUsageMax());
      Serial.print("%    Memory: ");
      Serial.print(AudioMemoryUsageMax());
      Serial.println();
      AudioProcessorUsageMaxReset(); //We need to reset these valuse so we get a real idea of wh the audio code is doing rather than jsut peaking in every half a second
      AudioMemoryUsageMaxReset();
    */
  }
}


float fract(float x) {
  return x - int(x);
}

float mix(float a, float b, float t) {
  return a + (b - a) * t;
}

float step(float e, float x) {
  return x < e ? 0.0 : 1.0;
}

byte RedLight;
byte GreenLight;
byte BlueLight;


void hsv(int pixel, int h, byte s, byte v) {

  if (h < 0) {
    h += 255;
  }
  if (h > 255) {
    h -= 255;
  }
  h = (h * 192) / 256;  // 0..191
  unsigned int i = h / 32;   // We want a value of 0 thru 5
  unsigned int f = (h % 32) * 8;   // 'fractional' part of 'i' 0..248 in jumps

  unsigned int sInv = 255 - s;  // 0 -> 0xff, 0xff -> 0
  unsigned int fInv = 255 - f;  // 0 -> 0xff, 0xff -> 0
  byte pv = v * sInv / 256;  // pv will be in range 0 - 255
  byte qv = v * (256 - s * f / 256) / 256;
  byte tv = v * (256 - s * fInv / 256) / 256;

  switch (i) {
    case 0:
      RedLight = v;
      GreenLight = tv;
      BlueLight = pv;
      break;
    case 1:
      RedLight = qv;
      GreenLight = v;
      BlueLight = pv;
      break;
    case 2:
      RedLight = pv;
      GreenLight = v;
      BlueLight = tv;
      break;
    case 3:
      RedLight = pv;
      GreenLight = qv;
      BlueLight = v;
      break;
    case 4:
      RedLight = tv;
      GreenLight = pv;
      BlueLight = v;
      break;
    case 5:
      RedLight = v;
      GreenLight = pv;
      BlueLight = qv;
      break;
  }
  leds.setPixelColor(pixel, RedLight, GreenLight, BlueLight);

}
