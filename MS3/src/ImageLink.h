
#ifndef IMAGE_LINK_H
#define IMAGE_LINK_H

class ImageLink
{
public:
	ImageLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~ImageLink()
	{

	}

	ImageLink *next;
	ImageLink *prev;
};

#endif

// --- End of File --------------------------------------------------
