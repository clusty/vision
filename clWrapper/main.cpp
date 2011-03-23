#include <iostream>
#include <CLContext.h>

int main (int argc, char * const argv[]) {
    // insert code here...
	CLContext context(CLContext::ALL);
	context.buildProgramFromSource(  "__kernel void hello(void) { } ");
    return 0;
}
