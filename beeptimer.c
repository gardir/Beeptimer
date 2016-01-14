#include "beeptimer.h"

int argument_parser(int argc, char *argv[], char *options)
{
	int i;
	int seconds = 0;
	for (i=1; i<argc; i++) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 0) {
				return -1; // illegal option
			} else if (argv[i][1] == '-') {
				if (strncmp((&argv[i][2]), "help", 5) == 0) {
					usage();
					return 0; // legal return
				}
				continue; // skip the options below, as double '--' only goes to help for now
			}
			char option = argv[i++][1];
			if (i >= argc) {
				printf("Provide option after '%s'\n", argv[i-1]);
				return -1;
			}
			int value = atoi(argv[i]);
			if (option == 's') { // manual seconds
				seconds += value;
			}
			else if (option == 'm') { // manual minutes
				seconds += value * 60;
			}
			else if (option == 'h') { // manual hours
				seconds += value * 3600;
			}
			else if (option == 'r') { // enables restart
				(*options) |= 1; // restart bit 1
			}
		}
		else { // the semi-optional seconds argument
			return (seconds += atoi(argv[i]));
		}
	}
	return seconds;
}

void display_timer(int seconds)
{
	fprintf(stderr, "\r");
	if (seconds > 3600) {
		fprintf(stderr, "%dhr, ", seconds / 3600);
		seconds %= 3600;
	}
	if (seconds > 60) {
		fprintf(stderr, "%dmin, ", seconds / 60);
		seconds %= 60;
	}
	fprintf(stderr, "%ds", seconds);
}

int init_device()
{
	// Play sound
	SDL_AudioSpec want;
	SDL_memset(&want, 0, sizeof(want));
	want.freq = 48000;
	want.format = AUDIO_S16;
	want.channels = 2;
	want.samples = 4096;
	want.callback = play_beep;
	want.userdata = NULL;
	if (SDL_OpenAudio(&want, NULL) < 0) {
		fprintf(stderr, "Couldn't open audio device: %s\n", SDL_GetError());
		return (-1);
	}
	return 0;
}

void play_beep(void *udata, Uint8 *stream, int len)
{
	/* Only play if we have data left */
	if (wav_len == 0) {
		return;
	}

	/* Mix as much data as possible */
	len = (len > wav_len ? wav_len : len);
	SDL_MixAudio(stream, wav_pos, len, SDL_MIX_MAXVOLUME);
	wav_pos += len;
	wav_len -= len;
}

int prompt_continue()
{
	char again[2];
	printf("Again? (y/n) ");
	fgets(again, 2, stdin);
	while (again[0] != 'y' && again[0] != 'n') {
		printf("Please answers y/n: ");
		fgets(again, 2, stdin);
	}
	return again[0] - 'n';
}

int start_audio()
{
	SDL_RWops *wav_data;
	wav_data = SDL_RWFromMem(&din_ding_wav, din_ding_wav_len);
	
	if (SDL_LoadWAV_RW(wav_data, 1, &wav_spec, &wav_pos, &wav_len) == NULL) {
		fprintf(stderr, "Could not load sound: %s\n", SDL_GetError());
		return 1;
	}
	return 0;
}

void timer(int seconds)
{
	// Setup
	int interval = 0;
	time_t *tStart = calloc(1, sizeof(time_t));
	time_t *tEnd = calloc(1, sizeof(time_t));

	// Timer
	time(tStart);
	do {
		time(tEnd);
		interval = (int)(*tEnd)-(int)(*tStart);
		display_timer(interval);
		sleep(1);
	} while (interval < seconds);
	printf("\n");
	// Clean
	free(tStart);
	free(tEnd);
}

void usage()
{
	int shortLen = 6;
	char *usageString;
	printf("Usage: $ ./Beeptimer [options] (seconds)\n");
	printf("The seconds will turn into seconds if provided, but is not in itself necessary as long as any option provided has included numeric.\n");
	asprintf(&usageString, "%%-%ds - %%s\n", shortLen);
	printf(usageString, "--help", "Shows this information.");
	printf(usageString, "-h H", "stands for [h]ours and numeric (H) after is required.");
	printf(usageString, " ", "Will turn H into H*3600 seconds added before beep.");
	printf(usageString, "-m M", "stands for [m]inutes and numeric (M) after is required.");
	printf(usageString, " ", "Will turn M into M*60 seconds added before beep.");
	printf(usageString, "-s S", "stands for [s]econds and numeric (S) after is required.");
	printf(usageString, " ", "Will turn S into S seconds added before beep.");
	printf("Any option above can be simultanously provided, and thus be added up.\n");
	printf("=== EXAMPLEs ===\n");
	printf("'./Beeptimer -m 1 2' is equivalent to\n");
	printf("'./Beeptimer -s 2 -m 1' is equivalent to\n");
	printf("'./Beeptimer 62'\n");
	free(usageString);
}

int main(int argc, char *argv[])
{
	
    // Argument check
	if (argc < 2) {
		printf("Argument missing, see --help for usage");
		return 1; // BeeptimerArgumentError
	}

	char options = 0; // option byte
	/*
	  1  - RESTART
	  2  - UNUSED
	  4  - UNUSED
	  8  - UNUSED
	  16 - UNUSED
	  32 - UNUSED
	  64 - UNUSED
	  128- UNUSED
	 */
	
	// Seconds setup
	int seconds = argument_parser(argc, argv, &options);

	if (seconds < 1) { // quit here, no seconds to measure against.
		if (seconds) {
			return seconds;
		}
		return 0; // help was called, or (illegal) 0 seconds was given
	}
	
	// Init sound
	if (SDL_Init(SDL_INIT_AUDIO)) {
		fprintf(stderr, "\nUnable to initialize SDL: %s\n", SDL_GetError());
		return 2; // BeeptimerInitError
	}

	if (start_audio()) {
		return 3; // BeeptimerAudioError
	}

	// Loop for easier "restart" of timer, if enabled
	do {
		// Timer-wait
		timer(seconds);	

		// init/reset device
		if (init_device()) {
			return 4; // BeeptimerInitDeviceError
		}
		SDL_PauseAudio(0);
		while (wav_len > 0) {
			SDL_Delay(100);
		}

	} while (restart && prompt_continue());
		
	SDL_CloseAudio();
	// Clean
	atexit(SDL_Quit);
	return 0;
}
