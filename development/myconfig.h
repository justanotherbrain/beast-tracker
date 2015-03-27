/*----------------------- Share Memory Declaration ------------------------------*/
/* #define AD_RANGE 4096		 NI PCI6071E */
/* #define AD_RANGE 65536		 NI PCI6035E */

/*---------------------------Select features---------------------*/
#define ANALOG_STATE_FEATURE 		1 /* 1 diff channel */
#define STIM_FEATURE			1 /* 2 channels */
#define EYE_FEATURE			1 /* 2 diff channels */
#define HAND_FEATURE			1 /* 2 diff channels */
#define DISPLAY_FEATURE			1 /* 1 diff channel */
#define RAW_SET_1_FEATURE		1 /* 4 contact channels */
#define MU_SET_1_FEATURE		1 /* 4 contact channels */
#define RAW_SET_2_FEATURE               0 /* 2 se channels */
#define MU_SET_2_FEATURE                0 /* 2 se channels */

/* ----------------Specific constants for code------------------- */
#define SHARE_KEY 		6398	/* The Key to create share memory */
#define SHARE_SEM 		5215	/* The Key to create sempahore */
#define AD_RANGE_RECORD 	4096 	/* NI PCI6071E */
#define AD_RANGE_STIM 		65536 	/* NI PCI6035E */
#define RECORD_CHILD 		"/mnt/raid/record_stimulate/trunk/record_child"
#define STIM_CHILD 		"/mnt/raid/record_stimulate/trunk/stim_child"
#define CH_RECORD     		16	/* total number of channels in acquisition */
#define RAW_OUTER 		4	/* Width of raw circular buffer */
#define MU_OUTER 		4	/* Width of mu circular buffer  */
#define LFP_OUTER 		4	/* Width of lfp circular buffer */
#define CH_STIM			1	/* stim channels */

/* ----------------Generic constants for code------------------- */
#define KHZ 		1000	/* Kilhohertz in Hz */
#define SR 		20000	/* Sampling Rate */
#define SS 		2	/* sample size */
#define DUR 		100	/* Duration of SHM buffer in seconds */
#define PERIOD 		7500	/* Tunable sleep paramter in acquisition loop */
#define VERBOSE 	0	/* 0 = not verbose, 1 = verbose */
#define COMEDI_DEVICE_STIM "/dev/comedi0"
#define COMEDI_DEVICE_RECORD "/dev/comedi1"
#define HIST 		SR/KHZ	/* state buffer size in elements */
#define DT 		SR/KHZ	/* stim buffer size in elements */
#define STIM_HIST 	2	/* history buffer size in elements */
#define DISPLAY_HIST 	HIST	/* display buffer size in elements */

/* ----------------Configure analog state---------------------- */
#define INPUT_STATE_CHANNEL 	0		/* A/D channel index */
#define OUTPUT_STATE_CHANNEL 	0		/* File channel index */
#define STATE_SR 		SR      	/* Sampling rate */
#define AREF_STATE 		AREF_DIFF
#define RAWSTATE_FILE 		"file.state.dat"
#define EV_FILE 		"file.ev.txt"

/*-----------------Configure display sensor------------------- */
#define INPUT_DISPLAY_CHANNEL 	5		/* A/D channel index */
#define OUTPUT_DISPLAY_CHANNEL 	5        	/* File channel index*/
#define DISPLAY_SR 		SR	       	/* Sampling rate */
#define AREF_DISPLAY 		AREF_DIFF
#define DISPLAYRAW_FILE 	"file.display.dat"
#define DISPLAYTXT_FILE 	"file.display.txt"

/*-----------------Configure eye position---------------------*/
#define INPUT_EYE_CHANNEL_HOR 	1         	/* A/D channel index */
#define INPUT_EYE_CHANNEL_VER 	2         	/* A/D channel index */
#define OUTPUT_EYE_CHANNEL_HOR 	1        	/* File channel index */
#define OUTPUT_EYE_CHANNEL_VER 	2        	/* File channel index */
#define EYE_SR 			KHZ	 	/* Sampling rate */
#define AREF_EYE 		AREF_DIFF
#define EYE_FILE 		"file.eye.dat"

/*-----------------Configure hand position------------------*/
#define INPUT_HAND_CHANNEL_HOR 	3        	/* A/D channel index */
#define INPUT_HAND_CHANNEL_VER 	4        	/* A/D channel index */
#define OUTPUT_HAND_CHANNEL_HOR 3       	/* File channel index */
#define OUTPUT_HAND_CHANNEL_VER 4       	/* File channel index */
#define AREF_HAND 		AREF_DIFF
#define HAND_SR 		KHZ	       	/* Sampling rate */
#define HAND_FILE 		"file.hnd.dat"

/*-----------------Configure raw neural------------------*/
#define INPUT_RAW_SET_1_CHANNEL_1 	18  	/* A/D channel index */
#define INPUT_RAW_SET_1_CHANNEL_2 	21  	/* A/D channel index */
#define INPUT_RAW_SET_1_CHANNEL_3 	34  	/* A/D channel index */
#define INPUT_RAW_SET_1_CHANNEL_4 	37  	/* A/D channel index */
#define OUTPUT_RAW_SET_1_CHANNEL_1 	8  	/* File channel index */
#define OUTPUT_RAW_SET_1_CHANNEL_2 	9 	/* File channel index */
#define OUTPUT_RAW_SET_1_CHANNEL_3 	10 	/* File channel index */
#define OUTPUT_RAW_SET_1_CHANNEL_4 	11 	/* File channel index */
#define AREF_RAW 			AREF_GROUND
#define RAW_SR 				SR	/* Sampling rate */
#define RAW_FILE 			"file.raw.dat"

/*-----------------Configure multiunit neural------------------*/
#define AREF_MU 			AREF_GROUND
#define INPUT_MU_SET_1_CHANNEL_1 	18  	/* A/D channel index */
#define INPUT_MU_SET_1_CHANNEL_2 	21  	/* A/D channel index */
#define INPUT_MU_SET_1_CHANNEL_3 	34  	/* A/D channel index */
#define INPUT_MU_SET_1_CHANNEL_4 	37  	/* A/D channel index */
#define OUTPUT_MU_SET_1_CHANNEL_1 	12  	/* File channel index */
#define OUTPUT_MU_SET_1_CHANNEL_2 	13 	/* File channel index */
#define OUTPUT_MU_SET_1_CHANNEL_3 	14 	/* File channel index */
#define OUTPUT_MU_SET_1_CHANNEL_4 	15 	/* File channel index */
#define MU_SR 				SR	/* Sampling rate */
#define LFP_SR 				SR	/* Sampling rate */

/*-----------------Configure raw neural------------------*/
#define INPUT_RAW_SET_2_CHANNEL_1 	19  	/* A/D channel index */
#define INPUT_RAW_SET_2_CHANNEL_2 	22  	/* A/D channel index */
#define INPUT_RAW_SET_2_CHANNEL_3 	35  	/* A/D channel index */
#define INPUT_RAW_SET_2_CHANNEL_4 	38  	/* A/D channel index */
#define OUTPUT_RAW_SET_2_CHANNEL_1 	10  	/* File channel index */
#define OUTPUT_RAW_SET_2_CHANNEL_2 	11 	/* File channel index */
#define OUTPUT_RAW_SET_2_CHANNEL_3 	12 	/* File channel index */
#define OUTPUT_RAW_SET_2_CHANNEL_4 	13 	/* File channel index */

/*-----------------Configure multiunit neural------------------*/
#define INPUT_MU_SET_2_CHANNEL_1 	19  	/* A/D channel index */
#define INPUT_MU_SET_2_CHANNEL_2 	22  	/* A/D channel index */
#define INPUT_MU_SET_2_CHANNEL_3 	35  	/* A/D channel index */
#define INPUT_MU_SET_2_CHANNEL_4 	38  	/* A/D channel index */
#define OUTPUT_MU_SET_2_CHANNEL_1 	9  	/* File channel index */
#define OUTPUT_MU_SET_2_CHANNEL_2 	11 	/* File channel index */
#define OUTPUT_MU_SET_2_CHANNEL_3 	13 	/* File channel index */
#define OUTPUT_MU_SET_2_CHANNEL_4 	15 	/* File channel index */

/*-----------------Configure stimulation-------------------*/
#define INPUT_STIMPULSE_CHANNEL 	6     	/* A/D channel stim pulse index */
#define INPUT_STIMRAW_CHANNEL 		7       /* A/D channel stim raw index */
#define OUTPUT_STIMPULSE_CHANNEL 	6    	/* File channel stim pulse index */
#define OUTPUT_STIMRAW_CHANNEL 		7      	/* File channel stim raw index */
#define AREF_STIM 			AREF_DIFF
#define STIMPULSE_FILE 			"file.stimpulse.dat"
#define STIMRAW_FILE 			"file.stimraw.dat"
#define STIMTXT_FILE 			"file.stim.txt"

/*--------------Configure stimulation ------------------*/
#define STIM_SR		1000
#define STIM_DUR		1

/*------------------------Pipe preproc parameters-------------------------------------*/
#define STIM_THRESH 		3000
#define DISPLAY_THRESHOLD	3000 /* Depends on which NI card */

/* ---------------------- Shared memory parameters ----------------------------------------*/
#define STATE_OUTER 		3  	/* Width of state circular buffer (trial,state,time) */
#define STATE_INNER 		1000  	/* Length of state circular buffer                 */
#define STIM_OUTER 		2           /* Width of stim circular buffer (trial,time) */
#define STIM_INNER 		1000        /* Length of stim circular buffer                 */
#define DISPLAY_OUTER 		2           /* Width of display circular buffer (trial,time) */
#define DISPLAY_INNER 		1000        /* Length of display circular buffer                 */

#define EYE_OUTER 		2    		/* Width of eye position circular buffer (x,y)     */
#define EYE_INNER 		DUR*EYE_SR  /* Length of eye position circular buffer          */
#define HAND_OUTER 		2   /* Width of hand position circular buffer (x,y)     */
#define HAND_INNER 		DUR*HAND_SR  /* Length of hand position circular buffer          */

#define RAW_INNER 		DUR*RAW_SR  /* Length of raw circular buffer                   */
#define MU_INNER 		DUR*MU_SR  /* Length of mu circular buffer                   */
#define LFP_INNER 		DUR*LFP_SR  /* Length of lfp circular buffer                   */

#define STIMPULSE_OUTER 	1
#define STIMPULSE_INNER 	DUR*STIM_SR  /* Length of stim circular buffer                   */
#define STIMRAW_OUTER 		1
#define STIMRAW_INNER 		DUR*STIM_SR  /* Length of stim circular buffer                   */
#define DISPLAYRAW_OUTER	1
#define DISPLAYRAW_INNER 	DUR*DISPLAY_SR  /* Length of raw display sensor circular buffer */
#define STATERAW_OUTER 		1
#define STATERAW_INNER 		DUR*DISPLAY_SR  /* Length of raw analog state circular buffer */

#define AOSTIM_INNER		DUR*RAW_SR /* Length of stimulation sequence buffer */

#define BURST_INNER		1000 /* Length of stimulation buffer. Should equal longest stim sequence in MS */

/*-------------------------------------------------------------------------------*/
int shm_id;
typedef struct shared_use_st 
{
	short int file_state;
	char file_name[128];
	unsigned long  int  time;
        int display_threshold;
	unsigned long  int  analog_state_index;
	unsigned long  int  display_state_index;
	unsigned long  int  state[STATE_OUTER][STATE_INNER];
	unsigned short int  state_pos;
	unsigned long  int  eye_pos;
	unsigned long  int  hand_pos;
	unsigned long  int  raw_pos;
	unsigned long  int  mu_pos;
	unsigned long  int  lfp_pos;
	unsigned long  int  stim_pos;
        unsigned long  int  stimraw_pos;
        unsigned long  int  stimpulse_pos;
	unsigned long  int  displayraw_pos;
	unsigned long  int  display_pos;
	unsigned long  int  stateraw_pos;
        unsigned long  int  stateraw[STATERAW_OUTER][STATERAW_INNER];
        unsigned long  int  displayraw[DISPLAYRAW_OUTER][DISPLAYRAW_INNER];
	unsigned long  int  stimpulse[STIMPULSE_OUTER][STIMPULSE_INNER];
        unsigned long  int  stimraw[STIMRAW_OUTER][STIMRAW_INNER];
/*        float               stim[STIM_OUTER][STIM_INNER]; */
	unsigned long  int  analog_state_buffer[STATE_OUTER][STATE_INNER];
	unsigned long  int  display_state_buffer[DISPLAY_OUTER][DISPLAY_INNER];
	unsigned short int  eye[EYE_OUTER][EYE_INNER];
	unsigned short int  hand[HAND_OUTER][HAND_INNER];
	unsigned short int  raw[RAW_OUTER][RAW_INNER];
	unsigned short int  mu[MU_OUTER][MU_INNER];
        unsigned short int  lfp[LFP_OUTER][LFP_INNER];
	unsigned short      aostim[AOSTIM_INNER];
	unsigned long int   stim_on;
	unsigned long int   stim_threshold;
	unsigned long int   stim_behavior_state;
        unsigned short int  check;
        unsigned short int  stim[BURST_INNER*STIM_SR/1000];
	unsigned short int  stim_length;
}shared_use_st;
/*----------------------------------------------------------------------*/
