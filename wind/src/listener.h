#ifndef LISTENER_HPP
#define LISTENER_HPP

#include <unordered_map>
#include <string>
#include "task.h"

class Listener: private Task {
public:
	typedef void (*Factory)(Scheduler& scheduler, FD accept_fd, responseMap_t & dict);
	static Listener* create(Scheduler& scheduler,const char* name,short port,Factory factory,int backlog,bool reuse_addr=false);
	void add_uri_response(const std::string & key, const std::string & val);
	void add_uri_response(const char* key, size_t key_len, const char* val, size_t val_len);
	std::string pop_uri_response(const char* key, size_t key_len, bool & success);
	std::string pop_uri_response(const std::string & key, bool & success);

private:
	Listener(Scheduler& scheduler,const char* name,short port,Factory factory,int backlog,bool reuse_addr);
	void dump_context(FILE* out) const;
	void do_construct();
	void read();
private:
	const char* const name;
	const short port;
	const Factory factory;
	const int backlog;
	const bool reuse_addr;
	responseMap_t response_dict;
};

#endif //LISTENER_HPP

