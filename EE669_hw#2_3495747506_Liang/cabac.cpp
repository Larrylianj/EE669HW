//
//  main.cpp
//  CABAC_coder
//
//  Created by Siyang Liang on 2/13/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <iostream>
#include "QMCODER.H"
#define BYTE_OF_BIT 8


int main(int argc, const char * argv[])
{
    if (argc != 3) {
        printf(" format: program_name input_file output_file\n");
        exit(1);
    }

    FILE * input_handle = NULL;
    FILE * output_handle = NULL;
    unsigned char buf;
    unsigned char bit;
    int context;
    
    input_handle = fopen(argv[1], "rb");
    if (NULL == input_handle) {
        printf("Fail to open the file, check the file path!\n");
        exit(1);
    }
    
    output_handle = fopen(argv[2], "wb");
    if (NULL == output_handle) {
        printf("Fail to open the file, check the file path!\n");
        exit(1);
    }


    QM cabac_coder(output_handle);
    cabac_coder.StartQM("encode");
    cabac_coder.reset();
    while (fread(&buf, sizeof(unsigned char), 1, input_handle)) {
        
        for (int i = 0; i < BYTE_OF_BIT; i ++) {
            
            // decide bit value
            if (buf > 0x80) {
                bit = 1;
            }else{
                bit = 0;
            }
            // decide context value
            if (buf & 0x02 == 0x02) {
                context = 1;
            }else{
                context = 0;
            }
//            switch (buf & 0x06) {
//                case 0x00:
//                    context = 0;
//                    break;
//                case 0x02:
//                    context = 1;
//                    break;
//                case 0x04:
//                    context = 2;
//                    break;
//                case 0x06:
//                    context = 3;
//                    break;
//                    
//                default:
//                    break;
//            }
//            switch (buf & 0x0E) {
//                case 0x00:
//                    context = 0;
//                    break;
//                case 0x02:
//                    context = 1;
//                    break;
//                case 0x04:
//                    context = 2;
//                    break;
//                case 0x06:
//                    context = 3;
//                    break;
//                case 0x08:
//                    context = 4;
//                    break;
//                case 0x0A:
//                    context = 5;
//                    break;
//                case 0x0C:
//                    context = 6;
//                    break;
//                case 0x0E:
//                    context = 7;
//                    break;
//                default: context = 0;
//                    break;
//            }
            cabac_coder.encode(bit, context);
            buf <<= 1;
        }

    }
    cabac_coder.Flush();
    printf("Count = %d\n",cabac_coder.Counting());
    
//    QM cabac_decoder(input_handle);
//    cabac_decoder.StartQM("decode");
//    cabac_decoder.reset();
//    cabac_decoder.DebugOn();
//    do {
//        for (int i = 0; i < BYTE_OF_BIT; i ++) {
//            
//            bit = cabac_decoder.decode(2);
//            buf = 0x00;
//            if (bit == 1) {
//                buf = buf | 0x80;
//            }else{
//                bit = buf | 0x00;
//            }
//            buf <<= 1;
//            
//        }
//        fwrite(&buf, sizeof(unsigned char), 1, output_handle);
//    }while (!cabac_decoder.isEnd());
    
    fclose(input_handle);
    fclose(output_handle);
//    closeFile(recon_handle);
//    closeFile(reconOut_hanlde);
    
}

