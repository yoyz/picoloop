
#include "DEBSystem.h"

int DEBSystem::MainLoop() {
} ;

void DEBSystem::Boot(int argc,char **argv) 
{
	System::Install(new DEBSystem()) ;
	FileSystem::Install(new UnixFileSystem()) ;

  // Install aliases

	char buff[1024];
	//ssize_t len = ::readlink("/proc/self/exe",buff,sizeof(buff)-1);
	ssize_t len=-1;
	if (len != -1)
	{
		buff[len] = 0;
	}
	else
	{
		strcpy(buff,".");
	}
	//Path::SetAlias("bin",dirname(buff)) ;
	Path::SetAlias("bin","bin") ;

	Path::SetAlias("root","bin:..") ;

	Trace::GetInstance()->SetLogger(*(new StdOutLogger()));

	Path logPath("bin:lgpt.log");
	Path::SetAlias("root","bin:..") ;
	Config::GetInstance()->ProcessArguments(argc,argv) ;

	// See if jack available
	 {
		Trace::Debug("Audio","Jack not found.. using default audio driver") ;
		AudioSettings hints ;
		hints.bufferSize_= 256 ;
		hints.preBufferCount_=2 ;
		//Audio::Install(new RTAudioStub(hints)) ;
		//MidiService::Install(new RTMidiService()) ;
	}
} ;

void DEBSystem::Shutdown() {
} ;

static int secbase=0 ;

unsigned long DEBSystem::GetClock() {

   struct timeval tp;

   gettimeofday(&tp, NULL);
   if (!secbase)
    {
        secbase = tp.tv_sec;
        return long(tp.tv_usec/1000.0);
     }
     return long((tp.tv_sec - secbase)*1000 + tp.tv_usec/1000.0);
}

void DEBSystem::Sleep(int millisec) {
/*	if (millisec>0)
		::Sleep(millisec) ;
*/}

void *DEBSystem::Malloc(unsigned size) {
	void *ptr=malloc(size) ;
//	Trace::Debug("alloc:%x  (%d)",ptr,size) ;
	return ptr ;
}

void DEBSystem::Free(void *ptr) {
//	Trace::Debug("free:%x",ptr) ;
	free(ptr) ;
} 

void DEBSystem::Memset(void *addr,char val,int size) {

    unsigned int ad=(unsigned int)addr ;
    if (((ad&0x3)==0)&&((size&0x3)==0)) { // Are we 4-byte aligned ?
        unsigned int intVal=0 ;
        for (int i=0;i<4;i++) {
             intVal=(intVal<<8)+val ;  
        }
        unsigned int *dst=(unsigned int *)addr ;
        size_t intSize=size>>2 ;

        for (unsigned int i=0;i<intSize;i++) {
            *dst++=intVal ;
        }
    } else {
        memset(addr,val,size) ;
    } ;
} ;

void *DEBSystem::Memcpy(void *s1, const void *s2, int n) {
    return memcpy(s1,s2,n) ;
} ;  

void DEBSystem::AddUserLog(const char *msg) {
	fprintf(stderr,"LOG: %s\n",msg) ;
};

void DEBSystem::PostQuitMessage() {
  //SDLEventManager::GetInstance()->PostQuitMessage() ;
} ; 

unsigned int DEBSystem::GetMemoryUsage() {
	return 0 ;
} ;
