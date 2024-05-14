/*E*/
/*  Fichier : $Id: xzag01sp.h,v 1.1 1994/11/03 11:03:34 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 11:03:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	02/11/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transfere les donnees du mois dans la base de sauvegarde
* /restauration
* 
* Sequence d'appel
* SP	XZAG01_Transfert_Mois
* 
* Arguments en entree
* XDY_Mot	va_Mois_in
* XDY_Mot	va_Annee_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV	: parametre d'entree null ou date demandee trop proche
* XDC_PRC_INC	: procedure XZAG;11 ou XZAE;70 pas en base
* 
* Conditions d'utilisation
* - Il faut que la date courante soit au moins superieure a
* au mois specifie plus 7 jours sinon erreur de parametre
* - Le mois vaut de 1 a 12
* - L'annee vaut au moins 1900
* - La periode de sauvegarde est du debut de mois - 7 jours a la
* la fin du mois + 7 jours
* 
* Fonction
* Copier de la base historique dans la base sauvegarde/restauration
* les evenements clos durant le mois specifie et les actions relatives
* a ces evenements, les etats des equipements et les mesures de trafic.
-------------------------------------------------------*/

 
#define XZAG01_Transfert_Mois	"XZAG;01" 
