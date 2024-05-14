/* fichier : %W	Release : 1.1   Date : 03/01/95	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : cscan_direc				*
* FICHIER : cscan_direc.c			*	
*************************************************
* DESCRIPTION :
*
*	Il s'agit du module directeur de la tache
* tcscan. Cette derni…re » pour charge de surveiller
* les ≈quipement de communications, c'est » dire :
* le fax .
*
*************************************************
* HISTORIQUE :
*
* JMG	01/03/95	Creation
* AMI 28/01/21 : SAE_191: chantier date: analyse de la commande time
*
************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <xdg.h>
#include <xdc.h>
#include <xdm.h>
#include <xzsm.h>
#include <xzsc.h>

#include "cscan_direc.h"

/***********************************************/
/* Constantes 				       */
/***********************************************/
#define	MAXCARLIGNE		150
#define CSCANC_DELAI_DE_TEST	(10)
#define REPCRFAXEMIS		"fichiers/dyn"

/* d≈finitions des variables globales » l'application */
XDY_NomTache	vg_nomTache;
XDY_NomMachine	vg_nomMachine;

/* definition des variables locales au module */
static char *	version = "@(#)cscan_direc.c	1.1 1.1 03/01/95" ;
static char	vg_pathCOM[ 200];


/* fichier : @(#)cscan_direc.c	1.1   Release : 1.1  Date : 03/01/95	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : cscan_direc				*
*************************************************
* DESCRIPTION :
* fonction d'arret du module
*
*************************************************
* SEQUENCE D'APPEL :
*/

void		cscan_direc_arret_tache(
T_MSG_HEADER *			       pa_Entete,
char *				       pa_Corps
				       )

/*
* PARAMETRE EN ENTREE :
*
*	Pa_Entete	: Entete RTWorks du msg
*	pa_Corps	: Corps du message
*
************************************************/
{
	/* sortie */
  	exit(0);
}
/* fichier : @(#)cscan_direc.c	1.1   Release : 1.1   Date : 03/01/95	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : cscan_direc				*
*************************************************
* DESCRIPTION :
*
* R≈colte les CR d'≈mission de fax pr≈sents, cr≈≈s par
* getcremis.sh dans le r≈pertoire faxcr, puis les
* communique a la base.
*
*************************************************
* SEQUENCE D'APPEL :
*/
static

void	cscan_direc_Get_CR(void)

/*
************************************************/
{
FILE 	*pl_fd;
char 	vl_cmd[ 150],
	vl_ligne[ MAXCARLIGNE +1];
char 	vl_nomFic[ 20],
	vl_result[ 20];
int	vl_reqId,
	vl_numEvt;


/* r≈cup≈ration de la liste de tous les fichiers CR pr≈sents */
sprintf( vl_cmd, "cat %s/%s/*.cr 2>&-", vg_pathCOM, REPCRFAXEMIS);
if ((pl_fd = popen( vl_cmd, "r")) == NULL)
{
   	XZST_04TRACECOND(XZSTC_WARNING,"cscan_direc_Get_CR: pb sur popen()") ;
	return;
}

/* POUR chaque CR dans la liste ... */
while( fgets( vl_ligne, MAXCARLIGNE, pl_fd) != NULL)
{
	/* on d≈code son contenu */
	if (sscanf(vl_ligne, "%s %s %d %d", vl_nomFic, vl_result, &vl_reqId, &vl_numEvt) == 4)
	{
		/* ECRIRE ICI LE TRAITEMENT ASSOCIE AU COMPTE_RENDU */
		/* TACLI OU XZAC18 */


		/* on d≈truit le fichier CR */
		sprintf( vl_cmd, "rm -f %s/%s/%s", vg_pathCOM, REPCRFAXEMIS, vl_nomFic);
		system( vl_cmd);
	}
	else
	{
		XZST_04TRACECOND(XZSTC_WARNING,"cscan_direc_Get_CR: format fichier xxxx.cr invalide") ;
	}

} /* FPOUR */

pclose( pl_fd);

}



/* fichier : @(#)cscan_direc.c	1.1   Release : 1.1  Date : 03/01/95	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : cscan_direc				*
*************************************************
* DESCRIPTION :
*
*	Point d'entr≈e de la tache tcscan.
*	La t¿che tcscan effectue p≈riodiquement
* les actions suivantes :
*		- traitement des msgs applicatifs
*		- connaissance des Compte rendu 
*		  d'≈mission des fax pr≈c≈demment
*		  ≈mis
*
*************************************************
* SEQUENCE D'APPEL :
*/

int	main(
int	    argc,
char *	    argv[]
	    )

/*
* PARAMETRE EN ENTREE :
*
*
************************************************/
{
	int		vl_ret;
	char		*pl_c;
	int 		vl_numFichiers;
	char 		vl_cmd[30];
	time_t		vl_Ex_T_Cmd  = (time_t)0 ,
			vl_Instant_T = (time_t)0 ;

/* il manque au moins un argument */
if (argc < 2)
{
   /* signale le et abandonne */
   printf("\n$0 : pas assez d'argument") ;
   exit(2) ;
}
/* qui suis je */
strcpy(vg_nomTache,argv[0]) ;

/* connexion au RTserver*/
XZSC_01InitCnxSup (argv[1],vg_nomTache,
		    XZSCC_INTERDIRE,
		    XZSCC_INTERDIRE,
		    XZSCC_INTERDIRE,
		    NULL,NULL);

/* recuperation du nom de la machine hote*/
vl_ret=XZSC_07NomMachine(vg_NomMachine);
if (vl_ret != XDC_OK) 
{
  XZST_03EcritureTrace(XZSTC_WARNING,
			"Recuperation nom machine a echoue %d\n",
			vl_ret);
  return (XDC_NOK);
}

/* r≈cup…re le PATH du r≈pertoire des fax */
strcpy( vg_pathCOM, "/produits/migrazur/appliSD");

/* traitement nominal RTWORKS */
for (;;)
{
/*	TipcFlush() ;*/

	/* si presence de message en provenance du RTSERVER */
	/*if (TipcWaitForServer(5,0) == TRUE)
	{
		TipcReadFromServer();
		TipcProcessMessages();
	}*/

        while (TipcSrvCheck(T_IO_CHECK_READ,0.0))
        {
    	   if (TipcSrvRead(0.0))
	   {
	       while ((vl_RTmsg=TipcSrvMsgNext(0.0))!=NULL)
	       {
		 TipcSrvMsgProcess(vl_RTmsg);
		 TipcMsgDestroy(vl_RTmsg);
	       }
           }
        }
	
	/* determine l'instant T */
   // sae_191 : ne sert qu'a faire un delta entre 2 reference temps donc peut rester comme cela	
   vl_Instant_T = time((time_t *)NULL) ;

	/* si il est temps de surveiller les p≈riph≈riques */
	if (vl_Instant_T - vl_Ex_T_Cmd > CSCANC_DELAI_DE_TEST)
	{
	   /* oblitere cette surveillance */
	   vl_Ex_T_Cmd = vl_Instant_T ;

	   /* r≈colte les ≈ventuels CR d'≈mission de fax */
	   cscan_direc_Get_CR() ;
	}
}
}

