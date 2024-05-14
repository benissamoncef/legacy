/*E Fichier : $Id: acli_EA_AS_F.h,v 1.6 2012/02/14 14:40:29 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2012/02/14 14:40:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_FctUtil_XZEA_XZAS * FICHIER ACLI_EA_AS_F.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Contient les fonctions utilisateurs de TACLI qui sont passees
* aux fonction de EQUEST (XZEA*) et qui permet d appeler les fonctions
* XZAS de mise en base des donnees de mesures 6mn, mesure Poids, 
* Niveau de services et pesage
****************************
* 	acli_XZEA03_XZAS10_FU est la fonction utilisateur appelee
*	par xzea03 . Elle permet a TACLI de mettre en base les mesures 6mn.
****************************
* 	acli_XZEA04_XZAS10_FU est la fonction utilisateur appelee
*	par xzea04 . Elle permet a TACLI de mettre en base les mesures 6mn.
****************************
* 	acli_XZEA05_XZAS12_FU est la fonction utilisateur appelee
*	par xzea05 . Elle permet a TACLI de mettre en base les niveaux de services.
****************************
* 
------------------------------------------------------
* HISTORIQUE :
*
* mercier	08 Nov 1994	: Creation
* C.T.          27 JAN 1995     : Modif nom fichier trop long (V 1.1)
* P.N.          04 OCT 1996     : Ajout pesage (eqt10) v1.2
* P.N.          10 Oct 1996	: ajout de la mise en base des donnees meteo (dem/1232) (1.3)
* P.N.          04 Fev 1996	: Ajout TDP 1.4
* JPL		12/12/2011	: Suppression avertissements de compilation mineurs  1.5
* JMG		31/01/12 : tdp etendu dem/1014 1.6
----------------------------------------------------- */

#ifndef ACLI_FctUtil_XZEA_XZAS
#define ACLI_FctUtil_XZEA_XZAS

/* fichiers inclus */
	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
		/* sybase*/
	
	#include 	"cstypes.h"
	#include	"ctpublic.h"
	
	/* includes applicatifs Migrazur */
	
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xzag.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include	"asql_com.h"
	#include	"xzea.h"
	#include	"xzas10.h"
	#include	"xzas11.h"
	#include	"xzas12.h"
	
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void  acli_XZEA03_XZAS10_FU (XZEAT_EMes_RADT ,	int *);
extern void  acli_XZEA04_XZAS10_FU (XZEAT_EMes_RADT ,	int *);
extern void  acli_XZEA05_XZAS12_FU (XDY_ENiveau_Service,int *);
extern void  acli_XZEA07_XZAS11_FU ( XDY_EMes_Poids ,	int *);

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAS50_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les mesures de pesage KC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void  acli_XZEA_XZAS50_FU (	XZEAT_EMES_RADT_SILHOUETTE va_Mes_KC, int *pa_ResultatKC ) ;
/*
* ARGUMENTS EN ENTREE :
*	va_Mes_KC 		contient les arguments a faire passer a XZAS50
*	va_Resultat 
*
* ARGUMENTS EN SORTIE :
*
*   a_ResultatKC va contenir le resultat d execution de XZAS50_Ajout_Mesure_KC
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
*	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des mesures KC
* 	
------------------------------------------------------*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAS51_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les mesures de pesage EC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void  acli_XZEA_XZAS51_FU ( XZEAT_EMES_RADT_ESSIEU va_Mes_EC,int *pa_ResultatEC) ;
/*
* ARGUMENTS EN ENTREE :
*	va_Mes _EC		contient les arguments a faire passer a XZAS51
*
* ARGUMENTS EN SORTIE :
*
*   pa_ResultatEC va contenir le resultat d execution de XZAS51_Ajout_Mesure_EC
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
*	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des mesures EC
* 	
------------------------------------------------------*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAS52_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les mesures de pesage PC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void  acli_XZEA_XZAS52_FU ( XZEAT_EMES_RADT_POIDS va_Mes_PC, int *pa_ResultatPC ) ;
/*
* ARGUMENTS EN ENTREE :
*	va_Mes_PC 		contient les arguments a faire passer a XZAS52.
*
* ARGUMENTS EN SORTIE :
*
*   a_ResultatPC va contenir le resultat d execution de XZAS52_Ajout_Mesure_PC
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
*	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des mesures PC
* 	
------------------------------------------------------*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAS53_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les mesures meteo.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void  acli_XZEA_XZAS53_FU ( XDY_EMes_Meteo va_Mes, int *pa_Resultat );
/*
* ARGUMENTS EN ENTREE :
*	va_Mes 		contient les arguments a faire passer a XZAS53.
*
* ARGUMENTS EN SORTIE :
*
*   a_ResultatPC va contenir le resultat d execution de XZAS53_Ajout_Mesure_MET
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
*	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des mesures METEO
* 	
------------------------------------------------------*/

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAD02_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les temps de parcours 6mn.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void  acli_XZEA_XZAD02_FU ( XDY_EMes_TdP_6mn va_Mes, XDY_EMes_TdP_6mn va_MesRADT, int *pa_Resultat ) ;
/*
* ARGUMENTS EN ENTREE :
*	va_Mes 		contient les arguments a faire passer a XZAD02.
*
* ARGUMENTS EN SORTIE :
*
*   a_ResultatPC va contenir le resultat d execution de XZAD02_Ajout_TDP_6mn
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
*	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des temps de parcours
* 	
------------------------------------------------------*/




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAD02_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les temps de parcours 6mn.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void  acli_XZEA_XZAD03_FU ( XDY_EMes_TdP_1h va_Mes, int *pa_Resultat );
/*
* ARGUMENTS EN ENTREE :
*	va_Mes 		contient les arguments a faire passer a XZAD03.
*
* ARGUMENTS EN SORTIE :
*
*   a_ResultatPC va contenir le resultat d execution de XZAD03_Ajout_TDP_1h
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
*	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des temps de parcours
* 	
------------------------------------------------------*/
#endif
