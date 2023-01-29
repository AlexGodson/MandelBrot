#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WHITE   0x00000000
#define BLUE    0xFF000000
#define GREEN   0x00FF0000
#define RED     0x0000FF00
#define BLACK   0xFFFFFF00

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


void init_BMP_HEAD(int width, int heigth, struct BMP_HEADER* head) {
    
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

int main(int argc, char **argv) {
    printf("Hello World\n");
    return 0;
}