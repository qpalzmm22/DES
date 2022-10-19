#include <iostream>
#include <sstream>
#define ull unsigned long long 

using namespace std;
ull parity_drop[56]={57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
ull compression_table[48]={14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
ull initial_permutation[64]={58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7};
ull final_permutation[64]={40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25};
ull expansion_pbox[48]={32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1};
ull straight_pbox[32]={16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
ull sbox[8][16][4]={{14,0,4,15,4,15,1,12,13,7,14,8,1,4,8,2,2,14,13,4,15,2,6,9,11,13,2,1,8,1,11,7,3,10,15,5,10,6,12,11,6,12,9,3,12,11,7,14,5,9,3,10,9,5,10,0,0,3,5,6,7,8,0,13},
	{15,3,0,13,1,13,14,8,8,4,7,10,14,7,11,1,6,15,10,3,11,2,4,15,3,8,13,4,4,14,1,2,9,12,5,11,7,0,8,6,2,1,12,7,13,10,6,12,12,6,9,0,0,9,3,5,5,11,2,14,10,5,15,9},
	{10,13,13,1,0,7,6,10,9,0,4,13,14,9,9,0,6,3,8,6,3,4,15,9,15,6,3,8,5,10,0,7,1,2,11,4,13,8,1,15,12,5,2,14,7,14,12,3,11,12,5,11,4,11,10,5,2,15,14,2,8,1,7,12},
	{7,13,10,3,13,8,6,15,14,11,9,0,3,5,0,6,0,6,12,10,6,15,11,1,9,0,7,13,10,3,13,8,1,4,15,9,2,7,1,4,8,2,3,5,5,12,14,11,11,1,5,12,12,10,2,7,4,14,8,2,15,9,4,14},
	{2,14,4,11,12,11,2,8,4,2,1,12,1,12,11,7,7,4,10,1,10,7,13,14,11,13,7,2,6,1,8,13,8,5,15,6,5,0,9,15,3,15,12,0,15,10,5,9,13,3,6,10,0,9,3,4,14,8,0,5,9,6,14,3},
	{12,10,9,4,1,15,14,3,10,4,15,2,15,2,5,12,9,7,2,9,2,12,8,5,6,9,12,15,8,5,3,10,0,6,7,11,13,1,0,14,3,13,4,1,4,14,10,7,14,0,1,6,7,11,13,0,5,3,11,8,11,8,6,13},
	{4,13,1,6,11,0,4,11,2,11,11,13,14,7,13,8,15,4,12,1,0,9,3,4,8,1,7,10,13,10,14,7,3,14,10,9,12,3,15,5,9,5,6,0,7,12,8,15,5,2,0,14,10,15,5,2,6,8,9,3,1,6,2,12},
	{13,1,7,2,2,15,11,1,8,13,4,14,4,8,1,7,6,10,9,4,15,3,12,10,11,7,14,8,1,4,2,13,10,12,0,15,9,5,6,12,3,6,10,9,14,11,13,0,5,0,15,3,0,14,3,5,12,9,5,6,7,2,8,11}};

ull r_keys[16];

void assign(bool txt[64], ull orig_txt){
  for(int i = 0 ; i < 64; i++){
    txt[i] = (orig_txt & 1) ;
    orig_txt = orig_txt >> 1;
  }
}

void printTxt(ull txt){
  for(int i = 63 ; i >= 0; i--){
    cout << ((txt >> i) & 1) << " ";
    if(i % 8 == 0)
      cout << "\n";
  }
  cout << "\n";
}

ull initial_perm(ull plain_txt){
  ull ctxt = 0;
  for(int i = 63 ; i >= 0; i--){
    ctxt = ctxt << 1;
    ctxt |= ((plain_txt >> (initial_permutation[i] - 1)) & 1);
  }
  //printTxt(ctxt);
  return ctxt;
}

ull final_perm(ull plain_txt){
  ull ctxt = 0;
  for(int i = 63 ; i >= 0; i--){
    ctxt = ctxt << 1;
    ctxt |= ((plain_txt >> (final_permutation[i] - 1)) & 1);
  }
  //printTxt(ctxt);
  return ctxt;
}

ull left_shift(ull num, int bits, int total_bits){
  return (((num << bits) | (num >> (total_bits - bits))) & ((1 << total_bits) - 1)) ;
}

void key_generation(ull key){
  ull new_key = 0;
  //cout << "B4 : parity_drop "  << key << "\n";
  //printTxt(key);
  for(int i = 0 ; i < 56 ; i++){
    new_key = new_key << 1;
    new_key |= ((key >> (64 - parity_drop[i] )) & 1);
  }
  cout << "key after parity_drop : "  << new_key << "\n";
  
  ull left_key;
  ull right_key;
  
  for(int i = 0 ; i < 16; i++){
    right_key = new_key & 0xfffffff; // 28
    left_key = new_key >> 28;
    
    int round  = i + 1;
    if(round == 1 || round == 2 || round == 9 || round == 16){
      left_key = left_shift(left_key, 1, 28);
      right_key = left_shift(right_key, 1, 28);
    } else{
      left_key = left_shift(left_key, 2, 28);
      right_key = left_shift(right_key, 2, 28);
    }
   
    cout << "key after shift : Left Key : "  << left_key << "\n";
    cout << "key after shift : Right Key : "  << right_key << "\n";

    new_key = (left_key << 28) | right_key;
    
    cout << "key after combining : "  << new_key << "\n";
    
    ull tmp = new_key;
    for(int j = 0; j < 48; j++){
      r_keys[i] = r_keys[i] << 1;
      r_keys[i] |= ((new_key >> (56 - compression_table[j])) & 1);

    }
    cout << i << "-th key : "<< r_keys[i] << "\n" ;
  }
}

ull des_function(ull plain_txt, ull r_key){
  // expansion_pbox
  ull ctxt = 0;
  for(int i = 47 ; i >=0 ; i--){
    ctxt = ctxt << 1;
    ctxt |= ((plain_txt >> (expansion_pbox[i] - 1)) & 1);
  }

  cout << "cipher after expansion_pbox "  << ctxt << "\n";
  
  // xor with key
  ctxt = ctxt ^ r_key;

  
  cout << "cipher after xor with key " << ctxt << "\n";
  
    // s-box
  ull sboxed_txt = 0;
  
  for(int i = 0 ; i < 8; i++){
    int boxMask = 0b111111;
    int colMask = 0b011110;
    int rowMask = 0b100001;

    ull box = ctxt & boxMask;
    int col = (box & colMask) >> 1; 
    int row = ((((box >> 5) & 1 ) << 1) | (box & 0b1)) ;

    ctxt = ctxt >> 6;
    sboxed_txt |= sbox[7-i][col][row] << (4 * i) ;
  }

  cout << "cipher after sbox " << sboxed_txt << "\n";
  
  
  ull res_txt = 0;
  // straight p-box
  for(int i = 0; i < 32; i++){
    res_txt = res_txt << 1;
    res_txt |= ((sboxed_txt >> (32 - straight_pbox[i])) & 1) ;
  }
  cout << "cipher after straight_pbox " << res_txt<< "\n";
  
  return res_txt;
}

int main(){
  ull key = 0; // 8byte
  ull new_key = 0; 
  ull plain_txt = 0;
  ull cipher_txt = 0;
  
  string key_input;
  string plain_txt_input;

  cout << "Plaintxt : ";
  cin >> plain_txt_input;
  cout << "Key : ";
  cin >> key_input; 

  stringstream convert_key(key_input);
  stringstream convert_txt(plain_txt_input);
  convert_key >> hex >> key;
  convert_txt >> hex >> plain_txt;

  cipher_txt = plain_txt;
  //cout << hex << "B4 : initial_permutation "  << cipher_txt << "\n";
  cipher_txt = initial_perm(cipher_txt);
  cout << hex << "cipher after initial_permutation "  << cipher_txt << "\n";
 
  key_generation(key); // keys are created

  int round = 0 ;
  ull right_txt = cipher_txt & 0xffffffff ;
  ull left_txt = cipher_txt >> 32; 
  
  //cout  << "   Round  |   Left  |  Right | Round Key\n";
  for(round = 0; round < 16; round++){
 
    ull tmp = left_txt;
    left_txt = right_txt;
    right_txt = tmp ^ des_function(right_txt, r_keys[round]);
  
    if(round == 15){
      tmp = left_txt;
      left_txt = right_txt;
      right_txt = tmp;
    }
    
    printf(" Round %2d : \nleft key : %08llx \nright_key %08llx \nkey %012llx\n", round + 1, left_txt, right_txt, r_keys[round]);
}
  
  cipher_txt = (left_txt << 32) | right_txt;

  //cout << "B4 final_permutation : " << cipher_txt  << "\n";
  cipher_txt = final_perm(cipher_txt);
  cout << "after final_permutation : " << cipher_txt  << "\n";
  
  cout << hex << "Cipher text : " << cipher_txt << "\n";
  return 0;
}
