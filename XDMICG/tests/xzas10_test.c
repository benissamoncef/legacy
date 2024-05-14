/*E*/
/*Fichier : $Id: xzas10_test.c,v 1.2 1994/11/09 12:13:20 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/11/09 12:13:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzas10_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester les fonctions de xzat.c
* qui envoient des messages RTWks, et par la meme occasion
* permet de tester les callback de tacli qui sont declenchees sur
* la reception de ces messages.
* Il faut pour cela que tacli (BASEDD/bin) soit lance sur la meme
* machine que xzas10_test.x afin de s abonner aux memes data Groups.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	08 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "ctpublic.h"
#include "cstypes.h"
#include "xdc.h"
#include "xdy.h"
#include "xzag.h"
#include "xzas10.h"
#include "xzst.h"
#include "xzsc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas10_test.c,v 1.2 1994/11/09 12:13:20 mercier Exp $ : xzas10_test" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* CONDITION D'UTILISATION
*	SQL serveur doit etre lance.
*	
*	On teste sur SDCI1.
*	
* FONCTION 
*   
*
------------------------------------------------------*/
{
	XDY_Mot  	vl_Station_in =1;
	XDY_Sens 	vl_Sens_in=1;
	XDY_Octet 	vl_Voie_in=1;
	XDY_Horodate	vl_Horodate_in=0.;
	char		vl_Horodatechar[20];
	XDY_Mot		vl_Q_in=-1;
	XDY_Booleen	vl_ValiditeQ_in=1;
	XDY_Octet 	vl_V_in=255;
	XDY_Booleen	vl_ValiditeV_in=1;
	XDY_Octet	vl_TO_in=255;
	XDY_Booleen	vl_ValiditeTO_in=1;
	XDY_Octet	vl_PL_in=255;
	XDY_Booleen 	vl_ValiditePL_in=1;
	
	int 		vl_retcode;
		
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzas10_test.x : Debut d execution");
	strcpy(vl_Horodatechar,"29/01/1994 12:23:45");
	vl_retcode = XZAG02_Connexion_Base("CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",vl_retcode);
	
	/*A
	** Declenchement de XZAS10_Ajout_Mesure_6mn avec la globale d initialisation a vrai
	*/
	if((vl_retcode = XZSM_13ConversionHorodateSec(vl_Horodatechar,&vl_Horodate_in))!=XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"XZSM_13ConversionHorodateSec a echoue");
	}
	if( XZAS10_Ajout_Mesure_6mn (vl_Station_in ,
				vl_Sens_in,
				vl_Voie_in,
				vl_Horodate_in,
				vl_Q_in,
				vl_ValiditeQ_in,
				vl_V_in,
				vl_ValiditeV_in,
				vl_TO_in,
				vl_ValiditeTO_in,
				vl_PL_in,
				vl_ValiditePL_in) != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzas10_test.x : XZAS10_Ajout_Mesure_6mn() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzas10_test.x : XZAS10_Ajout_Mesure_6mn() a reussi");
	}
	
	
	vl_retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_retcode);
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzas10_test.x : Fin d execution");
}
