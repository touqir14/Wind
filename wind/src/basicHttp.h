#ifndef BASICHTTP_HPP
#define BASICHTTP_HPP

#include "error.h"
#include "listener.h"
#include "console.h"
#include "http.h"
#include <string>
#include <vector>

#include <signal.h>
#include <unistd.h>
#include "valgrind/valgrind.h"

#include <cstdint>

namespace basicHttp {

std::string concat_str(std::vector<std::string> & strings);

#define BACKLOG 1000 // 100 is default

class BasicHttp: public HttpServerConnection {

public:
	static void factory(Scheduler& scheduler, FD accept_fd, responseMap_t & dict);

protected:
	BasicHttp(Scheduler& scheduler,FD accept_fd,responseMap_t & dict): HttpServerConnection(scheduler,accept_fd), count(0), response_dict(dict) {
	}
	void on_body(std::vector<std::string> & strings); 

private:
	int count;
	responseMap_t & response_dict;
};

std::string concat_str(std::vector<std::string> & strings);

auto create_dummy_endpoints();

void assign_uri_responses(responseMap_t & responses, Listener* listener);

int init(uint16_t port, Listener** listener_ptr, Scheduler** scheduler_ptr, std::string serverName = "HTTP");

int run_scheduler(Scheduler* scheduler, Listener* listener);

} // End of basicHttp namespace




#endif
