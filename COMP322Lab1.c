#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE 1024

int binToDec(char *sevenBits) {
   
   // convert sevenBits to decimal value
   
   int i, j = 0, decValue = 0;   
   
   for (i = 6; i >= 0; i--) {
      
      if (sevenBits[i] == '1') {
   
         decValue = decValue + intPow(2,j);  
      }
      
      j++;
   } 
   
   return decValue;
}

int intPow(int base, int exp) {

   // integer exponential function
   
   int i, result = 1;
   
   for (i = 0; i < exp; i++) {
   
      result = result * base;  
   }
   
   return result;
}

int parity(char *eightBits) {

   // error check by counting even/odd number of ones

   int i, count = 0;
   
   for (i = 0; i < 8; i++) {
   
      if (eightBits[i] == '1') {
      
         count++;
      }   
   }  
   
   return count % 2;
}

void printVals(char * eightBits) {

   char sevenBits[8]; // ASCII Value
   int k,decValue;
   
   // Non-printable characters
   char * aliasList[33] = {
      "NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL","BS","HT","LF","VT",
      "FF","CR","SO","SI","DLE","DC1","DC2","DC3","DC4","NAK","SYN","ETB","CAN",
      "EM","SUB","ESC","FS","GS","RS","US","SPACE"
   };
   
   // strip off MSD and convert to ASCII           
   for (k = 1; k < 8; k++) {
               
      sevenBits[k - 1] = eightBits[k];      
   }
           
   decValue = binToDec(sevenBits);
            
   // print to stdout binary value
   printf("%s ",eightBits);
            
   // print printable ASCII characters
   if (decValue > 32 && decValue < 127) {
      printf("%c\t",decValue);
   }
            
   // print DEL char
   else if (decValue == 127) {
      printf("DEL\t");
   }
            
   // print all other non-printable chars            
   else {
      printf("%s\t",aliasList[decValue]);         
   }
            
   // print decimal value
   printf("%d\t",decValue);
            
   // print parity and t.error
   if (parity(eightBits) == 0) {            
      printf("EVEN\tFALSE\n");
   }
            
   else {            
      printf("ODD\tTRUE\n");
   }
}

int main(int argc, char *argv[], char **envp) {

   // Reads file and converts to char array buffer
   // Every 8 chars that are either 1 or 0 are converted to decimal
   // ASCII value of the first seven digits
   // Determine even parity of 8 char array
      
   int fileDescriptor = 0;
   int i,j = 0,k;
   char buffer[BUFFERSIZE];
   char byteValue[9]; // ASCII Value with parity bit
   
   // open non-stdin file      
   if (argc > 1) {
   
      fileDescriptor = open(argv[1], O_RDONLY);      
   }
   
   printf("Original ASCII\tDecimal\tParity\tT.Error\n");
   printf("-------- -----\t-------\t------\t-------\n");
   
   // reads file until EOF reached
   while (read(fileDescriptor, buffer, BUFFERSIZE) > 0) {
      
      // parse each buffer to read 8 bit binary values   
      for (i = 0; i < BUFFERSIZE; i++) {
         
         if ((buffer[i] == '0' || buffer[i] == '1') && j < 8) {
         
            byteValue[j] = buffer[i]; 
            j++;         
         }
         
         // 8 bit binary value read
         else if ((buffer[i] == '0' || buffer[i] == '1') && j == 8) {
           
            j = 0;
            byteValue[8] = '\0';            
            printVals(byteValue);
            byteValue[j] = buffer[i]; 
            j++;                       
         }         
      }      
   }
   
   // incomplete 8 bit value at EOF gets padded and also printed
   if (j > 0 && j < 8) {
     
      for (k = j; k < 8; k++) {
      
         byteValue[k] = '0';
      }
      
      byteValue[8] = '\0';
      printVals(byteValue);
   }
   
   return 0;
}


