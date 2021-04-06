#define LAST

#ifdef LAST


#define HAND 100
#define UNDERARM 200
#define OVERARM 203
#pragma pack(push, 1)
typedef struct data100{
	unsigned char sw[4];
	unsigned char pg[2];
}data100_t;

typedef struct data200{
	float vect_underarm[9];
	unsigned char sw[4];
	unsigned char pg[2];
}data200_t;

typedef struct data300{
	float vect_overarm[3];
	float vect_handpoint[3]; // handpoint[1] handpoint[2]
	float vect_underarm[9];	//  2 = < -0.8 == Hold       8 + painting -leftclick 
	unsigned char sw[4];
	unsigned char pg[2];
}data300_t;

struct packet100{
	char start_byte;
	char second_byte;
	char direct_byte;
	char size_byte;
	char command_byte;
	data100_t data;
	char crc_byte;
};


struct packet200{
	char start_byte;
	char second_byte;
	char direct_byte;
	char size_byte;
	char command_byte;
	data200_t data;
	char crc_byte;
};


struct packet300{
	char start_byte;
	char second_byte;
	char direct_byte;
	char size_byte;
	char command_byte;
	data300_t data;
	char crc_byte;
};


#pragma pack(pop)



#define INBUF_SIZE 1280
enum MSP_protocol_bytes {
	IDLE,
	HEADER_START,
	HEADER_M,
	HEADER_ARROW,
	HEADER_SIZE,
	HEADER_CMD
};

enum Serial_Com{
	COM_UNRECEIVED,
	COM_RECEIVED
};

typedef unsigned char uint8_t;

static data100 data_hand;
static data200 data_underarm;
static data300 data_overarm;




static struct packet100 packet_hand;
static struct packet200 packet_underarm;
static struct packet300 packet_overarm;

static uint8_t inBuf[INBUF_SIZE];
static uint8_t checksum;
static uint8_t indRX;
static uint8_t cmdMSP;


static uint8_t read8()  {
	return inBuf[indRX++] & 0xff;
}
static void s_struct_w(uint8_t *cb, uint8_t siz) {
	while (siz--) *cb++ = read8();
}

int evaluateCommand(uint8_t c);


int serialCom(Serial* SP) {
	uint8_t cc, state;
	char c;
	static uint8_t offset;
	static uint8_t dataSize;
	static uint8_t c_state;
	int ret = 0;
	static long long a, b = 0;
	while ((cc = SP->ReadData(&c, 1)) == 1)
	{
		// write(serial_fd,&c,1);
		//printf("\n%d\n",c);
		state = c_state;
		// regular data handling to detect and handle MSP and other data
		if (state == IDLE) {
			a++;
			//printf("%d ",a);
			if (c == '$') state = HEADER_START;

		}
		else if (state == HEADER_START) {
			state = (c == 'M') ? HEADER_M : IDLE;
		}
		else if (state == HEADER_M) {
			state = (c == '<') ? HEADER_ARROW : IDLE;
		}
		else if (state == HEADER_ARROW) {
			if ((unsigned char)c > INBUF_SIZE) {  // now we are expecting the payload size
				state = IDLE;
				//printf("\nNONONONONONONO\n");
				continue;
			}
			dataSize = c;
			checksum = c;
			offset = 0;
			indRX = 0;
			state = HEADER_SIZE;  // the command is to follow
		}
		else if (state == HEADER_SIZE) {
			cmdMSP = c;
			checksum ^= c;
			state = HEADER_CMD;
		}
		else if (state == HEADER_CMD) {
			if ((unsigned char)offset < (unsigned char)dataSize) {
				checksum ^= c;
				inBuf[offset++] = c;
			}
			else {
				if ((unsigned char)checksum == (unsigned char)c){ // compare calculated and transferred checksum

					b++;
					//printf(" %d\n",b);
					ret = evaluateCommand(cmdMSP); // we got a valid packet, evaluate it
				}
				state = IDLE;
				cc = 0; // no more than one MSP per port and per cycle
			}
		}
		c_state = state;
	}
	return ret;
}


int evaluateCommand(uint8_t c) {
	switch (c) {
	case HAND:
		s_struct_w((uint8_t*)&data_hand, sizeof(data_hand));

		return COM_RECEIVED;

		break;

	case UNDERARM:
		s_struct_w((uint8_t*)&data_underarm, sizeof(data_underarm));
		return COM_RECEIVED;

		break;

	case OVERARM:
		s_struct_w((uint8_t*)&data_overarm, sizeof(data_overarm));
		//printf("\n%lf %lf %lf",data_overarm.vect_underarm[0],data_overarm.vect_underarm[3],data_overarm.vect_underarm[6]);
		//if(data_overarm.vect_overarm[0] > 0.9)
		//printf("\n%lf %lf %lf",data_overarm.vect_overarm[0],data_overarm.vect_overarm[1],data_overarm.vect_overarm[2]);

		//     printf("\n%lf %lf %lf",data_overarm.vect_handpoint[0],data_overarm.vect_handpoint[1],data_overarm.vect_handpoint[2]);

		//printf("\n%d %d %d %d", MouseFlag, data_overarm.sw[0], data_overarm.pg[0], data_overarm.pg[1]);
		return COM_RECEIVED;

		break;

	}
	return COM_UNRECEIVED;

}
// application reads from the specified serial port and reports the collected data


#else



#define HAND 100
#define UNDERARM 200
#define OVERARM 203
#define RESPONSE 233
#pragma pack(push, 1)
typedef struct data100{
	unsigned char sw[4];
	unsigned char pg[2];
}data100_t;

typedef struct data200{
	float vect_underarm[9];
	unsigned char sw[4];
	unsigned char pg[2];
}data200_t;

typedef struct data300{
	float vect_overarm[3];
	float vect_handpoint[3]; // handpoint[1] handpoint[2]
	float vect_underarm[9];
	unsigned char sw[4];
	unsigned char pg[2];
}data300_t;

struct packet100{
	char start_byte;
	char second_byte;
	char direct_byte;
	char size_byte;
	char command_byte;
	data100_t data;
	char crc_byte;
};


struct packet200{
	char start_byte;
	char second_byte;
	char direct_byte;
	char size_byte;
	char command_byte;
	data200_t data;
	char crc_byte;
};


struct packet300{
	char start_byte;
	char second_byte;
	char direct_byte;
	char size_byte;
	char command_byte;
	data300_t data;
	char crc_byte;
};


struct packetResponse{
	char start_byte;
	char second_byte;
	char direct_byte;
	char size_byte;
	char command_byte;
	unsigned long long int timestamp;
	char crc_byte;
};
#pragma pack(pop)

#define INBUF_SIZE 1280
enum MSP_protocol_bytes {
	IDLE,
	HEADER_START,
	HEADER_M,
	HEADER_ARROW,
	HEADER_SIZE,
	HEADER_CMD
};

enum Serial_Com{
	COM_UNRECEIVED,
	COM_RECEIVED
};

static DWORD receive_stamp = 0;

typedef unsigned char uint8_t;

static data100 data_hand;
static data200 data_underarm;
static data300 data_overarm;

static struct packet100 packet_hand;
static struct packet200 packet_underarm;
static struct packet300 packet_overarm;

static uint8_t inBuf[INBUF_SIZE];
static uint8_t checksum;
static uint8_t indRX;
static uint8_t cmdMSP;


static uint8_t read8()  {
	return inBuf[indRX++] & 0xff;
}
static void s_struct_w(uint8_t *cb, uint8_t siz) {
	while (siz--) *cb++ = read8();
}

#define LPF_FILTER 7.9577e-3

float lowpassfilter_x(float input, float dt) { // dt in milliseconds
	static float last_input = 0;
	/// Low pass filter cut frequency for derivative calculation.
	float filter = LPF_FILTER; // Set to  "1 / ( 2 * PI * f_cut )";
	// Examples for _filter:
	//    float filter = 5.3052e-3; // Set to  "1 / ( 2 * PI * f_cut )";
	// f_cut = 10 Hz -> _filter = 15.9155e-3
	// f_cut = 15 Hz -> _filter = 10.6103e-3
	// f_cut = 20 Hz -> _filter =  7.9577e-3
	// f_cut = 25 Hz -> _filter =  6.3662e-3
	// f_cut = 30 Hz -> _filter =  5.3052e-3
	float wannaend = 0;

	// discrete low pass filter, cuts out the
	// high frequency noise that can drive the controller crazy
	wannaend = last_input + (dt / (filter + dt)) * (input - last_input);
	// update state
	last_input = wannaend;

	// add in derivative component
	return  wannaend;
}

float lowpassfilter_y(float input, float dt) { // dt in milliseconds
	static float last_input = 0;
	/// Low pass filter cut frequency for derivative calculation.
	float filter = LPF_FILTER; // Set to  "1 / ( 2 * PI * f_cut )";
	// Examples for _filter:
	//    float filter = 5.3052e-3; // Set to  "1 / ( 2 * PI * f_cut )";
	// f_cut = 10 Hz -> _filter = 15.9155e-3
	// f_cut = 15 Hz -> _filter = 10.6103e-3
	// f_cut = 20 Hz -> _filter =  7.9577e-3
	// f_cut = 25 Hz -> _filter =  6.3662e-3
	// f_cut = 30 Hz -> _filter =  5.3052e-3
	float wannaend = 0;

	// discrete low pass filter, cuts out the
	// high frequency noise that can drive the controller crazy
	wannaend = last_input + (dt / (filter + dt)) * (input - last_input);
	// update state
	last_input = wannaend;

	// add in derivative component
	return  wannaend;
}
//END OF JH CODE

void send_response(Serial* SP){
	unsigned long long int stamp = GetTickCount();

	struct packetResponse packet_response;
	packet_response.start_byte = '$';
	packet_response.second_byte = 'M';
	packet_response.direct_byte = '<';
	packet_response.size_byte = sizeof(stamp);
	packet_response.command_byte = RESPONSE;
	packet_response.timestamp = stamp;
	packet_response.crc_byte = 0;
	char *crc_cal = (char *)&(packet_response.size_byte);
	while (crc_cal != &packet_response.crc_byte){
		packet_response.crc_byte ^= *(crc_cal++);
	}
	SP->WriteData((char *)&packet_response, sizeof(packet_response));
}


int evaluateCommand(uint8_t c, Serial* SP);

int serialCom(Serial* SP) {
	uint8_t cc, state;
	char c;
	static uint8_t offset;
	static uint8_t dataSize;
	static uint8_t c_state;
	int ret = 0;
	static long long a, b = 0;
	while ((cc = SP->ReadData(&c, 1)) == 1)
	{
		// write(serial_fd,&c,1);
		//printf("\n%d\n",c);
		state = c_state;
		// regular data handling to detect and handle MSP and other data
		if (state == IDLE) {
			a++;
			//printf("%d ",a);
			if (c == '$') state = HEADER_START;

		}
		else if (state == HEADER_START) {
			state = (c == 'M') ? HEADER_M : IDLE;
		}
		else if (state == HEADER_M) {
			state = (c == '<') ? HEADER_ARROW : IDLE;
		}
		else if (state == HEADER_ARROW) {
			if ((unsigned char)c > INBUF_SIZE) {  // now we are expecting the payload size
				state = IDLE;
				//printf("\nNONONONONONONO\n");
				continue;
			}
			dataSize = c;
			checksum = c;
			offset = 0;
			indRX = 0;
			state = HEADER_SIZE;  // the command is to follow
		}
		else if (state == HEADER_SIZE) {
			cmdMSP = c;
			checksum ^= c;
			state = HEADER_CMD;
		}
		else if (state == HEADER_CMD) {
			if ((unsigned char)offset < (unsigned char)dataSize) {
				checksum ^= c;
				inBuf[offset++] = c;
			}
			else {
				if ((unsigned char)checksum == (unsigned char)c){ // compare calculated and transferred checksum
					send_response(SP);
					b++;
					//printf(" %d\n",b);
					ret = evaluateCommand(cmdMSP,SP); // we got a valid packet, evaluate it
				}
				state = IDLE;
				cc = 0; // no more than one MSP per port and per cycle
			}
		}
		c_state = state;
	}

	if (GetTickCount() - receive_stamp > 100)
		send_response(SP);

	return ret;
}

int evaluateCommand(uint8_t c, Serial* SP) {
	receive_stamp = GetTickCount();
	switch (c) {
	case HAND:
		s_struct_w((uint8_t*)&data_hand, sizeof(data_hand));
		return COM_RECEIVED;
		break;

	case UNDERARM:
		s_struct_w((uint8_t*)&data_underarm, sizeof(data_underarm));
		send_response(SP);
		return COM_RECEIVED;
		break;

	case OVERARM:
		s_struct_w((uint8_t*)&data_overarm, sizeof(data_overarm));
		send_response(SP);
		//printf("\n%lf %lf %lf",data_overarm.vect_underarm[0],data_overarm.vect_underarm[3],data_overarm.vect_underarm[6]);
		//if(data_overarm.vect_overarm[0] > 0.9)
		//printf("\n%lf %lf %lf",data_overarm.vect_overarm[0],data_overarm.vect_overarm[1],data_overarm.vect_overarm[2]);
		//printf("\n%lf %lf %lf", data_overarm.vect_handpoint[0], data_overarm.vect_handpoint[1], data_overarm.vect_handpoint[2]);
		//printf("\n%d %d %d %d", MouseFlag, data_overarm.sw[0], data_overarm.pg[0], data_overarm.pg[1]);
		return COM_RECEIVED;
		break;
	}
	return COM_UNRECEIVED;
}

#endif