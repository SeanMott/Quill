#include <fstream>

#ifdef Q_DEBUG
#include <iostream>
#endif // Q_DEBUG

#include "Converter.h"

//converts Quill into HTML and writes to a file
void Convert(vector<Tag>& tags, string buildPath)
{
	//compiler states
	
	//React
	bool hasIncludedReact = false;
	bool devFlagState = false;
	string reactLibInclude = "";
	vector<string> reactComponents;

	//Head
	bool hasWrittenHead = false;

	//Div
	Tag divData;

	//Tag
	string tag = "";

	//creates file
	ofstream writeStream;
	writeStream.open(buildPath + "\\index.html");

	//write boiler plate html
	writeStream << "<!DOCTYPE html>\n";

	//if head is needed
	for (Tag t : tags)
	{
		if (t.tagType == IncludeCSS || t.tagType == IncludeJS || t.tagType == IncludeReact || t.tagType == DevFlag || t.tagType == Title)
		{
			if (!hasWrittenHead)
			{
				writeStream << "<head>\n\n";
				hasWrittenHead = true;
			}

			//includes CSS
			if (t.tagType == IncludeCSS)
				writeStream << "<link rel=\"stylesheet\" href=" + t.attributes[0] + ">\n";

			//includes javascript
			else if (t.tagType == IncludeJS)
				writeStream << "<script src=" + t.attributes[0] + "></script>\n";

			//includes title
			else if (t.tagType == Title)
				writeStream << "<title> " + t.attributes[0] + "</title>\n";

			//sets dev flag
			else if (t.tagType == DevFlag)
			{
				if (t.attributes[0].find("true") != string::npos)
					devFlagState = true;
			}

			//includes React
			else if (t.tagType == IncludeReact)
			{
				if (!hasIncludedReact)
					hasIncludedReact = true;

				reactComponents.emplace_back(t.attributes[0]);
			}
		}
	}

	//finishs head
	if (hasWrittenHead)
	{
		if (hasIncludedReact)
		{
			if (devFlagState)
			{
				writeStream << "\n<script src=\"https://unpkg.com/react@16/umd/react.development.js\" crossorigin></script>\n";
				writeStream << "<script src=\"https://unpkg.com/react-dom@16/umd/react-dom.development.js\" crossorigin></script>\n";
			}
			else
			{
				writeStream << "\n<script src=\"https://unpkg.com/react@16/umd/react.production.min.js\" crossorigin></script>\n";
				writeStream << "<script src=\"https://unpkg.com/react-dom@16/umd/react-dom.production.min.js\" crossorigin></script>\n";
			}
		}

		writeStream << "\n</head>\n\n";
	}

	writeStream << "<body>\n\n";

	unsigned int tagCount = tags.size();

	//goes through tag tokens
	for (unsigned int t = 0; t < tagCount; t++)
	{
		//Comment
		if (tags[t].tagType == Comment)
			writeStream << "<!--" + tags[t].attributes[0] + "-->\n";

		//Div Start
		else if (tags[t].tagType == DivOpen)
		{
			unsigned int divPos = 0;

			//get div
			for (unsigned int i = 0; i < tagCount; i++)
			{
				if (tags[i].tagType == Div)
				{
					divData = tags[i];
					divPos = i;
					break;
				}
			}

			string divName = "\n<div";

			WriteStyleAttributes(divName, divData);

			writeStream << divName + '\n';
			divData.Reset();
			tags[divPos].tagType = Null;
		}

		//Div end
		else if (tags[t].tagType == DivClosed)
			writeStream << "</div>\n\n";

		//Header
		else if (tags[t].tagType == Header)
		{
			string tagName = "h";

			//sets size
			if (tags[t].attributes[1] != "")
				tagName += tags[t].attributes[1];
			else
				tagName += '1';

			tag =  '<' + tagName;

			//sets style
			WriteStyleAttributes(tag, tags[t]);
		
			tag += tags[t].attributes[0] + "</" + tagName + ">\n";
			writeStream << tag;
		}
	
		//Paragraph
		else if (tags[t].tagType == Paragraph)
		{
			tag = "<p";

			//sets style
			WriteStyleAttributes(tag, tags[t]);

			tag += tags[t].attributes[0] + "</p>\n";
			writeStream << tag;
		}

		//Link
		else if (tags[t].tagType == Link)
			writeStream << "<a href = " + tags[t].attributes[10] +'>' + tags[t].attributes[0] + "</a>\n";

		//Image
		else if (tags[t].tagType == Image)
		{
			string tag = "<img src=" + tags[t].attributes[10] + " alt=\"" + tags[t].attributes[0] + "\" style=\"";

			if (tags[t].attributes[2] != "")
				tag += "width:" + tags[t].attributes[2] + "px; ";

			if (tags[t].attributes[1] != "")
				tag += "height:" + tags[t].attributes[1] + "px;";

			writeStream << tag + "\">\n";
		}
	}

	//finsish file
	writeStream << "\n</body>\n\n";

	//import React
	if (hasIncludedReact)
	{
		for (string comp : reactComponents)
		{
			writeStream << "<script src=" + comp + "></script>\n";
		}
	}

	writeStream << "\n</html>";
}

//writes the style attributes of a tag
void WriteStyleAttributes(string& tag, Tag& tagData)
{
	//class
	if (tagData.attributes[3] != "")
	{
		tag += " class=\"" + tagData.attributes[3] + "\">";
		return;
	}

	//id
	else if (tagData.attributes[4] != "")
	{
		tag += " id=\"" + tagData.attributes[4] + "\">";
		return;
	}

	//attributes
	else
	{
		bool hasWrittenStyle = false;
		for (unsigned int s = 5; s < AttributeLength; s++)
		{

			if (tagData.attributes[s] != "")
			{
				if (!hasWrittenStyle)
				{
					tag += " style=\"";
					hasWrittenStyle = true;
				}

				switch (s)
				{
					//fontSize
				case 5:
					tag += "font-size:" + tagData.attributes[5] + "; ";
					break;

					//fontFamily
				case 6:
					tag += "font-family:" + tagData.attributes[6] + "; ";
					break;

					//color
				case 7:
					tag += "color:" + tagData.attributes[7] + "; ";
					break;

					//background color
				case 8:
					tag += "background-color:" + tagData.attributes[8] + "; ";
					break;

					//horizontal align
				case 9:
					tag += "text-align:" + tagData.attributes[9] + "; ";
					break;
				}
			}
		}

		if (hasWrittenStyle)
			tag += '\"';

		tag += '>';
		//return;
	}
}