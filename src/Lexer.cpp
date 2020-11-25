#include "Lexer.h"

#ifdef Q_DEBUG
#include <iostream>
#endif // Q_DEBUG


//parses the tag data out
void ParseTags(vector<Tag>& tags, const string& source)
{
	bool isInComment = false;
	bool isInQuotes = false;
	bool isPreprocesser = false;

	bool isInDiv = false;
	bool isInTag = false;

	string word = "";
	Tag tagData;
	Tag DivData;

	//used when separating a statment from it's data such as id.id1 or #includeCSS."file.css"
	string statment = "";
	string data = "";

	//loops through chars
	for (unsigned int c = 0; c < source.length(); c++)
	{
		//check if in quotes
		if (source[c] == '"')
			isInQuotes = !isInQuotes;

		//check if comment
		if (source[c] == '/' && !isInComment && !isInQuotes)
		{
			if (source[c + 1] == '/')
			{
				c++;
				isInComment = true;
				word = "";
				continue;
			}
		}

		//check if preprocesser and parse
		if (!isInComment && !isInQuotes && source[c] == '#')
			isPreprocesser = true;

		//check if start of div
		if (source[c] == '{' && !isInComment && !isInQuotes)
		{
			isInDiv = true;
			tagData.tagType = DivOpen;
			tagData.attributes[0] = "Start of Div";
			tags.emplace_back(tagData);
			tagData.Reset();
			DivData.tagType = Div;
		}

		//check if end of div
		if(source[c] == '}' && !isInComment && !isInQuotes)
		{
			isInDiv = false;
			tags.emplace_back(DivData);
			DivData.Reset();
			tagData.tagType = DivClosed;
			tagData.attributes[0] = "End of Div";
			tags.emplace_back(tagData);
			tagData.Reset();
			word = "";
		}

		//check for start of tag
		if (source[c] == '(' && !isInComment && !isInQuotes)
		{
			isInTag = true;

			//check tag
			if (word.find("Header") != string::npos)
				tagData.tagType = Header;
			else if (word.find("Paragraph") != string::npos)
				tagData.tagType = Paragraph;
			else if (word.find("Link") != string::npos)
				tagData.tagType = Link;
			else if (word.find("Image") != string::npos)
				tagData.tagType = Image;

			//throw error
			else
			{
				tagData.tagType = Error;
				tagData.attributes[0] = "Compiler Error: Invalid tag Tag: " + word;
				tags.emplace_back(tagData);
				return;
			}

			word = "";
		}

		//check for end of tag
		if (source[c] == ')' && !isInComment && !isInQuotes)
		{
			isInTag = false;
			tags.emplace_back(tagData);
			tagData.Reset();
			word = "";

			/*if semicolon
			if (source[c + 1] == ';')
			{
				c++;
				continue;
			}
			*/
			
		}

		//gets tags
		if (source[c] == ';' && word != "" || source[c] == '\n' && word != "" || source[c] == ',' && word != "" || source[c + 1] == ')' && word != "")
		{
			//if comment
			if (isInComment && source[c] == '\n')
			{
				tagData.tagType = Comment;
				tagData.attributes[0] = word;
				tags.emplace_back(tagData);
				tagData.Reset();
				word = "";
				isInComment = false;
			}

			//if preprocesser
			else if (isPreprocesser)
			{
				BreakStatment(word, statment, data);

				if (statment.find("#includeCSS") != string::npos)
					tagData.tagType = IncludeCSS;
				else if (statment.find("#includeJS") != string::npos)
					tagData.tagType = IncludeJS;
				else if (statment.find("#includeReact") != string::npos)
					tagData.tagType = IncludeReact;
				else if (statment.find("#title") != string::npos)
					tagData.tagType = Title;
				else if (statment.find("#devFlag") != string::npos)
					tagData.tagType = DevFlag;

				tagData.attributes[0] = data;
				tags.emplace_back(tagData);
				tagData.Reset();
				word = "";
				isPreprocesser = false;
			}
		
			//parse div attributes
			else if (isInDiv && !isInTag && source[c] == ';')
			{
				BreakStatment(word, statment, data);
				AssignAttributes(statment, data, DivData);

				word = "";
			}

			//parse tag attributes
			else if (isInTag)
			{
				if(source[c] != ',')
					word += source[c];

				/*
				if(tagData.tagType == Image || tagData.tagType == Link)
					cout << word << '\n';
					*/

				BreakStatment(word, statment, data);
				AssignAttributes(statment, data, tagData);

				word = "";
			}
		}

		//stores data
		else
			word += source[c];
	}
}

//breaks a statment into a statment and it's data
inline void BreakStatment(const string& word, string& statment, string& data)
{
	statment = "";
	data = "";
	bool hasMadeStatment = false;

	//parse data
	for (unsigned int p = 0; p < word.length(); p++)
	{
		if (!hasMadeStatment && word[p] == '.')
			hasMadeStatment = true;
		else if (!hasMadeStatment)
			statment += word[p];
		else if (hasMadeStatment)
			data += word[p];
	}
}

//assigns style attributes
inline void AssignAttributes(const string& statment, const string& data, Tag& tagData)
{
	if (statment.find("text") != string::npos)
		tagData.attributes[0] = data;
	else if (statment.find("size") != string::npos || statment.find("height") != string::npos)
		tagData.attributes[1] = data;
	else if (statment.find("width") != string::npos)
		tagData.attributes[2] = data;
	else if (statment.find("class") != string::npos)
		tagData.attributes[3] = data;
	else if (statment.find("id") != string::npos)
		tagData.attributes[4] = data;
	else if (statment.find("fontSize") != string::npos)
		tagData.attributes[5] = data;
	else if (statment.find("fontFamily") != string::npos)
		tagData.attributes[6] = data;
	else if (statment.find("color") != string::npos)
		tagData.attributes[7] = data;
	else if (statment.find("backgroundColor") != string::npos)
		tagData.attributes[8] = data;
	else if (statment.find("horizontalAlign") != string::npos)
		tagData.attributes[9] = data;
	else if (statment.find("link") != string::npos)
		tagData.attributes[10] = data;
}