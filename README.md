# Wind
This is a work-in-progress python http server library and not a production ready server yet. So far, it support Http 1.0, 1.1 and can simply fetch web pages. Even though **Wind** has a long way to go before becoming a fully fledged http server library like **Django**, it attains the highest performance amongst python server libraries in some of the benchmarks below.

## Benchmarks
The benchmarks were carried out in my linux work machine using **ApacheBench** under four scenarios as given below in the table. In every request, the servers respond with the string "Hello World". The table entries indicate the number of requests per second that each server can respond to and the bold entries indicate the fastest reading under that particular testing scenario. In almost all of the cases below, **Wind** outperforms all the other libraries significantly. The N/A below indicates timeout errors.

| Library     		| ```ab -n 50000``` | ```ab -n 100000 -c 30``` | ```ab -k -n 200000```| ```ab -k -n 400000 -c 30``` |
|-------------------|-------------|-------------------|---------------|----------------------|
| Wind     			| **21629.83** | 32753.86 | **65191.02** | **112788.88**  |
| Flask    			|  1602.98 |  1713.47 |  1569.49 |   1672.74  |
| Django   			|   884.80 |  1020.06 | N/A | N/A |
| Japronto          | 13286.20 | **34405.91** | 13207.65 | 33169.15   |
| Python HttpServer | 5523.79  | N/A | 5068.46 | N/A |
| Sanic    			| 5823.02  | 11156.12 | 13062.08 | 19631.69 |
| Tornado   		| 1838.15  | 2468.80 | 2300.07 | 2776.09 |
| Twisted 			| 2045.28  | 3344.92 | 2091.33 | 3019.14 |

## Installation
**Wind** is mostly coded using C++ and Python C API. Hence, a C++14 compliant compiler will be required along with Python C API header files. **Wind** supports Python 3.5+ and currently Linux systems. Enter ```python setup.py install``` in the terminal to install the library.

### TODO : Documentation.
