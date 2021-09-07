/**
 * @author     Fernando Gambino
 * @license    Este proyecto se publica bajo la licencia MIT (MIT)
 * @copyright  Copyright (c) 2015 Electronica Gambin
 * @date       Jun 2018
 * @brief      Proyecto Final Curso PSOC 5LP
 *
 */

#include "project.h"
#include <stdio.h>
#include <stdbool.h>

#define DS1307_dir 0x68         
#define ds1307_dir_memory 0x00
#define direccion_de_registro_control 0x07
#define registro_control 0b10010001


volatile bool bandera = false;
volatile bool banderaM = false;
volatile bool banderaI = false;
volatile bool banderaF = false;
volatile bool banderaS = false;
volatile bool banderaH = false;
volatile bool cont=false;
volatile bool item1[4]={0,0,0,0};
volatile bool item2[4]={0,0,0,0};
volatile bool item3[4]={0,0,0,0};
volatile bool item4[4]={0,0,0,0};
volatile uint16 temp2=0;
volatile char item=0;


typedef union
{struct{
  char sec;
  char min;
  char hour;
  char weekDay;
  char date;
  char month;
  char year;  
};
char datos[8];
}rtc_t;

rtc_t ds;

volatile char wHoraInicio[3];
volatile char xHoraInicio[3];
volatile char yHoraInicio[3];
volatile char zHoraInicio[3];

volatile char wHoraFin[3];
volatile char xHoraFin[3];
volatile char yHoraFin[3];
volatile char zHoraFin[3];

uint16 aux[2]={0,0};

void DS_begintx (void){
        do{
                //Espera mienstras el esclavo le responde
        }while(I2C_MasterSendStart(DS1307_dir, I2C_WRITE_XFER_MODE)!=I2C_MSTR_NO_ERROR);
}

void DS_init(void){
    DS_begintx();
    I2C_MasterWriteByte(direccion_de_registro_control);  
    I2C_MasterWriteByte(registro_control);
    I2C_MasterSendStop();
}

void DS_set_data(){
    uint8 i; 
    
    for(i=0;i<=7;i++){
        DS_begintx();
        I2C_MasterWriteByte(i);     // Escribe la posicion 
        I2C_MasterWriteByte(ds.datos[i]);            // Escribe el dato correspondiente
        I2C_MasterSendStop();
    }
}

void DS_get_data(){        
        uint8 i;
    for(i=0;i<=7;i++){
        DS_begintx();
        I2C_MasterWriteByte(i);//Pone direccion de memoria que quiere leer 
        I2C_MasterSendRestart(DS1307_dir, I2C_READ_XFER_MODE); // Re transmite para obtener datos
        ds.datos[i]=I2C_MasterReadByte(I2C_NAK_DATA);
        I2C_MasterSendStop();
    }
    
}

CY_ISR(InterrupRx){
    char dato;
    dato=UART_GetChar();//recibe el dato del bluetooth
    if(bandera==true){
        switch (dato){
            case 'w':{
                PINA_Write(~PINA_Read());            
                break;
            }
            case 'x':{ 
                PINB_Write(~PINB_Read());
                break;
            }
            case 'y':{
                PINC_Write(~PINC_Read());            
                break;
            }
            case 'z':{
                PIND_Write(~PIND_Read());
                break;
            }
            case 's':{
                LCD_Position(0,0);
                LCD_PrintString("Hall    %");
                banderaS=true; 
                break;
            }
            case 'h':{   
                LCD_Position(0,0);
                LCD_PrintString("Room    %");
                banderaH=true; 
                break;
            }
            case 'i':{
                LCD_Position(0,0);
                LCD_PrintString("ON        ");
                banderaI=true;
                break;
            }
            case 'f':{
                LCD_Position(0,0);
                LCD_PrintString("OFF       ");
                banderaF=true;
                break;
            }
            case 'm':{
                LCD_Position(0,0);
                LCD_PrintString("Select:     ");
                banderaM=true;
                break;
            }
            default:
            {
                if (banderaS==true){
                    char dimmer=((255*dato)/100);
                    LCD_Position(0,6);
                    LCD_PrintNumber(dato);
                    PWM_WriteCompare2(dimmer);
                    banderaS=false;
                }
                if (banderaH==true){
                    char dimmer=((255*dato)/100);
                    LCD_Position(0,6);
                    LCD_PrintNumber(dato);
                    PWM_WriteCompare1(dimmer);
                    banderaH=false;
                }
                if (banderaI==true){
                    if(cont==true){
                        if(dato>9){//Solo para impresion
                            LCD_PrintNumber(dato);
                        }else{
                            LCD_PrintNumber(0);
                            LCD_PrintNumber(dato);
                        }
                        banderaI=false;
                        cont=false;
                        //Transpaso de datos
                        char a;
                        a=((dato/10)<<4)+dato%10;
                        switch(item){
                            case 1:{
                                wHoraInicio[1]=a;
                                if(dato==0){//Condicion de reset
                                    item1[1]=false;
                                }
                                else{
                                    item1[1]=true;
                                }
                                break;
                            }
                            case 2:{
                                xHoraInicio[1]=a;
                                if(dato==0){//Condicion de reset
                                    item2[1]=false;
                                }
                                else{
                                    item2[1]=true;
                                }
                                break;
                            }
                            case 3:{
                                yHoraInicio[1]=a;
                                if(dato==0){//Condicion de reset
                                    item3[1]=false;
                                }
                                else{
                                    item3[1]=true;
                                }                               
                                break;
                            }
                            case 4:{
                                zHoraInicio[1]=a;
                                if(dato==0){//Condicion de reset
                                    item4[1]=false;
                                }
                                else{
                                    item4[1]=true;
                                }                                
                                break;
                            }
                            default:{
                                break;}
                        }
                        
                    }else{
                        LCD_Position(0,5);
                        LCD_PrintNumber(dato);
                        LCD_PutChar(':');
                        cont=true;
                        //Poner datos
                        char a;
                        a=((dato/10)<<4)+dato%10;
                        switch(item){
                            case 1:{
                                wHoraInicio[2]=a;
                                if(dato==0){//Condicion de reset
                                    item1[0]=false;
                                }
                                else{
                                    item1[0]=true;
                                }                                
                                break;
                            }
                            case 2:{
                                xHoraInicio[2]=a;
                                if(dato==0){//Condicion de reset
                                    item2[0]=false;
                                }
                                else{
                                    item2[0]=true;
                                }  
                                break;
                            }
                            case 3:{
                                yHoraInicio[2]=a;
                                if(dato==0){//Condicion de reset
                                    item3[0]=false;
                                }
                                else{
                                    item3[0]=true;
                                }  
                                break;
                            }
                            case 4:{
                                zHoraInicio[2]=a;
                                if(dato==0){//Condicion de reset
                                    item4[0]=false;
                                }
                                else{
                                    item4[0]=true;
                                }                                  
                                break;
                            }
                            default:{
                                break;}
                        }
                        
                    }
                    
                }
                if (banderaF==true){
                    if(cont==true){
                        if(dato>9){
                            LCD_PrintNumber(dato);
                        }else{
                            LCD_PrintNumber(0);
                            LCD_PrintNumber(dato);
                        }
                        banderaF=false;
                        cont=false;
                        //Transpaso de datos
                        char a;
                        a=((dato/10)<<4)+dato%10;
                        switch(item){
                            case 1:{
                                wHoraFin[1]=a;
                                if(dato==0){//Condicion de reset
                                    item1[3]=false;
                                }
                                else{
                                    item1[3]=true;
                                }
                                break;
                            }
                            case 2:{
                                xHoraFin[1]=a;
                                if(dato==0){//Condicion de reset
                                    item2[1]=false;
                                }
                                else{
                                    item2[1]=true;
                                }
                                break;
                            }
                            case 3:{
                                yHoraFin[1]=a;
                                if(dato==0){//Condicion de reset
                                    item3[3]=false;
                                }
                                else{
                                    item3[3]=true;
                                }
                                break;
                            }
                            case 4:{
                                zHoraFin[1]=a;
                                if(dato==0){//Condicion de reset
                                    item4[3]=false;
                                }
                                else{
                                    item4[3]=true;
                                }                                
                                break;                               
                            }
                            default:{
                                break;}
                        }
                    }else{
                        LCD_Position(0,5);
                        LCD_PrintNumber(dato);
                        LCD_PutChar(':');
                        cont=true;
                        //Poner datos
                        char a;
                        a=((dato/10)<<4)+dato%10;
                        switch(item){
                            case 1:{
                                wHoraFin[2]=a;
                                if(dato==0){//Condicion de reset
                                    item1[2]=false;
                                }
                                else{
                                    item1[2]=true;
                                }
                                break;
                            }
                            case 2:{
                                xHoraFin[2]=a;
                                if(dato==0){//Condicion de reset
                                    item2[2]=false;
                                }
                                else{
                                    item2[2]=true;
                                }
                                break;
                            }
                            case 3:{
                                yHoraFin[2]=a;
                                if(dato==0){//Condicion de reset
                                    item3[2]=false;
                                }
                                else{
                                    item3[2]=true;
                                }
                                break;
                            }
                            case 4:{
                                zHoraFin[2]=a;
                                if(dato==0){//Condicion de reset
                                    item4[2]=false;
                                }
                                else{
                                    item4[2]=true;
                                }
                                break;
                               
                            }
                            default:{
                                break;}
                        }
                        
                    }
                }
                if (banderaM==true){
                    LCD_Position(0,8);
                    LCD_PrintNumber(dato);// muestra item
                    item=dato;
                    banderaM=false;
                }
                break;
            }
    }
    }else{
        if(dato=='*'){
            LCD_Position(0,0);
            LCD_PrintString("Conect");//Se detecta la ultima linea del inicio
            bandera=true;
        }    
    }
}

void reloj(){
  
        LCD_Position(1,4);
        LCD_PrintNumber(0x03&(ds.hour>>4));
        LCD_PrintNumber((0b00001111)&ds.hour);
        LCD_PutChar(':');
        LCD_PrintNumber(ds.min>>4);
        LCD_PrintNumber((0b00001111)&ds.min);
        LCD_PutChar(':');       
        LCD_PrintNumber(ds.sec>>4);
        LCD_PrintNumber((0b00001111)&ds.sec);
}

void muestreo(){
    //Temperatura
    uint16 temp=0;
    aux[0]=aux[1];//Actualiza dato pasado
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    temp=ADC_GetResult16();
    aux[1]=ADC_CountsTo_mVolts(temp);
    if((2500>aux[0])&&(aux[1]>=2500)){//Nueva medida es mayor a 2500 y la anterior era menor 2500 ->50°
        char t[1];
        temp2=temp2+1;//Aumenta el numero de alertas
        LCD_Position(0,0);
        LCD_PrintString("Warning T #");
        LCD_PrintNumber(temp2);
        UART_PutString("t");
        t[0]=(0xFF&temp2);
        UART_PutString(t);
        
    }
    temp=EEPROM_ReadByte(0);//Actualiza aca ya que en la interupcion no se puede
    if(temp2!=temp){
        if(temp2>temp){
            EEPROM_WriteByte(temp2,0);
        }else{
            temp2=temp;
        }    
    }
    
}

void comparacion(){
    char a[3]={};
    if(item1[0]||item1[1]){
//        LCD_Position(0,0);
//        LCD_PrintString("Puede imprimir");
        if((wHoraInicio[2]==((ds.hour)&(0b00111111)))&(wHoraInicio[1]==ds.min)&(ds.sec==0x0)){
            PINA_Write(1);
            sprintf(a,"w%d",1);
            UART_PutString(a);   
        }
    }
    if(item1[2]||item1[3]){
        if((wHoraFin[2]==((ds.hour)&(0b00111111)))&(wHoraFin[1]==ds.min)&(ds.sec==0x0)){
            PINA_Write(0);
            
            sprintf(a,"w%d",0);
            UART_PutString(a);   
        }
    }
    if(item2[0]||item2[1]){
        if((xHoraInicio[2]==((ds.hour)&(0b00111111)))&(xHoraInicio[1]==ds.min)&(ds.sec==0x0)){
            PINB_Write(1);
            
            sprintf(a,"x%d",1);
            UART_PutString(a);   
        }
    }
    if(item2[2]||item2[3]){
        if((xHoraFin[2]==((ds.hour)&(0b00111111)))&(xHoraFin[1]==ds.min)&(ds.sec==0x0)){
            PINB_Write(0);
            
            sprintf(a,"x%d",0);
            UART_PutString(a);   
        }
    }
    if(item3[0]||item3[1]){
        if((yHoraInicio[2]==((ds.hour)&(0b00111111)))&(yHoraInicio[1]==ds.min)&(ds.sec==0x0)){
            PINC_Write(1);
            
            sprintf(a,"y%d",1);
            UART_PutString(a);   
        }
    }
    if(item3[2]||item3[3]){
        if((yHoraFin[2]==((ds.hour)&(0b00111111)))&(yHoraFin[1]==ds.min)&(ds.sec==0x0)){
        PINC_Write(0);
        
            sprintf(a,"y%d",0);
            UART_PutString(a);   
        }
    }
    if(item4[0]||item4[1]){
        if((zHoraInicio[2]==((ds.hour)&(0b00111111)))&(zHoraInicio[1]==ds.min)&(ds.sec==0x0)){
        PIND_Write(1);
        
            sprintf(a,"z%d",1);
            UART_PutString(a);   
        }
    }
    if(item4[2]||item4[3]){        
        if((zHoraFin[2]==((ds.hour)&(0b00111111)))&(zHoraFin[1]==ds.min)&(ds.sec==0x0)){
        PIND_Write(0);
        
            sprintf(a,"z%d",0);
            UART_PutString(a);   
        }
    }

}

CY_ISR(InterrupISR){
    char m[1];
        temp2=temp2+1;//Aumenta el numero de alertas
        LCD_Position(0,0);
        LCD_PrintString("Warning M #");
        LCD_PrintNumber(temp2);
        UART_PutString("m");
        m[0]=(0xFF&temp2);
        UART_PutString(m);
        PINMOV_ClearInterrupt();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    IRQRX_StartEx(InterrupRx);
    ISR_MOV_StartEx(InterrupISR);
    UART_Start();
    LCD_Start();
    I2C_Start();
    PWM_Start();
    ADC_Start();
    /*
    ds.sec =  0x00; //  
    ds.min =  0x22;//
    ds.hour = 0b00010011;//Formato 24 horas bit 6 en 0 - 16 horas
    ds.date = 0x03; // dia 2
    ds.month = 0x03;//marzo
    ds.year = 0x19; // 2019
    ds.weekDay = 6; // Sunday: 6th day of week considering monday as first day.;
    DS_init();//Configura 
    DS_set_data();*/ 
    PINA_Write(0);
    PINB_Write(0);
    PINC_Write(0);
    PIND_Write(0);
    PWM_WriteCompare1(0);
    PWM_WriteCompare2(0); 
    EEPROM_Start();
    EEPROM_WriteByte(0,0);//Comentar en la presentacion
    
    for(;;)
    {       
        reloj();
        DS_get_data();   
        comparacion();
        muestreo();
        CyDelay(500);
    }
}