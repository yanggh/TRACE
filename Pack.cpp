#include "Pack.h"
#include <string>

using namespace std;
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

std::string Pack::pop_stack(const Data &data)
{
	string output("invalid");

	string spli1; 
	spli1.resize(2);
	spli1[0]=0x0d;
	spli1[1]=0x0a;

	string spli2; 
	spli2.resize(1);
	spli2[0]=0x7e;

	string dst = msg_stack.top().data;
	string::size_type pos1 = dst.find(spli1);
	if(string::npos != pos1)
	{
		if(pos1 == 0)
		{
			msg_stack.top().data.erase(0, 2);
		}
		else
		{
			output = dst.substr(0, pos1+2);
			msg_stack.top().data.erase(0, pos1+2);
		}
	}
	else
	{
	    pos1 = dst.find(spli2);
            if(string::npos != pos1)
	    {
	    	string::size_type pos2 = dst.find(spli2, pos1+1);

		if(string::npos != pos2)
		{
			if(pos2 - pos1 == 1)
			{
				pos1 = pos2;
				pos2 = dst.find(spli2, pos1+1);
				if(string::npos != pos2)
				{
					output = dst.substr(pos1, pos2+1);
					msg_stack.top().data.erase(0, pos2+1);
				}
			}
			else
			{
				output = dst.substr(pos1, pos2+1);
				msg_stack.top().data.erase(0, pos2+1);
			}
		}
	    }
	}

	return  output;
}

string Pack::impl(const Data &data)
{
	push_stack(data);
	string dst = pop_stack(data);

	return dst;
}

