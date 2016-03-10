void setup(){
  Serial1.begin(9600);

  pinMode(9, OUTPUT);
}

void loop(){
  if(Serial1.available())
    digitalWrite(9, HIGH);
}

