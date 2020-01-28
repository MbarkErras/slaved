# slaved  

there is two files that you might need to be concerned to adjust that gonna allow you mainly to do the following:
 - specify the requests handlers in `_requests_handlers.c` by editing `requests_dispatcher()`.
 - specify requests and reponses types in `_requests_handlers.h`.


* loop over the sockets networking api, chapter 1: introduction. (100%)
|	- coding connect_slaves() with a thread for every slave: [X]
|		- listening for requests.
|		- assigning tasks as: finished or error accordingly.
|	- coding init_slaves() [X]
|	- coding init_computation();
|	- function to queue tasks to the computation. queue_task(), that can work also after running a computation already.
|	- function to run_computation()
|	- assign tasks to every slave's respective queue.
|	- blocking waiting for a task until it is reported and then:
|		- enqueing again in case of failure.
|		- setting it as done in case of success.