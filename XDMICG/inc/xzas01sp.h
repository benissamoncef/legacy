/*E*/
/*  Fichier : $Id: xzas01sp.h,v 1.1 1994/11/25 10:42:08 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/25 10:42:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas01sp.prc
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
* Prepare un nuage de points Q, V, TO, Horodate 
* 
* Sequence d'appel
* SP	XZAS01_Points_QVTO
* 
* Arguments en entree
* XDY_Mot	va_NumStation_in	: numero de station
* XDY_Sens	va_Sens_in		: XDC_SENS_NORD ou XDC_SENS_SUD
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures
* XDY_Datetime	va_HorodateFin_in	: date de fin des mesures
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Horodate	horodate 	: date de la mesure
* XDY_Octet	V 		: vitesse
* XDY_Octet	validite_V 	: validite a correcte ou incorrecte ou inconnue
* XDY_Octet	T_O 		: taux d'occupation
* XDY_Octet	validite_TO 	: validite a correcte ou incorrecte ou inconnue
* XDY_Octet	Q 		: quantite de vehicules
* XDY_Octet	validite_Q	: validite a correcte ou incorrecte ou inconnue
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  station inexistante
* XZASC_RADT_PAS_MES	: pas de mesure
* 
* Conditions d'utilisation
* La date de depart des mesures est egale a la date donnee ou
* superieure de 5mm 59s maximum
* La valeur XDC_RADT_MES_INCONNU et la validite XDC_RADT_INCONNU sont
* retournees dans la liste des mesures si pas d'enregistrement a une date
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

 
#define XZAS01_Points_QVTO	"XZAS;01" 
