// The programe reads the image data from an image file "~.raw"
// Last updated on 02/20/2010 by Steve Cho

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "const_def_DCT_Quantization.h"
#include "Matrix_ImageBlocking.h"


// Here we assume the image is of size 256*256 and is of raw format
// You will need to make corresponding changes to accommodate images of different sizes and types


int main(int argc, char *argv[])

{
	// file pointer
	FILE *file;
	int BytesPerPixel = 1; 	// 1: gray image, 3: color image
	int Size = 16;        		// default size
	
	// image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];
	
	if(argc < 3){
		printf("usage: program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 256]\n");
		exit(1);
	}
	if(argc >= 4 ){
		BytesPerPixel = atoi(argv[3]);
	}
	if(argc >= 5 ){
		Size = atoi(argv[4]);
	}
	
	if(BytesPerPixel > 3 || Size > 256){
		printf("This code assumes BytesPerPixel is less or equal to 3, Size is less or equal to 256\n Violating this assumption may lead to wrong result or seg fault\n");
	}
    
	// read image "ride.raw" into image data matrix
	if (!(file=fopen(argv[1],"rb")))
	{
		printf("Cannot open file1: %s\n", argv[1]);
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
    
	// do some image processing task...
    
 //print the entire image
    for (int i = 0; i < Size; i ++) {
        for (int j = 0; j < Size; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                printf("%d ",Imagedata[i][j][u]);
            }
        }
        printf("\n");
    }
    
    // offset processing
    char Imagedata_Offset[Size][Size][BytesPerPixel];
    for (int i = 0; i < Size; i ++) {
        for (int j = 0; j < Size; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                Imagedata_Offset[i][j][u] = Imagedata[i][j][u] - 128;
            }
        }
    }
    printf("done!\n");
    
    //    for (int i = 0; i < UNIT_SIZE; i ++) {
    //        for (int j = 0; j < UNIT_SIZE; j ++) {
    //            for (int u = 0; u < BytesPerPixel; u ++) {
    //                printf("%d ",Imagedata_Offset[i][j][u]);
    //            }
    //        }
    //        printf("\n");
    //    }
    
    // Partitioning, split the image into blocks
    int block_num = Size/UNIT_SIZE;
    UncharMatrix block[block_num][block_num][BytesPerPixel]; // create block array(3D)
    UncharMatrix * blockPointer[block_num][block_num][BytesPerPixel];
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                blockPointer[i][j][u] = &block[i][j][u];
            }
        }
    }
    
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            
            for (int ii = 0; ii < UNIT_SIZE; ii ++) {
                for (int jj = 0; jj < UNIT_SIZE; jj ++) {
                    for (int u = 0; u < BytesPerPixel; u ++) {
                        // In this problem, only deal with the grey scale image
                        block[i][j][u].data[ii][jj] = Imagedata_Offset[ii + i*UNIT_SIZE][jj + j * UNIT_SIZE][u];
                    }
                }
            }
            
        }
    }
    
    //    for (int i = 0; i < UNIT_SIZE; i ++) {
    //        for (int j = 0; j < UNIT_SIZE; j ++) {
    //            printf("%d ",block[0][0][0].data[i][j]);
    //        }
    //        printf("\n");
    //    }
    
    // 2DCT
    DoubleMatrix dct_coeff[block_num][block_num][BytesPerPixel]; // create matrix to store the DCT result
    DoubleMatrix * dct_pointer[block_num][block_num][BytesPerPixel];
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                dct_pointer[i][j][u] = &dct_coeff[i][j][u];
            }
        }
    }
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                dct_pointer[i][j][u] = two_D_dct(blockPointer[i][j][u]);;
            }
        }
    }
    
    // Quantization
    QUANTIZATION_MATRIX;
    DoubleMatrix quan_coeff[block_num][block_num][BytesPerPixel]; // create matrix to store the quantization result
    DoubleMatrix * quan_pointer[block_num][block_num][BytesPerPixel];
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                quan_pointer[i][j][u] = &quan_coeff[i][j][u];
            }
        }
    }
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                
                for (int ii = 0; ii < UNIT_SIZE; ii ++) {
                    for (int jj = 0; jj < UNIT_SIZE; jj ++) {
                        quan_pointer[i][j][u]->data[ii][jj] = round(dct_pointer[i][j][u]->data[ii][jj]/Q_50[ii][jj]);
                    }
                }
                
            }
        }
    }
    print_MatrixDouble(quan_pointer[0][0][0]);
    
    // Combine blocks into an image
    char ImageOutput[Size][Size][BytesPerPixel];
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                
                for (int ii = 0; ii < UNIT_SIZE; ii ++) {
                    for (int jj = 0; jj < UNIT_SIZE; jj ++) {
                        // In this problem, only deal with the grey scale image
                        ImageOutput[ii + i * UNIT_SIZE][jj + j * UNIT_SIZE][u] = quan_pointer[i][j][u]->data[ii][jj];
                    }
                }
                
            }
        }
    }
    
    
    // quantization matrix manipulation
    UncharMatrix q_50,q_90,q_10;
    UncharMatrix * q_50_pointer = &q_50;
    UncharMatrix * q_90_pointer = &q_90;
    UncharMatrix * q_10_pointer = &q_10;
    
    for (int i = 0; i < UNIT_SIZE; i ++) {
        for (int j = 0; j < UNIT_SIZE; j ++) {
            q_50_pointer->data[i][j] = Q_50[i][j];
        }
    }
    
    q_90_pointer = transferQuantizationMatrix(q_50_pointer, 90);
    q_10_pointer = transferQuantizationMatrix(q_50_pointer, 10);
    
    // quantize Lena image using two newly created quantization matrices
    DoubleMatrix quan_90_coeff[block_num][block_num][BytesPerPixel]; // create matrix for Q90
    DoubleMatrix * quan_90_pointer[block_num][block_num][BytesPerPixel];
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                quan_90_pointer[i][j][u] = &quan_90_coeff[i][j][u];
            }
        }
    }
    DoubleMatrix quan_10_coeff[block_num][block_num][BytesPerPixel]; // create matrix for Q10
    DoubleMatrix * quan_10_pointer[block_num][block_num][BytesPerPixel];
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                quan_10_pointer[i][j][u] = &quan_10_coeff[i][j][u];
            }
        }
    }
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                
                for (int ii = 0; ii < UNIT_SIZE; ii ++) {
                    for (int jj = 0; jj < UNIT_SIZE; jj ++) {
                        quan_90_pointer[i][j][u]->data[ii][jj] = round(dct_pointer[i][j][u]->data[ii][jj]/q_90_pointer->data[ii][jj]);
                    }
                }
                
            }
        }
    }
    print_MatrixDouble(quan_90_pointer[0][0][0]);
    for (int i = 0; i < block_num; i ++) {
        for (int j = 0; j < block_num; j ++) {
            for (int u = 0; u < BytesPerPixel; u ++) {
                
                for (int ii = 0; ii < UNIT_SIZE; ii ++) {
                    for (int jj = 0; jj < UNIT_SIZE; jj ++) {
                        quan_10_pointer[i][j][u]->data[ii][jj] = round(dct_pointer[i][j][u]->data[ii][jj]/q_10_pointer->data[ii][jj]);
                    }
                }
                
            }
        }
    }
    print_MatrixDouble(quan_10_pointer[0][0][0]);
    
    
	// write image data to "~.raw"
	if (!(file=fopen(argv[2],"wb")))
	{
		printf("Cannot open file2: %s\n", argv[2]);
		exit(1);
	}
	fwrite(ImageOutput, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
    
	return 0;
}

