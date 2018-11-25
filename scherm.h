#ifndef SCHERM_H
#define SCHERM_H

class scherm{
	public:
		//virtual Scherm()=0;
		//virtual ~Scherm()=0;
		virtual void begin();
		virtual void end();
		virtual void refresh();
};
#endif
