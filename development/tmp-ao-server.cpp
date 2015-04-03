#include <stdio.h>
#include <comedilib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <cstdlib>
//#include "/usr/local/src/new/comedilib/demo/examples.h"
#include "myconfig.h"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <array>

comedi_t *dev;
#define SAMPLE_CT 100

// Initialize xpos
float xpos = 0;
// Initialize ypos
float ypos = 0;
/* frequency of the sine wave to output */
double waveform_frequency	= 10.0;

/* peak-to-peak amplitude, in DAC units (i.e., 0-4095) */
double amplitude		= 4000;

/* offset, in DAC units */
double offset			= 2048;

/* This is the size of chunks we deal with when creating and
   outputting */
//#define BUF_LEN 10
#define BUF_LEN 0x8000

const char *comdevice = COMEDI_DEVICE_AO;


int external_trigger_number = 0;

sampl_t data[SAMPLE_CT];

void dds_output(sampl_t *buf,int n);
void dds_init(double waveform_frequency, double update_frequency, int fn);

void dds_init_sine(void);

struct parsed_options_hold{
  char filename[256];
  double value;
  int subdevice;
  int channel;
  int aref;
  int range;
  int physical;
  int verbose;
  int n_chan;
  int n_scan;
  double freq;
};



char *cmd_src(int src,char *buf)
{
        buf[0]=0;

        if(src&TRIG_NONE)strcat(buf,"none|");
        if(src&TRIG_NOW)strcat(buf,"now|");
        if(src&TRIG_FOLLOW)strcat(buf, "follow|");
        if(src&TRIG_TIME)strcat(buf, "time|");
        if(src&TRIG_TIMER)strcat(buf, "timer|");
        if(src&TRIG_COUNT)strcat(buf, "count|");
        if(src&TRIG_EXT)strcat(buf, "ext|");
        if(src&TRIG_INT)strcat(buf, "int|");
        if(src&TRIG_OTHER)strcat(buf, "other|");

        if(strlen(buf)==0){
                sprintf(buf,"unknown(0x%08x)",src);
        }else{
                buf[strlen(buf)-1]=0;
        }

        return buf;
}

void dump_cmd(FILE *out,comedi_cmd *cmd)
{
        char buf[10];

        fprintf(out,"subdevice:      %d\n",
                cmd->subdev);

        fprintf(out,"start:      %-8s %d\n",
                cmd_src(cmd->start_src,buf),
                cmd->start_arg);

        fprintf(out,"scan_begin: %-8s %d\n",
                cmd_src(cmd->scan_begin_src,buf),
                cmd->scan_begin_arg);

        fprintf(out,"convert:    %-8s %d\n",
                cmd_src(cmd->convert_src,buf),
                cmd->convert_arg);

        fprintf(out,"scan_end:   %-8s %d\n",
                cmd_src(cmd->scan_end_src,buf),
                cmd->scan_end_arg);

        fprintf(out,"stop:       %-8s %d\n",
                cmd_src(cmd->stop_src,buf),
                cmd->stop_arg);
}


int comedi_internal_trigger(unsigned int subd, unsigned
int trignum)
{
  comedi_insn insn;
  lsampl_t data[1];

  memset(&insn, 0, sizeof(comedi_insn));
  insn.insn = INSN_INTTRIG;
  insn.subdev = subd;
  insn.data = data;
  insn.n = 1;

  data[0] = trignum;

  return comedi_do_insn(dev, &insn);
}


static void (* const dds_init_function[])(void) = {
	dds_init_sine,
};

#define NUMFUNCS	(sizeof(dds_init_function)/sizeof(dds_init_function[0]))

int main(int argc, char *argv[])
{
	comedi_cmd cmd;
	int err;
	int n,m, i;
	int total=0, n_chan = 0, freq = 80000;
	int subdevice = -1;
	int verbose = 0;
	comedi_t *dev;
	unsigned int chanlist[16];
	unsigned int maxdata;
	comedi_range *rng;
	int ret;
	//struct parsed_options options;
	int fn;
	int aref = AREF_GROUND;
	int range = 0;
	int channel = 0;
	int buffer_length;
	subdevice = -1;
	n_chan = -1;


	/* Use n_chan to select waveform (cheat!) */
	fn = n_chan;

	/* Force n_chan to be 1 */
	n_chan = 1;
	
	dev = comedi_open(comdevice);
	if(dev == NULL){
		fprintf(stderr, "error opening %s\n", comdevice);
		return -1;
	}
	
	if(subdevice <0)
		subdevice = comedi_find_subdevice_by_type(dev, COMEDI_SUBD_AO, 0);
	assert(subdevice >= 0);	

	maxdata = comedi_get_maxdata(dev, subdevice, 0);
	rng = comedi_get_range(dev, subdevice, 0, 0);

	offset = (double)comedi_from_phys(0.0, rng, maxdata);
	amplitude = (double)comedi_from_phys(1.0, rng, maxdata) - offset;

	memset(&cmd,0,sizeof(cmd));
	cmd.subdev = subdevice;
	cmd.flags = CMDF_WRITE;
	cmd.start_src = TRIG_INT;
	cmd.start_arg = 0;
	cmd.scan_begin_src = TRIG_TIMER;
	cmd.scan_begin_arg = 1e9 / freq;
	cmd.convert_src = TRIG_NOW;
	cmd.convert_arg = 0;
	cmd.scan_end_src = TRIG_COUNT;
	cmd.scan_end_arg = n_chan;
	cmd.stop_src = TRIG_COUNT;
	cmd.stop_arg = SAMPLE_CT;

	cmd.chanlist = chanlist;
	cmd.chanlist_len = n_chan;

	chanlist[0] = CR_PACK(channel, range, aref);

	dds_init(waveform_frequency, freq, fn);

	if (verbose) 
		dump_cmd(stdout,&cmd);

	err = comedi_command_test(dev, &cmd);
	if (err < 0) {
		comedi_perror("comedi_command_test");
		exit(1);
	}

	err = comedi_command_test(dev, &cmd);
	if (err < 0) {
		comedi_perror("comedi_command_test");
		exit(1);
	}

	if ((err = comedi_command(dev, &cmd)) < 0) {
		comedi_perror("comedi_command");
		exit(1);
	}

	//dds_output(data,BUF_LEN);
	n = SAMPLE_CT * sizeof(sampl_t);
	data[SAMPLE_CT - 1] = offset;
	for(i=0; i<SAMPLE_CT; i++){
		if(i%10 < 5)
			data[i]=rint(offset);
		else if(i%10 >=5)
			data[i]=rint(offset+amplitude);
	}
	m = write(comedi_fileno(dev), (void *)data, n);
	if(m < 0){
		perror("write");
		exit(1);
	}else if(m < n)
	{
		fprintf(stderr, "failed to preload output buffer with %i bytes, is it too small?\n"
			"See the --write-buffer option of comedi_config\n", n);
		exit(1);
	}
	if (verbose)
		printf("m=%d\n",m);

	ret = comedi_internal_trigger(dev, subdevice, 0);
	if(ret < 0){
		perror("comedi_internal_trigger\n");
		exit(1);
	}
	
	comedi_cancel(dev,subdevice);
	
        // set up socket and listen
        int status;
        struct addrinfo host_info;
        struct addrinfo *host_info_list;

        memset(&host_info, 0, sizeof host_info);

        std::cout << "Setting up the structs..." << std::endl;

        host_info.ai_family = AF_UNSPEC;
        host_info.ai_socktype = SOCK_STREAM;
        host_info.ai_flags = AI_PASSIVE;

        status = getaddrinfo(NULL, "5555", &host_info, &host_info_list);
        if (status != 0) std::cout << "getaddrinfo error" << gai_strerror(status);

        std::cout << "Creating a socket..." << std::endl;
        int socketfd;
        socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
        if (socketfd == -1) std::cout << std::endl;

        std::cout << "Binding socket..." << std::endl;
        int yes = 1;
        status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        status = bind(socketfd, host_info_list->ai_addr,host_info_list->ai_addrlen);
        if (status == -1) std::cout << "bind error" << std::endl;

        std::cout << "Listening for connections..." << std::endl;
        status = listen(socketfd,1);
        if (status == -1) std::cout << "listen error" << std::endl;

        int new_sd;
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        new_sd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
        if (new_sd == -1){
                std::cout << "listen error" << std::endl;
        }
        else{
                std::cout << "Connection accepted. Using new socketfd: " << new_sd << std::endl;
        }


        float xmax = 1280;
        float ymax = 960;
        int buffer_size = 30;
        ssize_t bytes_recieved;
        char incomming_data_buffer[buffer_size];
        int separator;
//	amplitude = 4000;

	for(;;){
	        bytes_recieved = recv(new_sd, incomming_data_buffer, buffer_size, 0);
                if (incomming_data_buffer[0] == 'q'){
                        std::cout << "Eye-tracker closed" << std::endl;
                        freeaddrinfo(host_info_list);
                        shutdown(new_sd,2);
                        shutdown(socketfd,2);
                        return 0;
		}
		else{
                        // parse incomming data buffer
                        for (int i = 0; i < buffer_size; i = i+1){
                                if (incomming_data_buffer[i]==','){
                                        separator = i;
                                }
                        }
                        char first[separator];
                        char last[100-separator];
                        for (int i=0; i<separator; i = i+1){
                                first[i]=incomming_data_buffer[i];
                        }

                        for (int i=0; i<buffer_size-separator; i=i+1){
                                last[i] = incomming_data_buffer[i+separator+1];
                        }


                        float f = std::atof (first);
                        float l = std::atof (last);

			//set xpos and ypos
                        xpos = (f/xmax)*amplitude+offset;
                        ypos = (l/ymax)*amplitude+offset;
			
//			std::cout << xpos << std::endl;
			//dds_output(data,BUF_LEN);
	        	err = comedi_command(dev, &cmd);
			assert(err >= 0);
			n = SAMPLE_CT * sizeof(sampl_t);
	        	data[SAMPLE_CT - 1] = offset;
		        for(i=0; i<sizeof(data); i++){
				data[i] = xpos;
//                		if(i%10 < 5)
//        		                data[i]=rint(offset);
//	                	else if(i%10 >=5)
//        	                	data[i]=rint(offset+amplitude);
		        }
			std::cout << data[0] << std::endl;
			std::cout << data[6] << std::endl;
        		m = write(comedi_fileno(dev), (void *)data, n);
			assert(m==n);
			ret = comedi_internal_trigger(dev,subdevice,0);
			usleep(1.1e3);
			comedi_cancel(dev,subdevice);
//			dds_output(data,BUF_LEN);
//			n=BUF_LEN*sizeof(sampl_t);
//
//       		data[BUF_LEN - 1] = offset;
//        		for(i=0; i<1000; i++){
//                        	data[i]=rint(xpos);
//		                if(i%1000 < 500)
//                		        data[i]=rint(offset);
//		                else if(i%1000 >=500)
//	               		        data[i]=rint(offset+amplitude);
//        		}
//			std::cout << data[0] << std::endl;
//        		
//			m = write(comedi_fileno(dev), (void *)data, n);
//			ret = comedi_internal_trigger(dev, subdevice, 0);
		        //if(ret < 0){
		        //        perror("comedi_internal_trigger\n");
		        //        exit(1);
		        //}

			//usleep(1.1e4);
			//assert(ret>=0);
			//comedi_cancel(dev, subdevice);
			//assert(err >= 0);
		}
	}

	return 0;
}



#define WAVEFORM_SHIFT 16
#define WAVEFORM_LEN (1<<WAVEFORM_SHIFT)
#define WAVEFORM_MASK (WAVEFORM_LEN-1)


sampl_t waveform[WAVEFORM_LEN];

unsigned int acc;
unsigned int adder;

void dds_init(double waveform_frequency, double update_frequency, int fn)
{
	adder = waveform_frequency / update_frequency * (1 << 16) * (1 << WAVEFORM_SHIFT);

	(*dds_init_function[fn])();
}

void dds_output(sampl_t *buf,int n)
{
	int i;
	sampl_t *p=buf;

	for(i=0;i<n;i++){
		*p=waveform[(acc>>16)&WAVEFORM_MASK];
		p++;
		acc+=adder;
	}
}

/* Defined for x in [0,1] */
static inline double triangle(double x)
{
	return (x > 0.5) ? 1.0 - x : x;
}

void dds_init_sine(void)
{
	int i;

	for(i=0;i<WAVEFORM_LEN;i++){
		waveform[i]=rint(xpos);
	}
}

