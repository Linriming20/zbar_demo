/* reference: zbar-0.10/examples/scan_image.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zbar.h"


// 编译时Makefile里控制
#ifdef ENABLE_DEBUG
	#define DEBUG(fmt, args...)     printf(fmt, ##args)
#else
	#define DEBUG(fmt, args...)
#endif


int getPgmPicGrayData(char *pgmFileame, int width, int height, char *dataBuf);
int getYuvPicGrayData(char *yuvFileame, int width, int height, char *dataBuf);


int main(int argc, char *argv[])
{
    int width = 0;
	int height = 0;
	char *buf = NULL;
	int count = 0;
	zbar_symbol_type_t type;
	zbar_image_scanner_t *scanner = NULL;
	zbar_image_t *image = NULL;
	const zbar_symbol_t *symbol = NULL;

	if (argc != 4)
	{
		printf("Usage: \n"
			   "   %s <pgm_file> <width> <height>\n"
			   "   %s <yuv_file> <width> <height>\n"
			   "   (\033[33mnote: only support [\"P5\" pgm] and [\"Planar/Semi-Planar\" YUV] in this demo.)\033[0m\n"
			   "examples: \n"
			   "   %s ./pics/barcode.yuv 1120 730\n"
			   "   %s ./pics/qrcode.pgm 250 250\n"
			   "   %s ./pics/multi.pgm 2792 1648\n"
			   "   %s ./pics/nothing.pgm 2792 1648\n",
			   argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
		return -1;
	}

	width = atoi(argv[2]);
	height = atoi(argv[3]);
	buf = (char *)malloc(width*height);

	/* get pic'gray data  */
	if(strstr(argv[1], "pgm"))
	{
		int ret = getPgmPicGrayData(argv[1], width, height, buf);
		if(ret)
		{
			printf("get pgm pic gray data failed!\n");
			return -1;
		}
	}
	else if(strstr(argv[1], "yuv"))
	{
		int ret = getYuvPicGrayData(argv[1], width, height, buf);
		if(ret)
		{
			printf("get pgm pic gray data failed!\n");
			return -1;
		}
	}
	else
	{
		printf("Don't support parse this format now!\n");
		return -1;
	}

    /* step 1/6: create a reader */
    scanner = zbar_image_scanner_create();

    /* step 2/6: configure the reader */
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

    /* step 3/6: wrap image data */
    image = zbar_image_create();
    zbar_image_set_format(image, *(int*)"Y800");
    zbar_image_set_size(image, width, height);
    zbar_image_set_data(image, buf, width * height, zbar_image_free_data);

    /* step 4/6: scan the image for barcodes (optional) */
    count = zbar_scan_image(scanner, image);
	printf("[scan result] count:\033[32m %d \033[0m\n", count);

    /* step 5/6: extract results */
    symbol = zbar_image_first_symbol(image);
    for(; symbol; symbol = zbar_symbol_next(symbol))
	{
        /* do something useful with results */
        type = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
        printf("\033[32m[type: %s]\t [data: %s] \033[0m\n", zbar_get_symbol_name(type), data);
    }

    /* step 6/6: clean up */
    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

	// don't need to free, because it had free by `zbar_image_free_data` in `zbar_image_set_data` callback
	//free(buf);

	return 0;
}


int getPgmPicGrayData(char *pgmFilename, int width, int height, char *dataBuf)
{
	FILE *fp = NULL;
	char pgmFormat[2] = {0}; // "P2" or "P5"
	int pgmWidth = 0;
	int pgmHeight = 0;
	int pgmMaxGrayVal = 0;
	int readBytes = 0;

	if(!pgmFilename || !width || !height || !dataBuf)
	{
		printf("[%s:%d] Parameter is invalid!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	fp = fopen(pgmFilename, "rb");
	if(!fp)
	{
		perror("open file error!");
		return -1;
	}

	fscanf(fp, "%s\n"
			   "%d %d\n"
			   "%d\n",
			   pgmFormat, &pgmWidth, &pgmHeight, &pgmMaxGrayVal);
	DEBUG("\n%s | format: %s   width: %d   height: %d   max gray value: %d\n\n",
		  pgmFilename, pgmFormat, pgmWidth, pgmHeight, pgmMaxGrayVal);

	/* Only spuuort "P5" format and one byte gray value now */
	if (strcmp(pgmFormat, "P5") || \
	    (pgmWidth != width)     || \
	    (pgmHeight != height)   || \
		(pgmMaxGrayVal != 255))
	{
		printf("Format not supported or image size mismatch!\n");
		return -1;
	}

	readBytes = fread(dataBuf, 1, width*height, fp);
	if(readBytes != width*height)
	{
		printf("Read %s error: data corruption!\n", pgmFilename);
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}

int getYuvPicGrayData(char *yuvFilename, int width, int height, char *dataBuf)
{
	FILE *fp = NULL;
	int readBytes = 0;

	if(!yuvFilename || !width || !height || !dataBuf)
	{
		printf("[%s:%d] Parameter is invalid!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	fp = fopen(yuvFilename, "rb");
	if(!fp)
	{
		perror("open file error!");
		return -1;
	}

	readBytes = fread(dataBuf, 1, width*height, fp);
	if(readBytes != width*height)
	{
		printf("Read %s error: data corruption!\n", yuvFilename);
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}



