//Basic Latin Alphabet Shift JS decoder
#include <stdio.h>
#include "shiftjs.h"

int decode(unsigned char *mc, int offset){

for(int i=0; i < 128; i+=2){
	switch(mc[offset + i]){
		case 0x81: //Symbols
			switch(mc[offset + i + 1]){
				case 0x40:
					printf(" ");
					break;
				case 0x46:
					printf(":");
					break;
				case 0x5e:
					printf("/");
					break;
				case 0x5f:
					printf("\\");
					break;
				case 0x6d:
					printf("[");
					break;
				case 0x6e:
					printf("]");
					break;
				case 0x71:
					printf("<");
					break;
				case 0x72:
					printf(">");
					break;
				case 0x7c:
					printf("-");
					break;
			}
			break;
		case 0x82:
			if(mc[offset + i +1] < 0x80){printf("%c", mc[offset + i + 1] - 0x1f); break;} //Capital Letters & Numbers
			if(mc[offset + i +1] < 0x9c){printf("%c", mc[offset + i + 1] - 0x20); break;} //Lower Case Letters
			break;
		case 0x83:
			break;

		}

	}
}

