#include <stdio.h>
#include <string.h>
#include <iconv.h>
#include <openssl/evp.h>
#include <errno.h>

main(int argc, char *argv[]) {

  EVP_MD_CTX mdctx;
  char *password, *password_copy;
  char *md_value, *md_value_copy;
  int md_len, i;
  size_t pwlen, pwlen_copy, pwlen2;
  iconv_t cd;
  const char *salt = "WDC.";

  pwlen = strlen(argv[1]) + 4;
  password = malloc (pwlen+1);
  password_copy = password;
  strcpy (password, salt);
  strcat (password, argv[1]);
  pwlen_copy = pwlen;

  cd = iconv_open ("UTF-16LE", "UTF-8");
  md_value = malloc (EVP_MAX_MD_SIZE);
  md_value_copy = md_value;
  md_len = iconv (cd, &password_copy, &pwlen_copy, &md_value_copy, &pwlen2);
  iconv_close (cd);
  md_len = md_value_copy - md_value;

  OpenSSL_add_all_digests();

  for (i=0;i<1000;i++) {
    EVP_MD_CTX_init(&mdctx);
    EVP_DigestInit_ex(&mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(&mdctx, md_value, md_len);
    EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
    EVP_MD_CTX_cleanup(&mdctx);
    }

  for(i = 0; i < md_len; i++) printf("%02x", md_value[i] & 0xff);
  printf("\n");
  }
