# bisect
A little command line utility to quickly extract lines from huge log files

Let's say you have a big log with timestamped data, like this:

```
2015-04-01T22:06:04.681Z 17850 TID-os8b9gfek ... 0.324 sec
2015-04-01T22:06:04.690Z 17850 TID-os8bci8c4 ... 0.33 sec
2015-04-01T22:06:04.824Z 17850 TID-os8ao0r4k ... 0.491 sec
2015-04-01T22:06:05.159Z 17850 TID-os8ao3qac ... 0.797 sec
2015-04-01T22:06:05.166Z 17850 TID-os8ao17bw ... 0.808 sec
2015-04-01T22:06:05.271Z 17850 TID-os8ao6ubo ... 0.908 sec
```

And you want to investigate something which occured 1h ago, you'll probably do something like `cat log | grep "2015-04-01T21:0"` to get the logs around and then refine with more grep but... oh wait... it's taking ages. And you'll suffer the same delay at each iteration unless you first copy the result to a temp file.

But as you may know, if the lines are properly sorted (which is probably the case) you can extract the range you're interested in way faster using [bisection](http://en.wikipedia.org/wiki/Bisection_method), here comes `bisect`:

```
bisect log "2015-04-01T21:0"
```

## How to build

    git clone http://github.com/jarthod/bisect
    cd bisect
    make

## TODO

- [ ] Benchmark seek vs mmap
- [ ] Refactor code
- [ ] Write specs