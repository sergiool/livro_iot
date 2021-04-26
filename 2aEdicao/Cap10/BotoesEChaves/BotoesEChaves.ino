void setup() {
    Serial.begin(115200);
    pinMode(D0, INPUT);
    pinMode(D1, INPUT_PULLUP);
    pinMode(D2, INPUT_PULLUP);
    pinMode(D3, INPUT_PULLUP);
    pinMode(D4, INPUT_PULLUP);
    pinMode(D5, INPUT_PULLUP);
    pinMode(D6, INPUT_PULLUP);
    pinMode(D7, INPUT_PULLUP);
    pinMode(D8, INPUT);
}

void loop() {
    delay(1000);
    Serial.print(" D0:"); Serial.print(digitalRead(D0)?'1':'0');
    Serial.print(" D1:"); Serial.print(digitalRead(D1)?'1':'0');
    Serial.print(" D2:"); Serial.print(digitalRead(D2)?'1':'0');
    Serial.print(" D3:"); Serial.print(digitalRead(D3)?'1':'0');
    Serial.print(" D4:"); Serial.print(digitalRead(D4)?'1':'0');
    Serial.print(" D5:"); Serial.print(digitalRead(D5)?'1':'0');
    Serial.print(" D6:"); Serial.print(digitalRead(D6)?'1':'0');
    Serial.print(" D7:"); Serial.print(digitalRead(D7)?'1':'0');
    Serial.print(" D8:"); Serial.println(digitalRead(D8)?'1':'0');
}
