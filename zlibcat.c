#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int CatDeflate(int readfd, int writefd);

int main(int argc, char **argv) {
	CatDeflate(0,1);
}


int CatDeflate(int readfd, int writefd) {
	int return_value = -1;
	
	// Initialise zlib
	z_streamp deflate_stream;
	int deflate_status;
	deflate_stream = calloc(sizeof(z_stream), 1);
	deflate_status = deflateInit(deflate_stream, 9);
	if (Z_OK != deflate_status) goto error_1;
	
	// Allocate read / write buffers
	size_t buflen = 256 * 1024;
	Bytef *readbuf = calloc(2, buflen);
	Bytef *outbuf = readbuf + buflen;
	size_t readlen;

	while (1) {
		readlen = read(0, readbuf, buflen);
		if (readlen==-1)  goto error_3;
		deflate_stream->next_in = readbuf;
		deflate_stream->avail_in = readlen;
		
		// deflate until the read buffer is empty
		while (1) {
			deflate_stream->next_out = outbuf;
			deflate_stream->avail_out = buflen;
			deflate_status = deflate(deflate_stream, readlen == 0 ? Z_FINISH : Z_NO_FLUSH);
			
			// write everything we have
			Bytef *next_write = outbuf;
			while (next_write < deflate_stream->next_out) {
				size_t writelen = write(1, next_write, deflate_stream->next_out-next_write);
				if (writelen == -1) goto error_3;
				next_write += writelen;
			}
			
			if (readlen && deflate_stream->avail_in == 0) break;
			if (!readlen && deflate_status==Z_STREAM_END) break;
			if (deflate_status != Z_OK)  goto error_3;
		}
		if (!readlen) break;
	}
		
	// No errors encountered!
	return_value = 0;
	
error_3:
	free(readbuf);

error_2:	
	deflate_status = deflateEnd(deflate_stream);
	if (deflate_status!=Z_OK) {
		return_value = -2;
	}
	
error_1:
	free(deflate_stream);
	
	return return_value;
}
