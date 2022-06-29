#include "Mylogger.h"

int main()
{
	logInfo("The log is info message");   
	logError("The log is error message");
	logWarn("The log is warn message");
	logDebug("The log is debug message");
    Mylogger::destroy();
    return 0;
}
