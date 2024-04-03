#include <M5Stack.h>    //librería m5stack
#include "M5Stack.c"   //main menu image
#include "Chabots_Ocelot_Logo2.c"   //chabots logo

int estado=0;   /*estado de los botones.
                0=Menu principal
                1=setMode
                2=partido
                3=sub menu*/

float xLineaIR;   //variables para imprimir el ir
float yLineaIR;
float angleLineaIR;

float xLineaLS;   //variables para imprimir el line sensor
float yLineaLS;
float angleLineaLS;

void resetEstadoA(){    //funciones para leer el estado de los botones
  if(M5.BtnA.wasPressed()){
    if(estado==1) {
      estado=0;
    }
    else if(estado==0) {
      estado=1;
    }
  }
}

void resetEstadoB(){
  if(M5.BtnB.wasPressed()){
    if(estado==2) {
      estado=0;
    }
    else if(estado==0) {
      estado=2;
    }
  }
}

void resetEstadoC(){
  if(M5.BtnC.wasPressed()){
    if(estado==3) {
      estado=0;
    }
    else if(estado==0) {
      estado=3;
    }
  }
}

void setup() {
    M5.begin();        
    M5.Power.begin();   
    M5.Lcd.setTextColor(GREEN);  
    M5.Lcd.setTextSize(5);
    M5.Lcd.println("ocelot");
    delay(250);     
    Serial.begin(115200);
}

void setMode(){  //interfaz setMode (cero artificial)
  Serial.write(estado);
  if(estado==1){
    M5.Lcd.clear(BLACK);
    setText(1, WHITE, 0,0, "SETMODE");
    crearBarra("SET", BLUE,80,90,150,55);
    delay(100);
  }
  delay(20);
}


void partido(){   //interfaz partido
  Serial.write(estado);
  if(estado==2){
    M5.Lcd.clear(BLACK);
    setText(1, WHITE, 0,0, "PARTIDO");
    M5.Lcd.drawBitmap(50,0,240,240,(uint8_t *)Chabots_Ocelot_Logo2_map);
    delay(100);
  }
  delay(20);
}

void setText(int size, uint16_t color, int x, int y, String text){ //imprimir y settear texto (tamaño, color, x, y y texto)
    M5.Lcd.setTextSize(size);
    M5.Lcd.setCursor(x, y);
    M5.Lcd.setTextColor(color);
    M5.Lcd.print(text);
}

void crearBarra(String nombre, uint16_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){ //crear un rectangulo alrededor de un texto  (texto, color, x, y, ancho, alto)
    M5.Lcd.drawRect(x, y, w, h, color);    
    setText(5, WHITE, x+5, y+5, nombre);
}

bool mantener(){    //regresarse en el sub menu
    if(M5.BtnA.read())
        return 0;
    else
        return 1;
}

void printLsInfo(){   //imprimir informacion recibida del sensor de linea
  if(Serial.available()){
    angleLineaLS = Serial.read();
    angleLineaLS=angleLineaLS*2;
    //Serial.println(angleLineaLS);
    M5.Lcd.println(angleLineaLS);  
    }   
  xLineaLS=cos((angleLineaLS-90)*3.1416/180);
  yLineaLS=sin((angleLineaLS-90)*3.1416/180);
  M5.Lcd.drawCircle(160, 120, 100, BLUE);  
  M5.Lcd.drawLine(160, 120, xLineaLS*100+160, yLineaLS*100+120, WHITE);
}

void printIrInfo() {    //imprimir informacion recibida del IR
  if(Serial.available()){
    angleLineaIR = Serial.read();
    angleLineaIR=angleLineaIR*2;
    //Serial.println(angleLineaIR);
    M5.Lcd.println(angleLineaIR);  
  }
  xLineaIR=cos((angleLineaIR-90)*3.1416/180);
  yLineaIR=sin((angleLineaIR-90)*3.1416/180);
  M5.Lcd.drawCircle(160, 120, 100, BLUE);  // Draw a red circle of radius 50 at (x,y)
  M5.Lcd.drawLine(160, 120, xLineaIR*100+160, yLineaIR*100+120, WHITE);
}

void drawField(){   //dibujo de la cancha
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.drawRect(38,29,243,182,WHITE);
    M5.Lcd.drawRect(63,54,193,132,WHITE);
    //Neutral spots
    M5.Lcd.drawCircle(108,90,1,WHITE);
    M5.Lcd.drawCircle(108,150,1,WHITE);
    M5.Lcd.drawCircle(211,90,1,WHITE);
    M5.Lcd.drawCircle(211,150,1,WHITE);
    M5.Lcd.drawCircle(160,120,1,WHITE);
    M5.Lcd.drawCircle(160,120,30,WHITE);
    //porterias
    M5.Lcd.drawRect(55,90,8,60,BLUE);
    M5.Lcd.fillRect(55,90,8,60,BLUE);
    M5.Lcd.drawRect(256,90,8,60,YELLOW);
    M5.Lcd.fillRect(256,90,8,60,YELLOW);
    //AREA PENALTI
    M5.Lcd.drawLine(63,85,73,85,WHITE);
    M5.Lcd.drawLine(63,155,73,155,WHITE);
    M5.Lcd.drawCircleHelper(73, 100, 15, 2, WHITE);
    M5.Lcd.drawCircleHelper(73, 140, 15, 4, WHITE);
    M5.Lcd.drawLine(88,100,88,140,WHITE);
    //////
    M5.Lcd.drawLine(255,85,245,85,WHITE);
    M5.Lcd.drawLine(255,155,245,155,WHITE);
    M5.Lcd.drawCircleHelper(245, 100, 15, 1, WHITE);
    M5.Lcd.drawCircleHelper(245, 140, 15, 8, WHITE);
    M5.Lcd.drawLine(230,100,230,140,WHITE);
}

int posData[7][2]={   //array posicion del submenu
  {10,10},
  {160,10},
  {10,65},
  {160,65},
  {10,120},
  {160,120},
  {10,175}
};

int posDataLS[12][2]{
  {160.00,20.00},
  {210.00,33.40},
  {246.60,70.00},
  {260.00,120.00},	
  {246.60,170.00},
  {210.00,206.60},
  {160.00,220.00},
  {110.00,206.60},
  {73.40,170.00},
  {60.00,120.00},	
  {73.40,70.00},
  {110.00,33.40}        
};

void menuData(){    //interfaz sub menu
  M5.Lcd.clear(BLACK);
  setText(1, WHITE, 0,0, "DATOS");

  crearBarra("IR", BLUE, 10,10,150,55);   //tabla de datos
  crearBarra("IMU", BLUE, 160,10,150,55);
  crearBarra("LS", BLUE, 10,65,150,55);
  crearBarra("PWM", BLUE, 160,65,150,55);
  crearBarra("IRI", BLUE, 10,120,150,55);
  crearBarra("FLD", BLUE, 160,120,150,55);
  crearBarra("EXIT", BLUE, 10,175,150,55);
    
  int contador=-1;
  int estadoInterno = 1;

  while(mantener()){
    M5.update();
    if(M5.BtnB.wasReleased()){
      Serial.println("OK");
      switch (contador){
        case 0: //IR
          M5.Lcd.clear(BLACK);
          crearBarra("IR", BLUE, 80,90,150,55);
          delay(500);
          M5.Lcd.clear(BLACK);
          Serial.println("IR");
          printIrInfo();
          break;

        case 1: //IMU
          M5.Lcd.clear(BLACK);
          crearBarra("IMU", BLUE, 80,90,150,55);
          Serial.println("IMU");          
          break;

        case 2: //Line sensor
          M5.Lcd.clear(BLACK);
          crearBarra("LS", BLUE, 80,90,150,55);
          Serial.println("LS");
          delay(500);
          M5.Lcd.clear(BLACK);
          printLsInfo();
          break;

        case 3: //PWM
          M5.Lcd.clear(BLACK);
          crearBarra("PWM", BLUE, 80,90,150,55);
          Serial.println("PWM");
          break;
        
        case 4: //Intensidad IR
          M5.Lcd.clear(BLACK);
          crearBarra("IRI", BLUE, 80,90,150,55);
          Serial.println("IRI");
          break;

        case 5: //Cancha
        M5.Lcd.clear(BLACK);
        Serial.println("FLD");
        drawField();
        while(mantener()){
          Serial.println("Cancha MAMALONA");
        }
        break;

        case 6: //EXIT
          M5.Lcd.clear(BLACK);
          crearBarra("EXIT", BLUE, 80,90,150,55);
          Serial.println("EXIT");
          estado=0;
          break;
      }
    }   
    else if(M5.BtnC.wasReleased()){
      M5.Lcd.clear(BLACK);
      contador < 6 ? contador += 1 : contador = 0;
      setText(1, WHITE, 0,0, "DATOS");
      crearBarra("IR", BLUE, 10,10,150,55);
      crearBarra("IMU", BLUE, 160,10,150,55);
      crearBarra("LS", BLUE, 10,65,150,55);
      crearBarra("PWM", BLUE, 160,65,150,55);
      crearBarra("IRI", BLUE, 10,120,150,55);
      crearBarra("FLD", BLUE, 160,120,150,55);
      crearBarra("EXIT", BLUE, 10,175,150,55);
      M5.Lcd.fillRect(posData[contador][0], posData[contador][1], 150, 55, WHITE); 
      //Serial.println(contador);
      //Serial.println("");
      //Serial.println(mantener());   
    }
  }
}

void mainMenu(){    //interfaz menu principal
  M5.Lcd.clear(BLACK);
  M5.Lcd.drawRect(0, 200, 100, 25, BLUE);
  M5.Lcd.drawRect(115, 200, 95, 25, BLUE);
  M5.Lcd.drawRect(230, 200, 75, 25, BLUE);
  setText(2,WHITE,2,205,"set Mode");
  setText(2,WHITE,122,205,"PARTIDO");
  setText(2,WHITE,235,205,"Datos");
  M5.Lcd.drawBitmap(0,15,320,240,(uint8_t *)M5Stack_map);
}

void loop() {
  M5.update();
  Serial.println(mainMenu); 
  resetEstadoA();   //leemos los botones
  resetEstadoB();
  resetEstadoC();

  switch (estado) {
    case 1:   //boton A
      setMode();
      break;

    case 2:   //boton B
      partido();
      break;

    case 3: //boton C
      menuData();
      break;

    default:
      mainMenu();
      break;
  }
}