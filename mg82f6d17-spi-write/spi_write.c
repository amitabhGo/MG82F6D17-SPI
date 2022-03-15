/*

	SPI WRITE
	
	11-03-22
	
	authors:	@amitabhgogoi,
	
	--------------------xxx--------------------
	
	SFRPI: SFR PAGE INDEX REGISTER
	SPCON: SPI CONTROL REGISTER
	SPSTAT: SPI STATUS REGISTER
	SPDAT: SPI DATA REGISTER
	
	P1: PORT1	
	P10: PORT1 PIN 0
	
	P1M0: PORT1 MODE0 REGISTER
	P1M1: PORT1 MODE1 REGISTER


	P3: PORT3	
	P33: PORT3 PIN 3
	
	P3M0: PORT3 MODE0 REGISTER
	P3M1: PORT3 MODE1 REGISTER
	

	AUXR10: SPI PIN CHANGE REGISTER(NSS,MISO,MOSI,SCK)
	
	

	--------------------xxx--------------------
	

		SPI MODE0 PINS
		
		nSS = P3.3
		MOSI = P1.5
		MISO = P1.6
		SCK = P1.7
		
		
		SPI MODE1 PINS
		
		nSS = P1.7
		MOSI = 3.5
		MISO = 3.4
		SCK = 3.3
		
	--------------------xxx--------------------

			
		void spiPinSelect(unsigned int) : SET SPI PINS ACCORDING TO THE MODE (0/1)
		
		void spiMode(unsigned int ) : SET SPI MODE ACCORDING TO CLOCK POL AND PHASE
		
		void spiClockRate(unsigned int): SET CLOCK FREQUENCY
		
		void spiTransfer(unsigned char) : TRANSFER DATA
		
		
	
*/


#define PIN_MODE0 0 //SPI MODE 0
#define PIN_MODE1 1 //SPI MODE 1


//PAGE INDEX REGISTER, GLOBAL DECLARATION
sfr SFRPI = 0xAC;

//SPI REGISTER INITIALIZATION

//SPI REGISTERS
sfr SPCON = 0x85;
sfr SPSTAT = 0x84;
sfr SPDAT = 0x86;
	
	
//SPI PIN SELECTION REGISTER, PIN4, DEFAULT SPIPS0=0 SELECTS MODE 0,
sfr AUXR10 = 0xA4;

	
//PORT1 REGISTER (FOR SPI PINS MOSI, MISO, SCK)
sfr P1 = 0x90;
sbit P17 = P1^7;
sbit P15 = P1^5;

//PORT1 MODE REGISTERS
sfr P1M0 = 0x91;
sfr P1M1 = 0x92;
	
	
//PORT3 REGISTER (SPI nSS PIN)
sfr P3 = 0xB0;
sbit P33 = P3^3;

//PORT3 MODE REGISTERS
sfr P3M0 = 0xB1;
sfr P3M1 = 0xB2;


unsigned int PIN_MODE;


void spiPinSelect(unsigned int mode){
	
	PIN_MODE = mode;
		
	SFRPI = 0;
	//SPI ENABLE (SPE=1)
	SPCON |=  (1<<6);
	
	if(mode == 0){
		
		//SPI PIN CONFIGURATION, MODE0 
		//nSS = P3.3,	MOSI = P1.5, MISO = P1.6,	SCK = P1.7 
		SFRPI = 7;
		AUXR10 &= ~(1<<4);
		
	}
	if(mode == 1){
		
		//nSS = P1.7,	MOSI = 3.5, MISO = 3.4, SCK = 3.3
		SFRPI = 7;
		AUXR10 |= (1<<4);
	
	}
	
	
}


void spiMode(unsigned int mode){
				
	
				//PAGE INDEX
				SFRPI = 0;
	
				switch(mode){
					
					case 0:								
								//(CPOL=0, CPHA=0)
								SPCON &= ~(1<<3) & ~(1<<2);
								break;
					
					case 1:
								//(CPOL=0, CPHA=1)
								SPCON &= ~(1<<3) | (1<<2);
								break;
					
					case 2:
								//(CPOL=1, CPHA=0)
								SPCON |= (1<<3) & ~(1<<2);
								break;
					
					case 3:
								//(CPOL=1, CPHA=1)
								SPCON |= (1<<3) | (1<<2);
								break;
					
					default:
								break;
					
				}
				
				
}


void spiClockRate(unsigned int mode){
	
			SFRPI = 0;
	
			switch(mode){
				
				case 0:
							//(SPR1=0, SPR0=0), 12MHz/4 = 3MHz
							SPCON &= ~(1<<1) & ~(1<<0);
								
							//(SPR2=0) 0xC8
							SPSTAT = 0x00;
							
							break;
					
				case 1:					
							//(SPR1=0, SPR0=1), 12MHz/8 = 1.5MHz
							SPCON &= ~(1<<1) | (1<<0);
								
							//(SPR2=0) 0xC8
							SPSTAT = 0x00;
				
							break;
					
				case 2:
							//(SPR1=1, SPR0=0), 12MHz/16 = 750KHz
							SPCON |= (1<<1) & ~(1<<0);
								
							//(SPR2=0) 0xC8
							SPSTAT = 0x00;
							
							break;
				
				case 3:							
							//(SPR1=1, SPR0=1), 12MHz/32 = 375KHz
							SPCON |= (1<<1) | (1<<0);
								
							//(SPR2=0) 0xC8
							SPSTAT = 0x00;
							
							break;
				
				case 4:							
							//(SPR1=0, SPR0=0), 12MHz/64 = 187.5KHz
							SPCON &= ~(1<<1) & ~(1<<0);
								
							//(SPR2=1) 0xC8
							SPSTAT = 0x01;
							
							break;
				
				default:							
							break;
				
			}
			
	
}

//unsigned char receivedData;


char spiTransfer(unsigned char value){
	
			
			if(PIN_MODE == 0){
				
				
				//SET PAGE INDEX = 0
				SFRPI = 0;

				/*
				 WHEN CPHA=0, SSIG=0, nSS DETERMINES MASTER/SLAVE 
				 WHEN CPHA=1, SSIG=0/1 :
				 IF SSIG=1 MSTR DETERMINES MASTER/SLAVE
 				 IF SSIG=0 SAME AS BEFORE 
				*/
				
				//(SSIG=1, DORD=0, MSTR=1)
				SPCON |= (1<<7) & ~(1<<5) | (1<<4); 
				
				//P1.5(MOSI) P1.7(SCK) AS PUSH-PULL OUTPUT, P1.6(MISO) AS INPUT
				P1M0 |= (1<<5) | (1<<7) & ~(1<<6); 
				P1M1 &= ~(1<<5) & ~(1<<7) & ~(1<<6);
			
				//OUTPUT 0
				P15 = 0;
				P17 = 0;
				
				//P3.3(nSS), AS PUSH-PULL OUTPUT  
				
				P3M0 &= ~(1<<3);
				P3M1 |= (1<<3);
								
				P33 = 1;				
				
				
				//SPI ENABLE (SPE=1)
				//SPCON |=  (1<<6);
				
				
				//SLAVE SELECT
				P33 = 0;
				
				//WAIT UNTIL THRF IS CLEARED
				//while((SPSTAT & (1<<5)));
			
			
				//WRITE TO SPDAT FOR TRANSMISSION			
				SPDAT = value;
			
				
				
				//WAIT UNTIL THE SPIBSY FLAG IS CLEARED BY H/W
				//while((SPSTAT & (1<<4)));
			
			
				//WAIT UNTIL THE TRANSMISSION IS COMPLETED, INTERUPT GENERATE
				while(!(SPSTAT & (1<<7)));
				
				//nSS HIGH, SLAVE NOT SELECT
				P33=1;
							
				//receivedData = SPDAT;
				
				
				
				//CLEAR SPIF
				SPSTAT = 0x80;
				
				
				
				//SSIG=0, MSTR=0
				SPCON &= ~(1<<7) & ~(1<<4);
				
							
				//P1.5(MOSI) P1.7(SCK) AS INPUT, P1.6(MISO) AS OUTPUT
				P1M0 &= ~(1<<5) & ~(1<<7) | (1<<6); 
				P1M1 &= ~(1<<5) & ~(1<<7) & ~(1<<6);
			
				
				//P3.3(nSS) AS INPUT
				
				P3M0 |= (1<<3);
				P3M1 &= ~(1<<3);
				
				
			}
			
			
	
			//return receivedData;
	
	
}



void main(void){
		
		
		//SPI MODE 0
		spiPinSelect(PIN_MODE0);
				
		spiMode(1);
	
		spiClockRate(0);
			
		//nSS IS HIGH, NO TRANSMISSION
		//P33 = 1;
		
		
		while(1){
			
			
			//spi_transfer('0'); //48
				
			//spi_transfer('1'); //49
			
			spiTransfer('2'); //50
			
			while(1);
						
			
		}
		
}


