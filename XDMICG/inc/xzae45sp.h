/*E*/
/*  Fichier : $Id: xzae45sp.h,v 1.1 1994/11/03 14:53:21 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae45sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	31/08/94	: Creation
* C.T.	12/08/94	: Modif table FMC_VEH (numero_vehicule 
*                         remplace evenement) et 
*                         position param entree
* C.T.	04/10/94	: Ajout table FMC_VEH (site_depannage)
*                         at dans l'interface
*                         remplace va_Enleve_out par 
*                         va_Depannage_out	(V 1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante vehicule
* non implique dans un accident
------------------------------------------------------- 
* Sequence d'appel
* SP	XZAE45_Lire_Fiche_Vehicule
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet		va_Type_out
* XDY_Octet		va_Passagers_out
* XDY_Booleen		va_Bebe_out
* XDY_Booleen		va_PersAgee_out
* XDY_Booleen		va_Animaux_out
* XDY_Booleen		va_VehEscota_out
* XDY_Octet		va_Chargement_out
* char(6)		va_CodeMat_out
* XDY_Entier		va_Depannage_out
* XDY_Site		va_SiteDepannage_out
* char(100)		va_Texte_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* Fonction
* Select dans la table VEHICULE
* where numero evenement and horodate
-------------------------------------------------------*/

 
#define XZAE45_Lire_Fiche_Vehicule	"XZAE;45" 
