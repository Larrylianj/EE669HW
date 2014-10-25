{\rtf1\ansi\ansicpg1252\cocoartf1265\cocoasubrtf190
{\fonttbl\f0\fswiss\fcharset0 Helvetica;\f1\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red100\green56\blue32;\red196\green26\blue22;}
\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural

\f0\fs24 \cf0 #EE669 Homework Assignment #2\
#Feb 28, 2014\
#Name: Siyang Liang\
#ID: 3495-7475-06\
#E-mail: siyangli@usc.edu\
#compiled on OS X 10.9.1with gcc\
\
prob 1:\
	PartB(1):\
	
\f1\fs22 \CocoaLigature0 //main function is 
\f0\fs24 \CocoaLigature1 qm_coder.c  \
	
\f1\fs22 \CocoaLigature0 //
\f0\fs24 \CocoaLigature1 should include following head files and corresponding .c file with the same name\
	
\f1\fs22 \cf2 \CocoaLigature0 #include \cf3 <stdio.h>\
\cf2 	#include \cf3 <stdlib.h>\cf2 \
	#include \cf3 "bitio.h"\cf2 \
	#include \cf3 "errhand.h"\
	\cf0 //There should be 3 parameters! The format is\
	//program_name input_file transition_table output_file\
\
	
\f0\fs24 \CocoaLigature1 PartB(2):\
	
\f1\fs22 \CocoaLigature0 //main function is 
\f0\fs24 \CocoaLigature1 cabac.cpp  \
	
\f1\fs22 \CocoaLigature0 //
\f0\fs24 \CocoaLigature1 should include following head files and corresponding .c file with the same name\
	
\f1\fs22 \cf2 \CocoaLigature0 #include \cf3 <stdio.h>\
\cf2 	#include \cf3 <stdlib.h>\cf2 \
	#include \cf3 "QMCODER.H"\
	\cf0 //format: program_name input_file output_file\
\
\

\f0\fs24 \CocoaLigature1 prob 2:\
	PartA:\
	
\f1\fs22 \CocoaLigature0 //main function is 
\f0\fs24 \CocoaLigature1 quan.c\
	
\f1\fs22 \CocoaLigature0 //
\f0\fs24 \CocoaLigature1 should include following head files \
	
\f1\fs22 \cf2 \CocoaLigature0 #include \cf3 <stdio.h>\
\cf2 	#include \cf3 <stdlib.h>\cf2 \
	#include \cf3 <math.h>\cf2 \
	#include \cf3 "const_def_VQ.h"\
	\cf0 //There should be 6 parameters! The format is\
	//program_name training_set1 training_set2 training_set3 test_set1 output_image\
\
	
\f0\fs24 \CocoaLigature1 PartB(Image blocking):\
	
\f1\fs22 \CocoaLigature0 //main function is 
\f0\fs24 \CocoaLigature1 imageblocking.c\
	
\f1\fs22 \CocoaLigature0 //
\f0\fs24 \CocoaLigature1 should include following head files and corresponding .c file with the same name\
	
\f1\fs22 \cf2 \CocoaLigature0 #include \cf3 <stdio.h>\cf2 \
	#include \cf3 <sidlib.h>\cf2 \
	#include \cf3 "Matrix_ImageBlocking.h"\
	\cf0 //usage: program_name training_data output_vector BytePerPixel Legnth Width block_size\cf3 \
\
	\cf0 \
prob3:\
	PartA:\
	//main function is 
\f0\fs24 \CocoaLigature1 dactQuantization.c  \
	
\f1\fs22 \CocoaLigature0 //
\f0\fs24 \CocoaLigature1 should include following head files and corresponding .c file with the same name\
	
\f1\fs22 \cf2 \CocoaLigature0 #include \cf3 <stdio.h>\cf2 \
	#include \cf3 <stdlib.h>\cf2 	\
	#include \cf3 <math.h>\cf2 \
	#include \cf3 "const_def_DCT_Quantization.h"\cf2 \
	#include \cf3 "Matrix_ImageBlocking.h"\
	\cf0 //usage: program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 256]\
\cf3 \
	\cf0 PartC:\
	//main function is 
\f0\fs24 \CocoaLigature1 dactQuantization.c  \
	
\f1\fs22 \CocoaLigature0 //
\f0\fs24 \CocoaLigature1 should include following head files and corresponding .c file with the same name\
	
\f1\fs22 \cf2 \CocoaLigature0 #include \cf3 <stdio.h>\cf2 \
	#include \cf3 <stdlib.h>\cf2 \
	#include \cf3 <math.h>\cf2 \
	#include \cf3 "const_def_rba.h"\cf2 \
	#include \cf3 "Matrix_Operation_rba.h"\
	\cf0 //usage: program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 256]\
\
}