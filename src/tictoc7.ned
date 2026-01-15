simple Txc7
{
    parameters:
        // "volatile" means: re-evaluate this every time it is read!
        // "@unit(s)" means: this value represents seconds
        volatile double delayTime @unit(s);
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

network Tictoc7
{
    @display("bgb=174,197");
    submodules:
        tic: Txc7 {
            parameters:
                @display("i=,cyan;p=32,160");
        }
        toc: Txc7 {
            parameters:
                @display("i=,gold;p=130,38");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
