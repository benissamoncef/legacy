/*E*/
/*  Fichier : $Id: xzae15sp.h,v 1.2 2009/03/03 11:09:34 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/03/03 11:09:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.     	24/10/94	: Creation 	V 1.1
* JPL		14/01/2009	: Degroupage de la procedure 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Confie le droit d'enrichir au site ou au poste operateur.
* 
* Sequence d'appel
* XZAE15_Transmettre_Droit_Enrichir
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Octet	va_Site_in
* XDY_Machine	va_PosteOperateur_in
* 
* Arguments en sortie
* XDY_Entier	va_Resultat_out
* 
* Code retour
* XDC_OK
* XDC_NOK		: modif du droit impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee XZAR;03 ou XZAE;18 n'existe pas en base
* < 0                   : erreur sybase
* 
* Conditions d'utilisation
* Soit le poste operateur, soit le site doit etre value a non null
* Pas l'autorisation de transmettre un evenement de type operateur
* Valeur prise par va_Resultat_out :
* - XZAEC_FMC_PAS_ENR	: pas le droit de transmettre l'enrichissement 
*                         (evenement pas sur le site local ou
*                          de type operateur)
* - XZAEC_FMC_OPR_INC	: operateur n'est pas en activite
* - XDC_OK		: le droit d'enrichir a ete transmis
* 
* Fonction
* Pour le transfert du droit d'enrichir a un site : 
* 	Update du champ site dans la table EVENEMENT, 
* 	EVENEMENT_HISTORIQUE et dans 
* 	la table de complement d'evenement (ou insert 
* 	distant puis delete local)
* Pour le tranfert du droit d'enrichir a un poste operateur du même site :
* 	Update du champ poste_enrichisseur dans la table EVENEMENT
---------------------------------------------------------------*/

 
#define XZAE15_Transmettre_Droit_Enrichir	"XZAE15" 
