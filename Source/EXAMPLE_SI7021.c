/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: Temperature & Humidity sensor
     File Name		: SENSOR_SI7021.c
     Function		: SENSOR_SI7021
     Create Date	: 2017/07/19
---------------------------------------------------------------------- */
#ifndef __SI7021_EXAMPLE__
#define __SI7021_EXAMPLE__

#define SI7021_INIFINITE 			0	//set "1" -> read SI7021 inifinite
#define SI7021_RESET_ONCE   		1      //reset SI7021 once!
#define SI7021_LCD_DISPLAY		1	//if set "1 " enable LCD display data 


#include <stdio.h>
#include <math.h>
#include <delay.h>

#include "SENSOR_SI7021.h"
#include "Porting_Layer.h"

#if SI7021_LCD_DISPLAY==1
#include "alcd.h"
#endif


#if SI7021_RESET_ONCE==1
        CHAR8U SI7021_reset_once = 1;
#endif

void  EXAMPLE_SI7021(void);


void  EXAMPLE_SI7021(void)
{

		CHAR8S status = 0;
		FLOAT temperature =0.0,humidity = 0.0;
		CHAR8U mode = 0,battery_status = 0;

#if SI7021_LCD_DISPLAY==1
        	INT32U data[5]={0};                 
        	CHAR8U lcd_char_data[5]={0};
#endif			
               printf("-------------------- Temperature & Humidity sensor SI7021 --------------------\r\n");
              /* Temperature & Humidity sensor  SI7021 */

 #if SI7021_RESET_ONCE==1           
        if(SI7021_reset_once==1)    
 #endif       
            {
			 /* RESET SI7021 */
			 status = SI7021_RESET();
			 if(status==0){ printf(" RESET SI7021 success!\r\n");}
			 else {printf(" RESET SI7021 fail!\r\n");}
			 SI7021_reset_once=0;
            }
           	
			delay_ms(17);	/* after RESET SI7021 -> wait 15mS */

			 /* disable Heat IC function */
			 /* mode 0 : Humidity -> 12bit  ; Temperature -> 14bit */
			status = SI7021_INIT();
			 if(status==0) printf(" INITIAL SI7021 success!\r\n");
			 else printf(" INITIAL SI7021 fail!\r\n");


			/* check battery status */
			status = SI7021_CHECK_BATTERY_STATUS(&battery_status);
			if(status==0)
						{		
							if(battery_status==0x01)printf(" SI7021 Battery 1.8V<VDD<1.9v	VDD is not good!\r\n");
							if(battery_status==0x00)printf(" SI7021 Battery VDD>1.9v		VDD OK!\r\n");
						}
			else 		
						{
							printf(" SI7021 read battery status fail!\r\n");
						}


			/* check mode */
			status = SI7021_READ_MODE(&mode);		
			if(status ==0)
				{
					printf("  SI7021 mode %x!\r\n",mode);
				}
			else 
				{
					printf(" SI7021 read mode fail!\r\n");
				}
			
#if SI7021_INIFINITE ==1
		while(1)
		{
#endif	
				
			status = SI7021_READ_TEMPERATURE(&temperature,mode);
			if(status!=0) printf(" read SI7021 temperature data fail\r\n");
                 
			/*wait 5mS*/
			delay_ms(5);		                          
                
			status = SI7021_READ_HUMIDITY(&humidity,mode);
			if(status!=0) printf(" read SI7021 humidity data fail\r\n");

	#if SI7021_LCD_DISPLAY == 1						
			/* TEMPERATURE DISPLAY & PRINT TO UART */
         	      data[0]= (INT32U)(temperature);
	             data[1]= (INT32U)(temperature*10)%10;   
	             data[2]= (INT32U)(temperature*100)%10;   	  
	             data[3]= (INT32U)(temperature*1000)%10; 
 	             data[4]= (INT32U)(temperature*10000)%10;    		    
	             printf("TEMPERATURE = %d.%d%d%d%d C \r\n",data[0],data[1],data[2],data[3],data[4]);        
                                                
                    lcd_char_data[0] = (INT32U)(temperature/100)%10;     
                    lcd_char_data[1] = (INT32U)(temperature/10)%10;
                    lcd_char_data[2] = (INT32U)(temperature)%10;  
                    lcd_char_data[3] = (INT32U)(temperature*10)%10;   
                    lcd_char_data[4] = (INT32U)(temperature*100)%10;   	  

                    lcd_gotoxy(0,0);
		      lcd_putsf("T:");
			  
			if(temperature<0)
	            	{
	            		lcd_putchar(45);		/* LCD show "-"  mean negative */
			}                             
	           	else
			{
				lcd_putchar(32);		/* LCD show " "  mean positive */
			}   
							  
                    lcd_putchar(48+lcd_char_data[0]);
                    lcd_putchar(48+lcd_char_data[1]);
                    lcd_putchar(48+lcd_char_data[2]); 
                    lcd_putchar(46);    /* print "."  */ 
                    lcd_putchar(48+lcd_char_data[3]);  
                    lcd_putchar(48+lcd_char_data[4]);  
                    //lcd_putchar(178);    /* "o"  */   
                    lcd_putchar(67);    /* "C"   */                                
                      
                                         
			/* HUMIDITY DISPLAY & PRINT TO UART */    
      	            data[0]= (INT32U)(humidity);
	            data[1]= (INT32U)(humidity*10)%10;   
	            data[2]= (INT32U)(humidity*100)%10;   	  
	            data[3]= (INT32U)(humidity*1000)%10; 
 	            data[4]= (INT32U)(humidity*10000)%10;    		
	            printf("HUMIDITY = %d.%d%d%d%d %%RH \r\n",data[0],data[1],data[2],data[3],data[4]);        
                   
                    lcd_char_data[0] = (INT32U)(humidity/100)%10;     
                    lcd_char_data[1] = (INT32U)(humidity/10)%10;
                    lcd_char_data[2] = (INT32U)(humidity)%10;  
                    lcd_char_data[3] = (INT32U)(humidity*10)%10;   
                    lcd_char_data[4] = (INT32U)(humidity*100)%10;   	  

                    lcd_gotoxy(0,1);
		      lcd_putsf("H:");
                    lcd_putchar(48+lcd_char_data[0]);
                    lcd_putchar(48+lcd_char_data[1]);
                    lcd_putchar(48+lcd_char_data[2]); 
                    lcd_putchar(46);    /* print "."  */ 
                    lcd_putchar(48+lcd_char_data[3]);  
                    lcd_putchar(48+lcd_char_data[4]);  
                    lcd_putchar(37);    /* "%"  */     
                    //lcd_putsf("RH");                                        
       #endif         
                    
                       		 
#if SI7021_INIFINITE ==1
			SI7021_WAIT(1500);
		}
#else
			delay_ms(100);
			
#endif				

               printf("-------------------- Temperature & Humidity sensor SI7021 --------------------\r\n");
	  
}


#endif //#ifndef	SI7021_EXAMPLE
