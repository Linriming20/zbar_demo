#include <stdio.h>
#include <stdlib.h>
#include <zbar.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zbar.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

zbar_image_scanner_t *scanner = NULL;

int user_fillBlob_func(unsigned char *blob, size_t bloblen, unsigned char *pu8Path)
{
	size_t ret;
	int fd = open(pu8Path, O_RDONLY);
	if(fd < 0)
	{
		printf("open failed!\n");
		return -1;
	}
	//lseek(fd, 17, SEEK_CUR);
	lseek(fd, 15, SEEK_CUR);
	
	ret = read(fd, blob, bloblen);
	if(ret != bloblen)
	{
		printf("read %d byte!\n", (int)ret);
		//return -1;
	}
	
	close(fd);
	
	return 0;
}

int main (int argc, char **argv)
{
    //if(argc < 2) return(1);

    /* create a reader */
    scanner = zbar_image_scanner_create();

    /* configure the reader */
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);

    /* obtain image data */
    int width = 1920, height = 1080;
	size_t bloblen = width * height;
	unsigned char *blob = malloc(bloblen);
    user_fillBlob_func(blob, bloblen, (unsigned char *)"./backup.pgm");

    /* wrap image data */
    zbar_image_t *image = zbar_image_create();
    zbar_image_set_format(image, *(int*)"Y800");
    zbar_image_set_size(image, width, height);
    zbar_image_set_data(image, blob, width * height, zbar_image_free_data);

    /* scan the image for barcodes */
    int n = zbar_scan_image(scanner, image);
	printf("n = %d\n", n);
    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    for(; symbol; symbol = zbar_symbol_next(symbol)) {
        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
        printf("decoded %s symbol \"""\x1b[34m""%s\"""\n""\x1b[0m",
               zbar_get_symbol_name(typ), data);
    }

    /* clean up */
    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

    return(0);
}
