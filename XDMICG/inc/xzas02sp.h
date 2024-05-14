/*E*/
/*  Fichier : $Id: xzas02sp.h,v 1.2 1996/10/04 10:17:35 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/10/04 10:17:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas02sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/11/94	: Creation	(V 1.1)
* P.N. 23/09/96		: Ajout de la plage (v1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare un graphe trafic INRETS 
* 
* Sequence d'appel
* SP	XZAS02_Graphe_Trafic
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PRDebut_in		
* XDY_PR	va_PRFin_in		
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures multiple de 6mn
* XDY_Octet	va_Plage_in	: duree de la plage de donnees

* Arguments en sortie
*
* Liste 1 retournee :
* XDY_Entier	numero	: reference de la zone d'influence
* XDY_PR	PRDebut	: PR de debut de la zone
* XDY_PR	PRFin	: PR de fin de la zone
* 
* Liste 2 retournee :
* XDY_Entier	numero	: reference de la zone d'influence
* XDY_Horodate	Horodate: date de la mesure
* XDY_Octet	Niveau	: niveau de trafic
* 
* Code retour
* XDC_OK
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  date non multiple de 6 mn
* 
* Conditions d'utilisation
* Les mesures retournees sont datees a partir de la date de debut donnee
* et pendant 6 heures
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
* selectionner dans la table MESURE_NIVEAU
-------------------------------------------------------*/

 
#define XZAS02_Graphe_Trafic	"XZAS;02" 
