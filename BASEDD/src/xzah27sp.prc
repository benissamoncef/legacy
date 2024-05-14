/*E*/
/*  Fichier : $Id: xzah27sp.prc,v 1.1 1995/01/17 14:04:07 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 14:04:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah27sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture des fmc pour satir
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	16/01/95	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAH27_ListeSatir
* 
* Arguments en entree
* 
* Arguments en sortie (tous dans un select)
* XDY_Entier    va_NumFmc_out
* XDY_Octet     va_CleFmc_out
* XDY_Horodate  va_Horodate_out
*
* Service rendu
* Envoie » satir les fmc modifiees
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
---------------------------------------------------*/

use PRC
go

create procedure  XZAH;27
as
	begin tran

/*A		on retourne uniquement les lignes qui n'ont pas ≈t≈ lues */
	
		select	evenement,
			cle,
			horodate
		from EXP..FMC_SAT holdlock
		where modif<>XDC_LU
	
/*A 		on efface les lignes marqu≈es "SUPPRESSION" */

		delete EXP..FMC_SAT
		where modif=XDC_SUPPR	

/*A			et on marque "LU" les lignes envoy≈es */

		update EXP..FMC_SAT
		set modif=XDC_LU
	commit tran
	return XDC_OK
go
