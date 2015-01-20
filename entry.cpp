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
	TGeneralBuffer<PCM_BUF_TYPE> *buffer;
	snd_pcm_t *playback_handle;
	snd_pcm_hw_params_t *hw_params;
	int err, dir;
	unsigned int srate = 44100;
	pthread_mutex_t *mutex;

	buffer = (TGeneralBuffer<PCM_BUF_TYPE>*)data;
	mutex = buffer->getMutex();

	if ((err = snd_pcm_open (&playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		fprintf (stderr, "cannot open audio device %s (%s)\n", 
			 "default",
			 snd_strerror (err));
		exit (1);
	}
  
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &srate, &dir)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_channels (playback_handle, hw_params, 2)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (playback_handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

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
}


int main(void)
{
	const char * fname = "/home/charlie/Brain in a Fish Tank.flac";
	//TGeneralBuffer<PCM_BUF_TYPE> buffer;
	//read_file (fname, &buffer);
	RuFlacLoad f;
	f.printJacks();

	return 0 ;
} /* main */




