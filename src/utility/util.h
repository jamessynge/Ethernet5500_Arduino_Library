#ifndef ETHERNET5500_SRC_UTILITY_UTIL_H_
#define ETHERNET5500_SRC_UTILITY_UTIL_H_

#define htons(x) (((x) << 8 & 0xFF00) | ((x) >> 8 & 0x00FF))
#define ntohs(x) htons(x)

#define htonl(x)                                            \
  (((x) << 24 & 0xFF000000UL) | ((x) << 8 & 0x00FF0000UL) | \
   ((x) >> 8 & 0x0000FF00UL) | ((x) >> 24 & 0x000000FFUL))
#define ntohl(x) htonl(x)

#endif  // ETHERNET5500_SRC_UTILITY_UTIL_H_
