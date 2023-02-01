#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#define BLACK   0x00000000
#define BLUE    0x0000FF00
#define GREEN   0x00FF0000
#define RED     0xFF000000
#define WHITE   0xFFFFFF00

#define XLOWER -2.0
#define XUPPER  1.0
#define YLOWER -1.0
#define YUPPER  1.0

#define WIDTH  1200
#define HEIGHT  800

#define ITERATIONS 100

#define IN 1
#define OUT 0

#pragma pack(push, 1)
// struct that holds all of the meta-data for the .bmp file provided
// Total: 54 bytes
struct BMP_HEADER {
    uint16_t  type;               // Magic identifier: 0x4d42
    uint32_t  size;               // File size in bytes
    uint16_t  reserved1;          // Not used
    uint16_t  reserved2;          // Not used
    uint32_t  offset;             // Offset to image data in bytes from beginning of file (54 bytes)
    uint32_t  dib_header_size;    // DIB Header size in bytes (40 bytes)
    int32_t   width_px;           // Width of the image
    int32_t   height_px;          // Height of image
    uint16_t  num_planes;         // Number of color planes
    uint16_t  bits_per_pixel;     // Bits per pixel
    uint32_t  compression;        // Compression type
    uint32_t  image_size_bytes;   // Image size in bytes
    int32_t   x_resolution_ppm;   // Pixels per meter
    int32_t   y_resolution_ppm;   // Pixels per meter
    uint32_t  num_colors;         // Number of colors
    uint32_t  important_colors;   // Important colors
};
#pragma pack(pop)

struct BMP_HEADER init_BMP_HEAD(int xpixels, int ypixels) {
    struct BMP_HEADER head = {0};

    head.type = 0x4d42;
    head.size = 4 * xpixels * ypixels + 138;
    head.offset = 0x8a;
    head.dib_header_size = 124;
    head.width_px = xpixels;
    head.height_px = ypixels;
    head.num_planes = 1;
    head.bits_per_pixel = 32;
    head.image_size_bytes = 4 * xpixels * ypixels;
    return head;
}

// For debugging purposes
// Prints all of the meta-data associated with the provided BMP file
void print_head(struct BMP_HEADER image_head) {
    printf("Image: type: %X\n", image_head.type);
    printf("Image: size: %d\n", image_head.size);
    printf("Image: reserved1: %d\n", image_head.reserved1);
    printf("Image: reserved2: %d\n", image_head.reserved2);
    printf("Image: offset: %d\n", image_head.offset);
    printf("Image: offset: %x\n", image_head.offset);
    printf("Image: dib_header_size: %d\n", image_head.dib_header_size);
    printf("Image: width_px: %d\n", image_head.width_px);
    printf("Image: height_px: %d\n", image_head.height_px);
    printf("Image: num_planes: %d\n", image_head.num_planes);
    printf("Image: bits_per_pixel: %d\n", image_head.bits_per_pixel);
    printf("Image: compression: %d\n", image_head.compression);
    printf("Image: image_size_bytes: %d\n", image_head.image_size_bytes);
    printf("Image: x_resolution_ppm: %d\n", image_head.x_resolution_ppm);
    printf("Image: y_resolution_ppm: %d\n", image_head.y_resolution_ppm);
    printf("Image: num_colors: %d\n", image_head.num_colors);
    printf("Image: important_colors: %d\n", image_head.important_colors);
}

struct complex_num {
    double real;
    double img;
};

struct complex_num mult_C(struct complex_num A, struct complex_num B) {
    struct complex_num C;
    C.real = A.real * B.real - A.img * B.img;
    C.img = A.real * B.img + A.img * B.real;
    return C;
}

struct complex_num sqr_C(struct complex_num A) {
    struct complex_num C;
    C.real = A.real * A.real - A.img * A.img;
    C.img = 2 * A.real * A.img;
    return C;
}

struct complex_num add_C(struct complex_num A, struct complex_num B) {
    struct complex_num C;
    C.real = A.real + B.real;
    C.img = A.img + B.img;
    return C;
}

struct complex_num minus_C(struct complex_num A, struct complex_num B) {
    struct complex_num C;
    C.real = A.real - B.real;
    C.img = A.img - B.img;
    return C;
}

struct complex_num sin_C(struct complex_num A) {
    struct complex_num C;
    C.real = sin(A.real) * cosh(A.img);
    C.img = cos(A.real) * sinh(A.img);
    return C;
}

int Mandelbrot_recurse(struct complex_num C) {
    int count = 0;
    struct complex_num C2 = C;
    while (count < ITERATIONS) {
        C2 = sqr_C(C2);
        C2 = add_C(C2, C);
        if (C2.real > XUPPER || C2.real < XLOWER || C2.img > YUPPER || C2.img < YLOWER) {
            return OUT;
        }
        count++;
    }
    return IN;
}

int Mandelbrot_recurse_alt(struct complex_num C) {
    int count = 0;
    struct complex_num C2 = C;
    while (count < ITERATIONS) {
        C2 = mult_C(C2, sin_C(C2));
        if (C2.real > 1 || C2.real < -2 || C2.img > 1 || C2.img < -1) {
            return OUT;
        }
        count++;
    }
    return IN;
}

int main(int argc, char **argv) {
    FILE *fptr = fopen("MandelbrotSet.bmp", "wb+");

    if (fptr == NULL) {
        fprintf(stderr, "Error opening/creating file - (%d)\n", errno);
        exit(1);
    }

    struct BMP_HEADER head = init_BMP_HEAD(WIDTH, HEIGHT);

    if (!fwrite(&head, 54, 1, fptr)) {
        fprintf(stderr, "Error writing header data to .bmp file - (%d)\n", errno);
        exit(1);
    }

    if (fseek(fptr, head.offset - 1, SEEK_SET) == -1) {
        fprintf(stderr, "Error finding data offset in the bmp file - (%d)\n", errno);
        exit(1);
    }

    int *fractal = (int*)malloc(head.image_size_bytes);

    if (fractal == NULL) {
        fprintf(stderr, "Error allocating memory for the bmp image - (%d)\n", errno);
        exit(1);
    }


    double x_step = (XUPPER - XLOWER) / WIDTH;
    double y_step = (YUPPER - YLOWER) / HEIGHT;

    struct complex_num Z;

    for (int h = 0; h < HEIGHT; ++h) {
        Z.img = h * y_step + YLOWER;
        for (int w = 0; w < WIDTH; ++w) {
            Z.real = w * x_step + XLOWER;
            if (Mandelbrot_recurse(Z)) {
                *(fractal + (h * WIDTH + w)) = BLACK;
            } else {
                *(fractal + (h * WIDTH + w)) = WHITE;
            }
        }
    }

    if (!fwrite(fractal, head.image_size_bytes, 1, fptr)) {
        fprintf(stderr, "Error writing data to the bmp image - (%d)\n", errno);
        exit(1);
    }

    free(fractal);

    fclose(fptr);

    printf("Complete :)\n");

    return 0;
}