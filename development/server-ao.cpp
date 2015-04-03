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
#include <sys/mman.h>
#include <stdlib.h>
#include <cstdlib>

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

struct parsed_options{
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

int comedi_internal_trigger(comedi_t *dev, unsigned int subd, unsigned
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
        char buf[100];

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



#define CJMLEN 100
#define BUF_LEN 0x1000
int main(void){
  int AMP = 4000;
  int OFFSET = 2048;
  comedi_cmd cmd;
  int err;
  int n,m;
  comedi_t *dev;
  unsigned int chanlist[16];
  int ret;
  struct parsed_options options;
  int fn;

  short int cjm[CJMLEN];
  sampl_t data[BUF_LEN];

  int i;
  
  options.subdevice = -1;

  // CJM
  strncpy(options.filename, "/dev/comedi0", 256);
  options.subdevice = 1;
  options.freq = 120;


  fn = 0;

  /* Force n_chan to be 1 */
  options.n_chan = 1;

  dev = comedi_open(options.filename);
  if(dev == NULL){
        fprintf(stderr, "error opening %s\n", options.filename);
        return -1;
  }

  memset(&cmd,0,sizeof(cmd));
  cmd.subdev = options.subdevice;
  cmd.flags = CMDF_WRITE;
  cmd.start_src = TRIG_INT;
  cmd.start_arg = 0;
  cmd.scan_begin_src = TRIG_TIMER;
  cmd.scan_begin_arg = 1e9 / options.freq;
  cmd.convert_src = TRIG_NOW;
  cmd.convert_arg = 0;
  cmd.scan_end_src = TRIG_COUNT;
  cmd.scan_end_arg = options.n_chan;
  cmd.stop_src = TRIG_NONE;
  cmd.stop_arg = 0;

  cmd.chanlist = chanlist;
  cmd.chanlist_len = options.n_chan;

  chanlist[0] = CR_PACK(options.channel, options.range, options.aref);
  //chanlist[1] = CR_PACK(options.channel + 1, options.range,

  dump_cmd(stdout,&cmd);
  


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
	//int last;
	ssize_t bytes_recieved;
	char incomming_data_buffer[buffer_size];
	int separator;
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
			
			std::cout << f << std::endl;
			std::cout << l << std::endl;
			
			float xpos = (f/xmax)*AMP-OFFSET;
			float ypos = (l/ymax)*AMP-OFFSET;
			
			write(comedi_fileno(dev),(void *)&(xpos),sizeof(float));
		}
	}

}
