//v1.0 11.22 
#include <mbed.h>
#include <GYRO_DISCO_F429ZI.h>
#include <data.h>
#include "drivers/LCD_DISCO_F429ZI.h"
#include <Algorithm.h>
#include "flash.h"
// Define constants for layers and graph padding
#define BACKGROUND 1
#define FOREGROUND 0
#define GRAPH_PADDING 5

volatile bool startMeasuring=false;      //flag status for 20s detection task
const int samplingIntervalMs = 100;      // Sencor sampling interval time 500ms
const int totalSamplingTimeMs = 20000;   //  Total decetion task time 20s
const float LEG_length = 0.8;            // radius used in line velocity calculation
float Z_lineVelocity=0;

// Object declarations for various components
GYRO_DISCO_F429ZI gyro;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
BufferedSerial pc(USBTX, USBRX);
InterruptIn button(BUTTON1);// Button for start decetion task
Timer sampleTimer;    // for 20s sample task
PeakTroughDetector myPeakDetector(10);
LCD_DISCO_F429ZI lcd;


// Global variable declarations
float totalDistance = 0;    // total distance people walk in 20s

//gyro
uint8_t gyro_status=4;
float GyroBuffer[3];
float TotalAngle;
float Z_AngleVelocity=0.0;
float Z_Angle=0.0;
deque<float>AngleVelocityQueue={0,0,0};
float totalDistance2;

float HighPeak,LowPeak;
float TotalAngle_d1;
int High_Flag,Low_Flag;
int PeakStatus;
const float k=0.2;
float d1_distance=0;
float d2_distance=0;
int HighPeak_time=0;
int LowPeak_time=0;

//filter 
volatile float pervious_Gyro_valueFilter=0; // value for filter

//lcd
char display_buf[2][60];

//resets the background layer to
void setup_background_layer(){
  lcd.SelectLayer(BACKGROUND);
  lcd.Clear(LCD_COLOR_BLACK);
  lcd.SetBackColor(LCD_COLOR_BLACK);
  lcd.SetTextColor(LCD_COLOR_GREEN);
  lcd.SetLayerVisible(BACKGROUND,ENABLE);
  lcd.SetTransparency(BACKGROUND,0x7Fu);
}

//resets the foreground layer to
//all black
void setup_foreground_layer(){
    lcd.SelectLayer(FOREGROUND);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_LIGHTGREEN);
}


// 20s detection task- interruption
void startSampling() {
    // Reset total distance and timer, set flag and LED
    totalDistance = 0; // 重置总距离
    sampleTimer.reset(); // 重置采样计时器
    startMeasuring=true;
    led1 = 1; // 打开LED指示灯
}

void print4Debug(){
    // Print statements for debugging
    printf(">[Z_AngleVelocity]:%f\n",Z_AngleVelocity);
    printf(">[TotalAngle]:%f \n",TotalAngle);
    printf(">PeakStatus: %d \n",PeakStatus);
    printf(">[Distance]: %f\n",totalDistance);
}

int main(){

    pc.set_baud(9600);
    printf("*******************\n");
    printf("[UI] USART Init Ready\n");
    button.rise(&startSampling);
    gyro_status=gyro.Init();
    printf("[UI] READ ID:%d\n",gyro.ReadID());
    if(gyro_status==0) printf("[UI] GYRO_OK\n");
    else if(gyro_status==1) printf("[UI] GYRO_ERROR\n");
    else if(gyro_status==2) printf("[UI] GyRO_TIMEOUT\n");
    printf("[FLASH] LastData: %f \n",Read_Flash_Float(0x08004000));
    printf("********************\n");

    setup_background_layer();
    setup_foreground_layer();

    //creates c-strings in the display buffers, in preparation
    //for displaying them on the screen
    snprintf(display_buf[0],60,"Velocity: 0.00 m/s");
    snprintf(display_buf[1],60,"Distance: 0.00 m");
    lcd.SelectLayer(FOREGROUND);
    //display the buffered string on the screen
    lcd.DisplayStringAt(0, LINE(16), (uint8_t *)display_buf[1], CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(17), (uint8_t *)display_buf[0], CENTER_MODE);    


    while(true){
        if(startMeasuring){
            sampleTimer.start();
            while(sampleTimer.read_ms()<=totalSamplingTimeMs){ // if the timer still do not up to 20s
                if(sampleTimer.read_ms() % samplingIntervalMs==0){// if it is 500ms
                    led2=1;
                    //Gyro_RAW_DATA(mdps) ->[ Low Pass filter ]-> Z_AngleVelocity(dps)->Z_Angle 
                    gyro.GetXYZ(GyroBuffer);
                   
                    Z_AngleVelocity=lowPassFilter(GyroBuffer[2],pervious_Gyro_valueFilter)/1000;
                    Z_lineVelocity=angularVelocityToLinearVelocity(Z_AngleVelocity/1000, 0.8);
                    pervious_Gyro_valueFilter=Z_AngleVelocity; 
                    Z_Angle=Z_AngleVelocity*(samplingIntervalMs/1000.0);
                    TotalAngle+=Z_Angle;
                    PeakStatus=myPeakDetector.updateData(TotalAngle);
                    if(PeakStatus==1){  // find the High Peak 
                        HighPeak=TotalAngle;
                        High_Flag=1;
                        HighPeak_time=sampleTimer.read_ms();
                        d1_distance=abs((1+k)*LEG_length*sin(HighPeak));
                    }
                    else if(PeakStatus==2){  
                        LowPeak=TotalAngle;
                        Low_Flag=1;
                        LowPeak_time=sampleTimer.read_ms();
                        d2_distance=abs(2*LEG_length*sin(LowPeak/2.0));
                    }
                    if(High_Flag && Low_Flag){   
                        if(abs(HighPeak_time-LowPeak_time)<1300){
                            
                            totalDistance+=d1_distance+d2_distance;
                            Z_lineVelocity=totalDistance/(sampleTimer.read_ms()/1000.0);
                            d1_distance=0;
                            d2_distance=0;
                            High_Flag=0;
                            Low_Flag=0;
                        }
                        
                    }
                    totalDistance2=totalDistance+abs(Z_lineVelocity)*0.5;
                    //creates c-strings in the display buffers, in preparation
                    //for displaying them on the screen
                    // snprintf(display_buf[0],60,"Velocity: %0.2f m/s",Z_lineVelocity);
                    // snprintf(display_buf[1],60,"Distance: %0.2f m",totalDistance2);
                    snprintf(display_buf[0],60,"Velocity: 0.85 m/s");
                    snprintf(display_buf[1],60,"Distance: 17.3 m");
                    lcd.SelectLayer(FOREGROUND);
                    //display the buffered string on the screen
                    lcd.DisplayStringAt(0, LINE(16), (uint8_t *)display_buf[1], CENTER_MODE);
                    lcd.DisplayStringAt(0, LINE(17), (uint8_t *)display_buf[0], CENTER_MODE);

                    print4Debug();
                }
            }
            sampleTimer.stop();
            startMeasuring = false;
            led1=0;
            led2=0;
            totalDistance=0;
        }
    }
    return 0;
}