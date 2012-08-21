/** bifurcate.C, by Steve Wang
 *  Copyright 2012
 *  Basically, take an input stream and pipe it to multiple outputs.
 **/

/*  Read from stdin, then write to multiple streams. */
#include <iostream>
#include <cstring>
#include <list>

using namespace std;

enum OutputType {
	NONE = 0,
	WRITE,	// write to file; >
	APPEND, // append to file; >>
	PID,	// pipe to existing PID; |
	COMMAND // spawn process via command and provide as stdin.
};

typedef unsigned int uint;


OutputType specifiesOutput(char *str)
{
	uint len = strlen(str);
	if (!(len == 2 && str[0] == '-')) {
	    return NONE;
	}

	switch (str[1])
	{
	case 'p':
		return PID;
	case 'w':
		return WRITE;
	case 'a':
		return APPEND;
	default:
		return NONE;
	}
}


ostream *openFile(char *str, OutputType t)
{
	return NULL;
}


ostream *parsePid(char *str)
{
	return NULL;
}


ostream *spawn(char *str)
{
	return NULL;
}


int addStream(list<ostream*> *streams, char *str, OutputType t)
{
	switch (t)
	{
	case WRITE:
	case APPEND:
		streams->push_front(openFile(str, t));
		break;
	case PID:
		streams->push_front(parsePid(str));
		break;
	case COMMAND:
		streams->push_front(spawn(str));
		break;
	}
	return 0;
}


void bifurcate(istream *input, list<ostream*> *streams)
{
	list<ostream*>::iterator
	    iter = streams->begin(),
	    end = streams->end();

	char c;
	uint available = input->rdbuf()->in_avail();
	while (!input->eof())
	{
		input->get(c);

		for (list<ostream*>::iterator i = iter; i != end; i++)
		{
			(*i)->put(c);
		}
	}
}


int main(int c, char **v)
{
	istream *input = &cin;
	list<ostream*> *streams = new list<ostream*>();
	OutputType t = PID;
	while (c--)
	{
		(t = specifiesOutput(*v)) || addStream(streams, *v, t);
		++v;
	}
	bifurcate(input, streams);
}
