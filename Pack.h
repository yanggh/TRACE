#ifndef __PACK_H_
#define __PACK_H_
#include <stack>
#include "Reader.h"
#include "Glue.h"
class Pack
{
private:
    std::stack<Data>  msg_stack;

    void  push_stack(const Data &data);

public:
    std::string pop_stack(const Data &data);
    std::string impl(const Data &data);
};
#endif
