#ifndef HEADER_FILE
#define HEADER_FILE

typedef struct hoffman{
	int freq;
	char c;
	struct hoffman *right;
	struct hoffman *left;
}hoffman;


void shift_right_char(char *arr);	/*shifting the char array right*/
void shift_right(hoffman *arr, int index);	/*shifting the hoffman array to the right by index times*/
void shift_left(char *arr, int turn);	/*shifting the char array left by turn times*/
void sort_hoffarr(hoffman *arr);	/*sorting the hoffman array by it's frequencies as increasing order*/
hoffman *read_file(char *file);	
void bit_codes(hoffman *head, int index, int *char_ind, char encrypts[300][20]); /*creating huffman tree recursively*/
int byte_to_int(char *byte);	/*turning a byte to integer*/
void int_to_byte(int data, char ans[9]);	/*turning integer to a byte*/
void write_encrfile(char encrypts[50][20], char *file, char* encr_file);
void read_encrfile(char encrypts[50][20], char* encr_file, char* decr_file);
void calculate_size_diff(char *file, char *encr_file);	/*calculating the size difference of opened and compressed file*/

#endif