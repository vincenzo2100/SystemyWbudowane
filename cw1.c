#include <p24fj128ga010.h>

//Konfiguracja dla Explorer 16 z progr. icd2
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & FWDTEN_OFF)
_CONFIG2(FCKSM_CSDCMD & OSCIOFNC_ON & POSCMOD_HS & FNOSC_PRI)


#define SCALE 308L

int ktyBit(int n, int k)//funkcja zwraca k-ty bit liczby n
{
    return (n >> (k-1)) & 1;
}
int bitShift(int n, int k, int x) //przesunięcie liczby n o k miejsc w prawo lub w lewo w zależności od x
{
    if(x)
        {
            return n >> k;
        }
        return n << k;
}

int main(void){
unsigned long i;
unsigned char display=0;
unsigned long j=1; //licznik do przełączania podprogramów
unsigned char x=0; //licznik wykorzystywany w poszczególnych podpogramach
int y=0; //licznik wykorzystywany w poszczególnych podpogramach
unsigned char jed; //liczba jedności w zad 5,6
unsigned char dzie; //liczba dziesiątek w zad 5,6
unsigned char waz = 7; //licznik używany w zad 7
int k = 0; //zmienna uzywana w zad 7, decyduje o kierunku poruszanie się węża
int kolejka=0; //zmienna uzywana w zad 8, przechowująca wartość kolejki
int l = 8; //licznik używany w zad 8
int temp = 8; //zmienna używana w zad 8
unsigned char index=0; //zmienna używana w zad 8
int seed = 1; // zmienna używana w zad 9, wartość początkowa
int s1, s2, s5, s6, a; //zmienne używane w zad 9

//inicjalizacja
 PORTA=0x0000;
 TRISA=0xFF00;
 TRISD=0xFFFF;

//niekończąca się pętla
again:
	Nop();
	PORTA=(unsigned int) display;
	TRISA=0xFF00;
	TRISD=0xFFFF;
	for(i=500L*SCALE;i>0;i--) Nop();
	if (PORTDbits.RD13 == 0){ //przełączenie podporgramu o 1 w górę przez wciśnięcie przycisku RD13
	   j=j+1; //zwiększenie licznika decydującego o tym który z podpogramów zostanie uruchomiony o 1
	   display = 0; //reset wartości wyświetlanej na wyświetlaczu
       x=0; //reset licznika
       y=0; //reset licznika
	}
	else if(PORTDbits.RD6 == 0){
	   j=j-1; //zmniejszenie licznika decydującego o tym który z podpogramów zostanie uruchomiony o 1
	   display = 0; //reset wartości wyświetlanej na wyświetlaczu
       x=0; //reset licznika
       y=0; //reset licznika
	}
	else
	   if (j>9){ //zapewnienie ciągłości działania programu, przełączanie z podporgramu 9 na 1
   	   	j=1;
	   }
	   if (j<1){ //zapewnienie ciągłości działania programu, przełączanie z podporgramu 1 na 9
   	   	j=9;
	   }
       if (y>99){ //zapewnienie działania w zakresie od 0 do 99
   	   	y=0;
	   }
	   if (y<0){ //zapewnienie działania w zakresie od 0 do 99
   	   	y=99;
	   }
	   switch (j){ //wybrór podprogramu w zależności od wartości licznika
		case 1: //8 bitowy licznik binarny zliczający w górę (0...255)
			display=display+1; //zwiększenie wartości wyświetlanej o 1
			break;
		case 2: //8 bitowy licznik binarny zliczający w dół (255...0)
			display=display-1; //zmniejszenie wartości wyświetlanej o 1
			break;
		case 3: //8 bitowy licznik w kodzie Graya zliczający w górę (repr. 0...255)
			display = x^(x>>1); //zamiana na kod Graya
			x++; //zwiększenie wartości licznika o 1, przez co poruszamy się od 0 do 255
			break;
		case 4: //8 bitowy licznik w kodzie Graya zliczający w dół (repr. 255...0)
			display = x^(x>>1); //zamiana na kod Graya
			x--; //zmniejszenie wartości licznika o 1, przez co poruszamy się od 255 do 0
			break;
		case 5: //2x4 bitowy licznik w kodzie BCD zliczający w górę (0...99)
			jed = y%10; //obliczenie wartości jedności
			dzie = y/10; //obliczenie wartości dziesiątek
			display = (dzie << 4) + jed; //zamiana wartości dziesiątek i jedności na liczbę w kodzie BCD
			y++; //zwiększenie wartości licznika o 1
			break;
		case 6: //. 2x4 bitowy licznik w kodzie BCD zliczający w dół (99...0)
			jed = y%10; //obliczenie wartości jedności
			dzie = y/10; //obliczenie wartości dziesiątek
			display = (dzie << 4) + jed; //zamiana wartości dziesiątek i jedności na liczbę w kodzie BCD
			y--; //zmniejszenie wartości licznika o 1
			break;
		case 7: //3 bitowy wężyk poruszający się lewo-prawo
			display=waz; //wyswietlenie wartosci przechowywanej w zmiennej waz
			if (waz==224){ //jeśli wąż dojdzie do "brzegu" wyświetlacza
				k=1; //zmiana kierunku poruszania się węża
			}
			if (waz==7){ //jeśli wąż dojdzie do "brzegu" wyświetlacza
				k=0; //zmiana kierunku poruszania się węża
			}
			if (k==1){ //jeśli k=1
				waz=waz/2; //wąż idzie w dół
			}
			if (k==0){ //jeśli k=0
				waz=waz*2; //wąż idzie w górę
			}
			break;
		case 8: //Kolejka
			kolejka = kolejka - index; //obliczenie wartości zmiennej kolejka
			index = index << 1; //przesunięcie o 1 bit w lewo
			if(index == 0)index = 1; //reset wartości zmiennej index
			l--; //zmniejszenie licznika o 1
			kolejka = kolejka + index; //obliczenie wartości zmiennej kolejka
			if(l==0){ //jeśli l=0
				temp--; //odejmujemy 1 od zmiennej temp
				l=temp; //l przyjmuje wartość zmiennej temp
				index=0; //index wraca do wartości 0
			}
			display = kolejka; //wyświetlenie wartości zmiennej kolejka
			if(display == 255){ //jeśli display=255, czyli max to resetujemy wartości zmiennych
				l = 8;
				temp = 8;
				index = 0;
				kolejka = 0;
			}
			break;
		case 9: //6 bitowy generator liczb pseudolosowych oparty o konfigurację 1110011
	    	s1 = ktyBit(seed,1); //pierwszy bit liczby seed
	    	s2 = ktyBit(seed,2); //drugi bit liczby seed
	    	s5 = ktyBit(seed,5); //piąty bit liczby seed
	    	s6 = ktyBit(seed,6); //szósty bit liczby seed
	    	a = s1 ^ s2 ^ s5 ^ s6; //działanie XOR na wszystkich liczbach
	    	seed = bitShift(seed,1,1); //przesunięcie liczby seed o 1 miejsce w prawo
	    	if (a)
	    	{
	    		seed = seed | 32; //działanie OR
	    	}
	    	display = seed; //wyświetlenie wartości
			break;
	}
	goto again;
}
