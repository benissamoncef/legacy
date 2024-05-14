/*E*/
/*Fichier : $Id: xzas11_test.c,v 1.2 1994/11/09 12:13:24 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/11/09 12:13:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzas11_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester les fonctions de xzas*.c
* qui envoient des messages RTWks, et par la meme occasion
* permet de tester les callback de tacli qui sont declenchees sur
* la reception de ces messages.
* Il faut pour cela que tacli (BASEDD/bin) soit lance sur la meme
* machine que xzas11_test.x afin de s abonner aux memes data Groups.
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
#include "xzas11.h"
#include "xzst.h"
#include "xzsc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas11_test.c,v 1.2 1994/11/09 12:13:24 mercier Exp $ : xzas11_test" ;

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
	char		vl_Horodatechar[20];
	XDY_Horodate	vl_Horodate_in=0.;
	XDY_Entier	vl_Cumul_in=-1;
	XDY_Entier	vl_NbPl_in=-1;
	
	int 		vl_retcode;
		
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzas11_test.x : Debut d execution");
	strcpy(vl_Horodatechar,"09/11/1994 12:23:23");
	
	vl_retcode = XZAG02_Connexion_Base("CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",vl_retcode);
	
	XZST_03EcritureTrace( XZSTC_INFO,"1 er Test XZAS11");
	
	/*A
	** Declenchement de XZAS11_Ajout_Mesure_Poids avec la globale d initialisation a vrai
	*/
	if((vl_retcode = XZSM_13ConversionHorodateSec(vl_Horodatechar,&vl_Horodate_in))!=XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"XZSM_13ConversionHorodateSec a echoue");
	}
	if( XZAS11_Ajout_Mesure_Poids (	vl_Station_in ,
					vl_Sens_in,
					vl_Horodate_in,
					vl_Cumul_in,
					vl_NbPl_in) != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzas11_test.x : XZAS11_Ajout_Mesure_Poids() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzas11_test.x : XZAS11_Ajout_Mesure_Poids() a reussi");
	}
	/*A
	** Deuxieme test
	*/
	XZST_03EcritureTrace( XZSTC_INFO,"2eme Test XZAS11");
	
	strcpy(vl_Horodatechar,"09/11/1994 12:23:23");
	
	/*A
	** Declenchement de XZAS11_Ajout_Mesure_Poids avec la globale d initialisation a vrai
	*/
	if((vl_retcode = XZSM_13ConversionHorodateSec(vl_Horodatechar,&vl_Horodate_in))!=XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"XZSM_13ConversionHorodateSec a echoue");
	}
	vl_Station_in	=1;
	vl_Sens_in	=1;
	vl_Cumul_in	=23;
	vl_NbPl_in	=34;
	/*A
	** Declenchement de XZAS11_Ajout_Mesure_Poids avec la globale d initialisation a vrai
	*/
	if((vl_retcode = XZSM_13ConversionHorodateSec(vl_Horodatechar,&vl_Horodate_in))!=XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"XZSM_13ConversionHorodateSec a echoue");
	}
	if( XZAS11_Ajout_Mesure_Poids (	vl_Station_in ,
					vl_Sens_in,
					vl_Horodate_in,
					vl_Cumul_in,
					vl_NbPl_in) != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzas11_test.x : XZAS11_Ajout_Mesure_Poids() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzas11_test.x : XZAS11_Ajout_Mesure_Poids() a reussi");
	}
	
	/*A
	** Troisieme test
	*/
	XZST_03EcritureTrace( XZSTC_INFO,"3eme Test XZAS11");
	
	strcpy(vl_Horodatechar,"09/11/1994 12:23:23");
	
	/*A
	** Declenchement de XZAS11_Ajout_Mesure_Poids avec la globale d initialisation a vrai
	*/
	if((vl_retcode = XZSM_13ConversionHorodateSec(vl_Horodatechar,&vl_Horodate_in))!=XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"XZSM_13ConversionHorodateSec a echoue");
	}
	vl_Station_in	=1;
	vl_Sens_in	=1;
	vl_Cumul_in	=40;
	vl_NbPl_in	=50;
	/*A
	** Declenchement de XZAS11_Ajout_Mesure_Poids avec la globale d initialisation a vrai
	*/
	if((vl_retcode = XZSM_13ConversionHorodateSec(vl_Horodatechar,&vl_Horodate_in))!=XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"XZSM_13ConversionHorodateSec a echoue");
	}
	if( XZAS11_Ajout_Mesure_Poids (	vl_Station_in ,
					vl_Sens_in,
					vl_Horodate_in,
					vl_Cumul_in,
					vl_NbPl_in) != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzas11_test.x : XZAS11_Ajout_Mesure_Poids() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzas11_test.x : XZAS11_Ajout_Mesure_Poids() a reussi");
	}
	
	
	
	vl_retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_retcode);
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzas11_test.x : Fin d execution");
}
