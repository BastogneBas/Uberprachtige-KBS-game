#ifndef SCHERM_H
#define SCHERM_H

class scherm
{
  public:
	//virtual Scherm()=0;
	//virtual ~Scherm()=0;

	// Loads and shows the screen
	virtual void begin();
	// Saves variables and clears the screen
	virtual void end();
	// Refreshes the current frame
	// Will be run in te infinite while loop
	virtual void refresh();
};
#endif
