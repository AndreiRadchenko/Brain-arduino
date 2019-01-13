#include <avr/io.h> 
#include <util/delay.h>

#define NO_CTRL 0b00011111 //îòäàåì óïðàâëåíèå êàì. êàíàëó åñëè ïî ëèíèÿì óïðàâëåíèÿ
				   		   //íè÷åãî íå ïðèõîäèò

//configuring all used ports pins for output DDxn = 1
void Set_pin_out()
{

	DDRF = DDRF | 0b11111111;
	DDRK = DDRK | 0b01111111;
	DDRE = DDRE | 0b00111011;
	DDRB = DDRB | 0b11100000;
	DDRH = DDRH | 0b01100011;
	DDRD = DDRD | 0b10000111;
	DDRA = DDRA | 0b11111111;
	DDRC = DDRC | 0b11111111;
	DDRG = DDRG | 0b00000111;
	DDRL = DDRL | 0b11111111;
	DDRJ = DDRJ | 0b00000011;

	PORTF &= 0b00000000;
	PORTK &= 0b10000000;
	PORTE &= 0b11000100;
	PORTB &= 0b00011111;
	PORTH &= 0b10011100;
	PORTD &= 0b01111000;
	PORTA &= 0b00000000;
	PORTC &= 0b00000000;
	PORTG &= 0b11111000;
	PORTL &= 0b00000000;
	PORTJ &= 0b11111100;

}
							  
//CCU1 is connected to F0, F1, F2 - cont2, cont1, cont0; F3, F4 - mic1, mic2
void CCU1_out(unsigned char ch4out)
{

	char pf = PORTF;
	pf &= 0b11100000; 
	pf |= (ch4out & 0b00011111);
	PORTF = pf;	

}

//CCU2 is connected to F5, F6, F7 - cont2, cont1, cont0; K0, K1 - mic1, mic2
void CCU2_out(unsigned char ch4out)
{

	char pf1 = PORTF;
	pf1 &= 0b00011111;
	pf1 |= (ch4out<<5);
	PORTF = pf1;
	
	char pk = PORTK;
	pk &= 0b11111100;
	pk |= ((ch4out>>3) & 0b00000011);
	PORTK = pk;

}

//CCU3 is connected to K2, K3, K4 - cont2, cont1, cont0; K5, K6 - mic1, mic2
void CCU3_out(unsigned char ch4out)
{

	char pk1 = PORTK;
	pk1 &= 0b10000011;
	pk1 |= ((ch4out<<2) & 0b01111100);
	PORTK = pk1;
	
}

//CCU4 is connected to E3, E4, E5 - cont2, cont1, cont0; E0, E1 - mic1, mic2
void CCU4_out(unsigned char ch4out)
{

	char pe = PORTE;

	char temp = ch4out;
	temp &= 0b00011000;
	temp = (temp>>3);
//	temp = (ch4out<<3);// & 0b00111000;
//	temp = temp | (ch4out>>3); 

	pe &= 0b11000100;
	pe |= (ch4out<<3) & 0b00111000;
	pe |= temp;

// 	pe = temp;
	PORTE = pe;
	
}

//CCU5 is connected to B5, B6, B7 - cont2, cont1, cont0; H5, H6 - mic1, mic2
void CCU5_out(unsigned char ch4out)
{

	char pb = PORTB;
	pb &= 0b00011111;
	pb |= (ch4out<<5);
	PORTB = pb;
	
	char ph = PORTH;
	ph &= 0b10011111;
	ph |= (ch4out<<2) & 0b01100000;
	PORTH = ph;

}

//CCU6 is connected to D0, D1, D2 - cont2, cont1, cont0; H0, H1 - mic1, mic2
void CCU6_out(unsigned char ch4out)
{

	char pd = PORTD;
	pd &= 0b11111000;
	pd |= (ch4out & 0b00000111);
	PORTD = pd;
	
	char ph = PORTH;
	ph &= 0b11111100;
	ph |= ((ch4out>>3) & 0b00000011);
	PORTH = ph;

}

//CCU7 is connected to A0, A1, A2 - cont2, cont1, cont0; A3, A4 - mic1, mic2
void CCU7_out(unsigned char ch4out)
{

	char pa = PORTA;
	pa &= 0b11100000; 
	pa |= (ch4out & 0b00011111);	
	PORTA = pa;	

}

//CCU8 is connected to A5, A6, A7 - cont2, cont1, cont0; C6, C7 - mic1, mic2
void CCU8_out(unsigned char ch4out)
{

	char temp = ch4out;
//	temp &= 0b11000000;
//	temp = (temp<<3);

	char pa1 = PORTA;
	pa1 &= 0b00011111;
	pa1 |= (ch4out<<5);
	PORTA = pa1;
	
	char pc = PORTC;
	pc &= 0b00111111;
	pc |= ((temp<<3) & 0b11000000);
	PORTC = pc;

}

//CCU9 is connected to C1, C2, C3 - cont2, cont1, cont0; C4, C5 - mic1, mic2
void CCU9_out(unsigned char ch4out)
{

	char pc1 = PORTC;
	pc1 &= 0b11000001; 
	pc1 |= ((ch4out<<1) & 0b00111110);	
	PORTC = pc1;	

}

//CCU10 is connected to G0, G1, G2 - cont2, cont1, cont0; D7, C0 - mic1, mic2
void CCU10_out(unsigned char ch4out)
{

	char pg = PORTG;
	pg &= 0b11111000;
	pg |= (ch4out & 0b00000111);
	PORTG = pg;
	
	char pd = PORTD;
	pd &= 0b01111111;
	pd |= ((ch4out<<4) & 0b10000000);
	PORTD = pd;

	char pc2 = PORTC;
	pc2 &= 0b11111110;
	pc2 |= ((ch4out>>4) & 0b00000001);
	PORTC = pc2;

}

//CCU11 is connected to L3, L4, L5 - cont2, cont1, cont0; L6, L7 - mic1, mic2
void CCU11_out(unsigned char ch4out)
{

	char pl = PORTL;
	pl &= 0b00000111; 
	pl |= (ch4out<<3);	
	PORTL = pl;	

}

//CCU12 is connected to L0, L1, L2 - cont2, cont1, cont0; J0, J1 - mic1, mic2
void CCU12_out(unsigned char ch4out)
{

	char pl = PORTL;
	pl &= 0b11111000;
	pl |= (ch4out & 0b00000111);
	PORTL = pl;
	
	char pj = PORTJ;
	pj &= 0b11111100;
	pj |= ((ch4out>>3) & 0b00000011);
	PORTJ = pj;

}

//function gain_change_ccu take as pointer one of the functions CCU1_out - CCU12_out and value of mic1 mic2
void gain_change_ccu(void (*ccu_out)(unsigned char), char mic1, char mic2)
{
	//char mic1;
	//char mic2;

	//if (ccu == 0xC0) //îòäàåì óïðàâëåíèå êàì. êàíàëó åñëè ïî ëèíèÿì óïðàâëåíèÿ
				   //íè÷åãî íå ïðèõîäèò
	//	{	
	//		CCU1_out(NO_CTRL);
	//	}
	//else
	//	{
			mic1 = 4 - mic1; //������� ����� �������� ������� ������������ �������� �������
			mic1 &= 0b00000111;
			mic1 |= 0b00001000;

			mic2 = 4 - mic2;
			mic2 &= 0b00000111;
			mic2 |= 0b00010000;

			//portD_out(~mic1);
			(*ccu_out)(~mic1);
			_delay_ms(250);
			//portD_out(~mic2);
			(*ccu_out)(~mic2);
			//_delay_ms(250);
	//	};
};


