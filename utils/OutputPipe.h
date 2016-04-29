//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_OUTPUTPIPE_H
#define UNTITLED_OUTPUTPIPE_H

#include <unistd.h>

namespace Utils
{
    class OutputPipe {
        int descriptor;
    public:
        int getDescriptor() const;
        OutputPipe(int outputPipeDesc);
        int write(const char * buffer, int size);
        int close();
    };
}



#endif //UNTITLED_OUTPUTPIPE_H
