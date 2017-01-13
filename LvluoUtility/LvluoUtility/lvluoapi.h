#ifndef LVLUOAPI_H
#define LVLUOAPI_H

#ifdef LVLUO_EXPORT  
#define LVLUO_API _declspec(dllexport)  
#else  
#define LVLUO_API _declspec(dllimport)  
#endif 

#endif // LVLUOAPI_H