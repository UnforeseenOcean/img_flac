#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>

#define DR_WAV_IMPLEMENTATION
    #include "dr_wav.h"
 #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#ifdef WIN32
    #include <direct.h>
    #include <malloc.h>
#else
    #include <unistd.h>
    #include <sys/stat.h>
#endif

void std_err(void);
void io_err(void);

#define width_ 512
#define height_ 512

void save_file(char* image, char* comp_file)
{
	int width,height,comp;
	unsigned char* image_data =  stbi_load(image, &width, &height, &comp, 4);
	drwav wav;
	drwav_data_format format;
    format.container = drwav_container_riff;     // <-- drwav_container_riff = normal WAV files, drwav_container_w64 = Sony Wave64.
    format.format = DR_WAVE_FORMAT_PCM;          // <-- Any of the DR_WAVE_FORMAT_* codes.
    format.channels = 1;
    format.sampleRate = 44100;
    format.bitsPerSample = 8;
    drwav_init_file_write(&wav, comp_file, &format, NULL);
	drwav_write_raw(&wav, width*height*4, image_data);
	drwav_uninit(&wav);
	free(image_data);
	printf("File made successfully.\n");
}

void load_file(char* file)
{
	drwav wav;
	if (!drwav_init_file(&wav, file, NULL)) {
        // Error opening WAV file.
    }
    drwav_uint8* pDecodedInterleavedPCMFrames = (drwav_uint8*)malloc(wav.totalPCMFrameCount * wav.channels * sizeof(drwav_uint8));
	drwav_read_raw(&wav, width_*height_*4, pDecodedInterleavedPCMFrames);
	stbi_write_png("out.png", width_, height_, 4, pDecodedInterleavedPCMFrames, width_*4);
	drwav_uninit(&wav);
	free(pDecodedInterleavedPCMFrames);

}


void std_err(void) {
    perror("\nError");
    exit(1);
}

void io_err(void) {
    fputs("\nError: I/O error, the file is incomplete\n", stdout);
    exit(1);
}

#define VER         "0.2"

int main(int argc, char* argv[]) {
    setbuf(stdout, NULL);
    fputs("\n"
    "img_flac "VER":\n"
	"Convert images to audio\n"
    "by mudlord\n"
    "web:    mudlord.github.io\n"
    "\n", stdout);

	save_file("original.png", "out.wav");
	load_file("out.wav");

	printf("\n");
	system("pause");
	return 0;
}
