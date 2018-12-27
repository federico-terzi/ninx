//
// Created by federico.terzi2 on 27/12/2018.
//

#ifndef NINX_ASSIGNMENT_H
#define NINX_ASSIGNMENT_H

#include "Statement.h"
#include "Block.h"

namespace ninx {
    namespace parser {
        namespace element {
            class Assignment : public Statement {
            public:
                explicit Assignment(const std::string &name, std::unique_ptr<Block> block);

                std::string dump() const override;

                Block * get_block();
            private:
                std::string name;  // Variable name

                std::unique_ptr<Block> block;   // The content of the variable
            };
        }
    }
}

#endif //NINX_ASSIGNMENT_H
