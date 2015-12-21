# include <stdio.h>
# include <stdlib.h>
# include <string.h>

struct JPEG_TIFF_Header
{
	char start_marker[2];
	char app1_marker[2];
	short app1_length;
	char exif[6];
	short endianness;
	short version_number;
	int offset;
};

struct TIFF_Tag
{
	short tag_id;
	short data_type;
	int num_items;
	int offset_or_data;
};

int main(int arg, char *args[])
{
	struct JPEG_TIFF_Header Header;
	struct TIFF_Tag Tag;
	unsigned short count[1];
	short valid_TIFF_tags[3] = {0x010F,0x0110,0x8769};
	short valid_TIFF_tags2[7] = {0xA002,0xA003,0x8827,0x829A,0x829D,0x920A,0x9003};
	char exif[5] = "Exif";
	int i;
	int j;
	int current_offset;
	float x;
	float y;
	float z;
	char string[100];
	unsigned int uint[2];
	int goto_sub_block = 0;
	
	FILE *img = fopen(args[1], "r");
	
	//Read in the Exif/Jpeg/Tiff Header to the struct 'header'
	//Read in the number of Tiff Tags to the int 'count' and mark the current_offset as '22' because 22 bytes have currently been read in
	fread(&Header, 1, 20, img);
	fread(count, 2, 1, img);
	current_offset = 22;

	//If the 'Exif' string is not in the header	display an error message and end the program
	if(strcmp(Header.exif,exif) != 0)
		printf("Exif Tag does not exists. Please try another image file.\n");
		
	//If the endianness is not equal to '0x4949' then display an error message and end the program
	else if(Header.endianness != 0x4949)
	{
		printf("Endianness not supported. Please try another image file.\n");
	}
	
	else
	{
	
	//Iterate through the Tiff Tags until the "0x8769" Tag is found to move to Exif sub block or until the last tag is reached.
	for(i = 0; i < count[0] && !goto_sub_block; i++)
	{
		//Read in Tag and update current offset
		fread(&Tag, 1, 12, img);
		current_offset += 12;
		
		//Iterate through the first array of valid TIFF tags
		for(j = 0; j < 3; j++)
		{
			if(Tag.tag_id == valid_TIFF_tags[j])
			{
				//If the Manufacturer/Model tag is found then seek to the offset given in its data section
				//Read in the string using the number of data items as a max character value
				//Finally seek back to the current offset and print out the data string that was found
				if(Tag.data_type == 2)
				{
					fseek(img, (Tag.offset_or_data + 12), SEEK_SET);
					fread(string, Tag.num_items, 1, img);
					fseek(img, current_offset, SEEK_SET);
				
					if(Tag.tag_id == valid_TIFF_tags[0])
						printf("Manufacturer: %s\n",string);
				
					else
						printf("Model: %s\n",string);
				}
				
				//If the Exif sub block tag is found then raise the flag to end the outer for loop
				//Update the current offset to the Exif sub block address and then seek to that offset
				//Read in the number of Tiff Tags to the int 'count' and update the current offset again
				else
				{
					goto_sub_block = 1;
					current_offset = Tag.offset_or_data + 12;
					fseek(img, current_offset, SEEK_SET);
					fread(count, 2, 1, img);
					current_offset += 2;
				}	
			}
		}
	}
	
	//Iterate through the sub block Tiff Tags until the last tag is reached.
	for(i = 0; i < count[0]; i++)
	{
		//Read in Tag and update current offset
		fread(&Tag, 1, 12, img);
		current_offset += 12;
		
		//Iterate through the second array of valid TIFF tags
		for(j = 0; j < 7; j++)
		{
			if(Tag.tag_id == valid_TIFF_tags2[j])
			{
				//If the Date Taken tag is found then seek to the offset given in its data section
				//Read in the string using the number of data items as a max character value
				//Finally seek back to the current offset and print out the data string that was found
				if(Tag.data_type == 2)
				{
					fseek(img, (Tag.offset_or_data + 12), SEEK_SET);
					fread(string, Tag.num_items, 1, img);
					fseek(img, current_offset, SEEK_SET);

					printf("Date Taken: %s\n",string);
				}
				
				//If the ISO speed tag is found simply use the last 3 bytes as the value of the data and print out the data that was found
				else if(Tag.data_type == 3)
				{	
					printf("ISO: ISO %d\n",Tag.offset_or_data);
				}
				
				//If the Height/Width tag is found simply use the last 4 bytes as the value of the data and print out the data that was found
				else if(Tag.data_type == 4)
				{	
					if(Tag.tag_id == valid_TIFF_tags2[0])
						printf("Width: %d pixels\n",Tag.offset_or_data);
					else
						printf("Height: %d pixels\n",Tag.offset_or_data);
				}
				
				//If the Exposure Speed/F-Stop/Focal Length tag is found then seek to the offset given in its data section
				//Read in 2 unsigned ints into the unsigned int array 'uint'
				//Seek back to the current offset
				//Calculate the fraction of the two integers by assigning their values to floats and dividing them
				//Finally print out the data string that was found
				if(Tag.data_type == 5)
				{
					fseek(img, (Tag.offset_or_data + 12), SEEK_SET);
					fread(uint, 8, 1, img);
					fseek(img, current_offset, SEEK_SET);
	
					x = uint[0];
					y = uint[1];
					z = x/y;
					
					if(Tag.tag_id == valid_TIFF_tags2[3])
						printf("Exposure Time: %d/%d second\n",uint[0],uint[1]);
					else if(Tag.tag_id == valid_TIFF_tags2[4])
					{
						printf("F-Stop: f/%.2f\n",z);	
					}
					else if(Tag.tag_id == valid_TIFF_tags2[5])
						printf("Focal Length: %.2f mm\n",z);
						
				}
				
			}
		}
	}
	
	}
}
