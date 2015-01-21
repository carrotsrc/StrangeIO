#include "system/units/RuFlacLoad.h"
#define FRPP 2048
#define PCM_TYPE short
#define PCM_BUF_TYPE short*

static void *audioOut(void*);
static void *combo(short*, int);
static void read_file (const char * fname, TGeneralBuffer<PCM_BUF_TYPE> *tbuf)
{
	PCM_TYPE *buffer, *position, *period;
	signed int i = 0, dir = 1, err, psize = 0;
	sf_count_t bsize = 0, count;
	pthread_t thrAudio;

	err = pthread_create(&thrAudio, NULL, audioOut, (void*)tbuf);

	SndfileHandle file(fname) ;
	if(file.error()) {
		printf("Error loading file %d... Exit\n", file.error());
		return;
	}
	printf ("Opened file '%s'\n", fname) ;

	bsize = file.frames()<<1;
	buffer = (short*)calloc(bsize, sizeof(PCM_TYPE));
	position = buffer;

	while(file.read (position, 4096) == 4096)
		position += 4096;


	position = buffer;
	count = bsize;
	psize = FRPP;
	do {
		if(!tbuf->isFull() && tbuf->tryLock() == 0) {
			period = (PCM_TYPE*)calloc(psize, sizeof(PCM_TYPE));
			if(count < psize) psize = count;
			memcpy(period, position, psize<<1);
			tbuf->add(period);
			tbuf->unlock();

			count -= psize;
			position += psize;
		}
	} while(count > 0);
	free(buffer);
}

static void *audioOut(void *data) {
/*	TGeneralBuffer<PCM_BUF_TYPE> *buffer;
	snd_pcm_t *playback_handle;

	unsigned int psize = FRPP;
	unsigned int plen = psize>>1;
	short *period = NULL;
	do {
		if(buffer->isFull() && buffer->tryLock() == 0) {
			period = buffer->read();
			buffer->unlock();
			if ((err = snd_pcm_writei(playback_handle, period, plen)) != plen) {
				fprintf (stderr, "write to audio interface failed (%s)\n",
					 snd_strerror (err));
				snd_pcm_recover(playback_handle, err, 1);
			}
			free(period);
			period = NULL;
		}
	} while(1);
	snd_pcm_close (playback_handle);
*/
}


int main(void)
{
	const char * fname = "/home/charlie/Brain in a Fish Tank.flac";
	RuFlacLoad f;
	f.printJacks();

	return 0 ;
} /* main */




