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
	char* in_file_name = NULL;
	char* out_file_name = NULL;
	int number_of_generations = 5;
	int print_stats = 0; // print stats at the end?
	int torus = 0; // use the torus topology ?
	for (int i = 1; i < argc; ++i){
		if (!strcmp(argv[i],"-i")){
			if (i+1 < argc){
				in_file_name = argv[i+1];
			} else {
				fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
				exit(0);
			}
		}
		if (!strcmp(argv[i], "-o")){
			if (i+1 < argc){
				out_file_name = argv[i+1];
			} else {
				fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
				exit(0);
			}
		}
		if (!strcmp(argv[i],"-g")){
			if (i+1 < argc){
				number_of_generations = atoi(argv[i+1]);
				if (!number_of_generations){
					fprintf(stdout, "WARNING: number of evolutions set to 0 - this could be because the argument could not be parsed or you entered 0.");
				}
				
			} else{
				fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
				exit(0);
			}
			
		}
		if (!strcmp(argv[i],"-s")){
			print_stats = 1;
		}
		if (!strcmp(argv[i],"-t")){
			torus = 1;
		}
	}
	if (in_file_name != NULL){
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
	
	if (out_file_name != NULL){
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
