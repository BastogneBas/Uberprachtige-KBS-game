#ifndef SCREEN_H
#define SCREEN_H


class screen /// Defines the screen class
{
  public:
	//virtual Scherm()=0;
	virtual ~ screen() = default;

	// Loads and shows the screen
	virtual void begin();
	// Saves variables and clears the screen
	virtual void end();
	// Refreshes the current frame
	// Will be run in the infinite while loop
	virtual void refresh();
};
#endif
