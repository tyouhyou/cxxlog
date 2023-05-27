__*Refer to [wiki page](https://github.com/tyouhyou/cxx-toolset/wiki) for detail inforamtion of the utilities.*__

In this toolset, we have utilities such as Logger, Stopwatch, Decorator etc.

# _TOC_
- [Logger](#Logger)
- [Stopwatch](#Stopwatch)
- [Decorator](#Decorator)

# Logger
Just leave the log text, Logger will take care the rest of matter such as logging time, logging level, file name, line etc. As follows:

``` 
DL << "This is debug message. i=" << i;
IL << "For your information. x=" << foo << "; y=" << bar;
WL << "Caution!";
EL << "Error! => " << e.what();
```
Output is like
```outputting sample
[2020-05-03, 13:28:54][DEBUG][main.cpp(4)::main] This is debug message. i=2
[2020-05-03, 13:28:54][INFO ] For your information.
[2020-05-03, 13:28:54][WARN ] Caution!
[2020-05-03, 13:28:54][ERROR][main.cpp(7)::main] Error! => exception message.
```

# Stopwatch
To record a performance
```
sw.start();
// your code
auto dur = sw.elaspsed();
IL << "The duration from start is " << dur << "micro seconds.";
// another code
dur = sw.wrap();
IL << "The stopwatch return to zero. and the duration from start by now is " << dur << "micro seconds.";
// code
dur = sw.elaspsed();
IL << "The duration from wrap by now is " << dur << "micro seconds.";
```
And more convient recording api allowing us to recording duration between any two points.
```
sw.mark("first point");
// code
sw.mark("second point");
// code
sw.mark("third point");
dur1 = sw.measure("first point", "second point");
dur2 = sw.measure("first point", "third point");
dur3 = sw.measure("second point", "third point");
```

# Decorator
We can wrap a function to leave log before and after the function run. Sure, it is not just for logging. Do what you do in your python decorator.
```
auto fun = deco_func<int, int>()
  .wrap(some_function)
  .wrap_before([](int)->bool {IL << "before run function."; return true})
  .wrap_after([](int)->bool {IL << "before run function."; return true})
fun.call(100);
auto rst = fun.result();
```
