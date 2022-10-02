#include<stdio.h>
#include<string.h>
#include <stdint.h>

typedef uint32_t word;
typedef uint8_t byte;

static const word Rcon[] = {
    0x01000000,
    0x02000000,
    0x04000000,
    0x08000000,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000,
    0x1b000000,
    0x36000000
};

static const byte S_Box[] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16   
};

static const byte Inv_S_Box[] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,  
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,  
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,  
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,  
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,  
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,  
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,  
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,  
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,  
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,  
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,  
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,  
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,  
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,  
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,  
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d   
};

static const byte Diffusion_M[4][4] = {
    0x02, 0x03, 0x01, 0x01,
    0x01, 0x02, 0x03, 0x01,
    0x01, 0x01, 0x02, 0x03,
    0x03, 0x01, 0x01, 0x02
};

static const byte Inv_Diffusion_M[4][4] = {
    0x0e, 0x0b, 0x0d, 0x09,
    0x09, 0x0e, 0x0b, 0x0d,
    0x0d, 0x09, 0x0e, 0x0b,
    0x0b, 0x0d, 0x09, 0x0e
};

void SplitWord(word value, byte b[]){
  b[3] = (value & 0x000000ff);
  b[2] = (value & 0x0000ff00) >> 8;
  b[1] = (value & 0x00ff0000) >> 16;
  b[0] = (value & 0xff000000) >> 24;
}

word MergeWord(byte b1, byte b2, byte b3, byte b4){
  return ((word)(b1)<<24) + ((word)(b2)<<16) + ((word)(b3)<<8) + (word)(b4);
}

word RotWord(word value){
  byte b[4];
  SplitWord(value, b);
  return MergeWord(b[1], b[2], b[3], b[0]);
 }


word SubWord(word value){
  byte b[4];
  SplitWord(value, b);
  for (unsigned i=0; i<4; ++i)
    b[i] = S_Box[b[i]];
  return MergeWord(b[0], b[1], b[2], b[3]);
}

void KeyExpansion(byte key[16], word w[]){
  for(unsigned i=0; i<4; ++i)
    w[i] = MergeWord(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);

  for (unsigned i = 4; i < 44; ++i) {
    word temp = w[i - 1];
  
    if (i % 4 == 0) 
      temp = SubWord(RotWord(temp)) ^ Rcon[i/4-1];
    w[i] = w[i - 4] ^ temp;
  }
}

void SubBytes(byte b[][4]){
  for (unsigned i = 0; i < 4; ++i)
    for (unsigned j = 0; j < 4; ++j)
      b[i][j] = S_Box[b[i][j]];
}

void InvSubBytes(byte b[][4]){
  for (unsigned i = 0; i < 4; ++i)
    for (unsigned j = 0; j < 4; ++j)
      b[i][j] = Inv_S_Box[b[i][j]];
}

void ShiftRows(byte b[][4]){
  byte temp[4];
  for(unsigned i = 1; i < 4; i++){
    for(unsigned j = 0; j < 4; j++)
      temp[j] = b[i][(i+j)%4];
    for(unsigned k = 0; k < 4; k++)
      b[i][k] = temp[k];
  }
}

void InvShiftRows(byte b[][4]){
  byte temp[4];
  for(unsigned i = 1; i < 4; i++){
    for(unsigned j = 0; j < 4; j++)
      temp[j] = b[i][(4+j-i)%4];
    for(unsigned k = 0; k < 4; k++)
      b[i][k] = temp[k];
  }
}

byte GF_Mul(byte a, byte b){
  byte res = 0;
  for (; b; b >>= 1) {
    if (b & 1)  res ^= a;
    a = a << 1 ^ (a & 0x80 ? 0x1b : 0);
  }
  
  return res;
}

void Matrix_GF_Mul(const byte a[][4], byte b[][4], byte result[][4]){
  for(unsigned k = 0; k < 4; k++)
    for(unsigned i = 0; i < 4; i++)
      for(unsigned j = 0; j < 4; j++)
        result[i][k] ^= GF_Mul(a[i][j], b[j][k]);
}

void MixColumns(byte b[][4]){
  byte mix[4][4]={0};
  Matrix_GF_Mul(Diffusion_M, b, mix);
  memcpy(b, mix, 16*sizeof(byte));
}

void InvMixColumns(byte b[][4]){
  byte inv_mix[4][4]={0};
  Matrix_GF_Mul(Inv_Diffusion_M, b, inv_mix);
  memcpy(b, inv_mix, 16*sizeof(byte));
}

void AddRoundKey(byte b[][4], word w[], int round){
  byte cur_round_key[4][4], temp[4];
  for(unsigned i = 0; i < 4; ++i){
    SplitWord(w[i + 4*round], temp);
    for(unsigned j = 0; j < 4; ++j)
      b[j][i]^=temp[j];
  }
}

void Array_To_Grid(byte a[16], byte b[4][4]){
  for(unsigned i=0; i<4; ++i)
    for(unsigned j=0; j<4; ++j)
      b[j][i] = a[4*i+j];
}

void Grid_To_Array(byte a[16], byte b[4][4]){
  for(unsigned i=0; i<4; ++i)
    for(unsigned j=0; j<4; ++j)
      a[4*i+j] = b[j][i];
}

void PrintGrid(byte b[][4]){
  printf("\nState:\n");
  for(unsigned i=0; i<4; ++i){
    for(unsigned j=0; j<4; ++j)
      printf("%2x|", b[i][j]);
    printf("\n");
  }
}

void AES128_Enc(byte plain[16], byte cipher[16], word w[44]){
  byte state[4][4];
  Array_To_Grid(plain, state);

  AddRoundKey(state, w, 0);
  for (int j = 1; j <= 10; ++j) {
    SubBytes(state);
    ShiftRows(state);
    if(j<10) MixColumns(state);
    AddRoundKey(state, w, j);
  }

  Grid_To_Array(cipher, state);
}

void AES128_Dec(byte cipher[16], byte plain[16], word w[44]){
  byte state[4][4];
  Array_To_Grid(cipher, state);
  
  AddRoundKey(state, w, 10);
  for (int j = 9; j >= 0; --j) {
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, w, j);
    if(j>0) InvMixColumns(state);
  }
  
  Grid_To_Array(plain, state);
}

unsigned To_Byte_Array(char s[], byte byte_arr[]){
  if(strlen(s)!=32) {printf("ERROR: Wrong length of string\nProgram Aborted\n"); return 0;}
  for(unsigned i=0; i<32; i+=2){
    char a=s[i], b=s[i+1];
    if(!((a>='0' && a<='9')||(a>='a' && a<='f'))) {printf("ERROR: Inappropriate character appeared\nProgram Aborted\n\n"); return 0;};
    if(!((b>='0' && b<='9')||(b>='a' && b<='f'))) {printf("ERROR: Inappropriate character appeared\nProgram Aborted\n\n"); return 0;};
    
    a = (a>='a' && a<='f') ? a-87 : a-'0';
    b = (b>='a' && b<='f') ? b-87 : b-'0';
    
    byte_arr[i/2] = a * 16 + b;
  }
}
unsigned main(){
  char s[40], *p;
  byte input[16], key[16];
  printf("\nAES128\nAuthor: zty\n\nInitialization (Press any key to continue)\n");
  getchar();  //clear buffer
  while(1){
    printf("\nEnter message to be encrypted:\n(32 hexadecimal characters required):\n");
    fgets(s, 40, stdin);
    if (p=strchr(s,'\n')) *p = '\0';
    if (!To_Byte_Array(s, input)) continue;
    break;
  }

  while(1){
    printf("\nEnter encryption key:\n(32 hexadecimal characters required):\n");
    fgets(s, 40, stdin);
    if (p=strchr(s,'\n')) *p = '\0';
    if (!To_Byte_Array(s, key)) continue;
    break;
  }
  
  /*  
  //test case 1 
  byte input[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                  0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
  
  byte key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
      
  /*
  //test case 2
  byte key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  
  byte input[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 
                  0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
  */
  printf("\n==================================\n");
  printf("AES starts:");
  word roundkeys[44];
  byte output[16];

  KeyExpansion(key, roundkeys);
  
  printf("\n\nPlain text:\n");
  for(int i=0;i<16;i++) {
    if(input[i]<16) printf("0");
    printf("%x", input[i]);
  }

  AES128_Enc(input, output, roundkeys);
  printf("\n\nAfter enc:\n");
  for(int i=0;i<16;i++) {
    if(output[i]<16) printf("0");
    printf("%x", output[i]);
  }
  
  AES128_Dec(output, input, roundkeys);
  printf("\n\nAfter dec:\n");
  for(int i=0;i<16;i++) {
    if(input[i]<16) printf("0");
    printf("%x", input[i]);
  }
  printf("\n\nProgram Ended\n");
  return 0;
}