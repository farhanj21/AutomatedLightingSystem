const int pirPin = 2;      
const int ldrPin = A0;     
const int ledPin = 9;      
const int switchPin = 3;   

bool systemOn = true;
unsigned long lastMotionTime = 0;  

const unsigned long timeout = 5000;
// const unsigned long timeout = 300000; 


void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);

  Serial.begin(9600); 
}

void loop() {
 
  systemOn = digitalRead(switchPin) == HIGH; //current switch state

  if (!systemOn) {
    digitalWrite(ledPin, LOW);
    Serial.println("System is Off. LED turned Off.");
    return;
  }

  int motionDetected = digitalRead(pirPin);
  int lightLevel = analogRead(ldrPin); 

  Serial.print("LDR Value: ");
  Serial.print(lightLevel);
  Serial.print(" and PIR: ");
  Serial.println(motionDetected ? "Motion Detected" : "No Motion");

  if (motionDetected) {
    lastMotionTime = millis(); // Reset when motion is detected
  }

  // check if motion detected and dark
  if ((millis() - lastMotionTime < timeout) && (lightLevel < 200)) {
    int brightness = map(lightLevel, 0, 1023, 255, 0); 
    analogWrite(ledPin, brightness);
    Serial.print("LED On - Brightness: ");
    Serial.println(brightness);
  } else {
    digitalWrite(ledPin, LOW); 
    Serial.println("LED Off - No motion/Enough Light");
  }

  delay(500);
}
