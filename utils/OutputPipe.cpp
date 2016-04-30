//
// Created by piotr on 29.04.16.
//

#include "OutputPipe.h"


Utils::OutputPipe::OutputPipe(int i): descriptor(i) {

}

int Utils::OutputPipe::write(const char * buffer, int size) {
    return (int)::write(descriptor,buffer,(size_t)size);
}

int Utils::OutputPipe::close() {
    return ::close(descriptor);
}

int Utils::OutputPipe::getDescriptor() const {
    return descriptor;
}

int Utils::OutputPipe::write(std::string msg) {
    const char * buff = msg.c_str();
    int len = (int)msg.length();
    return write(buff,len);
}

Utils::OutputPipe::OutputPipe(OutputPipe &second) {
    descriptor = second.descriptor;
}











