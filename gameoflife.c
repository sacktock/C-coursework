#include "gol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[]){
	struct universe v; 
	char* input_filename = NULL;
	char* output_filename = NULL;
	int number_of_generations = 5; // default number of evolutions is 5
	int set_number_of_generations = 0;
	int print_stats = 0; // print stats at the end?
	int torus = 0; // use the torus topology?
	int arg_flag = 0;
	for (int i = 1; i < argc; i++){
		if (strlen(argv[i])==2 && argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'i': // if i try to parse the input file
					if (i+1 < argc){
						if(!input_filename || !strcmp(argv[i+1], input_filename)){
							input_filename = argv[i+1];
							arg_flag = 1;
						} else { // conflicting arguments
							fprintf(stderr, "Error conflicting arguments - don't give the same arguments different values\n");
							exit(-1);
						}
					} else { // filename misisng
						fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
						exit(-1);
					}
					break;
				case 'o':
					if (i+1 < argc){ // if o try to parse the output file
						if(!output_filename || !strcmp(argv[i+1], output_filename)){
							output_filename = argv[i+1];
							arg_flag = 1;
						} else { // conflicting arguments
							fprintf(stderr, "Error conflicting arguments - don't give the same arguments different values\n");
							exit(-1);
						}
					} else { // filename missing
						fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
						exit(-1);
					}
					break;
				case 'g': // if g try to parse the following numeric argument
					if (i+1 < argc){
						for (int j = 0 ; (unsigned) j < strlen(argv[i+1]); j++){
							if (!isdigit(argv[i+1][j])){ // check following argument is numeric
								fprintf(stderr, "Error number of generations needs to be numeric - please enter a valid number after -g\n");
								exit(-1);
							}
						}
						if (!set_number_of_generations || atoi(argv[i+1]) == number_of_generations){ // check the number of generations has not already been specified
							number_of_generations = atoi(argv[i+1]);								
							set_number_of_generations = 1;
							arg_flag = 1;
						} else {
							fprintf(stderr, "Error conflicting arguments - don't give the same arguments different values\n");
							exit(-1);
						}
					} else{ // following argument missing
						fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
						exit(0);
					}
					break;
				case 's': // set the print statistics flag to 1
					arg_flag = 0;
					print_stats = 1;
					break;
				case 't': // set the torus flag to 1
					arg_flag = 0;
					torus = 1;
					break;
				default: // if we get an unexpected argument throw error
					if (!arg_flag){
						fprintf(stderr, "Error unknown argument %c - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n", argv[i][1]);
						exit(-1);
					} else {
						arg_flag = 0;
					}	
			}
		} else {
			if (!arg_flag){
				fprintf(stderr, "Error unxpected argument %s - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n", argv[i]);
				exit(-1);
			} else {
				arg_flag = 0;
			}
		}
	}
	
	if (input_filename != NULL){ // open file and read in the input if given
		FILE* fp = fopen(input_filename, "r");
		read_in_file(fp,&v);
		fclose(fp);
	} else { // take user input otherwise 
		read_in_file(stdin,&v); // set fp to stdin and read in
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
		write_out_file(stdout,&v);
	}
	
	if(print_stats){ // print stats
		print_statistics(&v); 
	}
	return 0;	  
}
