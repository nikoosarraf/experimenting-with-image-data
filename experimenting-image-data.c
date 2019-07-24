/* FILE: A3_solutions.c is where you will code your answers for Assignment 3.
 * 
 * Each of the functions below can be considered a start for you. They have 
 * the correct specification and are set up correctly with the header file to
 * be run by the tester programs.  
 *
 * You should leave all of the code as is, especially making sure not to change
 * any return types, function name, or argument lists, as this will break
 * the automated testing. 
 *
 * Your code should only go within the sections surrounded by
 * comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A3_solutions.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "A3_provided_functions.h"

unsigned char*
bmp_open( char* bmp_filename,        unsigned int *width, 
          unsigned int *height,      unsigned int *bits_per_pixel, 
          unsigned int *padding,     unsigned int *data_size,
	  unsigned int *data_offset					)
{
  unsigned char *img_data=NULL;
  // REPLACE EVERYTHING FROM HERE

  //opening file
  FILE *bmpfile = fopen( bmp_filename, "rb");
  if (bmpfile==NULL){
	printf("error opening file");
 	return NULL;
}

  char b, m;
  fread(&b,1,1,bmpfile);
  fread(&m,1,1,bmpfile);

  //get file size
  fread( data_size,1,sizeof(unsigned int), bmpfile);
  
  //rewind file pointer to the beginning and read the entire contents
  rewind(bmpfile);
  img_data = (unsigned char *) malloc(sizeof(unsigned char) *(*data_size));
  if( fread( img_data,1,*data_size, bmpfile)!= *data_size) {
	printf("I was unable to read the requested %d bytes.\n", *data_size);
	return NULL;
  }
    
  //width
  unsigned int* w = (unsigned int*)(img_data+18);
  *width = *w;

  //height
  unsigned int* h = (unsigned int*)(img_data+22);
  *height = *h;

  //bpp
  unsigned short* bpp = (unsigned short*)(img_data+28);
  *bits_per_pixel = *bpp;

  //padding
  unsigned int p = (unsigned int)(*width%4); 
  *padding = p;

  //data_offset
  unsigned int* data = (unsigned int*)(img_data+10);
  *data_offset = *data;
  // TO HERE!  
  return img_data;  
}

void 
bmp_close( unsigned char **img_data )
{
  // REPLACE EVERYTHING FROM HERE
  free(*img_data);
  img_data=NULL;
  // TO HERE!  
}

unsigned char***  
bmp_scale( unsigned char*** pixel_array, unsigned char* header_data, unsigned int header_size,
           unsigned int* width, unsigned int* height, unsigned int num_colors,
           float scale )
{
  unsigned char*** new_pixel_array = NULL; 
  // REPLACE EVERYTHING FROM HERE
  unsigned int newHeight = (unsigned int)((*height)*scale);
  *height = newHeight;
  unsigned int newWidth = (unsigned int)((*width)*scale);
  *width = newWidth;
  unsigned int pixel_size = (unsigned int) ((newWidth)*(newHeight)*(num_colors));
  unsigned int padding = ((4-((newWidth*num_colors)%4))%4);
  *(((unsigned int*)(header_data+2))) = (header_size + pixel_size + (newHeight*padding));
  *(((unsigned int*)(header_data+18))) = newWidth;
  *(((unsigned int*)(header_data+22))) = newHeight;
       
  new_pixel_array = (unsigned char***)malloc( sizeof(unsigned char**) * (*height));
  if( new_pixel_array == NULL ){
    printf( "Error: failed to allocate memory for image of height %d.\n", (*height) );
    return NULL;
  }
  
  for( int row=0; row<*height; row++ ){
    new_pixel_array[row] = (unsigned char**)malloc( sizeof(unsigned char*) * (*width) );
    for( int col=0; col<*width; col++ ){
      new_pixel_array[row][col] = (unsigned char*)malloc( sizeof(unsigned char) * (num_colors) );
    }
  }
    
  for( int row=0; row<*height; row++ )
    for( int col=0; col<*width; col++ )
	for (int color =0; color<num_colors; color++)
	 new_pixel_array[row][col][color] = pixel_array[(int)(row/scale)][(int)(col/scale)][color];
  // TO HERE! 
  return new_pixel_array;
}         

int 
bmp_collage( char* background_image_filename,     char* foreground_image_filename, 
             char* output_collage_image_filename, int row_offset,                  
             int col_offset,                      float scale )
{
  // REPLACE EVERYTHING FROM HERE
  //make a char array of each image
  //loop thru background and where it overlaps w the foreground (if pixels are not = to 0), replace the pixel with the foreground
  //bmp from 3d array

  //background
  unsigned int height1;
  unsigned int width1;
  unsigned char* header_data1;
  unsigned int header_size1;
  unsigned int num_colors1;
  
  //foreground
  unsigned int height2;
  unsigned int width2;
  unsigned char* header_data2;
  unsigned int header_size2;
  unsigned int num_colors2;
  
  unsigned char*** background_pixel_array = bmp_to_3D_array(background_image_filename, &header_data1, &header_size1, &width1, &height1, &num_colors1);
  unsigned char*** foreground_pixel_array = bmp_to_3D_array(foreground_image_filename, &header_data2, &header_size2, &width2, &height2, &num_colors2); 
  unsigned char*** new_foreground = NULL;

  new_foreground = bmp_scale(foreground_pixel_array, header_data2, header_size2, &width2, &height2, num_colors2, scale);
  
  for (int row =0; row<height2; row++)
     for (int col=0; col<width2; col++)
        for (int color =0; color<num_colors1; color++)
	  if (new_foreground[row][col][color] != 0)
		background_pixel_array[row+row_offset][col+col_offset][color] = new_foreground[row][col][color];
	 
  bmp_from_3D_array(output_collage_image_filename, header_data1, (int)header_size1, background_pixel_array, (int)width1, (int)height1, (int)num_colors1);
 
  // TO HERE! 
  return 0;
}             
