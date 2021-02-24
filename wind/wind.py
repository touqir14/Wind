from . import _wind
from pathlib import Path

class Server:
	def __init__(self, port=8080, name='server1'):
		self.port = port
		self.name = name
		self.endpoints = {}
		self.__server = _wind.create_server(port, name)

	def add_endpoint(self, endpoint, response_body):
		if type(endpoint) is not str:
			raise ValueError('endpoint must be a string')
		if type(response_body) is not str:
			raise ValueError('response_body must be a string')

		self.endpoints[endpoint] = response_body

	def del_endpoint(self, endpoint):
		if endpoint in self.endpoints:
			del self.endpoints[endpoint]

	def clear_endpoints(self):
		self.endpoints.clear()

	def start(self):
		if self.__server is None:
			self.__server = _wind.create_server(self.port, self.name)

		self.__server.set_responses(self.endpoints)
		self.__server.run_server()
		self.__server = None

	def add_directory(self, endpoint, path, filetypes, recursive=False):
		if endpoint[-1] != '/':
			endpoint += '/'
		if endpoint[0] != '/':
			endpoint = '/' + endpoint

		files = []
		for ftype in filetypes:
			if recursive:
				files += list(Path(path).rglob("*."+ftype))
			else:
				files += list(Path(path).glob("*."+ftype))

		for file in files:
			uri = endpoint + str(file.relative_to(path))
			self.add_endpoint(uri, file.read_text())


