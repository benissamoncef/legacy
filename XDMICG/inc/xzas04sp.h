/*E*/
/*  Fichier : $Id: xzas04sp.h,v 1.1 1994/11/25 10:42:43 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/25 10:42:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare un graphe trafic 3D pour Q, V, TO 
* 
* Sequence d'appel
* SP	XZAS04_Graphe_3D
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PRDebut_in		: pr de debut
* XDY_PR	va_PRFin_in		: pr de fin
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures
* 
* Arguments en sortie
* 
* Liste 1 retournee :
* XDY_Entier	numero	: reference de la zone d'influence
* XDY_PR	PRDebut	: PR de debut de la zone
* XDY_PR	PRFin	: PR de fin de la zone
* 
* Liste 2 retournee :
* XDY_Entier	numero		: reference de la zone d'influence
* XDY_Horodate	Horodate	: date de la mesure
* XDY_Mot	Q		: mesure de la quantite de vehicules
* XDY_Booleen	ValiditeQ	: validite de Q
* XDY_Octet	V		: mesure de la vitesse
* XDY_Booleen	ValiditeV	: validite de V
* XDY_Octet	TO		: mesure du taux d'occupation
* XDY_Booleen	ValiditeTO	: validite de TO
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  date non multiple de 6 mn
* 
* Conditions d'utilisation
* La date de depart des mesures doit etre un multiple de 6 minutes
* Les mesures retournees sont celles de la voie XDC_RADT_CHAUSSEE
* pendant 6 heures a partir de la date de debut donnee
* Retourne la liste des PR debut, PR fin des zones d'influence
* dans l'ordre des PR de debut (croissant si sens sud sinon decroissant)
* puis la liste des mesures avec la date dans l'ordre des PR de debut
* (croissant si sens sud sinon decroissant) et des dates croissantes
*
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 jour)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
*
* Fonction
* selectionner dans la table MESURE_TRAFIC_6_MINUTES
-------------------------------------------------------*/

 
#define XZAS04_Graphe_3D	"XZAS;04" 
