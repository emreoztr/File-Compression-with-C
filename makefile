HUFFMAN_TREE:
	gcc -o huffman_compress_test huffman_compress_test.c HuffmanCompress.c HuffmanCompress.h -lm
	./huffman_compress_test
