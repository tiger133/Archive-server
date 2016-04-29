//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_INPUTPIPE_H
#define UNTITLED_INPUTPIPE_H

namespace Utils
{
    class InputPipe {
    private:
        int descriptor;
    public:
        int getDescriptor() const;
        InputPipe(int i);
        int read(char * buffer, int size);
        int close(int x);
    };
}



#endif //UNTITLED_INPUTPIPE_H
