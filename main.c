/*
 * File:   main.c
 * Author: Rafael Greca
 *
 * Created on 06 de Fevereiro de 2018, 02:33
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////// GLOBAL VARIABLES ///////////////////////

unsigned char indexx[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"};

/////////////////////// FUNCTIONS ///////////////////////

unsigned char reverse32(char ch){

    if (ch >= 'A' && ch <= 'Z'){
            ch -= 'A';
    }

    if (ch >= '2' && ch <= '7') {
            ch = ch - '2' + 26;
    }

    return ch;
}

//encrypts the string
unsigned char encodeB32(unsigned char text[], unsigned char cript[],int size){
    //5 input bits == 8 b32 bits output
    //blocks = determinates how many even sets the input has
    //lefbits = how many bits left
    int blocks, var1, var2, leftbits;

    blocks = (size / 5) * 5;
    leftbits = (size % 5);

    for(var1=0,var2=0; var1<blocks; var1+=5, var2+=8){
        cript[var2]= indexx[text[var1] >> 3];
        cript[var2+1]= indexx[((text[var1] & 0x07) << 2) + (text[var1+1] >> 6)];
        cript[var2+2]= indexx[(text[var1+1] & 0x3E) >> 1];
        cript[var2+3]= indexx[((text[var1+1] & 0x01) << 4) + (text[var1+2] >> 4)];
        cript[var2+4]= indexx[((text[var1+2] & 0x0F) << 1) + (text[var1+3] >> 7)] ;
        cript[var2+5]= indexx[((text[var1+3] & 0x7C) >> 2)];
        cript[var2+6]= indexx[((text[var1+3] & 0x03) << 3) + (text[var1+4]  >> 5)];
        cript[var2+7]= indexx[text[var1+4] & 0x1f];

    }

    if(leftbits == 1){
        cript[var2]= indexx[text[var1] >> 3];
        cript[var2+1]= indexx[((text[var1] & 0x07) << 2)];
        cript[var2+2]= '=';
        cript[var2+3]= '=';
        cript[var2+4]= '=';
        cript[var2+5]= '=';
        cript[var2+6]= '=';
        cript[var2+7]= '=';

        var2 += 8;

    }else{
        if(leftbits == 2){
            cript[var2]= indexx[text[var1] >> 3];
            cript[var2+1]= indexx[((text[var1] & 0x07) << 2) + (text[var1+1] >> 6)];
            cript[var2+2]= indexx[(text[var1+1] & 0x3E) >> 1];
            cript[var2+3]= indexx[(text[var1+1] & 0x01) << 4];
            cript[var2+4]= '=';
            cript[var2+5]= '=';
            cript[var2+6]= '=';
            cript[var2+7]= '=';

            var2 += 8;

        }else{
            if(leftbits == 3){
                cript[var2]= indexx[text[var1] >> 3];
                cript[var2+1]= indexx[((text[var1] & 0x07) << 2) + (text[var1+1] >> 6)];
                cript[var2+2]= indexx[(text[var1+1] & 0x3E) >> 1];
                cript[var2+3]= indexx[((text[var1+1] & 0x01) << 4) + (text[var1+2] >> 4)];
                cript[var2+4]= indexx[((text[var1+2] & 0x0F) << 1)];
                cript[var2+5]= '=';
                cript[var2+6]= '=';
                cript[var2+7]= '=';

                var2 += 8;

            }else{
                if(leftbits == 4){
                    cript[var2]= indexx[text[var1] >> 3];
                    cript[var2+1]= indexx[((text[var1] & 0x07) << 2) + (text[var1+1] >> 6)];
                    cript[var2+2]= indexx[(text[var1+1] & 0x3E) >> 1];
                    cript[var2+3]= indexx[((text[var1+1] & 0x01) << 4) + (text[var1+2] >> 4)];
                    cript[var2+4]= indexx[((text[var1+2] & 0x0F) << 1) + (text[var1+3] >> 7)] ;
                    cript[var2+5]= indexx[((text[var1+3] & 0x7C) >> 2)];
                    cript[var2+6]= indexx[((text[var1+3] & 0x03) << 3)];
                    cript[var2+7]= '=';

                    var2 += 8;
                }
            }
        }
    }

    cript[var2] = '\0';
    return (var2);

}

//decrypts the string
unsigned char decodeB32(unsigned char textcript[], unsigned char uncript[],int sizecript){
    //8 b32 input bits == 5 bits output
    //blocks = determinates how many even sets the input has
    //lefbits = how many bits left

    int var1, var2, blocks, leftbits;

    fflush(stdin);

    while(textcript[sizecript - 1] == '='){
        sizecript--;
    }

    blocks = (sizecript / 8) * 8;
    leftbits = (sizecript % 8);

    for(var1=0, var2=0; var2 < blocks; var1 += 5, var2+= 8){
        uncript[var1]= (reverse32(textcript[var2]) << 3) | (reverse32((textcript[var2+1]) & 0x1C) >> 2);
        uncript[var1+1]= (reverse32(textcript[var2+1]) << 6) | (reverse32(textcript[var2+2]) << 1) + (reverse32(textcript[var2+3]) >> 4);
        uncript[var1+2]= (reverse32(textcript[var2+3]) << 4) | (reverse32(textcript[var2+4]) >> 1);
        uncript[var1+3]= (reverse32(textcript[var2+4]) << 7) | (reverse32(textcript[var2+5]) << 2) + (reverse32(textcript[var2+6]) >> 3);
        uncript[var1+4]= (reverse32(textcript[var2+6]) << 5) | (reverse32(textcript[var2+7]));
    }


    if(leftbits == 2){
        uncript[var1]= (reverse32(textcript[var2]) << 3) | (reverse32((textcript[var2+1]) & 0x1C) >> 2);

        var1 += 1;
    }else{
        if(leftbits == 4){
            uncript[var1]= (reverse32(textcript[var2]) << 3) | (reverse32((textcript[var2+1]) & 0x1C) >> 2);
            uncript[var1+1]= (reverse32(textcript[var2+1]) << 6) | (reverse32(textcript[var2+2]) << 1) + (reverse32(textcript[var2+3]) >> 4);

            var1 += 2;
        }else{
            if(leftbits == 5){
                uncript[var1]= (reverse32(textcript[var2]) << 3) | (reverse32((textcript[var2+1]) & 0x1C) >> 2);
                uncript[var1+1]= (reverse32(textcript[var2+1]) << 6) | (reverse32(textcript[var2+2]) << 1) + (reverse32(textcript[var2+3]) >> 4);
                uncript[var1+2]= (reverse32(textcript[var2+3]) << 4) | (reverse32(textcript[var2+4]) >> 1);

                var1 += 3;
            }else{
                if(leftbits == 7){
                    uncript[var1]= (reverse32(textcript[var2]) << 3) | (reverse32((textcript[var2+1]) & 0x1C) >> 2);
                    uncript[var1+1]= (reverse32(textcript[var2+1]) << 6) | (reverse32(textcript[var2+2]) << 1) + (reverse32(textcript[var2+3]) >> 4);
                    uncript[var1+2]= (reverse32(textcript[var2+3]) << 4) | (reverse32(textcript[var2+4]) >> 1);
                    uncript[var1+3]= (reverse32(textcript[var2+4]) << 7) | (reverse32(textcript[var2+5]) << 2) + (reverse32(textcript[var2+6]) >> 3);

                    var1 += 4;
                }
            }
        }
    }

    uncript[var1] = '\0';
    return (var1);
}

int main()
{
    int op;

    menu();
    printf("\n");
    printf("Option: ");
    scanf("%d",&op);

    if(op == 1){
        ENCODEBASE32();
    }else{
        if(op == 2){
            DECODEBASE32();
        }else{
            if(op == 3){
                exit(0);
            }
        }
    }

    return 0;
}

//print the menu
void menu(){

printf("****************************************************************************************\n");
printf("****************************************************************************************\n");
printf("**                                                                                    **\n");
printf("**                                                                                    **\n");
printf("**                                                                                    **\n");
printf("**                       What operation do you want to do?                            **\n");
printf("**                                                                                    **\n");
printf("**                        1- Encode B32                                               **\n");
printf("**                        2- Decode B32                                               **\n");
printf("**                        3- Exit                                                     **\n");
printf("**                                                                                    **\n");
printf("**                                                                                    **\n");
printf("****************************************************************************************\n");
printf("****************************************************************************************\n");

}

void ENCODEBASE32(){

    //text = saves the decrypted variable entered by the user
    //cript = save the result of the string enconding
    //size = size of the text variable
    unsigned char text[1024];
    unsigned char cript[1024];
    int size;

    fflush(stdin);
    printf("\n\nEnter the phrase you want to encrypt: ");
    scanf(" %[^\n]s",text);

    size = strlen(text);

    encodeB32(text,cript,size);
    printf("\n\n");
    printf("The encrypted phrase is: %s\n\n\n",cript);

}

void DECODEBASE32(){
    //textcript = saves the encrypted variable entered by the user
    //uncript = save the result of the string decoding
    //sizecript = size of the textcript variable

    unsigned char textcript[1024];
    unsigned char uncript[1024];
    int sizecript;

    fflush(stdin);
    printf("\n\nEnter the phrase you want to decrypt: ");
    scanf(" %[^\n]s",textcript);

    sizecript = strlen(textcript);

    decodeB32(textcript,uncript,sizecript);
    printf("\n\n");
    printf("The decrypted phrase is: %s\n\n",uncript);

}
