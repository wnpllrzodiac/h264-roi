#include "zhash.h"
#include "zexception.h"
#include "xxhash.h"
#include <functional>

namespace LibChaos {

void ZHashBase::hashData(const zbyte *data, zu64 size, hashMethod method){
    switch(method){
        // Simple hash from some stackoverflow answer
        case hashType1_64: {
            zu64 hash = 5381;
            for(zu64 i = 0; i < size; ++i){
                hash = ((hash << 5) + hash) + data[i]; /* hash * 33 + c */
            }
            return hash;
        }

        // XXH64 hash
        case xxHash64:
            return XXH64(data, size, 0);

        default:
            throw ZException("Unknown hashMethod in hashData");
    }
}

}
