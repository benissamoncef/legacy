/*E*/
/*  Fichier : $Id: xzas06sp.h,v 1.1 1994/10/27 15:10:27 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/27 15:10:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	27/06/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare une courbe f(t) pour les mesures trafic 6 minutes
* sur une periode de 6 heures
* 
* Sequence d'appel
* SP	XZAS06_Ajout_Niveau_Service
* 
* Arguments en entree
* XDY_Mot		va_Station_in		: numero de station
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Voie_in
* XDY_Datetime		va_HorodateDebut_in	: date de debut des mesures
* XDY_Octet		va_TypeDeDonnees_in	: type de courbe
* 
* Arguments en sortie
* XDY_Datetime		va_HorodateDebut_out	: date de debut de la courbe
*
* XDY_Mot		va_Valeur_out		: valeur de la mesure	-- donnees retournees
* XDY_Octet		va_Validite_out		: validite de la mesure	-- dans une liste
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  station inexistante
* XZASC_RADT_PAS_MES	: pas de mesures
* 
* Conditions d'utilisation
* Type de courbe (va_TypeDeDonnees_in) retourne :
* - XZASC_RADT_V	: la vitesse en fonction du temps
* - XZASC_RADT_Q	: la quantite de vehicules en fonction du temps
* - XZASC_RADT_PL	: la quantite de poids lourds en fonction du temps
* - XZASC_RADT_TO	: le taux d'occupation en fonction du temps
* La date de depart des mesures est egale a la date donnee ou
* superieure de 5mm 59s maximum ou egale la derniere date inseree moins 6 heures
* (si la date donnee est superieure a la derniere date inseree moins 6 heures)
* La valeur XDC_RADT_MES_INCONNU et la validite XDC_RADT_INCONNU sont
* retournees dans la liste des mesures si pas d'enregistrement a une date
* Les mesures au nombre de 60 sont retournees dans une liste
*
* Fonction
* selectionner dans la table MESURE_TRAFIC_6_MINUTES
-------------------------------------------------------*/

 
#define XZAS06_Ajout_Niveau_Service	"XZAS;06" 
