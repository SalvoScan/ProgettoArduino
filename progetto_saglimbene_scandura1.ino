#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {0,6,5,4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3,2,8,7}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal lcd(13, A1, A2, A3, A4, A5);  
  
char ins_pass[4];
int i = 0;
int flag = 0;
char password[] = {'9','6','9','6'};
int count_pass = 3;

int temp = A0;
int val_temp = 0;
int t = 0;

int ventola = 9;
int v1 = 0;
int v0 = 0;

int buzz = 10;

int trig = 12;
int echo = 11;

long durata = 0;
long dist = 0;

int soglia = 20; // 20 centimetri

int contatore = 0;
int k = 0;

void setup() {  
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  
  pinMode(temp, INPUT);
  
  pinMode(ventola, OUTPUT);
  
  pinMode(buzz, OUTPUT);
  
  pinMode(trig, OUTPUT);
  
  pinMode(echo, INPUT);
}  
  
void loop() {  
 
  while(flag == 0){
    lcd.setCursor(0, 0);
    lcd.print("INSERIRE PASS");
    
  	char c = keypad.getKey();
    if(c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0' || c == '*' || c == '#' || c == 'A' || c == 'B' || c == 'C' || c == 'D'){
    	ins_pass[i] = c;
      	
      	lcd.setCursor(i, 1);
      	lcd.print("*");
      
      	i += 1;
    }
    
    if(i == 4){
      	lcd.clear();
      	
      
      	for(int j = 0; j < 4; ++j){
          if(ins_pass[j] != password[j]){
            lcd.clear();
            
          	lcd.setCursor(0, 0);
            lcd.print("ACCESSO NEGATO");
            
            lcd.setCursor(0, 1);
            
            count_pass -= 1;
            
            if(count_pass == 0){
            	lcd.clear();
              
              	lcd.setCursor(1, 0);
              	lcd.print("ACCESSO NEGATO");
              
              	lcd.setCursor(1, 1);
              	lcd.print("PORTA BLOCCATA");
              
                tone(buzz, 2000, 350);
              	
              	flag = -1;
              
              	break;
            }
            
            lcd.print(count_pass);
            
            lcd.setCursor(2, 1);
            lcd.print("Tentativi 3sec");
            
            tone(buzz, 2000, 350);
            
            delay(3000);
            
            lcd.clear();
            
            break;
          }
          if(j == 3 && ins_pass[j] == password[j]){
          	lcd.clear();
            
            lcd.setCursor(3, 0);
            lcd.print("BENVENUTI");
            
            lcd.setCursor(4, 1);
            lcd.print("RAGAZZI");
            
            tone(buzz, 1000, 250);
            delay(500);
            tone(buzz, 1000, 250);
            
            flag = 1;
          }
          
      	}//End For
      	
		i = 0;
      
    }//End if(i == 4)
    
    delay(50);
  }
 
  if(flag == -1){
  	Serial.print("PORTA BLOCCATA\n");
    Serial.print("Chiamare Operatore\n\n");
    
    delay(5000);
    
    Serial.print("Sto terminando...");
    
    lcd.clear();
    
    lcd.setCursor(2, 0);
    lcd.print("Terminazione");
    
    lcd.setCursor(2, 1);
    lcd.print("in corso...");
    
    while(true){}
    
    flag = 6;
  }
  
  if(flag == 1){
  	delay(3000);
    
    lcd.clear();
    
    lcd.setCursor(2, 0);
    lcd.print("TEMPERATURA");
    
    val_temp = analogRead(temp);
    
    t = ((val_temp*0.00488) - 0.5)/0.01;
    
    lcd.setCursor(5, 1);
    lcd.print(t);
    
    lcd.setCursor(8, 1);
    lcd.print("gradi");
    
    flag = 6;
  }
  
  //if(flag == 6): CONTINUE
    
  lcd.clear();
  
  lcd.setCursor(2, 0);
  lcd.print("TEMPERATURA");
    
  val_temp = analogRead(temp);
    
  t = ((val_temp*0.00488) - 0.5)/0.01;
    
  lcd.setCursor(3, 1);
  lcd.print(t);
    
  lcd.setCursor(8, 1);
  lcd.print("gradi");
  
  if(v0 == 0){
    if(t > 30){
      Serial.print("STATO INIZIALE : ");
      Serial.print("VENTOLA ACCESA\n\n");
      digitalWrite(ventola, HIGH);
      v0 = 1;
    }
    else{
      Serial.print("STATO INIZIALE : ");
      Serial.print("VENTOLA SPENTA\n\n");
      digitalWrite(ventola, LOW);
      v0 = 1;
    }
  }
  
  if(t > 30){
    if(v1 == 0){
      Serial.print("VENTOLA ACCESA\n\n");
      v1 = 1;
    }
    digitalWrite(ventola, HIGH);
  }
  if(t < 25){
  	if(v1 == 1){
      Serial.print("VENTOLA SPENTA\n\n");
      v1 = 0;
    }
    digitalWrite(ventola, LOW);
  }
  
  digitalWrite(trig, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 

  digitalWrite(trig, LOW);
  
  durata = pulseIn(echo, HIGH);
  
  dist = durata/450;
  
  if(dist > soglia){
  	contatore += 1;
  }
  
  Serial.print("Distanza = ");
  Serial.print(dist);
  Serial.print(" cm\n");
  
  if(contatore != 0 && contatore % 2 == 0){
    k += 1;
    Serial.print("Time : ");
    Serial.print(k);
    Serial.print(" sec\n\n");
  }
  if(dist < soglia){
  	contatore = 0;
    k = 0;
  }
  
  // Problemi Temperatura
  //   |
  //   V
  /*if(k != 0 && k % 10 == 0){
  	//tone(buzz, 2000, 400);
    Serial.print("SUONA\n\n");
  }*/
  
  delay(500);
}  






