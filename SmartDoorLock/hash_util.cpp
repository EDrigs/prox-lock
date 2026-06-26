#include "hash_util.h"

#include <Crypto.h>           // Software hashing
#include <SHA256.h>
#include <FlashStorage.h>     // For flash storage on Nano 33 IoT

String HashUtil::hashCode(const String &code) {
    byte hash[32];

    // --- Use software SHA-256 hashing only ---
    // The ECCX08 hardware path requires exact 64-byte padding which is impractical
    // for variable-length passcodes. Software hashing is deterministic and sufficient.
    SHA256 sha256;
    sha256.update((const byte*)code.c_str(), code.length());
    sha256.finalize(hash, sizeof(hash)); 

    return bytesToHex(hash, sizeof(hash));
}

// Helper function: convert bytes to lowercase hex string
String HashUtil::bytesToHex(const byte *data, size_t length) {
    const char *hexmap = "0123456789abcdef";
    String hexString;
    hexString.reserve(length * 2);
    for (size_t i = 0; i < length; i++) {
        uint8_t b = data[i];
        hexString += hexmap[(b >> 4) & 0x0F];
        hexString += hexmap[b & 0x0F];
    }
    return hexString;
}