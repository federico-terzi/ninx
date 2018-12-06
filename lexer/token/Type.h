//
// Created by Federico on 06-Dec-18.
//

#ifndef NINX_TYPE_H
#define NINX_TYPE_H

namespace ninx {
    namespace lexer {
        namespace token {
            enum class Type {
                TEXT,
                LIMITER,
                ID,
                VARIABLE
            };
        }
    }
}

#endif //NINX_TYPE_H
