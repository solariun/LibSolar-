/* LZH.H */

#include <Util.hpp>
#include <stdint.h>
#include <unistd.h>

#include <string>

using namespace std;

uint32_t lzh_encode(uint8_t *inbuf, uint32_t inlen, uint8_t *outbuf);
uint32_t lzh_decode(uint8_t *inbuf, uint32_t inlen, uint8_t *outbuf);

uint32_t lzh_decode(uint8_t *inbuf, uint32_t inlen, string& outbuf);
uint32_t lzh_encode(uint8_t *inbuf, uint32_t inlen, string& strRet);

