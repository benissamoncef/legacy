/*E*/
/*  Fichier : $Id: xzae41sp.h,v 1.2 2011/11/10 16:43:07 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/11/10 16:43:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae41sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/94	: Creation
* C.T.	12/08/94	: Modif table FMC_VEH et position
*                         param entree
* C.T.	04/10/94	: Ajout en parametre de retour dans la
*                         liste site_depannage	(V 1.5)
* JPL	10/11/11 : Procedure degroupee; ajout champs conditions accident  (DEM 1006)  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'un complement de fiche main courante 
* de type accident 
* 
* Sequence d'appel
* SP	XZAE41_Lire_Fiche_Accident
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet		va_VL_out
* XDY_Octet		va_PL_out
* XDY_Octet		va_Remorque_out
* XDY_Octet		va_Car_out
* XDY_Octet		va_Moto_out
* XDY_Octet		va_BlesseLeg_out
* XDY_Octet		va_BlesseGra_out
* XDY_Octet		va_Mort_out
* XDY_Booleen		va_VehEscota_out
* XDY_Booleen		va_MatDang_out
* XDY_Booleen		va_HomEscota_out
* XDY_Booleen		va_Personalite_out
* XDY_Booleen		va_DegatsDom_out	Pour compatibilite seulement
* XDY_Mot		va_Cond_Meteo_out
* XDY_Mot		va_Cond_Visibilite_out
* XDY_Mot		va_Cond_Etat_Chau_out
* XDY_Mot		va_Cond_Chantier_out
* *** XDY_Mot		va_Cond_Obstacle_out	ANNULE
* *** XDY_Mot		va_Cond_Animal_out	ANNULE
*
* XDY_Octet		va_Type_out	-- informations par vehicule implique 
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
* XDY_Octet		va_NumeroVehicule_out
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* Fonction
* Select dans les tables ACCIDENT et VEHICULE
* where numero numero and horodate
------------------------------------------------------*/

 
#define XZAE41_Lire_Fiche_Accident	"XZAE41"
