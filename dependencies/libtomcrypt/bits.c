/* portable way to get secure random bits to feed a PRNG */
#ifdef WIN32
#include <windows.h>
#endif

#include "mycrypt.h"

#ifdef DEVRANDOM
/* on *NIX read /dev/random */
static unsigned long rng_nix(unsigned char *buf, unsigned long len, 
                             void (*callback)(void))
{
#ifdef NO_FILE
    return 0;
#else
    FILE *f;
    unsigned long x;
#ifdef TRY_URANDOM_FIRST
    f = fopen("/dev/urandom", "rb");
    if (f == NULL)
#endif /* TRY_URANDOM_FIRST */
       f = fopen("/dev/random", "rb");

    if (f == NULL) {
       return 0;
    }
    
    /* disable buffering */
    if (setvbuf(f, NULL, _IONBF, 0) != 0) {
       fclose(f);
       return 0;
    }   
 
    x = (unsigned long)fread(buf, 1, (size_t)len, f);
    fclose(f);
    return x;
#endif /* NO_FILE */
}

#endif /* DEVRANDOM */

#ifdef SONY_PS2
#include <eetypes.h>
#include <eeregs.h>
#define min(a,b) ((a) < (b) ? (a) : (b))
// Very simple/stupid MD5-based RNG that samples "entropy" from various PS2 control registers
static unsigned long rng_ps2(unsigned char *buf, unsigned long len, 
                             void (*callback)(void))
{
  static unsigned long lastx[2] = { 0xaab7cb4b2fd3b2b9, 0xcec58aff72afe49f }; // md5sum of bits.c
  unsigned long j;
  unsigned int samples[10];  // number of sample data sources
  int l;
  hash_state md;

  for (j = 0; j < len; j += sizeof(lastx)) {
    md5_init(&md);
    samples[0] = *T2_COUNT;
    samples[1] = *T3_COUNT;
    samples[2] = *IPU_TOP;
    samples[3] = *GIF_TAG0;
    samples[4] = *GIF_TAG1;
    samples[5] = *GIF_TAG2;
    samples[6] = *VIF1_CODE;
    samples[7] = *VIF0_CODE;
    samples[8] = *D0_MADR;
    samples[9] = *D1_MADR;
    md5_process(&md, (unsigned char *)(&samples[0]), sizeof(samples));
    // include previous round
    md5_process(&md, (unsigned char *)(&lastx[0]), sizeof(lastx));
    md5_done(&md, (unsigned char *)(&lastx[0]));
    l = min(sizeof(lastx), len-j);
    memcpy(buf+j, &lastx[0], l); //min(sizeof(lastx), len-j));
  }
  return len;
}
#endif /* SONY_PS2 */

/* on ANSI C platforms with 100 < CLOCKS_PER_SEC < 10000 */
#if !defined(SONY_PS2) && defined(CLOCKS_PER_SEC)

#define ANSI_RNG

static unsigned long rng_ansic(unsigned char *buf, unsigned long len, 
                               void (*callback)(void))
{
   clock_t t1;
   int l, acc, bits, a, b;

   if (XCLOCKS_PER_SEC < 100 || XCLOCKS_PER_SEC > 10000) {
      return 0;
   }

   l = len;
   bits = 8;
   acc  = a = b = 0;
   while (len--) {
       if (callback != NULL) callback();
       while (bits--) {
          do {
             t1 = XCLOCK(); while (t1 == XCLOCK()) a ^= 1;
             t1 = XCLOCK(); while (t1 == XCLOCK()) b ^= 1;
          } while (a == b);
          acc = (acc << 1) | a;
       }
       *buf++ = acc; 
       acc  = 0;
       bits = 8;
   }
   acc = bits = a = b = 0;
   return l;
}

#endif 

/* Try the Microsoft CSP */
#ifdef WIN32
#define _WIN32_WINNT 0x0400

#include <wincrypt.h>

static unsigned long rng_win32(unsigned char *buf, unsigned long len, 
                               void (*callback)(void))
{
   HCRYPTPROV hProv = 0;
   if (!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 
                            (CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET)) && 
       !CryptAcquireContext (&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, 
                            CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET | CRYPT_NEWKEYSET))
      return 0;

   if (CryptGenRandom(hProv, len, buf) == TRUE) {
      CryptReleaseContext(hProv, 0);
      return len;
   } else {
      CryptReleaseContext(hProv, 0);
      return 0;
   }
}

#endif /* WIN32 */

unsigned long rng_get_bytes(unsigned char *buf, unsigned long len, 
                            void (*callback)(void))
{
   unsigned long x;

   _ARGCHK(buf != NULL);

#ifdef SONY_PS2
   x = rng_ps2(buf, len, callback);   if (x != 0) { return x; }
#elif defined(DEVRANDOM)
   x = rng_nix(buf, len, callback);   if (x != 0) { return x; }
#endif
#ifdef WIN32
   x = rng_win32(buf, len, callback); if (x != 0) { return x; }
#endif
#ifdef ANSI_RNG
   x = rng_ansic(buf, len, callback); if (x != 0) { return x; }
#endif
   return 0;
}

int rng_make_prng(int bits, int wprng, prng_state *prng, 
                  void (*callback)(void))
{
   unsigned char buf[256];
   int err;
   
   _ARGCHK(prng != NULL);

   /* check parameter */
   if ((err = prng_is_valid(wprng)) != CRYPT_OK) {
      return err;
   }

   if (bits < 64 || bits > 1024) {
      return CRYPT_INVALID_PRNGSIZE;
   }

   if ((err = prng_descriptor[wprng].start(prng)) != CRYPT_OK) {
      return err;
   }

   bits = ((bits/8)+((bits&7)!=0?1:0)) * 2;
   if (rng_get_bytes(buf, (unsigned long)bits, callback) != (unsigned long)bits) {
      return CRYPT_ERROR_READPRNG;
   }

   if ((err = prng_descriptor[wprng].add_entropy(buf, (unsigned long)bits, prng)) != CRYPT_OK) {
      return err;
   }

   if ((err = prng_descriptor[wprng].ready(prng)) != CRYPT_OK) {
      return err;
   }

   #ifdef CLEAN_STACK
      zeromem(buf, sizeof(buf));
   #endif
   return CRYPT_OK;
}

