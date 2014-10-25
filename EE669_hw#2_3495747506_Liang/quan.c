//
//  main.c
//  Llyod_Max_SQ
//
//  Created by Siyang Liang on 2/11/14.
//  Copyright (c) 2014 SiyangLiang. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "const_def_VQ.h"


void initializeStorage(unsigned int * string);
void initializeBound(unsigned int M, unsigned char * x, unsigned char * b, double * pdf);
void initializeBoundUniform(unsigned int M, unsigned char * x, unsigned char * b, double * pdf);
void calculatePDF(unsigned int * string, double * pdf);
unsigned int conditionalMean(unsigned char b_lower, unsigned char b_upper, double * pdf);
double calculateMse(unsigned int M, unsigned char * x, unsigned char * b, unsigned int * string, double * pdf);
void readFileIntoSymbol(FILE * fp, unsigned int * string);
void Lloyd_Max_SQ(FILE * fp1, FILE * fp2, FILE * fp3, FILE * fin, FILE * fout);
unsigned char findQuantizedValue(unsigned char symbol, unsigned char * b, unsigned char * x, int M);

void compactCode(unsigned char * b, unsigned int * string, unsigned int M);
int findPeakInterval(unsigned char * b, unsigned int * string, unsigned int M);

void calculatePSNRforImage(unsigned char * b, unsigned char * x, int M, FILE * fp);

void printHistogramToFile(unsigned int * string);

int main(int argc, const char * argv[])
{
    if (argc != 6) {
        printf("There should be 6 parameters! The format is:\n");
        printf("program_name training_set1 training_set2 training_set3 test_set1 output_image\n");
        exit(1);
    }
    
    FILE * training1 = NULL;
    FILE * training2 = NULL;
    FILE * training3 = NULL;
    FILE * test = NULL;
    FILE * output = NULL;
    
    if (!(training1 = fopen(argv[1],"rb")))
	{
		printf("Cannot open file1: %s\n", argv[1]);
		exit(1);
	}
    if (!(training2 = fopen(argv[2],"rb")))
	{
		printf("Cannot open file2: %s\n", argv[2]);
		exit(1);
	}
    if (!(training3 = fopen(argv[3],"rb")))
	{
		printf("Cannot open file3: %s\n", argv[3]);
		exit(1);
	}
    if (!(test = fopen(argv[4],"rb")))
	{
		printf("Cannot open file4: %s\n", argv[4]);
		exit(1);
	}
    if (!(output = fopen(argv[5],"wb")))
	{
		printf("Cannot open file1: %s\n", argv[5]);
		exit(1);
	}
    
    Lloyd_Max_SQ(training1, training2, training3, test, output);
    
    fclose(training1);
    fclose(training2);
    fclose(training3);
    fclose(test);
    fclose(output);
    
    return 0;
    
}



// fp1,fp2,fp3 are three training files, fin is the file to be quantized, fout is the file after quantization
void Lloyd_Max_SQ(FILE * fp1, FILE * fp2, FILE * fp3, FILE * fin, FILE * fout)
{
    
    // basic definition
    unsigned char buf;
    int i;
    
    int low_index = 0;
    int high_index = SYMBOL_NUM;
    int iter_count = 0;
    
    unsigned int M = pow(2, BIT_RANGE);
    
    unsigned char b_bound[M + 1];
    unsigned char x_value[M];
    
    unsigned int pixelString[SYMBOL_NUM];
    double pixelPDF[SYMBOL_NUM];
    
    double previous_mse = 0.0;
    double current_mse;
    double seta;
    
    // initialization of the symbol stroing string
    initializeStorage(pixelString);
    
    // Read the symbols from file and store their appearances into string
    readFileIntoSymbol(fp1, pixelString);
    readFileIntoSymbol(fp2, pixelString);
    readFileIntoSymbol(fp3, pixelString);
    
    // convert the symbol string counts to pdf
    calculatePDF(pixelString, pixelPDF);
    
    // set lower bound and upper bound
    while (pixelString[low_index] == 0) {
        
        low_index ++;
        
    }
    
    while (pixelString[high_index - 1] == 0) {
        
        high_index --;
        
    }
    
    b_bound[0] = low_index;
    b_bound[M] = high_index - 1;
    
    // Initialization of the M representation points x1,x2,x3...xM and M+1 boundaries b0,b1,b2...bM
    initializeBoundUniform(M, x_value, b_bound, pixelPDF);
    
    compactCode(b_bound, pixelString, M);
    
    // Do the iteration to find suitable quantization values
    while (iter_count < MAX_ITERATION){
        
        iter_count ++;
        printf("In the %d th iteration.\n",iter_count);
        
//        for (int i = 0; i < M; i ++) {
//            printf("x[%d] = %d\n",i,x_value[i]);
//        }
//        for (int i = 0 ; i < M + 1; i++) {
//            printf("b[%d] = %d\n",i,b_bound[i]);
//        }
        for (i = 1; i < M; i ++) {
            
            b_bound[i] = (x_value[i - 1]+x_value[i] + 1)/2;
            
        }
        
        compactCode(b_bound, pixelString, M);
        
        for (i = 0; i < M; i ++) {
            
            x_value[i] = conditionalMean(b_bound[i], b_bound[i + 1], pixelPDF);
            
        }
        
        current_mse = calculateMse(M, x_value, b_bound, pixelString, pixelPDF);
        
        seta = (previous_mse - current_mse)/current_mse;
        
        if ( seta <= ERROR_THRESHOLD && seta >= - ERROR_THRESHOLD){
            
            break;
            
        }
        
        previous_mse = current_mse;
        
    }
    
    printf("It takes %d iterations to converge.\n",iter_count);
    
    // Quantize the input images
    fseek(fin,0,SEEK_SET); // point the file handle back to the begining of the file
    
    calculatePSNRforImage(b_bound, x_value, M, fin);
    
    fseek(fin,0,SEEK_SET); // point the file handle back to the begining of the file
    
    while (fread(&buf, sizeof(unsigned char), 1, fin)) {
        
        buf = findQuantizedValue(buf, b_bound, x_value, M);
        fwrite(&buf, sizeof(unsigned char), 1, fout);
        
    }

}

// Initialize the symbol string
void initializeStorage(unsigned int * string)
{
    int i;
    
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        string[i] = 0;
        
    }
    
}

// Read symbols and store the # of appearance of each symbol
void readFileIntoSymbol(FILE * fp, unsigned int * string)
{
    
    unsigned char buf;
    int i;
    
    while (fread(&buf, sizeof(unsigned char), 1, fp)) {
        
        for (i = 0; i < SYMBOL_NUM; i ++) {
            
            if (buf == i) {
                
                string[i] ++;
                
            }
            
        }
        
    }
    
}

// Initialization of the M representation points x1,x2,x3...xM and boundaries b0,b1,b2...bM
void initializeBound(unsigned int M, unsigned char * x, unsigned char * b, double * pdf)
{
    int i;
    
    for (i = 1; i < M; i++) {
        
        b[i] = (b[M]-b[0])/M*i;
        
    }
    // I use the conditional mean to initialize point values
    for (i = 0; i < M; i++) {
        
        x[i] = conditionalMean(b[i], b[i + 1], pdf);
        
    }
//    for (int i = 0; i < M + 1; i++) {
//        printf("b[%d] = %d\n",i,b[i]);
//    }
    
}

// Initialization of the M representation points x1,x2,x3...xM and boundaries b0,b1,b2...bM to be uniformly distributed
void initializeBoundUniform(unsigned int M, unsigned char * x, unsigned char * b, double * pdf)
{
    int i;
    
    for (i = 1; i < M; i++) {
        
        b[i] = (b[M]-b[0])/M*i + 3*(i%M == 3);
        
    }
    
    x[0] = b[0] + (b[M] - b[0])/M/2;
    for (i = 1; i < M; i++) {
        
        x[i] = x[0] + (b[M]-b[0])/M*i + 3*(i%M == 3);
        
    }
    
}

// Calculate PDF for the symbol string
void calculatePDF(unsigned int * string, double * pdf)
{
    int i;
    long long sum_symbols = 0;
    
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        sum_symbols += string[i];
        
    }
    
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        pdf[i] = string[i]/(double)sum_symbols;
        
    }
    
}

// Calcualtor the conditional mean in a given bounded region
unsigned int conditionalMean(unsigned char b_lower, unsigned char b_upper, double * pdf)
{
    int i;
    
    double sum_den = 0;
    double sum_nom = 0;
    
    for (i = b_lower; i < b_upper; i ++) {
        
        sum_den += i*pdf[i];
        sum_nom += pdf[i];
        
    }
    
    return (int)(sum_den/sum_nom);
    
}

// Dealing with the Compact Code problem
void compactCode(unsigned char * b, unsigned int * string, unsigned int M)
{
    int i,j,k;
    
    long int count;
    int peakindex;
    
    for (i = 0; i < M; i ++) {
        
        count = 0;
        
        for (j = b[i]; j < b[i + 1]; j ++) {
            
            count += string[j];
            
        }
        
        if (count == 0) {
            
            peakindex = findPeakInterval(b, string, M);
            
            if (peakindex < i) {
                
                for (k = i; k > peakindex + 1; k --) {
                    
                    b[k] = b[k - 1];
                    
                }
                
                b[peakindex + 1] = (b[peakindex] + b[peakindex + 1])/2;
                
            }else{
                
                for (k = i + 1; k < peakindex; k ++) {
                    
                    b[k] = b[k + 1];
                    
                }
                
                b[peakindex] = (b[peakindex] + b[peakindex + 1])/2;
                
            }
            
        }
        
    }
    
}


// Find the peak interval
int findPeakInterval(unsigned char * b, unsigned int * string, unsigned int M)
{
    int i,j;
    
    unsigned int count_interval[M];
    unsigned int peak_interval = 0;
    int peakIndex = 0;
    
    for (i = 0; i < M; i ++) {
        
        count_interval[i] = 0;
        
    }
    
    for (i = 0; i < M; i ++) {
        
        for (j = b[i]; j < b[i + 1]; j ++) {
            
            count_interval[i] += string[j];
            
        }
        
    }
    
    for (i = 0; i < M; i ++) {
        
        if (count_interval[i] > peak_interval) {
            
            peak_interval = count_interval[i];
            peakIndex = i;
            
        }
        
    }
    
    return peakIndex;
    
}
// Calculate mean square error and report the PSNR
double calculateMse(unsigned int M, unsigned char * x, unsigned char * b, unsigned int * string, double * pdf)
{
    int i,j;
    
    double mse_error[SYMBOL_NUM];
    double sum_mse = 0.0;
    long long sum_symbols = 0;
    double psnr;
    
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        sum_symbols += string[i];
        
    }
    
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        for (j = 0; j < M; j ++) {
            
            if (i >= b[j] && i < b[j + 1]) {
                
                mse_error[i] = pow((x[j] - i), 2);
                
            }
            
        }
        
        sum_mse += mse_error[i]*string[i];
        
    }
    
    psnr = 10*log10((pow(SYMBOL_NUM-1, 2))/(sum_mse/sum_symbols));
    
    printf("The PSNR is %f\n", psnr);
    
    printf("MSE IS %f\n",sum_mse/sum_symbols);
    
    return sum_mse/sum_symbols;
    
}

// Find the corresponding quantized value for a given symbol
unsigned char findQuantizedValue(unsigned char symbol, unsigned char * b, unsigned char * x, int M)
{
    int i;
    
    for (i = 0; i < M; i++) {
        
        if (symbol >= b[i] && symbol < b[i + 1]) {
            
            symbol = x[i];
            break;
            
        }
        
    }
    
    return symbol;
    
}

// Calculate PSNR obtained from quantization and theoretical PSNR for a given image file
void calculatePSNRforImage(unsigned char * b, unsigned char * x, int M, FILE * fp)
{
    int i,j;
    
    unsigned int string[SYMBOL_NUM];
    double pdf[SYMBOL_NUM];
    double mse_error[SYMBOL_NUM];
    double sum_mse = 0.0;
    long long sum_symbols = 0;
    double psnr;
    double psnr_theo;
    double entropy = 0.0;
    
    readFileIntoSymbol(fp, string);
    calculatePDF(string, pdf);
    
    // calculate quantization PSNR
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        sum_symbols += string[i];
        
    }
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        for (j = 0; j < M; j ++) {
            
            if (i >= b[j] && i < b[j + 1]) {
                
                mse_error[i] = pow((x[j] - i), 2);
                
            }
            
        }
        
        sum_mse += mse_error[i]*string[i];
        
    }
    psnr = 10*log10(pow((SYMBOL_NUM-1), 2)/(sum_mse/sum_symbols));
    printf("The quantized image has PSNR = %f\n",psnr);
    
    // calculate theoretical PSNR and avergae code length
    
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        if (pdf[i] != 0.0) {
            
            entropy += pdf[i] * log2(1.0/pdf[i]);
            
        }
        
    }
    
}

// print the histogram information to a file
void printHistogramToFile(unsigned int * string)
{
    int i;
    
    for (i = 0; i < SYMBOL_NUM; i ++) {
        
        //
        printf("Symbol %d with %d times\n", (char)i, string[i]);
        
    }
    
}
