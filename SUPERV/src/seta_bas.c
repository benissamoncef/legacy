/*E*/
/* Fichier :$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $		Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 17:53:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_BAS * FICHIER seta_bas.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module qui permet de gerer la redondance des serveurs, en 
*	definissant le serveur de secours et en fournissant des fonctions
*	de basculement.< description rapide des fonctions remplies par le module
*
* seta_definir_secours	: Determine si la machine est celle de Secours
* seta_envoie_ract	: Envoie le message XDM_TSE_RACT pour specifier la
*				machine de secours
* seta_synchroactif_cb 	: Declenche sur une demande de specification
*	de la machine de secours
* seta_bascule_cb	: Permet de declencher un basculement sur demande
* seta_consulter_etatlocal : Permet de recuperer l'etat de la machine locale
* seta_consulter_etatsecours : Permet de recuperer l'etat de la machine de secours
*
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 24 Oct 1994 : Creation 
* T.Milleville 7 Fev 1995 : Modification test de basculement 
*					V 1.5
* T.Milleville 25 Jul 1995 : Ajout de commentaires V1.6
* T.Milleville 26 Jul 1995 : Mise a jour entete V1.6
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.8
*******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

/* fichiers inclus */

#include <rtworks/ipc.h>

#include "xdm.h"
#include "xdl.h"
#include "xzsa.h"
#include "xzsx.h"
#include "xzsp.h"

#include "seta_don.h"
#include "seta_bas.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $ : seta_bas" ;

/* declaration de fonctions internes */

int seta_consulter_etatlocal( int *	pa_EtatLocal);
int seta_consulter_etatsecours( int *	pa_EtatSecours);

/* definition de fonctions externes */


/*X*/
/* Fichier :$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $		Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 17:53:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_BAS
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction determine si une machine correspond
* a la machine de secours de la machine locale et 
* affecte la variable SEP_MACHINE_SECOURS en 
* consequence.
* La machine de secours de machine locale correspond
* a la machine du site ayant le meme type.
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_definir_secours(XZSCT_NomMachine pa_NomMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine: nom de la machine testee
*
*
* PARAMETRES EN SORTIE :
* neant
*
* VALEUR(S) DE RETOUR : 
*	XDC_OK 	: ok
*
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
    static char *version = "$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $ : seta_definir_secours" ;
    int 		vl_ValRet 	= XDC_OK;
    XZSCT_NomSite 	vl_NomSite	= "";
    int 		vl_TypeMachine	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_definir_secours : IN \n");

    /* recherche du site de la machine */
    if(XZSA_23SiteMachine(pa_NomMachine, vl_NomSite) != XDC_OK)
    {
       /* erreur recherche site */
       vl_ValRet = XDC_NOK;
    }

    /* recherche du type de la machine */
    if(XZSA_17TypeMachine(pa_NomMachine, &vl_TypeMachine) != XDC_OK)
    {
       /* erreur recherche type machine */
       vl_ValRet = XDC_NOK;
    }

    if (vl_ValRet == XDC_OK)
    {
       /* si la machine correspond a la machine 
       *		redondante de la machine locale */
       if (!strcmp(SEP_NOM_SITE, vl_NomSite) &&
    	   (SEV_TYPE_MACHINE == vl_TypeMachine) &&
	   strcmp(SEP_NOM_MACHINE, pa_NomMachine))
       {
          strcpy(SEP_MACHINE_SECOURS, pa_NomMachine);
       }
    }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_definir_secours : OUT -- Code Retour %d \
		-- Mach Secours %s \n",vl_ValRet,SEP_MACHINE_SECOURS);

    return(vl_ValRet) ;

    
}	/* Fin seta_definir_secours */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'envoyer le message XDM_TSE_RACT 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int seta_envoie_ract(
	XZSCT_NomMachine	va_NomMachine,
	XZSCT_NomTache		va_NomTache)

/*
* ARGUMENTS EN ENTREE :
*    va_NomMachine	: Nom de la machine
*    va_NomTache	: Nom de la Tache
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK	: ok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere l'etat de la machine, puis construit le datagroup
*	NomMachine_Nomtache et envoie le message XDM_TSE_RACT
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $ : seta_envoie_ract " ;
	int	vl_ValRet	= XDC_OK;
	int	vl_EtatMachine	= 0;
   	XZSCT_Datagroup pl_NomDatagroup	= "";


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_envoie_ract	: IN \
		-- Nom Mach %s -- Nom Tache %s \n", 
		va_NomMachine , va_NomTache);

   /* recherche de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachine);

   /* envoi du message XDM_TSE_RACT a la tache demandeuse */
   XDG_EncodeDG2(pl_NomDatagroup, va_NomMachine, va_NomTache);

	XZST_03EcritureTrace(XZSTC_MESSAGE,
		"Envoi du Msg XDM_TSE_RACT --  Nom Mach %s \
		-- Etat Machine %d \n",SEP_NOM_MACHINE,vl_EtatMachine);


   TipcSrvMsgWrite(pl_NomDatagroup,
                  XDM_IdentMsg(XDM_TSE_RACT),
		  XDM_FLG_SRVMSGWRITE,
		  T_IPC_FT_STR, SEP_NOM_MACHINE,
                  T_IPC_FT_INT4, vl_EtatMachine,
                  NULL);

   if (TipcSrvFlush() != TRUE)
   {
      vl_ValRet = XDC_NOK;
   }

	return (vl_ValRet);


}	/* Fin seta_envoie_ract */



/*X*/
/* Fichier :$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $		Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 17:53:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_BAS
******************************************************
* DESCRIPTION DE LA FONCTION :
* 	Declenche sur reception du msg XDM_TSE_QACT
*	Renvoie le msg XDM_TSE_RACT
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_synchroactif_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG Arg)

/*
* PARAMETRES EN ENTREE :
*	Argument d'une callback
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
   static char *version="$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $ : seta_synchroactif_cb " ;
   int 		vl_ValRet	= 0;
#ifndef _TIBCO_EMS
   T_STR 	pl_NomMachine	= ""; /* nom de la machine demandeuse */
   T_STR 	pl_NomTache	= "";   /* nom de la tache demandeuse */
#else
	XDY_NomMachine pl_NomMachine = "";
	XDY_NomTache pl_NomTache = "";
#endif
   XZSCT_Datagroup      pl_NomDatagroup         = "";



	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_synchroactif_cb : IN \n");

   /* Recuperation du nom de la machine et du nom de la tache
      demandeuse */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR, &pl_NomMachine,
               T_IPC_FT_STR, &pl_NomTache,
	       NULL); 

	XZST_03EcritureTrace(XZSTC_MESSAGE,
		"Reception du Msg XDM_TSE_QACT --  Nom Mach %s \
		-- Nom Tache %s \n", pl_NomMachine , pl_NomTache);


	/*A Envoie du Message XDM_TSE_RACT */
	seta_envoie_ract(pl_NomMachine ,pl_NomTache);


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"seta_synchroactif_cb : OUT  \n");

}	/* Fin seta_synchroactif_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de declencher un basculement sur reception du msg XDM_TSE_BASC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void seta_bascule_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG Arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments specifiques RTWorks
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK 	: ok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere l'etat de la machine locale, si l'etat du calculateur
*	est ACTIF alors recupere le nom de la mchine de secours
*	puis on arrete la machine locale 
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $ : seta_bascule_cb " ;
	int		vl_ValRet 	= XDC_OK;
	int		vl_EtatLocal	= 0;
	int		vl_EtatSecours	= 0;
	int		vl_TypeMachine	= 0;


	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_bascule_cb : IN \n");

	/*A Recuperation de l'etat de la machine locale */
	seta_consulter_etatlocal(&vl_EtatLocal);

	/*A Suivant l'etat du Calculateur , on arrete la machine */
	if (	(vl_EtatLocal == XZSEC_ETAT_NACTIF) ||
		(vl_EtatLocal == XZSEC_ETAT_DEGRADE) )
	{
		/*B Recuperation de l'etat de la machine de secours */
		seta_consulter_etatsecours(&vl_EtatSecours);

		/*B Si la machine de secours est PASSIVE */
		if (vl_EtatSecours == XZSEC_ETAT_NPASSIF)
		{
			/*B Demande d'arret de la machine locale */
			vl_ValRet = XZSP_04ArretMachine(SEP_NOM_MACHINE,
							XZSPC_ARRET);
			if (vl_ValRet != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,
					"Pb sur arret Machine %s \n",
						SEP_NOM_MACHINE);
			}

			/*A Enregistrement du Msg de Basculement */
			XZSC_05TypeMachine(&vl_TypeMachine);
			if (vl_TypeMachine == XZSCC_TYPEM_SD)
			{
				/*B Basculement Serveur de Donnees */
				XZST_10ArchiverMsgSyst(XDL_SUP37_BASCUSD,
					SEP_NOM_SITE,NULL);
			}
			else
			{
				/*B Basculement d'un serveur de Com */
				XZST_10ArchiverMsgSyst(XDL_SUP35_BASCCOM,
					SEP_NOM_SITE,NULL);
			}
		}
		else
		{
			/*B Envoie msg archive */
			XZST_10ArchiverMsgSyst(XDL_SUP58_SECNOK,
					SEP_MACHINE_SECOURS,NULL);
		}
	}


	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_bascule_cb : OUT \n");

}	/* Fin seta_bascule_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de recuperer l'etat de la machine locale 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int seta_consulter_etatlocal( int *	pa_EtatLocal)

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
*
* ARGUMENTS EN SORTIE :
*   pa_EtatLocal : Etat de la machine locale
*
*
* CODE RETOUR : 
*   XDC_OK	: ok
*   XDC_NOK	: nok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Parcours le tableau  des machines surveilles et recupere
*	l'etat de la machine locale
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $ : seta_consulter_etatlocal " ;
	int	vl_ValRet	= XDC_NOK;
	int	vl_NumElt	= 0;


	for (vl_NumElt = 0; vl_NumElt < SEV_NB_MACH; vl_NumElt++)
	{
		if(!strcmp(SEP_ETAT_MACH[vl_NumElt].NomMachine,SEP_NOM_MACHINE))
		{
			*pa_EtatLocal = SEP_ETAT_MACH[vl_NumElt].Etat;

			vl_ValRet = XDC_OK;
			break;
		}
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_consulter_etatlocal : OUT \
		-- Etat Local %d -- Code Retour %d \n",
		*pa_EtatLocal, vl_ValRet);
	
	return (vl_ValRet);

}	/* Fin seta_consulter_etatlocal */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de recuperer l'etat de la machine de secours 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int seta_consulter_etatsecours( int *	pa_EtatSecours)

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
*
* ARGUMENTS EN SORTIE :
*   pa_EtatSecours : Etat de la machine locale
*
*
* CODE RETOUR : 
*   XDC_OK	: ok
*   XDC_NOK	: nok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Parcours le tableau  des machines surveilles et recupere
*	l'etat de la machine secours
*
------------------------------------------------------*/
{
	static char *version="$Id: seta_bas.c,v 1.9 2020/11/03 17:53:36 pc2dpdy Exp $ : seta_consulter_etatsecours " ;
	int	vl_ValRet	= XDC_NOK;
	int	vl_NumElt	= 0;


	for (vl_NumElt = 0; vl_NumElt < SEV_NB_MACH; vl_NumElt++)
	{
		if(!strcmp(SEP_ETAT_MACH[vl_NumElt].NomMachine,
						SEP_MACHINE_SECOURS))
		{
			*pa_EtatSecours = SEP_ETAT_MACH[vl_NumElt].Etat;

			vl_ValRet = XDC_OK;
			break;
		}
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"seta_consulter_etatsecours : OUT \
		-- Etat Secours %d -- Code Retour %d \n",
		*pa_EtatSecours, vl_ValRet);
	
	return (vl_ValRet);

}	/* Fin seta_consulter_etatsecours */

