/*E*/
/*Fichier : $Id: xzah_utils.c,v 1.4 2013/01/25 10:12:25 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2013/01/25 10:12:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH_UTIL * FICHIER xzah_utils.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzah_utils.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	26 Oct 1994	: Creation
* torregrossa	27 Mar 1995	: Ajout XDC_SYN_TRAVAUX_MOBILES_TETE_... et
*                                 XDC_SYN_TRAVAUX_MOBILES_QUEUE_... (V1.4)
* JMG		12/12/12	ajout FMC regulation 1.5
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah_utils.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah_utils.c,v 1.4 2013/01/25 10:12:25 pc2dpdy Exp $ : xzah_utils" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*	XZAH_Conv_xzahcxzaec_xdc : Effectue la conversion des constantes de BASEDD 
* (contenus dans xzahc.h et xzaec.h vers les constantes de xdc.h)
* vers les constantes des autres sous systemes (XDC.h).
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZAH_util_Conv_xzahcxzaec_xdc(XDY_Octet	va_TypeObjet_in , XDY_Octet *pa_TypeObjet_out)
/*
*
* PARAMETRES EN ENTREE : va_TypeObjet_in contient la constante BASEDD
*
* PARAMETRES EN SORTIE : pa_TypeObjet_out va contenir la conversion sous forme XDC 
* CODE RETOUR :
*
* 0	XDC_OK si tout est ok.
* 1	XDC_NOK si on a pas trouve de concordance.
*
* CONDITIONS D'UTILISATION :
* MECANISMES : switch sur les valeur de xzahc.h et de xzaec.h 
* et renvoie la valeur des constantes XDC.h
*
------------------------------------------------------*/

{		
	static char *version = "$Id: xzah_utils.c,v 1.4 2013/01/25 10:12:25 pc2dpdy Exp $ : XZAH_util_Conv_xzahcxzaec_xdc " ;
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : XZAH_util_Conv_xzahcxzaec_xdc est appelee avec %d",va_TypeObjet_in);
	switch(va_TypeObjet_in)
	{
		case XZAHC_Travaux_Mob_P:
			*pa_TypeObjet_out = XDC_SYN_TRAVAUX_MOBILES_PREVUS;
			break;
		case XZAHC_Travaux_Mob_R:
			*pa_TypeObjet_out = XDC_SYN_TRAVAUX_MOBILES_REELS;
			break;
		case XZAHC_Travaux_Fix_P:
			*pa_TypeObjet_out = XDC_SYN_TRAVAUX_FIXES_PREVUS;
			break;
		case XZAHC_Travaux_Fix_R:
			*pa_TypeObjet_out = XDC_SYN_TRAVAUX_FIXES_REELS;
			break;
		case XZAHC_Travaux_Mob_Tete_R:
			*pa_TypeObjet_out = XDC_SYN_TRAVAUX_MOBILES_TETE_REELS;
			break;
		case XZAHC_Travaux_Mob_Queue_R:
			*pa_TypeObjet_out = XDC_SYN_TRAVAUX_MOBILES_QUEUE_REELS;
			break;
		case XZAHC_Travaux_Mob_Tete_P:
			*pa_TypeObjet_out = XDC_SYN_TRAVAUX_MOBILES_TETE_PREVUS;
			break;
		case XZAHC_Travaux_Mob_Queue_P:
			*pa_TypeObjet_out = XDC_SYN_TRAVAUX_MOBILES_QUEUE_PREVUS;
			break;
		case XZAHC_Gendarme:
			*pa_TypeObjet_out = XDC_SYN_INTERVENANT_GENDARMERIE;
			break;
		case XZAHC_Pompier:
			*pa_TypeObjet_out = XDC_SYN_INTERVENANT_POMPIER;
			break;
		case XZAHC_Depanneur:
			*pa_TypeObjet_out = XDC_SYN_INTERVENANT_DEPANNEUR;
			break;
		case XZAHC_PMV_Fourgon:
			*pa_TypeObjet_out = XDC_SYN_INTERVENANT_FOURGON_SIS;
			break;
		case XZAHC_Patrouille:
			*pa_TypeObjet_out = XDC_SYN_INTERVENANT_PAT_ESCOTA;
			break;
		case XZAHC_Gare:
			*pa_TypeObjet_out = XDC_SYN_GARE;
			break;
		case XZAHC_PMV_Mobile:
			*pa_TypeObjet_out = XDC_SYN_PMV_MOBILE;
			break;
		case XZAEC_FMC_Accident:
			*pa_TypeObjet_out = XDC_SYN_FMC_ACCIDENT;
			break;
		case XZAEC_FMC_Contresens:
			*pa_TypeObjet_out = XDC_SYN_FMC_VEHICULE_CONTRE_SENS;
			break;	
			
		case XZAEC_FMC_VehArrete:
			*pa_TypeObjet_out = XDC_SYN_FMC_VEHICULE_ARRETE;
			break;	
		case XZAEC_FMC_VehPanne:
			*pa_TypeObjet_out = XDC_SYN_FMC_VEHICULE_PANNE;
			break;	
		case XZAEC_FMC_VehFeu:
			*pa_TypeObjet_out = XDC_SYN_FMC_VEHICULE_FEU;
			break;	
		case XZAEC_FMC_Cirque:
			*pa_TypeObjet_out = XDC_SYN_FMC_CIRQUE;
			break;	
		case XZAEC_FMC_ConvoiExc:
			*pa_TypeObjet_out = XDC_SYN_FMC_CONVOI_EXCEPTIONNEL;
			break;	
		case XZAEC_FMC_ConvoiMil:
			*pa_TypeObjet_out = XDC_SYN_FMC_CONVOI_MILITAIRE;
			break;	
		case XZAEC_FMC_VehLent:
			*pa_TypeObjet_out = XDC_SYN_FMC_VEHICULE_LENT;
			break;	
		case XZAEC_FMC_Manifestation:
			*pa_TypeObjet_out = XDC_SYN_FMC_MANIFESTATION;
			break;	
		case XZAEC_FMC_GreveESCOTA:
			*pa_TypeObjet_out = XDC_SYN_FMC_GREVE_ESCOTA;
			break;	
		case XZAEC_FMC_Escargot:
			*pa_TypeObjet_out = XDC_SYN_FMC_OPERATION_ESCARGOT;
			break;	
		case XZAEC_FMC_IncSsConces:
			*pa_TypeObjet_out = XDC_SYN_FMC_INCIDENT_SOUS_CONSC ;
			break;	
		case XZAEC_FMC_Eboulement:
			*pa_TypeObjet_out = XDC_SYN_FMC_EBOULEMENT;
			break;	
		case XZAEC_FMC_Incendie:
			*pa_TypeObjet_out = XDC_SYN_FMC_INCENDIE;
			break;	
		case XZAEC_FMC_Inondation:
			*pa_TypeObjet_out = XDC_SYN_FMC_INONDATION;
			break;	
		case XZAEC_FMC_Obstacle:
			*pa_TypeObjet_out = XDC_SYN_FMC_OBSTACLE;
			break;	
		case XZAEC_FMC_ChausseDeter:
			*pa_TypeObjet_out = XDC_SYN_FMC_CHAUSSEE_DETERIOREE;
			break;	
		case XZAEC_FMC_FlaqueHuile:
			*pa_TypeObjet_out = XDC_SYN_FMC_FLAQUE_HUILE;
			break;	
		case XZAEC_FMC_AnimalErrant:
			*pa_TypeObjet_out = XDC_SYN_FMC_ANIMAL_ERRANT;
			break;	
		case XZAEC_FMC_FrontiereFermee:
			*pa_TypeObjet_out = XDC_SYN_FMC_FRONTIERE_FERMEE;
			break;	
		case XZAEC_FMC_AltercationUsager:
			*pa_TypeObjet_out = XDC_SYN_FMC_ALTERCATION_USAGERS;
			break;	
		case XZAEC_FMC_Agression:
			*pa_TypeObjet_out = XDC_SYN_FMC_AGRESSION;
			break;	
		case XZAEC_FMC_MalaiseUsager:
			*pa_TypeObjet_out = XDC_SYN_FMC_MALAISE_USAGER;
			break;	
		case XZAEC_FMC_HoldUpVol:
			*pa_TypeObjet_out = XDC_SYN_FMC_HOLD_UP;
			break;	
		case XZAEC_FMC_PietonChaussee:
			*pa_TypeObjet_out = XDC_SYN_FMC_PIETON_CHAUSSEE;
			break;	
		case XZAEC_FMC_Autre:
			*pa_TypeObjet_out = XDC_SYN_FMC_AUTRE;
			break;		
		case XZAEC_FMC_Regulation:
			 *pa_TypeObjet_out = XDC_SYN_FMC_REGULATION_VITESSE;
			 break;
		default : 
			XZST_03EcritureTrace(XZSTC_WARNING,"XZAH_util_Conv_xzahcxzaec_xdc a recu un Type Objet inconnu a convertir !!!!!!!"); 
			*pa_TypeObjet_out = -1; /* Erreur */
			return(XDC_NOK);
			break;
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAH_util_Conv_xzahcxzaec_xdc retourne la valeur %d",*pa_TypeObjet_out);
	return(XDC_OK);
}

