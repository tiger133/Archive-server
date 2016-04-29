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

    class PipeFactory {
        PipeFactory(){};
    public:
        static std::pair<std::shared_ptr<InputPipe >, std::shared_ptr<OutputPipe >> createPipe();
    };
}


#endif //UNTITLED_PIPEWRAPPER_H
