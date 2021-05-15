/*********************************
 jpegtest.c
 $ gcc jpegtest.c -std=c11 -ljpeg
 *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <jpeglib.h>

#define   azul 0 
#define   verde 1 
#define  vermelho 2 
// JPEG image struct
typedef struct {
    uint8_t *data;   // raw data
    uint32_t width;
    uint32_t height;
    uint32_t ch;     // color channels
} JpegData;
 typedef struct {
    uint8_t azul2;
    uint8_t verde2;
    uint8_t  vermelho2;
 }Cor;  
// allocate memory for raw data
void alloc_jpeg(JpegData *jpegData)
{
    jpegData->data = NULL;
    jpegData->data = (uint8_t*) malloc(sizeof(uint8_t)  *
                                       jpegData->width  *
                                       jpegData->height *
                                       jpegData->ch);
}

// free memory for raw data
void free_jpeg(JpegData *jpegData)
{
    if (jpegData->data != NULL) {
        free(jpegData->data);
        jpegData->data = NULL;
    }
}

// read JPEG image
// 1. create JPEG decompression object
// 2. specify source data
// 3. read JPEG header
// 4. set parameters
// 5. start decompression
// 6. scan lines
// 7. finish decompression
bool read_jpeg(JpegData *jpegData,
              const char *srcfile,
              struct jpeg_error_mgr *jerr)
{
    // 1.
    struct jpeg_decompress_struct cinfo;
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(srcfile, "rb");
    if (fp == NULL) {
        printf("Error: failed to open %s\n", srcfile);
        return false;
    }
    // 2.
    jpeg_stdio_src(&cinfo, fp);

    // 3.
    jpeg_read_header(&cinfo, TRUE);

    // 4. omitted
    // 5.
    jpeg_start_decompress(&cinfo);

    jpegData->width  = cinfo.image_width;
    jpegData->height = cinfo.image_height;
    jpegData->ch     = cinfo.num_components;

    alloc_jpeg(jpegData);

    // 6. read line by line
    uint8_t *row = jpegData->data;
    const uint32_t stride = jpegData->width * jpegData->ch;
    for (int y = 0; y < jpegData->height; y++) {
        jpeg_read_scanlines(&cinfo, &row, 1);
        row += stride;
    }

    // 7.
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return true;
}

// write JPEG image
// 1. create JPEG compression object
// 2. specify destination data
// 3. set parameters
// 4. start compression
// 5. scan lines
// 6. finish compression
bool write_jpeg(const JpegData *jpegData,
                const char *dstfile,
                struct jpeg_error_mgr *jerr)
{
    // 1.
    struct jpeg_compress_struct cinfo;
    jpeg_create_compress(&cinfo);
    cinfo.err = jpeg_std_error(jerr);

    FILE *fp = fopen(dstfile, "wb");
    if (fp == NULL) {
        printf("Error: failed to open %s\n", dstfile);
        return false;
    }
    // 2.
    jpeg_stdio_dest(&cinfo, fp);

    // 3.
    cinfo.image_width      = jpegData->width;
    cinfo.image_height     = jpegData->height;
    cinfo.input_components = jpegData->ch;
    cinfo.in_color_space   = JCS_RGB;
    jpeg_set_defaults(&cinfo);

    // 4.
    jpeg_start_compress(&cinfo, TRUE);

    // 5.
    uint8_t *row = jpegData->data;
    const uint32_t stride = jpegData->width * jpegData->ch;
    for (int y = 0; y < jpegData->height; y++) {
        jpeg_write_scanlines(&cinfo, &row, 1);
        row += stride;
    }

    // 6.
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(fp);

    return true;
}

int main(void)
{
    JpegData jpegData;
    struct jpeg_error_mgr jerr;

    // src/dst file
    char *src = "./alfaceeditada.jpg";
    char *dst = "./out.jpg";

    if (!read_jpeg(&jpegData, src, &jerr)){
        free_jpeg(&jpegData);
        return -1;
    }
    printf("Read:  %s\n", src);

    int size = jpegData.width * jpegData.height * jpegData.ch;
    int contacor;
    int contaverde;   
      contacor=0;
      contaverde=0;
    int contaverdadeiro=0;
    uint8_t*   data=jpegData.data;
    Cor cororiginal;       
    for (int i = 0; i < size; i++){
         if(contacor==azul){
          cororiginal.azul2=jpegData.data[i];
          contacor++;
         }else if(contacor==verde){
           cororiginal.verde2=jpegData.data[i];
           contacor++;      
         }else{
           cororiginal.vermelho2=jpegData.data[i];
           if((cororiginal.azul2>10&&cororiginal.verde2>55&&cororiginal.verde2<240)||(cororiginal.verde2>55&&cororiginal.vermelho2>10&&cororiginal.verde2<240&&cororiginal.verde2)){    
              int i2;
              i=i-3;
              for(i2=0;i2<3;i2++,i++){
                jpegData.data[i]=255; 
              }        
              contaverde++;
             }
           contacor=0;   
         }  
        //substituir pela lógica da troca de coresx
         
    }
    printf(" a quantidade de pixels verde e %d", contaverde);  
    if (!write_jpeg(&jpegData, dst, &jerr)){
        free_jpeg(&jpegData);
        return -1;
    }
    printf("Write: %s\n", dst);

    free_jpeg(&jpegData);

    return 0;
}
