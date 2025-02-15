#include <3ds.h>
#include <stdio.h>
#include <string.h>

#define SAMPLE_RATE 32728  // Default 3DS mic sample rate
#define BUFFER_SIZE 1024   // Small buffer for low latency

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    micInit();
    csndInit();

    printf("3DS Mic Passthrough\nPress START to exit.\n");
    
    s16 buffer[BUFFER_SIZE];  // Buffer for mic samples
    u32 sampleCount;
    
    while (aptMainLoop()) {
        hidScanInput();
        if (hidKeysDown() & KEY_START) break;
        
        // Capture audio from the microphone
        micReadData(buffer, BUFFER_SIZE * sizeof(s16), &sampleCount);
        
        if (sampleCount > 0) {
            // Play the captured audio through speakers (channel 0, left/right stereo)
            CSND_playsound(0x8, SOUND_ONE_SHOT | SOUND_FORMAT_16BIT, SAMPLE_RATE, 1.0f, 1.0f, buffer, buffer, sampleCount * sizeof(s16));
        }
        
        gfxFlushBuffers();
        gfxSwapBuffers();
    }
    
    micExit();
    csndExit();
    gfxExit();
    return 0;
}
