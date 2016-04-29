//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_PIPEWRAPPER_H
#define UNTITLED_PIPEWRAPPER_H

#include <utility>
#include "OutputPipe.h"
#include "InputPipe.h"

namespace Utils
{
    typedef std::pair<std::shared_ptr<InputPipe >, std::shared_ptr<OutputPipe >> PipePair;

    class PipeFactory {
        PipeFactory(){};
    public:
        static PipePair createPipe();
    };

}


#endif //UNTITLED_PIPEWRAPPER_H
