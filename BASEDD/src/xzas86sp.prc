/*E*/
/*  Fichier : $Id: xzas86sp.prc,v 1.1 2008/11/17 11:26:38 pc2dpdy Exp $      Release : $Revision: 1.1 $      Date : $Date: 2008/11/17 11:26:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas86sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module aweb : Retourne la liste des niveaux de trafic
* Remarque :
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/09/08	creation DEM/836
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------

* Sequence d"appel
* SP    XZAS86
*
* Arguments en entree
*
* Arguments en sortie
*
* aucun :
*
* Service rendu
*
* Code retour
* XDC_OK
* XDC_NOK       :
* XDC_ARG_INV   : parametres d"entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d"utilisation
*
*
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS86' and type = 'P')

    drop procedure XZAS86

go

create procedure XZAS86 
as

select station,sens,niveau
      from EXP..MES_NIV_DER
return (XDC_OK)
go

