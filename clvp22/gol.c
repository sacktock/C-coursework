#include "gol.h"
#include <stdio.h>
#include <stdlib.h>

void read_in_file(FILE *infile, struct universe *u){
	
	/////////////////////////////////////////////
	// checking for any exceptions
	if (u == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe pointer is NULL\n"); 
        exit(-1);
	}
	if (infile == NULL)
	{
		fprintf( stderr,"NULL pointer exception - file pointer is NULL\n");  
        exit(-1); 
	}
	/////////////////////////////////////////////
	
	int rows = 0, columns = 0, count = 0, row = 0, column = 0;
	char ch, check_ch;
	
	while(!feof(infile))
	{
		ch = fgetc(infile);
		if (ch == '\n')
		{
			if (rows == 0){
				columns = count;
			}
			rows++;
			if (count != columns || columns == 0){
				fprintf( stderr,"Invalid input file format exception - rows are not equal lengths\n");  
				exit(-1); 
			}
			count = 0;
			check_ch = ch;
		} else if (ch == '.' || ch == '*'){
			count++;
			if (count > 512){
				fprintf( stderr,"Invalid input file format exception - row length exceeds 512 characters\n");  
				exit(-1); 
			}
			check_ch = ch;
		} else if (ch == -1){
			if (check_ch != '\n'){
				fprintf( stderr,"Invalid input file format exception - check file or user input ends with a newline character before EOF \n");  
				exit(-1);
			}
		} else {
			fprintf( stderr,"Invalid input file format exception - unexpected character\n");  
			exit(-1);
		}
	}
	
	int** array;
	array = malloc(rows * sizeof(*array)); /* Assuming `n` is the number of rows */
	if(!array) /* If `malloc` failed */
	{
	    fprintf( stderr,"Error allocating memory\n");
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

	        fprintf( stderr,"Error allocating memory\n");
	        exit(-1);
	    }
	}
	
	count = 0;
	rewind(infile);

	while(!feof(infile)){
		ch = fgetc(infile);
		if (ch == '\n')
		{
			row++;
			column=0;
		} else if (ch == '*'){
			count++;
			array[row][column] = 1;
			column++;
		} else if (ch == '.'){
			array[row][column] = 0;
			column++;
		} else if (ch != -1){
			exit(-1);
		}
	}
	u->gen_num = 1;
	u->total = count;
	u->array = array;
	u->rows = rows;
	u->columns = columns;
}

void write_out_file(FILE *outfile, struct universe *u){
	
	/////////////////////////////////////////////
	// checking for any exceptions
	if (outfile == NULL)
	{
		fprintf( stderr,"NULL pointer exception - file pointer is NULL\n"); 
        exit(-1);  
	}
	if (u == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe pointer is NULL\n"); 
        exit(-1);  
	}
	if (u->array == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe array pointer is NULL\n"); 
        exit(-1);  
	}
	/////////////////////////////////////////////
	
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
	
	/////////////////////////////////////////////
	// checking for any exceptions
	if (u == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe pointer is NULL\n"); 
        exit(-1);  
	}
	if (u->array == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe array pointer is NULL\n"); 
        exit(-1);  
	}	
	if (column >= u->columns || row >= u->rows || column < 0 || row < 0){
		fprintf( stderr,"Index out of bounds exception\n"); 
		exit(-1); 
	}
	/////////////////////////////////////////////
	
	return u->array[row][column];
}

int will_be_alive(struct universe *u, int column, int row){
	
	/////////////////////////////////////////////
	// checking for any exceptions
	if (u == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe pointer is NULL\n"); 
        exit(-1);  
	}
	if (u->array == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe array pointer is NULL\n"); 
        exit(-1);  
	}
	if (column >= u->columns || row >= u->rows || column < 0 || row < 0){
		fprintf( stderr,"Index out of bounds exception\n"); 
		exit(-1); 
	}
	/////////////////////////////////////////////
	
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
		if((count == 3) || (count == 2)){
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
	
	/////////////////////////////////////////////
	// checking for any exceptions
	if (u == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe pointer is NULL\n"); 
		exit(-1);  
	}
	if (u->array == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe array pointer is NULL\n"); 
        exit(-1);  
	}
	if (column >= u->columns || row >= u->rows || column < 0 || row < 0){
		fprintf( stderr,"Index out of bounds exception\n"); 
		exit(-1); 
	}
	/////////////////////////////////////////////
	
	int count = 0;
	count += u->array[(row+1) % (u->rows)][(column+1) % (u->columns)];
	count += u->array[(row) % (u->rows)][(column+1) % (u->columns)];
	count += u->array[(row-1 + u->rows) % (u->rows)][(column+1) % (u->columns)];
	count += u->array[(row+1) % (u->rows)][(column) % (u->columns)];
	count += u->array[(row-1 + u->rows) % (u->rows)][(column) % (u->columns)];
	count += u->array[(row+1) % (u->rows)][(column-1 + u->columns) % (u->columns)];
	count += u->array[(row) % (u->rows)][(column-1 + u->columns) % (u->columns)];
	count += u->array[(row-1 + u->rows) % (u->rows)][(column-1 + u->columns) % (u->columns)];
	if (is_alive(u, column, row)){
		if((count == 3) || (count == 2)){
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
	
	/////////////////////////////////////////////
	// checking for any exceptions
	if (u == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe pointer is NULL\n"); 
        exit(-1);  
	}
	if (u->array == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe array pointer is NULL\n"); 
        exit(-1);  
	}
	if (u->rows == 0 || u->columns == 0)
	{
		fprintf(stderr, "Undefined univserse exception - rows or columns equal to zero\n");
		exit(-1);
	}
	/////////////////////////////////////////////
	
	int count = 0;
	int array[u->rows][u->columns];
	for(int i = 0; i < u->rows; i++){
		for(int j = 0; j < u->columns; j++){
			array[i][j] = (*rule)(u,j,i);
			count += array[i][j];
		}
	}
	
	for(int i = 0; i < u->rows; i++){
		for(int j = 0; j < u->columns; j++){
			u->array[i][j] = array[i][j];
		}
	}
	
	u->total += count;
	u->gen_num++;
}

void print_statistics(struct universe *u){
	
	/////////////////////////////////////////////
	// checking for any exceptions
	if (u == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe pointer is NULL\n"); 
        exit(-1);  
	}
	if (u->array == NULL)
	{
		fprintf( stderr,"NULL pointer exception - universe array pointer is NULL\n"); 
        exit(-1);  
	}
	if (u->rows == 0 || u->columns == 0)
	{
		fprintf(stderr, "Undefined univserse exception - rows or columns equal to zero\n");
		exit(-1);
	}
	/////////////////////////////////////////////
	
	double count = 0;
	for(int i = 0; i < u->rows; i++){
		for(int j = 0; j < u->columns; j++){
			if (u->array[i][j] == 1){
				count += 1.0;
			}
		}
	};
	double avg_alive = (count / ((u->rows) * (u->columns)))*100.0;
	double avg = ((double)u->total / (double)((u->rows) * (u->columns) * (u->gen_num)))*100.0;
	fprintf(stdout, "%.3f%% of cells currently alive\n",avg_alive);
	fprintf(stdout, "%.3f%% of cells alive on average\n ",avg);
}