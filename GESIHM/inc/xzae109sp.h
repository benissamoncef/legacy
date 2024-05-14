/*E*/
/*Fichier : $Id: xzae109sp.h,v 1.1 1996/10/04 10:44:49 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/04 10:44:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae109sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* R≈cuperation du nom de la machine de l'operateur correspondant
* a la fmc op≈rateur donneer
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.  26/09/96        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE109_Machine_Operateur
*
* Arguments en entree
* int 		va_Numero_in
* smallint 	va_Cle_in
*
* Arguments en sortie
* T_NOM va_NomMachine_out

* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* 
-------------------------------------------------------*/

 
#define XZAE109_Machine_Operateur	"XZAE;109" 
