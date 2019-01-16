
void mainPage() {                                         // Main Page
  if (millis() - lcdPr >500) {
  lcd.setCursor (0, 0);
      lcd.print ("T=");
      if (hTempC<=0) lcd.print ("much");
      else {lcd.print ('-'); lcd.print (hTempC);lcd.print("'C");}
      lcd.print ("  ");
      
      lcd.setCursor (0, 1);
      lcd.print ("P=");
      if (pressurePa<=31.0){
      pressurePa = constrain (pressurePa, 0.0, 31.0); 
      lcd.print (pressurePa,1);
      lcd.print ("mBar ");}
      else lcd.print ("much    ");
      lcdPr = millis();
  }
}

/*void hashPage() {                                      //страница вызываемая нажитием #
  if (millis() - lcdPr >500) {
  lcd.setCursor (0, 0);
      lcd.print ("T=");
      lcd.print (temp);
      lcd.print (" ");
      lcd.setCursor (0, 1);
      lcd.print ("P=");
      lcd.print (pressure);
      lcd.print (" ");
      lcdPr = millis();
//      delay (1000);
  }
}*/

void viewPage() {                                                        //страница VIEW
  char keys [5];  
  again1:
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print ("T=-");
  for (int s=0; s<3;){
    key = keypad.getKey();
    if (key=='*') goto again1;
    lcd.blink(); 
    if(key) {lcd.print(key);if (key==0) key++; keys[s]=key; s++;}
  }
  tNeed = (keys[0]-'0')*100 + ((keys[1]-'0')*10 + keys[2]-'0');
  for (int l=0; l<5; l++) {keys[l]=0;}
      lcd.setCursor(3,0);
      if (tNeed<100) lcd.print('0');
      if (tNeed<10) lcd.print('0');
      lcd.print (tNeed);
      EEPROM_write (8, tNeed);
      again2:
      lcd.setCursor (0, 1);
      lcd.print("          ");
      lcd.setCursor (0, 1);
      lcd.print ("p=");
  for (int s=0; s<2;){
    key = keypad.getKey();
    if (key=='*') goto again2;
    lcd.blink(); 
    if(key) {lcd.print(key); keys[s]=key; s++;}
  }
  paNeed = (keys[0]-'0')*10 + (keys[1]-'0');
  for (int l=0; l<5; l++) {keys[l]=0;}
  lcd.setCursor(2,1);
  if (paNeed<10) lcd.print('0');
  lcd.print (paNeed);
  Serial.print("paNeed=");
  Serial.println(paNeed);
  lcd.print ("mBar");
  EEPROM_write (12, paNeed);
  again3:
  lcd.setCursor(11,0);
  lcd.print("   ");
  lcd.setCursor(11,0);
  for (int s=0; s<2;){
    key = keypad.getKey();
    if (key=='*') goto again3;
    lcd.blink(); 
    if(key) {lcd.print(key); keys[s]=key; s++;}
  }
  hoursNeed = (keys[0]-'0')*10 + (keys[1]-'0');
  for (int l=0; l<5; l++) {keys[l]=0;}
  lcd.setCursor(11,0);
  if (hoursNeed<10) lcd.print('0');
  lcd.print (hoursNeed);
  EEPROM_write (16, hoursNeed);
  lcd.print(":");
  again4:
  lcd.setCursor(14,0);
  lcd.print("  ");
  lcd.setCursor(14,0);
  for (int s=0; s<2;){
    key = keypad.getKey();
    if (key=='*') goto again4;
    lcd.blink();
    if(key) {lcd.print(key); keys[s]=key; s++;}
  }
  minutesNeed = (keys[0]-'0')*10 + (keys[1]-'0');
  for (int l=0; l<5; l++) {keys[l]=0;}
  lcd.noBlink();
  lcd.setCursor(14,0);
  if (minutesNeed<10) lcd.print('0');
  lcd.print (minutesNeed );
  EEPROM_write (20, minutesNeed);
  lcd.clear();
  needToWrite=false;
  key=0;
  Bpage();
}
                                                        // страница просмотра настроенных данных    B
void Bpage(){
  lcd.setCursor(0,0);
  lcd.print("Tneed:-");
  lcd.print(tNeed);
  lcd.setCursor(0,1);
  lcd.print("pNeed:");
  lcd.print(paNeed);
  lcd.print("mBar");
  lcd.setCursor(11,0);
  if (hoursNeed<10) lcd.print('0');
  lcd.print(hoursNeed);
  lcd.setCursor(13,0);
  lcd.print(":");
  if (minutesNeed<10) lcd.print('0');
  lcd.print(minutesNeed);
  page=0;
  delay (5000);
  lcd.clear();
  }
  
void backTimer () {
  if (flagTime){
o = hoursNeed;
t = minutesNeed;
flagTime=false;}
if (back_timer) {
      unsigned long startTime = millis();
      lcd.setCursor(11, 0);                       
       if (t == -1) {o--;t = 59;}
      if (o < 10) lcd.print(0);   
      lcd.print(o);
      if (millis() % 1000 <= 500) lcd.print(":"); else lcd.print(" ");      //моргаем ":" каждую секунду
      lcd.setCursor(14, 0);
      if (t < 10) lcd.print(0);
      lcd.print(t);
    if (startTime - wait >= minute) {t--;wait = millis();}
    }    
  if (o<=0&&t<=0){
    stopAll ();
  delay (1000);
  lcd.clear();
  if (buzz) {buzzTimerEnd ();buzz = false;}
  lcd.print("TIMES UP!");
  delay(1000);
  back_timer = false;
  lcd.clear();
  }
}
                                  // прерывание работы машины
void stopAll () {
  digitalWrite (15, LOW);
  digitalWrite (16, LOW);
  digitalWrite (17, LOW);
  t=0;
  o=0;
  flagWork = false;
  endWork=millis();
}
                                  // звук окончания таймера
void buzzTimerEnd (){
  tone (3, 2500, 250);         
  delay (300);
  tone (3, 1900, 150);
  delay (250);
  tone (3, 2100, 250);
  delay (400);
}
                                  // звук начала работы (нажатие 'C')
void buzzStartWork () {
  tone(3, 2500, 300);  
}
                                  // Звук прерывания работы машины
void buzzStop () {
  tone(3, 700, 200);
  delay(200);
  tone(3, 300, 300);
  delay(300);
}
                                  // Звук старта таймера
void buzzTimerStart(){
  tone (3, 4500, 25);
  delay (200);
  tone (3, 4500, 25);
  delay (200);
  tone (3, 4500, 50);  
}
