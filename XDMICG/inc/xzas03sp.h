/*E*/
/*  Fichier : $Id: xzas03sp.h,v 1.1 1994/11/25 10:42:32 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/25 10:42:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	18/11/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare un graphe bouchon
* 
* Sequence d'appel
* SP	XZAS03_Graphe_Bouchon
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PRDebut_in		
* XDY_PR	va_PRFin_in		
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures multiple de 6mn
* 
* Arguments en sortie
*
* Liste retournee :
* XDY_Horodate	Horodate	: date de validation
* XDY_PR	PRDebut		: PR de debut de bouchon
* XDY_PR	PRFin		: PR de fin de bouchon
* 
* Code retour
* XDC_OK
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee 
* 
* Conditions d'utilisation
* Retourne la liste des PR de debut, PR de fin de bouchon
* avec la date de validation associee
* Ecretage des PR de debut et de fin aux valeurs donnees en entree
* Attention aucun controle n'est effectue pour verifier que le PR
* de debut et de fin de bouchon correspondent au sens de l'autoroute
* au niveau des fiches evenements : pour le graphe c'est identique
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
* selectionner dans la table FMC_GEN, FMC_HIS, FMC_BOU
-------------------------------------------------------*/

 
#define XZAS03_Graphe_Bouchon	"XZAS;03" 
