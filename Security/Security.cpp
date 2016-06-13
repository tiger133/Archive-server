//
// Created by joanna on 16.05.16.
//
#include <iostream>
#include <sstream>
#include "Security.h"

Security::Security(Network::Connection x) : tcp(x) {
    CryptoPP::AutoSeededRandomPool prng;

    privKey.GenerateRandomWithKeySize(prng, MODULUS_SIZE);
    pubKey = CryptoPP::RSA::PublicKey(privKey);

    aesKey = new byte[32];

}

std::string encrypt(std::string& plain, CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption &cipher)
{
    std::string c;
    try
    {
        CryptoPP::StringSource
                ( plain, true, new CryptoPP::StreamTransformationFilter
                        ( cipher, new CryptoPP::StringSink( c ))
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
    try {
        CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption d;
        d.SetKey(&key, 32);

        CryptoPP::StringSource
                (cipher, true,
                        (new CryptoPP::StreamTransformationFilter
                                 (d, new CryptoPP::StringSink(recovered),
                                   CryptoPP::StreamTransformationFilter::NO_PADDING)));
        std::cout << "recovered text: " << recovered << std::endl;

    }  catch( CryptoPP::Exception& e ) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    return recovered;
}

int Security::send(std::string data, int size) {
    if(aesKey == nullptr)
        return -1; // error, session key wasn't established yet

    std::cout<<data<<std::endl;
    std::string ciphertext;

    try {
        CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption e;
        e.SetKey(aesKey, 32);

        ciphertext = encrypt(data, e);
    }
    catch( CryptoPP::Exception& e )
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    std::cout << std::endl << std::endl;
    std::cout<<ciphertext<<std::endl;


    std::string withHeader = "3" + ciphertext;
    //std::string withHeader = "3" + data; //wersja bez szyfrowania !!!!!!!!!!!!!!!
    withHeader[0]=3;
    char* toSend = new char[withHeader.size()];
    memcpy(toSend, withHeader.c_str(), withHeader.size());

    std::cout<< toSend << std::endl;

    tcp.send(std::shared_ptr<char>(toSend), withHeader.size());
}

std::string Security::receive() {
    std::string data;
    try {
        data = tcp.receive();
    } catch (const char* msg) {
        return nullptr;
    }
    int header = int(data[0]);
    std::cout<< header << std::endl;

    if(header == 0) { //client sent a request for public key
        std::cout<< "Sending public key..." << std::endl;
        CryptoPP::Integer modulus = pubKey.GetModulus();
        CryptoPP::Integer pub = pubKey.GetPublicExponent();
        char *key1 = new char[1 + MODULUS_SIZE/8];
        char *key2 = new char[1 + MODULUS_SIZE/8];

        key1[0]=1;
        modulus.Encode((byte*)key1+1,MODULUS_SIZE/8);
        key2[0]=1;
        pub.Encode((byte*)key2+1,MODULUS_SIZE/8);

        tcp.send(std::shared_ptr<char>(key1),1 + MODULUS_SIZE/8);
        tcp.send(std::shared_ptr<char>(key2),1 + MODULUS_SIZE/8);
    }
    else if(header==2) { //client sent encrypted session key
        std::cout<< "Session key received." << std::endl;
        //decoding key
        CryptoPP::Integer m((const byte *)data.data()+1, data.size()-1);
        // Decryption
        CryptoPP::AutoSeededRandomPool prng;
        CryptoPP::Integer r = privKey.CalculateInverse(prng, m);

        r.Encode(aesKey, 32);
    }
    else if(header == 3) { //receving data
        std::string msg_string(data.data()+1,data.size()-1);

        std::string decrypted = decrypt(*aesKey, msg_string);
        std::cout<< "decrypted message: " << decrypted << std::endl;
        return decrypted;
    }
}





