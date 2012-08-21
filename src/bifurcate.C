/** bifurcate.C, by Steve Wang
 *  Copyright 2012
 *  Basically, take an input stream and pipe it to multiple outputs.
 **/

/*  Read from stdin, then write to multiple streams. */
#include <iostream>
#include <cstring>
#include <list>
#include <cstdio>

#define HELP_STRING \
"Usage: bifurcate type targets...\n\
Types:\n\
  -a  Open the specified file(s) for appending.\n\
  -w  Open the specified file(s) for writing\n\
      (WARNING: will overwrite file contents).\n\
  -p  Pipe to the specified process id(s). Note: only supported\n\
      on flavors of Unix that support the procfs.\n\
  -c  Run the specified commands.\n\
  -h  Print this help message.\n\
\n\
targets corresponds to possible inputs.\n\
"

using namespace std;

enum OutputType
{
	NONE = 0,
	WRITE,	 // write to file; >
	APPEND,  // append to file; >>
	PID,	 // pipe to existing PID; |
	COMMAND, // spawn process via command and provide as stdin.
	HELP
};

typedef unsigned int uint;


void print_help()
{
	puts(HELP_STRING);
}


OutputType specifies_output(char *str)
{
	uint len = strlen(str);
	if (!(len == 2 && str[0] == '-'))
	{
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
	case 'h':
		return HELP;
	default:
		return NONE;
	}
}


ostream *open_file(char *str, OutputType t)
{
	return NULL;
}


ostream *parse_pid(char *str)
{
	return NULL;
}


ostream *spawn(char *str)
{
	return NULL;
}


int add_stream(list<ostream*> *streams, char *str, OutputType t)
{
	switch (t)
	{
	case WRITE:
	case APPEND:
		streams->push_front(open_file(str, t));
		break;
	case PID:
		streams->push_front(parse_pid(str));
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
		(t = specifies_output(*v)) || add_stream(streams, *v, t);
		if (t == HELP)
		{
			print_help();
			return 0;
		}
		++v;
	}
	bifurcate(input, streams);
}
