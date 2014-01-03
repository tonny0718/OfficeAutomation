#ifndef _EVENTS_H_
#define _EVENTS_H_

#include "ThreadPool.h"

void WINAPI OnLogin( PTP_CALLBACK_INSTANCE pInstance, void* packet);
void WINAPI OnSchedule( PTP_CALLBACK_INSTANCE pInstance, void* packet);
void WINAPI OnNote( PTP_CALLBACK_INSTANCE pInstance, void* packet);
void WINAPI OnSetDepartment( PTP_CALLBACK_INSTANCE pInstance, void* packet);
void WINAPI OnSetHuman( PTP_CALLBACK_INSTANCE pInstance, void* packet);
void WINAPI OnSendMessage( PTP_CALLBACK_INSTANCE pInstance, void* packet);
void WINAPI OnSendMail( PTP_CALLBACK_INSTANCE pInstance, void* packet);

#endif