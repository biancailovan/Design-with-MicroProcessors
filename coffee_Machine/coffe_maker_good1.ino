#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);
Servo myservo;
int pos = 0, i; 
const byte rows = 4;
const byte cols = 4;

char key[rows][cols] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte rowPins[rows] = {1,2,3,4};
byte colPins[cols] = {5,6,7};
Keypad keypad = Keypad(makeKeymap(key),rowPins,colPins,rows,cols);
int currentposition = 0;
int redled = 10;
int buzz = 8;
int invalidcount = 12;
char* sum = "060";

/* pretty print, functie folosita in setup */
void display_screen(){  
lcd.setCursor(0,0);
lcd.println("Enter the price");
lcd.setCursor(1 ,1);
lcd.println("for one coffee!");
}

/* functia de setup, pentru setarile initiale si mesajele initiale de pe LCD*/
void setup(){  
lcd.begin(16,2);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Setup is started");
lcd.setCursor(0,1);
lcd.print("Please wait...");
delay(1500);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Drink machine");
lcd.setCursor(0,1);
lcd.print("is initialized");
delay(1500);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("To start, you'll");
lcd.setCursor(0,1);
lcd.print("enter the price.");
delay(1500);
  
display_screen(); /*afiseaza*/
Serial.begin(9600);
pinMode(redled, OUTPUT);
pinMode(buzz, OUTPUT);
/*atasare servo*/
myservo.attach(9); 
lcd.begin(16,2);
}

/*functie clear screen*/
void clearscreen(){
lcd.setCursor(0,0);
lcd.println(" ");
lcd.setCursor(0,1);
lcd.println(" ");
lcd.setCursor(0,2);
lcd.println(" ");
lcd.setCursor(0,3);
lcd.println(" ");
}

void loop(){

if( currentposition == 0){
	display_screen();
}

char code = keypad.getKey();
if(code != NO_KEY){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Coffee payment:");
	lcd.setCursor(7,1);
	lcd.print(" ");
	lcd.setCursor(7,1);
  
	for(i = 0; i <= currentposition; i++){
		keypress();
	}
  
   /*in cazul in care utilizatorul a introdus suma corecta necesara prepararii unei cafele
   va fi afisat pe LCD un mesaj informativ in asa fel incat sa se observe ca se prepara cafeaua*/
	if (code == sum[currentposition] && digitalRead(12)==HIGH){
		++currentposition;
		if(currentposition == 3){
			unlock();
          	lcd.print("Preparing coffee");
			lcd.setCursor(0, 1);
			myservo.write(90);
			for (int i=0; i<= currentposition; i++) {
				lcd.print(".");
				delay(500);
			}
          /*mai pe urma apare mesajul de prelevare a cafelei*/
			myservo.write(0);
			lcd.clear();
			lcd.print("Take your coffee");
			delay(4000);
			lcd.clear();
			currentposition = 0;
		}
	}
    
  /*in cazul in care utlizatorul nu introduce suma corecta se va apela functia incorrect_sum ce il va 
  informa ca suma nu este cea potrivita*/
	else{
		++invalidcount;
		incorrect_sum();
		currentposition = 0;
	}
  
}
}

/*pentru ca servo ul sa se deblocheze la introducerea sumei corecte*/
void unlock(){
delay(900);
lcd.setCursor(0,0);
lcd.println(" ");
lcd.setCursor(1,0);
lcd.print("Transaction approved");
lcd.setCursor(4,1);
lcd.println("TAKE COFFEE");
//counter();
lcd.setCursor(15,1);
lcd.println(" ");
lcd.setCursor(16,1);
lcd.println(" ");
lcd.setCursor(14,1);
lcd.println(" ");
lcd.setCursor(13,1);
lcd.println(" ");
  
unlockbuzz();

/* de la 0 la 80*/
for(pos = 180; pos>=0; pos-=5){
	myservo.write(pos); //in ce pozitie trebuie sa ajunga
	delay(5); //asteapta pt a ajunge in pozitia respectiva
}
  
delay(2000);
delay(1000);
counter();
delay(1000);

/* de la 0 la 80*/
for(pos = 0; pos <= 180; pos +=5){ 
	myservo.write(pos); //in ce pozitie trebuie sa ajunge
	delay(15);
	currentposition=0;
	lcd.clear();
	display_screen();
}
}

/*in cazul in care suma introdusa de user nu e cea prestabilita, adica 0,60*/
void incorrect_sum(){
delay(500);
lcd.clear();
lcd.setCursor(1,0);
lcd.print("SUM");
lcd.setCursor(6,0);
lcd.print("INCORRECT");
lcd.setCursor(15,1);
lcd.println(" ");
lcd.setCursor(4,1);
lcd.println("Price: 0,60");

lcd.setCursor(13,1);
lcd.println(" ");
Serial.println("INCORRECT SUM");
digitalWrite(redled, HIGH);
digitalWrite(buzz, HIGH);
delay(3000);
lcd.clear();
digitalWrite(redled, LOW);
digitalWrite(buzz,LOW);
display_screen();
}


/* pentru ca buzzer-ul sa sune la apasarea unei taste */
void keypress(){
digitalWrite(buzz, HIGH);
delay(50);
digitalWrite(buzz, LOW);
}

void unlockbuzz(){
digitalWrite(buzz, HIGH);
delay(80);
digitalWrite(buzz, LOW);
delay(80);
digitalWrite(buzz, HIGH);
delay(80);
digitalWrite(buzz, LOW);
delay(200);
digitalWrite(buzz, HIGH);
delay(80);
digitalWrite(buzz, LOW);
delay(80);
digitalWrite(buzz, HIGH);
delay(80);
digitalWrite(buzz, LOW);
delay(80);
}

/*Pentru numararea secundelor in care iti vei putea lua cafeaua, pe LCD va aparea o numaratoarea sub
forma unui cronometru ce se va decrementa*/
void counter(){
delay(1200);
lcd.clear();
digitalWrite(buzz, HIGH);

lcd.setCursor(2,15);
lcd.println(" ");
lcd.setCursor(2,14);
lcd.println(" ");
lcd.setCursor(2,0);
delay(200);
lcd.println("GET YOUR COFFEE:");

lcd.setCursor(4,1);
lcd.print("5");
delay(200);
lcd.clear();
lcd.setCursor(2,0);
lcd.println("GET YOUR COFFEE:");
digitalWrite(buzz,LOW);
delay(1000);

digitalWrite(buzz, HIGH);
lcd.setCursor(2,0);
lcd.println("GET YOUR COFFEE:");
lcd.setCursor(4,1); //2
lcd.print("4");
delay(100);
lcd.clear();
lcd.setCursor(2,0);
lcd.println("GET YOUR COFFEE:");
digitalWrite(buzz,LOW);
delay(1000);

digitalWrite(buzz, HIGH);
lcd.setCursor(2,0);
lcd.println("GET YOUR COFFEE:");
lcd.setCursor(4,1); //3
lcd.print("3");
delay(100);
lcd.clear();
lcd.setCursor(2,0);
lcd.println("GET YOUR COFFEE:");
digitalWrite(buzz,LOW);
delay(1000);

digitalWrite(buzz, HIGH);
lcd.setCursor(2,0);
lcd.println("GET YOUR COFFEE:");
lcd.setCursor(4,1); //4
lcd.print("2");
delay(100);
lcd.clear();
lcd.setCursor(2,0);
lcd.println("GET YOUR COFFEE:");
digitalWrite(buzz,LOW);
delay(1000);

digitalWrite(buzz, HIGH);
lcd.setCursor(4,1);
lcd.print("1");
delay(100);
lcd.clear();
lcd.setCursor(2,0);
lcd.println("GET YOUR COFFEE:");
digitalWrite(buzz,LOW);
delay(1000);

digitalWrite(buzz, HIGH);
delay(40);
digitalWrite(buzz,LOW);
delay(40);
digitalWrite(buzz, HIGH);
delay(40);
digitalWrite(buzz,LOW);
delay(40);
digitalWrite(buzz, HIGH);
delay(40);
digitalWrite(buzz,LOW);
delay(40);
digitalWrite(buzz, HIGH);
delay(40);
digitalWrite(buzz,LOW);
lcd.clear();
lcd.setCursor(2,0);
lcd.print("RE-INITIALIZING");
delay(500);
lcd.setCursor(12,0);
lcd.print(".");
delay(500);
lcd.setCursor(13,0);
lcd.print(".");
delay(500);
lcd.setCursor(14,0);
lcd.print(".");
delay(400);
lcd.clear();
lcd.setCursor(4,0);
lcd.print("READY!");
delay(440);
}

