#ifndef SESSION_H
#define SESSION_H

void EndSession(ArgInfo* arg);
int StartSession(ArgInfo* arg);
void CleanupProc(ArgInfo* arg, const int argc);
int InitProc(ArgInfo* arg, const int argc, const char* argv[]);

#endif
