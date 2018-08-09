
#include "LiquidCrystal.h"

LiquidCrystal lcd(7,8,9,10,11,12);

// constante pour le voltmetre
float input_voltage1 = 0.0;
float input_voltage2 = 0.0;

int sensorPin1 = A1;
int sensorPin2 = A2;

float r1=10.2;
float r2=5.0;

 // decalration de variable pour le bouton
int inPin = A0;         // the number of the input pin
int iSetup = 0;      // the current state of the output pin
int precision = 0;





void setup()
{
   Serial.begin(9600);     //  opens serial port, sets data rate to 9600 bps
   lcd.begin(16, 2);       //// set up the LCD's number of columns and rows: 
   // setup par defaut
   lcd.setCursor(0,0);
   lcd.print("Appuyer derriere");
   lcd.setCursor(0,1);
   lcd.print("changer reglages");
   delay(4000);
   lcd.clear();
   lcd.print("Reglage numero ");
   lcd.print(iSetup);
   if (iSetup==0){
     lcd.setCursor(0,1);
     lcd.print("Jauges Simon");
   }
    delay(2000);
    lcd.clear();
   
   pinMode(inPin, INPUT);
}



// ###################################### 
// ############## MAIN CODE #############
// ###################################### 
void loop()
{  
  // lecture et conversion en V des deux voltage
  // aux pin A0 et A1
  input_voltage1 =  Lecture(r1,r2,sensorPin1);
  input_voltage2 =  Lecture(r1,r2,sensorPin2);

  // ##### Change les setups si on clique sur le bouton #####


    PressSwitch(inPin, &iSetup);


    
    Manipulation(&iSetup, &input_voltage1, &input_voltage2,&precision);

   // ##### Affiche sur l'écran les voltages #####
  Affichage(input_voltage1, input_voltage2, iSetup, &precision); // TODO mettre une varible qui dit quoi afficher selon le choix de setup

  delay(300); // délai d'affichage
}





float Lecture(float r1,float r2, int sensorPin)
{
  // float seuil = 0.1;
   float input_voltage=0.0;
   int analog_value = analogRead(sensorPin); //ToDo
   
   input_voltage = (analog_value * 5.0) / 1024.0; //5v sur le nombre de bit
   input_voltage = input_voltage / (r2/(r1+r2));   // diviseur de tension

  // if (input_voltage1 < seuil) 
  // {
  //   input_voltage=0.0;
  // } 
   
   return input_voltage;  
}


void Affichage(float input_voltage1, float input_voltage2, int iSetup, int *Pprecision)
{
    // Affiche les deux voltages
    lcd.setCursor(0,0);
    lcd.print("Amont=");
    lcd.print(input_voltage1,*Pprecision);
    if (iSetup==1){
     lcd.print(" V");
    }
    else{
      lcd.print(" mTorr");
    }
    lcd.setCursor(0, 1);
    lcd.print(" Aval=");
    lcd.print(input_voltage2,*Pprecision);
    if (iSetup==1){
      lcd.print(" V");
    }
    else {
      lcd.print(" mTorr");
    }
  }


void PressSwitch(int inPin, int *piSetup)
{
    int reading; 
    reading = digitalRead(inPin); // the current reading from the input pin
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
    if (reading == HIGH ) {
    *piSetup=*piSetup+1; // on incrémente setup de 1
    if (*piSetup >3){ *piSetup = 0;} // si il passe le 2, on le met à 0 (setup = 0,1,2)
    //time = millis();    //dont know..

    // Affiche la seclection
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reglage numero ");
    lcd.print(iSetup);
    switch (iSetup){
    case 0:  
      lcd.setCursor(0,1);
      lcd.print("Jauges Simon");
      break;
    case 1:  
      lcd.setCursor(0,1);
      lcd.print("Voltages");
      break;
    case 2:  
      lcd.setCursor(0,1);
      lcd.print("Jauges 0.1 Torr");
      break;
    case 3:  
      lcd.setCursor(0,1);
      lcd.print("Jauges 1 Torr");
      break;
    }
    delay(2000);
    lcd.clear();
    
    return 0;
  }
}
  
  

void Manipulation(int *PiSetup, float *V1, float *V2, int *Pprecision)
{
  switch(*PiSetup){
    case 0:
    // Simon, jaugse 1Torr avec des facteurs de calibrations (1.4 et 1.88)
      *Pprecision = 0;
      *V1 = (*V1) *140.0;
      *V2 = (*V2) *188.0;
      break;
    case 1:
      //Reste en Volt donc rien!
      *Pprecision = 2;
      break;
    case 2:
      // Jauges 0.1 Torr
      *Pprecision = 2;
      *V1 = (*V1) *10;
      *V2 = (*V2) *10;
      break;
    case 3:
      // Jauges 1 Torr
      *Pprecision = 0;
      *V1 = (*V1) *100;
      *V2 = (*V2) *100;
      break;
  }
  return 0;
  
}
