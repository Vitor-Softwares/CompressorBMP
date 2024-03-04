#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

BFHeader bf;
int main(int argc, char* argv[]) {

	FILE* fp;
	char arg[] = "";
	int larguratotal = 0;
	int bytesPerRow;
	int y, x;
	int byteAtual = 0;
	int cont = 0;
	int quantBits = 0;
	int quantBitsSeguidos = 0;
	unsigned char temp = 0;
	unsigned char bitAtual = 1;

	if (argc < 2) {
		fp = fopen(arg, "rb");
	}
	else {
		fp = fopen(argv[1], "rb");
	}

	if (!fp) {
		printf("Erro ao abrir a imagem!\n");
		return(0);
	}

	fillHeader(&bf, fp);

	if (bf.BIHeader.bitsPerPixel != 1) {
		printf("Essa imagem não é monocromatica!\n");
		fclose(fp);
		return(0);
	}

	larguratotal = bf.BIHeader.bitsPerPixel * bf.BIHeader.imageWidth;
	bytesPerRow = ceiling(larguratotal, 32) * 4;
	fseek(fp, bf.pixelDataOffset, SEEK_SET);

	for (y = 0; y < bf.BIHeader.imageHeigth; y++) {
		quantBits = quantBitsSeguidos = 0;
		for (x = 0; x < bytesPerRow; x++) {
			fread(&byteAtual, 1, 1, fp);
			for (cont = 7; cont >= 0; cont--) {
				if (quantBits < larguratotal) {
					temp = checkBit(&byteAtual, cont);
					if (temp == bitAtual) quantBitsSeguidos++;
					else {
						printf("%d,", quantBitsSeguidos);
						quantBitsSeguidos = 1;
						bitAtual = temp;
					}
					if (quantBitsSeguidos >= 0xff) {
						return(0);
					}
				}
				quantBits++;
			}
		}
		printf("%d,", quantBitsSeguidos);
	}
	return(0);
}