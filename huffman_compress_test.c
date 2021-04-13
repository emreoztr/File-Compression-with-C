#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "HuffmanCompress.h"

void main(){
	int *index=(int*)malloc(sizeof(int));
	char encrs[300][20]={'\0'}, temp;
	char *f_name="reference.txt"; /*where characters and their frequencies come*/
	char *f_message="reference.txt"; /*text which will be compressed*/
	char *bf_name="encoded.dat";	/*name of the compressed file*/
	char *df_name="decoded.txt";	/*name of the opened file*/
	hoffman *m;

	(*index)=0;
	m=read_file(f_name); /*taking the characters and frequencies*/

	bit_codes(m, 0, index, encrs); /*makind the codes for each character*/
	free(index);
	
	for(int i=0; encrs[i][0]!='\0'; i++){
		temp=encrs[i][strlen(encrs[i])-1];
		encrs[i][strlen(encrs[i])-1]='\0';
		encrs[i][strlen(encrs[i])-1]='\0';
		shift_right_char(encrs[i]);
		shift_right_char(encrs[i]);
		encrs[i][0]=temp;
		encrs[i][1]='!';
	}
	for(int i=0; encrs[i][0]!='\0'; i++){
		printf("%s\n", encrs[i]);
	}

	write_encrfile(encrs, f_message, bf_name);	/*writing the compressed file*/
	read_encrfile(encrs, bf_name, df_name);	/*reading and encoding the compressed file*/
	calculate_size_diff(f_message, bf_name); /*showing the differences*/
}

