int reed1 = 22;
int reed2 = 23;
int reed3 = 24;
int buzzer = 34 ;
int pir = 31;
int reed1out = 28;
int reed2out = 29;
int reed3out = 30;

void setup() {
  Serial.begin(9600);
  pinMode ( reed1out , OUTPUT );
  pinMode ( reed2out , OUTPUT );
  pinMode ( reed3out , OUTPUT );
  pinMode (pir, INPUT );
  pinMode ( buzzer , OUTPUT );
  pinMode ( reed1 , INPUT_PULLUP );
  pinMode ( reed2 , INPUT_PULLUP );
  pinMode ( reed3 , INPUT_PULLUP );
  digitalWrite(reed1out, LOW);
  digitalWrite(reed2out, LOW);
  digitalWrite(reed3out, LOW);
  delay(2000);

}

void loop() {
  if (digitalRead(reed1) != LOW || digitalRead(reed2) != LOW || digitalRead(reed3) != LOW || digitalRead(pir) == LOW )
  {
    Serial.print("OPENED \n");
    digitalWrite(buzzer, HIGH);
    delay(1000);
  }
  else
  {
    Serial.print("CLOSED \n");
    digitalWrite(buzzer, LOW);
    delay(1000);
  }

}
