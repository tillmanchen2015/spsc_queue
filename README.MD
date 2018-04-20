###wrapper boost::lockfree::spsc_quque

```C++
spsc_queue<int, 3> q(10);	//10 means thread sleep 10 milliseconds. If it's 0, it's lock-free.
										//3 means queue capacity.

//producer code:
q.push(100);

//consumer code:
int v;
q.pop(v);
    

```