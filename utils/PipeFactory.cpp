//
// Created by piotr on 29.04.16.
//

#include <unistd.h>
#include <iostream>
#include <memory>
#include "PipeFactory.h"

using InputPipe = Utils::InputPipe;
using OutputPipe = Utils::OutputPipe;

std::pair<std::shared_ptr<InputPipe >, std::shared_ptr<OutputPipe >> Utils::PipeFactory::createPipe() {
    int fd[2];
    if(pipe(fd) == -1)
        throw std::runtime_error("Cannot create a pipe");
    std::shared_ptr<InputPipe> inputPtr(new InputPipe(fd[0]));
    std::shared_ptr<OutputPipe> outputPtr(new OutputPipe(fd[1]));
    return std::make_pair<std::shared_ptr<InputPipe>,std::shared_ptr<OutputPipe>>((std::shared_ptr<InputPipe> &&) inputPtr,
                                                                           (std::shared_ptr<OutputPipe> &&) outputPtr);

    //return std::pair<std::shared_ptr<InputPipe>, std::shared_ptr<OutputPipe> >(inputPtr,outputPtr);
}

