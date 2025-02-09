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
	
	int rows = 0, columns = 0, count = 0, row = 0, column = 0; // delcare variables
	char ch, check_ch;
	
	while(!feof(infile)) // read file char by char until EOF
	{
		ch = fgetc(infile); // get next char
		if (ch == '\n')
		{
			if (rows == 0){
				columns = count; // set columns to the number of chars seen on the first line so far
			}
			rows++; // if newline increment rows
			if (rows == 1 && columns == 0){ // we have an empty file so throw error
				fprintf( stderr,"Invalid input file format exception - empty file\n");  
				exit(-1);
			}
			if (count != columns || columns == 0){ 
				fprintf( stderr,"Invalid input file format exception - rows are not equal lengths\n");  
				exit(-1); 
			}
			count = 0; // set count to 0
			check_ch = ch; // set the check char to ch
		} else if (ch == '.' || ch == '*'){
			count++; // increment count to count the amount of chars on the current line
			if (count > 512){
				fprintf( stderr,"Invalid input file format exception - row length exceeds 512 characters\n");  
				exit(-1); 
			}
			check_ch = ch; // set the check char to ch
		} else if (ch == -1){
			if (check_ch != '\n'){ // if we get EOF and the previous char was not a new line this is invalid
				fprintf( stderr,"Invalid input file format exception - check file or user input ends with a newline character before EOF \n");  
				exit(-1);
			}
		} else if (ch == 13) {
			// do nothing if we get a carriage return only needs to be handled on linux
		} else { // if we get a character that is not newline, EOF, * or . then print error
			fprintf( stderr,"Invalid input file format exception - unexpected character %c ASCII %d\n",ch,ch);  
			exit(-1);
		}
	}
	
	if (rows < 3 || columns < 3){ // we expect atleast a 3 x 3 grid so that all cells have 8 neighbours
		fprintf( stderr,"Invalid input file format exception - expected atleast a 3 x 3 grid\n");  
		exit(-1);
	}
	
	int** array;
	array = malloc(rows * sizeof(*array));// assign memeory for the amount of rows
	if(!array) // if malloc failed print error and exit
	{
	    fprintf( stderr,"Error allocating memory\n");
	    exit(-1);
	}
	for(int i = 0; i < rows; i++)
	{
	    array[i] = malloc(columns * sizeof(**array)); // assign memeory for each column
	    if(!array[i]) // if malloc failed print error and exit
	    {
	        for(int j = 0; j < i; j++) // free previously assigned memory
	        {
	            free(array[j]); 
	        }
	        free(array);

	        fprintf( stderr,"Error allocating memory\n");
	        exit(-1);
	    }
	}
	
	count = 0; // set count to 0 to count the number of alive cells
	rewind(infile); // move file pointer to the start

	while(!feof(infile)){ // read file char by char until EOF
		ch = fgetc(infile); // get next char
		if (ch == '\n')
		{
			row++; // move to next row if newline
			column=0;
		} else if (ch == '*'){ // if cell is alive
			count++; // increment count
			array[row][column] = 1; // set to 1 or 'true'
			column++; // move to the next column
		} else if (ch == '.'){ // if cell is dead
			array[row][column] = 0; // set to 0 or 'false'
			column++; // move to the next column
		} else if (ch == 13){
			// do nothing if we get a carriage return only needs to be handled on linux
		} else if (ch != -1){ // if we see any other char that is not EOF exit 
			fprintf(stderr, "Unexpected error occured");
			exit(-1);
		}
	}
	// assign values to the universe struct
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
		// iterate through the array and write char by char to the output file
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
	
	return u->array[row][column]; // 1 is alive 0 is dead
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
	
	// count the number of alive neighbours
	// assume neighbours outside the finite grid are dead
	int count = 0;
	if (row+1 < u->rows){ // make sure row+1 is inside the grid
		if (column+1 < u->columns){ // makse sure column+1 is inside the grid
			count += u->array[(row+1)][(column+1)];
		}
		if (column-1 >= 0){ // make sure column-1 is inside the grid
			count += u->array[(row+1)][(column-1)];
		}
		count += u->array[(row+1)][(column)];
	}
	if (row-1 >= 0){ // make sure row-1 is insid ethe grid
		if (column+1 < u->columns){ // make sure column+1 is inside the grid
			count += u->array[(row-1)][(column+1)];
		}
		if (column-1 >= 0){ // make sure column-1 is inside the grid
			count += u->array[(row-1)][(column-1)];
		}
		count += u->array[(row-1)][(column)];
	}
	if (column+1 < u->columns){ // make sure column+1 is inside the grid
		count += u->array[row][(column+1)];
	}
	if (column-1 >= 0){ // make sure column-1 is inside the grid
		count += u->array[row][(column-1)];
	}
	if (is_alive(u, column, row)){ // if cell is alive now then 
		if((count == 3) || (count == 2)){ // cell is alive next generation if 2 or 3 neighbours are alive now
			return 1;
		} else {
			return 0;
		}
	} else { // if cell is dead now then
		if(count == 3){ // cell is alive next generation if exactly 3 neighbours are alive now
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
	
	// count the number of alive neighbours
	// assume torus topology so use modular division
	int count = 0;
	count += u->array[(row+1) % (u->rows)][(column+1) % (u->columns)];
	count += u->array[(row) % (u->rows)][(column+1) % (u->columns)];
	count += u->array[(row-1 + u->rows) % (u->rows)][(column+1) % (u->columns)];
	count += u->array[(row+1) % (u->rows)][(column) % (u->columns)];
	count += u->array[(row-1 + u->rows) % (u->rows)][(column) % (u->columns)];
	count += u->array[(row+1) % (u->rows)][(column-1 + u->columns) % (u->columns)];
	count += u->array[(row) % (u->rows)][(column-1 + u->columns) % (u->columns)];
	count += u->array[(row-1 + u->rows) % (u->rows)][(column-1 + u->columns) % (u->columns)];
	if (is_alive(u, column, row)){ // if cell is alive now then 
		if((count == 3) || (count == 2)){ // cell is alive next generation if 2 or 3 neighbours are alive now
			return 1;
		} else {
			return 0;
		}
	} else { // if cell is dead now then
		if(count == 3){ // cell is alive next generation if exactly 3 neighbours are alive now
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
	int array[u->rows][u->columns]; // initialize a new array with the same dimensions
	for(int i = 0; i < u->rows; i++){
		for(int j = 0; j < u->columns; j++){ // iterate through the grid
			array[i][j] = (*rule)(u,j,i); // save if cell is alive in the next generation
			count += array[i][j]; // count the alive cells
		}
	}
	
	for(int i = 0; i < u->rows; i++){
		for(int j = 0; j < u->columns; j++){ // iterate through the grid
			u->array[i][j] = array[i][j]; // copy the alive values into the universe array
		}
	}
	
	u->total += count; // add the currently alive cells to the total
	u->gen_num++; // increment the generation number
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
		for(int j = 0; j < u->columns; j++){ // iterate through the grid
			if (u->array[i][j] == 1){
				count += 1.0; // count alive cells
			}
		}
	};
	double avg_alive = (count / ((u->rows) * (u->columns)))*100.0; // calculate average currently alive cells
	double avg = ((double)u->total / (double)((u->rows) * (u->columns) * (u->gen_num)))*100.0; // calculate the total average alive cells
	fprintf(stdout, "%.3f%% of cells currently alive\n",avg_alive);
	fprintf(stdout, "%.3f%% of cells alive on average\n ",avg);
}