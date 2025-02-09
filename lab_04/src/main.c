#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "md5.h"
#include "rsa.h"

void print_hash(uint8_t p[]) {
  for (int i = 0; i < RESULT_SIZE; ++i) {
    printf("%2.2x", p[i]);
  }
  printf("\n");
}

int main(const int argc, char *argv[]) 
{
  if (strcmp(argv[1], "h") == 0)
  {
    uint8_t result[RESULT_SIZE];
    FILE *f = fopen(argv[2], "r");
    md5sum(f, result);
    fclose(f);
    print_hash(result);
  } 
  else if (strcmp(argv[1], "e") == 0) {
    struct pub_key_t pub;
    struct priv_key_t priv;
    rsa_gen_keys(&pub, &priv);
    uint8_t hash[RESULT_SIZE] = {0};

    FILE *f = fopen(argv[2], "r");
    md5sum(f, hash);
    printf("before rsa: ");
    print_hash(hash);
    uint64_t after_rsa[RESULT_SIZE] = {0};
    FILE *sign_file = fopen("sign.txt", "w");

    for (int i = 0; i < RESULT_SIZE; ++i)
    {
      after_rsa[i] = rsa_encrypt((long long int)hash[i], &pub);
      fprintf(sign_file, "%llu\n", after_rsa[i]); 
      printf("%lld\n", after_rsa[i]);
    }
    // printf("after rsa: ");
    // print_hash(after_rsa);
    fclose(sign_file);
  } 
  else if (strcmp(argv[1], "d") == 0)
  {
    struct pub_key_t pub;
    struct priv_key_t priv;

    rsa_gen_keys(&pub, &priv);
    // char signature[];
    printf("Enter signature: \n");

    uint8_t after_rsa[RESULT_SIZE] = {0};
    FILE *sign_file = fopen("sign.txt", "r");
    for (int i = 0; i < RESULT_SIZE; i++)
    {
      long long int encrypted;
      fscanf(sign_file, "%lld", &encrypted); 
      after_rsa[i] = rsa_decrypt(encrypted, &priv);
    }
    printf("after rsa: \n");
    print_hash(after_rsa);

    uint8_t hash[RESULT_SIZE] = {0};

    FILE *f = fopen(argv[2], "r");
    md5sum(f, hash);
    printf("original: \n");
    print_hash(hash);
    for (int i = 0; i < RESULT_SIZE; i++)
    {
      if (after_rsa[i] != hash[i])
      {
        printf("file was changed by another\n");
        return -1;
      }
    }
    printf("file ok!\n");

  }


  //   struct pub_key_t pub;
  //   struct priv_key_t priv;
  //   rsa_gen_keys(&pub, &priv);
  //     long long x = rsa_encrypt(50, &pub);
  //     long long y = rsa_decrypt(x, &priv);
  // printf("%llu %llu\n", x, y);



  return 0;
}
