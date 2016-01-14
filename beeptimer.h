#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>

// Constants
static const int usage_len = 120;

// Embedded
extern unsigned char din_ding_wav;
extern unsigned int din_ding_wav_len;

// Pointers
static Uint8 *wav_file;
static int wav_file_len;
static SDL_AudioSpec wav_spec;
static Uint32 wav_len;
static Uint8 *wav_pos;

// Functions
int argument_parser(int argc, char *argv[]);
void display_timer(int seconds);
int init_device();
void play_beep(void *udata, Uint8 *stream, int len);
int prompt_continue();
int start_audio();
void timer(int seconds);
void usage();
int main(int argc, char *argv[]);
