
void setup() {
  pinMode (15, OUTPUT);
  pinMode (16, OUTPUT);
  pinMode (17, OUTPUT);
  Serial.begin (9600);
  long counter;
  EEPROM_read(0, counter);
  counter++;
  EEPROM_write(0, counter);
  Serial.print("It's reset #"); Serial.println(counter, DEC);
  EEPROM_read (8, tNeed);
  EEPROM_read (12, paNeed);
  EEPROM_read (16, hoursNeed);
  EEPROM_read (20, minutesNeed);
  Serial.println (tNeed);
  Serial.println (paNeed);
  Serial.println (hoursNeed);
  Serial.println (minutesNeed);
  //  Serial.println ();
  lcd.init();
  lcd.backlight();                            // Включаем подсветку дисплея
}

void loop() {
  if (millis() < 10000) {
    lcd.setCursor(0, 0);
    lcd.print("Loadings.");
    lcd.setCursor(0, 1);
    lcd.print("..........");
  }
  
                    ///////////////////////////////////////////////////////////////////
                    //// Перевод аналоговых значений датчика температуры в Градусы ////
                    ///////////////////////////////////////////////////////////////////

  hTemp = hTemperatureAnalogAverage;    // для UNO поменять на А3 / для Nano на А7
  hTempC = map (hTemp, 220, 900, 91, 5);
  temp = temperatureAnalogAverage;    // для UNO поменять на А3 / для Nano на А7
  tempC = map (temp, 220, 900, 91, 5);




  if (millis() - antiHype >= 1000) {
    Serial.print ("Real tempC = ");
    Serial.println (tempC);
    antiHype = millis();
  }
  pressure = paAnalogAverage; // для UNO поменять на А2 / для Nano на А6
  pressurePa = map (pressure, 550, 410, 318, 30) * 0.1;                                         //дописать
  pressurePa = constrain (pressurePa, -5.0, 31.0);
  key = keypad.getKey();
  switch (keypad.getState()) {
    case PRESSED:
      //if (key == '#') {if (page==0) {page=1; lcd.clear();} else {page=0; lcd.clear();}}
      if (key == 'A') {
        lcd.clear();
        needToWrite = true;
        viewPage();
      }
      if (key == 'B') {
        lcd.clear();
        Bpage();
      }
      if (key == 'C') {
        flagWork = true;
        flagTime = true;
        firstRun = true;
        buzzStartWork();
        startWork = millis();
        buzzTimSt = true;
        key = 0;
      }
      if (key == 'D') {
        lcd.clear();
        buzzStop();
        stopAll();
      }
      //Serial.println(key);
      break;
  }
  if (page == 0) {
    mainPage();
  }
  /*if (page==1) {
    hashPage();}*/
    
  if (flagWork) {
    if (millis() - eeprTimer > 600000) {        // Каждые 10 минут сохраняем оставшееся время в EEPROM
      EEPROM_write (20, t);
      EEPROM_write (16, o);
      eeprTimer = millis();
    }
    
    lig = (millis() - startWork) % secund160;
    n = pressurePa - paNeed;
    
    if (lig % 5000 < 20) {
      /*Serial.println(lig); Serial.print("  n= ");Serial.println(n);*/
    }
    if (tNeed - tempC <= 0)  {
      hype = true;  //если до необходимой температуры осталось 2 градусов, включаем таймер
      back_timer = true;
      if (buzzTimSt) {
        buzzTimerStart();
        buzzTimSt = false;
      }
    }
    if (tempC - tNeed > 2) {
      digitalWrite (16, LOW); /*Serial.write("k2 off"); */startWork = millis() - 5000; //если температура перевалила за 3 градусов (от неободимой) выключаем 2й компрессор
    }
    if ((millis() - startWork) >= minute6 && (firstRun)) {
      digitalWrite (16, HIGH);  // включаем 2й компрессор по прошестии 6 минут от старта
      firstRun = false;
    }
    if ((!firstRun) && (millis() - startWork) % secund160 < 1000) {
      digitalWrite (16, HIGH);  //включаем 2й компрессор по прошествии 160 сек после предыдущего выключения
      Serial.println(lig);
    }
    digitalWrite (15, HIGH);                                                                      // включаем 1й компрессор при старте
    /*
      if (pressurePa - paNeed > 2.0) digitalWrite (17, HIGH);                                      // включаем вакуумный насос если давление выше необходимого
      else if (paNeed - pressurePa >= 2.5) digitalWrite (17, LOW);                                  // выключаем вакуумный насос если давление ниже необходимого
    */
  }
  
  backTimer ();

  if ((millis() - endWork >= 30000) && (hype)) hype = false;
  
  averageTemp ();
  averageTempHype ();
  averagePa ();

}
