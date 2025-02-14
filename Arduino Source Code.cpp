// Pin Assignments
const int pirPin = 2;        // PIR sensor input
const int ldrPin = A0;       // LDR sensor input (analog)
const int ledPin = 9;        // LED output (PWM for brightness)
const int switchPin = 3;     // Master On/Off switch

// Timing Variables
unsigned long motionDetectedTime = 0;  
const unsigned long timeout = 5 * 60 * 1000; // 5 minutes in milliseconds

bool motionDetected = false;

void setup() {
    pinMode(pirPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(switchPin, INPUT_PULLUP); // Slide switch as on/off control
    Serial.begin(9600);
}

void loop() {
    int switchState = digitalRead(switchPin); 
    if (switchState == LOW) { // System turned OFF
        digitalWrite(ledPin, LOW);
        return;
    }

    int motionState = digitalRead(pirPin);
    int ldrValue = analogRead(ldrPin);

    Serial.print("LDR: "); Serial.print(ldrValue);
    Serial.print(" | PIR: "); Serial.println(motionState);

    if (motionState == HIGH) {  
        motionDetected = true;
        motionDetectedTime = millis(); // Reset timer
    }

    if (motionDetected) {
        if (ldrValue < 200) { // If it's dark
            int brightness = map(ldrValue, 0, 200, 255, 100); // Adjust brightness
            analogWrite(ledPin, brightness);
        } else {
            digitalWrite(ledPin, LOW); // Enough ambient light, turn off LED
        }

        // Check if 5 minutes have passed with no motion
        if (millis() - motionDetectedTime > timeout) {
            motionDetected = false;
            digitalWrite(ledPin, LOW);
        }
    } else {
        digitalWrite(ledPin, LOW);
    }
}
