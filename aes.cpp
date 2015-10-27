#include <iostream>
#include <stdlib.h>   
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cassert>
#include "aes.h"  //dependecy
using namespace std;
/*
 *  Task 1 : Read the message & key from the file \
 *           Store it in array.
 *
 *  Task 2 : Convert ASCII array to hexadecimal 
 *
 *  Task 3 : Generate keys for all rounds 
 *
 *  Task 4 : Perform encryption
 *           - ByteSub Transformation
 *           - Shift Rows
 *           - Mix Cloumns 
 *           - Add Round Key
 *
 * Task 5 : Perform decryption 
 *          - Inverse ByteSub Transformation
 *          - Inverse Shift Rows
 *          - Inverse Mix Cloumns 
 *          - Add Round Key
 *
 *  Author : Tushar Sharma <tushar.sharma1729@gmail.com> 
 *  Last Modified  : Oct 26, 2015 
 *  Status : OK
*/

void error_dsp(string msg) 
{
    cerr<<msg<<endl;
    exit(-1); 
}

void state_input(char foo[][4], char *filename)
{
    ifstream file(filename);
    string bar;

    assert(file.is_open());
    getline(file, bar);


    int index = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (index < bar.size()) {
                foo[j][i] = bar[index++];
            }
        }
    }
    file.close();
}

void dsp(char foo[][4])
{
    for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
        if (foo[i][j] == '\0')
        cout<<" ";
        cout<<foo[i][j]<<" | ";
    }
    cout<<endl;
    }
}

void dsp(char foo[][4][3])
{
    for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 3; k++) {
        if (foo[i][j][k] == '\0')
            cout<<" ";
        cout<<foo[i][j][k];
        }
        cout<<" |  ";
    }
    cout<<endl;
    }
}

void ascii_2_hex(char foo[][4], char bar[][4][3])
{
    int temp, index;
    char arr[2];

    memset(arr, '\0', sizeof(arr));

    for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
            if (foo[i][j] != '\0') {
            temp = (int)foo[i][j];
            //cout<<"\n check "<<temp<<endl;
            index = 0;
            while (temp > 0) {
            arr[index++] = base16[temp % 16];
            temp = temp / 16;
            }

            for (int k = 0; k < 2; k++) {
            bar[i][j][k] = arr[1 - k];
            }
            }
    }
    }
}

int toCheck(int temp)
{
    if (temp >= 65 && temp <= 90) {
    return (temp - 55);
    }
    else if (temp >= 97  && temp <= 122){
    return (temp - 87);
    } else {
    return (temp - 48);
    }
}

char toHex(int temp)
{
    char ch;
    while (temp > 0) {
    ch = base16[temp % 16];
    temp = temp / 16;
    }
    return ch;
}

void keyGen (char foo[][4][3], char bar[][4][3])
{
    //char temp[4][4][3];
    char temp[4][1][3];
    int offset = 0;

    memset(temp, '\0', sizeof(temp));

    for (int count = 0; count < 10; count++) {

        //copy previous key to next key
        for (int i = offset; i < (4 + offset); i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 2; k++) {
            if (offset == 0)
                bar[i][j][k] = foo[i][j][k];
            else
                bar[i][j][k] = bar[i - 4][j][k];
                }
        }
        }
        cerr<<"\n********Starting key Generation**********\n\n";
        //copy the fourth column of original key into temp
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                //bar[i][j][k] = foo[i][j][k]; 
        if (bar[i + offset][3][k] != '\0')
                    temp[i][0][k] = bar[i + offset][3][k];
        }
        }

        //Putting 1st row of temp matrix to last
        for (int i = 0; i < 3; i++) {
            for (int k = 0; k < 2; k++) {
                temp[i][0][k]     ^= temp[i + 1][0][k];
                temp[i + 1][0][k] ^= temp[i][0][k];
                temp[i][0][k]     ^= temp[i + 1][0][k];
        }
        }

        cerr<<"\nPutting 1st row of temp matrix to last\n";
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                cerr<<temp[i][0][k];
            }
            cerr<<endl;
        }

        //Applying sbox to column of temp matrix
        int arr[2] = {0};
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
            if (temp[i][0][k] != '\0') {
                    arr[k] = toCheck((int)temp[i][0][k]);
                }
        }


        for (int k = 0; k < 2; k++) {
                temp[i][0][k] = sbox[arr[0]][arr[1]][k];
        }

        arr[0] = 0;
        arr[1] = 0;
        }

        cerr<<"\nApplying sbox to column of temp matrix\n";
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                cerr<<temp[i][0][k];
            }
            cerr<<endl;
        }


        //Xor the last column of temp matrix with round constant
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                 if (temp[i][0][k] != '\0') {
                //cout<<"\ncheck\n";
                //cout<<(int)toCheck((int)bar[i][3][k])<<" "<<toCheck((int)Rcons[i][0][k]);
                    temp[i][0][k] = base16[toCheck((int)temp[i][0][k]) ^ toCheck((int)Rcons[i][count][k])];
                }
            }
        }

        cerr<<"\nXor the last column of temp matrix with round constant\n";
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                cerr<<temp[i][0][k];
            }
            cerr<<endl;
        }



        //for first column, xor first column with temp matrix 
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                bar[i + offset][0][k] = base16[toCheck((int)temp[i][0][k]) ^ toCheck((int)bar[i + offset][0][k])];
        }
        }

        cerr<<"\nFirst column is\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 2; k++) {
                    cerr<<bar[i][j][k];
                }
                cerr<<" ";
            }
            cerr<<endl;
        }



        //for 2nd column, xor second column with first column
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                bar[i + offset][1][k] = base16[toCheck((int)bar[i + offset][0][k]) ^ toCheck((int)bar[i + offset][1][k])];
        }
        }

        cerr<<"\nSecond column is\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 2; k++) {
                    cerr<<bar[i][j][k];
                }
                cerr<<" ";
            }
            cerr<<endl;
        }



        //for 3rd column, xor 3rd column with 2nd column
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                bar[i + offset][2][k] = base16[toCheck((int)bar[i + offset][1][k]) ^ toCheck((int)bar[i + offset][2][k])];
        }
        }

        cerr<<"\nThird column is\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 2; k++) {
                    cerr<<bar[i][j][k];
                }
                cerr<<" ";
            }
            cerr<<endl;
        }



        //for 4th column, xor 4th column with 3rd column
        for (int i = 0; i < 4; i++) {
            for (int k = 0; k < 2; k++) {
                bar[i + offset][3][k] = base16[toCheck((int)bar[i + offset][2][k]) ^ toCheck((int)bar[i + offset][3][k])];
        }
        }

        cerr<<"\nFourth column is\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 2; k++) {
                    cerr<<bar[i][j][k];
                }
                cerr<<" ";
            }
            cerr<<endl;
        }



        offset += 4; //point to next key
    }

    cout<<endl;
    int key_count = 1;
    for (int i = 0; i < 40; i++) {
        if (i % 4 == 0)
            cout<<"\nkey No "<<key_count++<<endl;

        for (int j = 0; j < 4; j++) {
                     for (int k = 0; k < 2; k++) {
                cout<<bar[i][j][k];
        }
        cout<<" | ";
    }

    if (i % 4 == 3)
        cout<<"\n\n";
    else
        cout<<"\n";
    }
}

void aes(char* m, char *k) 
{
    char state_msg[4][4];  //message in state form
    char state_key[4][4];  //key in state form
    char hex_msg[4][4][3]; //message in hex
    char hex_key[4][4][3]; //key in hex
    char current_key[4][4][3];
    char keys[40][4][3];  //stores next ten keys
    int offset = 0;

    //Initialization
    memset(state_msg, '\0', sizeof(state_msg)); //using null as padding                          
    /* Space is used as padding instead of '\0' to allow key less than 128 bits     */
    memset(state_key, ' ', sizeof(state_key));
    memset(hex_msg, '\0', sizeof(hex_msg));
    memset(hex_key, ' ', sizeof(hex_key));
    memset(keys, ' ', sizeof(keys));
    memset(current_key, ' ', sizeof(current_key));

    state_input(state_msg, m);
    state_input(state_key, k);

    cout<<"\n128 bit Message is\n";
    dsp(state_msg);
    cout<<"\n128 bit Key is\n";
    dsp(state_key);

    ascii_2_hex(state_msg, hex_msg);
    cout<<"\nMessage in hex\n";
    dsp(hex_msg);

    ascii_2_hex(state_key, hex_key);
    cout<<"\nkey in hex\n";
    dsp(hex_key);

    cout<<"\nKey Generation";
    keyGen(hex_key, keys);

    /******Encryption********/
    cout<<"\nEncryption\n";
    ARK (hex_msg, hex_key);
    cout<<"\nInitial Round -ARK\n";
    dsp(hex_msg);

    for (int round = 0; round < 9; round++) {
        cout<<"\n Round "<<round + 1<<endl;
        ByteSub(hex_msg, 0);  //0 is for encryption mode
        cout<<"\nByte Substitution\n";
        dsp(hex_msg);

        ShiftRows(hex_msg, 0);
        cout<<"\nShift Rows\n";
        dsp(hex_msg);

        MixColumns(hex_msg);
        cout<<"\nMix Columns\n";
        dsp(hex_msg);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 2; k++) {
            current_key[i][j][k] = keys[i + offset][j][k];
        }
        }
    }

    ARK(hex_msg, current_key);
    cout<<"\nARK \n";
    dsp(hex_msg);

    offset += 4;
    }

    cout<<"\n Round  10 \n";
    ByteSub(hex_msg, 0);
    cout<<"\nByte Substitution\n";
    dsp(hex_msg);

    ShiftRows(hex_msg, 0);
    cout<<"\nShift Rows\n";
    dsp(hex_msg);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 2; k++) {
        current_key[i][j][k] = keys[i + offset][j][k];
            }   
    }   
    }   

    ARK(hex_msg, current_key);
    cout<<"\nARK \n";
    dsp(hex_msg);

    cout<<"\nEncrypted Message in Hex\n\n";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 2; k++) cout<<hex_msg[i][j][k];
            cout<<" ";
        }
    }
    cout<<"\n"; 

    /******Decryption********/
    cout<<"\n\nDecryption\n";
    offset = 36;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 2; k++) {
	        current_key[i][j][k] = keys[i + offset][j][k];
	    }
	}
    }

    cout<<"\nInitial Round\n";
    ARK(hex_msg, current_key);
    cout<<"\nARK\n";
    dsp(hex_msg);

    ShiftRows(hex_msg, 1);  //1 is for decryption mode
    cout<<"\nInverse Shift Rows\n"; 
    dsp(hex_msg);

    ByteSub(hex_msg, 1);  //1 is for decryption mode
    cout<<"\nInverse Byte Substitution\n";
    dsp(hex_msg);
	    
    for (int round = 0; round < 9; round++) {
        offset -= 4;
	for (int i = 0; i < 4; i++) {
	    for (int j = 0; j < 4; j++) {
	        for (int k = 0; k < 3; k++) {
	            current_key[i][j][k] = keys[i + offset][j][k];
	        }
	    }
        }
	 
	cout<<"\nRound No "<<round + 1<<endl;
	ARK(hex_msg, current_key);
	cout<<"\nARK\n";
	dsp(hex_msg);

	InvMixColumns(hex_msg);
	cout<<"\nInverse Mix Columns\n";
	dsp(hex_msg);

	ShiftRows(hex_msg, 1);  //1 is for decryption mode
	cout<<"\nInverse Shift Rows\n";
	dsp(hex_msg);

	ByteSub(hex_msg, 1);  //1 is for decryption mode
	cout<<"\nInverse Byte Substitution\n";
	dsp(hex_msg);
    }

    ARK(hex_msg, hex_key);
    cout<<"\nRound 10 - ARK\n";
    dsp(hex_msg);

    cout<<"\nDecrypted Message\n";
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
             int factor = 16;
             int val = 0;
             for (int k = 0; k < 2; k++) {
                 if (hex_msg[i][j][k] != '\0') {
                     val += (char)toCheck(hex_msg[i][j][k]) * factor;
                 }
                 factor = factor / 16;
             }
             if (val == 0) cout<<" ";
             cout<<(char)val<<" | ";
        }
	cout<<endl;
    }

    cout<<endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
             int factor = 16;
             int val = 0;
             for (int k = 0; k < 2; k++) {
                 if (hex_msg[j][i][k] != '\0') {
                     val += (char)toCheck(hex_msg[j][i][k]) * factor;
                 }
                 factor = factor / 16;
             }
             cout<<(char)val;
        }
    }

}

int main(int argc, char **argv) 
{ 
    freopen( "output.txt", "w", stdout );
    freopen( "error.txt", "w", stderr );
    
    if (argc != 3) {
        error_dsp("Arguments Dont Match!");
    }

   
    aes(argv[1], argv[2]); 

    return 0;
}

void ARK(char foo[][4][3], char bar[][4][3])
{
    int temp1, temp2;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
	    int factor = 16;
	    for (int k = 0; k < 2; k++) {
                
	     
	        temp1 = (int)foo[i][j][k];
	        
                temp2 = (int)bar[i][j][k];
		
 
                if (temp1 == 0) {
		    temp1 = temp2;
                    temp1 = toCheck(temp1) * factor;

		}

	        else if ((temp1 != 0) && (temp2 !=0)){
		    temp1 = toCheck(temp1) * factor;
		    temp2 = toCheck(temp2) * factor;
		    temp1 = temp1 ^ temp2;
		}
                //cout<<"\n"<<temp1<<"\n";
          	factor = factor / 10;
 
                foo[i][j][k] = toHex(temp1);
                //cout<<"\n"<<foo[i][j][k]<<"\n";
               /* else if (foo[i][j][k] == '\0' && bar[i][j][k] !='\0') {
                    foo[i][j][k] = bar[i][j][k]; 
                }*/
	    }
	}
    }
}

void ByteSub (char foo[][4][3], int mode) 
{
    int arr[2] = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 2; k++) {
                if (foo[i][j][k] != '\0') {
                    arr[k] = toCheck((int)foo[i][j][k]);
		}
	    }

	    for (int k = 0; k < 2; k++) {
                if (mode == 0) 
	            foo[i][j][k] = sbox[arr[0]][arr[1]][k];  //for encryption
                else 
                    foo[i][j][k] = invsbox[arr[0]][arr[1]][k];  //for decryption
            }
	    arr[0] = 0;
	    arr[1] = 0;
	}
    }
}


void ShiftRows(char foo[][4][3], int mode)
{
    for (int i = 0; i < 4; i++) {
        int count = i;
	while (count-- > 0) {
            if (mode == 0) {  //for encryption
                for (int j = 0; j < 3 ; j++) {
                    for (int k = 0; k < 2; k++)  {
                       foo[i][j][k]     ^= foo[i][j + 1][k]; //swapping
		       foo[i][j + 1][k] ^= foo[i][j][k];
		       foo[i][j][k]     ^= foo[i][j + 1][k];
		    } 
                }  
            } else {  //for decryption
                for (int j = 3; j >= 1 ; j--) {
                   for (int k = 0; k < 2; k++)  {
                       foo[i][j][k]     ^= foo[i][j - 1][k]; //swapping
		       foo[i][j - 1][k] ^= foo[i][j][k];
		       foo[i][j][k]     ^= foo[i][j - 1][k];
		   }
                }
            }
        }  //end while
    }  //end for
}

void MixColumns (char foo[][4][3]) 
{
    int i, j, k, bar = 0; 
    int word[4][4][8] = {0};
    int r0[8] = {0}, r1[8] = {0}, r2[8] = {0}, r3[8] = {0};
    int val, temp;
    int word2[8] = {0, 0, 0, 1, 1, 0, 1, 1}; 
    int word3[8] = {0}, word4[8] = {0}, word5[8] = {0}, word6[8] = {0};
    int mat[4][4] = { {2, 1, 1, 3}, 
                      {3, 2, 1, 1}, 
                      {1, 3, 2, 1}, 
                      {1, 1, 3, 2} };
    int sum1, sum2, sum3, sum4;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            bar = 0;
            for (k = 0; k < 2; k++){
                if(foo[i][j][k]!='\0'){
                    val = toCheck((int)foo[i][j][k]);
                }
                else 
                    val = 0;
		 //cout<<val<<" ";
                 bar = bar * 16 + (val ); 
            }
	    //cout<<endl;
            k = 7; 
            while (bar > 0) {
                word[i][j][k--] = bar % 2; 
                bar = bar / 2;        
            }
         }
    } //end for
    cerr<<"\nMix column..\n";  
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 8; k++) {
                cerr<<word[i][j][k];
	    }
	    cerr<<" ";
	}
	cerr<<endl;
    }
   
  
    for (i = 0; i < 4; i++) { 
        for(j = 0; j < 4; j++) {
            for (k = 0; k < 8; k++) {
                word3[k] = word[j][i][k];
                word4[k] = word3[k];
                word5[k] = word4[k];
                word6[k] = word5[k];
            }
            if (mat[j][0] == 2) 
                f2 (word3);

            if (mat[j][0] == 3)
                f3(word3);
 
            if (mat[j][1] == 2) 
                f2 (word4);

            if (mat[j][1] == 3)
                f3(word4);
 
            if (mat[j][2] == 2) 
                f2 (word5);

            if (mat[j][2] == 3)
                f3(word5);
 
            if (mat[j][3] == 2) 
                f2 (word6);

            if (mat[j][3] == 3)
                f3(word6);

            for (k = 0; k < 8; k++) {
                r0[k] ^= word3[k]; 
                r1[k] ^= word4[k];
                r2[k] ^= word5[k];
                r3[k] ^= word6[k];
            }
        } 

        temp = 8;
        sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
        for (j = 0; j < 4; j++) {
            sum1 += r0[j] * temp; 
            sum2 += r1[j] * temp;
            sum3 += r2[j] * temp;
            sum4 += r3[j] * temp;

            temp /= 2;
        }

        foo[0][i][0] = base16[sum1 % 16]; 
        foo[1][i][0] = base16[sum2 % 16]; 
        foo[2][i][0] = base16[sum3 % 16]; 
        foo[3][i][0] = base16[sum4 % 16]; 
 
        temp = 8;
        sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
        for (j = 4; j < 8; j++) {
            sum1 += r0[j] * temp; 
            sum2 += r1[j] * temp;
            sum3 += r2[j] * temp;
            sum4 += r3[j] * temp;

            temp /= 2;
        }

        foo[0][i][1] = base16[sum1 % 16]; 
        foo[1][i][1] = base16[sum2 % 16]; 
        foo[2][i][1] = base16[sum3 % 16]; 
        foo[3][i][1] = base16[sum4 % 16]; 

        for (j = 0; j < 8; j++) {          //clean buffer
            r0[j] = 0; 
            r1[j] = 0; 
            r2[j] = 0; 
            r3[j] = 0;
        }
        //store values
    }
}


void InvMixColumns (char foo[][4][3]) 
{
    int i, j, k, bar = 0; 
    int word[4][4][8] = {0};
    int r0[8] = {0}, r1[8] = {0}, r2[8] = {0}, r3[8] = {0};
    int val, temp;
    int word2[8] = {0, 0, 0, 1, 1, 0, 1, 1}; 
    int word3[8] = {0}, word4[8] = {0}, word5[8] = {0}, word6[8] = {0};
    
    int mat[4][4] = { {14, 9, 13, 11}, 
                      {11, 14, 9, 13}, 
                      {13, 11, 14, 9}, 
                      {9, 13, 11, 14} 
    };

    int sum1, sum2, sum3, sum4;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            bar = 0;
            for (k = 0; k < 2; k++){
                if(foo[i][j][k]!='\0'){
                    val = toCheck((int)foo[i][j][k]);
                }
                else 
                    val = 0;
		 //cout<<val<<" ";
                 bar = bar * 16 + (val ); 
                 
            }
	    //cout<<endl;
            k = 7; 
            while (bar > 0) {
                word[i][j][k--] = bar % 2; 
                bar = bar / 2;        
            }
         }
    } //end for
    /*
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 8; k++) {
                cout<<word[i][j][k];
	    }
	    cout<<" ";
	}
	cout<<endl;
    }
    */
  
    for (i = 0; i < 4; i++) { 
        for(j = 0; j < 4; j++) {
            for (k = 0; k < 8; k++) {
                word3[k] = word[j][i][k];
                word4[k] = word3[k];
                word5[k] = word4[k];
                word6[k] = word5[k];
            }
	    //case 1
            if (mat[j][0] == 9) 
                f9(word3);

            if (mat[j][0] == 11)
                f11(word3);
 
            if (mat[j][0] == 13)
	        f13(word3);

            if (mat[j][0] == 14) 
	        f14(word3);
 
            //case 2
            if (mat[j][1] == 9) 
	        f9(word4);

            if (mat[j][1] == 11)
                f11(word4);

            if (mat[j][1] == 13) 
                f13(word4);

            if (mat[j][1] == 14)
                f14(word4);

            //case 3
            if (mat[j][2] == 9) 
                f9 (word5);

            if (mat[j][2] == 11)
                f11(word5);
 
            if (mat[j][2] == 13) 
                f13 (word5);

            if (mat[j][2] == 14)
                f14(word5);
 
            //case 4
            if (mat[j][3] == 9) 
                f9 (word6);

            if (mat[j][3] == 11)
                f11(word6);

            if (mat[j][3] == 13) 
                f13 (word6);

            if (mat[j][3] == 14)
                f14(word6);

            for (k = 0; k < 8; k++) {
                r0[k] ^= word3[k]; 
                r1[k] ^= word4[k];
                r2[k] ^= word5[k];
                r3[k] ^= word6[k];
            }
        } 

        temp = 8;
        sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
        for (j = 0; j < 4; j++) {
            sum1 += r0[j] * temp; 
            sum2 += r1[j] * temp;
            sum3 += r2[j] * temp;
            sum4 += r3[j] * temp;

            temp /= 2;
        }

        foo[0][i][0] = base16[sum1 % 16]; 
        foo[1][i][0] = base16[sum2 % 16]; 
        foo[2][i][0] = base16[sum3 % 16]; 
        foo[3][i][0] = base16[sum4 % 16]; 
 
        temp = 8;
        sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
        for (j = 4; j < 8; j++) {
            sum1 += r0[j] * temp; 
            sum2 += r1[j] * temp;
            sum3 += r2[j] * temp;
            sum4 += r3[j] * temp;

            temp /= 2;
        }

        foo[0][i][1] = base16[sum1 % 16]; 
        foo[1][i][1] = base16[sum2 % 16]; 
        foo[2][i][1] = base16[sum3 % 16]; 
        foo[3][i][1] = base16[sum4 % 16]; 

        for (j = 0; j < 8; j++) {          //clean buffer
            r0[j] = 0; 
            r1[j] = 0; 
            r2[j] = 0; 
            r3[j] = 0;
        }
        //store values
    }
}

void f2 (int *word3)
{
    int temp;
    // exclusive-ored with 1b 
    int word2[8] = {0, 0, 0, 1, 1, 0, 1, 1};
    
    //left shift 
    temp = word3[0];  //stores MSB

    for (int k = 0; k < 7; k++)
        word3[k] = word3[k + 1];

     word3[7] = 0;
     
     if (temp == 1) {
         for (int k = 0; k < 8; k++) 
             word3[k] ^= word2[k];
     }
       
}

void f3 (int *word3) 
{
    int word2[8] = {0}; 

    for (int i = 0; i < 8; i++)  
        word2[i] = word3[i]; 

    f2 (word2);

    for (int i = 0; i < 8; i++) 
        word3[i] ^= word2[i]; 

}

void f9 (int *word3)
{
    int word2[8] = {0};

    for (int i = 0; i < 8; i++)  
        word2[i] = word3[i]; 

    f2(word3); 
    f2(word3);
    f2(word3);

    for (int i = 0; i < 8; i++) {
        word3[i] ^= word2[i];
    }

}

void f11 (int *word3)
{
    int word2[8] = {0}, word1[8] = {0}; 

    for (int i = 0; i < 8; i++) {
        word2[i] = word3[i]; 
        word1[i] = word3[i];
    }

    f2(word3); 
    f2(word3);

    for (int i = 0; i < 8; i++) {
        word3[i] ^= word2[i];
    }

    f2(word3); 

    for (int i = 0; i < 8; i++) {
        word3[i] ^= word1[i];
    }
}

void f13 (int *word3)
{
    int word2[8] = {0}, word1[8] = {0};

    for (int i = 0; i < 8; i++) {
        word2[i] = word3[i]; 
	word1[i] = word3[i];
    }

    f2(word3); 

    for (int i = 0; i < 8; i++) {
        word3[i] ^= word2[i];
    }

    f2(word3);
    f2(word3);

    for (int i = 0; i < 8; i++) {
        word3[i] ^= word1[i];
    }
}

void f14 (int *word3)
{
    int word2[8] = {0}, word1[8] = {0};

    for (int i = 0; i < 8; i++) {
        word2[i] = word3[i]; 
	word1[i] = word3[i];
    }

    f2(word3); 

    for (int i = 0; i < 8; i++) {
        word3[i] ^= word2[i];
    }

    f2(word3);

    for (int i = 0; i < 8; i++) {
        word3[i] ^= word1[i];
    }

    f2(word3);
}
 
