simple Txc4
{
    parameters:
        // We define two parameters with default values
        bool sendMsgOnInit = default(false);
        int limit = default(2);
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

network Tictoc4
{
    @display("bgb=195,215");
    submodules:
        tic: Txc4 {
            parameters:
                sendMsgOnInit = true;   // Tic is configured to start
                @display("i=,cyan;p=33,146");
        }
        toc: Txc4 {
            parameters:
                sendMsgOnInit = false;  // Toc is configured to wait
                @display("i=,gold;p=136,34");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
