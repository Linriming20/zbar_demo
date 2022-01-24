#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zbar.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
		printf("read %d byte!\n", ret);
		//return -1;
	}
	
	close(fd);
	
	return 0;
}

int main(int argc, const char *argv[])
{
	unsigned int seq;
	int found = 0;
	int ret;
	static zbar_processor_t *processor = NULL;
	static int display = 0;
	static int width;
	static int height;
	static int xmllvl = 0;
	char *xmlbuf = NULL;
	unsigned xmlbuflen = 0;
	static int num_images = 0, num_symbols = 0;
	
	if(argc < 4)
	{
		printf("failed! ./xxx width height file\n");
		return -1;
	}
	
	width = atoi(argv[1]);
	height = atoi(argv[2]);

	processor = zbar_processor_create(0);
	
	assert(processor);
	if(zbar_processor_init(processor, NULL, display)) {
		zbar_processor_error_spew(processor, 0);
		return(1);
	}
	
	zbar_image_t *zimage = zbar_image_create();
	zbar_image_set_format(zimage, *(unsigned long*)"Y800");
	zbar_image_set_size(zimage, width, height);
	size_t bloblen = width * height;
	unsigned char *blob = malloc(bloblen);
	
	user_fillBlob_func(blob, bloblen, (unsigned char *)argv[3]);
	
	zbar_image_set_data(zimage, blob, bloblen, zbar_image_free_data);
	zbar_process_image(processor, zimage);
	// output result data
	const zbar_symbol_t *sym = zbar_image_first_symbol(zimage);
	for(; sym; sym = zbar_symbol_next(sym)) {
		zbar_symbol_type_t typ = zbar_symbol_get_type(sym);
		if(typ == ZBAR_PARTIAL)
			continue;
		else if(!xmllvl)
			printf("aaaaaaaaaaaa[%s][%s]:[%s]\n",
				   zbar_get_symbol_name(typ),
				   zbar_get_addon_name(typ),
				   zbar_symbol_get_data(sym));
		else if(xmllvl < 0)
			printf("bbbbbbbbb%s\n", zbar_symbol_get_data(sym));
		else {
			if(xmllvl < 3) {
				xmllvl++;
				printf("ccccccccc<index num='%u'>\n", seq);
			}
			zbar_symbol_xml(sym, &xmlbuf, &xmlbuflen);
			printf("dddddddd%s\n", xmlbuf);
		}
		found++;
		num_symbols++;
	}
	if(xmllvl > 2) {
		xmllvl--;
		printf("eeeeeeeee</index>\n");
	}
	fflush(stdout);
	
	zbar_image_destroy(zimage);

	zbar_processor_destroy(processor);
}


