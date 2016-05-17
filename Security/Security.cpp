//
// Created by joanna on 16.05.16.
//
#include <iostream>
#include <sstream>
#include "Security.h"

Security::Security(Network::Connection x) : tcp(x) {
    CryptoPP::AutoSeededRandomPool prng;

    privKey.GenerateRandomWithKeySize(prng, 64);
    pubKey = CryptoPP::RSA::PublicKey(privKey);
    /*
    CryptoPP::Integer n("0xbeaadb3d839f3b5f"), e("0x11"), d("0x21a5ae37b9959db9");

    privKey.Initialize(n, e, d);
    pubKey.Initialize(n, e);
     */
    aesKey = new byte[16];
    for(int i = 0; i < 16; i++)
    {
        aesKey[i] = 'a';
    }
}

std::string encrypt(std::string& plain, CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption &cipher)
{
    std::string c;
    try
    {
        CryptoPP::StringSource( plain, true,
                                new CryptoPP::StreamTransformationFilter( cipher,
                                                                          new CryptoPP::StringSink( c )
                                )
        );
    }catch( CryptoPP::Exception& e )
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return c;

}

std::string decrypt( unsigned char &key, std::string& cipher)
{
    std::string recovered;
    try
    {
        CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption d;
        d.SetKey( &key, 16 );

        CryptoPP::StringSource( cipher, true,
                                new CryptoPP::StreamTransformationFilter( d,
                                                                          new CryptoPP::StringSink( recovered )
                                )
        );

        std::cout << "recovered text: " << recovered << std::endl;
    }
    catch( CryptoPP::Exception& e )
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return recovered;
}

int Security::send(std::shared_ptr<char> data, int size) {
    if(aesKey == nullptr)
        return -1; // error, session key wasn't established yet
    std::string msg_string = data.get();
    std::cout<<msg_string<<std::endl;
    std::string ciphertext;

    try {
        CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption e;
        e.SetKey(aesKey, 16);

        ciphertext = encrypt(msg_string, e);
    }
    catch( CryptoPP::Exception& e )
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    //
    // Dump Cipher Text
    //
    std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;

    for( int i = 0; i < ciphertext.size(); i++ ) {
        std::cout << "0x" << std::hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
    }
    std::cout << std::endl << std::endl;
    std::cout<<ciphertext<<std::endl;
    std::string withHeader = "3" + ciphertext;
    char* toSend = new char[withHeader.size()];
    memcpy(toSend, withHeader.c_str(), withHeader.size());

    std::cout<< toSend << std::endl;
    std::cout<< withHeader.size() << std::endl;

    tcp.send(std::shared_ptr<char>(toSend), withHeader.size());
}

std::shared_ptr<char> Security::receive() {
    std::shared_ptr<char> data = tcp.receive();
    int header = int(data.get()[0]);

    if(header == 0) { //client sent a request for public key
        CryptoPP::Integer modulus = pubKey.GetModulus();
        CryptoPP::Integer pub = pubKey.GetPublicExponent();
        char *key1 = new char[2 + sizeof(modulus)];
        char *key2 = new char[2 + sizeof(pub)];

        std::stringstream ss;
        ss << std::hex << modulus;
        strcpy(key1, "1");
        memcpy(key1 + 1, ss.str().c_str(), sizeof(modulus));
        ss << std::hex << pub;
        strcpy(key2, "1");
        memcpy(key2 + 1, ss.str().c_str(), sizeof(pub));

        tcp.send(std::shared_ptr<char>(key1), sizeof(modulus) + 2);
        tcp.send(std::shared_ptr<char>(key2), sizeof(pub) + 2);
    }
    else if(header==2) { //client sent encrypted session key
        std::cout<<header<<std::endl;
        char* key = new char[sizeof(data) - 1];
        memcpy(key, data.get() + 1, sizeof(data) - 1);

        //decoding key
        std::string message = key;
        CryptoPP::Integer m((const byte *)message.data(), message.size());
        // Decryption
        CryptoPP::AutoSeededRandomPool prng;
        CryptoPP::Integer r = privKey.CalculateInverse(prng, m);
        std::cout << "r: " << std::hex << r << std::endl;

        // r: 736563726574h
        std::string recovered;
        recovered.resize(r.MinEncodedSize());

        r.Encode((byte *)recovered.data(), recovered.size());
        std::cout << "aesKey: " << recovered << std::endl;

        memcpy(aesKey, recovered.c_str(), 16);
    }
    else if(header == 3) { //receving data
        char* msg = new char[sizeof(data) - 1];
        memcpy(msg, data.get() + 1, sizeof(data) - 1);
        std::string msg_string = msg;
        std::string encoded;
        CryptoPP::StringSource( msg, true,
                                new CryptoPP::HexEncoder(
                                        new CryptoPP::StringSink( encoded )
                                ) // HexEncoder
        );
        std::cout << "cipher text: " << encoded << std::endl;
        std::string decrypted = decrypt(*aesKey, msg_string);
        return std::shared_ptr<char>((char*)decrypted.c_str());
    }
}




