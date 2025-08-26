/*
PSX Memory Card Reader
Memory Cards Files and the Block contained within that are used in this program are courtesy of Sony Computer Entertainment and the respective game's developers
*/
#include <stdio.h>
#include <string.h>
#include "shiftjs/shiftjs.h"

//flags
bool graphics_flag = true;
bool debug_flag = false;
int cflag = 0;
bool show_deleted_blocks = true;
int memory_card_format = 0;
int offset = 0;


int directory(unsigned char *Memory_Card, unsigned char dirlist[15]){
	int UB = 0; int DB = 0; int FB = 0;
	for(int b = 1;b < 16; b++){
		switch (Memory_Card[offset + (b*0x80)]){
		//Active Blocks
		case 0x51: // First/Single Block
			dirlist[b] = 0x51;
			UB++;
			break;
		case 0x52: // Middle Block
			dirlist[b] = 0x52;
			UB++;
			break;
		case 0x53: // Ending Block
			dirlist[b] = 0x53;
			UB++;
			break;
		//Deleted & Formatted Blocks
		case 0xA1: //Deleted First Block
			dirlist[b] = 0xA1;
			DB++;
			break;
		case 0xA2:
			dirlist[b] = 0xA2;
			DB++;
			break;
		case 0xA3: //Deleted Ending Block
			   dirlist[b] = 0xA3;
			   DB++;
			   break;
		case 0xA0: //Formatted
			dirlist[b] = 0x01;
			FB++;
			break;
		}
	}
	printf("--Free and Used Blocks--\n");
	printf("%d Blocks Used\n", UB);
	printf("%d Blocks Deleted\n", DB);
	printf("%d Blocks Formatted\n", FB);

	printf("\n--Directory--\n");
	if(debug_flag == true){
	for(int x = 1;x < 16; x++ ){
	printf("%d, 0x%x, %x\n", x, x*0x80,  dirlist[x]);
	}
	printf("\n");
	}
	

}

int icondraw(unsigned char *Memory_Card, int blocknumber){
	unsigned int R8[17];
	unsigned int G8[17];
	unsigned int B8[17];
   	unsigned char pixel1;
	unsigned char pixel2;
	unsigned int colortable555[129] = {0};
	for(int c = 0; c < 16; c++){
	colortable555[c] = Memory_Card[offset + (0x2000 * blocknumber) + 0x61 + c * 2] << 8;
	colortable555[c] += Memory_Card[offset + (0x2000 * blocknumber) + 0x60 + (c * 2)];
	R8[c] = (((colortable555[c] & 31) * 527) + 23) >> 6;
	G8[c] = ((((colortable555[c] >> 5) & 31) * 527) + 23) >> 6;
	B8[c] = ((((colortable555[c] >> 10) & 31) * 527) + 23) >> 6;
	}
	printf("\e[7m");
	int wrap = 0;
	for(int p = 0; p < 0x80; p++){
		if(wrap > 7){
			printf("\n	");
			wrap = 0;
		}
		//Get Pixels
		pixel1 = Memory_Card[offset + (0x2000 * blocknumber) + 0x80 + p] & 0b00001111;
		pixel2 = Memory_Card[offset + (0x2000 * blocknumber) + 0x80 + p] & 0b11110000; pixel2 = pixel2 >> 4;
		//Draw Pixels
		printf("\e[7m\e[38;2;%d;%d;%dm  ", R8[pixel1], G8[pixel1], B8[pixel1]);
		printf("\e[7m\e[38;2;%d;%d;%dm  ", R8[pixel2], G8[pixel2], B8[pixel2]);
		wrap++;
	}
	if(cflag == 1){
		for(int c = 0; c < 15; c++){
		printf("\e[7m\e[38;2;%d;%d;%dm  ", R8[c], G8[c], B8[c]);
		}
	}
	printf("\033[27m\033[38;5;15m\033[48;5;16m");
}


int main(int argc, char *argv[]){
//File Checks
	if(argv[1] == NULL){
		printf("Error: No file has been given to anlyze! \n");
		return 0;
	}
	if(argv[1][0] == 'h' && strlen(argv[1]) == 1){
	printf("psxmem *Mem Card File* c d g s\nd = Debug Mode (displays more infomration) \ng = No Graphics (doesn't display icons)\nc = Color Palette enable (displays color palette strip)\ns = Show Deleted Blocks\nProgram by Enzo_a_Person\n");
	return 0;
	}

//Putting file into memory
	//Creating Variables
	FILE * file;
	char *filename = argv[1];
	file = fopen (argv[1], "rb");

	//Checking file is a valid
	if (file == NULL){
		printf("Error: MC file does not exist! \n");
		return 1;
	}
	
	if(argc > 1){
		for(int a = 2; a < argc; a++){
		if(strlen(argv[a]) == 1){
			switch(argv[a][0]){
				case 'c':
				case 'C':
					cflag = 1;
					break;
				case 'd':
				case 'D':
					debug_flag = true;
					break;
				case 'g':
				case 'G':
					graphics_flag = false;
					break;
				case 's': 
				case 'S':
					 show_deleted_blocks = false;
					 break;
			}

		}
		}
	}
	//Putting Card into Memory
	unsigned char Memory_Card[0x20088];
	fread(Memory_Card, 1, sizeof(Memory_Card), file);

	
	//Header Check
	if(Memory_Card[0] != 'M' && Memory_Card[1] != 'C'){
		if(debug_flag == true){printf("Not a Raw PS MC\n");}
		if(Memory_Card[1] != 'P' && Memory_Card[2] != 'M' && Memory_Card[3] != 'V'){
		printf("Error: Memory Card's header is invalid or unknown Memory Card file type! \n");
		return 1;
		}
		memory_card_format = 1;
		offset = 0x80;
	}
	printf("Format detected: ");
	if(memory_card_format == 0){
	printf("Raw PS1 Memory Card File\n\n");
	}else{
	printf("PSP Memory Card File\n\n");
	}

	//Getting Directory List
	char dirlist[15]; // <-- 15 blocks is the maximum that a PS1 Mem Card can have
	directory(Memory_Card, dirlist);	

	//Displaying information

	for(int b = 1; b < 16; b++){
		if(show_deleted_blocks == false || dirlist[b] == 0x51 || dirlist[b] == 0x52 || dirlist[b] == 0x53){
			if(dirlist[b] != 0x01){
				printf("%d. ", b);
				if((dirlist[b] & 0xA0) == 0xA0){
					printf("\e[7m");
					printf("\033[1;31;47mDELETED BLOCK!!! - ");	
				}
				for(int l = 0; l < 20; l++){
					printf("%c", Memory_Card[(0x80 * b) + (0xA + l) + offset]);
				}
				printf("\n");
				//Shift JS Decoding
				decode(Memory_Card, (0x2000 * b+4) + offset);
				printf("\n");
				if(graphics_flag == true){
					printf("	");
					icondraw(Memory_Card, b);
					printf("\n");
				}
			printf("\033[27m\033[38;5;15m\033[48;5;16m");

			}
		}
	}
}

