//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_OUTPUTPIPE_H
#define UNTITLED_OUTPUTPIPE_H

#include <string>
#include <unistd.h>

namespace Utils
{
    class OutputPipe {
        int descriptor;
    public:
        int getDescriptor() const;
        OutputPipe(int outputPipeDesc);
        OutputPipe(OutputPipe & second);
        int write(const char * buffer, int size);
        int write(std::string msg);
        int close();
    };
}



#endif //UNTITLED_OUTPUTPIPE_H
