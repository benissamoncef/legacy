/*E*/  
/*Fichier : $Id: init_heure.c,v 1.4 1995/07/26 13:34:50 milleville Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 13:34:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SUPERV
------------------------------------------------------
* MODULE INIT_SYST * FICHIER init_heure.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module permet de mettre a jour l'heure systeme
*	au boot de la machine , juste avant le lancement
*	de l'application
*
------------------------------------------------------
* HISTORIQUE :
*
* T.Milleville	23 Nov 1994	: Creation
* T.Milleville	08 Mar 95 	: Modification des valeurs des 
*	arguments de la fonction XZEH_01 V1.2
* T.Milleville	22 Mar 95 	: Modification pour tenir compte
*	d'un eventuel pb d'horloge et suppression du repertoire
*	lie a la connexion RTWorks /tmp/rtworks V1.3
* T.Milleville	25 Jul 95 	: Mise a jour entete V1.4
------------------------------------------------------*/

/* fichiers inclus */
#include <time.h>
#include <stdio.h>

#include "xdc.h"
#include "xzeh.h"
#include "xzsm.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: init_heure.c,v 1.4 1995/07/26 13:34:50 milleville Exp $ : init_heure" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Programme principale permettant de mettre a jour 
*	l'heure systeme
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE :
*   argc 	: Nb arguments
*   argv 	: Liste des arguments
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
*   Executer au boot de la machine 
*
* FONCTION 
*   Utilise les modules d'interface XZEH, qui permettent
*	d'ouvrir la liaison avec la centrale horaire,
*	de recuperer l'heure, de refermer cette liaison
*	et de mettre a jour l'heure systeme
*
------------------------------------------------------*/
{
	static char *version="$Id: init_heure.c,v 1.4 1995/07/26 13:34:50 milleville Exp $ : main " ;
	int	vl_ValRet		= XDC_OK;
	XZSMT_Horodate    vl_LecHorodate;
	time_t          vl_HeureSystem;
	double          vl_DoubleHorodate	= 0.0;
	int             vl_JourSemaine          = 0;
	double          vl_Horodate             = 0.0;
	char		vl_cmd_sup_rtworks[50]  = "rm -r /tmp/rtworks";

	/*A Suppression du repertoire RTworks */
	system(vl_cmd_sup_rtworks);

	/*A Ouverture de la liaison avec  la centrale horaire*/
	if( (vl_ValRet = XZEH01_Ouverture_Horloge (XZEHC_NOM_RS232,XZSSC_RS_9600,
	     	XZEHC_RS232_INFO8,XZEHC_RS232_STOP1,XZEHC_RS232_NOPAR)) != XDC_OK)
	{
		printf("Probleme Ouverture Centrale Horaire !! \n");
	}
	else if ( (vl_ValRet =  XZEH02_Lecture_Horloge(vl_LecHorodate))  != XDC_OK )
	{
		 printf (" Probleme Lecture Centrale Horaire !! \n");
	}
	else if ( (vl_ValRet = XZEH03_Fermeture_Horloge()) != XDC_OK)
	{
		 printf (" Probleme Fermeture Centrale Horaire !! \n");
	}

	/*A Si Recuperation de l'heure a pose Probleme, alors
	*	on recupere l'heure courante */
	if (vl_ValRet != XDC_OK)
	{
		 /*A Recuperation de l'heure courante du Serveur */
		 vl_ValRet = XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME,
					 &vl_JourSemaine, &vl_Horodate);
		 printf (" Recuperation heure System %lf \n",vl_Horodate);
		 sprintf(vl_LecHorodate,"%lf",vl_Horodate);
	}


	/*  Transformation de l'horodate chaine en reel */
	vl_DoubleHorodate = atof(vl_LecHorodate);
	if( (vl_ValRet=XZSM_10ActualiserDeltaHorodate(vl_DoubleHorodate ))
				!= XDC_OK)
	{
		printf("Probleme Maj heure Systeme %d \n",errno);
	}

	
	printf("Maj heure systeme %s \n",vl_LecHorodate);

}	/* Fin main */


