/*E*/
/*  Fichier : $Id: xzas87sp.prc,v 1.1 2008/11/17 11:26:39 pc2dpdy Exp $      Release : $Revision: 1.1 $      Date : $Date: 2008/11/17 11:26:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas87sp.prc
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
* SP    XZAS87
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

if exists (select * from sysobjects where name = 'XZAS87' and type = 'P')

    drop procedure XZAS87

go

create procedure XZAS87 
	@va_station_in smallint,
	@va_sens_in	tinyint,
	@va_niveau_out	tinyint output
as

select @va_niveau_out=0

select @va_niveau_out=niveau
      from EXP..MES_NIV_DER
      where station = @va_station_in 
      and sens = @va_sens_in

return (XDC_OK)
go

