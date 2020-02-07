#include "gol.h"
#include <stdio.h>
#include <stdlib.h>	/* for atof() */
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1000

int main(int argc, char* argv[]){
	struct universe v; 
	  
	char* in_file_name;
	char* out_file_name;
	int number_of_generations=5;
	int print_stats = 0; // print stats at the end?
	int torus = 0; // use the torus topology ?
	for (int i = 1; i < argc; i++){
		if (strcmp(argv[i],"-i")){
			in_file_name = argv[i+1];
		}
		if (strcmp(argv[i], "-o")){
			out_file_name = argv[i+1];
		}
		if (strcmp(argv[i],"-g")){
			number_of_generations = atoi(argv[i+1]);
		}
		if (strcmp(argv[i],"-s")){
			print_stats = 1;
		}
		if (strcmp(argv[i],"-t")){
			torus = 1;
		}
	}
		
		  
	if (in_file_name){
		FILE* fp = fopen(in_file_name, "r");
		read_in_file(fp,&v);
		fclose(fp);
	} else {
		  // let user input
	}
	  
	  
	for (int i = 0; i < number_of_generations; i++){
		if (torus){
			evolve(&v,will_be_alive_torus);
		}else{
			evolve(&v,will_be_alive);
		}
	}
	
	if (out_file_name){
		FILE* op = fopen(out_file_name, "w");
		write_out_file(op,&v);
		fclose(op);	  
	} else {
		for(int i = 0; i < v.rows; i++){
			for(int j = 0; j < v.columns; j++){
				if (v.array[i][j] == 1){
					printf("*");
				} else {
					printf(".");
				}
			}
			printf("\n");
		}
	}
	
	if(print_stats){
		print_statistics(&v);
	}
	return 0;	  
}
