/*  Elevator simulation test code */
/*  usage: 'elevator' [output trace file] */
/*  runs the qpnano elevator model TOTAL_SIM_TIME seconds */
/*  with random floor calls every CALLTIME seconds */
/*  the trace file records every dispatch/even transision of the model */
/*  default setup tracks the average number of ticks (seconds) between floor call and arrival */

#include "qpn_port.h"
#include "elevator.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* local objects -----------------------------------------------------------*/
static FILE *l_outFile = (FILE *)0;
static void dispatch(QSignal sig);
static int outf;

#define CALLTIME 20 //200 or 100 or 50 or 20 or 10
#define TOTAL_SIM_TIME 1000000
#define TRUE 1
#define FALSE 0

/*..........................................................................*/
int main(int argc, char *argv[]) {
	QHsmTst_ctor();                                  /* instantiate the HSM */

	if (argc > 1) {                                  /* file name provided? */
		outf = TRUE;				 /* write output trace */
		l_outFile = fopen(argv[1], "w");
	} else  				 /* just do the stats */
		outf = FALSE;

	printf("Elevator Controller Model, built on %s at %s, QP-nano %s\n"
		"output saved to %s\n",
		__DATE__, __TIME__, QP_getVersion(),
		argv[1]);

	if (outf) fprintf(l_outFile, "QHsmTst example, QP-nano %s\n",
			  QP_getVersion());

	QHsm_init((QHsm *)&HSM_QHsmTst);    /* take the initial transitioin */
	
	printf("Testing elevator controller model!\n\n");

	int floor_call = 0;
	simTime = 0;
	printf("Entering random call mode!\n\n");
	srandom(time(NULL));
	//STOP_TIME_F += (rand()%10) +1;

	while (simTime < TOTAL_SIM_TIME){
		//STOP_TIME_F += (rand()%10) +1; nooo

        if (simTime%CALLTIME == 0){
			floor_call = (random()%5)+1;
            STOP_TIME_F = (rand()%10) +6;
			switch (floor_call) {
				case 1: {
					dispatch(F1_SIG);
					break;
					}
				case 2: {
					dispatch(F2_SIG);
					break;
					}
				case 3: {
					dispatch(F3_SIG);
					break;
					}
				case 4: {
					dispatch(F4_SIG);
					break;
					}
				case 5: {
					dispatch(F5_SIG);
					break;
					}
				}
			}

		if(simTime%500 == 0 && simTime%1000 != 0){
			dispatch(EMERGENCY_ON_SIG);
		}

		if(simTime%1000 == 0){
			dispatch(EMERGENCY_OFF_SIG);
		}	

			dispatch(TICK_SIG);
			simTime++;
	}

	printf("Results for call time:%d seconds, simulation time:%d seconds\n", CALLTIME, TOTAL_SIM_TIME);	
 	dispatch(PRINT_SIG);
	 
       
	if (outf) fclose(l_outFile);

}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
/*..........................................................................*/
void BSP_display(char const *msg) {
    if (outf) fprintf(l_outFile,"%s", msg);
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("Bye, Bye!");
    exit(0);
}
/*..........................................................................*/
static void dispatch(QSignal sig) {
    if (outf) fprintf(l_outFile, "\n%c:", 'A' + sig - F1_SIG);
    Q_SIG((QHsm *)&HSM_QHsmTst) = sig;
    QHsm_dispatch((QHsm *)&HSM_QHsmTst);              /* dispatch the event */
}
