simple Txc10
{
    parameters:
        @display("i=block/routing");
    gates:
        input in[];  // Vector gate (multiple inputs)
        output out[]; // Vector gate (multiple outputs)
}

network Tictoc10
{
    @display("bgb=173,193");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 100ms;
        }
    submodules:
        // Create 6 distinct nodes named tic[0], tic[1], ... tic[5]
        tic[6]: Txc10;
    connections:
        // Connect the specific layout manually
        // "++" means "add a new gate to the end of the list"
        tic[0].out++ --> Channel --> tic[1].in++;
        tic[0].in++ <-- Channel <-- tic[1].out++;

        tic[1].out++ --> Channel --> tic[2].in++;
        tic[1].in++ <-- Channel <-- tic[2].out++;

        tic[1].out++ --> Channel --> tic[4].in++;
        tic[1].in++ <-- Channel <-- tic[4].out++;

        tic[3].out++ --> Channel --> tic[4].in++;
        tic[3].in++ <-- Channel <-- tic[4].out++;

        tic[4].out++ --> Channel --> tic[5].in++;
        tic[4].in++ <-- Channel <-- tic[5].out++;
}
