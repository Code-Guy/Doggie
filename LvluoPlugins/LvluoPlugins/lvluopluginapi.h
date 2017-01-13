#ifndef LVLUOPLUGINAPI_H
#define LVLUOPLUGINAPI_H

#ifdef LVLUOPLUGIN_EXPORT  
#define LVLUOPLUGIN_API _declspec(dllexport)  
#else  
#define LVLUOPLUGIN_API _declspec(dllimport)  
#endif 

#endif // LVLUOPLUGINAPI_H
