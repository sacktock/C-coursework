#include "gol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEMP_INPUT "temp_input_file"

int main(int argc, char* argv[]){
	struct universe v; 
	char* input_filename = NULL;
	char* output_filename = NULL;
	int number_of_generations = 5; // default number of evolutions is 5
	int print_stats = 0; // print stats at the end?
	int torus = 0; // use the torus topology?
	for (int i = 1; i < argc; ++i){
		if (!strcmp(argv[i],"-i")){ // parse input file argument if possible
			if (i+1 < argc){
				input_filename = argv[i+1];
			} else {
				fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
				exit(-1);
			}
		}
		if (!strcmp(argv[i], "-o")){ // parse output file argument if possible
			if (i+1 < argc){
				output_filename = argv[i+1];
			} else {
				fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
				exit(-1);
			}
		}
		if (!strcmp(argv[i],"-g")){ // parse the number of generations / evolutions argument if possible
			if (i+1 < argc){
				number_of_generations = atoi(argv[i+1]);
				if (!number_of_generations){
					fprintf(stdout, "WARNING: number of evolutions set to 0 - this could be because the argument could not be parsed or you entered 0.\n");
				}
				
			} else{
				fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
				exit(0);
			}
			
		}
		if (!strcmp(argv[i],"-s")){ // show stats at the end set to 1 / true
			print_stats = 1;
		}
		if (!strcmp(argv[i],"-t")){ // use the torus topology set to 1 / true
			torus = 1;
		}
	}
	
	if (input_filename != NULL){ // open file and read in the input if given
		FILE* fp = fopen(input_filename, "r");
		read_in_file(fp,&v);
		fclose(fp);
	} else { // take user input otherwise
		FILE* fp = fopen(TEMP_INPUT, "w"); // open temporary file
		char ch;
		while (!feof(stdin)){ // read user input until EOF / CTRL+D
			ch = fgetc(stdin); // get next char
			fputc(ch, fp); // write char to temporary input file
		}
		fclose(fp); // close temporary file
		fp = fopen(TEMP_INPUT, "r"); // open temporary file for reading
		read_in_file(fp,&v); // read in the file
		remove(TEMP_INPUT); // remove the temporary file
		fclose(fp); // close file temporary file pointer
	}
	
	for (int i = 0; i < number_of_generations; i++){ // evolve the universe i number of times
		if (torus){
			evolve(&v,will_be_alive_torus);
		}else{
			evolve(&v,will_be_alive);
		}
	}
	
	if (output_filename != NULL){ // open the output file and write output if given
		FILE* op = fopen(output_filename, "w");
		write_out_file(op,&v);
		fclose(op);	 	
	} else { // else print to the stdout 
		for(int i = 0; i < v.rows; i++){
			for(int j = 0; j < v.columns; j++){ // iterate through the grid
				if (v.array[i][j] == 1){
					printf("*"); 
				} else {
					printf(".");
				}
			}
			printf("\n");
		}
	}
	
	if(print_stats){ // print stats
		print_statistics(&v); 
	}
	return 0;	  
}
