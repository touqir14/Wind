#include "basicHttp.h"
#include <string>
#include <vector>

#include <signal.h>
#include <unistd.h>
#include "valgrind/valgrind.h"

#include <cstdint>

namespace basicHttp {

void BasicHttp::factory(Scheduler& scheduler, FD accept_fd, responseMap_t & dict) {
	Cleanup<HttpServerConnection,CleanupClose> client(new BasicHttp(scheduler, accept_fd, dict));
	client->construct();
	client.detach();
}

void BasicHttp::on_body(std::vector<std::string> & strings) {
	count++;
	auto uri_str = std::string(this->uri);
	auto got = this->response_dict.find(uri_str);
	if (got != this->response_dict.end()) {
		writeHeader("Content-Length",std::to_string(got->second.size()).c_str());
		write(got->second.c_str());
	} else {
		writeHeader("Content-Length", std::to_string(strlen(HttpError::ENotFound)).c_str());
		writef(HttpError::ENotFound);
	}
	finish();
}

std::string concat_str(std::vector<std::string> & strings) {
	std::string output;
	for (auto s : strings) {
		output += (s + '\n');
	}
	return output;
}

auto create_dummy_endpoints() {
	responseMap_t dict;
	dict["/1"] = "Printing 1";
	dict["/2"] = "Printing 2";
	return dict;
}

void assign_uri_responses(responseMap_t & responses, Listener* listener) {
	for (auto& kv : responses) {
		listener->add_uri_response(kv.first, kv.second);
	}
}

int init(uint16_t port, Listener** listener_ptr, Scheduler** scheduler_ptr, std::string serverName = "HTTP") {
	bool init_success = -1;
	Listener* listener = NULL;
	Scheduler* scheduler = NULL;
	try {
		scheduler = new Scheduler();
		signal(SIGPIPE, SIG_IGN); // Ignoring SIGPIPE for now ??
		signal(SIGCHLD, SIG_IGN);
		listener = Listener::create(*scheduler,serverName.c_str(),port,BasicHttp::factory,BACKLOG,true);
		init_success = 0;
	} catch(Error* e) {
		e->dump();
		e->release();
	} catch(std::exception& e) {
		fprintf(stderr,"%s",e.what());
	} catch(...) {
		// fprintf(stderr,"unexpected exception!\n");
	}

	if (init_success != 0) {
		delete scheduler;
		delete listener;
		return -1;
	}
	
	*listener_ptr = listener;
	*scheduler_ptr = scheduler;
	return 0;
}

int run_scheduler(Scheduler* scheduler, Listener* listener) {
	try {
		scheduler->run();
	} catch(Error* e) {
		e->dump();
		e->release();
		return -1;
	} catch(std::exception& e) {
		fprintf(stderr,"%s",e.what());
		return -1;
	} catch(...) {
		// fprintf(stderr,"unexpected exception!\n");
		return -1;
	}
	return 0;
}

} // End of basicHttp namespace
