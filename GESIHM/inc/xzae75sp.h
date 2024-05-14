/*E*/
/*  Fichier : $Id: xzae75sp.h,v 1.1 1998/03/06 11:06:25 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/03/06 11:06:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae75sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	26/02/98	: Creation 	V 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier la FMC initiale au CI pour une FMC de type degrade
* 
* Sequence d'appel
* XZAE75_Modif_FMC_Initiale
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in		: queue de bouchon
* XDY_Octet	va_CleEvt_in
* XDY_Machine	va_PosteOperateur_in	: numero de machine
* XDY_Entier	va_NumFMCInit_in		: queue de bouchon
* XDY_Octet	va_CleFMCInit_in
* 
* Arguments en sortie
* XDY_Entier	va_Resultat_out : XDC_OK si achevenement de l'evt est ok
* 
* Code retour
* XDC_OK
* XDC_NOK		: evenement inexistant, suppression de la liste des evts
*                         a traiter impossible, site local inconnu,
*                         positionner la date de cloture impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee n'existe pas en base
* 
* Conditions d'utilisation
* au CI uniquement.
* va_Resultat_out vaut :
*   XDC_OK
*   XZAEC_FMC_INIT_INC	: FMC initiale inexistante
*   XZAEC_FMC_INIT_DEG	: toutes les actions relatives a l'evenement ne sont 
*   	      		    terminees
*   XZAEC_FMC_INIT_INCOMP: FMC initiale de type incomptable
*   XZAEC_FMC_INIT_PAS_ENR  : pas le droit d'enrichir la FMC initiale
* 
* Fonction
-------------------------------------------------------------*/

 
#define XZAE75_Modif_FMC_Initiale	"XZAE75" 
