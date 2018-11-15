#include <iostream>

#define HEIGHT 320
#define WHDTH 240
#define MAXID 3

using namespace std;

typedef struct {
    unsigned black;
    unsigned white;
} COLOR;

unsigned med = 0x3FFFFF; /* 00000000 11000000 11000000 11000000 */
unsigned q1 = 20000;
unsigned q2 = 40000;

FILE* readNextBMP(unsigned id) {
    FILE* fp;
    char path[20];
    sprintf(path,"c:\\out\\%u", id);
    strcat(path, ".bmp");
    fp=fopen(path, "rb");
    return fp;
}

FILE* waitNextBMP(FILE* fp, unsigned id) {
    printf("start find next id %d\n", id);
    do {
        printf("cur id : %d, wait 1s for id %d\n", id, id+1);
        Sleep(500);
        fp = readNextBMP(id);

    }
    while (fp == NULL);
    return fp;
}

COLOR countBlackWhite(FILE *fp, unsigned id) {
    COLOR color = {0, 0};
    unsigned rgb_int;
    unsigned char rgb_char[3];

    fseek(fp, 374*sizeof(unsigned char), SEEK_SET);

    for (unsigned h=0; h<HEIGHT; ++h) {
        for (unsigned w=0; w<WHDTH; ++w) {
            fread(rgb_char, 3*sizeof(unsigned char), 1, fp);
            rgb_int = 0;
            memcpy(&rgb_int, rgb_char,3);
            if (rgb_int < med) {
                ++color.black;
            } else {
                ++color.white;
            }
        }
    }
    cout << "PC2 : num:" << id << ", black:" << color.black << ", white:" << color.white << endl;
    return color;
}

unsigned separation(unsigned black) {
    if (black < q1) {
        cout << "PC2 : small" << endl;
        return 1;
    }
    if (black < q2) {
        cout << "PC2 : medium" << endl;
        return 2;
    }

    cout << "PC2 : large" << endl;
    return 3;
}
