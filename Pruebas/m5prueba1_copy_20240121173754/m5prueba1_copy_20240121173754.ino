#include <M5Stack.h>
#include "M5Stack.c"
#include "Chabots_Ocelot_Logo2.c"

int estado=0;
int mainMenu=1;
float xLinea;
float yLinea;
float angleLinea;

void resetEstadoA(){
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

void setMode(){
  Serial.write(estado);
  M5.Lcd.setCursor(70,90);
  if(estado==1){
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(35,90);
    M5.Lcd.println("IMU");
    delay(100);
  }
  delay(20);
}


void partido(){
  Serial.write(estado);
  M5.Lcd.setCursor(70,90);
  if(estado==2){
   M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(80,90);
    M5.Lcd.println("jugando");
    delay(100);
  }
  delay(20);
}


void infoIR(){
  //delay(1);
  M5.Lcd.setCursor(30,90);
  if(estado==3){
    M5.Lcd.clear(BLACK);
    if(Serial.available()){
      angleLinea = Serial.read();
      angleLinea=angleLinea*2;
      //Serial.println(angleLinea);
      M5.Lcd.println(angleLinea);  
    }   
    xLinea=cos((angleLinea-90)*3.1416/180);
    yLinea=sin((angleLinea-90)*3.1416/180);
    M5.Lcd.drawCircle(160, 120, 100, GREEN);  // Draw a red circle of radius 50 at (x,y)
    M5.Lcd.drawLine(160, 120, xLinea*100+160, yLinea*100+120, GREEN);
  }
}

void setText(int size, uint16_t color, int x, int y, String text){
    M5.Lcd.setTextSize(size);
    M5.Lcd.setCursor(x, y);
    M5.Lcd.setTextColor(color);
    M5.Lcd.print(text);
}

void crearBarra(String nombre, uint16_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h){
    M5.Lcd.drawRect(x, y, w, h, color);    
    setText(5, WHITE, x+5, y+5, nombre);
}


bool mantener(){
    if(M5.BtnA.read())
        return 0;
    else
        return 1;
}


void drawField(){
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

        int posData[7][2]={
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

void menuData(){
    M5.Lcd.clear(BLACK);
    setText(1, WHITE, 0,0, "DATOS");
    /*crearBarra("IR", BLUE, 10,10,150,55);
    crearBarra("IMU", BLUE, 160,10,150,55);
    crearBarra("LS", BLUE, 10,65,150,55);
    crearBarra("US1", BLUE, 160,65,150,55);
    crearBarra("US2", BLUE, 10,120,150,55);
    crearBarra("US3", BLUE, 160,120,150,55);
    crearBarra("COM", BLUE, 10,175,150,55);*/
    
    crearBarra("IR", BLUE, 10,10,150,55);
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
                    Serial.println("IR");
                    
                    break;
                case 1: //IMU
                    Serial.println("IMU");
                    
                    break;
                case 2: //LS
                    Serial.println("LS");
                    
                    break;
                case 3: //US1
                    Serial.println("US1");
                    drawField();
                    
                    while(mantener()){
                        Serial.println("Cancha MAMALONA");
                    }
                    break;
                case 4: //US2
                    Serial.println("US2");
                    drawField();
                    while(mantener()){
                        Serial.println("Cancha MAMALONA");
                    }
                    break;
                case 5: //US3
                    Serial.println("US3");
                    drawField();
                    while(mantener()){
                        Serial.println("Cancha MAMALONA");
                    }
                    break;
                case 6: //EXIT
                    Serial.println("EXIT");
                    estado=0;

                    break;
            }
            
        } 
        
        else if(M5.BtnC.wasReleased()){
            M5.Lcd.clear(BLACK);
            //contador < 6 ? contador += 1 : contador = 0;
            contador < 6 ? contador += 1 : contador = 0;
            setText(1, WHITE, 0,0, "DATOS");
    /*crearBarra("IR", BLUE, 10,10,150,55);
    crearBarra("IMU", BLUE, 160,10,150,55);
    crearBarra("LS", BLUE, 10,65,150,55);
    crearBarra("US1", BLUE, 160,65,150,55);
    crearBarra("US2", BLUE, 10,120,150,55);
    crearBarra("US3", BLUE, 160,120,150,55);
    crearBarra("COM", BLUE, 10,175,150,55);*/

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

void loop() {
  M5.update();
  Serial.println(mainMenu); 
  resetEstadoA();
  resetEstadoB();
  resetEstadoC();

  /*if(Serial.available()){
    angleLinea = Serial.read();
    angleLinea=angleLinea*2;
  }*/
  switch (estado) {
    case 1:
      mainMenu=0;
      setMode();
      break;
    case 2:
      mainMenu=0;
      partido();
      M5.Lcd.drawBitmap(0,0,320,240,(uint8_t *)Chabots_Ocelot_Logo2_map);
      break;
    case 3:
      //infoIR();
      mainMenu=0;
      menuData();
      break;
    default:
      mainMenu=1;
      
        M5.Lcd.clear(BLACK);
        M5.Lcd.drawRect(0, 200, 100, 25, BLUE);
        M5.Lcd.drawRect(115, 200, 95, 25, BLUE);
        M5.Lcd.drawRect(230, 200, 75, 25, BLUE);
        setText(2,WHITE,2,205,"set Mode");
        setText(2,WHITE,122,205,"PARTIDO");
        setText(2,WHITE,235,205,"Datos");
        M5.Lcd.drawBitmap(0,0,320,240,(uint8_t *)M5Stack_map);
      //infoIR();
      
      break;
  }

}

