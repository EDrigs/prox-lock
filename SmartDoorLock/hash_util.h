#ifndef HASH_UTIL_H
#define HASH_UTIL_H

#include <Arduino.h>

class HashUtil {
public:
    static String hashCode(const String &code);

private:
    static String bytesToHex(const byte *data, size_t length);
};

#endif
