#ifndef LIFEHASH_PNG_WRITER_HPP
#define LIFEHASH_PNG_WRITER_HPP

// ======================================================================
// PngWriter.hpp
//
// This class can write a png image using libpng routines (i.e. png.h).
//
// Sample usage:
//
//    PngWriter png(nx,ny);
//
//    // then one or more calls to...
//
//    png.set(i, j, red, green, blue); // 0 <= red,green,blue <= 255
//
//    png.write("myfile.png");
//
//    // at this point you can change the image and write again...
//
//    png.set(i, j, red, green, blue);
//
//    png.write("myfile2.png");
//
//
// History:
//  authors: Frank Ham & Phuc Quang - July 2013
// ======================================================================

#include <png.h>
#include <string>
#include <iostream>

class PNGWriter {
   private:
    uint8_t (*storage)[3];  // 0 <= r,g,b < 255
    size_t width, height;

   public:
    PNGWriter(size_t width, size_t height) : width(width), height(height) {
        storage = new uint8_t[width * height][3];

        for (size_t i = 0; i < width * height; ++i) {
            storage[i][0] = 0;
            storage[i][1] = 0;
            storage[i][2] = 0;
        }
    }

    ~PNGWriter() { delete[] storage; }

    void set(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b) {
        // recall that for png files, the pixels are ordered from the top left, so modify
        // this set routine's passed j so that zero is at the bottom left...
        storage[y * width + x][0] = r;
        storage[y * width + x][1] = g;
        storage[y * width + x][2] = b;
    }

    void write(std::string const& filename) {
        // note: we completely skip any error handling treatment here for simplicity.

        FILE* fp = fopen(filename.c_str(), "wb");
        if (!fp) {
            std::cout << "Warning: could not open png file: " << filename << std::endl;
            return;
        }

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
        if (!png_ptr) {
            fclose(fp);
            std::cout << "Warning: could not create png_ptr" << std::endl;
            return;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
            std::cout << "Warning: could not create info_ptr" << std::endl;
            return;
        }

        png_init_io(png_ptr, fp);

        png_set_IHDR(png_ptr, info_ptr, width, height,  // width, height
                     8,                          // bits per pixel -- 16 does not work with blockbuster
                     PNG_COLOR_TYPE_RGB,         // non-alpha options are PNG_COLOR_TYPE_RGB,PNG_COLOR_TYPE_GRAY,
                     PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        // Some bits per pixel notes: 16 does not work with blockbuster, and there are also
        // issues with PNG_COLOR_TYPE_GRAY interpretation, so stick to 8 and PNG_COLOR_TYPE_RGB
        // for now. Note that if you do use 16, pay attention to MSB/LSB order. Endian is
        // flipped on my linux workstation...

        png_write_info(png_ptr, info_ptr);

        // set up row pointers to point into the raw image data in buffer...

        png_byte* row_pointers[height];
        for (size_t i = 0; i < height; ++i) row_pointers[i] = (png_byte*)(storage + i * width);

        png_write_image(png_ptr, row_pointers);

        png_write_end(png_ptr, NULL);

        png_destroy_write_struct(&png_ptr, &info_ptr);

        fclose(fp);

        // leave the image data in buffer to allow the calling process to change it and/or
        // write another image. It is deleted in the destructor.
    }
};

#endif
