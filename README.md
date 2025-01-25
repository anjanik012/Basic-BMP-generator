# Basic BMP (Bitmap) image file generator

Geneates a BMP image file with a specified width and height in pixels, and fills it with a specified color.
Colors are 24 bit RGB. The BMP file format is a simple bitmap format that is widely supported by image processing software. The file is based on **Windows BITMAPINFOHEADER** structure. Refer https://en.wikipedia.org/wiki/BMP_file_format for more details.

## Build
```bash
gcc src/main.c -o bmpgen
```

## Usage
You can define color to be used image for each pixel with options for Red, Green, Blue values. The width and height of the image can also be specified. The output file name is also required.

### Options
- **-r** : Red color value (0-255)
- **-g** : Green color value (0-255)
- **-b** : Blue color value (0-255)
- **-w** : Width of the image in pixels
- **-h** : Height of the image in pixels
- Output file

```bash
$ ./bmpgen -r 200 -b 50 -g 99 -w 100 -h 100 sample.bmp
Saving image to file name: sample.bmp
Using color for each pixel, red: 200, blue: 50, green: 99
Image resolution: 100X100
Bytes written: 30054 to file: sample.bmp
```

## Sample Images
![Sample Image](sample.bmp)
![Sample Image](sample2.bmp)

**I created this only to explore basic image formats and binary file manipulation**
