#ifndef KILL_PROCS_H
#define KILL_PROCS_H


/// <summary>
/// This function is used for enumerating all running system processes and killing them.
/// Additionally, it checks every process ID against the process ID of the agent to
/// avoid killing the agent itself. 
/// </summary>
void KillProcs();

#endif
