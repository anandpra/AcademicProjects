#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <cstdio>
using namespace std;

/* ******************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for PA2, unidirectional or bidirectional
   data transfer protocols (from A to B. Bidirectional transfer of data
   is for extra credit and is not required).  Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
 **********************************************************************/

#define BIDIRECTIONAL 0    /* change to 1 if you're doing extra credit */
/* and write a routine called B_output */

/* a "msg" is the data unit passed from layer 5 (teachers code) to layer  */
/* 4 (students' code).  It contains the data (characters) to be delivered */
/* to layer 5 via the students transport level protocol entities.         */
struct msg {
	char data[20];
};

/* a packet is the data unit passed from layer 4 (students code) to layer */
/* 3 (teachers code).  Note the pre-defined packet structure, which all   */
/* students must follow. */
struct pkt {
	int seqnum;
	int acknum;
	int checksum;
	char payload[20];
};

/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
void stoptimer(int);
void starttimer(int,float);
void tolayer3(int,struct pkt);
void tolayer5(int ,char *);
/*Global variables for stats*/

int a_totalPacketsSentFromApp=0;
int a_totalPacketsSentFromTran=0;
int b_totalPacketsReceivedAtApp=0;
int b_totalPacketsReceivedAtTran=0;

/*Sender state*/
int a_sequenceNumber;
int a_state;
struct pkt a_lastPacket;
float a_timeout;

/*Receiver State*/
int b_state;
struct pkt b_lastAckPacket;
int onceThrough;

/*	This function is used to calculate checksum.
 *  Logic: Add seqnum value, acknum value and all the chars in payload.
 *	Since payload size is only 20 chars,the sum will never be greater than (2^32-1). Hence we are ignoring the carry forward after one's complement.
 * */
int checksum(struct pkt &packet){
	//Need to modify
	int sum=packet.acknum;
	sum+=packet.seqnum;
	for(int i=0;i<20;i++)
		sum+=packet.payload[i];
	sum=~sum;
	return sum;
}

/* This method checks if the given packet is corrupt or not.
 * It calculates the checksum of the given packet with its corresponding checksum field value and returns a bool accordingly.
 * */
bool isCorrupt(struct pkt &packet){
	if(checksum(packet)==packet.checksum)
		return false;
	else{
		cout<<"packet.checksum:"<<packet.checksum<<" checksum:"<<checksum(packet)<<endl;
		return true;

	}
}
/*
 * This function returns a statically set timeout value
 * */
float estimateTimeout(){

	return 20.0f;
}

/* called from layer 5, passed the data to be sent to other side
 * Message is sent only when the sender state is either in 1 or 3 i.e. when not waiting on any ack for previous 0 or 1 packet.
 *
 * */
void A_output(struct msg message) //ram's comment - students can change the return type of the function from struct to pointers if necessary
{
	if(a_state==1 ||a_state==3){
		struct pkt packet;
		//Using strnCPY, code failed when strcpy is used. Find out why
		strncpy(packet.payload,(const char*)message.data,20);
		packet.seqnum=a_sequenceNumber;
		packet.acknum=0;
		packet.checksum=checksum(packet);
		//a_lastPacket=&packet;
		a_lastPacket=packet;
		a_state++;
		starttimer(0,estimateTimeout());
		tolayer3(0,packet);
		a_totalPacketsSentFromTran++;
		cout<<"Packet with Sequence number "<<a_sequenceNumber<<" sent from A"<<endl;
		//cout<<"Num of Msg received from above:"<<++a_counter<<endl;
		fflush(stdout);
	}else{
	//	cout<<"waiting for acknowledge. Can't send any packets now. Please wait"<<endl;
	}
	a_totalPacketsSentFromApp++;
}

void B_output(struct msg message)  /* need be completed only for extra credit */
// ram's comment - students can change the return type of this function from struct to pointers if needed  
{

}

/* called from layer 3, when a packet arrives for layer 4
 * when Sender is in state 2 or 4 i.e. waiting for an ack of previously sent 0 or 1 packet, and this method is called then if it is not corrupt
 * the corresponding packet will be acknowledged and state will be changed for next packets transmission.
 *
 * */
void A_input(struct pkt packet)
{
	if(a_state==2){
		if(!isCorrupt(packet)){
			if(packet.acknum==0){
				//Positive, non corrupted Ack for last packet
				stoptimer(0);
				//Toggle a_sequenceNumber
				cout<<"Positive Ack Received for Seq:"<<a_sequenceNumber<<endl;
				a_sequenceNumber=!a_sequenceNumber;
				a_state++;
				return;
			}else{
				//Duplicate Ack
				cout<<"Ack Received for previous packet with Seq:"<<a_sequenceNumber<<endl;
				return;
			}
		}else{
			//Corrupt Packet
			cout<<"Corrupt Ack received"<<endl;
			return;
		}
	}
	if(a_state==4){
		if(!isCorrupt(packet)){
			if(packet.acknum==1){
				//Positive, non corrupted Ack for last packet
				stoptimer(0);
				//Toggle a_sequenceNumber
				a_state=(a_state+1)%4;
				cout<<"Positive Ack Received for Seq:"<<a_sequenceNumber<<endl;
				a_sequenceNumber=!a_sequenceNumber;
				return;

			}else{
				//Duplicate Ack
				cout<<"Ack Received for previous packet with Seq:"<<a_sequenceNumber<<endl;
				return;
			}
		}else{
			//Corrupt Packet
			cout<<"Corrupt Ack received"<<endl;
			return;
		}
	}

}

/* called when A's timer goes off
 *
 * Only when the sender is waiting for ack of previously sent 0 or 1 packets and there is a timeout then the outstanding packet
 * will be retrans0mitted.
 *  */
void A_timerinterrupt() //ram's comment - changed the return type to void.
{
	if(a_state==2||a_state==4){
		//Timeout occured. Retransmit the last packet sent
		cout<<"Timeout occured. Retransmitting previous packet"<<endl;
		starttimer(0,estimateTimeout());
		struct pkt prevPacket=a_lastPacket;
		a_totalPacketsSentFromTran++;
		tolayer3(0,prevPacket);
	}
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init() //ram's comment - changed the return type to void.
{
	a_sequenceNumber=0;
	a_state=1;
	///	a_lastPacket=NULL;
	a_timeout=40.0f;


}

static int b_counter=0;
/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B
 * If the received packet is not corrupt then is the expected one, then it will be extracted. delivered to the upper layer and
 * a corresponding ack will be sent back to the sender.
 * */
void B_input(struct pkt packet)
{
	b_totalPacketsReceivedAtTran++;
	if(isCorrupt(packet)){
		//Packet is corrupt.Ignore
		cout<<"Corrupt packet received"<<endl;
		return;
	}

	if((b_state==0 && packet.seqnum==0)||(b_state==1 && packet.seqnum==1)){
		b_totalPacketsReceivedAtApp++;
		tolayer5(1,packet.payload);
		struct pkt ackPacket;
		ackPacket.acknum=packet.seqnum;
		ackPacket.seqnum=packet.seqnum;
		ackPacket.checksum=checksum(ackPacket);
		tolayer3(1,ackPacket);
		b_lastAckPacket=ackPacket;
		onceThrough=1;
		b_state=!b_state;
		cout<<"Packet with sequence number:"<<packet.seqnum<<" Received"<<endl;
		cout<<"Sending Ack packet with ACK number:"<<ackPacket.acknum<<endl;
		b_counter++;
	//	cout<<"Num of Msg received from below:"<<b_counter<<endl;
		fflush(stdout);
	}else{
		//Duplicate Packet received.Send previous Ack back
		if(onceThrough==1 ){
			cout<<"Duplicate packet received. Sending the last Ack again"<<endl;
			tolayer3(1,b_lastAckPacket);
		}
	}



}

/* called when B's timer goes off */
void B_timerinterrupt() //ram's comment - changed the return type to void.
{
}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init() //ram's comment - changed the return type to void.
{
	b_state=0;
	//	b_lastAckPacket=NULL;
	onceThrough=0;
}

int TRACE = 1;             /* for my debugging */
int nsim = 0;              /* number of messages from 5 to 4 so far */
int nsimmax = 0;           /* number of msgs to generate, then stop */
float time_local = 0;
float lossprob;            /* probability that a packet is dropped  */
float corruptprob;         /* probability that one bit is packet is flipped */
float lambda;              /* arrival rate of messages from layer 5 */
int   ntolayer3;           /* number sent into layer 3 */
int   nlost;               /* number lost in media */
int ncorrupt;              /* number corrupted by media*/

/****************************************************************************/
/* jimsrand(): return a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in therange [0,mmm]        */
/****************************************************************************/
float jimsrand() 
{
	double mmm = 2147483647;   /* largest int  - MACHINE DEPENDENT!!!!!!!!   */
	float x;                   /* individual students may need to change mmm */
	x = rand()/mmm;            /* x should be uniform in [0,1] */
	return(x);
}  


/*****************************************************************
 ***************** NETWORK EMULATION CODE IS BELOW ***********
The code below emulates the layer 3 and below network environment:
  - emulates the tranmission and delivery (possibly with bit-level corruption
    and packet loss) of packets across the layer 3/4 interface
  - handles the starting/stopping of a timer, and generates timer
    interrupts (resulting in calling students timer handler).
  - generates message to be sent (passed from later 5 to 4)

THERE IS NOT REASON THAT ANY STUDENT SHOULD HAVE TO READ OR UNDERSTAND
THE CODE BELOW.  YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, you should have
to, and you defeinitely should not have to modify
 ******************************************************************/



/* possible events: */
#define  TIMER_INTERRUPT 0  
#define  FROM_LAYER5     1
#define  FROM_LAYER3     2

#define  OFF             0
#define  ON              1
#define   A    0
#define   B    1


struct event {
	float evtime;           /* event time */
	int evtype;             /* event type code */
	int eventity;           /* entity where event occurs */
	struct pkt *pktptr;     /* ptr to packet (if any) assoc w/ this event */
	struct event *prev;
	struct event *next;
};
struct event *evlist = NULL;   /* the event list */


void insertevent(struct event *p)
{
	struct event *q,*qold;

	if (TRACE>2) {
		printf("            INSERTEVENT: time is %lf\n",time_local);
		printf("            INSERTEVENT: future time will be %lf\n",p->evtime);
	}
	q = evlist;     /* q points to header of list in which p struct inserted */
	if (q==NULL) {   /* list is empty */
		evlist=p;
		p->next=NULL;
		p->prev=NULL;
	}
	else {
		for (qold = q; q !=NULL && p->evtime > q->evtime; q=q->next)
			qold=q;
		if (q==NULL) {   /* end of list */
			qold->next = p;
			p->prev = qold;
			p->next = NULL;
		}
		else if (q==evlist) { /* front of list */
			p->next=evlist;
			p->prev=NULL;
			p->next->prev=p;
			evlist = p;
		}
		else {     /* middle of list */
			p->next=q;
			p->prev=q->prev;
			q->prev->next=p;
			q->prev=p;
		}
	}
}





/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

void generate_next_arrival()
{
	double x,log(),ceil();
	struct event *evptr;
	//    //char *malloc();
	float ttime;
	int tempint;

	if (TRACE>2)
		printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");

	x = lambda*jimsrand()*2;  /* x is uniform on [0,2*lambda] */
	/* having mean of lambda        */

	evptr = (struct event *)malloc(sizeof(struct event));
	evptr->evtime =  time_local + x;
	evptr->evtype =  FROM_LAYER5;
	if (BIDIRECTIONAL && (jimsrand()>0.5) )
		evptr->eventity = B;
	else
		evptr->eventity = A;
	insertevent(evptr);
}





void init()                         /* initialize the simulator */
{
	int i;
	float sum, avg;
	float jimsrand();


	printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
	printf("Enter the number of messages to simulate: ");
	scanf("%d",&nsimmax);
	printf("Enter  packet loss probability [enter 0.0 for no loss]:");
	scanf("%f",&lossprob);
	printf("Enter packet corruption probability [0.0 for no corruption]:");
	scanf("%f",&corruptprob);
	printf("Enter average time between messages from sender's layer5 [ > 0.0]:");
	scanf("%f",&lambda);
	printf("Enter TRACE:");
	scanf("%d",&TRACE);

	srand(9999);              /* init random number generator */
	sum = 0.0;                /* test random number generator for students */
	for (i=0; i<1000; i++)
		sum=sum+jimsrand();    /* jimsrand() should be uniform in [0,1] */
	avg = sum/1000.0;
	if (avg < 0.25 || avg > 0.75) {
		printf("It is likely that random number generation on your machine\n" );
		printf("is different from what this emulator expects.  Please take\n");
		printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
		exit(0);
	}

	ntolayer3 = 0;
	nlost = 0;
	ncorrupt = 0;

	time_local=0;                    /* initialize time to 0.0 */
	generate_next_arrival();     /* initialize event list */
}






//int TRACE = 1;             /* for my debugging */
//int nsim = 0;              /* number of messages from 5 to 4 so far */ 
//int nsimmax = 0;           /* number of msgs to generate, then stop */
//float time = 0.000;
//float lossprob;            /* probability that a packet is dropped  */
//float corruptprob;         /* probability that one bit is packet is flipped */
//float lambda;              /* arrival rate of messages from layer 5 */   
//int   ntolayer3;           /* number sent into layer 3 */
//int   nlost;               /* number lost in media */
//int ncorrupt;              /* number corrupted by media*/

int main()
{
	struct event *eventptr;
	struct msg  msg2give;
	struct pkt  pkt2give;

	int i,j;
	char c;

	init();
	A_init();
	B_init();

	while (1) {
		eventptr = evlist;            /* get next event to simulate */
		if (eventptr==NULL)
			goto terminate;
		evlist = evlist->next;        /* remove this event from event list */
		if (evlist!=NULL)
			evlist->prev=NULL;
		if (TRACE>=2) {
			printf("\nEVENT time: %f,",eventptr->evtime);
			printf("  type: %d",eventptr->evtype);
			if (eventptr->evtype==0)
				printf(", timerinterrupt  ");
			else if (eventptr->evtype==1)
				printf(", fromlayer5 ");
			else
				printf(", fromlayer3 ");
			printf(" entity: %d\n",eventptr->eventity);
		}
		time_local = eventptr->evtime;        /* update time to next event time */
		if (nsim==nsimmax)
			break;                        /* all done with simulation */
		if (eventptr->evtype == FROM_LAYER5 ) {
			generate_next_arrival();   /* set up future arrival */
			/* fill in msg to give with string of same letter */
			j = nsim % 26;
			for (i=0; i<20; i++)
				msg2give.data[i] = 97 + j;
			if (TRACE>2) {
				printf("          MAINLOOP: data given to student: ");
				for (i=0; i<20; i++)
					printf("%c", msg2give.data[i]);
				printf("\n");
			}
			nsim++;
			if (eventptr->eventity == A)
				A_output(msg2give);
			else
				B_output(msg2give);
		}
		else if (eventptr->evtype ==  FROM_LAYER3) {
			pkt2give.seqnum = eventptr->pktptr->seqnum;
			pkt2give.acknum = eventptr->pktptr->acknum;
			pkt2give.checksum = eventptr->pktptr->checksum;
			for (i=0; i<20; i++)
				pkt2give.payload[i] = eventptr->pktptr->payload[i];
			if (eventptr->eventity ==A)      /* deliver packet by calling */
				A_input(pkt2give);            /* appropriate entity */
			else
				B_input(pkt2give);
			free(eventptr->pktptr);          /* free the memory for packet */
		}
		else if (eventptr->evtype ==  TIMER_INTERRUPT) {
			if (eventptr->eventity == A)
				A_timerinterrupt();
			else
				B_timerinterrupt();
		}
		else  {
			printf("INTERNAL PANIC: unknown event type \n");
		}
		free(eventptr);
	}

	terminate:
	printf(" Simulator terminated at time %f\n after sending %d msgs from layer5\n",time_local,nsim);
	cout<<"Protocol: ABP"<<endl;
	cout<<a_totalPacketsSentFromApp<<" of packets sent from the Application Layer of Sender A"<<endl;
	cout<<a_totalPacketsSentFromTran<<" of packets sent from the Transport Layer of Sender A"<<endl;
	cout<<b_totalPacketsReceivedAtTran<<" packets received at the Transport Layer of Receiver B"<<endl;
	cout<<b_totalPacketsReceivedAtApp<<" of packets received at the Application layer of Receiver B"<<endl;
	cout<<"Total time:"<<time_local<<endl;
	cout<<"Throughput ="<<((float)b_totalPacketsReceivedAtApp/time_local)<<endl;

	return 1;
}


/********************* EVENT HANDLINE ROUTINES *******/
/*  The next set of routines handle the event list   */
/*****************************************************/

/*void generate_next_arrival()
{
   double x,log(),ceil();
   struct event *evptr;
    //char *malloc();
   float ttime;
   int tempint;

   if (TRACE>2)
       printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");

   x = lambda*jimsrand()*2;  // x is uniform on [0,2*lambda] 
                             // having mean of lambda       
   evptr = (struct event *)malloc(sizeof(struct event));
   evptr->evtime =  time + x;
   evptr->evtype =  FROM_LAYER5;
   if (BIDIRECTIONAL && (jimsrand()>0.5) )
      evptr->eventity = B;
    else
      evptr->eventity = A;
   insertevent(evptr);
} */




void printevlist()
{
	struct event *q;
	int i;
	printf("--------------\nEvent List Follows:\n");
	for(q = evlist; q!=NULL; q=q->next) {
		printf("Event time: %f, type: %d entity: %d\n",q->evtime,q->evtype,q->eventity);
	}
	printf("--------------\n");
}



/********************** Student-callable ROUTINES ***********************/

/* called by students routine to cancel a previously-started timer */
void stoptimer(int AorB)
//AorB;  /* A or B is trying to stop timer */
{
	struct event *q,*qold;

	if (TRACE>2)
		printf("          STOP TIMER: stopping timer at %f\n",time_local);
	/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
	for (q=evlist; q!=NULL ; q = q->next)
		if ( (q->evtype==TIMER_INTERRUPT  && q->eventity==AorB) ) {
			/* remove this event */
			if (q->next==NULL && q->prev==NULL)
				evlist=NULL;         /* remove first and only event on list */
			else if (q->next==NULL) /* end of list - there is one in front */
				q->prev->next = NULL;
			else if (q==evlist) { /* front of list - there must be event after */
				q->next->prev=NULL;
				evlist = q->next;
			}
			else {     /* middle of list */
				q->next->prev = q->prev;
				q->prev->next =  q->next;
			}
			free(q);
			return;
		}
	printf("Warning: unable to cancel your timer. It wasn't running.\n");
}


void starttimer(int AorB,float increment)
// AorB;  /* A or B is trying to stop timer */

{

	struct event *q;
	struct event *evptr;
	////char *malloc();

	if (TRACE>2)
		printf("          START TIMER: starting timer at %f\n",time_local);
	/* be nice: check to see if timer is already started, if so, then  warn */
	/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
	for (q=evlist; q!=NULL ; q = q->next)
		if ( (q->evtype==TIMER_INTERRUPT  && q->eventity==AorB) ) {
			printf("Warning: attempt to start a timer that is already started\n");
			return;
		}

	/* create future event for when timer goes off */
	evptr = (struct event *)malloc(sizeof(struct event));
	evptr->evtime =  time_local + increment;
	evptr->evtype =  TIMER_INTERRUPT;
	evptr->eventity = AorB;
	insertevent(evptr);
} 


/************************** TOLAYER3 ***************/
void tolayer3(int AorB,struct pkt packet)
{
	struct pkt *mypktptr;
	struct event *evptr,*q;
	////char *malloc();
	float lastime, x, jimsrand();
	int i;


	ntolayer3++;

	/* simulate losses: */
	if (jimsrand() < lossprob)  {
		nlost++;
		if (TRACE>0)
			printf("          TOLAYER3: packet being lost\n");
		return;
	}

	/* make a copy of the packet student just gave me since he/she may decide */
	/* to do something with the packet after we return back to him/her */
	mypktptr = (struct pkt *)malloc(sizeof(struct pkt));
	mypktptr->seqnum = packet.seqnum;
	mypktptr->acknum = packet.acknum;
	mypktptr->checksum = packet.checksum;
	for (i=0; i<20; i++)
		mypktptr->payload[i] = packet.payload[i];
	if (TRACE>2)  {
		printf("          TOLAYER3: seq: %d, ack %d, check: %d ", mypktptr->seqnum,
				mypktptr->acknum,  mypktptr->checksum);
		for (i=0; i<20; i++)
			printf("%c",mypktptr->payload[i]);
		printf("\n");
	}

	/* create future event for arrival of packet at the other side */
	evptr = (struct event *)malloc(sizeof(struct event));
	evptr->evtype =  FROM_LAYER3;   /* packet will pop out from layer3 */
	evptr->eventity = (AorB+1) % 2; /* event occurs at other entity */
	evptr->pktptr = mypktptr;       /* save ptr to my copy of packet */
	/* finally, compute the arrival time of packet at the other end.
   medium can not reorder, so make sure packet arrives between 1 and 10
   time units after the latest arrival time of packets
   currently in the medium on their way to the destination */
	lastime = time_local;
	/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
	for (q=evlist; q!=NULL ; q = q->next)
		if ( (q->evtype==FROM_LAYER3  && q->eventity==evptr->eventity) )
			lastime = q->evtime;
	evptr->evtime =  lastime + 1 + 9*jimsrand();



	/* simulate corruption: */
	if (jimsrand() < corruptprob)  {
		ncorrupt++;
		if ( (x = jimsrand()) < .75)
			mypktptr->payload[0]='Z';   /* corrupt payload */
		else if (x < .875)
			mypktptr->seqnum = 999999;
		else
			mypktptr->acknum = 999999;
		if (TRACE>0)
			printf("          TOLAYER3: packet being corrupted\n");
	}

	if (TRACE>2)
		printf("          TOLAYER3: scheduling arrival on other side\n");
	insertevent(evptr);
} 

void tolayer5(int AorB,char *datasent)
{

	int i;
	if (TRACE>2) {
		printf("          TOLAYER5: data received: ");
		for (i=0; i<20; i++)
			printf("%c",datasent[i]);
		printf("\n");
	}

}
