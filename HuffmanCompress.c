#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "HuffmanCompress.h"

void sort_hoffarr(hoffman *arr){
	hoffman *min, temp;
	for(int i=0; arr[i].freq!=-1; i++){
		for(int j=i; arr[j].freq!=-1; j++){
			if(j==i) min=&arr[j];
			else if(arr[j].freq<min->freq) min=&arr[j];
		}
		if(arr[i+1].freq!=-1){
			temp=arr[i];
			arr[i]=*min;
			*min=temp;
		}
	}
}

void shift_right(hoffman *arr, int index){
	int i=index;
	hoffman temp1, temp2;

	temp1=arr[i];
	for(i=index; arr[i+1].freq!=-1; i++){
		temp2=arr[i+1];
		arr[i+1]=temp1;
		temp1=temp2;
	}
	temp2=arr[i+1];
	arr[i+1]=temp1;
	arr[i+2]=temp2;
}

hoffman *read_file(char *file){
	int stop_v=0, counter=0;
	char temp='\0';
	hoffman mid_node;
	hoffman* c_info=(hoffman*)malloc(sizeof(hoffman));
	FILE *fp;

	c_info->freq=-1;
	fp=fopen(file, "r");

	for(int i=0; !feof(fp); i++){
		fscanf(fp, "%c", &temp);
		if(feof(fp))
			stop_v=1;
		for(int j=0; c_info[j].freq!=-1 && stop_v!=1; j++){
			if(c_info[j].c==temp){
				c_info[j].freq+=1;
				stop_v=1;
			}
		}
		if(stop_v!=1){
			c_info=(hoffman*)realloc(c_info, (counter+2)*sizeof(hoffman));
			c_info[counter+1].freq=-1;
			c_info[counter+1].c='\0';
			c_info[counter].freq=1;
			c_info[counter].c=temp;
			c_info[counter].right=NULL;
			c_info[counter].left=NULL;
			counter++;
		}
		stop_v=0;
	}
	stop_v=0;
	sort_hoffarr(c_info);
	for(int i=0, j=counter; c_info[i].freq!=-1 && c_info[i+1].freq!=-1; i+=2){
		stop_v=0;
		c_info=(hoffman*)realloc(c_info, (j+2)*sizeof(hoffman));
		mid_node.c='<';
		mid_node.freq=c_info[i].freq+c_info[i+1].freq;
		mid_node.right=&c_info[i+1];
		mid_node.left=&c_info[i];
		
		for(int k=0; c_info[k].freq!=-1 && stop_v!=1; k++){
			if((c_info[k+1].freq==mid_node.freq && c_info[k].freq==mid_node.freq) && c_info[k+1].right==NULL){
				shift_right(c_info, k+1);
				c_info[k+1]=mid_node;
				stop_v=1;
			}
			else if(c_info[k+1].freq>mid_node.freq || c_info[k+1].freq==-1){
				if(c_info[k+1].freq!=-1) shift_right(c_info, k+1);
				else c_info[k+2]=c_info[k+1];
				c_info[k+1]=mid_node;
				stop_v=1;
			}
		}
		j++;
	}
	counter=0;
	while(c_info[counter+1].freq!=-1){
		counter++;
	}

	fclose(fp);
	return &c_info[counter];
}

void bit_codes(hoffman *head, int index, int *char_ind, char encrypts[300][20]){
	if(head->right==NULL && head->left==NULL){
		encrypts[*char_ind][index]='!';
		encrypts[*char_ind][index+1]=head->c;
		encrypts[*char_ind][index+2]='\0';
		(*char_ind)+=1;
	}

	else{
		encrypts[*char_ind][index]='0';
		bit_codes(head->left, index+1, char_ind, encrypts);
		for(int i=0; i<index; i++){
			encrypts[*char_ind][i]=encrypts[*char_ind-1][i];
		}
		encrypts[*char_ind][index]='1';
		bit_codes(head->right, index+1, char_ind, encrypts);	
	}
}

void shift_right_char(char *arr){
	int i=0;
	char temp1, temp2;
	temp1=arr[0];
	for(i=0; arr[i]!='\0'; i++){
		temp2=arr[i+1];
		arr[i+1]=temp1;
		temp1=temp2;
	}
	arr[i+1]=temp1;
}

int byte_to_int(char *byte){
	int ans=0;

	for(int i=7, k=0; i>=0; i--){
		if(byte[i]=='1'){
			ans+=pow(2, k);
		}
		k++;
	}
	return ans;
}

void int_to_byte(int data, char ans[9]){
	ans[8]='\0';
	for(int i=7; i>=0; i--){
		if(data%2==1)
			ans[i]='1';
		else if(data%2==0)
			ans[i]='0';
		data/=2;
	}
}

void write_encrfile(char encrypts[300][20], char *file, char* encr_file){
	char file_c;
	char byte_temp[8];
	unsigned char byte;
	int char_pointer=0, stop=0,c=0;
	FILE *fp;
	FILE *encr_fp;

	fp=fopen(file, "r");
	encr_fp=fopen(encr_file, "wb");

	for(int i=0; !feof(fp); i++){
		stop=0;
		fscanf(fp, "%c", &file_c);
		for(int j=0; encrypts[j][0]!='\0' && stop!=1; j++){
			if(encrypts[j][0]==file_c){
				for(int p=2; encrypts[j][p]!='\0'; p++){
					byte_temp[char_pointer]=encrypts[j][p];
					char_pointer+=1;
					if(char_pointer>=8){
						byte=byte_to_int(byte_temp);
						if(c==0){
							c++;
						}
						fwrite(&byte, 1, 1, encr_fp);
						char_pointer=0;
						stop=1;
					}
				}
			}
		}
	}
	fclose(fp);
	fclose(encr_fp);
}

void shift_left(char *arr, int turn){
	char temp;
	for(int i=0; i<turn; i++){
		for(int j=0; arr[j]!='\0'; j++){
			arr[j]=arr[j+1];
		}
	}
}

void read_encrfile(char encrypts[300][20], char* encr_file, char* decr_file){
	char bit_pattern[33];
	int longest, stop_v=0;
	unsigned char byte;
	FILE *encr_fp;
	FILE *decr_fp;

	longest=strlen(&encrypts[0][2]);
	for(int i=1; encrypts[i][0]!='\0'; i++){
		if(strlen(&encrypts[i][2])>longest){
			longest=strlen(&encrypts[i][2]);
		}
	}

	encr_fp=fopen(encr_file, "rb");
	decr_fp=fopen(decr_file, "w");
	fread(&byte, 1, 1, encr_fp);
	int_to_byte((int)byte, bit_pattern);
	fread(&byte, 1, 1, encr_fp);
	int_to_byte((int)byte, &bit_pattern[8]);
	fread(&byte, 1, 1, encr_fp);
	int_to_byte((int)byte, &bit_pattern[16]);
	fread(&byte, 1, 1, encr_fp);
	int_to_byte((int)byte, &bit_pattern[24]);


	for(int i=0; !feof(encr_fp); i++){
		stop_v=0;
		if(strlen(bit_pattern)<longest){
			fread(&byte, 1, 1, encr_fp);
			int_to_byte(byte, &bit_pattern[strlen(bit_pattern)]);
		}

		for(int j=0; encrypts[j][0]!='\0' && stop_v!=1; j++){
			if(strncmp(&encrypts[j][2], bit_pattern, strlen(&encrypts[j][2]))==0){
				fprintf(decr_fp, "%c", encrypts[j][0]);
				shift_left(bit_pattern, strlen(&encrypts[j][2]));
				stop_v=1;
			}
		}
	}
	fclose(encr_fp);
	fclose(decr_fp);
}

void calculate_size_diff(char *file, char *encr_file){
	int i, file_s, encrf_s;
	unsigned char temp;
	FILE *fp;
	FILE *encr_fp;

	fp=fopen(file, "r");
	encr_fp=fopen(encr_file, "r");

	for(i=-1; !feof(fp); i++){
		fread(&temp, 1, 1, fp);
	}
	file_s=i;
	for(i=-1; !feof(encr_fp); i++){
		fread(&temp, 1, 1, encr_fp);
	}
	encrf_s=i;

	printf("Normal size of file(%s): %d bytes\nCompressed size of file(%s): %d bytes\nTotal difference: %d bytes\n", file, file_s, encr_file, encrf_s, file_s-encrf_s);
	fclose(fp);
	fclose(encr_fp);
}