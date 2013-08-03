#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
 


#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define SCREEN_DEPTH	16
#define NCOEF	(32)

int y_m=SCREEN_HEIGHT/2;
int divide=128;
bool redraw=true;
int loop=0;
int offset=0;
float zoom=1.0;
int quit;

char * filename;

int len=1024*1024;
Sint16 buffer[1024*1024];

SDL_Surface *screen=NULL;

bool left=false;
bool right=false;
bool up=false;
bool down=false;
bool pageup=false;
bool pagedown=false;

typedef struct
{
  double fs;	/* sampling freq */
  double Ts;	/* sampling period */
  double phi;	/* global phase (for fondamental oscillator) */
  
  double f;	/* fundamental freq */
  double coefs[NCOEF];	/* current oscillator coeficients */
	
  SDL_AudioSpec as,as2;	/* SDL audio stuff */
	
} Synth;



void init_video()
{
  if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
    {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
    }
  
  printf("SDL initialized.\n");
 
  screen = SDL_SetVideoMode(SCREEN_WIDTH, 
			    SCREEN_HEIGHT, 
			    SCREEN_DEPTH,			
			    SDL_SWSURFACE|SDL_DOUBLEBUF);
  
  if (screen == NULL)
    {
      fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
    }

}


void load_raw(char * filepath)
{

  struct stat fileStat;
  FILE * fd;

  if(stat(filepath,&fileStat) < 0)    
  
  printf("Information for %s\n",filepath);
  printf("---------------------------\n");
  printf("File Size: \t\t%d bytes\n",fileStat.st_size);
  printf("Number of Links: \t%d\n",fileStat.st_nlink);
  printf("File inode: \t\t%d\n",fileStat.st_ino);
  
  printf("File Permissions: \t");
  printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
  printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
  printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
  printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
  printf("\n\n");
  
  for (int i=0;i<len;i++)
    {
      buffer[i]=0;
    }

  fd=fopen(filepath,"r");
  for (int i=0;i<len;i++)
    {
      fread(buffer+i,sizeof(Sint16),1,fd);
    }
  fclose(fd);  
}


void handle_key()
{
  SDL_Event event;

  /* Events */
  //  SDL_WaitEvent(&event);

  //  while ( SDL_PollEvent(&event) )  
  //SDL_WaitEvent(&event);
  //  if (SDL_PollEvent(&event)==1)
  //if (SDL_PollEvent(&event))
  //  int current_sdl_time  = 0;
  
  //  current_sdl_time = SDL_GetTicks();
  if (SDL_PollEvent(&event))
    {
      //  while (SDL_WaitEvent(&event))
      //    {
      printf("loop %d\n",loop);
      loop++;
      switch (event.type)
	{
	case SDL_QUIT:
	  printf("Exiting...\n");
	  quit = 1;
	  exit(0);
	  break;
	  
	case SDL_KEYUP:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_LEFT:
	      left=false;
	      break;
	      
	    case SDLK_RIGHT:
	      right=false;
	      break;
	      
	    case SDLK_UP:
	      up=false;
	      break;
	      
	    case SDLK_DOWN:
	      down=false;
	      break;

	    case SDLK_PAGEUP:
	      pageup=false;
	      break;
	      
	    case SDLK_PAGEDOWN:
	      pagedown=false;
	      break;


	    default:
	      break;
	    }
	  break;
	  
	  
	  

      
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_ESCAPE:
	      printf("Exiting...\n");
	      quit = 1;
	      exit(0);
	      break;
	      
	    case SDLK_LEFT:
	      left=true;
	      break;
	      
	    case SDLK_RIGHT:
	      right=true;
	      break;
	      
	    case SDLK_UP:
	      up=true;
	      break;
	      
	    case SDLK_DOWN:
	      down=true;
	      break;	 	  
	      	     
	    case SDLK_PAGEUP:
	      pageup=true;
	      break;
	      
	    case SDLK_PAGEDOWN:
	      pagedown=true;
	      break;
	      
	    default:
	      break;
	      
	    }
	  break;

	}
      printf("new event:%d %s\n",event.type,SDL_GetKeyName(event.key.keysym.sym));
    }

  printf("No new event\n");

  
  if(up)
    {
    if (divide<=32) divide=32;
    divide=divide/1.1;
    redraw=true;
    printf("key up\n");
    }

  if(down)
    {
      if (divide>=4096) divide=4096;
      divide=divide*1.1;
      redraw=true;
      printf("key down\n");
    }

  if(left)
    {
      offset=offset-10;
      if (offset <=0) offset=0;
      redraw=true;
      printf("key left\n");            
    }
  if (right)
    {
      offset=offset+10;
      redraw=true;
      printf("key right\n");      
    }

  if(pageup)
    {
      zoom=zoom*1.1;
      if (zoom>1024.0) zoom=1024;
      redraw=true;
      printf("key pgup %f\n",zoom);      
    }

  if(pagedown)
    {
      zoom=zoom/1.1;
      if (zoom<0.01) zoom=0.01;
      redraw=true;
      printf("key pgdown %f\n",zoom);
    }


  //No user activity => sleeping a while to keep cpu cool
  //it keep cpu under 100% usage
  if ((up || down || left || right || pagedown || pageup)==false)
    {
      printf("sleeping 100ms");
      SDL_Delay(100);
    }
  else
    {
      printf("sleeping 10ms");
      SDL_Delay(10);
    }
}


void draw_screen()
{
  //Blank screen
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));         
       
  for (int i = 0; i < SCREEN_WIDTH-1 ; i++)
    {
      int j;
      Sint16 y_value;
      Sint16 y_value_positive;
      SDL_Rect rect;
      j=(i+offset)*zoom;        // j       => offset and zoomX from index i
      y_value=buffer[j]/divide; // y_value => zoomY from value j
	  
	  
      y_value_positive=(buffer[j]/128) % 255;
      if (y_value_positive < 0) y_value_positive=y_value_positive*-1;
      
      //printf("buf[i]=%d\n",buffer[i]);
      rect.x = i;
	  //rect.y=240;	 
	  
      if (y_value>=0) rect.y = y_m-y_value;
      if (y_value<0)  rect.y = y_m;
      
      rect.w = 1;
      if (buffer[j]<0) rect.h=-1*y_value;
      if (buffer[j]>0) rect.h=y_value;
      
	  // Create a vertical colored line
      SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, y_value_positive, 0, 128));
    }
  
  SDL_Flip(screen);
  redraw=false;
  
}



int main(int argc,char ** argv)
{	
  
  
  if (argc!=2) { printf("arg please\n"); exit(1); }
  
  struct stat fileStat;
  if(stat(argv[1],&fileStat) < 0)    
    return 1;

  filename=argv[1];
  load_raw(argv[1]);


  init_video();

  
  /* Main loop */
  quit = 0;
  while(!quit)
    {
      if (redraw==true)
	{	  
	  draw_screen();
	}
      else 
	{ 
	  handle_key(); 
	}
    }

  
  //free(synth);
  return 0;
}



















