//
// Created by piotr on 29.04.16.
//

#include <unistd.h>
#include "Pipe.h"

std::pair<int, int> Utils::Pipe::createPipe() {
    int fd[2];
    if(pipe(fd)==-1)
    {

    }
    return std::pair<int, int>(fd[0],fd[1]);
}

