#include "gol.h"
#include <stdio.h>
#include <stdlib.h>	/* for atof() */
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>

void read_in_file(FILE *infile, struct universe *u){
	int rows = 0, columns = 0, count = 0, row = 0, column = 0;
	char ch;
	
	if (infile == NULL)
	{
		fprintf( stderr,"NULL file"); 
        return; 
	}
	
	while(!feof(infile))
	{
		ch = fgetc(infile);
		if (ch == '\n')
		{
			rows++;
		} else {
			count++;
		}
	}
	
	columns = count / rows;
	rewind(infile);
	
	int** array;
	array = malloc(rows * sizeof(*array)); /* Assuming `n` is the number of rows */
	if(!array) /* If `malloc` failed */
	{
	    fprintf( stderr,"Error allocating memory");
	    exit(-1);
	}
	
	for(int i = 0; i < rows; i++)
	{
	    array[i] = malloc(columns * sizeof(**array));
	    if(!array[i]) /* If `malloc` failed */
	    {
	        for(int j = 0; j < i; j++) /* free previously allocated memory */
	        {
	            free(array[j]); 
	        }
	        free(array);

	        fprintf( stderr,"Error allocating memory");
	        exit(-1);
	    }
	}
	count = 0;
	while(!feof(infile)){
		ch = fgetc(infile);
		if (ch == '\n')
		{
			row++;
			column=0;
		} else {
			if (ch == '*'){
				count++;
				array[row][column] = 1;
			} else {
				array[row][column] = 0;
			}
			column++;
		}
	}
	u->gen_num = 1;
	u->total = count;
	u->array = array;
	u->rows = rows;
	u->columns = columns;
}

void write_out_file(FILE *outfile, struct universe *u){
	if (outfile == NULL)
	{
		fprintf( stderr,"NULL file"); 
        return; 
	}
	for(int i = 0; i < u->rows; i++){
		for(int j = 0; j < u->columns; j++){
			if (u->array[i][j] == 1){
				fputc('*', outfile);
			} else {
				fputc('.', outfile);
			}
			
		}
		fputc('\n', outfile);
	}
}

int is_alive(struct universe *u, int column, int row){
	return u->array[row][column];
}

int will_be_alive(struct universe *u, int column, int row){
	int count = 0;
	if (row+1 < u->rows){
		if (column+1 < u->columns){
			count += u->array[(row+1)][(column+1)];
			
		}
		if (column-1 >= 0){
			count += u->array[(row+1)][(column-1)];
		}
		count += u->array[(row+1)][(column)];
	}
	if (row-1 >= 0){
		if (column+1 < u->columns){
			count += u->array[(row-1)][(column+1)];
		}
		if (column-1 >= 0){
			count += u->array[(row-1)][(column-1)];
		}
		count += u->array[(row-1)][(column)];
	}
	if (column+1 < u->columns){
		count += u->array[row][(column+1)];
	}
	if (column-1 >= 0){
		count += u->array[row][(column-1)];
	}
	if (is_alive(u, column, row)){
		if((count == 3) | (count == 2)){
			return 1;
		} else {
			return 0;
		}
	} else {
		if(count == 3){
			return 1;
		}else {
			return 0;
		}
	}
}

int will_be_alive_torus(struct universe *u,  int column, int row){
	int count = 0;
	count += u->array[(row+1) % u->rows][(column+1) % u->columns];
	count += u->array[row % u->rows][(column+1) % u->columns];
	count += u->array[(row-1) % u->rows][(column+1) % u->columns];
	count += u->array[(row+1) % u->rows][(column) % u->columns];
	count += u->array[(row) % u->rows][(column) % u->columns];
	count += u->array[(row+1) % u->rows][(column-1) % u->columns];
	count += u->array[row % u->rows][(column-1) % u->columns];
	count += u->array[(row-1) % u->rows][(column-1) % u->columns];
	if (is_alive(u, column, row)){
		if((count == 3) | (count == 2)){
			return 1;
		} else {
			return 0;
		}
	} else {
		if(count == 3){
			return 1;
		}else {
			return 0;
		}
	}
}
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)){
	int count = 0;
	for(int i = 0; i < u->rows; i++){
		for(int j = 0; j < u->columns; j++){
			u->array[i][j] = (*rule)(u,i,j);
			count += u->array[i][j];
		}
	}
	u->total += count;
	u->gen_num++;
}

void print_statistics(struct universe *u){
	double count = 0;
	for(int i = 0; i < u->rows; i++){
		for(int j = 0; j < u->columns; j++){
			count += u->array[i][j];
		}
	};
	double avg_alive = count / ((u->rows) * (u->columns));
	double avg = u->total / ((u->rows) * (u->columns) * (u->gen_num));
	fprintf(stdout, "%f percent of cells currently alive",avg_alive);
	fprintf(stdout, "%f percent of cells alive of average ",avg);
}