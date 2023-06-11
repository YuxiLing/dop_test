
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct wtap_nstime {
	time_t	secs;
	int	nsecs;
};

typedef struct {
    struct wtap_nstime now;
	time_t t0;
} mpeg_t;

struct _mpeg_magic {
	size_t len;
	const char* match;
} magic[] = {
	{ 3, "TAG" }, /* ID3v1 */
	{ 3, "ID3" }, /* ID3v2 */
	{ 3, "\0\0\1" }, /* MPEG PES */
	{ 2, "\xff\xfb" }, /* MP3, taken from http://en.wikipedia.org/wiki/MP3#File_structure */
	{ 0, NULL }
};

struct wtap{
    char* fh;
    void* priv;
};

int 
mpeg_open(struct wtap *wth, int *err)
{
	int bytes_read;
	char magic_buf[16];
	struct _mpeg_magic* m;
	mpeg_t *mpeg;
	
	bytes_read = file_read(magic_buf, sizeof magic_buf, wth->fh);
	if (bytes_read != (int) sizeof magic_buf) {
		*err = wth->fh;
		if (*err != 0)
			return -1;
		return 0;
	}

	for (m=magic; m->match; m++) {
		if (memcmp(magic_buf, m->match, m->len) == 0)
			goto good_magic;
	}
	
	return 0;

good_magic:

	mpeg = (mpeg_t *)g_malloc(sizeof(mpeg_t));
	wth->priv = (void *)mpeg;
	mpeg->now.secs = 0;
	mpeg->now.nsecs = 0;
	mpeg->t0 = mpeg->now.secs;

	return 1;
}

int main(){
    struct wtap *wth;
    int *err=0;
    mpeg_open(wth,err);
}