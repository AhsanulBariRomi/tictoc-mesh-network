// The Base Module
simple Txc6
{
    parameters:
        bool sendMsgOnInit = default(false); // Default: Don't start
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

// Specialized Tic: Always starts the process
simple Tic6 extends Txc6
{
    parameters:
        @class(Txc6); // Use the Txc6 C++ class
        @display("i=,cyan");
        sendMsgOnInit = true;
}

// Specialized Toc: Always waits
simple Toc6 extends Txc6
{
    parameters:
        @class(Txc6); // Use the Txc6 C++ class
        @display("i=,gold");
        sendMsgOnInit = false;
}

network Tictoc6
{
    @display("bgb=135,157");
    submodules:
        tic: Tic6 {
            @display("p=30,98");
        }
        toc: Toc6 {
            @display("p=86,34");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
