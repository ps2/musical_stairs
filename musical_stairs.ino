int txPin=10;
int rxPin=5;
int led=A3;
int enable=A4;
int currentBeam = 0;
int signalling = LOW;
int counter = 0;
int countAtDetect = 0;
int notes[8] = {0,2,4,5,7,9,11,12};
// Track which beams are active (seen within the last x cycles)
int offCount[8];
int state=LOW;

// Pins 5,6,7 are addressing pins

#define Frequency    38000L // This is the frequency (in Hertz) to drive the IR LED

void setup()
{
  midi_setup();
  pinMode(txPin,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(enable,OUTPUT);
  pinMode(rxPin,INPUT);
  digitalWrite(rxPin, HIGH);
 
  Serial.begin(9600);
 
  digitalWrite(led, LOW); 
  
  // Mark all the beams off; we need to get valid signal before tripping.
  for(int i=0; i<8; i++) {
    offCount[i] = 5;
  }

  // Select beam 0
  selectBeam(0);
  
  delay(1000);

  setVolume(120);
  setInstrument(0,12);
  //midi_loop();
  //playNote(1);
  tone(txPin, Frequency);
}

void selectBeam(int beamNo) {
  digitalWrite(enable,HIGH);
  
  // This affects both the mux and demux (input and output)
  digitalWrite(A0,bitRead(beamNo,0));
  digitalWrite(A1,bitRead(beamNo,1));
  digitalWrite(A2,bitRead(beamNo,2));

  digitalWrite(enable,LOW);
//  if ( beamNo == 7 ) {
//    digitalWrite(enable,LOW);
//  } else {
//    digitalWrite(enable,HIGH);
//  }
}

void playNote(int beamNo) {
  int note = notes[beamNo];
  noteOn(0, 45+note, 127);
  //delay(200);
  //noteOff(0, 45+note, 127);
}

void loop()
{
  
  if (state == LOW && digitalRead(rxPin) == LOW) {
    countAtDetect = counter;
    state=HIGH;
  }
  
  if (counter == 0) {
    if (state == HIGH) {
      // Beam is intact
//      if (offCount[currentBeam] > 0) {
//        Serial.print("beam ");
//        Serial.print(currentBeam);
//        Serial.print(" detected on count ");
//        Serial.print(countAtDetect);
//        Serial.print(" after  ");
//        Serial.print(offCount[currentBeam]);
//        Serial.print(" off cycles.  ");
//        Serial.print("\n");
//      }
      offCount[currentBeam] = 0;
    } else {
      // Beam is broken.
      offCount[currentBeam] += 1;
      
      if (offCount[currentBeam] == 3) {

        //digitalWrite(led, HIGH);
        //delay(50);
        //digitalWrite(led, LOW);
//        Serial.print("Playing note for beam: ");
//        Serial.print(currentBeam);
//        Serial.print("\n");
        //if (currentBeam != 0 && currentBeam != 1 && currentBeam != 7 && currentBeam != 6) {
          playNote(currentBeam);
        //}
      } else if (offCount[currentBeam] == 30) {        
//        Serial.print("beam ");
//        Serial.print(currentBeam);
//        Serial.print(" off for 30 cycles.\n");
      }
    }

    // Advance to next beam
    currentBeam += 1;
    if (currentBeam >= 8) {
      currentBeam = 0;
    }
    
    // Select address
    selectBeam(currentBeam);
    state = LOW;
    countAtDetect = 0;
    counter = 300;
  }
  
  counter--;
}
