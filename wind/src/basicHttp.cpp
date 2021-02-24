// #ifndef BASICHTTP_CPP
// #define BASICHTTP_CPP

#include "basicHttp.h"
// #include "listener.h"
// #include "console.h"
// #include "http.h"
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
	// std::string output = concat_str(strings);
	// strings.clear();
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

// int main(int argc,char* argv[]) {
// 	int port = 42042;
// 	bool console = false, timeouts = true, logging = true;
// 	int opt;
// 	while((opt = getopt(argc,argv,"p:chzlr")) != -1) {
// 		switch(opt) {
// 		case 'p':
// 			port = atoi(optarg);
// 			if(port < 1 || port > 0xffff) {
// 				fprintf(stderr,"port out of bounds\n");
// 				return 1;
// 			}
// 			break;
// 		case 'c':
// 			console = true;
// 			break;
// 		case 'z':
// 			timeouts = false;
// 			break;
// 		case 'l':
// 			logging = false;
// 			break;
// 		case '?':
// 			if('p'==optopt)
// 				fprintf (stderr,"Option -%c requires an argument.\n",optopt);
// 			else if(32 < optopt)
// 				fprintf (stderr,"Unknown option `-%c'.\n",optopt);
// 			else
// 				fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
//              		return 1;
//              	default:
//              		fprintf(stderr,"unknown option %c\n",opt);
//              		// fall through
//              	case 'h':
// 			fprintf(stderr,"usage: ./helloworld {-p [port]} {-f [num]} {-c} {-z} {-l}\n"
// 				"  -c enables a console (so you can type \"quit\" for a clean shutdown in valgrind)\n"
// 				"  -z disables all timeouts (useful for test scripts or debugging clients)\n"
// 				"  -l disables logging to file (logging is turned off if running under valgrind)\n"
// 				"  -r enables rtmp on port+2 (experimental)\n");
// 			return 0;
// 		}
// 	}
// 	try {
// 		if(logging && !RUNNING_ON_VALGRIND)
// 			InitLog("helloworld.log");
// 		printf("=== Starting BasicHttp ===\n");
// 		Scheduler scheduler;
// 		if(!timeouts)
// 			scheduler.enable_timeouts(false);
// 		signal(SIGPIPE, SIG_IGN); // Ignoring SIGPIPE for now ??
// 		signal(SIGCHLD, SIG_IGN);
// 		if(console)
// 			Console::create(scheduler);
// 		Listener::create(scheduler,"HTTP",port,BasicHttp::factory,BACKLOG,true);
// 		scheduler.run();
// 	} catch(Error* e) {
// 		e->dump();
// 		e->release();
// 	} catch(std::exception& e) {
// 		fprintf(stderr,"%s",e.what());
// 	} catch(...) {
// 		fprintf(stderr,"unexpected exception!\n");
// 	}
// 	return 0;
// }

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
		// assign_uri_responses(responses, listener);
		// scheduler.run();
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
		// delete scheduler; // SHould you also cacll delete to listener??
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

// int main() {
// 	uint16_t port = 8888;
// 	auto responses = basicHttp::create_dummy_endpoints();
// 	basicHttp::init(port, responses);
// }




// #endif