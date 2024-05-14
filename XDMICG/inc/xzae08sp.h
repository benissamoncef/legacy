/*E*/
/*Fichier :  $Id: xzae08sp.h,v 1.3 2008/02/13 13:24:24 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2008/02/13 13:24:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/09/94	: Creation	(V 1.1)
* C.T.	24/10/94	: Modif codage	(V 1.2)
*                         au niveau des valeurs possibles
*                         pour la machine operateur
*                         (cf conditions d'utilisation)
*                         pas reteste
* JPL	11/06/07	: Renommage (sans ';') version sccs 1.2
* JBL	Nov 2007	: DEM 662
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Cree une fiche main courante
* 
* Sequence d'appel
* SP	XZAE08_Creer_Fiche_MC
* 
* Arguments en entree
* XDY_Octet	va_TypeEvenement_in
* XDY_Machine	va_MachineOperateur_in
* XDY_Booleen	va_Prevision_ATraiter_in
* 
* Arguments en sortie
* XDY_Entier	va_NumEvenement_out
* XDY_Octet	va_CleEvenement_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: site local incorrect, insertion impossible, 
*                 type inconnu
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'evenement est a traiter, la machine operateur doit
* etre valuee 
* Si l'evenement n'est a traiter, la machine operateur doit
* etre a null (etat en prevision) 
* 
* Fonction
* Inserer dans la table EVENEMENT en indiquant le type 
* d'evenement et le site deduit local.
* Inserer dans la table de complement d'evenement en adequation 
* avec le type d'evenement.
* La fonction retourne le numero et la cle de l'evenement.
* Si l'evenement est a traiter, Insert dans la table a traiter
------------------------------------------------------------*/

 
#define XZAE08_Creer_Fiche_MC	"XZAE08" 
