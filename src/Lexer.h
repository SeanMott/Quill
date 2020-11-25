//parses the tag data for writing html files

#ifndef LexerHeader
#define LexerHeader

#include <string>
#include <vector>

#include "Tag.h"

using namespace std;

//parses the tag data out
void ParseTags(vector<Tag>& tags, const string& source);

//breaks a statment into a statment and it's data
inline void BreakStatment(const string& word, string& statment, string& data);

//assigns style attributes
inline void AssignAttributes(const string& statment, const string& data, Tag& tagData);

#endif