#include "MDADrumMachine.h"

//#define SAM 512
#define SAM 64

//enum {
//  FMTYPE_2_OP_AM,
//  FMTYPE_2_OP_FM
//};

#define MDA_BANK_SIZE         25
#define MDA_BANK_TR808         0
#define MDA_BANK_TR909         1
#define MDA_BANK_TR606         2
#define MDA_BANK_TR77          3
#define MDA_BANK_CR8000        4
#define MDA_BANK_CR78          5
#define MDA_BANK_LINN          6
#define MDA_BANK_LATIN         7
#define MDA_BANK_ELECTRO       8
#define MDA_BANK_ACOUSTIC      9
#define MDA_BANK_FERRARO       10
#define MDA_BANK_FARFISA       11
#define MDA_BANK_INSTRMNT      12
#define MDA_BANK_MAGNETBOY     13
#define MDA_BANK_EFFECTS       14
#define MDA_BANK_JERGENSOHN    15
#define MDA_BANK_R_B           16
#define MDA_BANK_MISC          17
#define MDA_BANK_MISC_CLAPS    18
#define MDA_BANK_MISC_BASS     19
#define MDA_BANK_MISC_ELECTRO  20
#define MDA_BANK_MISC_FX       21
#define MDA_BANK_MISC_PERC     22
#define MDA_BANK_MISC_HATS     23
#define MDA_BANK_MISC_SYNTH    24

#define MDA_BANK_EFFECTS_SIZE       16
#define MDA_BANK_CR8000_SIZE        11
#define MDA_BANK_CR78_SIZE          11
#define MDA_BANK_LINN_SIZE          16
#define MDA_BANK_LATIN_SIZE         16
#define MDA_BANK_JERGENSOHN_SIZE    10
#define MDA_BANK_ELECTRO_SIZE       14
#define MDA_BANK_TR808_SIZE         15
#define MDA_BANK_ACOUSTIC_SIZE      12
#define MDA_BANK_FERRARO_SIZE        8
#define MDA_BANK_FARFISA_SIZE        9
#define MDA_BANK_INSTRMNT_SIZE      17
#define MDA_BANK_MAGNETBOY_SIZE     27
#define MDA_BANK_MISC_CLAPS_SIZE    12
#define MDA_BANK_MISC_BASS_SIZE     18
#define MDA_BANK_MISC_SIZE         201
#define MDA_BANK_MISC_ELECTRO_SIZE  60
#define MDA_BANK_MISC_FX_SIZE       83
#define MDA_BANK_MISC_PERC_SIZE     62
#define MDA_BANK_MISC_HATS_SIZE     72
#define MDA_BANK_TR77_SIZE           7
#define MDA_BANK_TR606_SIZE          7
#define MDA_BANK_R_B_SIZE           15
#define MDA_BANK_MISC_SYNTH_SIZE    25
#define MDA_BANK_TR909_SIZE         13


MDADrumMachine::MDADrumMachine() : filter(), dsoop()
{
  DPRINTF("MDADrumMachine::MDADrumMachine()\n");  
  buffer=0;
  cutoff=125;
  resonance=10;
  index=0;

  lfo_depth=0;
  lfo_depth_shift=20;
  lfo_speed=0;

  trig_time_mode=0;
  trig_time_duration=0;
  trig_time_duration_sample=0;

  need_note_on=0;
  buffer_size=0;
  note_on=0;
}


MDADrumMachine::~MDADrumMachine()
{
  DPRINTF("MDADrumMachine::~MDADrumMachine()\n");  
}


void MDADrumMachine::init()
{
  DPRINTF("MDADrumMachine::init()\n");  
  int i;

  filter.init();
  buffer = (Sint16*)malloc(sizeof(Sint16)*SAM*8);
  //HO(44100);
  // if (buffer==0)
  //   {
  //     //HO=new Hiopl(44100);
  //     HO=new Hiopl(4096);
  //     buffer = (Sint16*)malloc(sizeof(Sint16)*SAM*8);
  //   }

  // DPRINTF("buffer:0x%08.8X\n",buffer);
  // for (i=0;i<SAM;i++)
  //   buffer[i]=0;


}

const char * MDADrumMachine::getMachineTwoParamCharStar(int machineParam,int paramValue1,int paramValue2)
{

  static const char * str_null       = "NELL ";

 static const char * bank_cr8000_array[MDA_BANK_CR8000_SIZE];

 static const char * bank_cr8000_Clap                              = "Clap";                        
 static const char * bank_cr8000_Clave                             = "Clave";                       
 static const char * bank_cr8000_Conga_l                           = "Conga_l";                     
 static const char * bank_cr8000_Conga_m                           = "Conga_m";                     
 static const char * bank_cr8000_Cowbell                           = "Cowbell";                     
 static const char * bank_cr8000_Cymbal                            = "Cymbal";                      
 static const char * bank_cr8000_Hat_c                             = "Hat_c";                       
 static const char * bank_cr8000_Hat_o                             = "Hat_o";                       
 static const char * bank_cr8000_Kick                              = "Kick";                        
 static const char * bank_cr8000_Rim                               = "Rim";                         
 static const char * bank_cr8000_Snare                             = "Snare";                       

 bank_cr8000_array[0]  = bank_cr8000_Clap;               
 bank_cr8000_array[1]  = bank_cr8000_Clave;              
 bank_cr8000_array[2]  = bank_cr8000_Conga_l;            
 bank_cr8000_array[3]  = bank_cr8000_Conga_m;            
 bank_cr8000_array[4]  = bank_cr8000_Cowbell;            
 bank_cr8000_array[5]  = bank_cr8000_Cymbal;             
 bank_cr8000_array[6]  = bank_cr8000_Hat_c;              
 bank_cr8000_array[7]  = bank_cr8000_Hat_o;              
 bank_cr8000_array[8]  = bank_cr8000_Kick;               
 bank_cr8000_array[9]  = bank_cr8000_Rim;                
 bank_cr8000_array[10] = bank_cr8000_Snare;              




 static const char * bank_cr78_array[MDA_BANK_CR78_SIZE];

static const char * bank_cr78_Bongo_h                             = "Bongo_h";                     
static const char * bank_cr78_Bongo_l                             = "Bongo_l";                     
static const char * bank_cr78_Clave                               = "Clave";                       
static const char * bank_cr78_Conga                               = "Conga";                       
static const char * bank_cr78_Cymbal                              = "Cymbal";                      
static const char * bank_cr78_Hihat                               = "Hihat";                       
static const char * bank_cr78_Kick                                = "Kick";                        
static const char * bank_cr78_Maracas                             = "Maracas";                     
static const char * bank_cr78_Rim                                 = "Rim";                         
static const char * bank_cr78_Snare                               = "Snare";                       
static const char * bank_cr78_Tamb                                = "Tamb";                        


 bank_cr78_array[0]  = bank_cr78_Bongo_h;         
 bank_cr78_array[1]  = bank_cr78_Bongo_l;         
 bank_cr78_array[2]  = bank_cr78_Clave;           
 bank_cr78_array[3]  = bank_cr78_Conga;           
 bank_cr78_array[4]  = bank_cr78_Cymbal;          
 bank_cr78_array[5]  = bank_cr78_Hihat;           
 bank_cr78_array[6]  = bank_cr78_Kick;            
 bank_cr78_array[7]  = bank_cr78_Maracas;         
 bank_cr78_array[8]  = bank_cr78_Rim;             
 bank_cr78_array[9]  = bank_cr78_Snare;           
 bank_cr78_array[10] = bank_cr78_Tamb;            
                          


 static const char * bank_Linn_array[MDA_BANK_LINN_SIZE];

static const char * bank_Linn_linn_cab                            = "linn_cab";                    
static const char * bank_Linn_linn_cga1                           = "linn_cga1";                   
static const char * bank_Linn_linn_cga2                           = "linn_cga2";                   
static const char * bank_Linn_linn_cga3                           = "linn_cga3";                   
static const char * bank_Linn_linn_cgam                           = "linn_cgam";                   
static const char * bank_Linn_linn_clp                            = "linn_clp";                    
static const char * bank_Linn_linn_cow                            = "linn_cow";                    
static const char * bank_Linn_linn_kik                            = "linn_kik";                    
static const char * bank_Linn_linn_snr                            = "linn_snr";                    
static const char * bank_Linn_linn_stk                            = "linn_stk";                    
static const char * bank_Linn_linn_tamb                           = "linn_tamb";                   
static const char * bank_Linn_linn_tom1                           = "linn_tom1";                   
static const char * bank_Linn_linn_tom2                           = "linn_tom2";                   
static const char * bank_Linn_linn_tom3                           = "linn_tom3";                   
static const char * bank_Linn_linn_tom4                           = "linn_tom4";                   
static const char * bank_Linn_linn_tom5                           = "linn_tom5";                   


 bank_Linn_array[0]  = bank_Linn_linn_cab;            
 bank_Linn_array[1]  = bank_Linn_linn_cga1;           
 bank_Linn_array[2]  = bank_Linn_linn_cga2;           
 bank_Linn_array[3]  = bank_Linn_linn_cga3;           
 bank_Linn_array[4]  = bank_Linn_linn_cgam;           
 bank_Linn_array[5]  = bank_Linn_linn_clp;            
 bank_Linn_array[6]  = bank_Linn_linn_cow;            
 bank_Linn_array[7]  = bank_Linn_linn_kik;            
 bank_Linn_array[8]  = bank_Linn_linn_snr;            
 bank_Linn_array[9]  = bank_Linn_linn_stk;            
 bank_Linn_array[10] = bank_Linn_linn_tamb;           
 bank_Linn_array[11] = bank_Linn_linn_tom1;           
 bank_Linn_array[12] = bank_Linn_linn_tom2;           
 bank_Linn_array[13] = bank_Linn_linn_tom3;           
 bank_Linn_array[14] = bank_Linn_linn_tom4;           
 bank_Linn_array[15] = bank_Linn_linn_tom5;           

 static const char * bank_Latin_array[16];

static const char * bank_Latin_Bongo_h                            = "Bongo_h";                     
static const char * bank_Latin_bongo_l                            = "bongo_l";                     
static const char * bank_Latin_Bongo_m2                           = "Bongo_m2";                    
static const char * bank_Latin_Bongo_mu                           = "Bongo_mu";                    
static const char * bank_Latin_Clave_we                           = "Clave_we";                    
static const char * bank_Latin_Gamelan                            = "Gamelan";                     
static const char * bank_Latin_guiro                              = "guiro";                       
static const char * bank_Latin_Maracas                            = "Maracas";                     
static const char * bank_Latin_one_clap                           = "one_clap";                    
static const char * bank_Latin_Shaker_1                           = "Shaker_1";                    
static const char * bank_Latin_Shaker_2                           = "Shaker_2";                    
static const char * bank_Latin_taiko                              = "taiko";                       
static const char * bank_Latin_Tamb_hit                           = "Tamb_hit";                    
static const char * bank_Latin_Tamb_shk                           = "Tamb_shk";                    
static const char * bank_Latin_ThumbPno                           = "ThumbPno";                    
static const char * bank_Latin_Triangle                           = "Triangle";                    


 bank_Latin_array[0] = bank_Latin_Bongo_h;             
 bank_Latin_array[1] = bank_Latin_bongo_l;             
 bank_Latin_array[2] = bank_Latin_Bongo_m2;            
 bank_Latin_array[3] = bank_Latin_Bongo_mu;            
 bank_Latin_array[4] = bank_Latin_Clave_we;            
 bank_Latin_array[5] = bank_Latin_Gamelan;             
 bank_Latin_array[6] = bank_Latin_guiro;               
 bank_Latin_array[7] = bank_Latin_Maracas;             
 bank_Latin_array[8] = bank_Latin_one_clap;            
 bank_Latin_array[9] = bank_Latin_Shaker_1;            
 bank_Latin_array[10] = bank_Latin_Shaker_2;            
 bank_Latin_array[11] = bank_Latin_taiko;               
 bank_Latin_array[12] = bank_Latin_Tamb_hit;            
 bank_Latin_array[13] = bank_Latin_Tamb_shk;            
 bank_Latin_array[14] = bank_Latin_ThumbPno;            
 bank_Latin_array[15] = bank_Latin_Triangle;            

 static const char * bank_JergenSohn_array[MDA_BANK_JERGENSOHN_SIZE];

static const char * bank_JergenSohn_hihatopclo                    = "hihatopclo";                  
static const char * bank_JergenSohn_kick                          = "kick";                        
static const char * bank_JergenSohn_ramacymlong                   = "ramacymlong";                 
static const char * bank_JergenSohn_ramacymshort                  = "ramacymshort";                
static const char * bank_JergenSohn_ride6                         = "ride6";                       
static const char * bank_JergenSohn_snare5                        = "snare5";                      
static const char * bank_JergenSohn_vibra                         = "vibra";                       
static const char * bank_JergenSohn_wood2                         = "wood2";                       
static const char * bank_JergenSohn_wood                          = "wood";                        
static const char * bank_JergenSohn_zurrr                         = "zurrr";                       


 bank_JergenSohn_array[0] = bank_JergenSohn_hihatopclo;            
 bank_JergenSohn_array[1] = bank_JergenSohn_kick;                  
 bank_JergenSohn_array[2] = bank_JergenSohn_ramacymlong;           
 bank_JergenSohn_array[3] = bank_JergenSohn_ramacymshort;          
 bank_JergenSohn_array[4] = bank_JergenSohn_ride6;                 
 bank_JergenSohn_array[5] = bank_JergenSohn_snare5;                
 bank_JergenSohn_array[6] = bank_JergenSohn_vibra;                 
 bank_JergenSohn_array[7] = bank_JergenSohn_wood2;                 
 bank_JergenSohn_array[8] = bank_JergenSohn_wood;                  
 bank_JergenSohn_array[9] = bank_JergenSohn_zurrr;                 

 static const char * bank_Electro_array[MDA_BANK_ELECTRO_SIZE];

static const char * bank_Electro_Boom                             = "Boom";                        
static const char * bank_Electro_H_closed                         = "H_closed";                    
static const char * bank_Electro_Hi_q_2                           = "Hi_q_2";                      
static const char * bank_Electro_Hi_q                             = "Hi_q";                        
static const char * bank_Electro_H_open                           = "H_open";                      
static const char * bank_Electro_K_8bit                           = "K_8bit";                      
static const char * bank_Electro_K_Linn                           = "K_Linn";                      
static const char * bank_Electro_K_reverb                         = "K_reverb";                    
static const char * bank_Electro_S_8bit                           = "S_8bit";                      
static const char * bank_Electro_s_eq                             = "s_eq";                        
static const char * bank_Electro_S_Linn                           = "S_Linn";                      
static const char * bank_Electro_S_reverb                         = "S_reverb";                    
static const char * bank_Electro_Syntom_1                         = "Syntom_1";                    
static const char * bank_Electro_Syntom_2                         = "Syntom_2";                    

 bank_Electro_array[0]  = bank_Electro_Boom;           
 bank_Electro_array[1]  = bank_Electro_H_closed;       
 bank_Electro_array[2]  = bank_Electro_Hi_q_2;         
 bank_Electro_array[3]  = bank_Electro_Hi_q;           
 bank_Electro_array[4]  = bank_Electro_H_open;         
 bank_Electro_array[5]  = bank_Electro_K_8bit;         
 bank_Electro_array[6]  = bank_Electro_K_Linn;         
 bank_Electro_array[7]  = bank_Electro_K_reverb;       
 bank_Electro_array[8]  = bank_Electro_S_8bit;         
 bank_Electro_array[9]  = bank_Electro_s_eq;           
 bank_Electro_array[10] = bank_Electro_S_Linn;         
 bank_Electro_array[11] = bank_Electro_S_reverb;       
 bank_Electro_array[12] = bank_Electro_Syntom_1;       
 bank_Electro_array[13] = bank_Electro_Syntom_2;       








 static const char * bank_tr808_array[MDA_BANK_TR808_SIZE];

static const char * bank_tr808_Clave                              = "Clave";                       
static const char * bank_tr808_Cowbell                            = "Cowbell";                     
static const char * bank_tr808_Handclap                           = "Handclap";                    
static const char * bank_tr808_Hat_c                              = "Hat_c";                       
static const char * bank_tr808_Hat_o                              = "Hat_o";                       
static const char * bank_tr808_Kick                               = "Kick";                        
static const char * bank_tr808_Kickhard                           = "Kickhard";                    
static const char * bank_tr808_Kicklong                           = "Kicklong";                    
static const char * bank_tr808_Ride                               = "Ride";                        
static const char * bank_tr808_Rimshot                            = "Rimshot";                     
static const char * bank_tr808_Snare                              = "Snare";                       
static const char * bank_tr808_Snr_snap                           = "Snr_snap";                    
static const char * bank_tr808_Tom_hi                             = "Tom_hi";                      
static const char * bank_tr808_Tom_lo                             = "Tom_lo";                      
static const char * bank_tr808_Tom_mid                            = "Tom_mid";                     


 bank_tr808_array[0]  = bank_tr808_Clave;          
 bank_tr808_array[1]  = bank_tr808_Cowbell;        
 bank_tr808_array[2]  = bank_tr808_Handclap;       
 bank_tr808_array[3]  = bank_tr808_Hat_c;          
 bank_tr808_array[4]  = bank_tr808_Hat_o;          
 bank_tr808_array[5]  = bank_tr808_Kick;           
 bank_tr808_array[6]  = bank_tr808_Kickhard;       
 bank_tr808_array[7]  = bank_tr808_Kicklong;       
 bank_tr808_array[8]  = bank_tr808_Ride;           
 bank_tr808_array[9]  = bank_tr808_Rimshot;        
 bank_tr808_array[10] = bank_tr808_Snare;          
 bank_tr808_array[11] = bank_tr808_Snr_snap;       
 bank_tr808_array[12] = bank_tr808_Tom_hi;         
 bank_tr808_array[13] = bank_tr808_Tom_lo;         
 bank_tr808_array[14] = bank_tr808_Tom_mid;        

 static const char * bank_Acoustic_array[MDA_BANK_ACOUSTIC_SIZE];

static const char * bank_Acoustic_Brush1                          = "Brush1";                      
static const char * bank_Acoustic_Brush2                          = "Brush2";                      
static const char * bank_Acoustic_JazzKick                        = "JazzKick";                    
static const char * bank_Acoustic_JazzSnr                         = "JazzSnr";                     
static const char * bank_Acoustic_JazzSnr_H                       = "JazzSnr_H";                   
static const char * bank_Acoustic_Kick                            = "Kick";                        
static const char * bank_Acoustic_K_Muffle                        = "K_Muffle";                    
static const char * bank_Acoustic_Plain_Kick                      = "Plain_Kick";                  
static const char * bank_Acoustic_Ride2                           = "Ride2";                       
static const char * bank_Acoustic_Ride3                           = "Ride3";                       
static const char * bank_Acoustic_Ride                            = "Ride";                        
static const char * bank_Acoustic_Snare                           = "Snare";                       

 bank_Acoustic_array[0] = bank_Acoustic_Brush1;          
 bank_Acoustic_array[1] = bank_Acoustic_Brush2;          
 bank_Acoustic_array[2] = bank_Acoustic_JazzKick;        
 bank_Acoustic_array[3] = bank_Acoustic_JazzSnr;         
 bank_Acoustic_array[4] = bank_Acoustic_JazzSnr_H;       
 bank_Acoustic_array[5] = bank_Acoustic_Kick;            
 bank_Acoustic_array[6] = bank_Acoustic_K_Muffle;        
 bank_Acoustic_array[7] = bank_Acoustic_Plain_Kick;      
 bank_Acoustic_array[8] = bank_Acoustic_Ride2;           
 bank_Acoustic_array[9] = bank_Acoustic_Ride3;           
 bank_Acoustic_array[10] = bank_Acoustic_Ride;            
 bank_Acoustic_array[11] = bank_Acoustic_Snare;           


 static const char * bank_Ferraro_array[MDA_BANK_FERRARO_SIZE];


static const char * bank_Ferraro_curb_meat_bass                   = "curb_meat_bass";              
static const char * bank_Ferraro_eel_bass                         = "eel_bass";                    
static const char * bank_Ferraro_feed_this                        = "feed_this";                   
static const char * bank_Ferraro_gerbil_snare                     = "gerbil_snare";                
static const char * bank_Ferraro_grumpy_buzzard                   = "grumpy_buzzard";              
static const char * bank_Ferraro_jaguar_hats                      = "jaguar_hats";                 
static const char * bank_Ferraro_radio_hats                       = "radio_hats";                  
static const char * bank_Ferraro_worm_bass                        = "worm_bass";                   

 bank_Ferraro_array[0] = bank_Ferraro_curb_meat_bass;      
 bank_Ferraro_array[1] = bank_Ferraro_eel_bass;            
 bank_Ferraro_array[2] = bank_Ferraro_feed_this;           
 bank_Ferraro_array[3] = bank_Ferraro_gerbil_snare;        
 bank_Ferraro_array[4] = bank_Ferraro_grumpy_buzzard;      
 bank_Ferraro_array[5] = bank_Ferraro_jaguar_hats;         
 bank_Ferraro_array[6] = bank_Ferraro_radio_hats;          
 bank_Ferraro_array[7] = bank_Ferraro_worm_bass;           

 static const char * bank_Farfisa_array[MDA_BANK_FARFISA_SIZE];

static const char * bank_Farfisa_Fisa_Bongo                       = "Fisa_Bongo";                  
static const char * bank_Farfisa_Fisa_Cym                         = "Fisa_Cym";                    
static const char * bank_Farfisa_Fisa_Hat                         = "Fisa_Hat";                    
static const char * bank_Farfisa_Fisa_K_H                         = "Fisa_K_H";                    
static const char * bank_Farfisa_Fisa_Kick                        = "Fisa_Kick";                   
static const char * bank_Farfisa_Fisa_K_K                         = "Fisa_K_K";                    
static const char * bank_Farfisa_Fisa_S_B                         = "Fisa_S_B";                    
static const char * bank_Farfisa_Fisa_Snare                       = "Fisa_Snare";                  
static const char * bank_Farfisa_Fisa_Tom                         = "Fisa_Tom";                    

 bank_Farfisa_array[0] = bank_Farfisa_Fisa_Bongo;   
 bank_Farfisa_array[1] = bank_Farfisa_Fisa_Cym;     
 bank_Farfisa_array[2] = bank_Farfisa_Fisa_Hat;     
 bank_Farfisa_array[3] = bank_Farfisa_Fisa_K_H;     
 bank_Farfisa_array[4] = bank_Farfisa_Fisa_Kick;    
 bank_Farfisa_array[5] = bank_Farfisa_Fisa_K_K;     
 bank_Farfisa_array[6] = bank_Farfisa_Fisa_S_B;     
 bank_Farfisa_array[7] = bank_Farfisa_Fisa_Snare;   
 bank_Farfisa_array[8] = bank_Farfisa_Fisa_Tom;     

 static const char * bank_instrmnt_array[MDA_BANK_INSTRMNT_SIZE];

static const char * bank_instrmnt_E_Piano                         = "E_Piano";                     
static const char * bank_instrmnt_flute                           = "flute";                       
static const char * bank_instrmnt_FM_Bass                         = "FM_Bass";                     
static const char * bank_instrmnt_Moog_bass                       = "Moog_bass";                   
static const char * bank_instrmnt_mute_bass                       = "mute_bass";                   
static const char * bank_instrmnt_Oboe                            = "Oboe";                        
static const char * bank_instrmnt_pizzi                           = "pizzi";                       
static const char * bank_instrmnt_soft_piano                      = "soft_piano";                  
static const char * bank_instrmnt_square_lead                     = "square_lead";                 
static const char * bank_instrmnt_sweep                           = "sweep";                       
static const char * bank_instrmnt_techno_stab                     = "techno_stab";                 
static const char * bank_instrmnt_Temple_Bell                     = "Temple_Bell";                 
static const char * bank_instrmnt_tinkle                          = "tinkle";                      
static const char * bank_instrmnt_trumpet                         = "trumpet";                     
static const char * bank_instrmnt_upright_bass                    = "upright_bass";                
static const char * bank_instrmnt_violin                          = "violin";                      
static const char * bank_instrmnt_whistler                        = "whistler";                    

 bank_instrmnt_array[0] = bank_instrmnt_E_Piano;            
 bank_instrmnt_array[1] = bank_instrmnt_flute;              
 bank_instrmnt_array[2] = bank_instrmnt_FM_Bass;            
 bank_instrmnt_array[3] = bank_instrmnt_Moog_bass;          
 bank_instrmnt_array[4] = bank_instrmnt_mute_bass;          
 bank_instrmnt_array[5] = bank_instrmnt_Oboe;               
 bank_instrmnt_array[6] = bank_instrmnt_pizzi;              
 bank_instrmnt_array[7] = bank_instrmnt_soft_piano;         
 bank_instrmnt_array[8] = bank_instrmnt_square_lead;        
 bank_instrmnt_array[9] = bank_instrmnt_sweep;              
 bank_instrmnt_array[10] = bank_instrmnt_techno_stab;        
 bank_instrmnt_array[11] = bank_instrmnt_Temple_Bell;        
 bank_instrmnt_array[12] = bank_instrmnt_tinkle;             
 bank_instrmnt_array[13] = bank_instrmnt_trumpet;            
 bank_instrmnt_array[14] = bank_instrmnt_upright_bass;       
 bank_instrmnt_array[15] = bank_instrmnt_violin;             
 bank_instrmnt_array[16] = bank_instrmnt_whistler;           







 static const char * bank_magnetboy_array[MDA_BANK_MAGNETBOY_SIZE];

static const char * bank_magnetboy_Eye_Eye                        = "Eye_Eye";                     
static const char * bank_magnetboy_Fp1                            = "Fp1";                         
static const char * bank_magnetboy_Mist1                          = "Mist1";                       
static const char * bank_magnetboy_Mist2                          = "Mist2";                       
static const char * bank_magnetboy_Randion1                       = "Randion1";                    
static const char * bank_magnetboy_Squelb                         = "Squelb";                      
static const char * bank_magnetboy_Throaties1                     = "Throaties1";                  
static const char * bank_magnetboy_Tone1                          = "Tone1";                       
static const char * bank_magnetboy_Tone2                          = "Tone2";                       
static const char * bank_magnetboy_Tone3                          = "Tone3";                       
static const char * bank_magnetboy_Tone4                          = "Tone4";                       
static const char * bank_magnetboy_Tone5                          = "Tone5";                       
static const char * bank_magnetboy_Tone6                          = "Tone6";                       
static const char * bank_magnetboy_Tone7                          = "Tone7";                       
static const char * bank_magnetboy_Tone8                          = "Tone8";                       
static const char * bank_magnetboy_Tone9                          = "Tone9";                       
static const char * bank_magnetboy_Tone10                         = "Tone10";                      
static const char * bank_magnetboy_Tone11                         = "Tone11";                      
static const char * bank_magnetboy_Tone12                         = "Tone12";                      
static const char * bank_magnetboy_Tone14                         = "Tone14";                      
static const char * bank_magnetboy_Tone15                         = "Tone15";                      
static const char * bank_magnetboy_Tone16                         = "Tone16";                      
static const char * bank_magnetboy_Tone17                         = "Tone17";                      
static const char * bank_magnetboy_Tone18                         = "Tone18";                      
static const char * bank_magnetboy_Tone19                         = "Tone19";                      
static const char * bank_magnetboy_Tone20                         = "Tone20";                      
static const char * bank_magnetboy_Tone_float1                    = "Tone_float1";                 


 bank_magnetboy_array[0] = bank_magnetboy_Eye_Eye;             
 bank_magnetboy_array[1] = bank_magnetboy_Fp1;                 
 bank_magnetboy_array[2] = bank_magnetboy_Mist1;               
 bank_magnetboy_array[3] = bank_magnetboy_Mist2;               
 bank_magnetboy_array[4] = bank_magnetboy_Randion1;            
 bank_magnetboy_array[5] = bank_magnetboy_Squelb;              
 bank_magnetboy_array[6] = bank_magnetboy_Throaties1;          
 bank_magnetboy_array[7] = bank_magnetboy_Tone1;               
 bank_magnetboy_array[8] = bank_magnetboy_Tone2;               
 bank_magnetboy_array[9] = bank_magnetboy_Tone3;               
 bank_magnetboy_array[10] = bank_magnetboy_Tone4;               
 bank_magnetboy_array[11] = bank_magnetboy_Tone5;               
 bank_magnetboy_array[12] = bank_magnetboy_Tone6;               
 bank_magnetboy_array[13] = bank_magnetboy_Tone7;               
 bank_magnetboy_array[14] = bank_magnetboy_Tone8;               
 bank_magnetboy_array[15] = bank_magnetboy_Tone9;               
 bank_magnetboy_array[16] = bank_magnetboy_Tone10;              
 bank_magnetboy_array[17] = bank_magnetboy_Tone11;              
 bank_magnetboy_array[18] = bank_magnetboy_Tone12;              
 bank_magnetboy_array[19] = bank_magnetboy_Tone14;              
 bank_magnetboy_array[20] = bank_magnetboy_Tone15;              
 bank_magnetboy_array[21] = bank_magnetboy_Tone16;              
 bank_magnetboy_array[22] = bank_magnetboy_Tone17;              
 bank_magnetboy_array[23] = bank_magnetboy_Tone18;              
 bank_magnetboy_array[24] = bank_magnetboy_Tone19;              
 bank_magnetboy_array[25] = bank_magnetboy_Tone20;              
 bank_magnetboy_array[26] = bank_magnetboy_Tone_float1;         

 static const char * bank_misc_claps_array[MDA_BANK_MISC_CLAPS_SIZE];

static const char * bank_misc_claps_big_beat_clap                 = "big_beat_clap";               
static const char * bank_misc_claps_bright_clap                   = "bright_clap";                 
static const char * bank_misc_claps_clap                          = "clap";                        
static const char * bank_misc_claps_clap2                         = "clap2";                       
static const char * bank_misc_claps_clap_layer                    = "clap_layer";                  
static const char * bank_misc_claps_classic_clap                  = "classic_clap";                
static const char * bank_misc_claps_electro_clap                  = "electro_clap";                
static const char * bank_misc_claps_filter_clappy                 = "filter_clappy";               
static const char * bank_misc_claps_pitch_clap                    = "pitch_clap";                  
static const char * bank_misc_claps_pitch_clap_2                  = "pitch_clap_2";                
static const char * bank_misc_claps_shutter_clap                  = "shutter_clap";                
static const char * bank_misc_claps_u_don_t_kno                   = "u_don_t_kno";                 


 bank_misc_claps_array[0] = bank_misc_claps_big_beat_clap;         
 bank_misc_claps_array[1] = bank_misc_claps_bright_clap;           
 bank_misc_claps_array[2] = bank_misc_claps_clap;                  
 bank_misc_claps_array[3] = bank_misc_claps_clap2;                 
 bank_misc_claps_array[4] = bank_misc_claps_clap_layer;            
 bank_misc_claps_array[5] = bank_misc_claps_classic_clap;          
 bank_misc_claps_array[6] = bank_misc_claps_electro_clap;          
 bank_misc_claps_array[7] = bank_misc_claps_filter_clappy;         
 bank_misc_claps_array[8] = bank_misc_claps_pitch_clap;            
 bank_misc_claps_array[9] = bank_misc_claps_pitch_clap_2;          
 bank_misc_claps_array[10] = bank_misc_claps_shutter_clap;          
 bank_misc_claps_array[11] = bank_misc_claps_u_don_t_kno;           


 static const char * bank_misc_bass_array[MDA_BANK_MISC_BASS_SIZE];

static const char * bank_misc_bass_a_day_in_the_noise             = "a_day_in_the_noise";          
static const char * bank_misc_bass_bumble_bass                    = "bumble_bass";                 
static const char * bank_misc_bass_depth_bass_drum                = "depth_bass_drum";             
static const char * bank_misc_bass_dirty_a                        = "dirty_a";                     
static const char * bank_misc_bass_filter_idea_bd                 = "filter_idea_bd";              
static const char * bank_misc_bass_jarre_bass_drum                = "jarre_bass_drum";             
static const char * bank_misc_bass_jungle_laser                   = "jungle_laser";                
static const char * bank_misc_bass_low_end_hum                    = "low_end_hum";                 
static const char * bank_misc_bass_melodictone                    = "melodictone";                 
static const char * bank_misc_bass_metal_bass                     = "metal_bass";                  
static const char * bank_misc_bass_more_bass                      = "more_bass";                   
static const char * bank_misc_bass_more_bass_2                    = "more_bass_2";                 
static const char * bank_misc_bass_rubber                         = "rubber";                      
static const char * bank_misc_bass_smooth_bass                    = "smooth_bass";                 
static const char * bank_misc_bass_solid                          = "solid";                       
static const char * bank_misc_bass_synthbass                      = "synthbass";                   
static const char * bank_misc_bass_the_funk                       = "the_funk";                    
static const char * bank_misc_bass_warm_bass                      = "warm_bass";                   


 bank_misc_bass_array[0] = bank_misc_bass_a_day_in_the_noise;       
 bank_misc_bass_array[1] = bank_misc_bass_bumble_bass;              
 bank_misc_bass_array[2] = bank_misc_bass_depth_bass_drum;          
 bank_misc_bass_array[3] = bank_misc_bass_dirty_a;                  
 bank_misc_bass_array[4] = bank_misc_bass_filter_idea_bd;           
 bank_misc_bass_array[5] = bank_misc_bass_jarre_bass_drum;          
 bank_misc_bass_array[6] = bank_misc_bass_jungle_laser;             
 bank_misc_bass_array[7] = bank_misc_bass_low_end_hum;              
 bank_misc_bass_array[8] = bank_misc_bass_melodictone;              
 bank_misc_bass_array[9] = bank_misc_bass_metal_bass;               
 bank_misc_bass_array[10] = bank_misc_bass_more_bass;                
 bank_misc_bass_array[11] = bank_misc_bass_more_bass_2;              
 bank_misc_bass_array[12] = bank_misc_bass_rubber;                   
 bank_misc_bass_array[13] = bank_misc_bass_smooth_bass;              
 bank_misc_bass_array[14] = bank_misc_bass_solid;                    
 bank_misc_bass_array[15] = bank_misc_bass_synthbass;                
 bank_misc_bass_array[16] = bank_misc_bass_the_funk;                 
 bank_misc_bass_array[17] = bank_misc_bass_warm_bass;                

 static const char * bank_misc_array[MDA_BANK_MISC_SIZE];

static const char * bank_misc_AAH                                 = "AAH";                         
static const char * bank_misc_airbrush_bd                         = "airbrush_bd";                 
static const char * bank_misc_ambient_noise_snare                 = "ambient_noise_snare";         
static const char * bank_misc_amen_bd                             = "amen_bd";                     
static const char * bank_misc_amen_sn_1                           = "amen_sn_1";                   
static const char * bank_misc_and_loud_sn                         = "and_loud_sn";                 
static const char * bank_misc_another_909_moment                  = "another_909_moment";          
static const char * bank_misc_another_example_snare               = "another_example_snare";       
static const char * bank_misc_application_bass_drum               = "application_bass_drum";       
static const char * bank_misc_b1_0                                = "b1_0";                        
static const char * bank_misc_b1_2                                = "b1_2";                        
static const char * bank_misc_b1_5                                = "b1_5";                        
static const char * bank_misc_b1_9                                = "b1_9";                        
static const char * bank_misc_B1_B2                               = "B1_B2";                       
static const char * bank_misc_basic_808_click_hum                 = "basic_808_click_hum";         
static const char * bank_misc_battle_hook_sn                      = "battle_hook_sn";              
static const char * bank_misc_bauble_bd                           = "bauble_bd";                   
static const char * bank_misc_beat_box_bd                         = "beat_box_bd";                 
static const char * bank_misc_beefy_wack                          = "beefy_wack";                  
static const char * bank_misc_big_80_s                            = "big_80_s";                    
static const char * bank_misc_big_80_s_2                          = "big_80_s_2";                  
static const char * bank_misc_big_sleeper_snare                   = "big_sleeper_snare";           
static const char * bank_misc_blended_snare                       = "blended_snare";               
static const char * bank_misc_bouncy809bd                         = "bouncy809bd";                 
static const char * bank_misc_break_kick                          = "break_kick";                  
static const char * bank_misc_breathing_snare                     = "breathing_snare";             
static const char * bank_misc_bright_house_sn                     = "bright_house_sn";             
static const char * bank_misc_buff_hit                            = "buff_hit";                    
static const char * bank_misc_buff_sound                          = "buff_sound";                  
static const char * bank_misc_bug_into_water                      = "bug_into_water";              
static const char * bank_misc_bulge_n_yr_woofer                   = "bulge_n_yr_woofer";           
static const char * bank_misc_bumpin_quickie                      = "bumpin_quickie";              
static const char * bank_misc_calamity_snare                      = "calamity_snare";              
static const char * bank_misc_change_the_channel_snare            = "change_the_channel_snare";    
static const char * bank_misc_chow_bd                             = "chow_bd";                     
static const char * bank_misc_Clap                                = "Clap";                        
static const char * bank_misc_clappy_snare                        = "clappy_snare";                
static const char * bank_misc_clean_low_key_kick                  = "clean_low_key_kick";          
static const char * bank_misc_clean_soft_808                      = "clean_soft_808";              
static const char * bank_misc_clearly_snare                       = "clearly_snare";               
static const char * bank_misc_close_mic_sn                        = "close_mic_sn";                
static const char * bank_misc_compression_does_it_808             = "compression_does_it_808";     
static const char * bank_misc_crusty_bump                         = "crusty_bump";                 
static const char * bank_misc_crusty_snare                        = "crusty_snare";                
static const char * bank_misc_cymbal                              = "cymbal";                      
static const char * bank_misc_decompression_snare                 = "decompression_snare";         
static const char * bank_misc_deep_bauble_bd                      = "deep_bauble_bd";              
static const char * bank_misc_deep_tones                          = "deep_tones";                  
static const char * bank_misc_difference_snare                    = "difference_snare";            
static const char * bank_misc_different_909_snare                 = "different_909_snare";         
static const char * bank_misc_dirty_world_bd                      = "dirty_world_bd";              
static const char * bank_misc_Distant_Thunder                     = "Distant_Thunder";             
static const char * bank_misc_dist_sub_kick                       = "dist_sub_kick";               
static const char * bank_misc_dist_sub_kick_2                     = "dist_sub_kick_2";             
static const char * bank_misc_electro_dnb_kick                    = "electro_dnb_kick";            
static const char * bank_misc_electronic_rim                      = "electronic_rim";              
static const char * bank_misc_electro_snare_layer                 = "electro_snare_layer";         
static const char * bank_misc_end_transmission                    = "end_transmission";            
static const char * bank_misc_every_909                           = "every_909";                   
static const char * bank_misc_fatness                             = "fatness";                     
static const char * bank_misc_fattie_bd                           = "fattie_bd";                   
static const char * bank_misc_feel_me_                            = "feel_me_";                    
static const char * bank_misc_few_people_will_use_this_snare      = "few_people_will_use_this_snare"; 
static const char * bank_misc_flat_tire                           = "flat_tire";                   
static const char * bank_misc_fm                                  = "fm";                          
static const char * bank_misc_fried_food                          = "fried_food";                  
static const char * bank_misc_funky_trashcan_snare                = "funky_trashcan_snare";        
static const char * bank_misc_fuzzy_sn                            = "fuzzy_sn";                    
static const char * bank_misc_gabbakick                           = "gabbakick";                   
static const char * bank_misc_gimme_a_hard_tone                   = "gimme_a_hard_tone";           
static const char * bank_misc_gimmie_a_tone                       = "gimmie_a_tone";               
static const char * bank_misc_hardcore_a_bd                       = "hardcore_a_bd";               
static const char * bank_misc_hardcore_bd                         = "hardcore_bd";                 
static const char * bank_misc_hard_electro_brush_sn               = "hard_electro_brush_sn";       
static const char * bank_misc_hard_electronic_snare               = "hard_electronic_snare";       
static const char * bank_misc_hardersnare                         = "hardersnare";                 
static const char * bank_misc_hard_hittin                         = "hard_hittin";                 
static const char * bank_misc_heady_mix_sn                        = "heady_mix_sn";                
static const char * bank_misc_heavy                               = "heavy";                       
static const char * bank_misc_hed_chunk_snare                     = "hed_chunk_snare";             
static const char * bank_misc_hi_pass                             = "hi_pass";                     
static const char * bank_misc_hip_hop_fantasy_kick                = "hip_hop_fantasy_kick";        
static const char * bank_misc_hi_pic                              = "hi_pic";                      
static const char * bank_misc_hipster_joint_bd                    = "hipster_joint_bd";            
static const char * bank_misc_hit_me_wooden_snare                 = "hit_me_wooden_snare";         
static const char * bank_misc_hitthosesubs                        = "hitthosesubs";                
static const char * bank_misc_hot_morning_bass_drum               = "hot_morning_bass_drum";       
static const char * bank_misc_HOTSNARE                            = "HOTSNARE";                    
static const char * bank_misc_i_luv_u_fat_noise                   = "i_luv_u_fat_noise";           
static const char * bank_misc_invisible_909_kick                  = "invisible_909_kick";          
static const char * bank_misc_it_s_like_that_snare                = "it_s_like_that_snare";        
static const char * bank_misc_just_the_bass_drum                  = "just_the_bass_drum";          
static const char * bank_misc_kneel_and_buzz                      = "kneel_and_buzz";              
static const char * bank_misc_large_hall_bd                       = "large_hall_bd";               
static const char * bank_misc_laughter_and_jellybeans_snare       = "laughter_and_jellybeans_snare"; 
static const char * bank_misc__lectro_hammer                      = "_lectro_hammer";              
static const char * bank_misc__lectro_square_hammer               = "_lectro_square_hammer";       
static const char * bank_misc_light_snare                         = "light_snare";                 
static const char * bank_misc_lo_fi_bass_drum                     = "lo_fi_bass_drum";             
static const char * bank_misc_lo_fi_march                         = "lo_fi_march";                 
static const char * bank_misc_lo_fi_rim                           = "lo_fi_rim";                   
static const char * bank_misc_loudness_sn                         = "loudness_sn";                 
static const char * bank_misc_lovely_clean_bd                     = "lovely_clean_bd";             
static const char * bank_misc_lower_electro_bd                    = "lower_electro_bd";            
static const char * bank_misc_mack_bd                             = "mack_bd";                     
static const char * bank_misc_manipulate_bd                       = "manipulate_bd";               
static const char * bank_misc_many_frequencies                    = "many_frequencies";            
static const char * bank_misc_mello_bd                            = "mello_bd";                    
static const char * bank_misc_mind_ya_h                           = "mind_ya_h";                   
static const char * bank_misc_missing_snare                       = "missing_snare";               
static const char * bank_misc_missing_snare_2                     = "missing_snare_2";             
static const char * bank_misc_more_basic_bd                       = "more_basic_bd";               
static const char * bank_misc_more_snap                           = "more_snap";                   
static const char * bank_misc_muted_snare                         = "muted_snare";                 
static const char * bank_misc_neo808                              = "neo808";                      
static const char * bank_misc_neo808_2                            = "neo808_2";                    
static const char * bank_misc_n_hi                                = "n_hi";                        
static const char * bank_misc_N_low                               = "N_low";                       
static const char * bank_misc_n_mid                               = "n_mid";                       
static const char * bank_misc_noise                               = "noise";                       
static const char * bank_misc_noise_calling_kick                  = "noise_calling_kick";          
static const char * bank_misc_noise_clip                          = "noise_clip";                  
static const char * bank_misc_nosie_calling_kick_2                = "nosie_calling_kick_2";        
static const char * bank_misc_N_vlow                              = "N_vlow";                      
static const char * bank_misc_odd_snare                           = "odd_snare";                   
static const char * bank_misc_original_tone_snare                 = "original_tone_snare";         
static const char * bank_misc_ot_12                               = "ot_12";                       
static const char * bank_misc_ot_34                               = "ot_34";                       
static const char * bank_misc_ot_sine                             = "ot_sine";                     
static const char * bank_misc_overdriver_kick                     = "overdriver_kick";             
static const char * bank_misc_pleasure_factory_kick               = "pleasure_factory_kick";       
static const char * bank_misc_pop_1_off_snare                     = "pop_1_off_snare";             
static const char * bank_misc_power_out                           = "power_out";                   
static const char * bank_misc_psychoacoutic_bd                    = "psychoacoutic_bd";            
static const char * bank_misc_punchy_clean_kick                   = "punchy_clean_kick";           
static const char * bank_misc_puppy_kick                          = "puppy_kick";                  
static const char * bank_misc_q_ey_bass_drum                      = "q_ey_bass_drum";              
static const char * bank_misc_q_kick_2                            = "q_kick_2";                    
static const char * bank_misc_questoin_kick                       = "questoin_kick";               
static const char * bank_misc_r_b_fantasy_kick                    = "r_b_fantasy_kick";            
static const char * bank_misc_reverb_please_snare                 = "reverb_please_snare";         
static const char * bank_misc_rimmy_noise                         = "rimmy_noise";                 
static const char * bank_misc_RimShot                             = "RimShot";                     
static const char * bank_misc_rimshot_1                           = "rimshot_1";                   
static const char * bank_misc_rimshot_2                           = "rimshot_2";                   
static const char * bank_misc_ring_fray_kick                      = "ring_fray_kick";              
static const char * bank_misc_ring_mod_drone_bd                   = "ring_mod_drone_bd";           
static const char * bank_misc_rm                                  = "rm";                          
static const char * bank_misc_rubber__82                          = "rubber__82";                  
static const char * bank_misc_rubbery_deep_house_kick             = "rubbery_deep_house_kick";     
static const char * bank_misc_Saber_fight                         = "Saber_fight";                 
static const char * bank_misc_serious_bd                          = "serious_bd";                  
static const char * bank_misc_short_909                           = "short_909";                   
static const char * bank_misc_simon_s_snare                       = "simon_s_snare";               
static const char * bank_misc_Simple_c                            = "Simple_c";                    
static const char * bank_misc_simple_click_bd                     = "simple_click_bd";             
static const char * bank_misc_simple_h                            = "simple_h";                    
static const char * bank_misc_simple_k                            = "simple_k";                    
static const char * bank_misc_Simple_r                            = "Simple_r";                    
static const char * bank_misc_Simple_s                            = "Simple-s";                    
static const char * bank_misc_skiwlz_snare                        = "skiwlz_snare";                
static const char * bank_misc_slepper_n_the_wilderness            = "slepper_n_the_wilderness";    
static const char * bank_misc_sloppy_electric                     = "sloppy_electric";             
static const char * bank_misc_smooth_rim                          = "smooth_rim";                  
static const char * bank_misc_snappy_809                          = "snappy_809";                  
static const char * bank_misc_softsnare                           = "softsnare";                   
static const char * bank_misc_sou_like_a_low_tom                  = "sou_like_a_low_tom";          
static const char * bank_misc_sounding_kick                       = "sounding_kick";               
static const char * bank_misc_special_man_bass_drum               = "special_man_bass_drum";       
static const char * bank_misc_splatter                            = "splatter";                    
static const char * bank_misc_sticky_q_kick                       = "sticky_q_kick";               
static const char * bank_misc_stiffy_snare                        = "stiffy_snare";                
static const char * bank_misc_sweet_tight_snare                   = "sweet_tight_snare";           
static const char * bank_misc_tchak_snare                         = "tchak_snare";                 
static const char * bank_misc_terror_n_yr_hood_snare              = "terror_n_yr_hood_snare";      
static const char * bank_misc_test                                = "test";                        
static const char * bank_misc_think_bd                            = "think_bd";                    
static const char * bank_misc_think_sn_1                          = "think_sn_1";                  
static const char * bank_misc_think_sn_2                          = "think_sn_2";                  
static const char * bank_misc_this_snare_spells_electronic        = "this_snare_spells_electronic"; 
static const char * bank_misc_tomita_clip                         = "tomita_clip";                 
static const char * bank_misc_tone                                = "tone";                        
static const char * bank_misc_tone_deg                            = "tone_deg";                    
static const char * bank_misc_Tone_dis                            = "Tone_dis";                    
static const char * bank_misc_tone_snare                          = "tone_snare";                  
static const char * bank_misc_Tone_sw2                            = "Tone_sw2";                    
static const char * bank_misc_Tone_swp                            = "Tone_swp";                    
static const char * bank_misc_transient_snare                     = "transient_snare";             
static const char * bank_misc_trashyovertones                     = "trashyovertones";             
static const char * bank_misc_type_snare                          = "type_snare";                  
static const char * bank_misc_ultra_electro_bd                    = "ultra_electro_bd";            
static const char * bank_misc_usgae                               = "usgae";                       


bank_misc_array[0] = bank_misc_AAH;                        
bank_misc_array[1] = bank_misc_airbrush_bd;                
bank_misc_array[2] = bank_misc_ambient_noise_snare;        
bank_misc_array[3] = bank_misc_amen_bd;                    
bank_misc_array[4] = bank_misc_amen_sn_1;                  
bank_misc_array[5] = bank_misc_and_loud_sn;                
bank_misc_array[6] = bank_misc_another_909_moment;         
bank_misc_array[7] = bank_misc_another_example_snare;      
bank_misc_array[8] = bank_misc_application_bass_drum;      
bank_misc_array[9] = bank_misc_b1_0;
 
bank_misc_array[10] = bank_misc_b1_2;                       
bank_misc_array[11] = bank_misc_b1_5;                       
bank_misc_array[12] = bank_misc_b1_9;                       
bank_misc_array[13] = bank_misc_B1_B2;                      
bank_misc_array[14] = bank_misc_basic_808_click_hum;        
bank_misc_array[15] = bank_misc_battle_hook_sn;             
bank_misc_array[16] = bank_misc_bauble_bd;                  
bank_misc_array[17] = bank_misc_beat_box_bd;                
bank_misc_array[18] = bank_misc_beefy_wack;                 
bank_misc_array[19] = bank_misc_big_80_s;
 
bank_misc_array[20] = bank_misc_big_80_s_2;                 
bank_misc_array[21] = bank_misc_big_sleeper_snare;          
bank_misc_array[22] = bank_misc_blended_snare;              
bank_misc_array[23] = bank_misc_bouncy809bd;                
bank_misc_array[24] = bank_misc_break_kick;                 
bank_misc_array[25] = bank_misc_breathing_snare;            
bank_misc_array[26] = bank_misc_bright_house_sn;            
bank_misc_array[27] = bank_misc_buff_hit;                   
bank_misc_array[28] = bank_misc_buff_sound;                 
bank_misc_array[29] = bank_misc_bug_into_water;
 
bank_misc_array[30] = bank_misc_bulge_n_yr_woofer;          
bank_misc_array[31] = bank_misc_bumpin_quickie;             
bank_misc_array[32] = bank_misc_calamity_snare;             
bank_misc_array[33] = bank_misc_change_the_channel_snare;   
bank_misc_array[34] = bank_misc_chow_bd;                    
bank_misc_array[35] = bank_misc_Clap;                       
bank_misc_array[36] = bank_misc_clappy_snare;               
bank_misc_array[37] = bank_misc_clean_low_key_kick;         
bank_misc_array[38] = bank_misc_clean_soft_808;             
bank_misc_array[39] = bank_misc_clearly_snare;
 
bank_misc_array[40] = bank_misc_close_mic_sn;                       
bank_misc_array[41] = bank_misc_compression_does_it_808;            
bank_misc_array[42] = bank_misc_crusty_bump;                        
bank_misc_array[43] = bank_misc_crusty_snare;                       
bank_misc_array[44] = bank_misc_cymbal;                             
bank_misc_array[45] = bank_misc_decompression_snare;                
bank_misc_array[46] = bank_misc_deep_bauble_bd;                     
bank_misc_array[47] = bank_misc_deep_tones;                         
bank_misc_array[48] = bank_misc_difference_snare;                   
bank_misc_array[49] = bank_misc_different_909_snare;
  
bank_misc_array[50] = bank_misc_fattie_bd;
bank_misc_array[51] = bank_misc_feel_me_;
bank_misc_array[52] = bank_misc_few_people_will_use_this_snare;     
bank_misc_array[53] = bank_misc_flat_tire;                          
bank_misc_array[54] = bank_misc_fm;                                 
bank_misc_array[55] = bank_misc_fried_food;                         
bank_misc_array[56] = bank_misc_funky_trashcan_snare;               
bank_misc_array[57] = bank_misc_fuzzy_sn;                           
bank_misc_array[58] = bank_misc_gabbakick;                          
bank_misc_array[59] = bank_misc_gimme_a_hard_tone;
 
bank_misc_array[60] = bank_misc_gimmie_a_tone;                      
bank_misc_array[61] = bank_misc_hardcore_a_bd;                      
bank_misc_array[62] = bank_misc_hardcore_bd;                        
bank_misc_array[63] = bank_misc_hard_electro_brush_sn;              
bank_misc_array[64] = bank_misc_hard_electronic_snare;              
bank_misc_array[65] = bank_misc_hardersnare;                        
bank_misc_array[66] = bank_misc_hard_hittin;                        
bank_misc_array[67] = bank_misc_heady_mix_sn;                       
bank_misc_array[68] = bank_misc_heavy;                              
bank_misc_array[69] = bank_misc_hed_chunk_snare;
 
bank_misc_array[70] = bank_misc_hi_pass;                            
bank_misc_array[71] = bank_misc_hip_hop_fantasy_kick;               
bank_misc_array[72] = bank_misc_hi_pic;                             
bank_misc_array[73] = bank_misc_hipster_joint_bd;                   
bank_misc_array[74] = bank_misc_hit_me_wooden_snare;                
bank_misc_array[75] = bank_misc_hitthosesubs;                       
bank_misc_array[76] = bank_misc_hot_morning_bass_drum;              
bank_misc_array[77] = bank_misc_HOTSNARE;                           
bank_misc_array[78] = bank_misc_i_luv_u_fat_noise;                  
bank_misc_array[79] = bank_misc_invisible_909_kick;
 
bank_misc_array[80] = bank_misc_it_s_like_that_snare;               
bank_misc_array[81] = bank_misc_just_the_bass_drum;                 
bank_misc_array[82] = bank_misc_kneel_and_buzz;                     
bank_misc_array[83] = bank_misc_large_hall_bd;                      
bank_misc_array[84] = bank_misc_laughter_and_jellybeans_snare;      
bank_misc_array[85] = bank_misc__lectro_hammer;                     
bank_misc_array[86] = bank_misc__lectro_square_hammer;              
bank_misc_array[87] = bank_misc_light_snare;                        
bank_misc_array[88] = bank_misc_lo_fi_bass_drum;                    
bank_misc_array[89] = bank_misc_lo_fi_march;
 
bank_misc_array[90] = bank_misc_lo_fi_rim;                          
bank_misc_array[91] = bank_misc_loudness_sn;                        
bank_misc_array[92] = bank_misc_lovely_clean_bd;                    
bank_misc_array[93] = bank_misc_lower_electro_bd;                   
bank_misc_array[94] = bank_misc_mack_bd;                            
bank_misc_array[95] = bank_misc_manipulate_bd;                      
bank_misc_array[96] = bank_misc_many_frequencies;                   
bank_misc_array[97] = bank_misc_mello_bd;                           
bank_misc_array[98] = bank_misc_mind_ya_h;                          
bank_misc_array[99] = bank_misc_missing_snare;
 
bank_misc_array[100] = bank_misc_missing_snare_2;                    
bank_misc_array[101] = bank_misc_more_basic_bd;                      
bank_misc_array[102] = bank_misc_more_snap;                          
bank_misc_array[103] = bank_misc_muted_snare;                        
bank_misc_array[104] = bank_misc_neo808;                             
bank_misc_array[105] = bank_misc_neo808_2;                           
bank_misc_array[106] = bank_misc_n_hi;                               
bank_misc_array[107] = bank_misc_N_low;                              
bank_misc_array[108] = bank_misc_n_mid;                              
bank_misc_array[109] = bank_misc_noise;
 
bank_misc_array[110] = bank_misc_noise_calling_kick;                 
bank_misc_array[111] = bank_misc_noise_clip;                         
bank_misc_array[112] = bank_misc_nosie_calling_kick_2;               
bank_misc_array[113] = bank_misc_N_vlow;                             
bank_misc_array[114] = bank_misc_odd_snare;                          
bank_misc_array[115] = bank_misc_original_tone_snare;                
bank_misc_array[116] = bank_misc_ot_12;                              
bank_misc_array[117] = bank_misc_ot_34;                              
bank_misc_array[118] = bank_misc_ot_sine;                            
bank_misc_array[119] = bank_misc_overdriver_kick;
 
bank_misc_array[120] = bank_misc_pleasure_factory_kick;              
bank_misc_array[121] = bank_misc_pop_1_off_snare;                    
bank_misc_array[122] = bank_misc_power_out;                          
bank_misc_array[123] = bank_misc_psychoacoutic_bd;                   
bank_misc_array[124] = bank_misc_punchy_clean_kick;                 
bank_misc_array[125] = bank_misc_puppy_kick;                        
bank_misc_array[126] = bank_misc_q_ey_bass_drum;                    
bank_misc_array[127] = bank_misc_q_kick_2;                          
bank_misc_array[128] = bank_misc_questoin_kick;                     
bank_misc_array[129] = bank_misc_r_b_fantasy_kick;
 
bank_misc_array[130] = bank_misc_reverb_please_snare;               
bank_misc_array[131] = bank_misc_rimmy_noise;                       
bank_misc_array[132] = bank_misc_RimShot;                           
bank_misc_array[133] = bank_misc_rimshot_1;                         
bank_misc_array[134] = bank_misc_rimshot_2;                         
bank_misc_array[135] = bank_misc_ring_fray_kick;                    
bank_misc_array[136] = bank_misc_ring_mod_drone_bd;                 
bank_misc_array[137] = bank_misc_rm;                                
bank_misc_array[138] = bank_misc_rubber__82;                        
bank_misc_array[139] = bank_misc_rubbery_deep_house_kick;
 
bank_misc_array[140] = bank_misc_Saber_fight;                       
bank_misc_array[141] = bank_misc_serious_bd;                        
bank_misc_array[142] = bank_misc_short_909;                         
bank_misc_array[143] = bank_misc_simon_s_snare;                     
bank_misc_array[144] = bank_misc_Simple_c;                          
bank_misc_array[145] = bank_misc_simple_click_bd;                   
bank_misc_array[146] = bank_misc_simple_h;                          
bank_misc_array[147] = bank_misc_simple_k;                          
bank_misc_array[148] = bank_misc_Simple_r;                          
bank_misc_array[149] = bank_misc_Simple_s;
 
bank_misc_array[150] = bank_misc_skiwlz_snare;                      
bank_misc_array[151] = bank_misc_slepper_n_the_wilderness;          
bank_misc_array[152] = bank_misc_sloppy_electric;                   
bank_misc_array[153] = bank_misc_smooth_rim;                        
bank_misc_array[154] = bank_misc_snappy_809;                        
bank_misc_array[155] = bank_misc_softsnare;                         
bank_misc_array[156] = bank_misc_sou_like_a_low_tom;                
bank_misc_array[157] = bank_misc_sounding_kick;                     
bank_misc_array[158] = bank_misc_special_man_bass_drum;             
bank_misc_array[159] = bank_misc_splatter;
 
bank_misc_array[160] = bank_misc_sticky_q_kick;                     
bank_misc_array[161] = bank_misc_stiffy_snare;                      
bank_misc_array[162] = bank_misc_sweet_tight_snare;                 
bank_misc_array[163] = bank_misc_tchak_snare;                       
bank_misc_array[164] = bank_misc_terror_n_yr_hood_snare;            
bank_misc_array[165] = bank_misc_test;                              
bank_misc_array[166] = bank_misc_think_bd;                          
bank_misc_array[167] = bank_misc_think_sn_1;                        
bank_misc_array[168] = bank_misc_think_sn_2;                        
bank_misc_array[169] = bank_misc_this_snare_spells_electronic;
 
bank_misc_array[170] = bank_misc_tomita_clip;                       
bank_misc_array[171] = bank_misc_tone;                          
bank_misc_array[172] = bank_misc_tone_deg;                      
bank_misc_array[173] = bank_misc_Tone_dis;                      
bank_misc_array[174] = bank_misc_tone_snare;                    
bank_misc_array[175] = bank_misc_Tone_sw2;                      
bank_misc_array[176] = bank_misc_Tone_swp;                      
bank_misc_array[177] = bank_misc_transient_snare;               
bank_misc_array[178] = bank_misc_trashyovertones;               
bank_misc_array[179] = bank_misc_type_snare;
 
bank_misc_array[180] = bank_misc_ultra_electro_bd;              
bank_misc_array[181] = bank_misc_usgae;                    
bank_misc_array[182] = bank_misc_dirty_world_bd;                     
bank_misc_array[183] = bank_misc_Distant_Thunder;                    
bank_misc_array[184] = bank_misc_dist_sub_kick;                      
bank_misc_array[185] = bank_misc_dist_sub_kick_2;                    
bank_misc_array[186] = bank_misc_electro_dnb_kick;                   
bank_misc_array[187] = bank_misc_electronic_rim;                     
bank_misc_array[188] = bank_misc_electro_snare_layer;                
bank_misc_array[189] = bank_misc_end_transmission;                   

bank_misc_array[200] = bank_misc_every_909;                          
bank_misc_array[201] = bank_misc_fatness;


 static const char * bank_misc_electro_array[MDA_BANK_MISC_ELECTRO_SIZE];

static const char * bank_misc_electro_applied_engineering         = "applied_engineering";         
static const char * bank_misc_electro_a_round_thing               = "a_round_thing";               
static const char * bank_misc_electro_atmosphere_of_space         = "atmosphere_of_space";         
static const char * bank_misc_electro_bottle_bill_fx              = "bottle_bill_fx";              
static const char * bank_misc_electro_casio_poppin                = "casio_poppin";                
static const char * bank_misc_electro_category                    = "category";                    
static const char * bank_misc_electro_cheap_fx                    = "cheap_fx";                    
static const char * bank_misc_electro_chrip_q                     = "chrip_q";                     
static const char * bank_misc_electro_clean_rock_bd               = "clean_rock_bd";               
static const char * bank_misc_electro_cold_shot                   = "cold_shot";                   
static const char * bank_misc_electro_confusion                   = "confusion";                   
static const char * bank_misc_electro_cymbal_madness              = "cymbal_madness";              
static const char * bank_misc_electro_depth_fx                    = "depth_fx";                    
static const char * bank_misc_electro_dialed                      = "dialed";                      
static const char * bank_misc_electro_doom_bump                   = "doom_bump";                   
static const char * bank_misc_electro_electric_triangle           = "electric_triangle";           
static const char * bank_misc_electro_electro_perc_long           = "electro_perc_long";           
static const char * bank_misc_electro_electro_perc_short          = "electro_perc_short";          
static const char * bank_misc_electro_electro_rim_2               = "electro_rim_2";               
static const char * bank_misc_electro_electro_rim_3               = "electro_rim_3";               
static const char * bank_misc_electro_electro_rim                 = "electro_rim";                 
static const char * bank_misc_electro_electro_squeek              = "electro_squeek";              
static const char * bank_misc_electro_fuzzy_q_bd                  = "fuzzy_q_bd";                  
static const char * bank_misc_electro_general_obscurity           = "general_obscurity";           
static const char * bank_misc_electro_g_question                  = "g_question";                  
static const char * bank_misc_electro_hard_bork                   = "hard_bork";                   
static const char * bank_misc_electro_hello_q                     = "hello_q";                     
static const char * bank_misc_electro_just_the_tweet              = "just_the_tweet";              
static const char * bank_misc_electro__lectro_spliff              = "_lectro_spliff";              
static const char * bank_misc_electro_lo_fi_house_c               = "lo_fi_house_c";               
static const char * bank_misc_electro_lo_fi_metal                 = "lo_fi_metal";                 
static const char * bank_misc_electro_lo_finess                   = "lo_finess";                   
static const char * bank_misc_electro_long_q_1                    = "long_q_1";                    
static const char * bank_misc_electro_long_q_2                    = "long_q_2";                    
static const char * bank_misc_electro_long_q_3                    = "long_q_3";                    
static const char * bank_misc_electro_long_quack                  = "long_quack";                  
static const char * bank_misc_electro_low_bit_tom                 = "low_bit_tom";                 
static const char * bank_misc_electro_metal_noise_shot            = "metal_noise_shot";            
static const char * bank_misc_electro_open_reso                   = "open_reso";                   
static const char * bank_misc_electro_paging_the_jetsons          = "paging_the_jetsons";          
static const char * bank_misc_electro_pan                         = "pan";                         
static const char * bank_misc_electro_pleasant_combo              = "pleasant_combo";              
static const char * bank_misc_electro_q_ziq_for_the_masses        = "q_ziq_for_the_masses";        
static const char * bank_misc_electro_rich_bd                     = "rich_bd";                     
static const char * bank_misc_electro_ring                        = "ring";                        
static const char * bank_misc_electro_ringing_sn                  = "ringing_sn";                  
static const char * bank_misc_electro_rockin_electro_bass_drum    = "rockin_electro_bass_drum";    
static const char * bank_misc_electro_shocking_kiss               = "shocking_kiss";               
static const char * bank_misc_electro_siren_sing                  = "siren_sing";                  
static const char * bank_misc_electro_slide_tom                   = "slide_tom";                   
static const char * bank_misc_electro_space_dynamic               = "space_dynamic";               
static const char * bank_misc_electro_spaceness                   = "spaceness";                   
static const char * bank_misc_electro_startling_one               = "startling_one";               
static const char * bank_misc_electro_tone_percussion             = "tone_percussion";             
static const char * bank_misc_electro_tweet                       = "tweet";                       
static const char * bank_misc_electro_wee_dog                     = "wee_dog";                     
static const char * bank_misc_electro_what_to_do_at_220           = "what_to_do_at_220";           
static const char * bank_misc_electro_who_hears_all_sound         = "who_hears_all_sound";         
static const char * bank_misc_electro_why_hit                     = "why_hit";                     
static const char * bank_misc_electro_yr_tv_will_be_next          = "yr_tv_will_be_next";          
static const char * bank_misc_electro_zappy_bells                 = "zappy_bells";                 

bank_misc_electro_array[0] = bank_misc_electro_applied_engineering;         
bank_misc_electro_array[1] = bank_misc_electro_a_round_thing;               
bank_misc_electro_array[2] = bank_misc_electro_atmosphere_of_space;         
bank_misc_electro_array[3] = bank_misc_electro_bottle_bill_fx;              
bank_misc_electro_array[4] = bank_misc_electro_casio_poppin;                
bank_misc_electro_array[5] = bank_misc_electro_category;                    
bank_misc_electro_array[6] = bank_misc_electro_cheap_fx;                    
bank_misc_electro_array[7] = bank_misc_electro_chrip_q;                     
bank_misc_electro_array[8] = bank_misc_electro_clean_rock_bd;               
bank_misc_electro_array[9] = bank_misc_electro_cold_shot;                   
bank_misc_electro_array[10] = bank_misc_electro_confusion;                   
bank_misc_electro_array[11] = bank_misc_electro_cymbal_madness;              
bank_misc_electro_array[12] = bank_misc_electro_depth_fx;                    
bank_misc_electro_array[13] = bank_misc_electro_dialed;                      
bank_misc_electro_array[14] = bank_misc_electro_doom_bump;                   
bank_misc_electro_array[15] = bank_misc_electro_electric_triangle;           
bank_misc_electro_array[16] = bank_misc_electro_electro_perc_long;           
bank_misc_electro_array[17] = bank_misc_electro_electro_perc_short;          
bank_misc_electro_array[18] = bank_misc_electro_electro_rim_2;               
bank_misc_electro_array[19] = bank_misc_electro_electro_rim_3;               
bank_misc_electro_array[20] = bank_misc_electro_electro_rim;                 
bank_misc_electro_array[21] = bank_misc_electro_electro_squeek;              
bank_misc_electro_array[22] = bank_misc_electro_fuzzy_q_bd;                  
bank_misc_electro_array[23] = bank_misc_electro_general_obscurity;           
bank_misc_electro_array[24] = bank_misc_electro_g_question;                  
bank_misc_electro_array[25] = bank_misc_electro_hard_bork;                   
bank_misc_electro_array[26] = bank_misc_electro_hello_q;                     
bank_misc_electro_array[27] = bank_misc_electro_just_the_tweet;              
bank_misc_electro_array[28] = bank_misc_electro__lectro_spliff;              
bank_misc_electro_array[29] = bank_misc_electro_lo_fi_house_c;               
bank_misc_electro_array[30] = bank_misc_electro_lo_fi_metal;                 
bank_misc_electro_array[31] = bank_misc_electro_lo_finess;                   
bank_misc_electro_array[32] = bank_misc_electro_long_q_1;                    
bank_misc_electro_array[33] = bank_misc_electro_long_q_2;                    
bank_misc_electro_array[34] = bank_misc_electro_long_q_3;                    
bank_misc_electro_array[35] = bank_misc_electro_long_quack;                  
bank_misc_electro_array[36] = bank_misc_electro_low_bit_tom;                 
bank_misc_electro_array[37] = bank_misc_electro_metal_noise_shot;            
bank_misc_electro_array[38] = bank_misc_electro_open_reso;                   
bank_misc_electro_array[39] = bank_misc_electro_paging_the_jetsons;          
bank_misc_electro_array[40] = bank_misc_electro_pan;                         
bank_misc_electro_array[41] = bank_misc_electro_pleasant_combo;              
bank_misc_electro_array[42] = bank_misc_electro_q_ziq_for_the_masses;        
bank_misc_electro_array[43] = bank_misc_electro_rich_bd;                     
bank_misc_electro_array[44] = bank_misc_electro_ring;                        
bank_misc_electro_array[45] = bank_misc_electro_ringing_sn;                  
bank_misc_electro_array[46] = bank_misc_electro_rockin_electro_bass_drum;    
bank_misc_electro_array[47] = bank_misc_electro_shocking_kiss;             
bank_misc_electro_array[48] = bank_misc_electro_siren_sing;                
bank_misc_electro_array[49] = bank_misc_electro_slide_tom;                 
bank_misc_electro_array[50] = bank_misc_electro_space_dynamic;             
bank_misc_electro_array[51] = bank_misc_electro_spaceness;                 
bank_misc_electro_array[52] = bank_misc_electro_startling_one;             
bank_misc_electro_array[53] = bank_misc_electro_tone_percussion;           
bank_misc_electro_array[54] = bank_misc_electro_tweet;                     
bank_misc_electro_array[55] = bank_misc_electro_wee_dog;                   
bank_misc_electro_array[56] = bank_misc_electro_what_to_do_at_220;         
bank_misc_electro_array[57] = bank_misc_electro_who_hears_all_sound;       
bank_misc_electro_array[58] = bank_misc_electro_why_hit;                   
bank_misc_electro_array[59] = bank_misc_electro_yr_tv_will_be_next;        
bank_misc_electro_array[60] = bank_misc_electro_zappy_bells;               




 static const char * bank_misc_fx_array[MDA_BANK_MISC_FX_SIZE];

static const char * bank_misc_fx_2_bit_click                      = "2_bit_click";                 
static const char * bank_misc_fx_2_bit_zone_drone                 = "2_bit_zone_drone";            
static const char * bank_misc_fx_a_blast                          = "a_blast";                     
static const char * bank_misc_fx_across_the_universe              = "across_the_universe";         
static const char * bank_misc_fx_alarm_3                          = "alarm_3";                     
static const char * bank_misc_fx_alarming_buzz_2                  = "alarming_buzz_2";             
static const char * bank_misc_fx_alarming_buzz                    = "alarming_buzz";               
static const char * bank_misc_fx_alien_curiousity                 = "alien_curiousity";            
static const char * bank_misc_fx_ambient_backgroud                = "ambient_backgroud";           
static const char * bank_misc_fx_click                            = "click";                       
static const char * bank_misc_fx_coo                              = "coo";                         
static const char * bank_misc_fx_detonate                         = "detonate";                    
static const char * bank_misc_fx_dirty_transform_2                = "dirty_transform_2";           
static const char * bank_misc_fx_dirty_transform                  = "dirty_transform";             
static const char * bank_misc_fx_distorion_oscilator              = "distorion_oscilator";         
static const char * bank_misc_fx_electric_intro_to_a              = "electric_intro_to_a";         
static const char * bank_misc_fx_electric_waves_out_my_window     = "electric_waves_out_my_window"; 
static const char * bank_misc_fx_elekperc1                        = "elekperc1";                   
static const char * bank_misc_fx_elekperc2                        = "elekperc2";                   
static const char * bank_misc_fx_end_of_broadcast_day             = "end_of_broadcast_day";        
static const char * bank_misc_fx_entre                            = "entre";                       
static const char * bank_misc_fx_experiment_1                     = "experiment_1";                
static const char * bank_misc_fx_experiment_2___turn_off_the_tv   = "experiment_2___turn_off_the_tv"; 
static const char * bank_misc_fx_experiment_3___the_transform     = "experiment_3___the_transform"; 
static const char * bank_misc_fx_experiment_4___clean             = "experiment_4___clean";        
static const char * bank_misc_fx_feedback                         = "feedback";                    
static const char * bank_misc_fx_flashy_wind                      = "flashy_wind";                 
static const char * bank_misc_fx_flipper                          = "flipper";                     
static const char * bank_misc_fx_f_m7_space_signal                = "f_m7_space_signal";           
static const char * bank_misc_fx_formant_techno                   = "formant_techno";              
static const char * bank_misc_fx_frequency_revolution             = "frequency_revolution";        
static const char * bank_misc_fx_fried_food_for_dinner            = "fried_food_for_dinner";       
static const char * bank_misc_fx_funky_stab                       = "funky_stab";                  
static const char * bank_misc_fx_future                           = "future";                      
static const char * bank_misc_fx_gmaj_lo_fi_chord_2               = "gmaj_lo_fi_chord_2";          
static const char * bank_misc_fx_gmaj_lo_fi_chord                 = "gmaj_lo_fi_chord";            
static const char * bank_misc_fx_halt_variety                     = "halt_variety";                
static const char * bank_misc_fx_hip_hop_stop_2                   = "hip_hop_stop_2";              
static const char * bank_misc_fx_hip_hop_stop                     = "hip_hop_stop";                
static const char * bank_misc_fx_hollow                           = "hollow";                      
static const char * bank_misc_fx_howdy_oil_barrel                 = "howdy_oil_barrel";            
static const char * bank_misc_fx_jaggies                          = "jaggies";                     
static const char * bank_misc_fx_laser_gun___ricochet             = "laser_gun_&_ricochet";        
static const char * bank_misc_fx_machine_drone                    = "machine_drone";               
static const char * bank_misc_fx_metal_twist                      = "metal_twist";                 
static const char * bank_misc_fx_modular                          = "modular";                     
static const char * bank_misc_fx_modular_fm_counterpoint          = "modular_fm_counterpoint";     
static const char * bank_misc_fx_multi_ping                       = "multi_ping";                  
static const char * bank_misc_fx_my_atari_memory                  = "my_atari_memory";             
static const char * bank_misc_fx_new_sweep                        = "new_sweep";                   
static const char * bank_misc_fx_noise_filtering                  = "noise_filtering";             
static const char * bank_misc_fx_not_trying_too_hard              = "not_trying_too_hard";         
static const char * bank_misc_fx_numb                             = "numb";                        
static const char * bank_misc_fx_obligatory_ufo_liftoff_2         = "obligatory_ufo_liftoff_2";    
static const char * bank_misc_fx_obligatory_ufo_liftoff           = "obligatory_ufo_liftoff";      
static const char * bank_misc_fx_oom_paa                          = "oom_paa";                     
static const char * bank_misc_fx_oom_pulse                        = "oom_pulse";                   
static const char * bank_misc_fx_owwww                            = "owwww";                       
static const char * bank_misc_fx_playing_in_water                 = "playing_in_water";            
static const char * bank_misc_fx_plot_thickens_2                  = "plot_thickens_2";             
static const char * bank_misc_fx_punch_flange_2                   = "punch_flange_2";              
static const char * bank_misc_fx_punch_flange                     = "punch_flange";                
static const char * bank_misc_fx_q_zone_drone                     = "q_zone_drone";                
static const char * bank_misc_fx_recption                         = "recption";                    
static const char * bank_misc_fx_sci_fi_door_open                 = "sci_fi_door_open";            
static const char * bank_misc_fx_sci_fi                           = "sci_fi";                      
static const char * bank_misc_fx_sickly_wineglass                 = "sickly_wineglass";            
static const char * bank_misc_fx_sign_off                         = "sign_off";                    
static const char * bank_misc_fx_slow_moving_traffic              = "slow_moving_traffic";         
static const char * bank_misc_fx_synthetic_dive_crust             = "synthetic_dive_crust";        
static const char * bank_misc_fx_synthetic_dive                   = "synthetic_dive";              
static const char * bank_misc_fx_teeth                            = "teeth";                       
static const char * bank_misc_fx_the_plot_thickens                = "the_plot_thickens";           
static const char * bank_misc_fx_the_tweek                        = "the_tweek";                   
static const char * bank_misc_fx_transportive                     = "transportive";                
static const char * bank_misc_fx_ufo_buzz_2                       = "ufo_buzz_2";                  
static const char * bank_misc_fx_ufo_buzz                         = "ufo_buzz";                    
static const char * bank_misc_fx_what_time_is_it                  = "what_time_is_it";             
static const char * bank_misc_fx_wipeout                          = "wipeout";                     
static const char * bank_misc_fx_wooo                             = "wooo";                        
static const char * bank_misc_fx_yeah_hit_dry                     = "yeah_hit_dry";                
static const char * bank_misc_fx_yeah_hit                         = "yeah_hit";                    
static const char * bank_misc_fx_zipper                           = "zipper";                      


bank_misc_fx_array[0] = bank_misc_fx_2_bit_click;                      
bank_misc_fx_array[1] = bank_misc_fx_2_bit_zone_drone;                 
bank_misc_fx_array[2] = bank_misc_fx_a_blast;                          
bank_misc_fx_array[3] = bank_misc_fx_across_the_universe;              
bank_misc_fx_array[4] = bank_misc_fx_alarm_3;                          
bank_misc_fx_array[5] = bank_misc_fx_alarming_buzz_2;                  
bank_misc_fx_array[6] = bank_misc_fx_alarming_buzz;                    
bank_misc_fx_array[7] = bank_misc_fx_alien_curiousity;                 
bank_misc_fx_array[8] = bank_misc_fx_ambient_backgroud;                
bank_misc_fx_array[9] = bank_misc_fx_click;                            
bank_misc_fx_array[10] = bank_misc_fx_coo;                              
bank_misc_fx_array[11] = bank_misc_fx_detonate;                         
bank_misc_fx_array[12] = bank_misc_fx_dirty_transform_2;                
bank_misc_fx_array[13] = bank_misc_fx_dirty_transform;                  
bank_misc_fx_array[14] = bank_misc_fx_distorion_oscilator;              
bank_misc_fx_array[15] = bank_misc_fx_electric_intro_to_a;              
bank_misc_fx_array[16] = bank_misc_fx_electric_waves_out_my_window;     
bank_misc_fx_array[17] = bank_misc_fx_elekperc1;                        
bank_misc_fx_array[18] = bank_misc_fx_elekperc2;                        
bank_misc_fx_array[19] = bank_misc_fx_end_of_broadcast_day;             
bank_misc_fx_array[20] = bank_misc_fx_entre;                            
bank_misc_fx_array[21] = bank_misc_fx_experiment_1;                     
bank_misc_fx_array[22] = bank_misc_fx_experiment_2___turn_off_the_tv;   
bank_misc_fx_array[23] = bank_misc_fx_experiment_3___the_transform;     
bank_misc_fx_array[24] = bank_misc_fx_experiment_4___clean;             
bank_misc_fx_array[25] = bank_misc_fx_feedback;                         
bank_misc_fx_array[26] = bank_misc_fx_flashy_wind;                      
bank_misc_fx_array[27] = bank_misc_fx_flipper;                          
bank_misc_fx_array[28] = bank_misc_fx_f_m7_space_signal;                
bank_misc_fx_array[29] = bank_misc_fx_formant_techno;                   
bank_misc_fx_array[30] = bank_misc_fx_frequency_revolution;             
bank_misc_fx_array[31] = bank_misc_fx_fried_food_for_dinner;            
bank_misc_fx_array[32] = bank_misc_fx_funky_stab;                       
bank_misc_fx_array[33] = bank_misc_fx_future;                           
bank_misc_fx_array[34] = bank_misc_fx_gmaj_lo_fi_chord_2;               
bank_misc_fx_array[35] = bank_misc_fx_gmaj_lo_fi_chord;                 
bank_misc_fx_array[36] = bank_misc_fx_halt_variety;                     
bank_misc_fx_array[37] = bank_misc_fx_hip_hop_stop_2;                   
bank_misc_fx_array[38] = bank_misc_fx_hip_hop_stop;                     
bank_misc_fx_array[39] = bank_misc_fx_hollow;                           
bank_misc_fx_array[40] = bank_misc_fx_howdy_oil_barrel;                 
bank_misc_fx_array[41] = bank_misc_fx_jaggies;                          
bank_misc_fx_array[42] = bank_misc_fx_laser_gun___ricochet;             
bank_misc_fx_array[43] = bank_misc_fx_machine_drone;                    
bank_misc_fx_array[44] = bank_misc_fx_metal_twist;                      
bank_misc_fx_array[45] = bank_misc_fx_modular;                          
bank_misc_fx_array[46] = bank_misc_fx_modular_fm_counterpoint;          
bank_misc_fx_array[47] = bank_misc_fx_multi_ping;                    
bank_misc_fx_array[48] = bank_misc_fx_my_atari_memory;               
bank_misc_fx_array[49] = bank_misc_fx_new_sweep;                     
bank_misc_fx_array[50] = bank_misc_fx_noise_filtering;               
bank_misc_fx_array[51] = bank_misc_fx_not_trying_too_hard;           
bank_misc_fx_array[52] = bank_misc_fx_numb;                          
bank_misc_fx_array[53] = bank_misc_fx_obligatory_ufo_liftoff_2;      
bank_misc_fx_array[54] = bank_misc_fx_obligatory_ufo_liftoff;        
bank_misc_fx_array[55] = bank_misc_fx_oom_paa;                       
bank_misc_fx_array[56] = bank_misc_fx_oom_pulse;                     
bank_misc_fx_array[57] = bank_misc_fx_owwww;                         
bank_misc_fx_array[58] = bank_misc_fx_playing_in_water;              
bank_misc_fx_array[59] = bank_misc_fx_plot_thickens_2;               
bank_misc_fx_array[60] = bank_misc_fx_punch_flange_2;                
bank_misc_fx_array[61] = bank_misc_fx_punch_flange;                  
bank_misc_fx_array[62] = bank_misc_fx_q_zone_drone;                  
bank_misc_fx_array[63] = bank_misc_fx_recption;                      
bank_misc_fx_array[64] = bank_misc_fx_sci_fi_door_open;              
bank_misc_fx_array[65] = bank_misc_fx_sci_fi;                        
bank_misc_fx_array[66] = bank_misc_fx_sickly_wineglass;              
bank_misc_fx_array[67] = bank_misc_fx_sign_off;                      
bank_misc_fx_array[68] = bank_misc_fx_slow_moving_traffic;           
bank_misc_fx_array[69] = bank_misc_fx_synthetic_dive_crust;          
bank_misc_fx_array[70] = bank_misc_fx_synthetic_dive;                
bank_misc_fx_array[71] = bank_misc_fx_teeth;                         
bank_misc_fx_array[72] = bank_misc_fx_the_plot_thickens;             
bank_misc_fx_array[73] = bank_misc_fx_the_tweek;                     
bank_misc_fx_array[74] = bank_misc_fx_transportive;                  
bank_misc_fx_array[75] = bank_misc_fx_ufo_buzz_2;                    
bank_misc_fx_array[76] = bank_misc_fx_ufo_buzz;                      
bank_misc_fx_array[77] = bank_misc_fx_what_time_is_it;               
bank_misc_fx_array[78] = bank_misc_fx_wipeout;                       
bank_misc_fx_array[79] = bank_misc_fx_wooo;                          
bank_misc_fx_array[80] = bank_misc_fx_yeah_hit_dry;                  
bank_misc_fx_array[81] = bank_misc_fx_yeah_hit;                      
bank_misc_fx_array[82] = bank_misc_fx_zipper;                        

 static const char * bank_misc_perc_array[MDA_BANK_MISC_PERC_SIZE];

static const char * bank_misc_perc_ambient_tom                    = "ambient_tom";                 
static const char * bank_misc_perc_bell                           = "bell";                        
static const char * bank_misc_perc_bell_rim                       = "bell_rim";                    
static const char * bank_misc_perc_clav_2                         = "clav_2";                      
static const char * bank_misc_perc_clav                           = "clav";                        
static const char * bank_misc_perc_deep_cowbell                   = "deep_cowbell";                
static const char * bank_misc_perc_dist_tom_2                     = "dist_tom_2";                  
static const char * bank_misc_perc_dist_tom                       = "dist_tom";                    
static const char * bank_misc_perc_door_knock                     = "door_knock";                  
static const char * bank_misc_perc_electric_clav                  = "electric_clav";               
static const char * bank_misc_perc_feel_me_clinky                 = "feel_me_clinky";              
static const char * bank_misc_perc_fidelity_faction_tom           = "fidelity_faction_tom";        
static const char * bank_misc_perc_funkay_timbale                 = "funkay_timbale";              
static const char * bank_misc_perc_funk_box_perc_1                = "funk_box_perc_1";             
static const char * bank_misc_perc_funk_box_perc_2                = "funk_box_perc_2";             
static const char * bank_misc_perc_give_it_to_me                  = "give_it_to_me";               
static const char * bank_misc_perc_hammond_cowbell                = "hammond_cowbell";             
static const char * bank_misc_perc_hollow_wood_2                  = "hollow_wood_2";               
static const char * bank_misc_perc_hollow_wood                    = "hollow_wood";                 
static const char * bank_misc_perc_it_s_the_simple_things         = "it_s_the_simple_things";      
static const char * bank_misc_perc_layered_sleigh_bell            = "layered_sleigh_bell";         
static const char * bank_misc_perc_light_tamb_1                   = "light_tamb_1";                
static const char * bank_misc_perc_light_tamb_2                   = "light_tamb_2";                
static const char * bank_misc_perc_light_tamb_3                   = "light_tamb_3";                
static const char * bank_misc_perc_like_a_tabla_1                 = "like_a_tabla_1";              
static const char * bank_misc_perc_like_a_tabla_2                 = "like_a_tabla_2";              
static const char * bank_misc_perc_like_a_tabla_3                 = "like_a_tabla_3";              
static const char * bank_misc_perc_limited_shaker                 = "limited_shaker";              
static const char * bank_misc_perc_lo_fi_tamb                     = "lo_fi_tamb";                  
static const char * bank_misc_perc_low_key_tom                    = "low_key_tom";                 
static const char * bank_misc_perc_low_ping                       = "low_ping";                    
static const char * bank_misc_perc_mellow_shake                   = "mellow_shake";                
static const char * bank_misc_perc_metallic_node                  = "metallic_node";               
static const char * bank_misc_perc_mild_tamb                      = "mild_tamb";                   
static const char * bank_misc_perc_ol__skool_shake                = "ol__skool_shake";             
static const char * bank_misc_perc_poppy                          = "poppy";                       
static const char * bank_misc_perc_q_tom                          = "q_tom";                       
static const char * bank_misc_perc_resonant_knock                 = "resonant_knock";              
static const char * bank_misc_perc_ringmod_rim_2                  = "ringmod_rim_2";               
static const char * bank_misc_perc_ringmod_rim                    = "ringmod_rim";                 
static const char * bank_misc_perc_shaaka_2                       = "shaaka_2";                    
static const char * bank_misc_perc_shaaka                         = "shaaka";                      
static const char * bank_misc_perc_short_tom_1                    = "short_tom_1";                 
static const char * bank_misc_perc_short_tom_2                    = "short_tom_2";                 
static const char * bank_misc_perc_short_tom                      = "short_tom";                   
static const char * bank_misc_perc_sickly_cowbell                 = "sickly_cowbell";              
static const char * bank_misc_perc_slinky_tamb                    = "slinky_tamb";                 
static const char * bank_misc_perc_super_tom                      = "super_tom";                   
static const char * bank_misc_perc_sweetness_shaker               = "sweetness_shaker";            
static const char * bank_misc_perc_timbale_sound                  = "timbale_sound";               
static const char * bank_misc_perc_tom                            = "tom";                         
static const char * bank_misc_perc_tommy                          = "tommy";                       
static const char * bank_misc_perc_tom_tone                       = "tom_tone";                    
static const char * bank_misc_perc_tone_tom                       = "tone_tom";                    
static const char * bank_misc_perc_torn_tweeter                   = "torn_tweeter";                
static const char * bank_misc_perc_tweety_cowbell_2               = "tweety_cowbell_2";            
static const char * bank_misc_perc_tweety_cowbell                 = "tweety_cowbell";              
static const char * bank_misc_perc_type_noise                     = "type_noise";                  
static const char * bank_misc_perc_very_high_clav                 = "very_high_clav";              
static const char * bank_misc_perc_video_game_clip                = "video_game_clip";             
static const char * bank_misc_perc_weird_1                        = "weird_1";                     
static const char * bank_misc_perc_wood_and_metal                 = "wood_and_metal";              

bank_misc_perc_array[0] = bank_misc_perc_ambient_tom;                  
bank_misc_perc_array[1] = bank_misc_perc_bell;                         
bank_misc_perc_array[2] = bank_misc_perc_bell_rim;                     
bank_misc_perc_array[3] = bank_misc_perc_clav_2;                       
bank_misc_perc_array[4] = bank_misc_perc_clav;                         
bank_misc_perc_array[5] = bank_misc_perc_deep_cowbell;                 
bank_misc_perc_array[6] = bank_misc_perc_dist_tom_2;                   
bank_misc_perc_array[7] = bank_misc_perc_dist_tom;                     
bank_misc_perc_array[8] = bank_misc_perc_door_knock;                   
bank_misc_perc_array[9] = bank_misc_perc_electric_clav;                
bank_misc_perc_array[10] = bank_misc_perc_feel_me_clinky;               
bank_misc_perc_array[11] = bank_misc_perc_fidelity_faction_tom;         
bank_misc_perc_array[12] = bank_misc_perc_funkay_timbale;               
bank_misc_perc_array[13] = bank_misc_perc_funk_box_perc_1;              
bank_misc_perc_array[14] = bank_misc_perc_funk_box_perc_2;              
bank_misc_perc_array[15] = bank_misc_perc_give_it_to_me;                
bank_misc_perc_array[16] = bank_misc_perc_hammond_cowbell;              
bank_misc_perc_array[17] = bank_misc_perc_hollow_wood_2;                
bank_misc_perc_array[18] = bank_misc_perc_hollow_wood;                  
bank_misc_perc_array[19] = bank_misc_perc_it_s_the_simple_things;       
bank_misc_perc_array[20] = bank_misc_perc_layered_sleigh_bell;          
bank_misc_perc_array[21] = bank_misc_perc_light_tamb_1;                 
bank_misc_perc_array[22] = bank_misc_perc_light_tamb_2;                 
bank_misc_perc_array[23] = bank_misc_perc_light_tamb_3;                 
bank_misc_perc_array[24] = bank_misc_perc_like_a_tabla_1;               
bank_misc_perc_array[25] = bank_misc_perc_like_a_tabla_2;               
bank_misc_perc_array[26] = bank_misc_perc_like_a_tabla_3;               
bank_misc_perc_array[27] = bank_misc_perc_limited_shaker;               
bank_misc_perc_array[28] = bank_misc_perc_lo_fi_tamb;                   
bank_misc_perc_array[29] = bank_misc_perc_low_key_tom;                  
bank_misc_perc_array[30] = bank_misc_perc_low_ping;                     
bank_misc_perc_array[31] = bank_misc_perc_mellow_shake;                 
bank_misc_perc_array[32] = bank_misc_perc_metallic_node;                
bank_misc_perc_array[33] = bank_misc_perc_mild_tamb;                    
bank_misc_perc_array[34] = bank_misc_perc_ol__skool_shake;              
bank_misc_perc_array[35] = bank_misc_perc_poppy;                        
bank_misc_perc_array[36] = bank_misc_perc_q_tom;                        
bank_misc_perc_array[37] = bank_misc_perc_resonant_knock;               
bank_misc_perc_array[38] = bank_misc_perc_ringmod_rim_2;                
bank_misc_perc_array[39] = bank_misc_perc_ringmod_rim;                  
bank_misc_perc_array[40] = bank_misc_perc_shaaka_2;                     
bank_misc_perc_array[41] = bank_misc_perc_shaaka;                       
bank_misc_perc_array[42] = bank_misc_perc_short_tom_1;                  
bank_misc_perc_array[43] = bank_misc_perc_short_tom_2;                  
bank_misc_perc_array[44] = bank_misc_perc_short_tom;                    
bank_misc_perc_array[45] = bank_misc_perc_sickly_cowbell;               
bank_misc_perc_array[46] = bank_misc_perc_slinky_tamb;              
bank_misc_perc_array[47] = bank_misc_perc_super_tom;                
bank_misc_perc_array[48] = bank_misc_perc_sweetness_shaker;         
bank_misc_perc_array[49] = bank_misc_perc_timbale_sound;            
bank_misc_perc_array[50] = bank_misc_perc_tom;                      
bank_misc_perc_array[51] = bank_misc_perc_tommy;                    
bank_misc_perc_array[52] = bank_misc_perc_tom_tone;                 
bank_misc_perc_array[53] = bank_misc_perc_tone_tom;                 
bank_misc_perc_array[54] = bank_misc_perc_torn_tweeter;             
bank_misc_perc_array[55] = bank_misc_perc_tweety_cowbell_2;         
bank_misc_perc_array[56] = bank_misc_perc_tweety_cowbell;           
bank_misc_perc_array[57] = bank_misc_perc_type_noise;               
bank_misc_perc_array[58] = bank_misc_perc_very_high_clav;           
bank_misc_perc_array[59] = bank_misc_perc_video_game_clip;          
bank_misc_perc_array[60] = bank_misc_perc_weird_1;                  
bank_misc_perc_array[61] = bank_misc_perc_wood_and_metal;           


 static const char * bank_misc_hats_array[MDA_BANK_MISC_HATS_SIZE];

static const char * bank_misc_hats_amen_ride                      = "amen_ride";                   
static const char * bank_misc_hats_band_pass_cymbal               = "band_pass_cymbal";            
static const char * bank_misc_hats_basic_hh                       = "basic_hh";                    
static const char * bank_misc_hats_bright_808_hat_2               = "bright_808_hat_2";            
static const char * bank_misc_hats_bright_808_hat_3               = "bright_808_hat_3";            
static const char * bank_misc_hats_bright_808_hat                 = "bright_808_hat";              
static const char * bank_misc_hats_bring_me_out                   = "bring_me_out";                
static const char * bank_misc_hats_clicky_hat                     = "clicky_hat";                  
static const char * bank_misc_hats_close_ride                     = "close_ride";                  
static const char * bank_misc_hats_default_hi_hat                 = "default_hi_hat";              
static const char * bank_misc_hats_drama_hat_2                    = "drama_hat_2";                 
static const char * bank_misc_hats_drama_hat                      = "drama_hat";                   
static const char * bank_misc_hats_dusty_simple_hh                = "dusty_simple_hh";             
static const char * bank_misc_hats_dusty_simple_oh                = "dusty_simple_oh";             
static const char * bank_misc_hats_electro_hh                     = "electro_hh";                  
static const char * bank_misc_hats_electrometallic_hat            = "electrometallic_hat";         
static const char * bank_misc_hats_errie_ride                     = "errie_ride";                  
static const char * bank_misc_hats_fedora_hat                     = "fedora_hat";                  
static const char * bank_misc_hats_feelin__high_hat_1             = "feelin__high_hat_1";          
static const char * bank_misc_hats_feelin__high_hat_2             = "feelin__high_hat_2";          
static const char * bank_misc_hats_feelin__high_hat_3             = "feelin__high_hat_3";          
static const char * bank_misc_hats_feelin__open_high_hat          = "feelin__open_high_hat";       
static const char * bank_misc_hats_flat_cymbal                    = "flat_cymbal";                 
static const char * bank_misc_hats_get_out                        = "get_out";                     
static const char * bank_misc_hats_hardhat_2                      = "hardhat_2";                   
static const char * bank_misc_hats_hardhat                        = "hardhat";                     
static const char * bank_misc_hats_hardly_hh                      = "hardly_hh";                   
static const char * bank_misc_hats_heavy_electrihat               = "heavy_electrihat";            
static const char * bank_misc_hats_hh                             = "hh";                          
static const char * bank_misc_hats_hip_overtone_hat               = "hip_overtone_hat";            
static const char * bank_misc_hats_large_open_hh                  = "large_open_hh";               
static const char * bank_misc_hats_lighthat                       = "lighthat";                    
static const char * bank_misc_hats_little_hat                     = "little_hat";                  
static const char * bank_misc_hats_live_hat                       = "live_hat";                    
static const char * bank_misc_hats_live_sound_hh                  = "live_sound_hh";               
static const char * bank_misc_hats_live_sound_oh                  = "live_sound_oh";               
static const char * bank_misc_hats_loaded_hat                     = "loaded_hat";                  
static const char * bank_misc_hats_loaded_open_hat                = "loaded_open_hat";             
static const char * bank_misc_hats_oh_so_high_hat                 = "oh_so_high_hat";              
static const char * bank_misc_hats_oil_hat                        = "oil_hat";                     
static const char * bank_misc_hats_pedal_hh                       = "pedal_hh";                    
static const char * bank_misc_hats_q_ride_2                       = "q_ride_2";                    
static const char * bank_misc_hats_q_ride                         = "q_ride";                      
static const char * bank_misc_hats_sandy_ride                     = "sandy_ride";                  
static const char * bank_misc_hats_scratchy_hh                    = "scratchy_hh";                 
static const char * bank_misc_hats_shaker_ride                    = "shaker_ride";                 
static const char * bank_misc_hats_shaker_ride_long               = "shaker_ride_long";            
static const char * bank_misc_hats_simple_ride                    = "simple_ride";                 
static const char * bank_misc_hats_sleepy_1                       = "sleepy_1";                    
static const char * bank_misc_hats_sleepy_2                       = "sleepy_2";                    
static const char * bank_misc_hats_sleepy_ride_2                  = "sleepy_ride_2";               
static const char * bank_misc_hats_sleepy_ride                    = "sleepy_ride";                 
static const char * bank_misc_hats_slick_hat_compressd_open       = "slick_hat_compressd_open";    
static const char * bank_misc_hats_slick_hat                      = "slick_hat";                   
static const char * bank_misc_hats_slick_hat_half_open            = "slick_hat_half_open";         
static const char * bank_misc_hats_slick_hat_open                 = "slick_hat_open";              
static const char * bank_misc_hats_slide_hat                      = "slide_hat";                   
static const char * bank_misc_hats_softie_lo_fi_hat               = "softie_lo-fi_hat";            
static const char * bank_misc_hats_softie_lo_fi_open_hat_2        = "softie_lo-fi_open_hat_2";     
static const char * bank_misc_hats_softie_lo_fi_open_hat          = "softie_lo-fi_open_hat";       
static const char * bank_misc_hats_squeeze_me_hat                 = "squeeze_me_hat";              
static const char * bank_misc_hats_squeeze_me_open_hat            = "squeeze_me_open_hat";         
static const char * bank_misc_hats_stack_hhh                      = "stack_hhh";                   
static const char * bank_misc_hats_starry_hh                      = "starry_hh";                   
static const char * bank_misc_hats_starry_short_oh                = "starry_short_oh";             
static const char * bank_misc_hats_static_tone_hat                = "static_tone_hat";             
static const char * bank_misc_hats_tarp_hh                        = "tarp_hh";                     
static const char * bank_misc_hats_the_sting_hat_2                = "the_sting_hat_2";             
static const char * bank_misc_hats_the_sting_hat                  = "the_sting_hat";               
static const char * bank_misc_hats_the_sting_open_hat             = "the_sting_open_hat";          
static const char * bank_misc_hats_threshold_noise                = "threshold_noise";             
static const char * bank_misc_hats_threshold_noise_open           = "threshold_noise_open";        
static const char * bank_misc_hats_touch_me_hat                   = "touch_me_hat";                

bank_misc_hats_array[0] = bank_misc_hats_amen_ride;                   
bank_misc_hats_array[1] = bank_misc_hats_band_pass_cymbal;            
bank_misc_hats_array[2] = bank_misc_hats_basic_hh;                    
bank_misc_hats_array[3] = bank_misc_hats_bright_808_hat_2;            
bank_misc_hats_array[4] = bank_misc_hats_bright_808_hat_3;            
bank_misc_hats_array[5] = bank_misc_hats_bright_808_hat;              
bank_misc_hats_array[6] = bank_misc_hats_bring_me_out;                
bank_misc_hats_array[7] = bank_misc_hats_clicky_hat;                  
bank_misc_hats_array[8] = bank_misc_hats_close_ride;                  
bank_misc_hats_array[9] = bank_misc_hats_default_hi_hat;              
bank_misc_hats_array[10] = bank_misc_hats_drama_hat_2;                 
bank_misc_hats_array[11] = bank_misc_hats_drama_hat;                   
bank_misc_hats_array[12] = bank_misc_hats_dusty_simple_hh;             
bank_misc_hats_array[13] = bank_misc_hats_dusty_simple_oh;             
bank_misc_hats_array[14] = bank_misc_hats_electro_hh;                  
bank_misc_hats_array[15] = bank_misc_hats_electrometallic_hat;         
bank_misc_hats_array[16] = bank_misc_hats_errie_ride;                  
bank_misc_hats_array[17] = bank_misc_hats_fedora_hat;                  
bank_misc_hats_array[18] = bank_misc_hats_feelin__high_hat_1;          
bank_misc_hats_array[19] = bank_misc_hats_feelin__high_hat_2;          
bank_misc_hats_array[20] = bank_misc_hats_feelin__high_hat_3;          
bank_misc_hats_array[21] = bank_misc_hats_feelin__open_high_hat;       
bank_misc_hats_array[22] = bank_misc_hats_flat_cymbal;                 
bank_misc_hats_array[23] = bank_misc_hats_get_out;                     
bank_misc_hats_array[24] = bank_misc_hats_hardhat_2;                   
bank_misc_hats_array[25] = bank_misc_hats_hardhat;                     
bank_misc_hats_array[26] = bank_misc_hats_hardly_hh;                   
bank_misc_hats_array[27] = bank_misc_hats_heavy_electrihat;            
bank_misc_hats_array[28] = bank_misc_hats_hh;                          
bank_misc_hats_array[29] = bank_misc_hats_hip_overtone_hat;            
bank_misc_hats_array[30] = bank_misc_hats_large_open_hh;               
bank_misc_hats_array[31] = bank_misc_hats_lighthat;                    
bank_misc_hats_array[32] = bank_misc_hats_little_hat;                  
bank_misc_hats_array[33] = bank_misc_hats_live_sound_hh;                 
bank_misc_hats_array[34] = bank_misc_hats_live_sound_oh;                 
bank_misc_hats_array[35] = bank_misc_hats_loaded_hat;                    
bank_misc_hats_array[36] = bank_misc_hats_loaded_open_hat;               
bank_misc_hats_array[37] = bank_misc_hats_oh_so_high_hat;                
bank_misc_hats_array[38] = bank_misc_hats_oil_hat;                       
bank_misc_hats_array[39] = bank_misc_hats_pedal_hh;                      
bank_misc_hats_array[40] = bank_misc_hats_q_ride_2;                      
bank_misc_hats_array[41] = bank_misc_hats_q_ride;                        
bank_misc_hats_array[42] = bank_misc_hats_sandy_ride;                    
bank_misc_hats_array[43] = bank_misc_hats_scratchy_hh;                   
bank_misc_hats_array[44] = bank_misc_hats_shaker_ride;                   
bank_misc_hats_array[45] = bank_misc_hats_shaker_ride_long;              
bank_misc_hats_array[46] = bank_misc_hats_simple_ride;                   
bank_misc_hats_array[47] = bank_misc_hats_sleepy_1;                      
bank_misc_hats_array[48] = bank_misc_hats_sleepy_2;                      
bank_misc_hats_array[49] = bank_misc_hats_sleepy_ride_2;                 
bank_misc_hats_array[50] = bank_misc_hats_sleepy_ride;                   
bank_misc_hats_array[51] = bank_misc_hats_slick_hat_compressd_open;      
bank_misc_hats_array[52] = bank_misc_hats_slick_hat;                     
bank_misc_hats_array[53] = bank_misc_hats_slick_hat_half_open;           
bank_misc_hats_array[54] = bank_misc_hats_slick_hat_open;                
bank_misc_hats_array[55] = bank_misc_hats_slide_hat;                     
bank_misc_hats_array[56] = bank_misc_hats_softie_lo_fi_hat;              
bank_misc_hats_array[57] = bank_misc_hats_softie_lo_fi_open_hat_2;       
bank_misc_hats_array[58] = bank_misc_hats_softie_lo_fi_open_hat;         
bank_misc_hats_array[59] = bank_misc_hats_squeeze_me_hat;                
bank_misc_hats_array[60] = bank_misc_hats_squeeze_me_open_hat;           
bank_misc_hats_array[61] = bank_misc_hats_stack_hhh;                     
bank_misc_hats_array[62] = bank_misc_hats_starry_hh;                     
bank_misc_hats_array[63] = bank_misc_hats_starry_short_oh;               
bank_misc_hats_array[64] = bank_misc_hats_static_tone_hat;               
bank_misc_hats_array[65] = bank_misc_hats_tarp_hh;                       
bank_misc_hats_array[66] = bank_misc_hats_the_sting_hat_2;               
bank_misc_hats_array[67] = bank_misc_hats_the_sting_hat;                 
bank_misc_hats_array[68] = bank_misc_hats_the_sting_open_hat;            
bank_misc_hats_array[69] = bank_misc_hats_threshold_noise;               
bank_misc_hats_array[70] = bank_misc_hats_threshold_noise_open;          
bank_misc_hats_array[71] = bank_misc_hats_touch_me_hat;                 


 static const char * bank_tr77_array[MDA_BANK_TR77_SIZE];

static const char * bank_tr77_Tr77_cui1                           = "Tr77_cui1";                   
static const char * bank_tr77_Tr77_cui2                           = "Tr77_cui2";                   
static const char * bank_tr77_Tr77_cym                            = "Tr77_cym";                    
static const char * bank_tr77_Tr77_hat                            = "Tr77_hat";                    
static const char * bank_tr77_Tr77_kick                           = "Tr77_kick";                   
static const char * bank_tr77_Tr77_snare                          = "Tr77_snare";                  
static const char * bank_tr77_Tr77_tom1                           = "Tr77_tom1";                   
static const char * bank_tr77_Tr77_tom2                           = "Tr77_tom2";                   


 bank_tr77_array[0] = bank_tr77_Tr77_cui1;             
 bank_tr77_array[1] = bank_tr77_Tr77_cui2;             
 bank_tr77_array[2] = bank_tr77_Tr77_cym;              
 bank_tr77_array[3] = bank_tr77_Tr77_hat;              
 bank_tr77_array[4] = bank_tr77_Tr77_kick;             
 bank_tr77_array[5] = bank_tr77_Tr77_snare;            
 bank_tr77_array[6] = bank_tr77_Tr77_tom1;             
 bank_tr77_array[7] = bank_tr77_Tr77_tom2;             

 static const char * bank_tr606_array[MDA_BANK_TR606_SIZE];

static const char * bank_tr606_TR606_Cymbal                       = "TR606_Cymbal";                
static const char * bank_tr606_TR606_Hat_C                        = "TR606_Hat_C";                 
static const char * bank_tr606_TR606_Hat_O                        = "TR606_Hat_O";                 
static const char * bank_tr606_TR606_Kick                         = "TR606_Kick";                  
static const char * bank_tr606_TR606_Snare                        = "TR606_Snare";                 
static const char * bank_tr606_TR606_Tom_H                        = "TR606_Tom_H";                 
static const char * bank_tr606_TR606_Tom_L                        = "TR606_Tom_L";                 

 bank_tr606_array[0] = bank_tr606_TR606_Cymbal;       
 bank_tr606_array[1] = bank_tr606_TR606_Hat_C;        
 bank_tr606_array[2] = bank_tr606_TR606_Hat_O;        
 bank_tr606_array[3] = bank_tr606_TR606_Kick;         
 bank_tr606_array[4] = bank_tr606_TR606_Snare;        
 bank_tr606_array[5] = bank_tr606_TR606_Tom_H;        
 bank_tr606_array[6] = bank_tr606_TR606_Tom_L;        

 static const char * bank_R_B_array[MDA_BANK_R_B_SIZE];

static const char * bank_R_B_clapfilt                             = "clapfilt";                    
static const char * bank_R_B_hat                                  = "hat";                         
static const char * bank_R_B_Hat_O                                = "Hat_O";                       
static const char * bank_R_B_hat_rattle                           = "hat_rattle";                  
static const char * bank_R_B_k_hard                               = "k_hard";                      
static const char * bank_R_B_k_muffle                             = "k_muffle";                    
static const char * bank_R_B_k_soft                               = "k_soft";                      
static const char * bank_R_B_old_kik                              = "old_kik";                     
static const char * bank_R_B_popclap                              = "popclap";                     
static const char * bank_R_B_popsnare                             = "popsnare";                    
static const char * bank_R_B_S_hollow                             = "S_hollow";                    
static const char * bank_R_B_snare_hat                            = "snare_hat";                   
static const char * bank_R_B_s_pitch                              = "s_pitch";                     
static const char * bank_R_B_s_spit                               = "s_spit";                      
static const char * bank_R_B_x_backwd                             = "x_backwd";                    


 bank_R_B_array[0] = bank_R_B_clapfilt;           
 bank_R_B_array[1] = bank_R_B_hat;                
 bank_R_B_array[2] = bank_R_B_Hat_O;              
 bank_R_B_array[3] = bank_R_B_hat_rattle;         
 bank_R_B_array[4] = bank_R_B_k_hard;             
 bank_R_B_array[5] = bank_R_B_k_muffle;           
 bank_R_B_array[6] = bank_R_B_k_soft;             
 bank_R_B_array[7] = bank_R_B_old_kik;            
 bank_R_B_array[8] = bank_R_B_popclap;            
 bank_R_B_array[9] = bank_R_B_popsnare;           
 bank_R_B_array[10] = bank_R_B_S_hollow;           
 bank_R_B_array[11] = bank_R_B_snare_hat;          
 bank_R_B_array[12] = bank_R_B_s_pitch;            
 bank_R_B_array[13] = bank_R_B_s_spit;             
 bank_R_B_array[14] = bank_R_B_x_backwd;           

 static const char * bank_misc_synth_array[MDA_BANK_MISC_SYNTH_SIZE];

 static const char * bank_misc_synth_average_rock                  = "average_rock";                
 static const char * bank_misc_synth_casio_piano_c                 = "casio_piano_c";               
 static const char * bank_misc_synth_chorded_perc                  = "chorded_perc";                
 static const char * bank_misc_synth_c_maj_chord                   = "c_maj_chord";                 
 static const char * bank_misc_synth_consumer_fm                   = "consumer_fm";                 
 static const char * bank_misc_synth_dark_and_sweet_fm             = "dark_and_sweet_fm";           
 static const char * bank_misc_synth_dist_high_c                   = "dist_high_c";                 
 static const char * bank_misc_synth_eno_s_lift                    = "eno_s_lift";                  
 static const char * bank_misc_synth_eno_s_pad                     = "eno_s_pad";                   
 static const char * bank_misc_synth_eno_s_ride                    = "eno_s_ride";                  
 static const char * bank_misc_synth_flat_square_c                 = "flat_square_c";               
 static const char * bank_misc_synth_fm_c_bass_burst               = "fm_c_bass_burst";             
 static const char * bank_misc_synth_fur_strings_2                 = "fur_strings_2";               
 static const char * bank_misc_synth_fur_strings                   = "fur_strings";                 
 static const char * bank_misc_synth_it_s_the_music_tom_c          = "it_s_the_music_tom_c";        
 static const char * bank_misc_synth_layered_bass_synth_c          = "layered_bass_synth_c";        
 static const char * bank_misc_synth_my_red_hot_guitar             = "my_red_hot_guitar";           
 static const char * bank_misc_synth_raver_organ                   = "raver_organ";                 
 static const char * bank_misc_synth_strong_c_dub_bass             = "strong_c_dub_bass";           
 static const char * bank_misc_synth_sync_drift                    = "sync_drift";                  
 static const char * bank_misc_synth_sync_man                      = "sync_man";                    
 static const char * bank_misc_synth_that_house_chord_cm           = "that_house_chord_cm";         
 static const char * bank_misc_synth_vibrato_pad                   = "vibrato_pad";                 
 static const char * bank_misc_synth_who_loves_ya__baby            = "who_loves_ya__baby";          
 static const char * bank_misc_synth_wowmelodictone                = "wowmelodictone";              
 
 
 bank_misc_synth_array[0]  = bank_misc_synth_average_rock;              
 bank_misc_synth_array[1]  = bank_misc_synth_casio_piano_c;             
 bank_misc_synth_array[2]  = bank_misc_synth_chorded_perc;              
 bank_misc_synth_array[3]  = bank_misc_synth_c_maj_chord;               
 bank_misc_synth_array[4]  = bank_misc_synth_consumer_fm;               
 bank_misc_synth_array[5]  = bank_misc_synth_dark_and_sweet_fm;         
 bank_misc_synth_array[6]  = bank_misc_synth_dist_high_c;               
 bank_misc_synth_array[7]  = bank_misc_synth_eno_s_lift;                
 bank_misc_synth_array[8]  = bank_misc_synth_eno_s_pad;                 
 bank_misc_synth_array[9]  = bank_misc_synth_eno_s_ride;

 bank_misc_synth_array[10] = bank_misc_synth_wowmelodictone;             
 bank_misc_synth_array[11] = bank_misc_synth_flat_square_c;             
 bank_misc_synth_array[12] = bank_misc_synth_fm_c_bass_burst;           
 bank_misc_synth_array[13] = bank_misc_synth_fur_strings_2;             
 bank_misc_synth_array[14] = bank_misc_synth_fur_strings;               
 bank_misc_synth_array[15] = bank_misc_synth_it_s_the_music_tom_c;      
 bank_misc_synth_array[16] = bank_misc_synth_layered_bass_synth_c;      
 bank_misc_synth_array[17] = bank_misc_synth_my_red_hot_guitar;         
 bank_misc_synth_array[18] = bank_misc_synth_raver_organ;               
 bank_misc_synth_array[19] = bank_misc_synth_strong_c_dub_bass;
 
 bank_misc_synth_array[20] = bank_misc_synth_sync_drift;                
 bank_misc_synth_array[21] = bank_misc_synth_sync_man;                  
 bank_misc_synth_array[22] = bank_misc_synth_that_house_chord_cm;       
 bank_misc_synth_array[23] = bank_misc_synth_vibrato_pad;               
 bank_misc_synth_array[24] = bank_misc_synth_who_loves_ya__baby;        



 const char * bank_tr909_array[13];

 static const char * bank_tr909_Clap                               = "Clap";                        
 static const char * bank_tr909_Hat_c2                             = "Hat_c2";                      
 static const char * bank_tr909_Hat_c                              = "Hat_c";                       
 static const char * bank_tr909_Hat_o2                             = "Hat_o2";                      
 static const char * bank_tr909_Hat_o                              = "Hat_o";                       
 static const char * bank_tr909_Kick_1                             = "Kick_1";                      
 static const char * bank_tr909_Kick_2                             = "Kick_2";                      
 static const char * bank_tr909_Kick_3                             = "Kick_3";                      
 static const char * bank_tr909_Kick                               = "Kick";                        
 static const char * bank_tr909_Snare2                             = "Snare2";                      
 static const char * bank_tr909_Snare                              = "Snare";                       
 static const char * bank_tr909_TR909_Clap                         = "TR909_Clap";                  
 static const char * bank_tr909_TR909_Kick                         = "TR909_Kick";                  

 bank_tr909_array[0] = bank_tr909_Clap;               
 bank_tr909_array[1] = bank_tr909_Hat_c2;             
 bank_tr909_array[2] = bank_tr909_Hat_c;              
 bank_tr909_array[3] = bank_tr909_Hat_o2;             
 bank_tr909_array[4] = bank_tr909_Hat_o;              
 bank_tr909_array[5] = bank_tr909_Kick_1;             
 bank_tr909_array[6] = bank_tr909_Kick_2;             
 bank_tr909_array[7] = bank_tr909_Kick_3;             
 bank_tr909_array[8] = bank_tr909_Kick;               
 bank_tr909_array[9] = bank_tr909_Snare2;             
 bank_tr909_array[10] = bank_tr909_Snare;              
 bank_tr909_array[11] = bank_tr909_TR909_Clap;         
 bank_tr909_array[12] = bank_tr909_TR909_Kick;         


  static const char * bank_Effects_array[MDA_BANK_EFFECTS];

  static const char * bank_Effects_Bubble                           = "Bubble";                      
  static const char * bank_Effects_Cicada                           = "Cicada";                      
  static const char * bank_Effects_dialing                          = "dialing";                     
  static const char * bank_Effects_Glass                            = "Glass";                       
  static const char * bank_Effects_Glass_rn                         = "Glass_rn";                    
  static const char * bank_Effects_Gunshot                          = "Gunshot";                     
  static const char * bank_Effects_hammer2                          = "hammer2";                     
  static const char * bank_Effects_Hammer                           = "Hammer";                      
  static const char * bank_Effects_impulses                         = "impulses";                    
  static const char * bank_Effects_Laser                            = "Laser";                       
  static const char * bank_Effects_loop                             = "loop";                        
  static const char * bank_Effects_Reverse                          = "Reverse";                     
  static const char * bank_Effects_Ringing                          = "Ringing";                     
  static const char * bank_Effects_Scissors                         = "Scissors";                    
  static const char * bank_Effects_Sonar                            = "Sonar";                       
  static const char * bank_Effects_thunder                          = "thunder";                     
  
  
  bank_Effects_array[0]  = bank_Effects_Bubble;     
  bank_Effects_array[1]  = bank_Effects_Cicada;     
  bank_Effects_array[2]  = bank_Effects_dialing;    
  bank_Effects_array[3]  = bank_Effects_Glass;      
  bank_Effects_array[4]  = bank_Effects_Glass_rn;   
  bank_Effects_array[5]  = bank_Effects_Gunshot;    
  bank_Effects_array[6]  = bank_Effects_hammer2;    
  bank_Effects_array[7]  = bank_Effects_Hammer;     
  bank_Effects_array[8]  = bank_Effects_impulses;   
  bank_Effects_array[9]  = bank_Effects_Laser;      
  bank_Effects_array[10] = bank_Effects_loop;       
  bank_Effects_array[11] = bank_Effects_Reverse;    
  bank_Effects_array[12] = bank_Effects_Ringing;    
  bank_Effects_array[13] = bank_Effects_Scissors;   
  bank_Effects_array[14] = bank_Effects_Sonar;      
  bank_Effects_array[15] = bank_Effects_thunder;    
  

  printf("MDACHECK %d %d %d\n",machineParam,paramValue1,paramValue2);

  switch (machineParam)
    {
    case OSC2_TYPE:
      if (paramValue1==MDA_BANK_TR808)
	return bank_tr808_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_TR909)
	return bank_tr909_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_TR606)
	return bank_tr606_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_TR77)
	return bank_tr77_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_CR8000)
	return bank_cr8000_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_CR78)
	return bank_cr78_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_LINN)
	return bank_Linn_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_LATIN)
	return bank_Latin_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_ELECTRO)
	return bank_Electro_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_ACOUSTIC)
	return bank_Acoustic_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_FERRARO)
	return bank_Ferraro_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_FARFISA)
	return bank_Farfisa_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_INSTRMNT)
	return bank_instrmnt_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MAGNETBOY)
	return bank_magnetboy_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_EFFECTS)
	return bank_Effects_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_JERGENSOHN)
	return bank_JergenSohn_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_R_B)
	return bank_R_B_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MISC)
	return bank_misc_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MISC_CLAPS)
	return bank_misc_claps_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MISC_BASS)
	return bank_misc_bass_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MISC_ELECTRO)
	return bank_misc_electro_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MISC_FX)
	return bank_misc_fx_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MISC_PERC)
	return bank_misc_perc_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MISC_HATS)
	return bank_misc_hats_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
      if (paramValue1==MDA_BANK_MISC_SYNTH)
	return bank_misc_synth_array[this->checkITwoVal(OSC2_TYPE,paramValue1,paramValue2)];
    // default:
    //   return str_null;
    }
  return str_null;
}

const char * MDADrumMachine::getMachineParamCharStar(int machineParam,int paramValue)
{

  static const char * str_null       = "NILL ";

  static const char * str_opl2_sine  = "  SIN";
  static const char * str_opl2_hsin  = " HSIN";
  static const char * str_opl2_absin = "ABSIN";
  static const char * str_opl2_qsin  = " PSIN";

  static const char * str_opl2_epsin = "EPSIN";
  static const char * str_opl2_asin  = " ASIN";
  static const char * str_opl2_sqr   = "  SQR";
  static const char * str_opl2_dsqr  = " DSQR";

  const char * str_osc[PICO_DBOPL_SIZE];

  static const char * str_fltr_algo_nofilter = "NOFL";
  static const char * str_fltr_algo_biquad   = "BIQU";
  static const char * str_fltr_algo_amsynth  = "AMST";
  
  const        char * str_fltr_algo[FILTER_ALGO_SIZE];


  static const char * str_fltr_type_lp   = "LP";
  static const char * str_fltr_type_bp   = "BP";
  static const char * str_fltr_type_hp   = "HP";

  const        char * str_fltr_type[FILTER_TYPE_SIZE];

  static const char * str_fm_type_am      = "2OP_AM ";
  static const char * str_fm_type_fm      = "2OP_FM ";

  const        char * str_fm_type[FM_TYPE_SIZE];


  const        char * bank_array[MDA_BANK_SIZE];

  static const char * bank_Effects                                  = "Effects";                     
  static const char * bank_cr8000                                   = "cr8000";                      
  static const char * bank_cr78                                     = "cr78";                        
  static const char * bank_Linn                                     = "Linn";                        
  static const char * bank_Latin                                    = "Latin";                       
  static const char * bank_JergenSohn                               = "JergenSohn";                  
  static const char * bank_Electro                                  = "Electro";                     
  static const char * bank_tr808                                    = "tr808";                       
  static const char * bank_Acoustic                                 = "Acoustic";                    
  static const char * bank_Ferraro                                  = "Ferraro";                     
  static const char * bank_Farfisa                                  = "Farfisa";                     
  static const char * bank_instrmnt                                 = "instrmnt";                    
  static const char * bank_magnetboy                                = "magnetboy";                   
  static const char * bank_misc_claps                               = "misc_claps";                  
  static const char * bank_misc_bass                                = "misc_bass";                   
  static const char * bank_misc                                     = "misc";                        
  static const char * bank_misc_electro                             = "misc_electro";                
  static const char * bank_misc_fx                                  = "misc_fx";                     
  static const char * bank_misc_perc                                = "misc_perc";                   
  static const char * bank_misc_hats                                = "misc_hats";                   
  static const char * bank_tr77                                     = "tr77";                        
  static const char * bank_tr606                                    = "tr606";                       
  static const char * bank_R_B                                      = "R_B";                         
  static const char * bank_misc_synth                               = "misc_synth";                  
  static const char * bank_tr909                                    = "tr909";                       
  
  bank_array[MDA_BANK_TR808]            = bank_tr808;                    
  bank_array[MDA_BANK_TR909]            = bank_tr909;                    
  bank_array[MDA_BANK_TR606]            = bank_tr606;                    
  bank_array[MDA_BANK_TR77]             = bank_tr77;                     
  bank_array[MDA_BANK_CR8000]           = bank_cr8000;                   
  bank_array[MDA_BANK_CR78]             = bank_cr78;                     
  bank_array[MDA_BANK_LINN]             = bank_Linn;                     
  bank_array[MDA_BANK_LATIN]            = bank_Latin;                    
  bank_array[MDA_BANK_ELECTRO]          = bank_Electro;                  
  bank_array[MDA_BANK_ACOUSTIC]         = bank_Acoustic;                 
  bank_array[MDA_BANK_FERRARO]          = bank_Ferraro;                  
  bank_array[MDA_BANK_FARFISA]          = bank_Farfisa;                  
  bank_array[MDA_BANK_INSTRMNT]         = bank_instrmnt;                 
  bank_array[MDA_BANK_MAGNETBOY]        = bank_magnetboy;                
  bank_array[MDA_BANK_EFFECTS]          = bank_Effects;                  
  bank_array[MDA_BANK_JERGENSOHN]       = bank_JergenSohn;               
  bank_array[MDA_BANK_R_B]              = bank_R_B;                      
  bank_array[MDA_BANK_MISC]             = bank_misc;                     
  bank_array[MDA_BANK_MISC_CLAPS]       = bank_misc_claps;               
  bank_array[MDA_BANK_MISC_BASS]        = bank_misc_bass;                
  bank_array[MDA_BANK_MISC_ELECTRO]     = bank_misc_electro;             
  bank_array[MDA_BANK_MISC_FX]          = bank_misc_fx;                  
  bank_array[MDA_BANK_MISC_PERC]        = bank_misc_perc;                
  bank_array[MDA_BANK_MISC_HATS]        = bank_misc_hats;                
  bank_array[MDA_BANK_MISC_SYNTH]       = bank_misc_synth;               
  
  








  static const char * str_drm_1       =   "patch/MDADrum/tr808/Kick.ds";
  static const char * str_drm_2       =   "patch/MDADrum/tr808/Snare.ds";
  static const char * str_drm_3       =   "patch/MDADrum/tr808/Rimshot.ds";
  static const char * str_drm_4       =   "patch/MDADrum/tr808/Ride.ds";
  
  static const char * str_drm_5       =   "patch/MDADrum/tr808/Hat_c.ds";
  static const char * str_drm_6       =   "patch/MDADrum/tr808/Hat_o.ds";

  static const char * str_drm_7       =   "patch/MDADrum/tr808/Cowbell.ds";
  static const char * str_drm_8       =   "patch/MDADrum/tr808/Clave.ds";


  const char * str_drm[PICO_DBOPL_SIZE];

  str_drm[0]                          = str_drm_1;
  str_drm[1]                          = str_drm_2;
  str_drm[2]                          = str_drm_3;
  str_drm[3]                          = str_drm_4;
  str_drm[4]                          = str_drm_5;
  str_drm[5]                          = str_drm_6;
  str_drm[6]                          = str_drm_7;
  str_drm[7]                          = str_drm_8;


  str_osc[PICO_DBOPL_SINE]            = str_opl2_sine;
  str_osc[PICO_DBOPL_HSIN]            = str_opl2_hsin;
  str_osc[PICO_DBOPL_ABSIN]           = str_opl2_absin;
  str_osc[PICO_DBOPL_QSIN]            = str_opl2_qsin;

  str_osc[PICO_DBOPL_EPSIN]           = str_opl2_epsin;
  str_osc[PICO_DBOPL_ASIN]            = str_opl2_asin;
  str_osc[PICO_DBOPL_SQR]             = str_opl2_sqr;
  str_osc[PICO_DBOPL_DSQR]            = str_opl2_dsqr;

  str_fltr_algo[FILTER_ALGO_NOFILTER] = str_fltr_algo_nofilter;
  str_fltr_algo[FILTER_ALGO_BIQUAD]   = str_fltr_algo_biquad;
  str_fltr_algo[FILTER_ALGO_AMSYNTH]  = str_fltr_algo_amsynth;

  str_fltr_type[FILTER_TYPE_LP]       = str_fltr_type_lp;
  str_fltr_type[FILTER_TYPE_BP]       = str_fltr_type_bp;
  str_fltr_type[FILTER_TYPE_HP]       = str_fltr_type_hp;

  str_fm_type[FM_TYPE_AM]             = str_fm_type_am;
  str_fm_type[FM_TYPE_FM]             = str_fm_type_fm;



  switch (machineParam)
    {
    case OSC1_TYPE:
      return bank_array[this->checkI(OSC1_TYPE,paramValue)];			

    case FILTER1_ALGO:
      return str_fltr_algo[this->checkI(FILTER1_ALGO,paramValue)];

    case FILTER1_TYPE:
      return str_fltr_type[this->checkI(FILTER1_TYPE,paramValue)];

    case FM_TYPE:
      return str_fm_type[this->checkI(FM_TYPE,paramValue)];

    }
  return str_null;
}



void MDADrumMachine::reset()
{
 sample_num=0;
}

int MDADrumMachine::checkITwoVal(int what,int val1,int val2)
{
  switch (what)
    {
    case OSC2_TYPE:
      if (val2<0) return 0;
      if (val2>127) return 127;

      if (val1==MDA_BANK_TR808) 
	if (val2>=MDA_BANK_TR808_SIZE) return MDA_BANK_TR808_SIZE-1;

      if (val1==MDA_BANK_TR909) 
	if (val2>=MDA_BANK_TR909_SIZE) return MDA_BANK_TR909_SIZE-1;

      if (val1==MDA_BANK_TR606) 
	if (val2>=MDA_BANK_TR606_SIZE) return MDA_BANK_TR606_SIZE-1;

      if (val1==MDA_BANK_TR77) 
	if (val2>=MDA_BANK_TR77_SIZE) return MDA_BANK_TR77_SIZE-1;

      if (val1==MDA_BANK_CR8000) 
	if (val2>=MDA_BANK_CR8000_SIZE) return MDA_BANK_CR8000_SIZE-1;

      if (val1==MDA_BANK_CR78) 
	if (val2>=MDA_BANK_CR78_SIZE) return MDA_BANK_CR78_SIZE-1;

      if (val1==MDA_BANK_LINN) 
	if (val2>=MDA_BANK_LINN_SIZE) return MDA_BANK_LINN_SIZE-1;

      if (val1==MDA_BANK_LATIN) 
	if (val2>=MDA_BANK_LATIN_SIZE) return MDA_BANK_LATIN_SIZE-1;

      if (val1==MDA_BANK_ELECTRO) 
	if (val2>=MDA_BANK_ELECTRO_SIZE) return MDA_BANK_ELECTRO_SIZE-1;

      if (val1==MDA_BANK_ACOUSTIC) 
	if (val2>=MDA_BANK_ACOUSTIC_SIZE) return MDA_BANK_ACOUSTIC_SIZE-1;

      if (val1==MDA_BANK_FERRARO) 
	if (val2>=MDA_BANK_FERRARO_SIZE) return MDA_BANK_FERRARO_SIZE-1;

      if (val1==MDA_BANK_FARFISA) 
	if (val2>=MDA_BANK_FARFISA_SIZE) return MDA_BANK_FARFISA_SIZE-1;

      if (val1==MDA_BANK_INSTRMNT) 
	if (val2>=MDA_BANK_INSTRMNT_SIZE) return MDA_BANK_INSTRMNT_SIZE-1;

      if (val1==MDA_BANK_MAGNETBOY) 
	if (val2>=MDA_BANK_MAGNETBOY_SIZE) return MDA_BANK_MAGNETBOY_SIZE-1;

      if (val1==MDA_BANK_EFFECTS) 
	if (val2>=MDA_BANK_EFFECTS_SIZE) return MDA_BANK_EFFECTS_SIZE-1;

      if (val1==MDA_BANK_JERGENSOHN) 
	if (val2>=MDA_BANK_JERGENSOHN_SIZE) return MDA_BANK_JERGENSOHN_SIZE-1;

      if (val1==MDA_BANK_R_B) 
	if (val2>=MDA_BANK_R_B_SIZE) return MDA_BANK_R_B_SIZE-1;

      if (val1==MDA_BANK_MISC) 
	if (val2>=MDA_BANK_MISC_SIZE) return MDA_BANK_MISC_SIZE-1;

      if (val1==MDA_BANK_MISC_CLAPS) 
	if (val2>=MDA_BANK_MISC_CLAPS_SIZE) return MDA_BANK_MISC_CLAPS_SIZE-1;

      if (val1==MDA_BANK_MISC_BASS) 
	if (val2>=MDA_BANK_MISC_BASS_SIZE) return MDA_BANK_MISC_BASS_SIZE-1;

      if (val1==MDA_BANK_MISC_ELECTRO) 
	if (val2>=MDA_BANK_MISC_ELECTRO_SIZE) return MDA_BANK_MISC_ELECTRO_SIZE-1;

      if (val1==MDA_BANK_MISC_FX) 
	if (val2>=MDA_BANK_MISC_FX_SIZE) return MDA_BANK_MISC_FX_SIZE-1;

      if (val1==MDA_BANK_MISC_PERC) 
	if (val2>=MDA_BANK_MISC_PERC_SIZE) return MDA_BANK_MISC_PERC_SIZE-1;

      if (val1==MDA_BANK_MISC_HATS) 
	if (val2>=MDA_BANK_MISC_HATS_SIZE) return MDA_BANK_MISC_HATS_SIZE-1;

      if (val1==MDA_BANK_MISC_SYNTH) 
	if (val2>=MDA_BANK_MISC_SYNTH_SIZE) return MDA_BANK_MISC_SYNTH_SIZE-1;

      return val2;
      break;

    default:
      if (val2<0)   return 0;
      if (val2>127) return 127;
      DPRINTF("WARNING: MDADrumMachine::checkITwoVal(%d,%d,%d)\n",what,val1,val2);
      return val2;
      break;      
    }  
  return val2;
}

int MDADrumMachine::checkI(int what,int val)
{
  switch (what)
    {
    case OSC1_TYPE:
      if (val<0) return 0;
      if (val>MDA_BANK_SIZE-1) return MDA_BANK_SIZE-1;
      osc1_type=val;
      return val;
      break;


    case OSC2_TYPE:
      if (val<0) return 0;      
      osc2_type=val;
      return val;
      break;

      

    case FILTER1_TYPE:
      if (val<0) return 0;
      if (val>3) return 3;
      return val;
      break;

    case FILTER1_ALGO:
      if (val<0) return 0;
      if (val>2) return 2;
      return val;
      break;


    default:
      if (val<0)   return 0;
      if (val>127) return 127;
      DPRINTF("WARNING: MDADrumMachine::checkI(%d,%d)\n",what,val);
      return val;
      break;      
    }
}


int MDADrumMachine::getI(int what)
{
  if (what==NOTE_ON) return keyon;
}

void MDADrumMachine::setF(int what,float val)
{
  //if (what==OSC1_FREQ)           { freq=val; }
}



void MDADrumMachine::setI(int what,int val)
{
  float        f_val_cutoff;
  float        f_val_resonance;


  if (what==NOTE_ON && val==1) 
    { 
      need_note_on=1;
    }
  if (what==OSC1_TYPE)           { osc1_type=this->checkI(OSC1_TYPE,val); }
  if (what==OSC2_TYPE)           { osc2_type=this->checkI(OSC2_TYPE,val); }

  if (what==NOTE1)
    {
      note=val;
      NoteFreq & NF = NoteFreq::getInstance();
      freq=NF.getINoteFreq(note);
    }
  
  if (what==FILTER1_CUTOFF)      
    { 
      cutoff=val;
      filter.setCutoff(cutoff);

  }
  if (what==FILTER1_RESONANCE)         
    { 
      resonance=val;
      filter.setResonance(resonance);
    }


  if (what==FILTER1_TYPE)     filter.setFilterType(val);
  if (what==FILTER1_ALGO)     filter.setFilterAlgo(val);


  // if (what==ADSR_ENV0_ATTACK)    {  param_t=val;       param_t=param_t/64; }
  // if (what==ADSR_ENV0_DECAY )    {  param_o=val;       param_o=param_o/64; }
  // if (what==ADSR_ENV0_SUSTAIN)   {  param_n=val;       param_n=param_n/64; }
  if (what==ADSR_ENV0_RELEASE)      {  param_time=val;    param_time=param_time/48.0; }

  // if (what==ADSR_ENV1_ATTACK)    {  param_tune=val;    param_tune=param_tune/128; }
  // if (what==ADSR_ENV1_DECAY )    {  param_time=val;    param_time=param_time/16; }


}


Sint32 MDADrumMachine::tick()
{
  int       i=0;
  Sint16 s_in=0;
  Sint16 s_out=0;
  Sint16 sum=0;
  Sint16 ret=0;

  if (index>=SAM | 
      index<0)
    index=0;


  if (need_note_on==1)
    {
      std::string  prefix="patch/MDADrum";
      std::string  bank=this->getMachineParamCharStar(OSC1_TYPE,osc1_type);
      std::string  sound=this->getMachineTwoParamCharStar(OSC2_TYPE,osc1_type,osc2_type);
      std::string  path=prefix+"/"+bank+"/"+sound+".ds";
      buffer_size=SAM;
      index=0;
      DPRINTF("MDA buffer_size: %d buffer:0x%08.8X\n",buffer_size,buffer);
      DPRINTF("MDA path: %s \n",path.c_str());
      dsoop.init();
      dsoop.set_tune(1.0+(float)freq/6.8);
      dsoop.set_time(param_time);
      dsoop.load_patch(path.c_str());
      
      need_note_on=0;
      note_on=1;
    }
  if (note_on)
    {
      if (index==0)
	{

	  ret=dsoop.generate(buffer,buffer_size);

	  // printf("~~");
	  //for (i=0;i<SAM;i++)
	  //sum=buffer[i]+sum;

	  //DPRINTF("MDA GENERATE : %d %d",sum,ret);
	  //   printf("%d",buffer[i]);
	  // printf("\n");
	}
      s_in=buffer[index]/8;
      //s_out=filter.process(s_in);
      s_out=filter.process_one_sample(s_in);
       	//return buffer[++index]/4;
      // else
      //  	note_on=0;
    }
  index++;
  
  return s_out;
}

