/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: Temperature & Humidity
     File Nam		: SENSOR_SI7021.h
     Function		: SENSOR_SI7021
     Create Date	: 2017/07/15
---------------------------------------------------------------------- */
#ifndef __SI7021_HEADER__
#define __SI7021_HEADER__


//--------------------------------- Define SLAVE ADDRESS -------------------------------------
// SI7021 SLAVE ADDRESS
#define SI7021_SLAVE_ADDRESS 				0x80	//the Same Slv Addr with SHT20 , HTU21D
//--------------------------------- Define SLAVE ADDRESS -------------------------------------


// ------------- DEFINE SI7021 REGISTER  ------------
// USE NO HOLD MASTER REGISTER ! --> if CHANGE to HOLD MASTER see SPEC!
//#define SI7021_HOLD_MASTER_TEMP			0xE3
//#define SI7021_HOLD_MASTER_HUM			0xE5
#define SI7021_NO_HOLD_MASTER_TEMP		0xF3
#define SI7021_NO_HOLD_MASTER_HUM		0xF5
#define SI7021_WRITE_USER_REG				0xE6

#define SI7021_READ_USER_REG				0xE7
// resolution set   (def:00)
// bit 7   bit0		RH   	  	Temp		   mode*
//   0       0		12bit		14bit			0
//   0       1		8bit           12bit			1		
//   1       0		10bit         13bit			2
//   1       1		11bit         11bit			3
//--------------------------
// bit 6  End of Battery (def:0) ;  if 0 -> [VDD>1.9v] VDD ok; if 1 ->[1.8<VDD<1.9v]   VDD Low
// ** NOTE : minimum recommended operating voltage is 1.9 V.
//--------------------------
// bit 5,4,3 Reserved	(def:0)
//--------------------------
// bit 2 Enable on-chip header !(def:0)   
// Note : OTP reload active loads default setting after each time a measurement command is issued.
//--------------------------
// bit 1 Disable OTP reload	 (def:1)	
																
#define SI7021_SOFT_RST						0xFE		//after command -> 15mS wait sensor reset ok!


//*** Undo read the SI7021 snesor ID , because is not necessary to do.
//--I2C 2 REG command to read 1st  ID 
//return 8 byte data
#define	SI7021_READ_ID11						0xFA
#define	SI7021_READ_ID12						0x0F

//--I2C 2 REG command to read 2nd  ID 
//return 6 byte data 
#define	SI7021_READ_ID21						0xFC
#define	SI7021_READ_ID22						0xC9

//--I2C 2 REG command to Read Firmware Revision
//return 1 byte data
#define	SI7021_READ_FW1						0x84			
#define	SI7021_READ_FW2						0xB8
// ------------- DEFINE SI7021 REGISTER  ------------




// ------------- DEFINE SI7021 STRUCT ------------

#define SI7021_USER_REG_DEFAULT 0x3A	//power on or after reset

//--------------------------------
//SET BIT7 & BIT 0
// read Temperature & Humidity
//   RH   	      Temp			mode*       bit7		bit0
//  12bit		14bit                  0              0		0
//  8bit           12bit		        	1			  0		1
//  10bit         13bit	              	2			  1		0
//  11bit         11bit                  3			  1		1
// return 2 value Temperature & Humidity
// check mode to know the resolution
typedef enum{
		 SI7021_MODE0 = 0x00,				
		 SI7021_MODE1 = 0X01,		
		 SI7021_MODE2 = 0X80,
		 SI7021_MODE3 = 0X81
}SI7021_MODE;	
//--------------------------------

//--------------------------------
//---------------------------
// RH  	 	
//			resolution         time
//        	12bit			12ms
//		  	  8bit			4ms
//         	10bit			6ms
//         	11bit			7ms
//---------------------------
// Temp 
//			resolution         time
//        	14bit			11ms
//		  	13bit			7ms
//         	12bit			4ms
//         	11bit			3ms
//---------------------------	
typedef enum{
		SI7021_RH_12BIT_DELAY		= 25,//16,
		SI7021_RH_8BIT_DELAY		= 8,//3,
		SI7021_RH_10BIT_DELAY		= 10,//5,
		SI7021_RH_11BIT_DELAY		= 20,//11,
		SI7021_T_14BIT_DELAY		= 70,//50,
		SI7021_T_13BIT_DELAY		= 35,//25,
		SI7021_T_12BIT_DELAY		= 25,//13,
		SI7021_T_11BIT_DELAY		= 15,//7
}SI7021_DELAY;
//--------------------------------

// ------------- DEFINE SI7021 STRUCT ------------



//********************************************* SYSTEM *************************************************
//--------------------------------------------------------------------------------------------------
// initial SI7021
// -> set Temperature/Humidity resoluction   ; default mode 0  // Humidity -> 12bit  ; Temperature -> 14bit 
// disable Heat IC function
CHAR8S SI7021_INIT(void);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// SI7021 IC WRITE command 
CHAR8S SI7021_WRITE_COMMAND(CHAR8U command);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// SI7021 IC READ command 
CHAR8S SI7021_READ_COMMAND(CHAR8U *r_data_stream);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// reset SI7021 IC
// Note : delay 15mS after set reset . let SI7021 initial!
// 		   special I2C protocol
// 		   write command 0xFE to reset.
CHAR8S SI7021_RESET(void);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// read Temperature 
//   RH   	      Temp			mode*
//  12bit		14bit                  0
//  8bit           12bit		        1		
//  10bit         13bit	              2
//  11bit         11bit                  3
// check mode to know the resolution
// MEASUREMENT TIME
// Temp 
//			resolution         time
//        	14bit			11ms
//		  	13bit			7ms
//         	12bit			4ms
//         	11bit			3ms
//---------------------------
// bit 7   bit0		RH   	  	Temp		   mode*
//   0       0		12bit		14bit			0
//   0       1		8bit           12bit			1		
//   1       0		10bit         13bit			2
//   1       1		11bit         11bit			3
//--------------------------
//**** calculate equation ****
//TEMPERATURME FORMULA =  -46.85 + 175.72 *( sample_T / 2^(sample bit)  ) , sample bit = resolution bit   , sample_RH = Read Value
CHAR8S SI7021_READ_TEMPERATURE(FLOAT *t,CHAR8U mode)	;		
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// read Humidity
//   RH   	      Temp			mode*
//  12bit		14bit                  0
//  8bit           12bit		        1		
//  10bit         13bit	              2
//  11bit         11bit                  3
// check mode to know the resolution
// MEASUREMENT TIME
// RH  	 	
//			resolution         time
//        	12bit			12ms
//		  	  8bit			4ms
//         	10bit			6ms
//         	11bit			7ms
//---------------------------
// bit 7   bit0		RH   	  	Temp		   mode*
//   0       0		12bit		14bit			0
//   0       1		8bit           12bit			1		
//   1       0		10bit         13bit			2
//   1       1		11bit         11bit			3
//--------------------------
//**** calculate equation ****
//HUMIDITY FORMULA = -6 + 125 *( sample_RH  / 2^(sample bit)   )   , sample bit = resolution bit   , sample_RH = Read Value
CHAR8S SI7021_READ_HUMIDITY(FLOAT *h,CHAR8U mode);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// use SI7021 check the battery(power) voltage  -> USER REG 0XE7 bit6
// ** NOTE : minimum recommended operating voltage is 1.9 V.
// status  if 0x00 ->  [VDD>1.9v] ; if 0x01 -> [1.8<VDD<1.9v]  
CHAR8S SI7021_CHECK_BATTERY_STATUS(CHAR8U *status);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// read / write User Register data 
// RW = 1 -> READ USER REG ; 0 -> WRITE USER REG
CHAR8S SI7021_RW_USER_REG(CHAR8U rw,CHAR8U *data);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// read mode from REG 0XE7
// RESOLUTION -> set mode0 ~ mode3
// **** use SI7021_MODE enum ****
CHAR8S SI7021_READ_MODE(CHAR8U *mode);
//--------------------------------------------------------------------------------------------------
// write mode from REG 0XE7
// RESOLUTION -> set mode0 ~ mode3
// **** use SI7021_MODE enum ****
CHAR8S SI7021_WRITE_MODE(CHAR8U mode);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// let SI7021 ""enable"" on-chip heater -> USER REG bit2
CHAR8S SI7021_HEAT_IC(void);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// let SI7021 ""disable"" on-chip heater -> USER REG bit2
CHAR8S SI7021_DIS_HEAT_IC(void);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// SI7021 CRC checksum 
// Function :  x^8  + x^5 + x^4 + 1		-> 0x0131
// return CRC 

//** first read high byte -> second read low byte
//check CRC function
//  [1bytes]   input 0xDC		->  output 0x79 	
//  [2bytes]   input 0x683A	->  output 0x7C
//  [2bytes]   input 0x4E85	->  output 0x6B 
CHAR8S SI7021_CRC_CHECKSUM(CHAR8U *data, CHAR8U bytes,CHAR8U checksum);
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
// SI7021 wait delay
// wait measurement time
void SI7021_WAIT(INT32U wait_time);
//--------------------------------------------------------------------------------------------------
//********************************************* SYSTEM *************************************************

#endif		//#ifndef __SI7021_HEADER__
