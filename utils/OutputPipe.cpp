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







