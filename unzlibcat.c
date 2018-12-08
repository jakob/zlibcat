#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int CatInflate(int readfd, int writefd);

int main(int argc, char **argv) {
	CatInflate(0,1);
}


int CatInflate(int readfd, int writefd) {
	int return_value = -1;
	
	// Initialise zlib
	z_streamp inflate_stream;
	int inflate_status;
	inflate_stream = calloc(sizeof(z_stream), 1);
	inflate_status = inflateInit(inflate_stream);
	if (Z_OK != inflate_status) goto error_1;
	
	// Allocate read / write buffers
	size_t buflen = 256 * 1024;
	Bytef *readbuf = calloc(2, buflen);
	Bytef *outbuf = readbuf + buflen;
	size_t readlen;

	while (1) {
		readlen = read(0, readbuf, buflen);
		if (readlen==-1)  goto error_3;
		inflate_stream->next_in = readbuf;
		inflate_stream->avail_in = readlen;
		
		// inflate until the read buffer is empty
		while (1) {
			inflate_stream->next_out = outbuf;
			inflate_stream->avail_out = buflen;
			inflate_status = inflate(inflate_stream, readlen == 0 ? Z_FINISH : Z_NO_FLUSH);
			
			// write everything we have
			Bytef *next_write = outbuf;
			while (next_write < inflate_stream->next_out) {
				size_t writelen = write(1, next_write, inflate_stream->next_out-next_write);
				if (writelen == -1) goto error_3;
				next_write += writelen;
			}
			
			if (readlen && inflate_stream->avail_in == 0) break;
			if (inflate_status==Z_STREAM_END) break;
			if (inflate_status != Z_OK)  goto error_3;
		}
		if (!readlen) break;
	}
		
	// No errors encountered!
	return_value = 0;
	
error_3:
	free(readbuf);

error_2:	
	inflate_status = inflateEnd(inflate_stream);
	if (inflate_status!=Z_OK) {
		return_value = -2;
	}
	
error_1:
	free(inflate_stream);
	
	return return_value;
}
