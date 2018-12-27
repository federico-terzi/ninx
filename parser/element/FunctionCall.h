//
// Created by federico.terzi2 on 25/12/2018.
//

#ifndef NINX_FUNCTIONCALL_H
#define NINX_FUNCTIONCALL_H

#include "Statement.h"

namespace ninx {
    namespace parser {
        namespace element {
            class FunctionCall : public Statement {
            public:
                explicit FunctionCall(const std::string &name);

                std::string dump(int level) const override;

            private:
                std::string name; // The name of the function to be called
            };
        }
    }
}


#endif //NINX_FUNCTIONCALL_H
