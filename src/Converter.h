//converts Quill into HTML and writes to a file

#ifndef ConverterHeader
#define ConverterHeader

#include <vector>
#include <string>

#include "Tag.h"

using namespace std;

//converts Quill into HTML and writes to a file
void Convert(vector<Tag>& tags, string buildPath);

//writes the style attributes of a tag
void WriteStyleAttributes(string& tag, Tag& tagData);
#endif