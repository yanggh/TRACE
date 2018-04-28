#include "Pack.h"

void  Pack::push_stack(const Data &data)
{
	if(msg_stack.size() > 0)
	{
		if(msg_stack.top().type != data.type)
		{
			msg_stack.push(data); 
		}
		else
		{	
			msg_stack.top().data += data.data;
		}
	}
	else
	{
		msg_stack.push(data);
	}
}

string Pack::pop_stack(const Data &data)
{
	std::cout << "------------------" << std::endl;
	std::cout << "type = " << std::hex << msg_stack.top().type << std::endl;
	std::cout << msg_stack.top().data << std::endl;

	string dst = get_dst(msg_stack.top());
	
	msg_stack.pop();

	return move(dst);
}

string Pack::impl(const Data &data)
{
	string dst = "invalid";
	push_stack(data);
	if(data.flag == 1)
	{
	    dst = pop_stack(data);
	}

	return move(dst);
}

