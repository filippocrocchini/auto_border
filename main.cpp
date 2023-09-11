#pragma warning (disable : 4996 )

#include "path_utils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

void process_image(const char* image_path)
{
    int width       = 0;
    int height      = 0;
    int components  = 0;
    auto image_data = stbi_load(image_path, &width, &height, &components, 3);

    if (image_data)
    {
        pixel* pixels = (pixel*) image_data;

        float aspect_ratio = 1;

        if (width < height)
        {
            aspect_ratio = 4.f / 5.f;
        }

        float min_border = 0.035f;

        int out_width  = width  * (1 + 2 * min_border);
        int out_height = height * (1 + 2 * min_border);

        int snapped_width  = out_height * aspect_ratio;
        int snapped_height = out_width  / aspect_ratio;

        if (out_width < snapped_width)
        {
            out_width = snapped_width;
        }
        else 
        {
            out_height = snapped_height;
        }

        int x_border = (out_width - width) / 2;
        int y_border = (out_height - height) / 2;

        pixel* out_pixels = (pixel*) malloc(sizeof(pixel) * out_width * out_height);

        // Fill with border color
        for (size_t y = 0; y < out_height; y++)
        {
            pixel* row_start = out_pixels + y * out_width;

            for (size_t x = 0; x < out_width; x++)
            {
                if (y >= y_border && y < y_border + height && x >= x_border && x < width + x_border)
                {
                    row_start[x] = pixels[(y - y_border) * width + x - x_border];
                }
                else
                {
                    row_start[x].r = 255;
                    row_start[x].g = 255;
                    row_start[x].b = 255;
                }
            }
        }

        str8_path out_path = split_path(image_path);

        char out_file_path[_MAX_PATH] = {};
        sprintf_s(out_file_path,
            "%.*s%.*s.border%.*s",
            (int) out_path.directory.count, out_path.directory.data,
            (int) out_path.name.count, out_path.name.data,
            (int) out_path.extension.count, out_path.extension.data
            );

        stbi_write_jpg(out_file_path, out_width, out_height, 3, out_pixels, 90);

        free(out_pixels);
        stbi_image_free(image_data);
    }
}

int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        printf("Usage: auto_border <image_path> ...\n");
    }

    for (int i = 1; i < argc; i++)
    {
        process_image(argv[i]);
    }

    return 0;
}