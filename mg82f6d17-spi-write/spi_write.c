/*

	SPI WRITE
	
	11-03-22
	
	@amitabhgogoi
	
	-------xxxxxxxxx----------
	
	SFRPI: SFR PAGE INDEX REGISTER
	SPCON: SPI CONTROL REGISTER
	SPSTAT: SPI STATUS REGISTER
	SPDAT: SPI DATA REGISTER
	
	P1: PORT1	
	P10: PORT1 PIN 0
	
	P1M0: PORT1 MODE0 REGISTER
	P1M1: PORT1 MODE1 REGISTER


	P2: PORT2	
	P22: PORT2 PIN 2
	
	P2M0: PORT2 MODE0 REGISTER
	P2M1: PORT2 MODE1 REGISTER
	

	AUXR10: SPI PIN CHANGE REGISTER(NSS,MISO,MOSI,SCK)
	
	

	--------xxxxxxxxxx------------
	

		MODE0
		
		nSS = P3.3
		MOSI = P1.5
		MISO = P1.6
		SCK = P1.7
		
		
		MODE1
		
		nSS = P1.7
		MOSI = 3.5
		MISO = 3.4
		SCK = 3.3
		
	-------xxxxxxxxxxxxxx---------

*/


//SPI REG
sfr SPCON = 0x85;
sfr SPSTAT = 0x84;
sfr SPDAT = 0x86;


//PAGE INDEX REG
sfr SFRPI = 0xAC;


//PORT1 REG (FOR SPI PINS)
sfr P1 = 0x90;

//PORT1 MODE REG
sfr P1M0 = 0x91;
sfr P1M1 = 0x92;


/*
//PORT3 REG (FOR nSS)
sfr P3 = 0xB0;
//nSS PIN
sbit P33 = P3^3;


//PORT3 MODE REG
sfr P3M0 = 0xB1;
sfr P3M1 = 0xB2;
*/

/*
//PORT2 REG
sfr P2 = 0xA0;
//DIGITAL OUTPUT PIN
sbit P22 = P2^2;


//PORT2 MODE REG
sfr P2M0 = 0x95;
sfr P2M1 = 0x92;
*/



//SPI PINS, DEFAULT SPIPS0=0
sfr AUXR10 = 0xA4;



void delay(unsigned int time){
		
	unsigned int i,j;
	
		for(i=0; i<time; i+=1)
			for(j=0; j<2000; j+=1);
		
}



unsigned char receivedData;

void main(void){
	
		/*	
		//P2.2 AS DIGITAL OUTPUT
		SFRPI = 0;
		//P2.2 MODE0
		P2M0 |= (1<<2);
		
		SFRPI = 1;
		//P2.2 MODE1
		P2M1 &= ~(1<<2);
		*/
	
	
		//SPI PIN CONFIGURATION, MODE0 { nSS = P3.3,	MOSI = P1.5, MISO = P1.6,	SCK = P1.7 }
		SFRPI = 7;
		AUXR10 &= ~(1<<4);
		
		
		SFRPI = 0;	
		//P1.5(MOSI) P1.7(SCK) AS OUTPUT, P1.6(MISO) AS INPUT
		P1M0 |= (1<<5) | (1<<7) & ~(1<<6); 
		P1M1 &= ~(1<<5) & ~(1<<7) & ~(1<<6);
		
		
		/* //nSS PIN CONFIG
		SFRPI = 0;
		
		//nSS AS PUSH-PULL OUTPUT
		P3M0 &= ~(1<<3); //0
		P3M1 |= (1<<3); //1
		*/
		//P1 |= (1<<5);
		
		SFRPI = 0;
		
		//SPCON (SSIG, SPEN, DORD, MSTR, CPOL, CPHA, SPR1, SPR0)
		SPCON |= (1<<7) | (1<<6) & ~(1<<5) | (1<<4) & ~(1<<3) | (1<<2) & ~(1<<1) & ~(1<<0);
	
		SFRPI = 0;
		
		//SPCON (SPR2)
		SPSTAT &= ~(1<<0);
		
		SFRPI = 0;
		
		//SPSTAT = 0x80;
		
		//nSS IS HIGH, NO TRANSMISSION
		//P33 = 1;
		
		while(1){
			
			/*
			//P2.2 OUTPUT 0
			//P22 = 0;
			*/
			
			//nSS LOW, ENABLE SLAVE
			//P33=0;
			
			//WAIT UNTIL THRF IS CLEARED
			//while((SPSTAT & (1<<5)));
			
			
			//WRITE TO SPDAT FOR TRANSMISSION			
			SPDAT = 'B';
			
			
			//WAIT UNTIL THE SPIBSY FLAG IS CLEARED BY H/W
			//while((SPSTAT & (1<<4)));
			
			
			//WAIT UNTIL THE TRANSMISSION IS COMPLETED, INTERUPT GENERATE
			while(!(SPSTAT & (1<<7)));
			
			//nSS HIGH, DISABLED SLAVE
			//P33=1;
			
			receivedData = SPDAT;
			
			
			//CLEAR SPIF
			SPSTAT = 0x80;
			
			//P2.2 OUTPUT 1
			//P22 = 1;
			
			delay(1000);
			
			//while(1);
			
		}
		
}


