//
// Created by joanna on 16.05.16.
//

#ifndef UNTITLED_SECURITY_H
#define UNTITLED_SECURITY_H
#include "../Network/TCP.h"
#include "../Network/Connection.h"
#include "crypto++/aes.h"
#include "crypto++/hex.h"
#include "crypto++/rsa.h"
#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>
#include "crypto++/modes.h"


class Security {
    const static int MODULUS_SIZE=512; //in bits
    CryptoPP::RSA::PrivateKey privKey;
    CryptoPP::RSA::PublicKey pubKey;
    byte *aesKey;
    Network::TCP tcp;
public:
    Security(Network::Connection x);
    int send(std::shared_ptr<char> data, int size);
    std::string receive();
};


#endif //UNTITLED_SECURITY_H
