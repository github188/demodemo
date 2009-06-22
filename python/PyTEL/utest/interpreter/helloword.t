
string abc = "hello world";

void function sayHello(string world, integer times)
    print(world);
    print("times:%d,", world);
endfunction

void function main(string args[])

    sayHello(abc, 123);
    
endfunction