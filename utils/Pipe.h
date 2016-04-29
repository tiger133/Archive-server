//
// Created by piotr on 29.04.16.
//

#ifndef UNTITLED_PIPEWRAPPER_H
#define UNTITLED_PIPEWRAPPER_H

#include <utility>

namespace Utils
{

    class Pipe {
    public:
        std::pair<int,int> createPipe();
        void closeChild();
        void closeParent();
    };
}


#endif //UNTITLED_PIPEWRAPPER_H
