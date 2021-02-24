#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "task.h"

class Console: private Task {
public:
	static void create(Scheduler& scheduler);
private:
	Console(Scheduler& scheduler);
	void do_construct();
	void read();
	void dump_context(FILE* out) const;
private:
	InLine<20> line;
};

#endif //CONSOLE_HPP

