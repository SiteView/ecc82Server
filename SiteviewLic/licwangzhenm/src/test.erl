-module(test).
-compile(export_all).

start() -> test().  
stop() -> ok.
test()->
    license:start(),
    
    io:format("~p~n",[license:mac()]),
    io:format("~p~n",[license:match("1234567")]).

	