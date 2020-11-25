/*
Quill allows you write HTML code in a C like syntax.
Quill takes in a settings file that has a source folder, build folder, and the file to convert to HTML.
Refrence for settings file is in the res folder.
Quill Site folder has a example of a .quill file.
*/

#include <iostream>
#include <fstream>

#include "Lexer.h"
#include "Converter.h"

using namespace std;

ifstream readStream;

bool useSetings = false;
string sourceFile = ""; //file name
string sourceDir = ""; //../QuillSite/src
string buildDir = ""; //../QuillSite/build
string path = "../QuillSite/settings.txt";
/*
Settings files are formated -
Source Dir
Build Dir
fileName.quill
*/

int main(int args, char* argv[])
{
	/*get source dir, build dir, and source file from setting file path
	cout << "Enter settings file path.\n";
	cin >> path;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clears the buffer
	readStream.open(path);
	if (!readStream.is_open())
	{
		cout << "Failed to retrive build dir, source dir, and source file from settings, Path: \"" + path + "\"\nterminating!";
		cin.get();
		return -1;
	}
	*/

	//if the settings file should be used
	if (useSetings)
	{
		getline(readStream, sourceDir);
		getline(readStream, buildDir);
		getline(readStream, sourceFile);
		readStream.close();
	}

	//get source code
	readStream.open(sourceDir + "/" + sourceFile + ".quill");
	if (!readStream.is_open())
	{
		cout << "Source file could not be found or openned, terminating!\n";
		cin.get();
		return -1;
	}

	string source;
	readStream.seekg(0, ios::end);
	source.resize(readStream.tellg());
	readStream.seekg(0, ios::beg);
	readStream.read(&source[0], source.size());
	readStream.close();

	//parse into tokens
	vector<Tag> tags;
	tags.reserve(100);

	ParseTags(tags, source);

	//check for error tags
	for (Tag t : tags)
	{
		if (t.tagType == Error)
		{
			cout << "Compiler Error: " << t.attributes[0] << '\n';
			cin.get();
			return -1;
		}
	}

	//displays the tag type and text attribute
#ifdef Q_DEBUG

	for (Tag t : tags)
	{
		if (t.tagType == Div)
		{
			if (t.attributes[3] != "")
				cout << "Class: " << t.attributes[3] << '\n';

			else if (t.attributes[4] != "")
				cout << "Id: " << t.attributes[4] << '\n';
		}

		if (t.tagType == Image || t.tagType == Link)
			cout << " Link: " << t.attributes[10] << '\n';

		else
			cout << t.tagType << " Data: " << t.attributes[0] << '\n';
	}

#endif // Q_DEBUG


	//convert to HTML and write to file
	Convert(tags, buildDir);


#ifdef Q_DEBUG
	cin.get();
#endif // Q_DEBUG
	return 0;
}