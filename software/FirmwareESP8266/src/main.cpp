/**
 * @author     Fernando Gambino
 * @license    Este proyecto se publica bajo la licencia MIT (MIT)
 * @copyright  Copyright (c) 2015 Electronica Gambin
 * @date       Jun 2018
 * @brief      Proyecto Final Curso PSOC 5LP
 *
 */

#include <Arduino.h>

/*************************************************************
/ * Comente esto para deshabilitar las impresiones y ahorrar espacio */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_MAX_SENDBYTES 256 //alargar el maxiomo del mensaje
// Debería obtener el token de autenticación en la aplicación Blynk.
// Vaya a Configuración del proyecto (icono de tuerca).

  char auth[] = "cbde13b152d3432da53ad5585f9eb550";//fmgambino

// Tus credenciales de WiFi.
// Establezca la contraseña en "" para redes abiertas.
//char ssid[] = "TESLA";
//char pass[] = "tesla0381";
char ssid[] = "TESLA";
char pass[] = "tesla0381";

char bandera[2];
int alert=0;
char tweets[256];
char dato[12];

bool bandraq=false;
bool banderaw=false;
bool banderax=false;
bool banderay=false;
bool banderaz=false;
bool banderat=false;
bool banderam=false;

//BLYNK_CONNECTED(){
  //Blynk.syncVirtual(V0);
  //Blynk.syncVirtual(V1);
  //Blynk.syncVirtual(V2);
  //Blynk.syncVirtual(V3);
//}

BLYNK_WRITE(V0) // V5 es el número de Virtual Pin Boton 1
{
  Serial.print("w");
}
BLYNK_WRITE(V1) // V5 es el número de Virtual Pin Boton 2
{
  Serial.print("x");
}

BLYNK_WRITE(V2) // V5 es el número de Virtual 
{
  Serial.print("y");
}

BLYNK_WRITE(V3) // V5 es el número de Virtual  
{
  Serial.print("z");
}

BLYNK_WRITE(V4) // V5 es el número de Virtual  
{
  int pinValue = param.asInt();
  
  //sprintf(dato,"S%d",pinValue);
  Serial.print("s");
  Serial.write(pinValue);
}

BLYNK_WRITE(V5) // V5 es el número de Virtual  
{
  int pinValue = param.asInt();
  Serial.print("h");
  Serial.write(pinValue);
}

BLYNK_WRITE(V6) // V5 es el número de Virtual  Sala
{
 int hora, minuto;
  int pinValue = param.asInt();
  hora=char(pinValue/3600);
  minuto=char((pinValue%3600)/60);
  Serial.print("i");  
  Serial.write(hora);//Envia parte alta
  Serial.write(minuto);//Envia parte baja 
}

BLYNK_WRITE(V7) // V5 es el número de Virtual Pin  
{
  int hora, minuto;
  int pinValue = param.asInt();
  hora=char(pinValue/3600);
  minuto=char((pinValue%3600)/60);
  Serial.print("f");  
  Serial.write(hora);//Envia parte alta
  Serial.write(minuto);//Envia parte baja 
}

BLYNK_WRITE(V8) // V5 es el número de Virtual Pin  
{
  int pinValue = param.asInt();
  Serial.print("m");
  Serial.write(pinValue);
}

void BlYNK_CONECTED_PSOC(int Puerto,int Hora,int Minuto)
{
 int tiempo=(Hora*3600)+(Minuto*60);
 Blynk.virtualWrite(Puerto,tiempo);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  Serial.print("*");
  delay(50);
  //BLYNK_CONNECTED();
}

void loop()
{
  Blynk.run();
  if(Serial.available()>0)
  {
    char in_serial=Serial.read();
    //Serial.print(in_serial);
    if(banderat){
      sprintf(tweets,"Alerta N%d de Temperatura",in_serial);
      Blynk.tweet(tweets);
      banderat=false;
      
    }
     if(banderam){
      sprintf(tweets,"Alerta N%d de Movimiento",in_serial);
      Blynk.tweet(tweets);
      banderam=false;
     }
     if (banderaw)
     {
      if(in_serial=='1')
      {
        Blynk.virtualWrite(V0,HIGH);
      }else{
        Blynk.virtualWrite(V0,LOW);
        }
       //BLYNK_CONNECTED();
       banderaw=false;
     }
     if(banderax)
     {
      if(in_serial=='1')
      {
        Blynk.virtualWrite(V1,HIGH);
      }else{
        Blynk.virtualWrite(V1,LOW);
        }
       //BLYNK_CONNECTED(); 
       banderax=false;
     }
     if(banderay)
     {
      if(in_serial=='1')
      {
        Blynk.virtualWrite(V2,HIGH);
      }else{
        Blynk.virtualWrite(V2,LOW);
        }
       //BLYNK_CONNECTED(); 
       banderay=false;
     }
     if(banderaz)
     {
     if(in_serial=='1')
      {
        Blynk.virtualWrite(V3,HIGH);
      }else{
        Blynk.virtualWrite(V3,LOW);
        }
       //BLYNK_CONNECTED(); 
       banderaz=false;
     }
     if(bandraq)
     {
      Blynk.virtualWrite(V4,in_serial);
       bandraq=false;
     }
///////////////banderas de estados;
     
    if (in_serial=='t')
    {
      banderat=true;
    }else if(in_serial=='m'){
      banderam=true;
      
    }else if(in_serial=='W'){
      banderaw=true;       
    }
     else if(in_serial=='X'){
      banderax=true;  
     }
     else if(in_serial=='Y'){
      banderay=true;
        }
     else if(in_serial=='Z'){
       banderaz=true;
        }else if(in_serial=='Q'){
       bandraq=true;
        }
  }
  
}