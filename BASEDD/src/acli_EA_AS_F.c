/*E*/
/*Fichier : $Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2012/02/14 14:34:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_FctUtil_XZEA_XZAS * FICHIER acli_EA_AS_F.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Contient les fonctions utilisateurs de TACLI qui sont passees
* aux fonction de EQUEST (XZEA*) et qui permet d appeler les fonctions
* XZAS de mise en base des donnees de mesures 6mn, mesure Poids, 
* Niveau de services
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
* 	acli_XZEA07_XZAS11_FU est la fonction utilisateur appelee
*	par xzea07 . Elle permet a TACLI de mettre en base les mesures Poids.
****************************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	08 Nov 1994	: Creation
* C.T.		27 JAN 1995	: Modif nom fichier trop long (V 1.1)
* B.G.		02 aou 1995	: ajout trace si pb xzas10+acknoledge (1.2)
* B.G.          13 sep 1995	: ajout acknoledge voie (1.3)
* P.N.          03 Oct 1996	: ajout de la mise en base du pesage (eqt10) (1.4)
* P.N.          10 Oct 1996	: ajout de la mise en base des donnees meteo (dem/1232) (1.5)
* JPL		12/12/2011	: Changement du fichier d'en-tete ; suppression avertissements mineurs  1.8
* JMG           13/01/2012      : tdp etendu DEM/1014 1.9
------------------------------------------------------*/

/* fichiers inclus */

#include "xzer.h"

#include "xzas50.h"
#include "xzas51.h"
#include "xzas52.h"
#include "xzas53.h"

#include "xzad02.h"
#include "xzad03.h"

#include "acli_EA_AS_F.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA_XZAS_FU" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA03_XZAS10_FU est la fonction utilisateur appelee
*	par xzea03 . Elle permet a TACLI de mettre en base les mesures 6mn.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA03_XZAS10_FU (XZEAT_EMes_RADT va_Mes,int *pa_Resultat ) 
/*
* ARGUMENTS EN ENTREE :
*	va_Mes 		contient les arguments a faire passer a XZAS10.
*	va_Resultat 
*
* ARGUMENTS EN SORTIE :
*
*   a_Resultat va contenir le resultat d execution de XZAS10_Ajout_Mesure_6mn
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
*	a l open Client afin d effectuer la mise en base des mesures 6min
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA03_XZAS10_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA03_XZAS10_FU : debut d execution \n Numero:%d Sens:%d Voie:%d Horodate:%lf Q:%d valQ:%d V:%d ValV:%d TO:%d ValTO:%d PL:%d ValPL:%d.",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate,                
   				va_Mes.Mesures.ValQ.Val,      
   				va_Mes.Mesures.ValQ.Indic ,   
   				va_Mes.Mesures.ValV.Val,     
   				va_Mes.Mesures.ValV.Indic,  
   				va_Mes.Mesures.ValTO.Val,    
   				va_Mes.Mesures.ValTO.Indic,  
   				va_Mes.Mesures.ValPL.Val,    
   				va_Mes.Mesures.ValPL.Indic);
 	/*A
 	** Execution de l Open Client XZAS10 afin de mettre en base les mesures
 	** 6 mns.
 	*/
	if (va_Mes.Mesures.ValQ.Indic>1)
		va_Mes.Mesures.ValQ.Indic=0;
	if (va_Mes.Mesures.ValV.Indic>1)
		va_Mes.Mesures.ValV.Indic=0;
	if (va_Mes.Mesures.ValTO.Indic>1)
		va_Mes.Mesures.ValTO.Indic=0;
	if (va_Mes.Mesures.ValPL.Indic>1)
		va_Mes.Mesures.ValPL.Indic=0;

	if((vl_retcode = XZAS10_Ajout_Mesure_6mn(va_Mes.Numero ,                            
   				va_Mes.Sens ,                
   				va_Mes.Voie ,
   				va_Mes.Horodate,                 
   				va_Mes.Mesures.ValQ.Val,      
   				va_Mes.Mesures.ValQ.Indic ,   
   				va_Mes.Mesures.ValV.Val,     
   				va_Mes.Mesures.ValV.Indic,  
   				va_Mes.Mesures.ValTO.Val,    
   				va_Mes.Mesures.ValTO.Indic,  
   				va_Mes.Mesures.ValPL.Val,    
   				va_Mes.Mesures.ValPL.Indic))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA03_XZAS10_FU : XZAS10_Ajout_Mesure_6mn a retourne %d .",vl_retcode);
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA03_XZAS10_FU : les args sont: \n Numero:%d Sens:%d Voie:%d Horodate:%lf Q:%d ValQ:%d V:%d ValV:%d TO:%d ValTO:%d PL:%d ValPL:%d.",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate,                
   				va_Mes.Mesures.ValQ.Val,      
   				va_Mes.Mesures.ValQ.Indic ,   
   				va_Mes.Mesures.ValV.Val,     
   				va_Mes.Mesures.ValV.Indic,  
   				va_Mes.Mesures.ValTO.Val,    
   				va_Mes.Mesures.ValTO.Indic,  
   				va_Mes.Mesures.ValPL.Val,    
   				va_Mes.Mesures.ValPL.Indic);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS10
   	*/
   	*pa_Resultat = vl_retcode;
   	
   	/* envoie un acknoledge au lt */
 	XZER03_Acknoledge_6min_QVTO (va_Mes.Numero , va_Mes.Horodate);  	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA03_XZAS10_FU : Fin d execution ");
	return;
        
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA04_XZAS10_FU est la fonction utilisateur appelee
*	par xzea04 . Elle permet a TACLI de mettre en base les mesures 6mn.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA04_XZAS10_FU ( XZEAT_EMes_RADT va_Mes,int *pa_Resultat ) 
/*
* ARGUMENTS EN ENTREE :
*	va_Mes 		contient les arguments a faire passer a XZAS10.
*	va_Resultat 
*
* ARGUMENTS EN SORTIE :
*
*   pa_Resultat va contenir le resultat d execution de XZAS10_Ajout_Mesure_6mn
*
* CODE RETOUR :  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
* 	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des mesures 6min
*
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA04_XZAS10_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA04_XZAS10_FU : debut d execution \n Numero:%d Sens:%d Voie:%d Horodate:%lf Q:%d valQ:%d V:%d ValV:%d TO:%d ValTO:%d PL:%d ValPL:%d.",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate,                
   				va_Mes.Mesures.ValQ.Val,      
   				va_Mes.Mesures.ValQ.Indic ,   
   				va_Mes.Mesures.ValV.Val,     
   				va_Mes.Mesures.ValV.Indic,  
   				va_Mes.Mesures.ValTO.Val,    
   				va_Mes.Mesures.ValTO.Indic,  
   				va_Mes.Mesures.ValPL.Val,    
   				va_Mes.Mesures.ValPL.Indic);
 	/*A
 	** Execution de l Open Client XZAS10 afin de mettre en base les mesures
 	** 6 mns.
 	*/
	if (va_Mes.Mesures.ValQ.Indic>1)
		va_Mes.Mesures.ValQ.Indic=0;
	if (va_Mes.Mesures.ValV.Indic>1)
		va_Mes.Mesures.ValV.Indic=0;
	if (va_Mes.Mesures.ValTO.Indic>1)
		va_Mes.Mesures.ValTO.Indic=0;
	if (va_Mes.Mesures.ValPL.Indic>1)
		va_Mes.Mesures.ValPL.Indic=0;

	if((vl_retcode = XZAS10_Ajout_Mesure_6mn(va_Mes.Numero ,                            
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate,                
   				va_Mes.Mesures.ValQ.Val,      
   				va_Mes.Mesures.ValQ.Indic ,   
   				va_Mes.Mesures.ValV.Val,     
   				va_Mes.Mesures.ValV.Indic,  
   				va_Mes.Mesures.ValTO.Val,    
   				va_Mes.Mesures.ValTO.Indic,  
   				va_Mes.Mesures.ValPL.Val,    
   				va_Mes.Mesures.ValPL.Indic))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA04_XZAS10_FU : XZAS10_Ajout_Mesure_6mn a retourne %d .",vl_retcode);
   		XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA04_XZAS10_FU : debut d execution \n Numero:%d Sens:%d Voie:%d Horodate:%lf Q:%d V:%d TO:%d PL:%d.",             
   				va_Mes.Numero,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate,                
   				va_Mes.Mesures.ValQ.Val,      
   				va_Mes.Mesures.ValQ.Indic ,   
   				va_Mes.Mesures.ValV.Val,     
   				va_Mes.Mesures.ValV.Indic,  
   				va_Mes.Mesures.ValTO.Val,    
   				va_Mes.Mesures.ValTO.Indic,  
   				va_Mes.Mesures.ValPL.Val,    
   				va_Mes.Mesures.ValPL.Indic);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS10
   	*/
   	*pa_Resultat = vl_retcode;
   	
   	/* envoie un acknoledge au lt */
 	XZER03_Acknoledge_6min_QVTO (va_Mes.Numero , va_Mes.Horodate);  	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA04_XZAS10_FU : Fin d execution ");
	return;     
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA05_XZAS12_FU est la fonction utilisateur appelee
*	par xzea05 . Elle permet a TACLI de mettre en base les niveaux de services.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA05_XZAS12_FU ( XDY_ENiveau_Service va_Mes,int *pa_Resultat ) 
/*
* ARGUMENTS EN ENTREE :
*	va_Mes 		contient les arguments a faire passer a XZAS12_Ajout_Niveau_Service.
*	va_Resultat 
*
* ARGUMENTS EN SORTIE :
*
*   pa_Resultat va contenir le resultat d execution de XZAS12_Ajout_Niveau_Service
*
* CODE RETOUR :  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
* 	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des niveaux de services.
*
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA05_XZAS12_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA05_XZAS12_FU : debut d execution \n Horodate:%lf Numero:%d Sens:%d Niveau de service:%d",
					va_Mes.Horodate,  va_Mes.Numero, va_Mes.Sens, va_Mes.NivService);
 	/*A
 	** Execution de l Open Client XZAS12_Ajout_Niveau_Service afin de mettre en base les mesures
 	** 6 mns.
 	*/
	if((vl_retcode = XZAS12_Ajout_Niveau_Service(va_Mes.Numero, va_Mes.Sens,va_Mes.Horodate,va_Mes.NivService))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA05_XZAS12_FU : XZAS12_Ajout_Niveau_Service a retourne %d .",vl_retcode);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS12_Ajout_Niveau_Service
   	*/
   	*pa_Resultat = vl_retcode;
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA05_XZAS12_FU : Fin d execution ");
	return;
        
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA07_XZAS11_FU est la fonction utilisateur appelee
*	par xzea07 . Elle permet a TACLI de mettre en base les mesures Poids.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA07_XZAS11_FU ( XDY_EMes_Poids va_Mes,int *pa_Resultat ) 
/*
* ARGUMENTS EN ENTREE :
*	va_Mes 		contient les arguments a faire passer a XZAS11_Ajout_Mesure_Poids.
*	va_Resultat 
*
* ARGUMENTS EN SORTIE :
*
*   pa_Resultat va contenir le resultat d execution de XZAS11_Ajout_Mesure_Poids
*
* CODE RETOUR :  aucun
*
* CONDITION D'UTILISATION
*
*	Le rtServeur ainsi que le SQL serveur doivent tourner.
*
* FONCTION 
* 	Recoie une structure contenant les informations a fournir
*	a l open Client afin d effectuer la mise en base des niveaux de services.
*
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA07_XZAS11_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA07_XZAS11_FU : debut d execution \n Station:%d Sens:%d Horodate:%lf Cumul:%d NbPL:%d",
				va_Mes.Numero,
				va_Mes.Sens,
				va_Mes.Horodate,
				va_Mes.Cumul,
				va_Mes.NB_PL);
	/*A
 	** Execution de l Open Client XZAS11_Ajout_Mesure_Poids afin de mettre en base les mesures
 	** 6 mns.
 	*/
	if((vl_retcode = XZAS11_Ajout_Mesure_Poids(va_Mes.Numero,va_Mes.Sens,va_Mes.Horodate,va_Mes.Cumul,va_Mes.NB_PL))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA07_XZAS11_FU : XZAS11_Ajout_Mesure_Poids a retourne %d .",vl_retcode);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS11_Ajout_Mesure_Poids
   	*/
   	*pa_Resultat = vl_retcode;
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA07_XZAS11_FU : Fin d execution ");
	return;
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAS50_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les mesures de pesage KC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA_XZAS50_FU (	XZEAT_EMES_RADT_SILHOUETTE va_Mes, int *pa_ResultatKC) 
/*
* ARGUMENTS EN ENTREE :
*	va_Mes _KC		contient les arguments a faire passer a XZAS50,51,52.
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
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA_XZAS50_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA_XZAS50_FU : debut d execution \n Numero:%d Sens:%d Voie:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate);
 	/*A
 	** Execution de l Open Client XZAS50 afin de mettre en base les mesures
 	** KC.
 	*/
	if((vl_retcode = XZAS50_Ajout_Mesure_KC(va_Mes.Numero ,                            
   				va_Mes.Sens ,                
   				va_Mes.Voie ,
   				va_Mes.Horodate,                 
   				va_Mes.Mesures))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAS50_FU : XZAS50_Ajout_Mesure_6mn a retourne %d .",vl_retcode);
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAS50_FU : les args sont: \n Numero:%d Sens:%d Voie:%d Horodate:%lf.",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS50
   	*/
   	*pa_ResultatKC = vl_retcode;
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA_XZAS50_FU : Fin d execution ");
	return;
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAS51_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les mesures de pesage EC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA_XZAS51_FU ( XZEAT_EMES_RADT_ESSIEU va_Mes, int *pa_ResultatEC ) 
/*
* ARGUMENTS EN ENTREE :
*	va_Mes_EC 		contient les arguments a faire passer a XZAS51.
*
* ARGUMENTS EN SORTIE :
*
*   a_ResultatEC va contenir le resultat d execution de XZAS51_Ajout_Mesure_EC
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
*	a l open Client afin d effectuer la mise en base des mesures KC, EC ET PC
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA_XZAS51_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA_XZAS51_FU : debut d execution \n Numero:%d Sens:%d Voie:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate);
  	/*A
 	** Execution de l Open Client XZAS51 afin de mettre en base les mesures
 	** EC.
 	*/
	if((vl_retcode = XZAS51_Ajout_Mesure_EC(va_Mes.Numero ,                            
   				va_Mes.Sens ,                
   				va_Mes.Voie ,
   				va_Mes.Horodate,                 
   				va_Mes.Mesures))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAS51_FU : XZAS51_Ajout_Mesure_EC a retourne %d .",vl_retcode);
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAS51_FU : les args sont: \n Numero:%d Sens:%d Voie:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS51
   	*/
   	*pa_ResultatEC = vl_retcode;
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA_XZAS51_FU : Fin d execution ");
	return;
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAS52_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les mesures de pesage PC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA_XZAS52_FU ( XZEAT_EMES_RADT_POIDS va_Mes, int *pa_ResultatPC ) 
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
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA_XZAS52_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA_XZAS52_FU : debut d execution \n Numero:%d Sens:%d Voie:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate);
 	/*A
 	** Execution de l Open Client XZAS52 afin de mettre en base les mesures
 	** PC.
 	*/
	if((vl_retcode = XZAS52_Ajout_Mesure_PC(va_Mes.Numero ,                            
   				va_Mes.Sens ,                
   				va_Mes.Voie ,
   				va_Mes.Horodate,                 
   				va_Mes.Mesures))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAS52_FU : XZAS52_Ajout_Mesure_PC a retourne %d .",vl_retcode);
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAS52_FU : les args sont: \n Numero:%d Sens:%d Voie:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Sens ,                
   				va_Mes.Voie , 
   				va_Mes.Horodate);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS52
   	*/
   	*pa_ResultatPC = vl_retcode;
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA_XZAS52_FU : Fin d execution ");
	return;
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAS53_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les mesures meteo.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA_XZAS53_FU ( XDY_EMes_Meteo va_Mes, int *pa_Resultat ) 
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
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA_XZAS53_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA_XZAS53_FU : debut d execution \n Numero:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Jour);
 	/*A
 	** Execution de l Open Client XZAS53 afin de mettre en base les mesures
 	** PC.
 	*/
	if((vl_retcode = XZAS53_Ajout_Mesure_MET(va_Mes.Numero ,                            
   				va_Mes.Jour,                 
   				va_Mes.Dispo,                 
   				va_Mes.Code,                 
   				va_Mes.HeureDebut,                 
   				va_Mes.HeureFin))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAS53_FU : XZAS53_Ajout_Mesure_MET a retourne %d .",vl_retcode);
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAS53_FU : les args sont: \n Numero:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Jour);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS53
   	*/
   	*pa_Resultat = vl_retcode;
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA_XZAS53_FU : Fin d execution ");
	return;
        
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAD02_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les temps de parcours 6mn.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA_XZAD02_FU ( XDY_EMes_TdP_6mn va_Mes, XDY_EMes_TdP_6mn va_MesRADT, int *pa_Resultat ) 
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
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA_XZAD02_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_XZEA_XZAD02_FU : debut d execution \n Numero:%d Horodate:%lf TDP=%d RADT=%d.",
				va_Mes.Numero ,               
   				va_Mes.Horodate,
				va_Mes.Temps,
				va_MesRADT.Temps);
 	/*A
 	** Execution de l Open Client XZAD02 afin de mettre en base les mesures
 	** PC.
 	*/

	if((vl_retcode = XZAD02_Ajout_TDP_6mn(va_Mes.Numero ,                            
   				va_Mes.Horodate,                 
   				va_Mes.Temps,                 
   				va_Mes.Dispo,
				va_MesRADT.Temps,
				va_MesRADT.Dispo))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAD02_FU : XZAD02_Ajout_TDP_6mn a retourne %d .",vl_retcode);
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAD02_FU : les args sont: \n Numero:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Horodate);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS53
   	*/
   	*pa_Resultat = vl_retcode;
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA_XZAD02_FU : Fin d execution ");
	return;
        
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* 	acli_XZEA_XZAD02_FU est la fonction utilisateur appelee
*	par xzea . Elle permet a TACLI de mettre en base les temps de parcours 6mn.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  acli_XZEA_XZAD03_FU ( XDY_EMes_TdP_1h va_Mes, int *pa_Resultat ) 
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
{
	static char *version = "$Id: acli_EA_AS_F.c,v 1.9 2012/02/14 14:34:15 pc2dpdy Exp $ : acli_XZEA_XZAD03_FU" ;
	int 		vl_retcode = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : acli_XZEA_XZAD03_FU : debut d execution \n Numero:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Horodate);
 	/*A
 	** Execution de l Open Client XZAD03 afin de mettre en base les mesures
 	** PC.
 	*/
	if((vl_retcode = XZAD03_Ajout_TDP_1h(va_Mes.Numero ,                            
   				va_Mes.Horodate,                 
   				va_Mes.Temps,                 
   				va_Mes.Dispo,
   				va_Mes.NbApp))!=XDC_OK)
   	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAD03_FU : XZAD03_Ajout_TDP_1h a retourne %d .",vl_retcode);
   		XZST_03EcritureTrace(XZSTC_WARNING,"acli_XZEA_XZAD03_FU : les args sont: \n Numero:%d Horodate:%lf .",
				va_Mes.Numero ,               
   				va_Mes.Horodate);
   	}
   	/*A
   	** Retourne le resultat d execution de XZAS53
   	*/
   	*pa_Resultat = vl_retcode;
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_XZEA_XZAD03_FU : Fin d execution ");
	return;
        
}
