//
// Created by piotr on 29.04.16.
//

#include <unistd.h>
#include "InputPipe.h"


Utils::InputPipe::InputPipe(int i): descriptor(i) {

}

int Utils::InputPipe::read(char *buffer, int size) {
    return (int)::read(descriptor, buffer,  (size_t)size);
}

int Utils::InputPipe::close(int x) {
    return ::close(descriptor);
}
int Utils::InputPipe::getDescriptor() const {
    return descriptor;
}




