/*E Fichier : $Id: isbi_sma.h,v 1.5 2018/01/09 10:23:13 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/01/09 10:23:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSMA * FICHIER isyn_sma.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module  directeur de la tache TISYN
*   
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	13 Sep 1994	: Creation
* JMG   20/03/17        SAGA DEM1191
* JMG	20/06/17 	tigmit 1.3
------------------------------------------------------ */

#ifndef MSMA
#define MSMA

/* fichiers inclus */
#include "xzst.h"
#include "xzsc.h"
#include "xdy.h"
#include "xzao730.h"

/* definitions de constantes exportees */

/* definitions de types exportes */
typedef struct  {
		   XZAOT_ConfEqtSAGA    Config;
		    void *               Suivant;
}DONNEES;
extern DONNEES          *pg_debut;

/* definition de macro exportees */

/* declarations de donnees globales */
XZSCT_NomMachine vm_NomMachine;
XDY_Nom vm_NomMachineSS;
XZSCT_NomSite    vm_NomSite;
int	    vm_TypeMachine;

XDY_Booleen vm_cx;

char	vg_titre[255];


int vm_sockfd, vm_ElfFd; /* Sockets pour RPC venant d'applix */

/* delaration de fonctions externes */
extern void
MajAdmSsSys( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
			  T_CB_ARG                   pa_Arg );
extern void MajAdmServeur( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
			  T_CB_ARG                   pa_Arg );
extern void MajAdmTache( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		T_CB_ARG                   pa_Arg );
extern void MajAdmPeriph( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
			  T_CB_ARG                   pa_Arg );
extern void MajAdmInterf( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
			  T_CB_ARG                   pa_Arg );
extern void MajAdmTrace( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
			  T_CB_ARG                   pa_Arg );
extern void MajAdmStatiM( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
			  T_CB_ARG                   pa_Arg );
MajAdmStatiE( T_IPC_CONN                 va_Cnx,
	     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
			  T_CB_ARG                   pa_Arg );

#endif
