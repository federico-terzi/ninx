//
// Created by federico.terzi2 on 27/12/2018.
//

#include "Assignment.h"

ninx::parser::element::Block *ninx::parser::element::Assignment::get_block() {
    return this->block.get();
}

std::string ninx::parser::element::Assignment::dump() const {
    return "Assignment: "+name+" = "+block->dump();
}

ninx::parser::element::Assignment::Assignment(const std::string &name,
                                              std::unique_ptr<ninx::parser::element::Block> block) : name(name),
                                                                                                     block(std::move(block)) {}
