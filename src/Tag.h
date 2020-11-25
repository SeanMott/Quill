//defines the tag data to be parsed

#ifndef TagHeader
#define TagHeader

#include <string>

enum TagType
{
	//Preprocesser tokens
	IncludeCSS, //for including external CSS
	IncludeJS, //for including external js
	IncludeReact, //for including external React components
	Title, //sets the title of the web page
	DevFlag, //tells the compiler if this website is in Debug or Release mainly affects React
	Error, //when the compiler encounters a error read out error message
	Null, //defines a token that exsists but has nothing in the data varable, gets ignored in convertion

	Header, //defines a header tag
	Paragraph, //defines a paragraph tag
	Link, //defines a link tag
	Image, //defines a image tag
	Comment, //defines a comment tag
	Div, //the data that makes up a div
	DivOpen, //defines a div being open
	DivClosed //defines a div being closed
};

#define AttributeLength 11

//defines a tag
struct Tag
{
	TagType tagType;

	std::string attributes[AttributeLength];
	/*
	0 = text || the text to be displayed or the alt of images
	1 = size/height || for headers it defines the number assoicated, on images it is height
	2 = width || image width
	3 = class || the style class being used
	4 = id || the style id being used
	5 = fontSize || the font size of text
	6 = fontFamily || the font family being used on text
	7 = color || the color of the element
	8 = backgroundColor || the background color of the element
	9 = horizontalAlign || specifies the horizontal alignment of text in an element
	10 = link || on links it is to a website, on images it is the image link
	*/

	//resets the data
	void Reset()
	{
		tagType = Null;

		for (unsigned char a = 0; a < AttributeLength; a++)
			attributes[a] = "";
	}
};

#endif