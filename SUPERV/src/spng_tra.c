/*E*/
/* Fichier : $Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $	Release : $Revision: 1.12 $        Date : $Date: 2020/11/03 17:54:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SPNG_TRA * FICHIER spng_tra.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient les deux fonctions qui sont activees
* sur reception d'un message RTWORKS, il s'agit de la
* fonction d'armement d'un test reseau, et de la fonction
* de desarmement de test reseau.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 30 Sep 1994	: Creation
* T.Milleville 08 Mar 95 	: Remplacement xzga.h par xzsi.h V1.3
* T.Milleville 11 Mai 95 	: Ajout de traces V1.5
* T.Milleville 25 Jul 95 	: Ajout de commentaires V1.7
* T.Milleville 9 Aou 95 	: Ajout test pour lancement des tests reseaux.
*	Si machine degrade, lancement des tests  V1.8
* T.Milleville 12 Oct 95 	: Ajout test pour envoyer etat 
*	du test reseau V1.9
* T.Milleville 02 Nov  95 	: Test si le test reseau existe deja.
*	Sinon ajoute dans la liste V1.9
* T.Milleville 02 Nov 95 	: On declenche la prochaine verifiaction
*	reseau a la fin du traitement des tests actuels. Ceci afin d'eviter
*	une accumulation de demandes de tests V1.9
* P.NIEPCRON	30 Avr 1998	: Correction du test du ping v1.11
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
*******************************************************/

/* fichiers inclus */
#include "xdc.h"
#include "xdm.h"
#include "xdg.h"
#include "xdl.h"

#include "xzsc.h"
#include "xzsa.h"
#include "xzst.h"
#include "xzse.h"
#include "xzsi.h"
#include "xzsg.h"


#include "spng_don.h"
#include "spng_tra.h"


/* definitions de constantes */

/* ping pour HP_UX **/
#define SG_PING         "/etc/ping"
/* Delai en secondes du time out                     */
#define SG_TO_PING      8
/* Nombre de Bytes envoyes par PING pour tester la connection **/
#define SG_BYTES_PING   64
/* Nombre de fois que l'on essaie l'envoi de messages **/
#define SG_COUNT_PING   1
/* chaine a recherche pour determiner si le ping     */
/* s'est correctement deroule                        */
/* Sur HP on recupere le nombre de bytes envoyes 64  **/
#define SG_TOKEN_TST    "64"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration des fonctions internes */
static char *version = "$Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $	 : SPNG_TRA" ;




/*X*/
/* Fichier : $Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $	Release : $Revision: 1.12 $        Date : $Date: 2020/11/03 17:54:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETRAITER
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Fonction declenchie sur reception du msg XDM_TSP_PING
* Cette fonction insere dans la liste des tests reseau
* un test pour la machine dont le nom est dans le
* message RTWORKS dont l'arrivee a provoquee l'appel
* de cette fonction.
* Recuperation du nom de la machine pour laquelle on demande
*	un test reseau. Insertion dans la liste chainee memorisant
* tous les tests a effectuer. Puis envoie du msg XDM_TSP_RES pour demander 
*	 le lancement du test.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sg_arme_test_cb(T_IPC_CONN va_Conn,
                   T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                   T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : neant
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $	 : sg_arme_test_cb" ;
#ifndef _TIBCO_EMS
	T_STR 		pl_NomMachine	= "\0";
#else
	XDY_NomMachine 		pl_NomMachine	= "\0";
#endif
   	SGT_TEST_RES 	*pl_NouvTest	= NULL;
   	XZSCT_Datagroup pl_NomDatagroup	= "\0";
	int		vl_IndPre = XDC_FAUX;
   	SGT_TEST_RES 	*pl_Test	= NULL;
  

   	XZST_03EcritureTrace(XZSTC_FONCTION,"sg_arme_test_cb : IN \n");


   	/* recuperation du nom de la machine */ 
   	TipcMsgRead(pa_Data -> msg,
	       	T_IPC_FT_STR,  &pl_NomMachine,
	       	NULL);

   	XZST_03EcritureTrace(XZSTC_MESSAGE,"Reception de msg XDM_TSP_PING \
		pour la machine %s \n",pl_NomMachine);

   	XZST_03EcritureTrace(XZSTC_INFO,	
			"arme test reseau sur %s",pl_NomMachine);

   	/* creation du test supplementaire */
   	pl_NouvTest = malloc(sizeof(SGT_TEST_RES));
   	strcpy(pl_NouvTest -> NomMachine, pl_NomMachine);
   	pl_NouvTest ->Time = 0;

	/* Teste le reseau */
   	se_test_res(pl_NouvTest -> NomMachine, &(pl_NouvTest -> EtatMachine));
   	pl_NouvTest -> TestSuiv = NULL;

   	/* ajout du test supplementaire dans la liste */
   	if (pm_PremTest == NULL)
   	{
      		/* premier element de la liste */
      		pm_PremTest = pl_NouvTest;
      		pl_NouvTest -> TestPrec = NULL;
   	}
   	else
   	{

/* Modif V1.9 */
		/* Parcours la liste pour savoir si le test existe */
		for (pl_Test = pm_PremTest ; pl_Test != NULL ; pl_Test = pl_Test->TestSuiv)
		{
			if ( !strcmp(pl_NouvTest -> NomMachine, pl_Test->NomMachine) )
			{
				/* Indicatuer de presence */
				vl_IndPre = XDC_VRAI;
			}
		}	

		if (vl_IndPre == XDC_FAUX)
		{
      			/* on relie le dernier element 
			*		de la liste a ce nouvel element */
      			pm_DernTest -> TestSuiv = pl_NouvTest; /* chainage avant */
      			pl_NouvTest -> TestPrec = pm_DernTest; /* chainage arriere */
		}
   	}

	if (vl_IndPre == XDC_FAUX)
	{
   		pm_DernTest = pl_NouvTest;

   		/* diffusion de l'etat de la machine */
   		XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPHIST, vg_NomSite);

		XZST_03EcritureTrace(XZSTC_MESSAGE," Arme test res \
			Envoie Msg XDM_TSE_RES \
			-- Datagroup %s -- Nom Mach %s -- Etat Machn %d \n",
			pl_NomDatagroup, pl_NouvTest -> NomMachine, pl_NouvTest -> EtatMachine);

   		if (!TipcSrvMsgWrite(pl_NomDatagroup,
                       XDM_IdentMsg(XDM_TSE_RES),
                       XDM_FLG_SRVMSGWRITE,
		       T_IPC_FT_STR, pl_NouvTest -> NomMachine,
                       T_IPC_FT_INT4, pl_NouvTest -> EtatMachine,
                       NULL))
   		{
                	XZST_03EcritureTrace(XZSTC_WARNING,
                		"se_etassys_cb: erreur TipcSrvMsgWrite XDM_TSE_SSYS : \
				NomMachine %s EtatMachine %d " ,pl_NouvTest -> NomMachine
				,pl_NouvTest -> EtatMachine );
   		}
	}

   	XZST_03EcritureTrace(XZSTC_FONCTION,"sg_arme_test_cb : OUT \n");

}	/* Fin sg_arme_test_cb */




/*X*/
/* Fichier : $Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $	Release : $Revision: 1.12 $        Date : $Date: 2020/11/03 17:54:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETRAITER
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Fonction declenchie sur reception du msg XDM_TSP_STOPPING
* Cette fonction supprime de la liste des tests reseau
* un test pour la machine dont le nom est dans le
* message RTWORKS dont l'arrivee a provoquee l'appel
* de cette fonction.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sg_arret_test_cb(T_IPC_CONN va_Conn,
                   T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                   T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
B
* Arguments specifiques a un callback IPC.
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : neant
*
* CONDITION D'ERREUR : test a desarmer non trouve dans
*			la liste des tests reseau.
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $	 : sg_arret_test_cb" ;
#ifndef _TIBCO_EMS
	T_STR 		pl_NomMachine	= "\0";
#else
	XDY_NomMachine 		pl_NomMachine	= "\0";
#endif
	SGT_TEST_RES 	*pl_Test	= NULL;
	int 		vl_TestTrouve 	= XDC_FAUX;
   
   	XZST_03EcritureTrace(XZSTC_FONCTION,"sg_arret_test_cb : IN \n");

   	/* recuperation du nom de la machine */ 
   	TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_STR,  &pl_NomMachine,
	       NULL);

   	XZST_03EcritureTrace(XZSTC_MESSAGE,"Reception de msg XDM_TSP_STOPPING \
		pour la machine %s \n",pl_NomMachine);


   	XZST_03EcritureTrace(XZSTC_INFO,
			"arret test reseau sur %s",pl_NomMachine);

   	/* recherche du test dans la liste */
   	for (pl_Test = pm_PremTest; 
		(pl_Test != NULL) && (!vl_TestTrouve);
			pl_Test = pl_Test -> TestSuiv)
   	{
      		if (!strcmp(pl_Test -> NomMachine, pl_NomMachine))
      		{
	 		/* suppression du test de la liste */
	 		if (pl_Test == pm_PremTest)
	 		{
	    			pm_PremTest = pm_DernTest = NULL;
         		}
	 		else
	 		{
	    			/* on relie l'element precedent 
				*	a l'element suivant */
	    		((SGT_TEST_RES *)(pl_Test -> TestPrec)) -> TestSuiv = 
				pl_Test -> TestSuiv; /* chainage avant */
	    
				/* si element a suprimer n est pas 
						le dernier de la liste */
	    			/*    on recalcule le chainage arriere */
	    			/* sinon maj de pm_DernTest */
	    			if (pl_Test != pm_DernTest)
	       ((SGT_TEST_RES *)(pl_Test -> TestSuiv)) -> TestPrec = pl_Test -> TestPrec; /* chainage arriere */
	    			else
	       				pm_DernTest = pl_Test -> TestPrec;
         		}
	 		free(pl_Test);
	 		vl_TestTrouve = XDC_VRAI;
			break;
      		}
   	}


   	if (vl_TestTrouve != XDC_VRAI)
   	{
      		XZST_03EcritureTrace(XZSTC_WARNING,
			      "sg_arret_test: test a desarmer non trouve");
   	}
   	else
      		XZST_03EcritureTrace(XZSTC_INFO,
			"arret test reseau sur %s OK",pl_NomMachine);


   	XZST_03EcritureTrace(XZSTC_FONCTION,"sg_arret_test_cb : OUT \n");

}	/* Fin sg_arret_test_cb */


/*X*/
/* Fichier : $Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $        Release : $Revision: 1.12 $        Date : $Date: 2020/11/03 17:54:21 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SPNG_DIR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction utilise la commande UNIX "ping" afin
* de verifier si la machine dont le nom lui est passe
* en parametre est bien presente sur le reseau.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void se_test_res(
	XZSCT_NomMachine pa_NomMachine,
	int *pa_EtatMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine a tester
*
* PARAMETRES EN SORTIE :
* pa_EtatMachine : etat de la machine
*
******************************************************/
{
	static char *version = "$Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $  : se_test_res" ;
	int     vl_EtatMachineLoc       = 0;
	char    pl_Cmd[80]              = "\0";
	char    pl_ResCmd[80]           = "\0" ;
	FILE    *pl_P                   = NULL ;

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"se_test_res : IN  Nom Machine %s  ", pa_NomMachine);

	 /* construit la commande test reseau de la machine demandee */
	 sprintf(pl_Cmd,"%s %s %u %u",SG_PING,pa_NomMachine,
				       SG_BYTES_PING, SG_COUNT_PING);

	/* execute la commande, */
	if ((pl_P = popen(pl_Cmd,"r")) != (FILE *)NULL)
	{
		/* recupere le resultat de cette commande */
		fgets(pl_ResCmd,sizeof(pl_ResCmd)-1,pl_P) ;
		XZST_03EcritureTrace(XZSTC_INFO,"result ping:%s",pl_ResCmd);
		fgets(pl_ResCmd,sizeof(pl_ResCmd)-1,pl_P) ;
		XZST_03EcritureTrace(XZSTC_INFO,"result2 ping:%s",pl_ResCmd);

		pclose(pl_P) ;

		if (strstr(pl_ResCmd,SG_TOKEN_TST) != NULL)
		{
			*pa_EtatMachine = XZSGC_RES_OK;
		}
		else
		{
			*pa_EtatMachine = XZSGC_RES_HS;
		}
	}
	else
		XZST_03EcritureTrace(XZSTC_WARNING,
		     "se_test_res - erreur execution de %s",pl_Cmd);

	XZST_03EcritureTrace(XZSTC_FONCTION,
	     "se_test_res : OUT  Etat Machine %d ", *pa_EtatMachine );


}       /* se_test_res */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de verifier cycliquement les tests en cours
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void sg_verif_test_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments de Callback
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Appel cyclique declenche par le Timer sur reception de XDM_TSP_VERIF
*
* FONCTION 
*   Parcourt la liste des machines necessitant un test reseau et 
*	declenche celui-ci et met a jour l'etat de la machine
*	par envoi du msg XDM_TSE_RES
*
------------------------------------------------------*/
{
	static char *version="$Id: spng_tra.c,v 1.12 2020/11/03 17:54:21 pc2dpdy Exp $ : sg_verif_test_cb " ;
	SGT_TEST_RES    *pl_Test                = NULL;
	XZSCT_Datagroup pl_NomDatagroup         = "\0";
	T_IPC_MSG       pl_Msg;
	int             vl_EtatMachineLoc       = 0;
	int             vl_EtatMachine          = 0;
	int		vl_PeriodeTest	= 0;
	int             vl_Jour_De_La_Semaine   = 0;
	double          vl_Horodate             = 0.0;
	XZSCT_NomMachine pl_NomMachine          = "\0";



	XZST_03EcritureTrace(XZSTC_FONCTION, "sg_verif_test_cb : IN \
			-- pm_PremTest %d \n", pm_PremTest);

	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;

	/* Lancement des tests reseau pour toutes les machines surveillees */
	for (pl_Test = pm_PremTest; pl_Test != NULL; pl_Test = pl_Test -> TestSuiv)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "Lancement test machine %s \n",
					pl_Test -> NomMachine) ;

		/* Recuperation etat machine locale */
		XZSE_09EtatMachine(&vl_EtatMachineLoc) ;

		if ( (vl_EtatMachineLoc == XZSEC_ETAT_NACTIF) || 
			(vl_EtatMachineLoc == XZSEC_ETAT_DEGRADE) )
		{
			/*B Lancement du test reseau */
			se_test_res(pl_Test -> NomMachine, &vl_EtatMachine);
			
			/*B incrementation varaible Time */
			pl_Test ->Time++;

	XZST_03EcritureTrace(XZSTC_INFO , "sg_verif_test_cb : \
		eta prec %d -- etat machine %d -- nb fois %d \n",pl_Test -> EtatMachine,
		vl_EtatMachine, pl_Test ->Time);

			/* si changement d'etat de la machine on diffuse le nouvel etat */
			if ( (vl_EtatMachine != pl_Test -> EtatMachine) ||
				(pl_Test ->Time >= 10) )
			{
				/*B reinitialisation varaible Time */
				pl_Test ->Time = 0; 

				/* B Memorisation Etat Machine */
				pl_Test -> EtatMachine = vl_EtatMachine;

		      		/* l'etat est diffuse aux serveurs du site */
				XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPHIST, vg_NomSite);

			 	XZST_03EcritureTrace(XZSTC_MESSAGE," Init TSPNG \
						 Envoie Msg XDM_TSE_RES \
						 -- Datagroup %s -- Nom Mach %s -- Etat Machn %d \n",
						 pl_NomDatagroup, pl_Test -> NomMachine,pl_Test -> EtatMachine);

				if (!TipcSrvMsgWrite(pl_NomDatagroup, XDM_IdentMsg(XDM_TSE_RES),
					XDM_FLG_SRVMSGWRITE, T_IPC_FT_STR, pl_Test -> NomMachine,
					T_IPC_FT_INT4, pl_Test -> EtatMachine, NULL))
				{								
					XZST_03EcritureTrace(XZSTC_WARNING,
						" erreur TipcSrvMsgWrite XDM_TSE_RES : \
						--  Nom Mach %s -- Etat Mach %s \n",
						pl_Test -> NomMachine,pl_Test -> EtatMachine);
				}
			}
		}
	}

#ifndef _TIBCO_EMS
	/* abonnement au Datagroup XDG_NSUPPING_NomMachine */
	sprintf(pl_NomDatagroup,"%s_%s",XDG_NSUPPING,pl_NomMachine) ;
	XDG_JoindreDG(pl_NomDatagroup);

	/*A Redeclenchement des prochaines verifications reseaux */
	XZSC_03PeriodeTestReseau(&vl_PeriodeTest);
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_Jour_De_La_Semaine,&vl_Horodate) ;
	if (XZSM_01DemanderProgrammation( (vl_Horodate+vl_PeriodeTest), 0,
				      pl_NomDatagroup, XDM_TSG_VERIF,"XDM_TSG_VERIF" , 0) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,
				  "sg_verif_test_cb : Erreur demande programmation test reseau Machine \n");
	}
#endif

	XZST_03EcritureTrace(XZSTC_FONCTION, "sg_verif_test_cb : OUT \n");

}	/* Fin sg_verif_test_cb */


