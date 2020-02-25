#include "gol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TEMP_INPUT "temp_input_file"

int main(int argc, char* argv[]){
	struct universe v; 
	char* input_filename = NULL;
	char* output_filename = NULL;
	int number_of_generations = 5; // default number of evolutions is 5
	int set_number_of_generations = 0;
	int print_stats = 0; // print stats at the end?
	int torus = 0; // use the torus topology?
	for (int i = 1; i < argc; i++){
		if (strlen(argv[i])==2 && argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'i':
					if (i+1 < argc){
						if(!input_filename){
							input_filename = argv[i+1];
							if (strlen(input_filename) == 2 && input_filename[0] == '-' && (input_filename[1] == 'i' || input_filename[1] == 'o'
							 || input_filename[1] == 'g' || input_filename[1] == 's' || input_filename[1] == 't')){
								fprintf(stderr, "Error cannot give command line switch argument as input file - please specify an input file\n");
								exit(-1);
							 }
						} else {
							fprintf(stderr, "Error input file name already given - don't give the same arguments twice\n");
							exit(-1);
						}
					} else {
						fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
						exit(-1);
					}
					break;
				case 'o':
					if (i+1 < argc){
						if(!output_filename){
							output_filename = argv[i+1];
							if (strlen(output_filename) == 2 && output_filename[0] == '-' && (output_filename[1] == 'i' || output_filename[1] == 'o'
								|| output_filename[1] == 'g' || output_filename[1] == 's' || output_filename[1] == 't')){
								fprintf(stderr, "Error cannot give command line switch argument as output file - please specify an input file\n");
								exit(-1);
							}
						} else {
							fprintf(stderr, "Error output file name already given - don't give the same arguments twice\n");
							exit(-1);
						}
					} else {
						fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
						exit(-1);
					}
					break;
				case 'g':
					if (i+1 < argc){
						if (!set_number_of_generations){
							for (int j = 0 ; (unsigned) j < strlen(argv[i+1]); j++){
								if (!isdigit(argv[i+1][j])){
									fprintf(stderr, "Error number of generations needs to be numeric - please enter a valid number after -g\n");
									exit(-1);
								}
							}
							number_of_generations = atoi(argv[i+1]);								set_number_of_generations = 1;
						} else {
							fprintf(stderr, "Error number of generations already given - don't give the same arguments twice\n");
							exit(-1);
						}
					} else{
						fprintf(stderr, "Error invalid arguments - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n");
						exit(0);
					}
					break;
				case 's':
					if(!print_stats){
						print_stats = 1;
					} else {
						fprintf(stderr, "Error print stats already given - don't give the same arguments twice\n");
						exit(-1);
					}
					break;
				case 't':
					if (!torus){
						torus = 1;
					} else {
						fprintf(stderr, "Error torus topology already specified - don't give the same arguments twice\n");
						exit(-1);
					}
					break;
				default:
					fprintf(stderr, "Error unknown argument %c - usage: ./gameoflife [-i <input_file> | -o <output_file> | -g <number of evolutions> | -s | -t]\n", argv[i][1]);
					exit(-1);
			}
			
		}
	}
	
	if (input_filename != NULL){ // open file and read in the input if given
		FILE* fp = fopen(input_filename, "r");
		read_in_file(fp,&v);
		fclose(fp);
	} else { // take user input otherwise
		FILE* fp = stdin; 
		read_in_file(fp,&v); // set fp to stdin and read in
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
